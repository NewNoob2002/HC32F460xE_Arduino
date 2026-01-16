#ifndef DATA_PROC_DEF_H
#define DATA_PROC_DEF_H

#include <stdint.h>

#if defined(__cplusplus)
namespace DataProc
{

/* Recorder */
typedef enum
{
    RECORDER_CMD_START,
    RECORDER_CMD_STOP,
} Recorder_Cmd_t;

/* StatusBar */
typedef enum
{
    STATUS_BAR_STYLE_TRANSP,
    STATUS_BAR_STYLE_BLACK,
} StatusBar_Style_t;

typedef enum
{
    STATUS_BAR_CMD_APPEAR,
    STATUS_BAR_CMD_SET_STYLE,
    STATUS_BAR_CMD_SET_LABEL_REC
} StatusBar_Cmd_t;

typedef struct
{
    StatusBar_Cmd_t cmd;
    struct
    {
        bool appear;
        bool delay;
        bool record_active;
        StatusBar_Style_t style;
    } param;
} StatusBar_Info_t;

typedef enum
{
    LED_CMD_BLINK,
		LED_CMD_NOT_BLINK,
} Led_Cmd_t;

typedef struct
{
    Led_Cmd_t cmd;
    struct
    {
        uint16_t powerLed_blink_interval;
				uint16_t charge_blink_interval;
				uint16_t funckey_blink_interval;
    } param;
} Led_Info_t;

}

#endif //__cplusplus

#endif