#ifndef LANGUAGE_RU_H
#define LANGUAGE_RU_H

#include "language.h"

#if defined(LOGO_RUSSION)

static const char *ru_texts[TEXT_MAX] = {
    [TEXT_ROVER] = "Ровер",
    [TEXT_BASE] = "База",
    [TEXT_POSITION_NONE] = "Никто",
    [TEXT_POSITION_SINGLE] = "Одинокий",
		[TEXT_POSITION_RTD] = "RTD",
    [TEXT_POSITION_FIX] = "Исправить",
    [TEXT_POSITION_FLOAT] = "Поплавок",
    [TEXT_POSITION_FIXEDPOS] = "Зафиксированный",

    [TEXT_MODE_RADIO_RECEIVE] = "Режим: Прием",
    [TEXT_MODE_RADIO_TRANSMIT] = "Режим: Передача",
    [TEXT_MODE_RADIO_BRIDGE] = "Режим: Мост",
    [TEXT_MODE_RADIO_PROTOCOL] = "Протокол:",
    [TEXT_MODE_RADIO_CHANNEL] = "Канал:",
    [TEXT_MODE_NTRIP_CLIENT] = "Клиент Ntrip",
    [TEXT_MODE_NTRIP_SERVER] = "NTRIP-сервер",

    [TEXT_RECORD_SPACE] = "Космос:",
    [TEXT_RECORD_NAME] = "Имя:",
    [TEXT_RECORD_TYPE] = "Тип:",
    [TEXT_RECORD_TIME] = "Время:",

    [TEXT_WIFI_MODE] = "Режим:",

		[TEXT_POWEROFF_LABEL_LINUX_OFF] = "выключение питания",
    [TEXT_POWEROFF_LABEL_LOW_BAT_OFF] = "Низкий заряд батареи",
    [TEXT_POWEROFF_LABEL_FORCE_OFF] = "Принудительное\n выключение",
    [TEXT_POWEROFF_LABEL_BUTTON_OFF] = "Кнопка выключения",
    [TEXT_POWEROFF_LABEL_NOTIFY] = "Продолжайте пресс-конференцию.",
		
		[TEXT_HARAWARE_CHECK] = "Потепление:Не отключайтесь сейчас!",
};

#endif

#endif // LANGUAGE_EN_H