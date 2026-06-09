/**
 *******************************************************************************
 * @file  hc32_ll_utility.c
 * @brief This file provides utility functions for DDL.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-06-30       CDT             Support re-target printf for IAR EW version 9 or later
   2023-06-30       CDT             Modify register USART DR to USART TDR
                                    Prohibit DDL_DelayMS and DDL_DelayUS functions from being optimized
   2024-08-31       CDT             Optimized the Delay functions as cache is enabled
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_UTILITY UTILITY
 * @brief DDL Utility Driver
 * @{
 */

#if (LL_UTILITY_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup UTILITY_Local_Variables UTILITY Local Variables
 * @{
 */
#endif

#ifdef __DEBUG
/**
 * @brief DDL assert error handle function
 * @param [in] file                     Point to the current assert the wrong file.
 * @param [in] line                     Point line assert the wrong file in the current.
 * @retval None
 */
__WEAKDEF void
DDL_AssertHandler(const char* file, int line) {
    /* Users can re-implement this function to print information */
    DDL_Printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    for (;;) {}
}
#endif /* __DEBUG */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
