#if defined(EEZ_FOR_LVGL)
#include <eez/core/vars.h>
#endif

#include "ui.h"
#include "screens.h"
#include "images.h"
#include "actions.h"
#include "vars.h"

#if defined(EEZ_FOR_LVGL)

void ui_init() {
    eez_flow_init(assets, sizeof(assets), (lv_obj_t **)&objects, sizeof(objects), images, sizeof(images), actions);
}

void ui_tick() {
    eez_flow_tick();
    tick_screen(g_currentScreen);
}

#else

static int16_t currentScreen = -1;

static lv_obj_t *getLvglObjectFromIndex(int32_t index) {
    if (index == -1) {
        return 0;
    }
    return ((lv_obj_t **)&objects)[index];
}

void loadScreen(enum ScreensEnum screenId) {
    currentScreen = screenId - 1;
    lv_obj_t *screen = getLvglObjectFromIndex(currentScreen);
//		if(screenId == SCREEN_ID_MAIN)
//		{
//			lv_group_t *defualt_group = lv_group_get_default();
//			lv_group_remove_all_objs(defualt_group);
//			lv_group_set_wrap(defualt_group, true);
//			lv_group_add_obj(defualt_group, objects.page_main.status_bottom_obj.workmode.button_settings);
//			lv_group_add_obj(defualt_group, objects.page_main.status_bottom_obj.radio_workmode.button_radio_workmode);
//			
//			lv_group_focus_obj(objects.page_main.status_bottom_obj.workmode.button_settings);
//		}
//		else if(screenId == SCREEN_ID_WORK_CONFIG)
//		{
//			lv_group_t *defualt_group = lv_group_get_default();
//			lv_group_remove_all_objs(defualt_group);
//			lv_group_set_wrap(defualt_group, true);
//			
//			lv_group_add_obj(defualt_group, objects.page_work.button_return);
//			lv_group_add_obj(defualt_group, objects.page_work.button_static);
//			lv_group_add_obj(defualt_group, objects.page_work.button_4g);
//			lv_group_add_obj(defualt_group, objects.page_work.button_charge);
//			
//			lv_group_focus_obj(objects.page_work.button_return);
//		}
//		else if(screenId == SCREEN_ID_RADIO_CONFIG)
//		{
//			lv_group_t *defualt_group = lv_group_get_default();
//			lv_group_remove_all_objs(defualt_group);
//			lv_group_set_wrap(defualt_group, true);
//			
//			lv_group_add_obj(defualt_group, objects.page_radio.button_return);
//			lv_group_add_obj(defualt_group, objects.page_radio.roller_radioFreq);
//			lv_group_add_obj(defualt_group, objects.page_radio.roller_radioProtocol);
//			lv_group_add_obj(defualt_group, objects.page_radio.button_cofirm);
//			lv_group_add_obj(defualt_group, objects.page_radio.button_close);
//			
//			lv_group_focus_obj(objects.page_radio.button_return);
//		}
//		else if(screenId == SCREEN_ID_STATIC_SETTINGS)
//		{
//			lv_group_t *defualt_group = lv_group_get_default();
//			lv_group_remove_all_objs(defualt_group);
//			lv_group_set_wrap(defualt_group, true);
//			
//			lv_group_add_obj(defualt_group, objects.page_static.button_static_back);
//			lv_group_add_obj(defualt_group, objects.page_static.switch_static_onoff);
//			
//			lv_group_focus_obj(objects.page_static.button_static_back);
//		}
//		else if(screenId == SCREEN_ID_NETWORK_SETTINGS)
//		{
//			lv_group_t *defualt_group = lv_group_get_default();
//			lv_group_remove_all_objs(defualt_group);
//			lv_group_set_wrap(defualt_group, true);
//			
//			lv_group_add_obj(defualt_group, objects.page_network.button_network_back);
//			lv_group_add_obj(defualt_group, objects.page_network.switch_4g_onoff);
//			
//			lv_group_focus_obj(objects.page_network.button_network_back);
//		}
//		else if(screenId == SCREEN_ID_BATTERY_INFO)
//		{
//			lv_group_t *defualt_group = lv_group_get_default();
//			lv_group_remove_all_objs(defualt_group);
//			lv_group_set_wrap(defualt_group, true);
//			
//			lv_group_add_obj(defualt_group, objects.page_batteryInfo.button_batteryInfo_back);
//			
//			lv_group_focus_obj(objects.page_batteryInfo.button_batteryInfo_back);
//		}
//		lv_scr_load(screen);
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
}

void ui_init() {
    create_screens();
    loadScreen(SCREEN_ID_POWER_OFF_CHARGE);
}

void ui_tick() {
		lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_HIDDEN);
    tick_screen(currentScreen);
}

#endif
