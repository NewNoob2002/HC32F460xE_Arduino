/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "Arduino.h"
#include "SEGGER_RTT.h"
#include "slave_i2c.h"

extern "C" {
SystemInfo_t systemInfo;
volatile SharedData_t shared_info __attribute__((section(".noinit")));
}

/**
 * @brief  Main function of SPI tx/rx dma project
 * @param  None
 * @retval int32_t return value, if needed
 */
int
main(void) {
    /* Peripheral registers write unprotected */
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
    /* Configure BSP */
    HAL::HAL_Init();
    HAL::Power_OnCheck();
    slave_i2c_init();
    /* Peripheral registers write protected */
    LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
    while (1) {
        slave_i2c_update();
        HAL::HAL_Update();
    }
}