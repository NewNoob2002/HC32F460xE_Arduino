#ifndef MY_SYMBOL_H__
#define MY_SYMBOL_H__

#include "lvgl.h"

LV_FONT_DECLARE(main_symbol);
LV_FONT_DECLARE(main_symbol2);
LV_FONT_DECLARE(symbol_satellite);
LV_FONT_DECLARE(symbol_radio);

#if !defined LV_SYMBOL_SATELLITE
#define LV_SYMBOL_SATELLITE           "\xEE\x9D\x9F" /*, 0xE8B8*/
#endif

#if !defined LV_SYMBOL_BASE
#define LV_SYMBOL_BASE                "\xEE\xAD\xA9" /*, 0xe6d6*/
#endif
#if !defined LV_SYMBOL_ROVER
#define LV_SYMBOL_ROVER           "\xEE\x98\x8D" /*, 0xec4a*/
#endif

#if !defined LV_SYMBOL_RADIO_TRANSMITTER
#define LV_SYMBOL_RADIO_TRANSMITTER                "\xEE\x99\x97" /*, 0xe894*/
#endif

#if !defined LV_SYMBOL_RADIO_RECEIVER
#define LV_SYMBOL_RADIO_RECEIVER             "\xEE\x9C\x93" /*, 0xe88b*/
#endif

#if !defined LV_SYMBOL_FREQUENCE
#define LV_SYMBOL_FREQUENCE                     "\xEE\x98\x93" /*, 0xec4a*/
#endif

#if !defined LV_SYMBOL_INFORMATION
#define LV_SYMBOL_INFORMATION                     "\xEE\x9D\xBD" /*, 0xec4a*/
#endif

#if !defined LV_SYMBOL_4G
#define LV_SYMBOL_4G                     "\xEE\xAC\x87" /*, 0xec4a*/
#endif

#if !defined LV_SYMBOL_STATIC
#define LV_SYMBOL_STATIC                     "\xEE\x9E\x93" /*, 0xec4a*/
#endif

#if !defined LV_SYMBOL_RUNTIME
#define LV_SYMBOL_RUNTIME                     "\xEE\x9C\xBE" /*, 0xec
#endif

#endif // __LVGL_SCREEN_H__
