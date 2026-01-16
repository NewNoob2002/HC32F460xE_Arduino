#pragma once

#include "SparkFun_Extensible_Message_Parser.h"

#define NM_PROTOCOL_SYN_BYTE1             0xaa
#define NM_PROTOCOL_SYN_BYTE2             0x44
#define NM_PROTOCOL_SYN_BYTE3             0x18
#define NM_PROTOCOL_HEADER_LEN            20
#define NM_PROTOCOL_CRC_LEN               4
#define NM_PROTOCOL_MSG_ID_INDEX_L        4
#define NM_PROTOCOL_MSG_ID_INDEX_H        5
#define NM_PROTOCOL_MSG_LEN_INDEX_L       12
#define NM_PROTOCOL_MSG_LEN_INDEX_H       13
#define NM_PROTOCOL_MSG_SENDER_INDEX      16
#define NM_PROTOCOL_MSG_TYPE_INDEX        17
#define NM_PROTOCOL_MSG_INTERVAL_INDEX    19

#define NM_PROTOCOL_MSG_SENDER_DEV        0x01
#define NM_PROTOCOL_MSG_SENDER_APP        0x00
#define NM_PROTOCOL_MSG_SENDER_PANEL     0x03

#define NM_MSG_QUERY_TYPE                 0x00
#define NM_MSG_QUERY_RES_TYPE             0x01
#define NM_MSG_SET_TYPE                   0x02
#define NM_MSG_SET_RES_TYPE               0x03
#define NM_MSG_ERR_REP_TYPE               0x05

#define NM_MSG_CRC_ERR_REP_ID             0x01
#define NM_MSG_MSG_ID_ERR_REP_ID          0x02
#define NM_MSG_MODULE_ID_ERR_REP_ID       0x03


#define NM_PANEL_INFO1_ID                 0x01
#define NM_PROTOCOL_INFO1_MSG_LEN         36    //32 bytes message length
#define NM_PROTOCOL_INFO1_MSG_PACK_LEN    60    //60 bytes package length

#define NM_PANEL_INFO2_ID                 0x02
#define NM_PROTOCOL_INFO2_MSG_LEN         8    //8 bytes message length
#define NM_PROTOCOL_PINFO2_MSG_PACK_LEN   32    //32 bytes package length

#define NM_PANEL_INFO3_ID                 0x07
#define NM_PROTOCOL_INFO3_MSG_LEN         88    //88 bytes message length
#define NM_PROTOCOL_PINFO3_MSG_PACK_LEN   112    //112 bytes package length

#define NM_PANEL_INFO4_ID                 0x0A
#define NM_PROTOCOL_INFO4_MSG_LEN         28    //28 bytes message length
#define NM_PROTOCOL_PINFO4_MSG_PACK_LEN   52    //52 bytes package length

#define NM_PANEL_RST_ID                   0x04
#define NM_PROTOCOL_RST_RESP_MSG_LEN      2    //2 bytes message length
#define NM_PROTOCOL_RST_RESP_MSG_PACK_LEN 26    //26 bytes package length

#define NM_PANEL_SET1_ID                  0x06
#define NM_PANEL_SET3_ID                  0x08
#define NM_PANEL_SET6_ID                  0x09
#define NM_PANEL_SET7_ID                  0x0C
#define NM_PANEL_SET3_1_ID                0x03

#define NM_PROTOCOL_SET_MSG_LEN           2    //2 bytes message length
#define NM_PROTOCOL_SET_MSG_PACK_LEN      26    //26 bytes package length

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t txBuffer_temp[NM_PROTOCOL_PINFO3_MSG_PACK_LEN];

int message_decode(SEMP_PARSE_STATE *parse, uint8_t *txBuffer);

#ifdef __cplusplus
}
#endif