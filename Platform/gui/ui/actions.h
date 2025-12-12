#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl/lvgl.h>
#include "screens.h"
#include "vars.h"
#include "mcu_config.h"
#ifdef __cplusplus
extern "C" {
#endif

extern void action_switch_to_screen_by_id(lv_event_t *e);
extern void action_turn_onoff_static(lv_event_t *e);
extern void action_turn_onoff_4g(lv_event_t *e);
	
extern void action_radio_roller(lv_event_t *e) ;
extern void action_radio_protocol_roller(lv_event_t *e);
extern void action_radio_set(lv_event_t *e);
extern void action_radio_close(lv_event_t *e);

extern void lv_update_battery(pBatteryInfo_t p_batteryState, battery_obj *battery);
extern void lv_update_battery_off(pBatteryInfo_t p_batteryState, battery_obj *battery);
extern void Page_Main_Update();
	
extern void Page_RadioConfig_Update();

extern void Page_StaticRecord_Update();

extern void Page_NtripServer_Update();

extern void start_animation_to_value(lv_obj_t *arc, lv_anim_t *anim, int32_t target_value);
extern void arc_angle_anim(void * obj, int32_t v);
extern void set_arc_completed_callback(arc_completion_callback_t func);

extern void set_poweroff_reason_str(const char* str);

extern void lv_focus_init(lv_obj_t *screen);
#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/