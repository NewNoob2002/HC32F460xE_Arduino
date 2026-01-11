#include "HAL.h"
#include "lv_port.h"
#include "lvgl_screen.h"

void HAL::Display_Init() {
  lv_init();
	lv_port_init();
  LVGL_screen_init();
  lv_screen6_appear();
}

void HAL::Display_Update() {
    if (screen_flag_change) {
      screen_flag_change = 0;
      // stop old page timer
      switch (last_screen_flag) {
      case 1:
        lv_screen1_disappear();
        break;
      case 2:
        lv_screen2_disappear();
        break;
      case 3:
        lv_screen3_disappear();
        break;
      case 4:
        lv_screen4_disappear();
        break;
      case 5:
        lv_screen5_disappear();
        break;
      case 6:
        lv_screen7_disappear();
        break;
      default:
        break;
      }
      switch (screen_flag) {
      case 1:
        lv_screen1_appear();
        break;
      case 2:
        lv_screen2_appear();
        break;
      case 3:
        lv_screen3_appear();
        break;
      case 4:
        lv_screen4_appear();
        break;
      case 5:
        lv_screen5_appear();
        break;
      case 6:
        lv_screen7_appear();
        break;
      default:
        break;
      }
    }
}
