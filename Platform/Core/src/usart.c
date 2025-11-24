#include "usart.h"
#include "hc32_ll.h"
#include "mcu_config.h"


static bool printf_init_done = false;

void USART_Init(void)
{
  stc_usart_uart_init_t stcUartInit;
  stc_irq_signin_config_t stcIrqSigninConfig;

  /* Configure USART TX pin. */
  GPIO_SetFunc(PRINTF_PORT, PRINTF_PIN, PRINTF_TX_GPIO_FUNC);

  /* Enable peripheral clock */
  USART_FCG_ENABLE();

  /* Initialize UART. */
  (void)USART_UART_StructInit(&stcUartInit);
  stcUartInit.u32ClockDiv = USART_CLK_DIV4;
  stcUartInit.u32Baudrate = 921600;
  stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
  if (LL_OK != USART_UART_Init(USART_UNIT, &stcUartInit, NULL))
  {
  }

  /* Register error IRQ handler && configure NVIC. */
//  stcIrqSigninConfig.enIRQn = USART_RX_ERR_IRQn;
//  stcIrqSigninConfig.enIntSrc = USART_RX_ERR_INT_SRC;
//  stcIrqSigninConfig.pfnCallback = &USART_RxError_IrqCallback;
//  (void)INTC_IrqSignIn(&stcIrqSigninConfig);
//  NVIC_ClearPendingIRQ(stcIrqSigninConfig.enIRQn);
//  NVIC_SetPriority(stcIrqSigninConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
//  NVIC_EnableIRQ(stcIrqSigninConfig.enIRQn);
	
		/* Enable RX/TX function */
	USART_FuncCmd(USART_UNIT, (USART_TX), ENABLE);
	
	printf_init_done = true;
}

size_t USART_PutC(char ch)
{
	if(!printf_init_done)
		return 0;
  while (RESET == USART_GetStatus(USART_UNIT, USART_FLAG_TX_EMPTY))
  {
    ;
  }
  USART_WriteData(USART_UNIT, ch);
	return 1;
}

size_t usart1_write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--)
  {
    if (USART_PutC(*buffer++))
      n++;
    else
      break;
  }
  return n;
}