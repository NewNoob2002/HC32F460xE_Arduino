#include "SparkFun_Extensible_Message_Parser.h"

#include <cassert>
#include <cstdint>
#include <vector>

namespace {
int messagesReceived = 0;

void messageReceived(SEMP_PARSE_STATE*, uint16_t) {
    ++messagesReceived;
}

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

std::vector<uint8_t> frame(size_t payloadLength) {
    std::vector<uint8_t> bytes(20 + payloadLength, 0);
    bytes[0] = 0xAA;
    bytes[1] = 0x44;
    bytes[2] = 0x18;
    bytes[3] = 20;
    bytes[4] = 1;
    bytes[12] = static_cast<uint8_t>(payloadLength);
    bytes[13] = static_cast<uint8_t>(payloadLength >> 8);
    for (size_t i = 0; i < payloadLength; ++i) {
        bytes[20 + i] = static_cast<uint8_t>(i);
    }
    const uint32_t crc = crc32(bytes.data(), bytes.size());
    bytes.push_back(static_cast<uint8_t>(crc));
    bytes.push_back(static_cast<uint8_t>(crc >> 8));
    bytes.push_back(static_cast<uint8_t>(crc >> 16));
    bytes.push_back(static_cast<uint8_t>(crc >> 24));
    return bytes;
}

void feed(SEMP_PARSE_STATE* parse, const std::vector<uint8_t>& bytes) {
    for (uint8_t byte : bytes) {
        sempParseNextByte(parse, byte);
    }
}
}

int main() {
    const SEMP_PARSE_ROUTINE parsers[] = {sempCustomPreamble};
    const char* names[] = {"CUSTOM"};
    SEMP_PARSE_STATE* parse = sempBeginParser(parsers, 1, names, 1, 0, 512, messageReceived, "host", nullptr, nullptr);
    assert(parse != nullptr);

    feed(parse, frame(0));
    assert(messagesReceived == 1);

    auto badCrc = frame(3);
    badCrc.back() ^= 0x01;
    feed(parse, badCrc);
    assert(messagesReceived == 1);

    feed(parse, {0x10, 0xAA, 0x10, 0xAA, 0x44});
    feed(parse, frame(4));
    assert(messagesReceived == 2);

    feed(parse, frame(488));
    assert(messagesReceived == 3);

    auto oversizedHeader = frame(0);
    oversizedHeader.resize(20);
    oversizedHeader[12] = 0xE9;
    oversizedHeader[13] = 0x01;
    feed(parse, oversizedHeader);
    feed(parse, frame(0));
    assert(messagesReceived == 4);

    feed(parse, frame(0));
    feed(parse, frame(0));
    assert(messagesReceived == 6);

    sempStopParser(&parse);
    assert(parse == nullptr);
}
