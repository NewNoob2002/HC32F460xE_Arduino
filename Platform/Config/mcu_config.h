#pragma once
#include "mcu_define.h"
#include "../support/support.h"

#define FIRMWARE_NAME "PRO_Panel"
#define AUTHOR_NAME     " "
#define SOFTWARE_VERSION "V2.1.0"
// 2.0 修复X1pro充电问题， 增加强制关机机制防止卡死无法关机
// 2.0.1 增加关机充电提示和开机按键提示动画，增加设备信息页面
#define SOFTWARE_BUILD_DATE __DATE__
#define SOFTWARE_BUILD_TIME __TIME__

/* Number to string macro */
#define _VERSION_NUM_TO_STR_(n)  #n
#define VERSION_NUM_TO_STR(n)   _VERSION_NUM_TO_STR_(n)

/* Compiler Version */
#if defined(_MSC_FULL_VER)
#  define VERSION_COMPILER      "MSVC\nv" VERSION_NUM_TO_STR(_MSC_FULL_VER)
#elif defined(__ARMCC_VERSION)
#  define VERSION_COMPILER      "ARMCCv" VERSION_NUM_TO_STR(__ARMCC_VERSION)
#elif defined(__GNUC__)
#  define VERSION_COMPILER      "GCC"\
                                "v"\
                                VERSION_NUM_TO_STR(__GNUC__)\
                                "."\
                                VERSION_NUM_TO_STR(__GNUC_MINOR__)\
                                "."\
                                VERSION_NUM_TO_STR(__GNUC_PATCHLEVEL__)
                                #else
                                #  define VERSION_COMPILER      "UNKNOW"
                                #endif
/*POWER*/
#define POWER_CONTROL_PIN		PB3
#define WATCHDOG_FEED_PIN		PA6
#define WATCHDOG_FEED_TIME	5000

/*Function Key*/
#define FUNCTION_KEY_PIN PA15

/*power_key_pin*/
#define POWER_KEY_PIN PA0

/*Status LED*/
#define POWER_LED_PIN PC13
#define CHARGE_LED_PIN PH2
#define FUNCTION_LED_PIN PB5

/* SPI Class */
#define SPI_CLASS_3_ENABLE 1
#if SPI_CLASS_3_ENABLE
#define SPI_CLASS_3_SPI CM_SPI3
#endif

/* Screen */
//#define CONFIG_SCREEN_CS_PIN   PH2
//#define CONFIG_SCREEN_DC_PIN   PB8
//#define CONFIG_SCREEN_RST_PIN  PB9
//#define CONFIG_SCREEN_SCK_PIN  PB6
//#define CONFIG_SCREEN_MOSI_PIN PB7
//#define CONFIG_SCREEN_BLK_PIN  PB3 // TIM3
//#define CONFIG_SCREEN_SPI      SPI_3
//#define CONFIG_SCREEN_HOR_RES  320
//#define CONFIG_SCREEN_VER_RES  172
//////////////////////////////////////////
#define CONFIG_SCREEN_CS_PIN   PB14
#define CONFIG_SCREEN_DC_PIN   PB1
#define CONFIG_SCREEN_RST_PIN  PB2
#define CONFIG_SCREEN_SCK_PIN  PB15
#define CONFIG_SCREEN_MOSI_PIN PB13
//#define CONFIG_SCREEN_BLK_PIN  PB3 // TIM3
#define CONFIG_SCREEN_SPI      SPI_3
#define CONFIG_SCREEN_HOR_RES  294
#define CONFIG_SCREEN_VER_RES  126

/* DMA definition */
#define DMA_UNIT                        (CM_DMA1)
#define DMA_CLK                         (FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS)
#define DMA_TX_CH                       (DMA_CH0)
#define DMA_TX_TRIG_CH                  (AOS_DMA1_0)

#define SPI_TX_EVT_SRC                  (EVT_SRC_SPI3_SPTI)