#include "lvgl_screen.h"

lv_obj_t *scr9         = nullptr;
lv_timer_t *scr9_timer = nullptr;

struct {
    lv_obj_t *cont;
    struct {
        lv_obj_t *cont;
        lv_obj_t *label;
        struct {
            lv_obj_t *obj;
            lv_anim_t anim;
        } bar;
    } sync;
    lv_anim_timeline_t *anim_timeline;
} ui;

static bool anim_complement_callback_do = false;
extern volatile SharedData_t shared_info;

static void syncbar_timer_callback(lv_timer_t *timer)
{
    //	shared_info.command = CMD_SKIP_DELAY;
    //	shared_info.reset_count = 0x2006;
    lv_screen9_disappear();
    NVIC_SystemReset();
}

static void syncbar_anim_done_callback(lv_anim_t *a)
{
    HAL::Power_Shutdown(true);
    lv_timer_t *timer = lv_timer_create(syncbar_timer_callback, 1000, nullptr);
    lv_timer_set_repeat_count(timer, 1);
}

void lv_screen9_init(void)
{
    scr9 = lv_obj_create(NULL);
    if (RootDefaultStyle) {
        lv_obj_add_style(scr9, RootDefaultStyle, LV_PART_MAIN);
    }
    const lv_font_t *font       = &font_oswaldBold_18;
    const lv_font_t *font_small = &font_oswaldBold_12;

    lv_obj_t *root      = scr9;
    lv_obj_t *main_cont = lv_obj_create(root);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(main_cont);

    /*
     * Second screen;
     */
    lv_obj_t *second_cont = lv_obj_create(root);
    lv_obj_remove_style_all(second_cont);
    lv_obj_set_size(second_cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(second_cont);
    ui.sync.cont = second_cont;

    lv_obj_t *sync_label = lv_label_create(second_cont);
    lv_obj_set_style_text_font(sync_label, font, 0);
    lv_obj_set_style_text_color(sync_label, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_label_set_text(sync_label, "Warming: Saving SystemConfig");
    lv_obj_center(sync_label);
    ui.sync.label = sync_label;

    lv_obj_t *bar_cont = lv_obj_create(second_cont);
    lv_obj_remove_style_all(bar_cont);
    lv_obj_set_style_border_color(bar_cont, lv_color_hex(0x8BCA93), 0);
    lv_obj_set_style_border_width(bar_cont, 2, 0);
    lv_obj_set_size(bar_cont, 102, 12);
    lv_obj_align(bar_cont, LV_ALIGN_CENTER, 0, 40);

    lv_obj_t *sync_bar = lv_obj_create(bar_cont);
    lv_obj_remove_style_all(sync_bar);
    lv_obj_set_size(sync_bar, 0, 10);
    lv_obj_set_style_bg_color(sync_bar, lv_color_hex(0x8BCA93), 0);
    lv_obj_set_style_bg_opa(sync_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(sync_bar, LV_OPA_COVER, 0);
    lv_obj_align(sync_bar, LV_ALIGN_LEFT_MID, 0, 0);
    ui.sync.bar.obj = sync_bar;

    lv_anim_init(&ui.sync.bar.anim);
    lv_anim_set_var(&ui.sync.bar.anim, sync_bar);
    lv_anim_set_exec_cb(&ui.sync.bar.anim, LV_ANIM_EXEC(width));
    lv_anim_set_values(&ui.sync.bar.anim, 0, 100);
    lv_anim_set_time(&ui.sync.bar.anim, 10000);
    lv_anim_set_playback_time(&ui.sync.bar.anim, 0);
    lv_anim_set_repeat_count(&ui.sync.bar.anim, 0);
    lv_anim_set_deleted_cb(&ui.sync.bar.anim, syncbar_anim_done_callback);

    ui.anim_timeline = lv_anim_timeline_create();
#define ANIM_DEF(start_time, obj, attr, start, end)                                       \
    {                                                                                     \
        start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true \
    }

    const lv_anim_timeline_wrapper_t wrapper[] =
        {
            ANIM_DEF(0, bar_cont, width, 0, lv_obj_get_style_width(bar_cont, 0)),
            ANIM_DEF(300, sync_label, y, lv_obj_get_style_height(second_cont, 0), lv_obj_get_y(sync_label)),
            LV_ANIM_TIMELINE_WRAPPER_END};

    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void lv_shutdown_set_src(const char *src)
{
    lv_label_set_text_fmt(ui.sync.label, "%s", src);
}

void lv_screen9_appear()
{
    lv_anim_timeline_start(ui.anim_timeline);
    lv_anim_start(&ui.sync.bar.anim);
    lv_scr_load_anim(scr9, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    CORE_DEBUG_PRINTF("lv_screen9_appear");
}

void lv_screen9_disappear(void)
{
    if (ui.anim_timeline) {
        lv_anim_timeline_del(ui.anim_timeline);
    }
    CORE_DEBUG_PRINTF("lv_screen9_disappear");
}