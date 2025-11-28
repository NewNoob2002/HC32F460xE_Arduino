#include "lv_port.h"
#include "Arduino.h"
#include "Adafruit_ST7789.h"

typedef Adafruit_ST7789 SCREEN_CLASS;

static SCREEN_CLASS screen(
	  &CONFIG_SCREEN_SPI,
    CONFIG_SCREEN_CS_PIN,
    CONFIG_SCREEN_DC_PIN,
    CONFIG_SCREEN_RST_PIN
);

#define SCREEN_BUFFER_SIZE (CONFIG_SCREEN_HOR_RES * CONFIG_SCREEN_VER_RES)

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

void spi_trans(void *buf, uint16_t len)
{
	DMA_ChCmd(DMA_UNIT, DMA_TX_CH, DISABLE);
	DMA_SetSrcAddr(DMA_UNIT, DMA_TX_CH, (uint32_t)buf);
	DMA_SetTransCount(DMA_UNIT, DMA_TX_CH, len);
	
	DMA_ChCmd(DMA_UNIT, DMA_TX_CH, ENABLE);
	
	SPI_Cmd(SPI_CLASS_3_SPI, ENABLE);
	while(DMA_GetTransCompleteStatus(DMA_UNIT, DMA_FLAG_TC_CH0) == RESET)
	{
	}
	DMA_ClearTransCompleteStatus(DMA_UNIT, DMA_FLAG_TC_CH0);
	
////	/* Disable SPI */
//  SPI_Cmd(SPI_CLASS_3_SPI, DISABLE);
}

static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map) {
    if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

        int32_t x;
        int32_t y;
				const lv_coord_t w = (area->x2 - area->x1 + 1);
				const lv_coord_t h = (area->y2 - area->y1 + 1);
				const uint32_t len = w * h * 2;
			
				screen.setAddrWindow(area->x1, area->y1, w, h);
				digitalWrite(CONFIG_SCREEN_CS_PIN, LOW);
				digitalWrite(CONFIG_SCREEN_DC_PIN, HIGH);
				spi_trans((uint8_t *)px_map, len);
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);
}

void lv_port_disp_init()
{
	screen.init(CONFIG_SCREEN_VER_RES, CONFIG_SCREEN_HOR_RES);
	screen.fillScreen(ST77XX_BLACK);
	pinMode(CONFIG_SCREEN_BLK_PIN, OUTPUT);
	digitalWrite(CONFIG_SCREEN_BLK_PIN, LOW);
	
	/*DMA init*/
	stc_dma_init_t stcDmaInit;
	
  FCG_Fcg0PeriphClockCmd(DMA_CLK, ENABLE);
  (void)DMA_StructInit(&stcDmaInit);
  stcDmaInit.u32BlockSize  = 1UL;
  stcDmaInit.u32TransCount = 65535;
  stcDmaInit.u32DataWidth  = DMA_DATAWIDTH_8BIT;
  /* Configure TX */
  stcDmaInit.u32SrcAddrInc  = DMA_SRC_ADDR_INC;
  stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
  stcDmaInit.u32SrcAddr     = (uint32_t)(0);
  stcDmaInit.u32DestAddr    = (uint32_t)(&SPI_CLASS_3_SPI->DR);
  if (LL_OK != DMA_Init(DMA_UNIT, DMA_TX_CH, &stcDmaInit)) {
      for (;;) {
      }
  }
  AOS_SetTriggerEventSrc(DMA_TX_TRIG_CH, SPI_TX_EVT_SRC);
	
	DMA_Cmd(DMA_UNIT, ENABLE);
  DMA_ChCmd(DMA_UNIT, DMA_TX_CH, ENABLE);
	/*------------------------------------
  * Create a display and set a flush_cb
  * -----------------------------------*/
  lv_display_t * disp = lv_display_create(screen.width(), screen.height());
  lv_display_set_flush_cb(disp, disp_flush);
	
  /* Example 3
   * Two buffers screen sized buffer for double buffering.
   * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
  LV_ATTRIBUTE_MEM_ALIGN
  static uint8_t buf_3_1[SCREEN_BUFFER_SIZE * 2];

  LV_ATTRIBUTE_MEM_ALIGN
  static uint8_t buf_3_2[SCREEN_BUFFER_SIZE * 2];
  lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);
}