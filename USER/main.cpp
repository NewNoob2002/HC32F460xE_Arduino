/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "App.h"
#include "HAL/HAL.h"
#include "slave_i2c.h"

SystemInfo_t systemInfo;

static void
reset_interrupt_controller() {
    constexpr uint32_t irq_count = 128U;
    constexpr uint32_t vector_irq_count = 16U;
    constexpr uint32_t intc_sel_reset_value = 0x1FFUL;

    __disable_irq();

    for (uint32_t i = 0; i < irq_count / 32U; i++) {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }

    volatile uint32_t* const intc_sel = &CM_INTC->SEL0;
    for (uint32_t i = 0; i < irq_count; i++) {
        intc_sel[i] = intc_sel_reset_value;
    }

    volatile uint32_t* const intc_vssel = &CM_INTC->VSSEL128;
    for (uint32_t i = 0; i < vector_irq_count; i++) {
        intc_vssel[i] = 0UL;
    }

    CM_INTC->SWIER = 0UL;
    CM_INTC->EVTER = 0UL;
    CM_INTC->WUPEN = 0UL;
    CM_INTC->EIFCR = 0xFFFFUL;

    __DSB();
    __ISB();
    __enable_irq();
}

/**
 * @brief  Main function of SPI tx/rx dma project
 * @param  None
 * @return This function does not return.
 */
int
main(void) {
    /* Peripheral registers write unprotected */
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
    reset_interrupt_controller();
    HAL::HAL_Init();
    HAL::Display_Init();
    App_Init();
    HAL::Power_OnCheck();
    /* Configure BSP */
    slave_i2c_init();
    /* Peripheral registers write protected */
    LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
    while (1) {
        slave_i2c_update();
        HAL::HAL_Update();
        App_Update();
        lv_timer_handler();
        __WFI();
    }
}
