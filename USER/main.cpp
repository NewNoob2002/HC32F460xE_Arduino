/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "Arduino.h"
#include "Simulator/App/App.h"
#include "slave_i2c.h"

SystemInfo_t systemInfo;
/**
 * @brief  Main function of SPI tx/rx dma project
 * @param  None
 * @retval int32_t return value, if needed
 */
int main(void)
{
	/* Peripheral registers write unprotected */
  LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
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
  }
}