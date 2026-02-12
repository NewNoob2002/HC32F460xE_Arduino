/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "Arduino.h"
#include "SEGGER_RTT.h"
#include "slave_i2c.h"

SystemInfo_t systemInfo;
volatile SharedData_t shared_info __attribute__((section(".bss.NoInit"), used));
/**
 * @brief  Main function of SPI tx/rx dma project
 * @param  None
 * @retval int32_t return value, if needed
 */
int main(void)
{
	/* Peripheral registers write unprotected */
  LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
	shared_info.reset_count = 0xf;
	/* Configure BSP */
	HAL::HAL_Init();
	HAL::Power_OnCheck();
	slave_i2c_init();
	/* Peripheral registers write protected */
  LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
	shared_info.reset_count = 0xff;
  while (1) {
		slave_i2c_update();
		HAL::HAL_Update();
  }
}