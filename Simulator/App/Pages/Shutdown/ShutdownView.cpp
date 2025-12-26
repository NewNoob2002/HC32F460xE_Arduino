#include "ShutdownView.h"

using namespace Page;

void ShutdownView::Create(lv_obj_t *root) {
    const lv_font_t *font = ResourcePool::GetFont("oswaldBold_18");
    const lv_font_t *font_small = ResourcePool::GetFont("oswaldBold_12");

    lv_obj_t *main_cont = lv_obj_create(root);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(main_cont);
    ui.shutdown.cont = main_cont;

    lv_obj_t *label = lv_label_create(main_cont);
    lv_obj_remove_style_all(label);
    lv_obj_set_style_text_font(label, font_small, 0);
    lv_label_set_text(label, "Keep Press Shutdown\nShort Press Return");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t *cont = lv_obj_create(main_cont);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 100, 10);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 10);
    ui.shutdown.bar.cont = cont;

    lv_obj_t *bar = lv_obj_create(cont);
    lv_obj_remove_style_all(bar);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0xf9181c), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_width(bar, 0);
    lv_obj_set_height(bar, 10);
    lv_obj_align(bar, LV_ALIGN_LEFT_MID, 0, 0);
    ui.shutdown.bar.obj = bar;

    lv_anim_init(&ui.shutdown.bar.anim);
    lv_anim_set_var(&ui.shutdown.bar.anim, bar);
    lv_anim_set_playback_time(&ui.shutdown.bar.anim, 0);
    lv_anim_set_repeat_count(&ui.shutdown.bar.anim, 0);

    lv_obj_t *barPercent = lv_label_create(main_cont);
    lv_obj_remove_style_all(barPercent);
    lv_obj_set_style_text_font(barPercent, font_small, 0);
    lv_label_set_text(barPercent, "0%");
    lv_obj_align(barPercent, LV_ALIGN_CENTER, 70, 10);
    ui.shutdown.bar.label = barPercent;

    lv_obj_t *btnPress = lv_obj_create(main_cont);
    lv_obj_remove_style_all(btnPress);
    lv_obj_set_size(btnPress, 50, 30);
    lv_obj_clear_flag(btnPress, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(btnPress, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_set_style_bg_opa(btnPress, LV_OPA_COVER, 0);
    lv_obj_set_style_width(btnPress, 60, LV_STATE_PRESSED);
    lv_obj_set_style_height(btnPress, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xdd3c3b), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(btnPress, 9, 0);

    static lv_style_transition_dsc_t tran;
    static constexpr lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        200,
        0,
        nullptr
    );
    lv_obj_set_style_transition(btnPress, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(btnPress, &tran, LV_STATE_FOCUSED);
    lv_obj_update_layout(btnPress);
    ui.shutdown.btnPress = btnPress;

    lv_obj_t *label_btn = lv_label_create(btnPress);
    lv_obj_remove_style_all(label_btn);
    lv_obj_set_style_text_font(label_btn, font, 0);
    lv_label_set_text(label_btn, "PRESS");
    lv_obj_center(label_btn);

    /*
     * Second screen;
     */
    lv_obj_t *second_cont = lv_obj_create(root);
    lv_obj_remove_style_all(second_cont);
    lv_obj_set_size(second_cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(second_cont);
    lv_obj_add_flag(second_cont, LV_OBJ_FLAG_HIDDEN);
    ui.sync.cont = second_cont;

    lv_obj_t *sync_label = lv_label_create(second_cont);
    lv_obj_set_style_text_font(sync_label, ResourcePool::GetFont("oswaldBold_18"), 0);
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
    lv_anim_set_time(&ui.sync.bar.anim, 8000);
    lv_anim_set_playback_time(&ui.sync.bar.anim, 0);
    lv_anim_set_repeat_count(&ui.sync.bar.anim, 0);

    ui.anim_timeline = lv_anim_timeline_create();
#define ANIM_DEF(start_time, obj, attr, start, end) \
{start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    const lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, bar_cont, width, 0, lv_obj_get_style_width(bar_cont, 0)),
        ANIM_DEF(300, sync_label, y, lv_obj_get_style_height(second_cont, 0), lv_obj_get_y(sync_label)),
        LV_ANIM_TIMELINE_WRAPPER_END
    };

    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void ShutdownView::Delete() {
    if (ui.anim_timeline) {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}
