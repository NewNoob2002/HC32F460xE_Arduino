#ifndef __LVGL_SCREEN_H__
#define __LVGL_SCREEN_H__

#include "lvgl.h"

#ifndef nullptr
#define nullptr 0
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	
extern lv_style_t* RootDefaultStyle;
#define SCREEN_PAGE_NUM 6
void LVGL_screen_init();
extern int screen_destition;
	
#include "HAL/HAL.h"
#include "lv_screen1_main.h"
#include "lv_screen2_workstatus.h"
#include "lv_screen3_coordinate.h"
#include "lv_screen4_record.h"
#include "lv_screen5_wifi.h"
#include "lv_screen6_startUp.h"
#include "lv_screen7_poweroff.h"
#include "lv_screen8_hardware.h"
#include "screen1_top.h"
#include "screen1_bottom.h"

#include "lv_obj_ext_func.h"
#include "lv_anim_label.h"
#include "lv_anim_timeline_wrapper.h"
#include "myFont.h"
#include "myImage.h"
#include "mcu_config.h"
#include "core_debug.h"

#ifdef __cplusplus
}
#endif

#define LV_FONT_EXTERNAL 1

#if LV_FONT_EXTERNAL
LV_FONT_DECLARE(lv_font_singularxyz_22)
#endif
#if LV_FONT_EXTERNAL
LV_FONT_DECLARE(lv_font_singularxyz_28)
#endif
#if LV_FONT_EXTERNAL
LV_FONT_DECLARE(lv_font_singularxyz_32)
#endif
#if LV_FONT_EXTERNAL
LV_FONT_DECLARE(lv_font_singularxyz_48)
#endif
#if LV_FONT_EXTERNAL
LV_FONT_DECLARE(lv_font_singularxyz_53)
#endif

LV_FONT_DECLARE(lv_workstatus_icon)
LV_FONT_DECLARE(icon_system_26);


#if !defined LV_SYMBOL_MOBILE
#define LV_SYMBOL_MOBILE           "\xEE\x98\x80" /*, 0xE600*/
#endif
#if !defined LV_SYMBOL_RECORD
#define LV_SYMBOL_RECORD           "\xEE\x98\x84" /*, 0xE604*/
#endif
#if !defined LV_SYMBOL_ROUTER
#define LV_SYMBOL_ROUTER           "\xEE\x98\x97" /*, 0xE617*/
#endif
#if !defined LV_SYMBOL_TRANS
#define LV_SYMBOL_TRANS           "\xEE\x99\x80" /*, 0xE640*/
#endif
#if !defined LV_SYMBOL_COORDINATE
#define LV_SYMBOL_COORDINATE           "\xEE\x9A\xA2" /*, 0xE6A2*/
#endif
#if !defined LV_SYMBOL_INTERCOM
#define LV_SYMBOL_INTERCOM           "\xEE\x99\xBE" /*, 0xE67E*/
#endif

#if !defined LV_SYMBOL_SATELLITE
#define LV_SYMBOL_SATELLITE           "\xEE\x98\x86" /*, 0xE606*/
#endif
#if !defined LV_SYMBOL_ROVER
#define LV_SYMBOL_ROVER          "\xEE\x99\xB5" /*, 0xE675*/
#endif
#if !defined LV_SYMBOL_VERSION
#define LV_SYMBOL_VERSION          "\xEE\x9A\x95" /*, 0xE695*/
#endif
#if !defined LV_SYMBOL_POSITION
#define LV_SYMBOL_POSITION          "\xEE\x98\x8B" /*, 0xE60B*/
#endif
#if !defined LV_SYMBOL_BASE
#define LV_SYMBOL_BASE          "\xEE\x9C\x97" /*, 0xE717*/
#endif
#if !defined LV_SYMBOL_POWER_FULL
#define LV_SYMBOL_POWER_FULL          "\xEE\x9A\xBD" /*, 0xE6BD*/
#endif
#if !defined LV_SYMBOL_BRIDGING
#define LV_SYMBOL_BRIDGING          "\xEE\x9D\x8C" /*, 0xE74C*/
#endif
#if !defined LV_SYMBOL_RADIO
#define LV_SYMBOL_RADIO          "\xEE\x9A\x88" /*, 0xE688*/
#endif
#if !defined LV_SYMBOL_IMU
#define LV_SYMBOL_IMU          "\xEE\x98\xB2" /*, 0xE632*/
#endif
#if !defined LV_SYMBOL_LTE
#define LV_SYMBOL_LTE          "\xEE\x98\xBF" /*58943, 0xE63F*/
#endif

#if !defined LV_SYMBOL_RUNTIME
#define LV_SYMBOL_RUNTIME          "\xEE\x9C\xBE" /*EE9CBE*/
#endif

#if !defined LV_SYMBOL_COMPILE
#define LV_SYMBOL_COMPILE          "\xEE\xA7\x9F" /*EEA79F*/
#endif

#if !defined LV_SYMBOL_FIRMWARE
#define LV_SYMBOL_FIRMWARE          "\xEE\x98\x80" /*EE9880*/
#endif

#if !defined LV_SYMBOL_COPYTIGHT
#define LV_SYMBOL_COPYTIGHT          "\xEE\xA7\x88" /*EEA788*/
#endif

#if !defined LV_SYMBOL_workstatus1
#define LV_SYMBOL_workstatus1          "\xEE\x98\x80" 
#endif
#if !defined LV_SYMBOL_workstatus2
#define LV_SYMBOL_workstatus2          "\xEE\x9A\xB4" 
#endif
#if !defined LV_SYMBOL_workstatus3
#define LV_SYMBOL_workstatus3          "\xEE\x9D\x8C" 
#endif
#if !defined LV_SYMBOL_workstatus4
#define LV_SYMBOL_workstatus4          "\xEE\x9B\x87" 
#endif
#if !defined LV_SYMBOL_workstatus5
#define LV_SYMBOL_workstatus5          "\xEE\x98\xBB" 
#endif

extern int screen_flag;
extern int last_screen_flag;
extern int screen_flag_change;
extern int workstatus_change;

extern int workstatus; 
extern int last_workstatus;
extern int work_status_enable;
#endif
