#ifndef __HAL_H
#define __HAL_H

#include "HAL_CONFIG.h"
#include "mcu_config.h"
#include "../../../Arduino/core_debug.h"
#include "CommonMacro.h"

#ifdef __cplusplus
namespace HAL
{
	void HAL_Init();
	void HAL_Update();
	
	void HAL_Deinit();
	/* I2C */
	int I2C_Scan();
	
	/* Power */
	void Power_Init();
	void Power_OnCheck();
	void Power_HandleTimeUpdate();
	void Power_Shutdown(bool en);
	void Power_PowerOffMonitor();
	bool Power_ShutdownEnsure();
	bool Power_ShutdownForce();
	bool Power_ShutdownLinux();
	bool Power_ShutdownLowBattery();
	bool Power_ShutdownSoftReset();
	void Power_Update();
	const char *Power_GetPowerOffCause();
	void WatchDog_Feed();
	/* Encoder */
	void PowerKey_SetEnable(bool en);
	void Key_Init();
	void Key_Update();
		/* Dispaly */
	void Display_Init();
	void Display_Update();
}
#endif /*__cplusplus*/

#ifdef __cplusplus
 extern "C" {
#endif

void Led_Init();
void Led_Update();
// Led Control interface
void Led_Power_switch(const uint8_t level);
void Led_Charge_switch(const uint8_t level);
void Led_Function_switch(const uint8_t level);
	 
void Charger_Control_GPIO_Init(void);
void USB_Switch_GPIO_Init(void);
void USB_Switch_GPIO_Control(uint8_t state);
void Charge_Enable_Switch(uint8_t state);
void Charge_Current_Select(uint16_t select);
void Charger_Control_Monitor(BatteryInfo_t *batteryState);
/** @defgroup HAL_Exported_Constants HAL Exported Constants
  * @{
  */

/** @defgroup HAL_TICK_FREQ Tick Frequency
  * @{
  */
typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

/** @defgroup HAL_Private_Macros HAL Private Macros
  * @{
  */
#define IS_TICKFREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ)  || \
                           ((FREQ) == HAL_TICK_FREQ_100HZ) || \
                           ((FREQ) == HAL_TICK_FREQ_1KHZ))


/** @addtogroup HAL_Exported_Variables
  * @{
  */
extern volatile uint32_t uwTick;
extern uint32_t uwTickPrio;
extern HAL_TickFreqTypeDef uwTickFreq;


/* Exported functions --------------------------------------------------------*/
/** @addtogroup HAL_Exported_Functions
  * @{
  */
/** @addtogroup HAL_Exported_Functions_Group1
  * @{
  */
/* Initialization and Configuration functions  ******************************/
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority);

void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
uint32_t HAL_GetTickPrio(void);
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef HAL_GetTickFreq(void);

void HAL_SuspendTick(void);
void HAL_ResumeTick(void);

bool chagrer_begin(pBatteryInfo_t p_batteryState);
void charger_update(pBatteryInfo_t p_batteryState);
void checkBatteryInfo(pBatteryInfo_t p_batteryState);

#ifdef __cplusplus
 }
#endif /*__cplusplus*/
 
#endif