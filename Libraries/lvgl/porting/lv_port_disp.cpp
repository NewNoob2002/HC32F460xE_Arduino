#include "Adafruit_ST7789.h"
#include "lv_port.h"

typedef Adafruit_ST7789 SCREEN_CLASS;

static SCREEN_CLASS screen(&CONFIG_SCREEN_SPI, CONFIG_SCREEN_CS_PIN, CONFIG_SCREEN_DC_PIN, CONFIG_SCREEN_RST_PIN);

#define SCREEN_BUFFER_SIZE (CONFIG_SCREEN_HOR_RES * CONFIG_SCREEN_VER_RES)

static lv_disp_drv_t* disp_drv_p = nullptr;
static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/

/* Example for 1) */
//    static lv_disp_draw_buf_t draw_buf_dsc_1;
//    static lv_color_t buf_1[SCREEN_BUFFER_SIZE] __attribute__((aligned(4)));
//    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, SCREEN_BUFFER_SIZE);   /*Initialize the display buffer*/
/* Example for 2) */
static lv_disp_draw_buf_t draw_buf_dsc_2;
static lv_color_t buf_2_1[SCREEN_BUFFER_SIZE / 4] __attribute__((aligned(4))); /*A buffer for 10 rows*/
static lv_color_t buf_2_2[SCREEN_BUFFER_SIZE / 4] __attribute__((aligned(4))); /*An other buffer for 10 rows*/

void
spi_dma_trans(void* buf, uint16_t len) {
    if (SPI_CLASS_3_SPI->CR1 & SPI_CR1_SPE) {
        SPI_Cmd(SPI_CLASS_3_SPI, DISABLE);
    }
    DMA_SetSrcAddr(DMA_UNIT, DMA_TX_CH, (uint32_t)buf);
    DMA_SetTransCount(DMA_UNIT, DMA_TX_CH, len);

    DMA_ChCmd(DMA_UNIT, DMA_TX_CH, ENABLE);

    SPI_Cmd(SPI_CLASS_3_SPI, ENABLE);
}

void
my_rounder_cb(lv_disp_drv_t* disp_drv, lv_area_t* area) {
    area->x1 = area->x1 & ~1;
    area->x2 = area->x2 | 1;
    area->y1 = area->y1 & ~1;
    area->y2 = area->y2 | 1;
}

static void
disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
    disp_drv_p = disp_drv;

    const lv_coord_t w = (area->x2 - area->x1 + 1);
    const lv_coord_t h = (area->y2 - area->y1 + 1);
    const uint32_t len = w * h;

    screen.setAddrWindow(area->x1, area->y1, w, h);
    digitalWrite(CONFIG_SCREEN_CS_PIN, LOW);
    digitalWrite(CONFIG_SCREEN_DC_PIN, HIGH);
    spi_dma_trans((uint16_t*)color_p, len * 2);
}

static void
DMA_TransCompleteCallback(void) {
    DMA_ClearTransCompleteStatus(DMA_UNIT, DMA_FLAG_TC_CH0);

    lv_disp_flush_ready(disp_drv_p);
}

void
lv_port_disp_init() {
    /*DMA init*/
    stc_dma_init_t stcDmaInit;

    FCG_Fcg0PeriphClockCmd(DMA_CLK, ENABLE);
    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCount = 65535;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    /* Configure TX */
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
    stcDmaInit.u32SrcAddr = (uint32_t)(0);
    stcDmaInit.u32DestAddr = (uint32_t)(&SPI_CLASS_3_SPI->DR);
    if (LL_OK != DMA_Init(DMA_UNIT, DMA_TX_CH, &stcDmaInit)) {
        for (;;) {}
    }
    AOS_SetTriggerEventSrc(DMA_TX_TRIG_CH, SPI_TX_EVT_SRC);

    stc_irq_signin_config_t stcIrqSignConfig;
    stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC0;
    stcIrqSignConfig.enIRQn = INT009_IRQn;
    stcIrqSignConfig.pfnCallback = &DMA_TransCompleteCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_04);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);

    DMA_Cmd(DMA_UNIT, ENABLE);
    DMA_ChCmd(DMA_UNIT, DMA_TX_CH, ENABLE);

    screen.init(CONFIG_SCREEN_VER_RES, CONFIG_SCREEN_HOR_RES);
    screen.fillScreen(ST77XX_BLACK);
#if defined(CONFIG_SCREEN_BLK_PIN)
    pinMode(CONFIG_SCREEN_BLK_PIN, OUTPUT);
    digitalWrite(CONFIG_SCREEN_BLK_PIN, LOW);
#endif
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, SCREEN_BUFFER_SIZE / 4); /*Initialize the display buffer*/
    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = screen.width();
    disp_drv.ver_res = screen.height();

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    disp_drv.rounder_cb = my_rounder_cb;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_90;
    /*Required for Example 3)*/
    //    disp_drv.full_refresh = 1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

void
lv_port_reset() {
    screen.init(CONFIG_SCREEN_VER_RES, CONFIG_SCREEN_HOR_RES);
    screen.fillScreen(ST77XX_BLACK);
}