#include "message_decode.h"
#include "Arduino.h"

static uint32_t calculate_crc(const char *msg, unsigned int len)
{
    if (msg == nullptr || len == 0) {
        return 0;
    }
    unsigned long crc = 0xFFFFFFFF;
    for (int n = 0; n < len; n++) {
        crc = semp_crc32Table[(crc ^ msg[n]) & 0xff] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

int message_decode(SEMP_PARSE_STATE *parse, uint8_t *txBuffer)
{
    DDL_ASSERT(parse != nullptr);
    DDL_ASSERT(txBuffer != nullptr);
    SEMP_CUSTOM_HEADER *messageHeader = (SEMP_CUSTOM_HEADER *)parse->buffer;
    uint16_t messageId                = *(uint16_t *)&messageHeader->messageId_L;
    uint8_t messageType               = messageHeader->messageType;
    printf("messageId: %d, h:%d, l:%d,\n", messageId, messageHeader->messageId_H, messageHeader->messageId_L);
    for (int i = 0; i < parse->length; i++) {
        printf("%02x ", parse->buffer[i]);
    }
    printf("\n");
    digitalToggle(FUNCTION_LED_PIN);
    uint8_t *msg = txBuffer;
    switch (messageId) {
        case NM_PANNEL_INFO_ID: {

            uint16_t len                      = NM_PROTOCOL_PINFO_MSG_LEN;
            msg[0]                            = NM_PROTOCOL_SYN_BYTE1;
            msg[1]                            = NM_PROTOCOL_SYN_BYTE2;
            msg[2]                            = NM_PROTOCOL_SYN_BYTE3;
            msg[3]                            = NM_PROTOCOL_HEADER_LEN;
            msg[NM_PROTOCOL_MSG_ID_INDEX_L]   = messageHeader->messageId_L;
            msg[NM_PROTOCOL_MSG_ID_INDEX_H]   = messageHeader->messageId_H;
            msg[NM_PROTOCOL_MSG_LEN_INDEX_L]  = (char)(len & 0x00FF);
            msg[NM_PROTOCOL_MSG_LEN_INDEX_H]  = (char)((len >> 8) & 0x00FF);
            msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANNEL;
            msg[NM_PROTOCOL_MSG_TYPE_INDEX]   = NM_MSG_QUERY_RES_TYPE;
            memcpy(&msg[NM_PROTOCOL_PHV_OFFSET], HW_VERSION, strlen(HW_VERSION));
            memcpy(&msg[NM_PROTOCOL_PFV_OFFSET], SW_VERSION, strlen(SW_VERSION));

            memcpy(&msg[NM_PROTOCOL_PBL_OFFSET], &systemInfo.batteryInfo.Percent, 2);
            memcpy(&msg[NM_PROTOCOL_PBT_OFFSET], &systemInfo.batteryInfo.Temp, 2);
            memcpy(&msg[NM_PROTOCOL_PBV_OFFSET], &systemInfo.batteryInfo.Voltage, 2);

            msg[NM_PROTOCOL_PSS_OFFSET] = 1;

            const uint32_t crc = calculate_crc((const char *)msg, (NM_PROTOCOL_PINFO_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

            msg[NM_PROTOCOL_PINFO_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
            msg[NM_PROTOCOL_PINFO_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
            msg[NM_PROTOCOL_PINFO_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
            msg[NM_PROTOCOL_PINFO_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);
            return NM_PROTOCOL_PINFO_MSG_PACK_LEN;
            break;
        }
        case NM_PANNEL_CTRL_ID: {
            uint16_t len                      = NM_PROTOCOL_PCTRL_MSG_LEN;
            msg[0]                            = NM_PROTOCOL_SYN_BYTE1;
            msg[1]                            = NM_PROTOCOL_SYN_BYTE2;
            msg[2]                            = NM_PROTOCOL_SYN_BYTE3;
            msg[3]                            = NM_PROTOCOL_HEADER_LEN;
            msg[NM_PROTOCOL_MSG_ID_INDEX_L]   = messageHeader->messageId_L;
            msg[NM_PROTOCOL_MSG_ID_INDEX_H]   = messageHeader->messageId_H;
            msg[NM_PROTOCOL_MSG_LEN_INDEX_L]  = (char)(len & 0x00FF);
            msg[NM_PROTOCOL_MSG_LEN_INDEX_H]  = (char)((len >> 8) & 0x00FF);
            msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANNEL;
            msg[NM_PROTOCOL_MSG_TYPE_INDEX]   = NM_MSG_QUERY_RES_TYPE;

            msg[NM_PROTOCOL_PRE_OFFSET] = systemInfo.reset_flag;
            msg[NM_PROTOCOL_PPC_OFFSET] = systemInfo.poweroff_flag;

            msg[NM_PROTOCOL_PRC_OFFSET] = systemInfo.record_flag;
            msg[NM_PROTOCOL_PRO_OFFSET] = systemInfo.record_op;
            systemInfo.record_op        = 0;

            msg[NM_PROTOCOL_PEP_OFFSET] = systemInfo.usb_power_flag;

            const uint32_t crc = calculate_crc((const char *)msg, (NM_PROTOCOL_PCTRL_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

            msg[NM_PROTOCOL_PCTRL_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
            msg[NM_PROTOCOL_PCTRL_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
            msg[NM_PROTOCOL_PCTRL_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
            msg[NM_PROTOCOL_PCTRL_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);
            return NM_PROTOCOL_PCTRL_MSG_PACK_LEN;
            break;
        }
        case NM_PANNEL_HOST_ID: {
            uint8_t record_state_set = parse->buffer[NM_PROTOCOL_HEADER_LEN];

            if (!systemInfo.record_op) {
                if (record_state_set) {
                    systemInfo.record_flag = 1;
                } else {
                    systemInfo.record_flag = 0;
                }
            }

            //            state = pMsg[NM_PROTOCOL_HNS_OFFSET];

            uint16_t len                      = NM_PROTOCOL_HOST_MSG_LEN;
            msg[0]                            = NM_PROTOCOL_SYN_BYTE1;
            msg[1]                            = NM_PROTOCOL_SYN_BYTE2;
            msg[2]                            = NM_PROTOCOL_SYN_BYTE3;
            msg[3]                            = NM_PROTOCOL_HEADER_LEN;
            msg[NM_PROTOCOL_MSG_ID_INDEX_L]   = messageHeader->messageId_L;
            msg[NM_PROTOCOL_MSG_ID_INDEX_H]   = messageHeader->messageId_H;
            msg[NM_PROTOCOL_MSG_LEN_INDEX_L]  = (char)(len & 0x00FF);
            msg[NM_PROTOCOL_MSG_LEN_INDEX_H]  = (char)((len >> 8) & 0x00FF);
            msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANNEL;
            msg[NM_PROTOCOL_MSG_TYPE_INDEX]   = NM_MSG_SET_RES_TYPE;

            msg[NM_PROTOCOL_HEADER_LEN] = 1;

            const uint32_t crc = calculate_crc((const char *)msg, (NM_PROTOCOL_HOST_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

            msg[NM_PROTOCOL_HOST_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
            msg[NM_PROTOCOL_HOST_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
            msg[NM_PROTOCOL_HOST_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
            msg[NM_PROTOCOL_HOST_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);
            return NM_PROTOCOL_HOST_MSG_PACK_LEN;
            break;
        }
        case NM_PANNEL_RST_ID: {
            systemInfo.reset_flag = parse->buffer[NM_PROTOCOL_HEADER_LEN];

            uint16_t len                      = NM_PROTOCOL_RST_RESP_MSG_LEN;
            msg[0]                            = NM_PROTOCOL_SYN_BYTE1;
            msg[1]                            = NM_PROTOCOL_SYN_BYTE2;
            msg[2]                            = NM_PROTOCOL_SYN_BYTE3;
            msg[3]                            = NM_PROTOCOL_HEADER_LEN;
            msg[NM_PROTOCOL_MSG_ID_INDEX_L]   = messageHeader->messageId_L;
            msg[NM_PROTOCOL_MSG_ID_INDEX_H]   = messageHeader->messageId_H;
            msg[NM_PROTOCOL_MSG_LEN_INDEX_L]  = (char)(len & 0x00FF);
            msg[NM_PROTOCOL_MSG_LEN_INDEX_H]  = (char)((len >> 8) & 0x00FF);
            msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANNEL;
            msg[NM_PROTOCOL_MSG_TYPE_INDEX]   = NM_MSG_SET_RES_TYPE;

            msg[NM_PROTOCOL_HEADER_LEN] = 1;

            const uint32_t crc = calculate_crc((const char *)msg, (NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

            msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
            msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
            msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
            msg[NM_PROTOCOL_RST_RESP_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);
            return NM_PROTOCOL_RST_RESP_MSG_PACK_LEN;
            break;
        }
        case NM_PANNEL_POWER_ID: {
            if (parse->buffer[NM_PROTOCOL_HEADER_LEN]) {
                systemInfo.poweroff_flag              = 1;
                systemInfo.powerMonitor.LinuxPowerOff = true;
            }

            uint16_t len                      = NM_PROTOCOL_POWER_RESP_MSG_LEN;
            msg[0]                            = NM_PROTOCOL_SYN_BYTE1;
            msg[1]                            = NM_PROTOCOL_SYN_BYTE2;
            msg[2]                            = NM_PROTOCOL_SYN_BYTE3;
            msg[3]                            = NM_PROTOCOL_HEADER_LEN;
            msg[NM_PROTOCOL_MSG_ID_INDEX_L]   = messageHeader->messageId_L;
            msg[NM_PROTOCOL_MSG_ID_INDEX_H]   = messageHeader->messageId_H;
            msg[NM_PROTOCOL_MSG_LEN_INDEX_L]  = (char)(len & 0x00FF);
            msg[NM_PROTOCOL_MSG_LEN_INDEX_H]  = (char)((len >> 8) & 0x00FF);
            msg[NM_PROTOCOL_MSG_SENDER_INDEX] = NM_PROTOCOL_MSG_SENDER_PANNEL;
            msg[NM_PROTOCOL_MSG_TYPE_INDEX]   = NM_MSG_SET_RES_TYPE;

            msg[NM_PROTOCOL_HEADER_LEN] = 1;

            const uint32_t crc = calculate_crc((const char *)msg, (NM_PROTOCOL_POWER_RESP_MSG_PACK_LEN - NM_PROTOCOL_CRC_LEN));

            msg[NM_PROTOCOL_POWER_RESP_MSG_PACK_LEN - 1] = (char)((crc >> 24) & 0x000000FF);
            msg[NM_PROTOCOL_POWER_RESP_MSG_PACK_LEN - 2] = (char)((crc >> 16) & 0x000000FF);
            msg[NM_PROTOCOL_POWER_RESP_MSG_PACK_LEN - 3] = (char)((crc >> 8) & 0x000000FF);
            msg[NM_PROTOCOL_POWER_RESP_MSG_PACK_LEN - 4] = (char)(crc & 0x000000FF);
            return NM_PROTOCOL_POWER_RESP_MSG_PACK_LEN;
            break;
        }
        default:
            break;
    }
    return 0;
}