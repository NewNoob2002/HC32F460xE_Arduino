#include "delay.h"
#include "dwt.h"
#include "yield.h"

uint32_t HAL_GetTick(void);

uint32_t millis(void)
{
  // ToDo: ensure no interrupts
  return HAL_GetTick();
}

// Interrupt-compatible version of micros
uint32_t micros(void)
{
  return 0;
}

void delay_ms(uint32_t ms)
{
	if (ms != 0) {
    uint32_t start = HAL_GetTick();
    do {
      yield();
    } while (HAL_GetTick() - start < ms);
  }
}

void delay_us(uint32_t us)
{
#if defined(DWT_BASE)
	if(dwt_getStatus())
	{
		int32_t start  = dwt_getCycles();
		int32_t cycles = us * (SystemCoreClock / 1000000);
		while ((int32_t)dwt_getCycles() - start < cycles);
	}
	else
	{
		  /* 2 NO OPERATION instructions */
		__asm volatile(" nop      \n\t"
								   " nop      \n\t");
	}
#else
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
#endif
}