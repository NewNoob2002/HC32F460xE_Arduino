#include "lvgl_screen.h"

int screen_flag        = 1;
int last_screen_flag   = 1;
int screen_flag_change = 0;
int workstatus_change  = 0;

int screen_destition = 10;

/* Root style */
lv_style_t *RootDefaultStyle;

void LVGL_screen_init()
{
    /* Set screen style */
    lv_disp_t *disp_p = lv_disp_get_default();
    // lv_theme_t *theme = lv_theme_basic_init(disp_p);
    lv_theme_t *theme = lv_theme_default_init(
        disp_p, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
        true, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp_p, theme);
    static lv_style_t rootStyle;
    lv_style_init(&rootStyle);
    lv_style_set_width(&rootStyle, LV_HOR_RES);
    lv_style_set_height(&rootStyle, LV_VER_RES);
    lv_style_set_bg_color(&rootStyle, lv_color_black());
    lv_style_set_pad_all(&rootStyle, 0);
    lv_style_set_border_width(&rootStyle, 0);
    lv_style_set_radius(&rootStyle, 0);
    RootDefaultStyle = &rootStyle;
    lv_screen1_init();
    lv_screen2_init();
    lv_screen3_init();
    lv_screen4_init();
    lv_screen5_init();
    lv_screen6_init();
    lv_screen7_init();
    lv_screen8_init();
		lv_screen9_init();
}
