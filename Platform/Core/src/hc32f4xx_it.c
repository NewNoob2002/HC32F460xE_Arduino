#include "HAL.h"
#include "lvgl.h"
#include "mcu_define.h"

static void HardFault_HandlerC(uint32_t* stack_frame) __attribute__((used, noinline));

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void
NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {}
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Memory management fault.
 */
void
MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void
BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void
UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void
SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void
DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void
PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

__attribute__((naked)) void
HardFault_Handler(void) {
    __asm volatile("TST LR, #4      \n"
                   "ITE EQ          \n"
                   "MRSEQ R0, MSP   \n"
                   "MRSNE R0, PSP   \n"
                   "B HardFault_HandlerC \n");
}

static void
HardFault_HandlerC(uint32_t* stack_frame) {
    shared_info.magic = SHARED_MAGIC_CRASH;
    shared_info.command = CMD_NORMAL_BOOT;
    shared_info.crash_pc = stack_frame[6];
    shared_info.reset_count++;

    CORE_DEBUG_PRINTF("HardFault PC=0x%08lx LR=0x%08lx CFSR=0x%08lx HFSR=0x%08lx", stack_frame[6], stack_frame[5],
                      SCB->CFSR, SCB->HFSR);

    __disable_irq();
    while (1) {}
}

void
SysTick_Handler() {
    HAL_IncTick();
    lv_tick_inc(1);
    systemInfo.i2c_communicate_err_count++;
}
