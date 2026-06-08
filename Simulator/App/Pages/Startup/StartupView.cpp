#include "StartupView.h"
#include <cstdio>

using namespace Page;

namespace {

constexpr lv_coord_t kStatusBarHeight = 26;
constexpr lv_color_t kBackground = LV_COLOR_MAKE(0x15, 0x15, 0x13);
constexpr lv_color_t kPanel = LV_COLOR_MAKE(0x22, 0x22, 0x1E);
constexpr lv_color_t kAmber = LV_COLOR_MAKE(0xE1, 0xAA, 0x22);
constexpr lv_color_t kAmberBright = LV_COLOR_MAKE(0xF6, 0xD0, 0x62);
constexpr lv_color_t kLineGrey = LV_COLOR_MAKE(0x72, 0x6E, 0x63);
constexpr lv_color_t kTextWarm = LV_COLOR_MAKE(0xE8, 0xE1, 0xCF);
constexpr lv_color_t kDanger = LV_COLOR_MAKE(0xD9, 0x58, 0x3E);

const char*
GetPowerStateText() {
    if (systemInfo.powerMonitor.ExternalPower == 1) {
        return "EXT";
    }
    if (systemInfo.powerMonitor.batteryInfo.chargeStatus != notCharge) {
        return "CHG";
    }
    return "BAT";
}

lv_color_t
GetBatteryColor() {
    const uint16_t percent = systemInfo.powerMonitor.batteryInfo.Percent;
    if (percent <= 20 && systemInfo.powerMonitor.ExternalPower != 1) {
        return kDanger;
    }
    return kAmberBright;
}

lv_obj_t*
CreateInfoLabel(lv_obj_t* parent, const lv_font_t* font, const char* title, const char* value, lv_align_t align,
                lv_coord_t x, lv_coord_t y, lv_color_t valueColor = kTextWarm) {
    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 72, 23);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(cont, align, x, y);

    lv_obj_t* titleLabel = lv_label_create(cont);
    lv_obj_remove_style_all(titleLabel);
    lv_obj_set_style_text_font(titleLabel, font, 0);
    lv_obj_set_style_text_color(titleLabel, kLineGrey, 0);
    lv_obj_set_style_text_letter_space(titleLabel, 1, 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t* valueLabel = lv_label_create(cont);
    lv_obj_remove_style_all(valueLabel);
    lv_obj_set_width(valueLabel, 72);
    lv_label_set_long_mode(valueLabel, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_font(valueLabel, font, 0);
    lv_obj_set_style_text_color(valueLabel, valueColor, 0);
    lv_obj_set_style_text_letter_space(valueLabel, 1, 0);
    lv_label_set_text(valueLabel, value);
    lv_obj_align(valueLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    return cont;
}

} // namespace

void
StartupView::Create(lv_obj_t* root) {
    const lv_font_t* font_small = ResourcePool::GetFont("oswaldBold_12");
    const lv_font_t* font_main = ResourcePool::GetFont("oswaldBold_18");

    lv_obj_set_style_bg_color(root, kBackground, 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);

    lv_obj_t* content = lv_obj_create(root);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, LV_HOR_RES, LV_VER_RES - kStatusBarHeight);
    lv_obj_set_pos(content, 0, kStatusBarHeight);
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(content, kBackground, 0);
    lv_obj_set_style_bg_grad_color(content, kPanel, 0);
    lv_obj_set_style_bg_grad_dir(content, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_bg_opa(content, LV_OPA_COVER, 0);

    char batteryText[8];
    const uint16_t batteryPercent = LV_MIN(systemInfo.powerMonitor.batteryInfo.Percent, 100);
    std::snprintf(batteryText, sizeof(batteryText), "%u%%", batteryPercent);

    CreateInfoLabel(content, font_small, "FW", systemInfo.software_version, LV_ALIGN_TOP_LEFT, 18, 15);
    CreateInfoLabel(content, font_small, "HW", systemInfo.hardware_version, LV_ALIGN_TOP_LEFT, 18, 54);
    CreateInfoLabel(content, font_small, "BAT", batteryText, LV_ALIGN_TOP_RIGHT, -8, 15, GetBatteryColor());
    CreateInfoLabel(content, font_small, "PWR", GetPowerStateText(), LV_ALIGN_TOP_RIGHT, -8, 54);

    lv_obj_t* accent = lv_obj_create(content);
    lv_obj_remove_style_all(accent);
    lv_obj_set_size(accent, 42, 2);
    lv_obj_set_style_bg_color(accent, kAmber, 0);
    lv_obj_set_style_bg_grad_color(accent, kLineGrey, 0);
    lv_obj_set_style_bg_grad_dir(accent, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_bg_opa(accent, LV_OPA_70, 0);
    lv_obj_align(accent, LV_ALIGN_TOP_MID, 0, 5);

    constexpr lv_coord_t arcSize = 62;
    lv_obj_t* arc = lv_arc_create(content);
    lv_obj_set_size(arc, arcSize, arcSize);
    lv_obj_align(arc, LV_ALIGN_TOP_MID, 0, 10);
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 0);
    lv_arc_set_bg_angles(arc, 135, 45);
    lv_arc_set_rotation(arc, 90);
    lv_obj_remove_style(arc, nullptr, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_arc_width(arc, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 5, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, kLineGrey, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, kAmberBright, LV_PART_INDICATOR);
    ui.arc = arc;

    lv_obj_t* labelPercent = lv_label_create(content);
    lv_obj_remove_style_all(labelPercent);
    lv_label_set_text(labelPercent, "0%");
    lv_obj_set_style_text_font(labelPercent, font_main, 0);
    lv_obj_set_style_text_color(labelPercent, kTextWarm, 0);
    lv_obj_align_to(labelPercent, arc, LV_ALIGN_CENTER, 0, -1);
    ui.arc_percent = labelPercent;

    lv_anim_init(&ui.arc_anim);
    lv_anim_set_var(&ui.arc_anim, ui.arc);
    lv_anim_set_time(&ui.arc_anim, 200);
    lv_anim_set_playback_time(&ui.arc_anim, 0);
    lv_anim_set_repeat_count(&ui.arc_anim, 0);
    lv_anim_set_path_cb(&ui.arc_anim, lv_anim_path_linear);

    lv_obj_t* btnPress = lv_obj_create(content);
    lv_obj_remove_style_all(btnPress);
    lv_obj_set_size(btnPress, 76, 22);
    lv_obj_clear_flag(btnPress, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(btnPress, kAmber, 0);
    lv_obj_set_style_bg_opa(btnPress, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_side(btnPress, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_color(btnPress, kAmberBright, 0);
    lv_obj_set_style_border_width(btnPress, 2, 0);
    lv_obj_set_style_border_opa(btnPress, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(btnPress, 2, 0);

    lv_obj_set_style_border_opa(btnPress, LV_OPA_COVER, LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(btnPress, LV_OPA_10, LV_STATE_FOCUSED);

    lv_obj_set_style_width(btnPress, 64, LV_STATE_PRESSED);
    lv_obj_set_style_translate_y(btnPress, 1, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, kAmberBright, LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(btnPress, LV_OPA_20, LV_STATE_PRESSED);
    lv_obj_set_style_border_color(btnPress, kAmberBright, LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btnPress, 3, LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btnPress, LV_OPA_COVER, LV_STATE_PRESSED);

    lv_obj_align(btnPress, LV_ALIGN_BOTTOM_MID, 0, -4);
    ui.btnPress = btnPress;

    lv_obj_t* labelBtn = lv_label_create(btnPress);
    lv_obj_remove_style_all(labelBtn);
    lv_obj_set_style_text_font(labelBtn, font_small, 0);
    lv_obj_set_style_text_color(labelBtn, kTextWarm, 0);
    lv_obj_set_style_text_opa(labelBtn, LV_OPA_80, 0);
    lv_obj_set_style_text_letter_space(labelBtn, 1, 0);
    lv_obj_center(labelBtn);
    ui.btnLabel = labelBtn;

    static lv_style_transition_dsc_t focusTransition;
    static constexpr lv_style_prop_t focusProps[] = {
        LV_STYLE_BG_OPA, LV_STYLE_BORDER_OPA,  LV_STYLE_BORDER_WIDTH,
        LV_STYLE_WIDTH,  LV_STYLE_TRANSLATE_Y, LV_STYLE_PROP_INV,
    };
    lv_style_transition_dsc_init(&focusTransition, focusProps, lv_anim_path_ease_out, 120, 0, nullptr);
    lv_obj_set_style_transition(btnPress, &focusTransition, LV_STATE_FOCUSED);
    lv_obj_set_style_transition(btnPress, &focusTransition, LV_STATE_PRESSED);

    ApplyLanguage();
}

void
StartupView::Delete() const {}

void
StartupView::Update() {}

void
StartupView::ApplyLanguage() const {
    lv_label_set_text(ui.btnLabel, I18n::Text(I18n::TextId::Press));
}
