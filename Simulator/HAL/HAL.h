//
// Created by guoti on 2025/12/20.
//

#ifndef LVGL_HAL_H
#define LVGL_HAL_H

#include "../../Platform/Config/mcu_config.h"
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
    void Power_Shutdown(bool en);
    void Power_PowerOffMonitor();
    void Power_Update();
    void Power_GetInfo(Power_Monitor_t *info);
    const char *Power_GetPowerOffCause();
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

#endif //LVGL_HAL_H