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
		[TEXT_POSITION_RTD] = "RTD",
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

		[TEXT_POWEROFF_LABEL_LINUX_OFF] = "Info: Shutdown Board",
    [TEXT_POWEROFF_LABEL_LOW_BAT_OFF] = "Info: Shutdown LowBattery",
    [TEXT_POWEROFF_LABEL_FORCE_OFF] = "Info: Shutdown Forced",
    [TEXT_POWEROFF_LABEL_BUTTON_OFF] = "Info: Shutdown PushKey",
    [TEXT_POWEROFF_LABEL_NOTIFY] = "Keep Press Shutdown",
		
		[TEXT_HARAWARE_CHECK] = "Warming: Don't Shutdown Now",
};
#endif

#endif // LANGUAGE_EN_H