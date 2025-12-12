#include <string.h>
#include <stdio.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "anim.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#if defined(HC32F460)
#include "mcu_config.h"
#include "delay.h"
#endif

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

const uint8_t radio_protocol_def[6] = {1, 2, 4, 5, 9, 26};

void create_screen_main()
{
  lv_obj_t *main_screen = lv_obj_create(0);
  objects.main = main_screen;
  lv_obj_set_pos(main_screen, 0, 0);
  lv_obj_set_size(main_screen, 294, 126);
  lv_obj_clear_flag(main_screen, LV_OBJ_FLAG_SCROLLABLE);
  {
    lv_obj_t *main_screen_obj = main_screen;
    {
      lv_obj_t *main_status_bar = lv_obj_create(main_screen_obj);
      lv_obj_remove_style_all(main_status_bar);
      lv_obj_set_size(main_status_bar, 294, 36);
      lv_obj_clear_flag(main_status_bar, LV_OBJ_FLAG_SCROLLABLE);
      objects.page_main.status_bar_appWindow = main_status_bar;
      {
        lv_obj_t *main_status_bar_obj = main_status_bar;
        {
          lv_obj_t *icon_4g = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_4g, LV_SYMBOL_4G);
          lv_obj_set_style_text_font(icon_4g, &main_symbol2, 0);
          lv_obj_align(icon_4g, LV_ALIGN_TOP_LEFT, 15, 10);
          lv_obj_set_style_text_color(
              icon_4g, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_4g = icon_4g;
        }
        {
          lv_obj_t *icon_bluetooth = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_bluetooth, LV_SYMBOL_BLUETOOTH);
          lv_obj_set_style_text_font(icon_bluetooth, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_bluetooth, LV_ALIGN_TOP_LEFT, 45, 10);
          lv_obj_set_style_text_color(
              icon_bluetooth, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_bluetooth = icon_bluetooth;
        }
        {
          lv_obj_t *icon_wifi = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);
          lv_obj_set_style_text_font(icon_wifi, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_wifi, LV_ALIGN_TOP_LEFT, 70, 10);
          lv_obj_set_style_text_color(
              icon_wifi, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_wifi = icon_wifi;
        }
        {
          lv_obj_t *icon_position = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_position, LV_SYMBOL_GPS);
          lv_obj_set_style_text_font(icon_position, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_position, LV_ALIGN_TOP_LEFT, 100, 10);
          lv_obj_set_style_text_color(
              icon_position, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_position = icon_position;

          lv_obj_t *label_position_t = lv_label_create(main_status_bar_obj);
          lv_label_set_text(label_position_t, "FLOAT");
          lv_obj_set_style_text_font(label_position_t, &Oswald_Bold_12,
                                     0);
          lv_obj_align_to(label_position_t, icon_position,
                          LV_ALIGN_OUT_RIGHT_MID, 5, 0);
          lv_obj_set_style_text_color(label_position_t, lv_color_hex(0xffffff),
                                      0);

          objects.page_main.status_bar_obj.label_position_t = label_position_t;
        }

        {
          lv_obj_t *icon_radio_status = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_radio_status, LV_SYMBOL_RADIO_TRANSMITTER);
          lv_obj_set_style_text_font(icon_radio_status, &main_symbol,
                                     0);
          lv_obj_align(icon_radio_status, LV_ALIGN_TOP_LEFT, 167, 5);
          lv_obj_set_style_text_color(
              icon_radio_status, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_radio_status = icon_radio_status;
        }

        {
          lv_obj_t *icon_sd = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_sd, LV_SYMBOL_SD_CARD);
          lv_obj_set_style_text_font(icon_sd, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_sd, LV_ALIGN_TOP_LEFT, 205, 10);
          lv_obj_set_style_text_color(icon_sd, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_sd = icon_sd;
        }

        {
          lv_style_init(&objects.page_main.status_bar_obj.battery.style_battery);

          lv_obj_t *img = lv_label_create(main_status_bar_obj);
          lv_label_set_recolor(img, true);
          lv_obj_align(img, LV_ALIGN_TOP_LEFT, 225, 10);
          lv_label_set_text(img, LV_SYMBOL_BATTERY_EMPTY);
          lv_obj_set_style_text_font(img, &lv_font_montserrat_18, 0);
          lv_obj_add_style(img, &objects.page_main.status_bar_obj.battery.style_battery, 0);

          lv_obj_t *useage = lv_obj_create(img);
          lv_obj_remove_style_all(useage);
          lv_obj_set_style_bg_opa(useage, LV_OPA_COVER, 0);
          lv_obj_set_style_opa(useage, LV_OPA_COVER, 0);
          lv_obj_set_size(useage, BATT_USAGE_WIDTH, BATT_USAGE_HEIGHT);
          lv_obj_align_to(useage, img, LV_ALIGN_LEFT_MID, 2, 0);
          lv_obj_add_style(useage, &objects.page_main.status_bar_obj.battery.style_battery, 0);

          objects.page_main.status_bar_obj.battery.label_useage = useage;
          objects.page_main.status_bar_obj.battery.icon_battery = img;

          lv_style_set_text_color(
              &objects.page_main.status_bar_obj.battery.style_battery,
              lv_color_hex(0xF44336));

          lv_obj_t *label = lv_label_create(main_status_bar_obj);
          lv_label_set_text(label, "50%");
          lv_obj_set_style_text_font(label, &Oswald_Bold_12, 0);
          lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
          lv_obj_align_to(label, img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

          objects.page_main.status_bar_obj.battery.label_text = label;
        }
      }
    }
    {
      lv_obj_t *main_status_bottom = lv_obj_create(main_screen_obj);
      lv_obj_remove_style_all(main_status_bottom);
      lv_obj_set_size(main_status_bottom, 294, 90);
      lv_obj_align(main_status_bottom, LV_ALIGN_BOTTOM_MID, 0, 0);
      lv_obj_clear_flag(main_status_bottom, LV_OBJ_FLAG_SCROLLABLE);
      objects.page_main.status_bottom_appWindow = main_status_bottom;

      lv_obj_t *main_status_bottom_obj = main_status_bottom;
      {
        // satellite
        lv_obj_t *icon_satellite_big = lv_label_create(main_status_bottom_obj);
        lv_label_set_text(icon_satellite_big, LV_SYMBOL_SATELLITE);
        lv_obj_set_style_text_font(icon_satellite_big, &symbol_satellite, 0);
        lv_obj_set_style_text_color(icon_satellite_big,
                                    lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
        lv_obj_align(icon_satellite_big, LV_ALIGN_LEFT_MID, 10, 0);

        objects.page_main.status_bottom_obj.satellite.icon_satellite_big =
            icon_satellite_big;
      }

      {
        lv_obj_t *cont_satellite = lv_obj_create(main_status_bottom_obj);
        lv_obj_set_size(cont_satellite, 90, 50);
        lv_obj_clear_flag(cont_satellite, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_align(cont_satellite, LV_ALIGN_LEFT_MID, 63, 0);
        // satellite_num
        lv_obj_t *label_satellite_num = lv_label_create(cont_satellite);
        lv_label_set_text(label_satellite_num, "0");
        lv_obj_set_style_text_font(label_satellite_num, &Rajdhani_Bold_0_9_40, 0);
        lv_obj_set_style_text_color(label_satellite_num, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_align(label_satellite_num, LV_ALIGN_LEFT_MID, -10, 0);

        objects.page_main.status_bottom_obj.satellite.label_sat_num = label_satellite_num;

        // satellite_num_real
        lv_obj_t *label_satellite_num_real = lv_label_create(cont_satellite);
        lv_label_set_text(label_satellite_num_real, "/ 0");
        lv_obj_set_style_text_font(label_satellite_num_real, &Rajdhani_Bold_0_9_20, 0);
        lv_obj_set_style_text_color(label_satellite_num_real,
                                    lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_align_to(label_satellite_num_real, label_satellite_num, LV_ALIGN_OUT_RIGHT_MID, 20, 10);

        objects.page_main.status_bottom_obj.satellite.label_sat_num_real = label_satellite_num_real;
      }

      {
        // button_settings
        lv_obj_t *button_settings = lv_btn_create(main_status_bottom_obj);
        lv_obj_align(button_settings, LV_ALIGN_LEFT_MID, 165, 0);
        lv_obj_set_size(button_settings, 50, 50);

        objects.page_main.status_bottom_obj.workmode.button_settings = button_settings;
        lv_obj_add_event_cb(button_settings, action_switch_to_screen_by_id,
                            LV_EVENT_PRESSED, (void *)SCREEN_ID_WORK_CONFIG);
        // icon_workMode
        {
          lv_obj_t *parent_obj = button_settings;
          lv_obj_t *icon_workMode = lv_label_create(parent_obj);
          lv_label_set_text(icon_workMode, LV_SYMBOL_BASE);
          lv_obj_set_style_text_font(icon_workMode, &main_symbol, 0);
          lv_obj_set_style_align(icon_workMode, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          objects.page_main.status_bottom_obj.workmode.icon_settings = icon_workMode;
        }
      }

      {
        // button_radioconfig
        lv_obj_t *button_radioconfig = lv_btn_create(main_status_bottom_obj);
        lv_obj_align(button_radioconfig, LV_ALIGN_LEFT_MID, 225, 0);
        lv_obj_set_size(button_radioconfig, 50, 50);

        objects.page_main.status_bottom_obj.radio_workmode.button_radio_workmode = button_radioconfig;
        lv_obj_add_event_cb(button_radioconfig, action_switch_to_screen_by_id,
                            LV_EVENT_PRESSED, (void *)SCREEN_ID_RADIO_CONFIG);
        {
          lv_obj_t *parent_obj = button_radioconfig;
          {
            // icon_radioconfig
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, LV_SYMBOL_RADIO_RECEIVER);
            lv_obj_set_style_text_font(obj, &main_symbol, 0);
            objects.page_main.status_bottom_obj.radio_workmode.icon_radio_workmode = obj;
          }
        }
      }
    }
  }

  tick_screen_main();
}

void tick_screen_main()
{
	lv_update_battery(&systemInfo.powerMonitor.batteryInfo, &objects.page_main.status_bar_obj.battery);
	Page_Main_Update();
}

void create_screen_radio_config()
{
  lv_obj_t *screen_radio = lv_obj_create(0);
  objects.radio_config = screen_radio;
  lv_obj_set_pos(screen_radio, 0, 0);
  lv_obj_set_size(screen_radio, 294, 126);
  {
    lv_obj_t *screen_radio_obj = screen_radio;

    {
      // button_return
      lv_obj_t *button_return_t = lv_btn_create(screen_radio_obj);
      lv_obj_align(button_return_t, LV_ALIGN_TOP_LEFT, 10, 10);
      lv_obj_set_size(button_return_t, 50, 30);
      lv_obj_add_event_cb(button_return_t, action_switch_to_screen_by_id,
                          LV_EVENT_PRESSED, (void *)SCREEN_ID_MAIN);

      objects.page_radio.button_return = button_return_t;
      {
        // label_back
        lv_obj_t *label_return_obj = lv_label_create(button_return_t);
        // objects.page_work. = label_return_obj;
        lv_obj_set_pos(label_return_obj, 0, 0);
        lv_obj_set_size(label_return_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_style_align(label_return_obj, LV_ALIGN_CENTER,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(label_return_obj, LV_SYMBOL_LEFT);
      }
    }

    {
      // label_radioStatus
      lv_obj_t *label_radioStatus = lv_label_create(screen_radio_obj);
      lv_label_set_text(label_radioStatus, LV_SYMBOL_RADIO_RECEIVER);
      lv_obj_set_style_text_font(label_radioStatus, &main_symbol, 0);
      lv_obj_set_style_text_color(label_radioStatus, lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(label_radioStatus, LV_ALIGN_TOP_RIGHT, -25, 5);

      objects.page_radio.lable_radio_status = label_radioStatus;
    }

    {
      lv_style_init(&objects.page_radio.style_roller);
      lv_style_set_text_color(&objects.page_radio.style_roller, lv_color_hex(0x000000));
      lv_style_set_bg_color(&objects.page_radio.style_roller, lv_color_hex(0xFFFFFF));
    }
    {
      lv_obj_t *icon_radioconfig_t = lv_label_create(screen_radio_obj);
      lv_label_set_text(icon_radioconfig_t, LV_SYMBOL_FREQUENCE);
      lv_obj_set_style_text_font(icon_radioconfig_t, &symbol_radio, 0);
      lv_obj_set_style_text_color(icon_radioconfig_t,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(icon_radioconfig_t, LV_ALIGN_LEFT_MID, 10, -5);

      lv_obj_t *label_radioFreq = lv_label_create(screen_radio_obj);
      lv_label_set_text(label_radioFreq, "Freq");
      lv_obj_set_style_text_font(label_radioFreq, &Oswald_Bold_12, 0);
      lv_obj_set_style_text_color(label_radioFreq,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(label_radioFreq, LV_ALIGN_LEFT_MID, 40, -5);

      const char *opts = "1[455.05]\n2[456.05]\n3[457.05]\n4[458.05]\n5[459.05]"
                         "\n6[460.05]\n7[461.05]\n8[462.05]\n9[463.05]";

      lv_obj_t *roller = lv_roller_create(screen_radio_obj);
      lv_roller_set_options(roller, opts, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_visible_row_count(roller, 2);
      lv_obj_set_size(roller, 125, 50);
      lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
      lv_obj_align(roller, LV_ALIGN_LEFT_MID, 90, -25);
      // lv_roller_set_selected(roller, 2, LV_ANIM_OFF);
      lv_obj_add_event_cb(roller, action_radio_roller, LV_EVENT_ALL, NULL);
      lv_obj_add_style(roller, &objects.page_radio.style_roller, 0);

      objects.page_radio.roller_radioFreq = roller;
    }

    {
      /*icon*/
      lv_obj_t *icon_radioProtocol = lv_label_create(screen_radio_obj);
      lv_label_set_text(icon_radioProtocol, LV_SYMBOL_SHUFFLE);
      lv_obj_set_style_text_font(icon_radioProtocol, &lv_font_montserrat_18,
                                 0);
      lv_obj_set_style_text_color(icon_radioProtocol,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(icon_radioProtocol, LV_ALIGN_LEFT_MID, 10, 40);

      lv_obj_t *label_radioProtocol = lv_label_create(screen_radio_obj);
      lv_label_set_text(label_radioProtocol, "Prot");
      lv_obj_set_style_text_font(label_radioProtocol, &Oswald_Bold_12, 0);
      lv_obj_set_style_text_color(label_radioProtocol,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(label_radioProtocol, LV_ALIGN_LEFT_MID, 40, 40);

      /*roller*/
      lv_obj_t *roller_radioProtocol = lv_roller_create(screen_radio_obj);
      lv_roller_set_options(roller_radioProtocol,
                            "TRIMTALK\nTRIMMK3\nTT450S\nTRANSEOT\nSOUTH\nCSS", LV_ROLLER_MODE_INFINITE);
      lv_roller_set_visible_row_count(roller_radioProtocol, 2);
      lv_obj_set_size(roller_radioProtocol, 125, 50);
      lv_obj_set_style_text_align(roller_radioProtocol, LV_TEXT_ALIGN_LEFT, 0);
      lv_obj_align(roller_radioProtocol, LV_ALIGN_LEFT_MID, 90, 32);
      lv_obj_add_event_cb(roller_radioProtocol, action_radio_protocol_roller, LV_EVENT_ALL, NULL);
      lv_obj_add_style(roller_radioProtocol, &objects.page_radio.style_roller, 0);

      objects.page_radio.roller_radioProtocol = roller_radioProtocol;
    }

    {
      // button_confirm
      lv_obj_t *button_confirm_t = lv_btn_create(screen_radio_obj);
      lv_obj_set_size(button_confirm_t, 50, 30);
      lv_obj_align(button_confirm_t, LV_ALIGN_TOP_RIGHT, -17, 40);
      lv_obj_add_event_cb(button_confirm_t, action_radio_set,
                          LV_EVENT_PRESSED, (void *)0);
      {
        lv_obj_t *button_confirm_obj = button_confirm_t;
        {
          // label_back
          lv_obj_t *label_return_obj = lv_label_create(button_confirm_obj);
          // objects.icon_worktomain = label_return_obj;
          lv_obj_set_pos(label_return_obj, 0, 0);
          lv_obj_set_size(label_return_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_set_style_align(label_return_obj, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_label_set_text(label_return_obj, "Set");
          lv_obj_set_style_text_font(label_return_obj, &Oswald_Bold_12, 0);
        }
      }
      objects.page_radio.button_cofirm = button_confirm_t;
    }

    {
      // button_close
      lv_obj_t *button_close_t = lv_btn_create(screen_radio_obj);
      lv_obj_set_size(button_close_t, 50, 30);
      lv_obj_align(button_close_t, LV_ALIGN_TOP_RIGHT, -17, 80);
      lv_obj_add_event_cb(button_close_t, action_radio_close,
                          LV_EVENT_PRESSED, (void *)0);
      {
        lv_obj_t *button_close_obj = button_close_t;
        {
          // label_back
          lv_obj_t *label_return_obj = lv_label_create(button_close_obj);
          // objects.icon_worktomain = label_return_obj;
          lv_obj_set_pos(label_return_obj, 0, 0);
          lv_obj_set_size(label_return_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_set_style_align(label_return_obj, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_label_set_text(label_return_obj, "Close");
          lv_obj_set_style_text_font(label_return_obj, &Oswald_Bold_12, 0);
        }
      }
      objects.page_radio.button_close = button_close_t;
    }
  }
  Page_RadioConfig_Update();
}

void tick_screen_radio_config()
{
	Page_RadioConfig_Update();
}

void create_screen_work_config()
{
  lv_obj_t *screen_work_config = lv_obj_create(0);
  objects.work_config = screen_work_config;
  lv_obj_set_pos(screen_work_config, 0, 0);
  lv_obj_set_size(screen_work_config, 294, 126);

  {
    lv_obj_t *parent_obj = screen_work_config;

    // First child: Back button
    {
      lv_obj_t *button_settingsBackMain = lv_btn_create(parent_obj);
      lv_obj_set_size(button_settingsBackMain, 50, 30);
      lv_obj_align(button_settingsBackMain, LV_ALIGN_TOP_LEFT, 20, 10);
      lv_obj_add_event_cb(button_settingsBackMain, action_switch_to_screen_by_id, 
      LV_EVENT_PRESSED, (void *)SCREEN_ID_MAIN);

      lv_obj_t *label_settingsBackMain = lv_label_create(button_settingsBackMain);
      lv_label_set_text(label_settingsBackMain, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_settingsBackMain, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_settingsBackMain, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_settingsBackMain);

      objects.page_work.button_return = button_settingsBackMain;
    }
    // Second child: Battery status
    {
      lv_obj_t *button_batteryStatus = lv_btn_create(parent_obj);
      lv_obj_set_size(button_batteryStatus, 50, 30);
      lv_obj_align(button_batteryStatus, LV_ALIGN_TOP_LEFT, 20, 50);
      lv_obj_add_event_cb(button_batteryStatus, action_switch_to_screen_by_id,
      LV_EVENT_PRESSED, (void *)SCREEN_ID_BATTERY_INFO);

      lv_obj_t *label_batteryStatus = lv_label_create(button_batteryStatus);
      lv_label_set_text(label_batteryStatus, LV_SYMBOL_CHARGE);
      lv_obj_set_style_text_font(label_batteryStatus, &lv_font_montserrat_12, 0);
      lv_obj_set_style_text_color(label_batteryStatus, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_batteryStatus);

      objects.page_work.button_charge = button_batteryStatus;
    }

    // Third child: 4G status and switch
    {
      // Static status icon
      lv_obj_t *icon_static = lv_label_create(parent_obj);
      lv_label_set_text(icon_static, LV_SYMBOL_STATIC);
      lv_obj_set_style_text_font(icon_static, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_static, lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_obj_align(icon_static, LV_ALIGN_TOP_LEFT, 110, 25);

      // Static status button
      lv_obj_t *button_static = lv_btn_create(parent_obj);
			lv_obj_set_size(button_static, 110, 40);
      lv_obj_align(button_static, LV_ALIGN_TOP_LEFT, 140, 20);
      lv_obj_add_event_cb(button_static, action_switch_to_screen_by_id, 
      LV_EVENT_PRESSED, (void *)SCREEN_ID_STATIC_SETTINGS);

      lv_obj_t *label_static = lv_label_create(button_static);
      lv_label_set_text(label_static, "Static Record");
      lv_obj_set_style_text_font(label_static, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label_static, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_static);


      objects.page_work.button_static = button_static;
      objects.page_work.icon_static = icon_static;
      // 4G status icon
      lv_obj_t *icon_4g = lv_label_create(parent_obj);
      lv_label_set_text(icon_4g, LV_SYMBOL_4G);
      lv_obj_set_style_text_font(icon_4g, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_4g, lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align_to(icon_4g, icon_static, LV_ALIGN_BOTTOM_MID, 0, 55);

      lv_obj_t *button_4g = lv_btn_create(parent_obj);
			lv_obj_set_size(button_4g, 110, 40);
      lv_obj_align(button_4g, LV_ALIGN_TOP_LEFT, 140, 70);
      lv_obj_add_event_cb(button_4g, action_switch_to_screen_by_id, 
      LV_EVENT_PRESSED, (void *)SCREEN_ID_NETWORK_SETTINGS);

      lv_obj_t *label_4g = lv_label_create(button_4g);
      lv_label_set_text(label_4g, "Ntrip Config");
      lv_obj_set_style_text_font(label_4g, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label_4g, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_4g);

      objects.page_work.button_4g = button_4g;
      objects.page_work.icon_4g = icon_4g;
    }

    // Third child: Firmware version and compile time
    {
      lv_obj_t *firmware_container = lv_obj_create(parent_obj);
      lv_obj_remove_style_all(firmware_container);
      lv_obj_set_size(firmware_container, 280, 25);
      lv_obj_align(firmware_container, LV_ALIGN_BOTTOM_MID, 0, 0);
      lv_obj_clear_flag(firmware_container, LV_OBJ_FLAG_SCROLLABLE);

      // Firmware version label
      lv_obj_t *label_firmware = lv_label_create(firmware_container);
      lv_label_set_text_fmt(label_firmware, "Rev: %s", SOFTWARE_VERSION);
      lv_obj_set_style_text_font(label_firmware, &Oswald_Bold_12, 0);
      lv_obj_set_style_text_color(label_firmware, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(label_firmware, LV_ALIGN_LEFT_MID, 10, 0);
    }
  }

  tick_screen_work_config();
}

void tick_screen_work_config()
{
  if(systemInfo.recordInfo.record_status)
  {
    lv_obj_set_style_text_color(objects.page_work.icon_static, lv_palette_main(LV_PALETTE_BLUE), 0);
  }
  else
  {
    lv_obj_set_style_text_color(objects.page_work.icon_static, lv_palette_main(LV_PALETTE_GREY), 0);
  }
  if(systemInfo.ntripInfo.gprs_status)
  {
    lv_obj_set_style_text_color(objects.page_work.icon_4g, lv_palette_main(LV_PALETTE_GREEN), 0);
  }
  else
  {
    lv_obj_set_style_text_color(objects.page_work.icon_4g, lv_palette_main(LV_PALETTE_GREY), 0);
  }
}

void create_screen_static_settings()
{
  lv_obj_t *screen_static_settings = lv_obj_create(0);
  objects.static_settings = screen_static_settings;
  lv_obj_set_pos(screen_static_settings, 0, 0);
  lv_obj_set_size(screen_static_settings, 294, 126);

  {
    lv_obj_t *parent_obj = screen_static_settings;
    {
      /*back button*/
      lv_obj_t *button_static_back = lv_btn_create(parent_obj);
      lv_obj_set_size(button_static_back, 50, 30);
      lv_obj_align(button_static_back, LV_ALIGN_TOP_LEFT, 20, 10);
      lv_obj_add_event_cb(button_static_back, action_switch_to_screen_by_id, 
      LV_EVENT_PRESSED, (void *)SCREEN_ID_WORK_CONFIG);

      lv_obj_t *label_static_back = lv_label_create(button_static_back);
      lv_label_set_text(label_static_back, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_static_back, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_static_back, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_static_back);

      objects.page_static.button_static_back = button_static_back;
    }

    {
      lv_obj_t *icon_static = lv_label_create(parent_obj);
      lv_label_set_text(icon_static, LV_SYMBOL_STATIC);
      lv_obj_set_style_text_font(icon_static, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_static, lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_obj_align(icon_static, LV_ALIGN_TOP_LEFT, 80, 15);

      lv_obj_t *label_static = lv_label_create(parent_obj);
      lv_label_set_text_fmt(label_static, "Status: %s", systemInfo.recordInfo.record_status ? "ON" : "OFF");
      lv_obj_set_style_text_font(label_static, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label_static, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_static, icon_static, LV_ALIGN_LEFT_MID, 25, 0);

      lv_obj_t *switch_static = lv_switch_create(parent_obj);
      lv_obj_align_to(switch_static, label_static, LV_ALIGN_RIGHT_MID, 70, 0);
      lv_obj_add_event_cb(switch_static, action_turn_onoff_static, LV_EVENT_VALUE_CHANGED, NULL);

      lv_obj_t *icon_static_name = lv_label_create(parent_obj);
      lv_label_set_text(icon_static_name, LV_SYMBOL_FILE);
      lv_obj_set_style_text_font(icon_static_name, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(icon_static_name, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_static_name, LV_ALIGN_TOP_LEFT, 10, 45);

      lv_obj_t *lable_static_name = lv_label_create(parent_obj);
      lv_label_set_text(lable_static_name, "REC: -");
      lv_obj_set_style_text_font(lable_static_name, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(lable_static_name, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(lable_static_name, icon_static_name, LV_ALIGN_LEFT_MID, 20, 0);

      lv_obj_t *icon_sd = lv_label_create(parent_obj);
      lv_label_set_text(icon_sd, LV_SYMBOL_SD_CARD);
      lv_obj_set_style_text_font(icon_sd, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(icon_sd, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_sd, LV_ALIGN_TOP_LEFT, 10, 70);

      lv_obj_t *label_sd = lv_label_create(parent_obj);
      lv_label_set_text_fmt(label_sd, "RSC: -M");
      lv_obj_set_style_text_font(label_sd, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label_sd, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_sd, icon_sd, LV_ALIGN_LEFT_MID, 20, 0);


      lv_obj_t *icon_static_type = lv_label_create(parent_obj);
      lv_label_set_text(icon_static_type, LV_SYMBOL_COPY);
      lv_obj_set_style_text_font(icon_static_type, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(icon_static_type, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_static_type, LV_ALIGN_TOP_LEFT, 10, 95);

      lv_obj_t *lable_static_type = lv_label_create(parent_obj);
      lv_label_set_text(lable_static_type, "Type: -");
      lv_obj_set_style_text_font(lable_static_type, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(lable_static_type, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(lable_static_type, icon_static_type, LV_ALIGN_LEFT_MID, 20, 0);

      lv_obj_t *icon_static_time = lv_label_create(parent_obj);
      lv_label_set_text(icon_static_time, LV_SYMBOL_BELL);
      lv_obj_set_style_text_font(icon_static_time, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(icon_static_time, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align_to(icon_static_time, icon_static_type, LV_ALIGN_LEFT_MID, 140, 0);

      lv_obj_t *lable_static_time = lv_label_create(parent_obj);
      lv_label_set_text(lable_static_time, "Time: -");
      lv_obj_set_style_text_font(lable_static_time, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(lable_static_time, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(lable_static_time, icon_static_time, LV_ALIGN_LEFT_MID, 20, 0);

      objects.page_static.icon_static = icon_static;
      objects.page_static.label_static = label_static;
      objects.page_static.switch_static_onoff = switch_static;
      objects.page_static.icon_static_name = icon_static_name;
      objects.page_static.label_static_name = lable_static_name;
      objects.page_static.icon_static_type = icon_static_type;
      objects.page_static.label_static_type = lable_static_type;
      objects.page_static.icon_static_time = icon_static_time;
      objects.page_static.label_static_time = lable_static_time;
      objects.page_static.icon_sd = icon_sd;
      objects.page_static.label_sd = label_sd;
    }
  }
  tick_screen_static_settings();
  Page_StaticRecord_Update();
}

void tick_screen_static_settings()
{
		Page_StaticRecord_Update();
}

void create_screen_network_settings()
{
  lv_obj_t *screen_network_settings = lv_obj_create(0);
  objects.network_settings = screen_network_settings;
  lv_obj_set_pos(screen_network_settings, 0, 0);
  lv_obj_set_size(screen_network_settings, 294, 126);
  {
    lv_obj_t *parent_obj = screen_network_settings;
    {
      /*back button*/
      lv_obj_t *button_network_back = lv_btn_create(parent_obj);
      lv_obj_set_size(button_network_back, 50, 30);
      lv_obj_align(button_network_back, LV_ALIGN_TOP_LEFT, 20, 10);
      lv_obj_add_event_cb(button_network_back, action_switch_to_screen_by_id, 
      LV_EVENT_PRESSED, (void *)SCREEN_ID_WORK_CONFIG);

      lv_obj_t *label_network_back = lv_label_create(button_network_back);
      lv_label_set_text(label_network_back, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_network_back, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_network_back, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_network_back);

      objects.page_network.button_network_back = button_network_back;
    }

    {
      lv_obj_t *icon_4g = lv_label_create(parent_obj);
      lv_label_set_text(icon_4g, LV_SYMBOL_4G);
      lv_obj_set_style_text_font(icon_4g, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_4g, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_4g, LV_ALIGN_TOP_LEFT, 90, 15);

      lv_obj_t *label_4g_status = lv_label_create(parent_obj);
      lv_label_set_text(label_4g_status, "-OFF");
      lv_obj_set_style_text_font(label_4g_status, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label_4g_status, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_4g_status, icon_4g, LV_ALIGN_TOP_LEFT, 75, 0);

      lv_obj_t *switch_4g = lv_switch_create(parent_obj);
      lv_obj_align_to(switch_4g, label_4g_status, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(switch_4g, action_turn_onoff_4g, 
      LV_EVENT_VALUE_CHANGED, NULL);

      lv_obj_t *lable_ntrip_server = lv_label_create(parent_obj);
      lv_label_set_text(lable_ntrip_server, "NTRIP Server:");
      lv_obj_set_style_text_font(lable_ntrip_server, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(lable_ntrip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align(lable_ntrip_server, LV_ALIGN_LEFT_MID, 10, 0);

      lv_obj_t *ip_ntrip_server = lv_label_create(parent_obj);
      lv_label_set_text(ip_ntrip_server, "-");
      lv_obj_set_style_text_font(ip_ntrip_server, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(ip_ntrip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(ip_ntrip_server, lable_ntrip_server, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

      lv_obj_t *lable_mount_nrtip_server = lv_label_create(parent_obj);
      lv_label_set_text(lable_mount_nrtip_server, "MountPoint:");
      lv_obj_set_style_text_font(lable_mount_nrtip_server, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(lable_mount_nrtip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(lable_mount_nrtip_server, lable_ntrip_server, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

      lv_obj_t *name_ntrip_server = lv_label_create(parent_obj);
      lv_label_set_text(name_ntrip_server, "-");
      lv_obj_set_style_text_font(name_ntrip_server, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(name_ntrip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(name_ntrip_server, lable_mount_nrtip_server, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

      objects.page_network.icon_4g = icon_4g;
      objects.page_network.label_4g_status = label_4g_status;
      objects.page_network.switch_4g_onoff = switch_4g;
      objects.page_network.ip_ntrip_server = ip_ntrip_server;
      objects.page_network.name_ntrip_server = name_ntrip_server;
    }
  }
}

void tick_screen_network_settings()
{
	Page_NtripServer_Update();
}

void create_screen_batteryInfo()
{
  lv_obj_t *screen_batteryInfo = lv_obj_create(0);
  objects.batteryInfo = screen_batteryInfo;
  lv_obj_set_pos(screen_batteryInfo, 0, 0);
  lv_obj_set_size(screen_batteryInfo, 294, 126);
  {
    lv_obj_t *parent_obj = screen_batteryInfo;
    {
      /*back button*/
      lv_obj_t *button_batteryInfo_back = lv_btn_create(parent_obj);
      lv_obj_set_size(button_batteryInfo_back, 50, 30);
      lv_obj_align(button_batteryInfo_back, LV_ALIGN_TOP_LEFT, 20, 10);
      lv_obj_add_event_cb(button_batteryInfo_back, action_switch_to_screen_by_id, 
      LV_EVENT_PRESSED, (void *)SCREEN_ID_WORK_CONFIG);

      lv_obj_t *label_network_back = lv_label_create(button_batteryInfo_back);
      lv_label_set_text(label_network_back, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_network_back, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_network_back, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_network_back);

      objects.page_batteryInfo.button_batteryInfo_back = button_batteryInfo_back;
    }
    {
      // battery icon
      lv_obj_t *battery_status_icon = lv_label_create(parent_obj);
      lv_label_set_text(battery_status_icon, LV_SYMBOL_CHARGE);
      lv_obj_set_style_text_font(battery_status_icon, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(battery_status_icon, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align_to(battery_status_icon, objects.page_batteryInfo.button_batteryInfo_back, LV_ALIGN_RIGHT_MID, 40, 0);

      objects.page_batteryInfo.icon_battery = battery_status_icon;
    }

    {
      // battery info
      lv_obj_t *battery_charge_percent = lv_label_create(parent_obj);
      lv_label_set_text(battery_charge_percent, "-%");
      lv_obj_set_style_text_font(battery_charge_percent, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(battery_charge_percent, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(battery_charge_percent, objects.page_batteryInfo.button_batteryInfo_back, LV_ALIGN_RIGHT_MID, 80, 0);

      lv_obj_t *battery_charge_voltage = lv_label_create(parent_obj);
      lv_label_set_text(battery_charge_voltage, "-V");
      lv_obj_set_style_text_font(battery_charge_voltage, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(battery_charge_voltage, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(battery_charge_voltage, objects.page_batteryInfo.button_batteryInfo_back, LV_ALIGN_RIGHT_MID, 140, 0);

      lv_obj_t *battery_charge_temp = lv_label_create(parent_obj);
      lv_label_set_text(battery_charge_temp, "-C");
      lv_obj_set_style_text_font(battery_charge_temp, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(battery_charge_temp, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(battery_charge_temp, objects.page_batteryInfo.button_batteryInfo_back, LV_ALIGN_RIGHT_MID, 190, 0);

      objects.page_batteryInfo.label_battery_percent = battery_charge_percent;
      objects.page_batteryInfo.label_battery_voltage = battery_charge_voltage;
      objects.page_batteryInfo.label_battery_temp = battery_charge_temp;
    }

//    {
//      // charge current & voltage
//      lv_obj_t *current = lv_label_create(parent_obj);
//			lv_label_set_text(current, "-mA");
//			lv_obj_set_style_text_font(current, &Oswald_Bold_18, 0);
//      lv_obj_set_style_text_color(current, lv_color_hex(0xffffff), 0);
//      lv_obj_align(current, LV_ALIGN_LEFT_MID, 20, 0);
//			
//			lv_obj_t *voltage = lv_label_create(parent_obj);
//			lv_label_set_text(voltage, "-mV");
//			lv_obj_set_style_text_font(voltage, &Oswald_Bold_18, 0);
//      lv_obj_set_style_text_color(voltage, lv_color_hex(0xffffff), 0);
//      lv_obj_align(voltage, LV_ALIGN_LEFT_MID, 100, 0);
//			
//			objects.page_batteryInfo.label_charge_current = current;
//			objects.page_batteryInfo.label_charge_voltage = voltage;
//    }
  }
}

void tick_screen_batteryInfo()
{
  bool is_charging = (systemInfo.powerMonitor.batteryInfo.chargeStatus != notCharge);
  if (is_charging)
  {
    lv_obj_set_style_text_color(objects.page_batteryInfo.icon_battery, lv_palette_main(LV_PALETTE_GREEN), 0);
  }
  else
  {
    lv_obj_set_style_text_color(objects.page_batteryInfo.icon_battery, lv_palette_main(LV_PALETTE_GREY), 0);
  }
	static uint32_t last_ = 0;
	if(millis() - last_ >= 1000)
	{
		last_ = millis();
//		lv_label_set_text_fmt(objects.page_batteryInfo.label_battery_percent, "%0.1f%%", systemInfo.powerMonitor.batteryInfo.Percent);
//		lv_label_set_text_fmt(objects.page_batteryInfo.label_battery_voltage, "%0.1fV", systemInfo.powerMonitor.batteryInfo.Voltage_f);
//		lv_label_set_text_fmt(objects.page_batteryInfo.label_battery_temp, "%0.1fC", systemInfo.powerMonitor.batteryInfo.Temp_f);
		
//		lv_label_set_text_fmt(objects.page_batteryInfo.label_charge_current, "%dmA", batteryState.chargeCurrent_mA);
//		lv_label_set_text_fmt(objects.page_batteryInfo.label_charge_voltage, "%dmV", batteryState.chargeCurrent_mV);
	}
}

void create_screen_poweroff()
{
  lv_obj_t *screen_poweroff = lv_obj_create(0);
  objects.power_off = screen_poweroff;
  lv_obj_set_pos(screen_poweroff, 0, 0);
  lv_obj_set_size(screen_poweroff, 294, 126);
  {
    lv_obj_t *parent_obj = screen_poweroff;

    lv_style_init(&objects.page_poweroff.battery.style_battery);

    lv_obj_t *img = lv_label_create(parent_obj);
    lv_label_set_recolor(img, true);
    lv_obj_align(img, LV_ALIGN_TOP_LEFT, 210, 10);
    lv_label_set_text(img, LV_SYMBOL_BATTERY_EMPTY);
    lv_obj_set_style_text_font(img, &lv_font_montserrat_18, 0);
    lv_obj_add_style(img, &objects.page_poweroff.battery.style_battery, 0);

    lv_obj_t *useage = lv_obj_create(img);
    lv_obj_remove_style_all(useage);
    lv_obj_set_style_bg_opa(useage, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(useage, LV_OPA_COVER, 0);
    lv_obj_set_size(useage, BATT_USAGE_WIDTH, BATT_USAGE_HEIGHT);
    lv_obj_align_to(useage, img, LV_ALIGN_LEFT_MID, 2, 0);
    lv_obj_add_style(useage, &objects.page_poweroff.battery.style_battery, 0);

    lv_obj_t *battery_label = lv_label_create(parent_obj);
    lv_label_set_text(battery_label, "0%");
    lv_obj_set_style_text_font(battery_label, &Oswald_Bold_18, 0);
    lv_obj_set_style_text_color(battery_label, lv_color_hex(0xffffff), 0);
    lv_obj_align_to(battery_label, img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    objects.page_poweroff.battery.icon_battery = img;
    objects.page_poweroff.battery.label_useage = useage;
    objects.page_poweroff.battery.label_text = battery_label;

    // Power off bar
    lv_obj_t *bar = lv_bar_create(parent_obj);
    lv_obj_set_size(bar, 220, 20);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x2d2d44), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 10, LV_PART_MAIN);

    lv_obj_set_style_bg_color(bar, lv_color_hex(0x4e54c8), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar, 10, LV_PART_INDICATOR);

    lv_bar_set_value(bar, 0, LV_ANIM_OFF);
    lv_bar_set_range(bar, 0, 100);
    lv_obj_center(bar);

    objects.page_poweroff.bar = bar;

    // ???????
    lv_obj_t *percentage_label = lv_label_create(parent_obj);
    lv_label_set_text(percentage_label, "0%");
    lv_obj_set_style_text_font(percentage_label, &Oswald_Bold_18, 0);
    lv_obj_set_style_text_color(percentage_label, lv_color_white(), 0);
    lv_obj_align_to(percentage_label, bar, LV_ALIGN_OUT_TOP_MID, 0, -10);

    objects.page_poweroff.percentage_label = percentage_label;

    // ????????
    lv_obj_t *bar_label = lv_label_create(parent_obj);
    lv_label_set_text(bar_label, "PowerOff");
    lv_obj_set_style_text_font(bar_label, &Oswald_Bold_18, 0);
    lv_obj_set_style_text_color(bar_label, lv_color_white(), 0);
    lv_obj_align(bar_label, LV_ALIGN_BOTTOM_MID, 0, -15);

    objects.page_poweroff.label = bar_label;
  }
}

void tick_screen_poweroff()
{
  lv_update_battery(&systemInfo.powerMonitor.batteryInfo, &objects.page_poweroff.battery);
}

void arc_callback(void)
{
	systemInfo.powerMonitor.panel_power_on = true;
}

void create_screen_poweroff_charge()
{
  lv_obj_t *screen_poweroff_charge = lv_obj_create(0);
  lv_obj_set_pos(screen_poweroff_charge, 0, 0);
  lv_obj_set_size(screen_poweroff_charge, 294, 126);
  objects.power_off_charge = screen_poweroff_charge;
  {
    lv_obj_t *screen_poweroff_obj = screen_poweroff_charge;
    {
      lv_style_init(&objects.page_poweroff_charge.battery.style_battery);

      lv_obj_t *img = lv_label_create(screen_poweroff_obj);
      lv_label_set_recolor(img, true);
      lv_obj_align(img, LV_ALIGN_TOP_LEFT, 210, 10);
      lv_label_set_text(img, LV_SYMBOL_BATTERY_EMPTY);
      lv_obj_set_style_text_font(img, &lv_font_montserrat_18, 0);
      lv_obj_add_style(img, &objects.page_poweroff_charge.battery.style_battery, 0);

      lv_obj_t *useage = lv_obj_create(img);
      lv_obj_remove_style_all(useage);
      lv_obj_set_style_bg_opa(useage, LV_OPA_COVER, 0);
      lv_obj_set_style_opa(useage, LV_OPA_COVER, 0);
      lv_obj_set_size(useage, BATT_USAGE_WIDTH, BATT_USAGE_HEIGHT);
      lv_obj_align_to(useage, img, LV_ALIGN_LEFT_MID, 2, 0);
      lv_obj_add_style(useage, &objects.page_poweroff_charge.battery.style_battery, 0);

      // Firmware version label
      lv_obj_t *label_firmware = lv_label_create(screen_poweroff_obj);
      lv_label_set_text_fmt(label_firmware, "Rev:%s", SOFTWARE_VERSION);
      lv_obj_set_style_text_font(label_firmware, &Oswald_Bold_12, 0);
      lv_obj_set_style_text_color(label_firmware, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(label_firmware, LV_ALIGN_BOTTOM_LEFT, 10, 0);

      // Compile time label
      lv_obj_t *label_compile_time = lv_label_create(screen_poweroff_obj);
      lv_label_set_text(label_compile_time, __DATE__ " " __TIME__);
      lv_obj_set_style_text_font(label_compile_time, &Oswald_Bold_12, 0);
      lv_obj_set_style_text_color(label_compile_time, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(label_compile_time, LV_ALIGN_BOTTOM_RIGHT, -10, 0);

      lv_obj_t *label = lv_label_create(screen_poweroff_obj);
      lv_label_set_text(label, "0:00:00");
      lv_obj_set_style_text_font(label, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 20, 10);
      objects.page_poweroff_charge.label_charge_time = label;

      label = lv_label_create(screen_poweroff_obj);
      lv_label_set_text(label, "0%");
      lv_obj_set_style_text_font(label, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label, img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

      lv_obj_t *label_charge_status = lv_label_create(screen_poweroff_obj);
      lv_obj_align_to(label_charge_status, label, LV_ALIGN_LEFT_MID, -45, -5);
      lv_label_set_text_fmt(label_charge_status, "N");
      lv_obj_set_style_text_font(label_charge_status, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label_charge_status, lv_color_hex(0xffffff), 0);

      objects.page_poweroff_charge.battery.label_text = label;
      objects.page_poweroff_charge.battery.label_useage = useage;
      objects.page_poweroff_charge.battery.icon_battery = img;
      objects.page_poweroff_charge.battery.label_charge_status = label_charge_status;

      lv_style_set_text_color(&objects.page_poweroff_charge.battery.style_battery, lv_color_hex(0xF44336));
    }

    {

      int arc_size = 126 * 0.7;

      lv_obj_t *arc = lv_arc_create(screen_poweroff_obj);
      lv_obj_set_size(arc, arc_size, arc_size);
      lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
      lv_arc_set_range(arc, 0, 100);
      lv_arc_set_value(arc, 0);
      lv_arc_set_bg_angles(arc, 0, 360);
      lv_obj_remove_style(arc, NULL, LV_PART_KNOB);

      lv_obj_set_style_arc_width(arc, 6, LV_PART_MAIN);
      lv_obj_set_style_arc_width(arc, 6, LV_PART_INDICATOR);
      lv_obj_set_style_arc_color(arc, lv_color_hex(0x505050), LV_PART_MAIN);
      lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);

      objects.page_poweroff_charge.arc_obj = arc;

      lv_obj_t *label_percent = lv_label_create(screen_poweroff_obj);
      lv_label_set_text(label_percent, "0%");
      lv_obj_align_to(label_percent, arc, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_style_text_font(label_percent, &Oswald_Bold_18, 0);

      lv_obj_t *label = lv_label_create(screen_poweroff_obj);
      lv_label_set_text(label, "Hold On");
      lv_obj_set_style_text_font(label, &Oswald_Bold_18, 0);
      lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label, arc, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

      objects.page_poweroff_charge.label_arc_percent = label_percent;

      lv_anim_init(&objects.page_poweroff_charge.arc_anim);
      lv_anim_set_var(&objects.page_poweroff_charge.arc_anim, arc);
      lv_anim_set_exec_cb(&objects.page_poweroff_charge.arc_anim, arc_angle_anim);
      lv_anim_set_time(&objects.page_poweroff_charge.arc_anim, 300); // 300ms
      lv_anim_set_playback_time(&objects.page_poweroff_charge.arc_anim, 0);
      lv_anim_set_repeat_count(&objects.page_poweroff_charge.arc_anim, 0);
      lv_anim_set_path_cb(&objects.page_poweroff_charge.arc_anim, lv_anim_path_linear);
    }
  }
  set_arc_completed_callback(arc_callback);
}

void tick_screen_poweroff_charge()
{
	char buf[32];
  /*Battery*/
	MakeTimeString(millis(), buf, 32);
  lv_update_battery_off(&systemInfo.powerMonitor.batteryInfo, &objects.page_poweroff_charge.battery);
  lv_label_set_text_fmt(objects.page_poweroff_charge.label_charge_time, "%s", buf);

  /*ARC*/
  uint8_t target_percent = (systemInfo.powerMonitor.PowerKey_PressCount * 2);
  target_percent = target_percent > 100 ? 100 : target_percent;

  static uint8_t last_target_percent = 0;
  if (target_percent != last_target_percent)
  {
    last_target_percent = target_percent;
    start_animation_to_value(objects.page_poweroff_charge.arc_obj, &objects.page_poweroff_charge.arc_anim, target_percent);
  }
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_work_config,
    tick_screen_radio_config,
    tick_screen_static_settings,
    tick_screen_network_settings,
    tick_screen_batteryInfo,
    tick_screen_poweroff,
    tick_screen_poweroff_charge,
};
void tick_screen(int screen_index)
{
  tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId)
{
  tick_screen_funcs[screenId - 1]();
}

void init_screen_theme()
{
  lv_disp_t *dispp = lv_disp_get_default();
  lv_theme_t *theme = lv_theme_default_init(
      dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
      true, LV_FONT_DEFAULT);
  lv_disp_set_theme(dispp, theme);
}
void create_screens()
{
	init_screen_theme();
  create_screen_main();
  create_screen_work_config();
  create_screen_radio_config();
  create_screen_static_settings();
  create_screen_network_settings();
  create_screen_batteryInfo();
  create_screen_poweroff();
	create_screen_poweroff_charge();
}