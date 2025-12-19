#include "HAL.h"
#include "Arduino.h"
#include "lv_port.h"

void HAL::Display_Init()
{
		CORE_DEBUG_PRINTF("Disaply Init");
    if (!lv_is_initialized()) {
        lv_init();
    }
    lv_port_init();
}

void HAL::Dispaly_Update()
{
    lv_timer_handler();
}