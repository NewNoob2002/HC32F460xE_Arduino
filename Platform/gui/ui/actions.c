#include <stdio.h>
#include "actions.h"
#include "ui.h"
#include "vars.h"
#include "anim.h"
#include "fonts.h"

static uint16_t radio_ch_index = 0;
static uint16_t radio_proc_index = 0;

static arc_completion_callback_t completion_callback = NULL;
static bool callback_triggered = false;

void action_switch_to_screen_by_id(lv_event_t *e) {
  enum ScreensEnum screenId = (enum ScreensEnum)lv_event_get_user_data(e);
  loadScreen(screenId);
}

void action_turn_onoff_static(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    if (lv_obj_has_state(obj, LV_STATE_CHECKED)) {
      systemInfo.recordInfo.record_status = 1;
      systemInfo.recordInfo.record_op = 1;
      systemInfo.panel_operation_flag = 1;
      lv_label_set_text_fmt(objects.page_static.label_static, "Status: ON");
    } else {
      systemInfo.recordInfo.record_status = 0;
      systemInfo.recordInfo.record_op = 1;
      systemInfo.panel_operation_flag = 1;
      lv_label_set_text_fmt(objects.page_static.label_static, "Status: OFF");
    }
  }
}

void action_turn_onoff_4g(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    if (lv_obj_has_state(obj, LV_STATE_CHECKED)) {
      systemInfo.panel_operation_flag = 1;
      systemInfo.work_mode = 1;
      systemInfo.radioInfo.radio_status = 0;
      systemInfo.ntripInfo.gprs_status = 1;
      systemInfo.ntripInfo.NtripClient_status = 0;
      systemInfo.ntripInfo.NtripServer_status = 1;
    } else {
      systemInfo.panel_operation_flag = 1;
      systemInfo.ntripInfo.gprs_status = 0;
      systemInfo.ntripInfo.NtripClient_status = 0;
      systemInfo.ntripInfo.NtripServer_status = 0;
    }
  }
}

lv_obj_t *mbox1 = NULL;

static void msgbox_event_cb(lv_timer_t *timer) { lv_msgbox_close(mbox1); }

void action_radio_roller(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    radio_ch_index = lv_roller_get_selected(obj) + 1;
    char radio_ch_str[16];
    lv_roller_get_selected_str(obj, radio_ch_str, sizeof(radio_ch_str));
    char display_str[32];
    snprintf(display_str, sizeof(display_str), "Set as %s(%d)", radio_ch_str,
             radio_ch_index);
    mbox1 =
        lv_msgbox_create(objects.radio_config, NULL, display_str, NULL, false);
    lv_obj_set_size(mbox1, 250, 100);
    lv_obj_center(mbox1);
    lv_timer_t *timer = lv_timer_create(msgbox_event_cb, 800, NULL);
    lv_timer_set_repeat_count(timer, 1);
  }
}

void action_radio_protocol_roller(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    radio_proc_index = radio_protocol_def[lv_roller_get_selected(obj)];

    char radio_proc_str[16];
    char display_str[32];
    lv_roller_get_selected_str(obj, radio_proc_str, sizeof(radio_proc_str));

    snprintf(display_str, sizeof(display_str), "Set as %s", radio_proc_str);
    mbox1 =
        lv_msgbox_create(objects.radio_config, NULL, display_str, NULL, false);
    lv_obj_set_size(mbox1, 250, 100);
    lv_obj_center(mbox1);

    lv_timer_t *timer = lv_timer_create(msgbox_event_cb, 800, NULL);
    lv_timer_set_repeat_count(timer, 1);
  }
}

void action_radio_set(lv_event_t *e) {
  systemInfo.panel_operation_flag = 1;
  systemInfo.radioInfo.radio_channel = radio_ch_index;
  systemInfo.radioInfo.radio_protocol = radio_proc_index;
  systemInfo.radioInfo.radio_mode = 0;
  systemInfo.radioInfo.radio_status = 1;
  systemInfo.ntripInfo.gprs_status = 0;
  systemInfo.ntripInfo.NtripClient_status = 0;
  systemInfo.ntripInfo.NtripServer_status = 0;
  systemInfo.work_mode = 1;
  loadScreen(SCREEN_ID_MAIN);
}

void action_radio_close(lv_event_t *e) {
  systemInfo.panel_operation_flag = 1;
  systemInfo.radioInfo.radio_mode = 1;
  systemInfo.radioInfo.radio_status = 0;
  systemInfo.ntripInfo.gprs_status = 0;
  systemInfo.ntripInfo.NtripClient_status = 0;
  systemInfo.ntripInfo.NtripServer_status = 0;
  systemInfo.work_mode = 0;
  loadScreen(SCREEN_ID_MAIN);
}

void lv_update_battery(pBatteryInfo_t p_batteryState, battery_obj *battery) {
  uint8_t battery_percentage = p_batteryState->Percent;
  /*battery*/
  lv_label_set_text_fmt(battery->label_text, "%d%%", battery_percentage);

  bool Is_BattCharging = p_batteryState->chargeStatus != 0x00;
  bool is_FullCharge = (battery_percentage >= 100);
  lv_obj_t *battery_useage = battery->label_useage;

  lv_coord_t width = lv_map(battery_percentage, 0, 100, 0, BATT_USAGE_WIDTH);
  if (Is_BattCharging && !is_FullCharge) {
    lv_style_set_bg_color(&battery->style_battery, lv_color_hex(0x4CAF50));
    lv_style_set_text_color(&battery->style_battery, lv_color_hex(0x4CAF50));
    lv_obj_set_width(battery_useage, width);
  } else {

    lv_color_t battery_color;
    if (battery_percentage > 50) {
      battery_color = lv_color_hex(0x4CAF50); // 绿色
    } else if (battery_percentage > 20 && battery_percentage <= 50) {
      battery_color = lv_color_hex(0xFF9800); // 橙色
    } else if (battery_percentage <= 20) {
      battery_color = lv_color_hex(0xF44336); // 红色
    }
    lv_style_set_bg_color(&battery->style_battery, battery_color);
    lv_style_set_text_color(&battery->style_battery, battery_color);
    lv_obj_set_width(battery_useage, width);
  }
}

void lv_update_battery_off(pBatteryInfo_t p_batteryState, battery_obj *battery) {
  uint8_t battery_percentage = p_batteryState->Percent;
  /*battery*/
  lv_label_set_text_fmt(battery->label_text, "%d%%", battery_percentage);

  bool Is_BattCharging =
      p_batteryState->chargeStatus == 1 || p_batteryState->chargeStatus == 2;
  bool is_FullCharge = (battery_percentage >= 100);
  lv_obj_t *battery_useage = battery->label_useage;
  static bool Is_BattChargingAnimActive = false;
  if (Is_BattCharging && !is_FullCharge) {
    if (p_batteryState->chargeStatus == 1) {
      lv_label_set_text(battery->label_charge_status, "O");
    } else if (p_batteryState->chargeStatus == 2) {
      lv_label_set_text(battery->label_charge_status, "F");
    }
    if (!Is_BattChargingAnimActive) {
      lv_style_set_bg_color(&battery->style_battery, lv_color_hex(0x4CAF50));
      lv_style_set_text_color(&battery->style_battery, lv_color_hex(0x4CAF50));
      StatusBar_AnimCreate(battery_useage);
      Is_BattChargingAnimActive = true;
    }
  } else {
    if (Is_BattChargingAnimActive) {
      lv_anim_del(battery_useage, NULL);
      StatusBar_ConBattSetOpa(battery_useage, LV_OPA_COVER);
      Is_BattChargingAnimActive = false;
    }
    lv_label_set_text(battery->label_charge_status, "N");
    lv_coord_t width = lv_map(battery_percentage, 0, 100, 0, BATT_USAGE_WIDTH);

    lv_color_t battery_color;
    if (battery_percentage > 50) {
      battery_color = lv_color_hex(0x4CAF50); // 绿色
    } else if (battery_percentage > 20 && battery_percentage <= 50) {
      battery_color = lv_color_hex(0xFF9800); // 橙色
    } else if (battery_percentage <= 20) {
      battery_color = lv_color_hex(0xF44336); // 红色
    }
    lv_style_set_bg_color(&battery->style_battery, battery_color);
    lv_style_set_text_color(&battery->style_battery, battery_color);
    lv_obj_set_width(battery_useage, width);
  }
}

static void page_mainTopUpdate(status_bar *bar) {
  // 4G_Icon
  if (systemInfo.ntripInfo.gprs_status)
    lv_obj_set_style_text_color(bar->icon_4g,
                                lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
  else
    lv_obj_set_style_text_color(bar->icon_4g, lv_palette_main(LV_PALETTE_GREY),
                                0);
  // Wifi
  if (systemInfo.wifiInfo.wifi_status) {
    lv_obj_set_style_text_color(bar->icon_wifi,
                                lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
    lv_obj_set_style_text_color(bar->icon_bluetooth,
                                lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
  } else {
    if (systemInfo.eg25_overtime) {
      lv_obj_set_style_text_color(bar->icon_wifi,
                                  lv_palette_main(LV_PALETTE_RED), 0);
      lv_obj_set_style_text_color(bar->icon_bluetooth,
                                  lv_palette_main(LV_PALETTE_RED), 0);
    } else {
      lv_obj_set_style_text_color(bar->icon_wifi,
                                  lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_set_style_text_color(bar->icon_bluetooth,
                                  lv_palette_main(LV_PALETTE_GREY), 0);
    }
  }
  // Position
  if (systemInfo.work_mode == base_mode ||
      systemInfo.work_mode == autobase_mode) {
    lv_obj_set_style_text_color(bar->icon_position,
                                lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_label_set_text(bar->label_position_t, "BASE");
  } else {
    switch (systemInfo.positionInfo.coordinate_status) {
    case 0: // NONE
      lv_obj_set_style_text_color(bar->icon_position,
                                  lv_palette_main(LV_PALETTE_RED), 0);
      lv_label_set_text(bar->label_position_t, "NONE");
      break;
    case 1: // Single
      lv_obj_set_style_text_color(bar->icon_position,
                                  lv_palette_main(LV_PALETTE_YELLOW), 0);
      lv_label_set_text(bar->label_position_t, "SINGLE");
      break;
    case 4: // FIX
      lv_obj_set_style_text_color(bar->icon_position,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_label_set_text(bar->label_position_t, "FIX");
      break;
    case 5: // FLOAT
      lv_obj_set_style_text_color(bar->icon_position,
                                  lv_palette_main(LV_PALETTE_YELLOW), 0);
      lv_label_set_text(bar->label_position_t, "FLOAT");
      break;
    case 7:
      lv_obj_set_style_text_color(bar->icon_position,
                                  lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_label_set_text(bar->label_position_t, "BASE");
      break;
    }
  }
  if (systemInfo.radioInfo.radio_status) {
    lv_obj_set_style_text_color(bar->icon_radio_status,
                                lv_palette_main(LV_PALETTE_GREEN), 0);
  } else {
    lv_obj_set_style_text_color(bar->icon_radio_status,
                                lv_palette_main(LV_PALETTE_RED), 0);
  }
  // SD
  if (systemInfo.recordInfo.record_status) {
    lv_obj_set_style_text_color(bar->icon_sd,
                                lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
  } else {
    lv_obj_set_style_text_color(bar->icon_sd, lv_palette_main(LV_PALETTE_GREY),
                                0);
  }
}
void Page_Main_Update() {
  // TOP Update
  page_mainTopUpdate(&objects.page_main.status_bar_obj);
  // satallite update
  lv_label_set_text_fmt(
      objects.page_main.status_bottom_obj.satellite.label_sat_num, "%d",
      systemInfo.positionInfo.satellite_number_used);
  lv_label_set_text_fmt(
      objects.page_main.status_bottom_obj.satellite.label_sat_num_real, "/ %d",
      systemInfo.positionInfo.satellite_number_track);

  switch (systemInfo.work_mode) {
  case rover_mode:
  case single_mode:
    lv_label_set_text(
        objects.page_main.status_bottom_obj.workmode.icon_settings,
        LV_SYMBOL_ROVER);
    break;
  case base_mode:
  case autobase_mode:
    lv_label_set_text(
        objects.page_main.status_bottom_obj.workmode.icon_settings,
        LV_SYMBOL_BASE);
    break;
  }
}

void Page_RadioConfig_Update() {
  // Update radio status
  if (systemInfo.radioInfo.radio_status) {
    lv_obj_set_style_text_color(objects.page_radio.lable_radio_status,
                                lv_palette_main(LV_PALETTE_GREEN), 0);
  } else {
    lv_obj_set_style_text_color(objects.page_radio.lable_radio_status,
                                lv_palette_main(LV_PALETTE_RED), 0);
  }
}

void Page_StaticRecord_Update() {
  if (systemInfo.recordInfo.record_status) {
    if (!lv_obj_has_state(objects.page_static.switch_static_onoff,
                          LV_STATE_CHECKED)) {
      lv_obj_add_state(objects.page_static.switch_static_onoff,
                       LV_STATE_CHECKED);
    }
    lv_obj_set_style_text_color(objects.page_static.icon_static,
                                lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_static_name,
                                lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_sd,
                                lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_static_type,
                                lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_static_time,
                                lv_palette_main(LV_PALETTE_BLUE), 0);

    lv_label_set_text_fmt(objects.page_static.label_static_name, "REC: %s",
                          systemInfo.recordInfo.record_name);
    lv_label_set_text_fmt(objects.page_static.label_static_type, "Type: %s",
                          systemInfo.recordInfo.record_type == 1 ? "XYZ" : "Rinex3.02");
    lv_label_set_text_fmt(objects.page_static.label_static_time,
                          "Time: %d hour", systemInfo.recordInfo.record_interval);
    lv_label_set_text_fmt(objects.page_static.label_sd, "RSC: %0.1fM",
                          systemInfo.recordInfo.record_leftspace);
  } else {
    if (lv_obj_has_state(objects.page_static.switch_static_onoff,
                         LV_STATE_CHECKED)) {
      lv_obj_clear_state(objects.page_static.switch_static_onoff,
                         LV_STATE_CHECKED);
    }
    lv_obj_set_style_text_color(objects.page_static.icon_static,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_static_name,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_static_type,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_static_time,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_text_color(objects.page_static.icon_sd,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(objects.page_static.label_static_name, "REC: -");
    lv_label_set_text(objects.page_static.label_static_type, "Type: -");
    lv_label_set_text(objects.page_static.label_static_time, "Time: -");
    lv_label_set_text_fmt(objects.page_static.label_sd, "RSC: -");
  }
}

void Page_NtripServer_Update() {
  // 4G_Icon
  if (systemInfo.ntripInfo.gprs_status) {
    lv_obj_set_style_text_color(objects.page_network.icon_4g,
                                lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
    lv_label_set_text(objects.page_network.label_4g_status, "-ON");
    lv_obj_add_state(objects.page_network.switch_4g_onoff, LV_STATE_CHECKED);
  } else {
    lv_obj_set_style_text_color(objects.page_network.icon_4g,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(objects.page_network.label_4g_status, "-OFF");
    lv_obj_clear_state(objects.page_network.switch_4g_onoff, LV_STATE_CHECKED);
  }

  if (systemInfo.ntripInfo.NtripServer_status) {
    lv_label_set_text(objects.page_network.ip_ntrip_server,
                      systemInfo.ntripInfo.NtripServer_IP);
    lv_label_set_text(objects.page_network.name_ntrip_server,
                      systemInfo.ntripInfo.NtripServer_Mountpoint);
  } else {
    lv_label_set_text(objects.page_network.ip_ntrip_server, "-");
    lv_label_set_text(objects.page_network.name_ntrip_server, "-");
  }
}

void start_animation_to_value(lv_obj_t *arc, lv_anim_t *anim,
                              int32_t target_value) {
  int32_t current_value = lv_arc_get_value(arc);

  if (current_value == target_value) {
    return;
  }

  // 设置动画参数
  lv_anim_set_values(anim, current_value, target_value);

  uint32_t time = 200;

  lv_anim_set_time(anim, time);
  lv_anim_start(anim);
}

void arc_angle_anim(void *obj, int32_t v) {
  lv_arc_set_value((lv_obj_t *)obj, v);
  lv_label_set_text_fmt(objects.page_poweroff_charge.label_arc_percent, "%d%%",
                        v);
  if (v >= 95) {
    lv_obj_set_style_arc_color(
        (lv_obj_t *)obj, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    if (v >= 100) {
      if (completion_callback != NULL && !callback_triggered) {
        callback_triggered = true;
        completion_callback();
      }
    }
  } else {
    lv_obj_set_style_arc_color((lv_obj_t *)obj, lv_palette_main(LV_PALETTE_RED),
                               LV_PART_INDICATOR);
    callback_triggered = false;
  }
}

void set_arc_completed_callback(arc_completion_callback_t func) {
  completion_callback = func;
}

void set_poweroff_reason_str(const char *str) {
  lv_label_set_text(objects.page_poweroff.label, str);
}

void add_objs_to_group(lv_obj_t *parent, lv_group_t *group) {
  if (parent == NULL)
    return;

  uint32_t child_cnt = lv_obj_get_child_cnt(parent);

  for (uint32_t i = 0; i < child_cnt; i++) {
    lv_obj_t *child = lv_obj_get_child(parent, i);

    if (lv_obj_check_type(child, &lv_roller_class) || // roller
        lv_obj_check_type(child, &lv_btn_class) ||    // Button
        lv_obj_check_type(child, &lv_switch_class)    // Switch
    ) {

      lv_group_add_obj(group, child);
    }

    add_objs_to_group(child, group);
  }
}

void lv_focus_init(lv_obj_t *screen) {
  lv_group_t *defualt_group = lv_group_get_default();
  lv_group_remove_all_objs(defualt_group);
}