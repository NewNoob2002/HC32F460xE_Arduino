#pragma once
#include "mcu_define.h"

#define  HW_VERSION    "V1.4"
#define  SW_VERSION    "V1.27"

/*POWER*/
#define POWER_CONTROL_PIN  PB3
#define WATCHDOG_FEED_PIN  PA6
#define WATCHDOG_FEED_TIME 5000

/*Function Key*/
#define FUNCTION_KEY_PIN PA15

/*power_key_pin*/
#define POWER_KEY_PIN PA0

/*Status LED*/
#define POWER_LED_PIN    PC13
#define CHARGE_LED_PIN   PH2
#define FUNCTION_LED_PIN PB5

/*charger_control*/
#define CHARGER_ENABLE_PIN 		PA4
#define CHARGER_CTRL_PIN 			PB7
#define CHARGER_CTRL_FAST_PIN PB6
#define CHARGER_SWITCH_PIN 		PB10

#define CHARGER_ADC_DETECT_PIN PA1

#define USB_SWITCH_PIN PB8

#define STABLE_TIME_MS         (20 * 60 * 1000)
#define FLASH_OFFSET_ADDR      0x0007A000