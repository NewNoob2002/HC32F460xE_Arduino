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

static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map) {
    if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

        int32_t x;
        int32_t y;
        for(y = area->y1; y <= area->y2; y++) {
            for(x = area->x1; x <= area->x2; x++) {
                /*Put a pixel to the display. For example:*/
								uint16_t color = (px_map[1] << 8) | px_map[0];
                screen.drawPixel(x, y, color);
                px_map += 2;
            }
        }
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