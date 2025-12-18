#include "StartupView.h"

using namespace Page;



void StartupView::Create(lv_obj_t *root) {
    const lv_font_t *font = ResourcePool::GetFont("oswaldBold_18");

    constexpr lv_coord_t arc_size = 126 * 0.6;
    lv_obj_t *arc = lv_arc_create(root);
    lv_obj_set_size(arc, arc_size, arc_size);
    lv_obj_center(arc);
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 0);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, nullptr, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x505050), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    ui.arc = arc;

    lv_obj_t *label_percent = lv_label_create(root);
    lv_label_set_text(label_percent, "0%");
    lv_obj_center(label_percent);
    lv_obj_set_style_text_font(label_percent, font, 0);
    ui.arc_percent = label_percent;

    lv_anim_init(&ui.arc_anim);
    lv_anim_set_var(&ui.arc_anim, ui.arc);
    lv_anim_set_time(&ui.arc_anim, 200);
    lv_anim_set_playback_time(&ui.arc_anim, 0);
    lv_anim_set_repeat_count(&ui.arc_anim, 0);
    lv_anim_set_path_cb(&ui.arc_anim, lv_anim_path_linear);

    lv_obj_t* btnPress = lv_obj_create(root);
    lv_obj_remove_style_all(btnPress);
    lv_obj_set_size(btnPress, 55, 40);
    lv_obj_clear_flag(btnPress, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(btnPress, LV_ALIGN_CENTER, 80, 0);

    lv_obj_set_style_bg_opa(btnPress, LV_OPA_COVER, 0);
    lv_obj_set_style_width(btnPress, 60, LV_STATE_PRESSED);
    lv_obj_set_style_height(btnPress, 35, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xdd3c3b), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(btnPress, 9, 0);

    static lv_style_transition_dsc_t tran;
    static constexpr lv_style_prop_t prop[] = { LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
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
    ui.btnPress = btnPress;

    lv_obj_t *label_btn = lv_label_create(btnPress);
    lv_obj_remove_style_all(label_btn);
    lv_obj_set_style_text_font(label_btn, font, 0);
    lv_label_set_text(label_btn, "PRESS");
    lv_obj_center(label_btn);

    // Firmware version label
    lv_obj_t *label_firmware = lv_label_create(root);
    lv_label_set_text_fmt(label_firmware, "Rev:%s", SOFTWARE_VERSION);
    lv_obj_set_style_text_font(label_firmware,
                               ResourcePool::GetFont("oswaldBold_12"), 0);
    lv_obj_set_style_text_color(label_firmware, lv_palette_main(LV_PALETTE_GREY),
                                0);
    lv_obj_align(label_firmware, LV_ALIGN_BOTTOM_LEFT, 10, 0);

    // Compile time label
    lv_obj_t *label_compile_time = lv_label_create(root);
    lv_label_set_text(label_compile_time, __DATE__ " " __TIME__);
    lv_obj_set_style_text_font(label_compile_time,
                               ResourcePool::GetFont("oswaldBold_12"), 0);
    lv_obj_set_style_text_color(label_compile_time,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_compile_time, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
}

void StartupView::Delete() const {

}

void StartupView::Update() {

}
