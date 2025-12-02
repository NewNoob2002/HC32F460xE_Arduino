#pragma once

/* USART unit definition */
#define USART_UNIT          (CM_USART1)
#define USART_FCG_ENABLE()  (FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USART1, ENABLE))

#define PRINTF_PORT         GPIO_PORT_A
#define PRINTF_PIN          GPIO_PIN_15
#define PRINTF_TX_GPIO_FUNC (GPIO_FUNC_32)

/* SPI Class */
#define SPI_CLASS_3_ENABLE 1
#if SPI_CLASS_3_ENABLE
#define SPI_CLASS_3_SPI CM_SPI3
#endif

/* Screen */
#define CONFIG_SCREEN_CS_PIN   PH2
#define CONFIG_SCREEN_DC_PIN   PB8
#define CONFIG_SCREEN_RST_PIN  PB9
#define CONFIG_SCREEN_SCK_PIN  PB6
#define CONFIG_SCREEN_MOSI_PIN PB7
#define CONFIG_SCREEN_BLK_PIN  PB3 // TIM3
#define CONFIG_SCREEN_SPI      SPI_3
#define CONFIG_SCREEN_HOR_RES  172
#define CONFIG_SCREEN_VER_RES  320

/* DMA definition */
#define DMA_UNIT                        (CM_DMA1)
#define DMA_CLK                         (FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS)
#define DMA_TX_CH                       (DMA_CH0)
#define DMA_TX_TRIG_CH                  (AOS_DMA1_0)

#define SPI_TX_EVT_SRC                  (EVT_SRC_SPI3_SPTI)