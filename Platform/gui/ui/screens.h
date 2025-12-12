#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include "vars.h"
#include <lvgl/lvgl.h>


#ifdef __cplusplus
extern "C" {
#endif

/*Main Page*/
typedef struct battery {
  lv_obj_t *icon_battery;
  lv_obj_t *label_useage;
  lv_style_t style_battery;

  lv_obj_t *label_text;
  lv_obj_t *label_charge_status;
} battery_obj;

typedef struct status_bar {
  /* main_status_bar */
  lv_obj_t *icon_4g;
  lv_obj_t *icon_bluetooth;
  lv_obj_t *icon_wifi;
  lv_obj_t *icon_position;
  lv_obj_t *label_position_t;
  lv_obj_t *icon_radio_status;
  lv_obj_t *icon_sd;

  battery_obj battery;
} status_bar;

typedef struct status_bottom {
  struct satellite {
    lv_obj_t *icon_satellite_big;
    lv_obj_t *label_sat_num;
    lv_obj_t *label_sat_num_real;
  } satellite;

  struct workmode {
    lv_obj_t *button_settings;
    lv_obj_t *icon_settings;
  } workmode;

  struct radio_workmode {
    lv_obj_t *button_radio_workmode;
    lv_obj_t *icon_radio_workmode;
  } radio_workmode;

} status_bottom;

typedef struct page_main {
  lv_obj_t *status_bar_appWindow;
  lv_obj_t *status_bottom_appWindow;
  status_bar status_bar_obj;
  status_bottom status_bottom_obj;

} page_main_t;

/*workConfig Page*/

typedef struct page_workconfig {
  lv_obj_t *button_return;

  lv_obj_t *icon_static;
  lv_obj_t *button_static;

  lv_obj_t *icon_4g;
  lv_obj_t *button_4g;

  lv_obj_t *button_charge;

} page_workconfig_t;

/*readioConfig Page*/

typedef struct page_radioconfig_t {
  lv_obj_t *button_return;

  lv_obj_t *lable_radio_status;

  lv_obj_t *roller_radioFreq;
  lv_obj_t *roller_radioProtocol;

  lv_style_t style_roller;

  lv_obj_t *button_cofirm;
  lv_obj_t *button_close;

} page_radioconfig_t;

typedef struct page_static_settings {
  lv_obj_t *button_static_back;

  lv_obj_t *icon_static;
  lv_obj_t *label_static;
  lv_obj_t *switch_static_onoff;
  lv_obj_t *icon_static_name;
  lv_obj_t *label_static_name;
  lv_obj_t *icon_static_type;
  lv_obj_t *label_static_type;
  lv_obj_t *icon_static_time;
  lv_obj_t *label_static_time;
  lv_obj_t *icon_sd;
  lv_obj_t *label_sd;

} page_static_settings_t;

typedef struct page_network_settings {
  lv_obj_t *button_network_back;

  lv_obj_t *icon_4g;
  lv_obj_t *label_4g_status;
  lv_obj_t *switch_4g_onoff;

  lv_obj_t *ip_ntrip_server;
  lv_obj_t *name_ntrip_server;

} page_network_settings_t;

typedef struct page_batteryInfo {
  lv_obj_t *button_batteryInfo_back;

  lv_obj_t *icon_battery;
  lv_obj_t *label_battery_percent;
  lv_obj_t *label_battery_voltage;
  lv_obj_t *label_battery_temp;

	lv_obj_t *label_charge_current;
	lv_obj_t *label_charge_voltage;
	
	lv_obj_t *label_battery_change_percent;

} page_batteryInfo_t;

/*poweroff_charge Page*/
typedef struct poweroff_charge {
  /* poweroff_charge */
  battery_obj battery;
  lv_obj_t *label_charge_time;

  lv_obj_t *arc_obj;
  lv_obj_t *label_arc_percent;
  lv_anim_t arc_anim;
} poweroff_charge;

/*poweroff page */
typedef struct poweroff {
  lv_obj_t *shutdown_cont;
  lv_obj_t *bar;
  lv_obj_t *label;
  lv_obj_t *percentage_label;

  battery_obj battery;

  lv_anim_t anim;
  bool anim_load;
} poweroff_t;

typedef struct _objects_t {
  /*pages*/
  lv_obj_t *main;
  lv_obj_t *work_config;
  lv_obj_t *radio_config;
  lv_obj_t *static_settings;
  lv_obj_t *network_settings;
  lv_obj_t *batteryInfo;
  lv_obj_t *power_off;
  lv_obj_t *power_off_charge;
  /*pages_objects*/
  page_main_t page_main;

  page_workconfig_t page_work;
  page_radioconfig_t page_radio;

  page_static_settings_t page_static;
  page_network_settings_t page_network;
  page_batteryInfo_t page_batteryInfo;

  poweroff_t page_poweroff;

  poweroff_charge page_poweroff_charge;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
  SCREEN_ID_MAIN = 1,
  SCREEN_ID_WORK_CONFIG,
  SCREEN_ID_RADIO_CONFIG,
  SCREEN_ID_STATIC_SETTINGS,
  SCREEN_ID_NETWORK_SETTINGS,
  SCREEN_ID_BATTERY_INFO,
  SCREEN_ID_POWER_OFF,
  SCREEN_ID_POWER_OFF_CHARGE,
  SCREEN_ID_MAX,
};

void create_screen_main();
void tick_screen_main();

void create_screen_work_config();
void tick_screen_work_config();

void create_screen_radio_config();
void tick_screen_radio_config();

void create_screen_static_settings();
void tick_screen_static_settings();

void create_screen_network_settings();
void tick_screen_network_settings();

void create_screen_poweroff();
void tick_screen_poweroff();

void create_screen_poweroff_charge();
void tick_screen_poweroff_charge();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void init_screen_theme();
void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/