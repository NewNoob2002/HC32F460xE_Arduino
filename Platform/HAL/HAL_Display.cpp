#include "Arduino.h"
#include "HAL.h"
#include "lv_port.h"


void
HAL::Display_Init() {
    if (!lv_is_initialized()) {
        lv_init();
    }
    CORE_DEBUG_PRINTF("Disaply Init");
    lv_port_init();
}