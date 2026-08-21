#include "message_decode.h"
#include "mcu_define.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

SystemInfo_t systemInfo;

namespace {
struct Request {
    std::vector<uint8_t> storage;
    SEMP_PARSE_STATE parse{};

    Request(uint16_t messageId, size_t payloadLength) : storage(1 + NM_PROTOCOL_HEADER_LEN + payloadLength + NM_PROTOCOL_CRC_LEN, 0) {
        parse.buffer = storage.data() + 1;
        parse.length = static_cast<uint16_t>(storage.size() - 1);
        parse.buffer[0] = NM_PROTOCOL_SYN_BYTE1;
        parse.buffer[1] = NM_PROTOCOL_SYN_BYTE2;
        parse.buffer[2] = NM_PROTOCOL_SYN_BYTE3;
        parse.buffer[3] = NM_PROTOCOL_HEADER_LEN;
        parse.buffer[NM_PROTOCOL_MSG_ID_INDEX_L] = static_cast<uint8_t>(messageId);
        parse.buffer[NM_PROTOCOL_MSG_ID_INDEX_H] = static_cast<uint8_t>(messageId >> 8);
        parse.buffer[NM_PROTOCOL_MSG_LEN_INDEX_L] = static_cast<uint8_t>(payloadLength);
        parse.buffer[NM_PROTOCOL_MSG_LEN_INDEX_H] = static_cast<uint8_t>(payloadLength >> 8);
    }

    uint8_t* payload() { return parse.buffer + NM_PROTOCOL_HEADER_LEN; }
};

uint32_t crc32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc >> 1) ^ (0xEDB88320U & (0U - (crc & 1U)));
        }
    }
    return crc ^ 0xFFFFFFFF;
}

void assertValidResponse(const uint8_t* response, size_t length) {
    assert(length >= NM_PROTOCOL_HEADER_LEN + NM_PROTOCOL_CRC_LEN);
    const uint32_t expected = crc32(response, length - NM_PROTOCOL_CRC_LEN);
    const size_t crcOffset = length - NM_PROTOCOL_CRC_LEN;
    const uint32_t actual = static_cast<uint32_t>(response[crcOffset]) |
                            (static_cast<uint32_t>(response[crcOffset + 1]) << 8) |
                            (static_cast<uint32_t>(response[crcOffset + 2]) << 16) |
                            (static_cast<uint32_t>(response[crcOffset + 3]) << 24);
    assert(actual == expected);
}
}

int main() {
    std::array<uint8_t, NM_PROTOCOL_PINFO3_MSG_PACK_LEN + 8> response{};

    std::memset(&systemInfo, 0, sizeof(systemInfo));
    systemInfo.powerMonitor.batteryInfo.chargeStatus = normalCharge;
    Request info2(NM_PANEL_INFO2_ID, 0);
    int length = message_decode(&info2.parse, response.data(), response.size());
    assert(length == NM_PROTOCOL_PINFO2_MSG_PACK_LEN);
    assert(response[3] == NM_PROTOCOL_HEADER_LEN);
    assert(response[NM_PROTOCOL_MSG_SENDER_INDEX] == NM_PROTOCOL_MSG_SENDER_PANEL);
    assert(response[NM_PROTOCOL_MSG_TYPE_INDEX] == NM_MSG_QUERY_RES_TYPE);
    assert(response[NM_PROTOCOL_HEADER_LEN + 4] == 1);
    assertValidResponse(response.data(), static_cast<size_t>(length));

    std::fill(response.begin(), response.end(), 0xA5);
    Request info3(NM_PANEL_INFO3_ID, 0);
    length = message_decode(&info3.parse, response.data(), response.size());
    assert(length == NM_PROTOCOL_PINFO3_MSG_PACK_LEN);
    assert(std::all_of(response.begin() + length, response.end(), [](uint8_t value) { return value == 0xA5; }));
    assertValidResponse(response.data(), static_cast<size_t>(length));

    std::memset(&systemInfo, 0, sizeof(systemInfo));
    std::memset(systemInfo.hardware_version, 'H', sizeof(systemInfo.hardware_version));
    std::memset(systemInfo.software_version, 'S', sizeof(systemInfo.software_version));
    Request info1(NM_PANEL_INFO1_ID, 0);
    length = message_decode(&info1.parse, response.data(), response.size());
    assert(length == NM_PROTOCOL_INFO1_MSG_PACK_LEN);
    assert(std::all_of(response.begin() + NM_PROTOCOL_HEADER_LEN, response.begin() + NM_PROTOCOL_HEADER_LEN + 8, [](uint8_t value) { return value == 'H'; }));
    assert(std::all_of(response.begin() + NM_PROTOCOL_HEADER_LEN + 8, response.begin() + NM_PROTOCOL_HEADER_LEN + 18, [](uint8_t value) { return value == 'S'; }));

    const uint16_t shortCommands[] = {NM_PANEL_RST_ID, NM_PANEL_SET1_ID, NM_PANEL_SET3_ID, NM_PANEL_SET6_ID, NM_PANEL_SET7_ID};
    for (uint16_t command : shortCommands) {
        std::memset(&systemInfo, 0, sizeof(systemInfo));
        systemInfo.work_mode = base_mode;
        const SystemInfo_t before = systemInfo;
        Request shortRequest(command, 0);
        std::fill(response.begin(), response.end(), 0xA5);
        assert(message_decode(&shortRequest.parse, response.data(), response.size()) == 0);
        assert(std::memcmp(&systemInfo, &before, sizeof(systemInfo)) == 0);
        assert(std::all_of(response.begin(), response.end(), [](uint8_t value) { return value == 0xA5; }));
    }

    std::memset(&systemInfo, 0, sizeof(systemInfo));
    Request set3(NM_PANEL_SET3_ID, 28);
    set3.payload()[0] = position_fix;
    const double longitude = 121.4737;
    const double latitude = 31.2304;
    const double altitude = 12.5;
    std::memcpy(set3.payload() + 4, &longitude, sizeof(longitude));
    std::memcpy(set3.payload() + 12, &latitude, sizeof(latitude));
    std::memcpy(set3.payload() + 20, &altitude, sizeof(altitude));
    length = message_decode(&set3.parse, response.data(), response.size());
    assert(length == NM_PROTOCOL_SET_MSG_PACK_LEN);
    assert(systemInfo.positionInfo.coordinate_status == position_fix);
    assert(systemInfo.positionInfo.coordinate_lon == longitude);
    assert(systemInfo.positionInfo.coordinate_lat == latitude);
    assert(systemInfo.positionInfo.coordinate_alt == altitude);
    assertValidResponse(response.data(), static_cast<size_t>(length));

    Request unknown(0x1234, 0);
    std::fill(response.begin(), response.end(), 0xA5);
    assert(message_decode(&unknown.parse, response.data(), response.size()) == 0);
    assert(std::all_of(response.begin(), response.end(), [](uint8_t value) { return value == 0xA5; }));

    std::memset(&systemInfo, 0, sizeof(systemInfo));
    const SystemInfo_t before = systemInfo;
    std::fill(response.begin(), response.end(), 0xA5);
    assert(message_decode(&info3.parse, response.data(), NM_PROTOCOL_PINFO3_MSG_PACK_LEN - 1) == 0);
    assert(std::memcmp(&systemInfo, &before, sizeof(systemInfo)) == 0);
    assert(std::all_of(response.begin(), response.end(), [](uint8_t value) { return value == 0xA5; }));
}
