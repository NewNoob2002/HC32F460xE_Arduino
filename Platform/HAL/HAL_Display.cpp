#include "HAL.h"
#include "Arduino.h"
#include "lv_port.h"

void HAL::Display_Init()
{
    if (!lv_is_initialized()) {
        lv_init();
    }
    lv_port_init();
}

void HAL::Dispaly_Update()
{
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_HIDDEN);
    lv_timer_handler();
}