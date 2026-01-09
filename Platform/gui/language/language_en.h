#ifndef LANGUAGE_EN_H
#define LANGUAGE_EN_H

#include "language.h"

#if defined(LOGO_RUSSION)

#else
static const char *en_texts[TEXT_MAX] = {
    [TEXT_ROVER] = "ROVER",
    [TEXT_BASE] = "BASE",
    [TEXT_POSITION_NONE] = "NONE",
    [TEXT_POSITION_SINGLE] = "SINGLE",
    [TEXT_POSITION_FIX] = "FIX",
    [TEXT_POSITION_FLOAT] = "FLOAT",
    [TEXT_POSITION_FIXEDPOS] = "FIXEDPOS",

    [TEXT_MODE_RADIO_RECEIVE] = "Mode: Receive",
    [TEXT_MODE_RADIO_TRANSMIT] = "Mode: Transmit",
    [TEXT_MODE_RADIO_BRIDGE] = "Mode: Bridge",
    [TEXT_MODE_RADIO_PROTOCOL] = "Protocol:",
    [TEXT_MODE_RADIO_CHANNEL] = "Channel:",
    [TEXT_MODE_NTRIP_CLIENT] = "Ntrip Client",
    [TEXT_MODE_NTRIP_SERVER] = "Ntrip Server",

    [TEXT_RECORD_SPACE] = "Space:",
    [TEXT_RECORD_NAME] = "Name:",
    [TEXT_RECORD_TYPE] = "Type:",
    [TEXT_RECORD_TIME] = "Time:",

    [TEXT_WIFI_MODE] = "Mode:",

    [TEXT_POWEROFF_CHARGE_HOLD] = "Hold On",

    [TEXT_POWEROFF_LABEL_LINUX_OFF] = "Board_Control_Shutdown",
    [TEXT_POWEROFF_LABEL_LOW_BAT_OFF] = "Low_Battery_Shutdown",
    [TEXT_POWEROFF_LABEL_FORCE_OFF] = "Force_Shutdown",
    [TEXT_POWEROFF_LABEL_BUTTON_OFF] = "Button_Shutdown",
    [TEXT_POWEROFF_LABEL_SYNC_DONE] = "Sync Done",
};
#endif

#endif // LANGUAGE_EN_H