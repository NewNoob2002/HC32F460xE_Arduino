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
    lv_anim_del(ui.arc, nullptr);
}

void StartupView::Update() {
    /*ARC*/
    uint8_t target_percent = (systemInfo.powerMonitor.PowerKey_PressCount * 2);
    target_percent = target_percent > 100 ? 100 : target_percent;
    static uint8_t last_target_percent = 0;
    if (target_percent != last_target_percent) {
        last_target_percent = target_percent;
        const int16_t current_value = lv_arc_get_value(ui.arc);
        lv_anim_set_values(&ui.arc_anim, current_value, target_percent);
        lv_anim_start(&ui.arc_anim);
    }
}
