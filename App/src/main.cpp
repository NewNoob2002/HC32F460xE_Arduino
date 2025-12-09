/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "Arduino.h"
#include "usart.h"
#include "lv_port.h"
#include "demos/lv_demos.h"
#include "slave_i2c.h"
void SystemClock_Config(void);

//static void anim_x_cb(void * var, int32_t v)
//{
//    lv_obj_set_x((lv_obj_t *) var, v);
//}

//static void anim_size_cb(void * var, int32_t v)
//{
//    lv_obj_set_size((lv_obj_t *) var, v, v);
//}

///**
// * Create a playback animation
// */
//void lv_example_anim_2(void)
//{

//    lv_obj_t * obj = lv_obj_create(lv_screen_active());
//    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
//    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
//    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

//    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

//    lv_anim_t a;
//    lv_anim_init(&a);
//    lv_anim_set_var(&a, obj);
//    lv_anim_set_values(&a, 10, 50);
//    lv_anim_set_duration(&a, 1000);
//    lv_anim_set_reverse_delay(&a, 100);
//    lv_anim_set_reverse_duration(&a, 300);
//    lv_anim_set_repeat_delay(&a, 500);
//    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

//    lv_anim_set_exec_cb(&a, anim_size_cb);
//    lv_anim_start(&a);
//    lv_anim_set_exec_cb(&a, anim_x_cb);
//    lv_anim_set_values(&a, 10, 240);
//    lv_anim_start(&a);
//}
/**
 * @brief  Main function of SPI tx/rx dma project
 * @param  None
 * @retval int32_t return value, if needed
 */
int main(void)
{
	/* Peripheral registers write unprotected */
  LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
	disable_JTAG();
	dwt_init();
	SystemClock_Config();
	HAL_Init();
	USART_Init();
  lv_init();
//	lv_port_disp_init();
//	lv_example_anim_2();
  /* Configure BSP */
	pinMode(PA0, OUTPUT);
	slave_i2c_init();
	/* Peripheral registers write protected */
  LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
  while (1) {
		static uint32_t last = 0;
		if(millis() - last >= 1000)
		{
			last = millis();
			digitalToggle(PA0);
		}
		slave_i2c_update();
//    lv_timer_handler();
  }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	
    stc_clock_xtal_init_t     stcXtalInit;
    stc_clock_pll_init_t      stcMpllInit;

    GPIO_AnalogCmd(XTAL_PORT, XTAL_PIN, ENABLE);
    (void)CLK_XtalStructInit(&stcXtalInit);
    (void)CLK_PLLStructInit(&stcMpllInit);

    /* Set bus clk div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                                      CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));

    /* Config Xtal and enable Xtal */
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);

    /* MPLL config (XTAL / pllmDiv * plln / PllpDiv = 200M). */
    stcMpllInit.PLLCFGR = 0UL;
    stcMpllInit.PLLCFGR_f.PLLM = 1UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLN = 50UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLP = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLQ = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLR = 2UL - 1UL;
    stcMpllInit.u8PLLState = CLK_PLL_ON;
    stcMpllInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcMpllInit);
    /* Wait MPLL ready. */
    while (SET != CLK_GetStableStatus(CLK_STB_FLAG_PLL)) {
        ;
    }

    /* sram init include read/write wait cycle setting */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    SRAM_SetWaitCycle((SRAM_SRAM12 | SRAM_SRAM3 | SRAM_SRAMR), SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);

    /* flash read wait cycle setting */
    (void)EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
    /* 3 cycles for 126MHz ~ 200MHz */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT3);
    /* Switch driver ability */
    (void)PWC_HighSpeedToHighPerformance();
    /* Switch system clock source to MPLL. */
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
    /* Reset cache ram */
    EFM_CacheRamReset(ENABLE);
    EFM_CacheRamReset(DISABLE);
    /* Enable cache */
    EFM_CacheCmd(ENABLE);
		
		SystemCoreClockUpdate();
}