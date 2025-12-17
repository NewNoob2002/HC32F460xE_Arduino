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
	void Power_HandleTimeUpdate();
	void Power_Shutdown();
	void Power_Update();
	void Power_EventMonitor();
	inline void Power_GetInfo(pBatteryInfo_t info) {
		info = &systemInfo.powerMonitor.batteryInfo;
	}
	typedef void(*Power_CallbackFunction_t)(void);
	void Power_SetEventCallback(Power_CallbackFunction_t callback);
	void WatchDog_Feed();
	/* KEY */
	void Key_Init();
	void Key_Update();
	int32_t Encoder_GetDiff();
	bool Encoder_GetIsPush();
	void Encoder_SetEnable(bool en);
	/* Dispaly */
	void Display_Init();
	void Dispaly_Update();
}
#endif /*__cplusplus*/

#ifdef __cplusplus
 extern "C" {
#endif
	 
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