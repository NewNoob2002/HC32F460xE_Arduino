#include "lvgl_screen.h"

#define BAR_WIDTH   100

struct {
  lv_obj_t *cont;
  lv_obj_t *logo_label;
  lv_obj_t *bar_label;
  lv_anim_t bar_anim;
  lv_obj_t *bar_percent;
  lv_anim_timeline_t *anim_timeline;
  lv_anim_t anim_label;

  lv_obj_t *img_logo;
} ui;

lv_obj_t *scr8 = nullptr;
lv_timer_t *scr8_timer = nullptr;

static bool do_it_once = false;
static uint32_t first_check_time = 0;
static uint8_t check_time = 0;

void lv_screen8_init(void)
{
  scr8 = lv_obj_create(NULL);
	lv_obj_t *root = scr8;
	    lv_obj_t* cont_screen = lv_obj_create(root);
    lv_obj_remove_style_all(cont_screen);
    lv_obj_add_flag(cont_screen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(cont_screen, LV_HOR_RES, LV_VER_RES);
    ui.cont = cont_screen;

    lv_obj_t* cont = lv_obj_create(cont_screen);
    lv_obj_remove_style_all(cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, 240, 70);
    lv_obj_set_style_border_color(cont, lv_color_hex(0x8BCA93), 0);
    lv_obj_set_style_border_side(cont, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(cont, 3, 0);
    lv_obj_set_style_border_post(cont, true, 0);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_RED), 0);
    lv_label_set_text(label, "Warming: Don't Shutdown Now");
    lv_obj_center(label);
    ui.logo_label = label;

    lv_obj_t* bar = lv_obj_create(cont_screen);
    lv_obj_remove_style_all(bar);
    lv_obj_set_size(bar, 1, 10);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x8BCA93), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(bar, LV_OPA_COVER, 0);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -10);
    ui.bar_label = bar;

    lv_obj_t* bar_percent = lv_label_create(cont_screen);
    lv_obj_remove_style_all(bar_percent);
    lv_obj_set_style_text_font(bar_percent, &font_oswaldBold_12, 0);
    lv_label_set_text(bar_percent, "0%");
    lv_obj_align_to(bar_percent, bar, LV_ALIGN_OUT_RIGHT_MID, 80, 0);
    ui.bar_percent = bar_percent;

    lv_anim_init(&ui.bar_anim);
    lv_anim_set_var(&ui.bar_anim, ui.bar_label);
    lv_anim_set_values(&ui.bar_anim, 1, BAR_WIDTH);
    lv_anim_set_time(&ui.bar_anim, 1000);
    lv_anim_set_playback_time(&ui.bar_anim, 200);
    lv_anim_set_repeat_count(&ui.bar_anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&ui.bar_anim, lv_anim_path_linear);
    lv_anim_set_exec_cb(&ui.bar_anim, LV_ANIM_EXEC(width));

    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
{start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    const lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, cont, width, 0, lv_obj_get_style_width(cont, 0)),
        ANIM_DEF(500, ui.logo_label, y, lv_obj_get_style_height(ui.cont, 0), lv_obj_get_y(ui.logo_label)),
        LV_ANIM_TIMELINE_WRAPPER_END
    };

    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);


    lv_obj_t *img_logo = lv_img_create(root);
    lv_img_set_src(img_logo, &img_src_startupLogo);
    const auto *img_satellite_ext = reinterpret_cast<lv_img_t *>(img_logo);
    lv_obj_set_size(img_logo, img_satellite_ext->w, img_satellite_ext->h);
    lv_obj_center(img_logo);
    ui.img_logo = img_logo;
}

static void onTimer(lv_timer_t* timer)
{
    if(!do_it_once) {
        do_it_once = true;
        lv_obj_fade_out(ui.img_logo, 300, 0);
        lv_obj_clear_flag(ui.cont, LV_OBJ_FLAG_HIDDEN);
        lv_anim_timeline_start(ui.anim_timeline);
        lv_anim_start(&ui.bar_anim);
    }
    check_time += 5;
    lv_label_set_text_fmt(ui.bar_percent, "%d%%", check_time);
		if (lv_tick_get() - first_check_time >= 30000 || systemInfo.online_device.eg25_board) {
        CORE_DEBUG_PRINTF("push main page, [%ld]", lv_tick_get() - first_check_time);
				lv_screen8_disappear();
        lv_screen1_appear();
    }
}

void lv_screen8_appear()
{
	lv_scr_load_anim(scr8, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, false);
	if(scr8_timer == nullptr)
	{
		CORE_DEBUG_PRINTF("scr8_timer_create");
		scr8_timer = lv_timer_create(onTimer, 2000, nullptr);
		lv_timer_resume(scr8_timer);
	}
	first_check_time = lv_tick_get();
}

void lv_screen8_disappear(void)
{
	 if (scr8_timer) {
        lv_timer_del(scr8_timer);
    }
		lv_anim_del(&ui.bar_anim, nullptr);
    do_it_once = false;
    check_time = 0;
}