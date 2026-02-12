#include "message_decode.h"
#include "core_debug.h"
#include "mcu_config.h"


static uint32_t
calculate_crc(const char* msg, unsigned int len) {
    if (msg == nullptr || len == 0) {
        return 0;
    }
    unsigned long crc = 0xFFFFFFFF;
    for (int n = 0; n < len; n++) {
        crc = semp_crc32Table[(crc ^ msg[n]) & 0xff] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

static int
message_info_encode(SEMP_PARSE_STATE* parse, uint8_t* txBuffer) {
    uint8_t* msg = txBuffer;
    uint16_t messageLength = 0;
    uint16_t packageLength = 0;
    switch (parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L]) {
        case NM_PANEL_INFO1_ID: messageLength = NM_PROTOCOL_INFO1_MSG_LEN; break;
        case NM_PANEL_INFO2_ID: messageLength = NM_PROTOCOL_INFO2_MSG_LEN; break;
        case NM_PANEL_INFO3_ID: messageLength = NM_PROTOCOL_INFO3_MSG_LEN; break;
        case NM_PANEL_INFO4_ID: messageLength = NM_PROTOCOL_INFO4_MSG_LEN; break;
        default: break;
    }

    msg[0] = NM_PROTOCOL_SYN_BYTE1;
    msg[1] = NM_PROTOCOL_SYN_BYTE2;
    msg[2] = NM_PROTOCOL_SYN_BYTE3;
    msg[3] = NM_PROTOCOL_HEADER_LEN;
    msg[NM_PROTOCOL_MSG_ID_INDEX_L] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L];
    msg[NM_PROTOCOL_MSG_ID_INDEX_H] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_H];
    msg[NM_PROTOCOL_MSG_LEN_INDEX_L] = (char)(messageLength & 0x00FF);
    msg[NM_PROTOCOL_MSG_LEN_INDEX_H] = (char)((messageLength >> 8) & 0x00FF);
    msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANEL;
    msg[NM_PROTOCOL_MSG_TYPE_INDEX] = NM_MSG_QUERY_RES_TYPE;

    switch (parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L]) {
        case NM_PANEL_INFO1_ID: {
            const char hardware[8] = HARDWARE_VERSION;
            const char software[8] = SOFTWARE_VERSION;
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 0], hardware, strlen(hardware)); // HardWare_Version
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 8], software, strlen(software)); // SoftWare_Version
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 18], &systemInfo.powerMonitor.batteryInfo.Percent, 2);
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 20], &systemInfo.powerMonitor.batteryInfo.Temp, 2);
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 22], &systemInfo.powerMonitor.batteryInfo.Voltage, 2);
        } break;
        case NM_PANEL_INFO2_ID:
            msg[NM_PROTOCOL_HEADER_LEN + 0] = systemInfo.powerMonitor.reset_flag;    // 主机复位控制
            msg[NM_PROTOCOL_HEADER_LEN + 1] = systemInfo.powerMonitor.poweroff_flag; // 主机关机控制
            msg[NM_PROTOCOL_HEADER_LEN + 2] = systemInfo.recordInfo.record_status;   // 静态记录状态
            msg[NM_PROTOCOL_HEADER_LEN + 3] = systemInfo.recordInfo.record_op;       // 静态记录开关
            msg[NM_PROTOCOL_HEADER_LEN + 4] =
                systemInfo.powerMonitor.batteryInfo.chargeStatus != notCharge; // 充电电源接入
            if (systemInfo.recordInfo.record_change_flag) {
                systemInfo.recordInfo.record_change_flag = 0;
            }
            systemInfo.recordInfo.record_op = 0;
            if (systemInfo.powerMonitor.poweroff_flag == 1) {
                systemInfo.powerMonitor.poweroff_flag = 0;
                systemInfo.powerMonitor.ShutdownEnsure = true;
                CORE_DEBUG_PRINTF("Shutdown Sync");
            }
            break;
        case NM_PANEL_INFO3_ID:
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
            break;
        case NM_PANEL_INFO4_ID:
            msg[NM_PROTOCOL_HEADER_LEN + 0] = systemInfo.recordInfo.record_status;
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 4], &systemInfo.recordInfo.record_leftspace, 4);
            memcpy(&msg[NM_PROTOCOL_HEADER_LEN + 8], &systemInfo.recordInfo.record_name, 16);
            msg[NM_PROTOCOL_HEADER_LEN + 24] = systemInfo.recordInfo.record_type;
            msg[NM_PROTOCOL_HEADER_LEN + 25] = systemInfo.recordInfo.record_interval;
            msg[NM_PROTOCOL_HEADER_LEN + 26] = systemInfo.recordInfo.record_change_flag;
            systemInfo.recordInfo.record_change_flag = 0;
            break;
        default: break;
    }

    const uint32_t crc = calculate_crc((const char*)msg, (messageLength + NM_PROTOCOL_HEADER_LEN));
    msg[messageLength + NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
    msg[messageLength + NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
    msg[messageLength + NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
    msg[messageLength + NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN - 4] = (char)(crc & 0x000000FF);

    return messageLength + NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN;
}

static int
message_reset_encode(SEMP_PARSE_STATE* parse, uint8_t* txBuffer) {
    uint8_t* msg = txBuffer;

    systemInfo.powerMonitor.reset_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN];

    const uint16_t messageLength = NM_PROTOCOL_RST_RESP_MSG_LEN;
    msg[0] = NM_PROTOCOL_SYN_BYTE1;
    msg[1] = NM_PROTOCOL_SYN_BYTE2;
    msg[2] = NM_PROTOCOL_SYN_BYTE3;
    msg[3] = NM_PROTOCOL_HEADER_LEN;
    msg[NM_PROTOCOL_MSG_ID_INDEX_L] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L];
    msg[NM_PROTOCOL_MSG_ID_INDEX_H] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_H];
    msg[NM_PROTOCOL_MSG_LEN_INDEX_L] = (char)(messageLength & 0x00FF);
    msg[NM_PROTOCOL_MSG_LEN_INDEX_H] = (char)((messageLength >> 8) & 0x00FF);
    msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANEL;
    msg[NM_PROTOCOL_MSG_TYPE_INDEX] = NM_MSG_SET_RES_TYPE;

    msg[NM_PROTOCOL_HEADER_LEN] = 1;

    const uint32_t crc = calculate_crc((const char*)msg, (NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

    msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
    msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
    msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
    msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);

    return NM_PROTOCOL_RST_RESP_MSG_PACK_LEN;
}

static int
message_set_encode(SEMP_PARSE_STATE* parse, uint8_t* txBuffer) {
    uint8_t* msg = txBuffer;

    switch (parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L]) {
        case NM_PANEL_SET1_ID:
            if (systemInfo.radioInfo.radio_change_flag == 0) {
                systemInfo.work_mode = (WorkMode_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 0];
                systemInfo.positionInfo.satellite_number_used = parse->buffer[NM_PROTOCOL_HEADER_LEN + 1];
                systemInfo.positionInfo.coordinate_status = (PositionStatus_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 2];
                systemInfo.ntripInfo.gprs_status = (On_Off_Status_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 3];
                systemInfo.ntripInfo.NtripServer_status = (On_Off_Status_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 4];
                systemInfo.ntripInfo.NtripClient_status = (On_Off_Status_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 5];
                systemInfo.radioInfo.radio_status = (On_Off_Status_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 6];

                systemInfo.positionInfo.satellite_number_track = parse->buffer[NM_PROTOCOL_HEADER_LEN + 7];

                uint8_t ExternalPower = parse->buffer[NM_PROTOCOL_HEADER_LEN + 8];
                if (systemInfo.powerMonitor.ExternalPower && ExternalPower == 0) {
                    systemInfo.powerMonitor.ExternalPowerChange = 1;
                }
                systemInfo.powerMonitor.ExternalPower = ExternalPower;
                systemInfo.radioInfo.radio_change_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN + 11];
                memcpy(&systemInfo.ntripInfo.NtripServer_IP, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 12], 4);
                memcpy(&systemInfo.ntripInfo.NtripServer_Mountpoint, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 16], 32);
                memcpy(&systemInfo.ntripInfo.NtripClient_IP, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 48], 4);
                memcpy(&systemInfo.ntripInfo.NtripClient_Mountpoint, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 52], 32);
                systemInfo.radioInfo.radio_mode = (RadioMode_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 84];
                systemInfo.radioInfo.radio_protocol = parse->buffer[NM_PROTOCOL_HEADER_LEN + 85];
                systemInfo.radioInfo.radio_channel = parse->buffer[NM_PROTOCOL_HEADER_LEN + 86];
                if (parse->buffer[NM_PROTOCOL_HEADER_LEN + 87]) {
                    systemInfo.powerMonitor.poweroff_flag = 1;
                    systemInfo.powerMonitor.LinuxPowerOff = true;
                }
            }
            break;
        case NM_PANEL_SET3_ID:
            systemInfo.positionInfo.coordinate_status = (PositionStatus_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 0];
            memcpy(&systemInfo.positionInfo.coordinate_lon, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 4], 8);
            memcpy(&systemInfo.positionInfo.coordinate_lat, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 12], 8);
            memcpy(&systemInfo.positionInfo.coordinate_alt, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 20], 8);
            break;
        case NM_PANEL_SET6_ID:
            if (systemInfo.recordInfo.record_change_flag == 0) {
                systemInfo.recordInfo.record_status = (On_Off_Status_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 0];
                memcpy(&systemInfo.recordInfo.record_leftspace, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 4], 4);
                memcpy(&systemInfo.recordInfo.record_name, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 8], 16);
                systemInfo.recordInfo.record_type = parse->buffer[NM_PROTOCOL_HEADER_LEN + 24];
                systemInfo.recordInfo.record_interval = parse->buffer[NM_PROTOCOL_HEADER_LEN + 25];
                systemInfo.recordInfo.record_change_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN + 26];
            }
            break;
        case NM_PANEL_SET7_ID:
            systemInfo.wifiInfo.wifi_status = (On_Off_Status_t)parse->buffer[NM_PROTOCOL_HEADER_LEN + 0];
            systemInfo.wifiInfo.wifi_mode = parse->buffer[NM_PROTOCOL_HEADER_LEN + 1];
            memcpy(&systemInfo.wifiInfo.wifi_ip, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 4], 4);
            memcpy(&systemInfo.wifiInfo.wifi_ssid, &parse->buffer[NM_PROTOCOL_HEADER_LEN + 8], 16);
            break;
        default: break;
    }

    const uint16_t messageLength = NM_PROTOCOL_SET_MSG_LEN;
    msg[0] = NM_PROTOCOL_SYN_BYTE1;
    msg[1] = NM_PROTOCOL_SYN_BYTE2;
    msg[2] = NM_PROTOCOL_SYN_BYTE3;
    msg[3] = NM_PROTOCOL_HEADER_LEN;
    msg[NM_PROTOCOL_MSG_ID_INDEX_L] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_L];
    msg[NM_PROTOCOL_MSG_ID_INDEX_H] = parse->buffer[NM_PROTOCOL_MSG_ID_INDEX_H];
    msg[NM_PROTOCOL_MSG_LEN_INDEX_L] = (char)(messageLength & 0x00FF);
    msg[NM_PROTOCOL_MSG_LEN_INDEX_H] = (char)((messageLength >> 8) & 0x00FF);
    msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANEL;
    msg[NM_PROTOCOL_MSG_TYPE_INDEX] = NM_MSG_SET_RES_TYPE;

    msg[NM_PROTOCOL_HEADER_LEN + 0] = 1;
    msg[NM_PROTOCOL_HEADER_LEN + 1] = 0;

    const uint32_t crc = calculate_crc((const char*)msg, (NM_PROTOCOL_SET_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

    msg[NM_PROTOCOL_SET_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
    msg[NM_PROTOCOL_SET_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
    msg[NM_PROTOCOL_SET_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
    msg[NM_PROTOCOL_SET_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);

    return NM_PROTOCOL_SET_MSG_PACK_LEN;
}

int
message_decode(SEMP_PARSE_STATE* parse, uint8_t* txBuffer) {
    DDL_ASSERT(parse != nullptr);
    DDL_ASSERT(txBuffer != nullptr);
    SEMP_CUSTOM_HEADER* messageHeader = (SEMP_CUSTOM_HEADER*)parse->buffer;
    uint16_t messageId = *(uint16_t*)&messageHeader->messageId_L;
    uint8_t messageType = messageHeader->messageType;
    if (!systemInfo.online_device.eg25_board) {
        systemInfo.online_device.eg25_board = true;
        systemInfo.i2c__err_count = 0;
    }
    switch (messageId) {
        case NM_PANEL_INFO1_ID:
        case NM_PANEL_INFO2_ID:
        case NM_PANEL_INFO3_ID:
        case NM_PANEL_INFO4_ID: return message_info_encode(parse, txBuffer); break;
        case NM_PANEL_RST_ID:
            CORE_DEBUG_PRINTF("[%d] Reset Panel", messageId);
            return message_reset_encode(parse, txBuffer);
            break;
        case NM_PANEL_SET1_ID:
        case NM_PANEL_SET3_ID:
        case NM_PANEL_SET6_ID:
        case NM_PANEL_SET7_ID:
        case NM_PANEL_SET3_1_ID: return message_set_encode(parse, txBuffer); break;
        default: break;
    }
    return 0;
}