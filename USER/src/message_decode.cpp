#include "message_decode.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "core_debug.h"
#include "mcu_define.h"

static uint32_t
calculate_crc(const char* msg, unsigned int len) {
    if (msg == nullptr || len == 0) {
        return 0;
    }
    unsigned long crc = 0xFFFFFFFF;
    for (unsigned int n = 0; n < len; n++) {
        crc = semp_crc32Table[(crc ^ msg[n]) & 0xff] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

static uint16_t
read_u16_le(const uint8_t* data) {
    return static_cast<uint16_t>(data[0] | (static_cast<uint16_t>(data[1]) << 8));
}

static uint16_t
request_message_id(const SEMP_PARSE_STATE* parse) {
    return read_u16_le(&parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L]);
}

static uint16_t
request_payload_length(const SEMP_PARSE_STATE* parse) {
    return read_u16_le(&parse->buffer[NM_PROTOCOL_MSG_LEN_INDEX_L]);
}

static void
write_u16_le(uint8_t* data, const uint16_t value) {
    data[0] = static_cast<uint8_t>(value & 0x00FF);
    data[1] = static_cast<uint8_t>((value >> 8) & 0x00FF);
}

static void
write_crc(uint8_t* msg, const uint16_t crcIndex) {
    const uint32_t crc = calculate_crc(reinterpret_cast<const char*>(msg), crcIndex);
    msg[crcIndex + 0] = static_cast<uint8_t>(crc & 0x000000FF);
    msg[crcIndex + 1] = static_cast<uint8_t>((crc >> 8) & 0x000000FF);
    msg[crcIndex + 2] = static_cast<uint8_t>((crc >> 16) & 0x000000FF);
    msg[crcIndex + 3] = static_cast<uint8_t>((crc >> 24) & 0x000000FF);
}

static void
write_success_payload(uint8_t* msg) {
    msg[NM_PROTOCOL_HEADER_LEN + 0] = 1;
    msg[NM_PROTOCOL_HEADER_LEN + 1] = 0;
}

typedef void (*PacketHandler)(const SEMP_PARSE_STATE* parse, uint8_t* msg);

typedef struct {
    uint16_t messageId;
    uint16_t minRequestPayloadLength;
    uint16_t responsePayloadLength;
    uint8_t responseType;
    PacketHandler handler;
} PacketDescriptor;

static void
fill_info1_response(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    (void)parse;
    systemInfo.messageDecode.Info1_count++;
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 0], systemInfo.hardware_version, strlen(systemInfo.hardware_version));
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 8], systemInfo.software_version, strlen(systemInfo.software_version));
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 18], &systemInfo.powerMonitor.batteryInfo.Percent, 2);
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 20], &systemInfo.powerMonitor.batteryInfo.Temp, 2);
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 22], &systemInfo.powerMonitor.batteryInfo.Voltage, 2);
}

static void
fill_info2_response(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    (void)parse;
    systemInfo.messageDecode.Info2_count++;
    msg[NM_PROTOCOL_HEADER_LEN + 0] = systemInfo.powerMonitor.reset_flag;
    msg[NM_PROTOCOL_HEADER_LEN + 1] = systemInfo.powerMonitor.poweroff_flag;
    msg[NM_PROTOCOL_HEADER_LEN + 2] = systemInfo.recordInfo.record_status;
    msg[NM_PROTOCOL_HEADER_LEN + 3] = systemInfo.recordInfo.record_op;
    msg[NM_PROTOCOL_HEADER_LEN + 4] = systemInfo.powerMonitor.batteryInfo.chargeStatus != notCharge;
    if (systemInfo.powerMonitor.poweroff_flag == 1) {
        systemInfo.powerMonitor.poweroff_flag = 0;
        systemInfo.powerMonitor.ShutdownEnsure = true;
        CORE_DEBUG_PRINTF("Shutdown Sync");
    }
}

static void
fill_info3_response(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    (void)parse;
    systemInfo.messageDecode.Info3_count++;
    msg[NM_PROTOCOL_HEADER_LEN + 0] = systemInfo.work_mode;
    msg[NM_PROTOCOL_HEADER_LEN + 1] = systemInfo.positionInfo.satellite_number_used;
    msg[NM_PROTOCOL_HEADER_LEN + 2] = systemInfo.positionInfo.coordinate_status;
    msg[NM_PROTOCOL_HEADER_LEN + 3] = systemInfo.ntripInfo.gprs_status;
    msg[NM_PROTOCOL_HEADER_LEN + 4] = systemInfo.ntripInfo.NtripServer_status;
    msg[NM_PROTOCOL_HEADER_LEN + 5] = systemInfo.ntripInfo.NtripClient_status;
    msg[NM_PROTOCOL_HEADER_LEN + 6] = systemInfo.radioInfo.radio_status;
    msg[NM_PROTOCOL_HEADER_LEN + 11] = systemInfo.radioInfo.radio_change_flag;
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 12], &systemInfo.ntripInfo.NtripServer_IP, 4);
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 16], &systemInfo.ntripInfo.NtripServer_Mountpoint, 32);
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 48], &systemInfo.ntripInfo.NtripClient_IP, 4);
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 52], &systemInfo.ntripInfo.NtripClient_Mountpoint, 32);
    msg[NM_PROTOCOL_HEADER_LEN + 84] = systemInfo.radioInfo.radio_mode;
    msg[NM_PROTOCOL_HEADER_LEN + 85] = systemInfo.radioInfo.radio_protocol;
    msg[NM_PROTOCOL_HEADER_LEN + 86] = systemInfo.radioInfo.radio_channel;

    systemInfo.radioInfo.radio_change_flag = 0;
}

static void
fill_info4_response(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    (void)parse;
    systemInfo.messageDecode.Info4_count++;
    msg[NM_PROTOCOL_HEADER_LEN + 0] = systemInfo.recordInfo.record_status;
    msg[NM_PROTOCOL_HEADER_LEN + 1] = systemInfo.recordInfo.record_op;
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 4], &systemInfo.recordInfo.record_leftspace, 4);
    memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 8], &systemInfo.recordInfo.record_name, 16);
    msg[NM_PROTOCOL_HEADER_LEN + 24] = systemInfo.recordInfo.record_type;
    msg[NM_PROTOCOL_HEADER_LEN + 25] = systemInfo.recordInfo.record_interval;
    msg[NM_PROTOCOL_HEADER_LEN + 26] = systemInfo.recordInfo.record_change_flag;
    if (systemInfo.recordInfo.record_change_flag) {
        systemInfo.recordInfo.record_change_flag = 0;
    }
    if (systemInfo.recordInfo.record_op) {
        systemInfo.recordInfo.record_op = 0;
    }
}

static void
fill_wifi_info_response(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    (void)parse;
    systemInfo.messageDecode.InfoWifi_count++;
    msg[NM_PROTOCOL_HEADER_LEN + 0] = systemInfo.wifiInfo.wifi_on_off_set;
    msg[NM_PROTOCOL_HEADER_LEN + 3] = systemInfo.wifiInfo.wifi_change_flag;
    systemInfo.wifiInfo.wifi_change_flag = 0;
}

static void
handle_reset_request(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    systemInfo.powerMonitor.reset_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN];
    write_success_payload(msg);
}

static void
handle_set1_request(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    systemInfo.messageDecode.Set1_count++;
    if (systemInfo.radioInfo.radio_change_flag == 0) {
        systemInfo.work_mode = static_cast<WorkMode_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 0]);
        systemInfo.positionInfo.satellite_number_used = parse->buffer[NM_PROTOCOL_HEADER_LEN + 1];
        systemInfo.positionInfo.coordinate_status =
            static_cast<PositionStatus_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 2]);
        systemInfo.ntripInfo.gprs_status = static_cast<On_Off_Status_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 3]);
        systemInfo.ntripInfo.NtripServer_status =
            static_cast<On_Off_Status_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 4]);
        systemInfo.ntripInfo.NtripClient_status =
            static_cast<On_Off_Status_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 5]);
        systemInfo.radioInfo.radio_status = static_cast<On_Off_Status_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 6]);

        systemInfo.positionInfo.satellite_number_track = parse->buffer[NM_PROTOCOL_HEADER_LEN + 7];

        const uint8_t externalPower = parse->buffer[NM_PROTOCOL_HEADER_LEN + 8];
        if (systemInfo.powerMonitor.ExternalPower && externalPower == 0) {
            systemInfo.powerMonitor.ExternalPowerChange = 1;
        }
        systemInfo.powerMonitor.ExternalPower = externalPower;
        systemInfo.radioInfo.radio_change_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN + 11];
        memcpy(&systemInfo.ntripInfo.NtripServer_IP, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 12], 4);
        memcpy(&systemInfo.ntripInfo.NtripServer_Mountpoint, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 16], 32);
        memcpy(&systemInfo.ntripInfo.NtripClient_IP, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 48], 4);
        memcpy(&systemInfo.ntripInfo.NtripClient_Mountpoint, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 52], 32);
        systemInfo.radioInfo.radio_mode = static_cast<RadioMode_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 84]);
        systemInfo.radioInfo.radio_protocol = parse->buffer[NM_PROTOCOL_HEADER_LEN + 85];
        systemInfo.radioInfo.radio_channel = parse->buffer[NM_PROTOCOL_HEADER_LEN + 86];
        if (parse->buffer[NM_PROTOCOL_HEADER_LEN + 87]) {
            systemInfo.powerMonitor.poweroff_flag = 1;
            systemInfo.powerMonitor.LinuxPowerOff = true;
        }
    }
    write_success_payload(msg);
}

static void
handle_set3_request(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    systemInfo.messageDecode.Set3_count++;
    systemInfo.positionInfo.coordinate_status =
        static_cast<PositionStatus_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 0]);
    memcpy(&systemInfo.positionInfo.coordinate_lon, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 4], 8);
    memcpy(&systemInfo.positionInfo.coordinate_lat, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 12], 8);
    memcpy(&systemInfo.positionInfo.coordinate_alt, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 20], 8);
    write_success_payload(msg);
}

static void
handle_set6_request(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    systemInfo.messageDecode.Set6_count++;
    if (systemInfo.recordInfo.record_change_flag == 0) {
        systemInfo.recordInfo.record_status = static_cast<On_Off_Status_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 0]);
        memcpy(&systemInfo.recordInfo.record_leftspace, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 4], 4);
        memcpy(&systemInfo.recordInfo.record_name, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 8], 16);
        systemInfo.recordInfo.record_type = parse->buffer[NM_PROTOCOL_HEADER_LEN + 24];
        systemInfo.recordInfo.record_interval = parse->buffer[NM_PROTOCOL_HEADER_LEN + 25];
        systemInfo.recordInfo.record_change_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN + 26];
    }
    write_success_payload(msg);
}

static void
handle_set_wifi_request(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    systemInfo.messageDecode.SetWifi_count++;
    if (systemInfo.wifiInfo.wifi_change_flag == 0) {
        systemInfo.wifiInfo.wifi_status = static_cast<On_Off_Status_t>(parse->buffer[NM_PROTOCOL_HEADER_LEN + 0]);
        systemInfo.wifiInfo.wifi_mode = parse->buffer[NM_PROTOCOL_HEADER_LEN + 1];
        memcpy(&systemInfo.wifiInfo.wifi_ip, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 4], 4);
        memcpy(&systemInfo.wifiInfo.wifi_ssid, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 8], 16);
    }
    write_success_payload(msg);
}

static void
handle_set_satellite_request(const SEMP_PARSE_STATE* parse, uint8_t* msg) {
    systemInfo.messageDecode.SetSatellite_count++;
    systemInfo.starMapInfo.numberGPS = parse->buffer[NM_PROTOCOL_HEADER_LEN + 0];
    systemInfo.starMapInfo.numberGLONASS = parse->buffer[NM_PROTOCOL_HEADER_LEN + 1];
    systemInfo.starMapInfo.numberGALILEO = parse->buffer[NM_PROTOCOL_HEADER_LEN + 2];
    systemInfo.starMapInfo.numberBDS = parse->buffer[NM_PROTOCOL_HEADER_LEN + 3];
    systemInfo.starMapInfo.numberSBAS = parse->buffer[NM_PROTOCOL_HEADER_LEN + 4];
    systemInfo.starMapInfo.numberQZSS = parse->buffer[NM_PROTOCOL_HEADER_LEN + 5];
    systemInfo.starMapInfo.numberIRNSS = parse->buffer[NM_PROTOCOL_HEADER_LEN + 6];
    systemInfo.starMapInfo.syncStatus = true;
    write_success_payload(msg);
}

static const PacketDescriptor packetTable[] = {
    {NM_PANEL_INFO1_ID, 0, NM_PROTOCOL_INFO1_MSG_LEN, NM_MSG_QUERY_RES_TYPE, fill_info1_response},
    {NM_PANEL_INFO2_ID, 0, NM_PROTOCOL_INFO2_MSG_LEN, NM_MSG_QUERY_RES_TYPE, fill_info2_response},
    {NM_PANEL_INFO3_ID, 0, NM_PROTOCOL_INFO3_MSG_LEN, NM_MSG_QUERY_RES_TYPE, fill_info3_response},
    {NM_PANEL_INFO4_ID, 0, NM_PROTOCOL_INFO4_MSG_LEN, NM_MSG_QUERY_RES_TYPE, fill_info4_response},
    {NM_PANEL_WIFI_SET_ID, 0, NM_PROTOCOL_WIFI_SET_MSG_LEN, NM_MSG_QUERY_RES_TYPE, fill_wifi_info_response},
    {NM_PANEL_RST_ID, 1, NM_PROTOCOL_RST_RESP_MSG_LEN, NM_MSG_SET_RES_TYPE, handle_reset_request},
    {NM_PANEL_SET1_ID, 88, NM_PROTOCOL_SET_MSG_LEN, NM_MSG_SET_RES_TYPE, handle_set1_request},
    {NM_PANEL_SET3_ID, 28, NM_PROTOCOL_SET_MSG_LEN, NM_MSG_SET_RES_TYPE, handle_set3_request},
    {NM_PANEL_SET6_ID, 27, NM_PROTOCOL_SET_MSG_LEN, NM_MSG_SET_RES_TYPE, handle_set6_request},
    {NM_PANEL_SET_WIFI_ID, 24, NM_PROTOCOL_SET_MSG_LEN, NM_MSG_SET_RES_TYPE, handle_set_wifi_request},
    {NM_PANEL_SET_SATELLITE_ID, 7, NM_PROTOCOL_SET_MSG_LEN, NM_MSG_SET_RES_TYPE, handle_set_satellite_request},
};

static const PacketDescriptor*
find_packet_descriptor(const uint16_t messageId) {
    for (unsigned int i = 0; i < (sizeof(packetTable) / sizeof(packetTable[0])); i++) {
        if (packetTable[i].messageId == messageId) {
            return &packetTable[i];
        }
    }
    return nullptr;
}

static bool
validate_request(const SEMP_PARSE_STATE* parse, const PacketDescriptor* descriptor) {
    if (parse->length < (NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN)) {
        return false;
    }

    const uint16_t payloadLength = request_payload_length(parse);
    if (payloadLength < descriptor->minRequestPayloadLength) {
        return false;
    }

    const uint32_t expectedLength = static_cast<uint32_t>(NM_PROTOCOL_HEADER_LEN) + payloadLength + NM_PROTOCOL_CRC_LEN;
    return parse->length >= expectedLength;
}

static int
encode_response(const SEMP_PARSE_STATE* parse, const PacketDescriptor* descriptor, uint8_t* txBuffer,
                const uint16_t txBufferSize) {
    const uint16_t packetLength =
        static_cast<uint16_t>(NM_PROTOCOL_HEADER_LEN + descriptor->responsePayloadLength + NM_PROTOCOL_CRC_LEN);
    if (packetLength > txBufferSize) {
        CORE_DEBUG_PRINTF("response too long: id=0x%04X, len=%u, buffer=%u", descriptor->messageId, packetLength,
                          txBufferSize);
        return 0;
    }

    memset(txBuffer, 0, packetLength);
    txBuffer[0] = NM_PROTOCOL_SYN_BYTE1;
    txBuffer[1] = NM_PROTOCOL_SYN_BYTE2;
    txBuffer[2] = NM_PROTOCOL_SYN_BYTE3;
    txBuffer[3] = NM_PROTOCOL_HEADER_LEN;
    txBuffer[NM_PROTOCOL_MSG_ID_INDEX_L] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L];
    txBuffer[NM_PROTOCOL_MSG_ID_INDEX_H] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_H];
    write_u16_le(&txBuffer[NM_PROTOCOL_MSG_LEN_INDEX_L], descriptor->responsePayloadLength);
    txBuffer[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANEL;
    txBuffer[NM_PROTOCOL_MSG_TYPE_INDEX] = descriptor->responseType;

    descriptor->handler(parse, txBuffer);
    write_crc(txBuffer, NM_PROTOCOL_HEADER_LEN + descriptor->responsePayloadLength);
    return packetLength;
}

int
message_decode(SEMP_PARSE_STATE* parse, uint8_t* txBuffer, const uint16_t txBufferSize) {
    DDL_ASSERT(parse != nullptr);
    DDL_ASSERT(txBuffer != nullptr);

    if (!systemInfo.online_device.eg25_board) {
        systemInfo.online_device.eg25_board = 1;
        systemInfo.i2c__err_count = 0;
    }

    if (parse->length < NM_PROTOCOL_HEADER_LEN) {
        systemInfo.messageDecode.error_count++;
        CORE_DEBUG_PRINTF("short message len: %d", parse->length);
        return 0;
    }

    const uint16_t messageId = request_message_id(parse);
    const PacketDescriptor* descriptor = find_packet_descriptor(messageId);
    if (descriptor == nullptr) {
        systemInfo.messageDecode.error_count++;
        CORE_DEBUG_PRINTF("unknown message id: 0x%04X, len: %d", messageId, parse->length);
        return 0;
    }

    if (!validate_request(parse, descriptor)) {
        systemInfo.messageDecode.error_count++;
        CORE_DEBUG_PRINTF("invalid message: id=0x%04X, payload=%u, len=%d", messageId, request_payload_length(parse),
                          parse->length);
        return 0;
    }

    if (messageId == NM_PANEL_RST_ID) {
        CORE_DEBUG_PRINTF("[%d] Reset Panel", messageId);
    }

    return encode_response(parse, descriptor, txBuffer, txBufferSize);
}
