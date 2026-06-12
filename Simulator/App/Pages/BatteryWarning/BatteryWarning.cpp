#include "BatteryWarning.h"

#include "Resource/ResourcePool.h"
#include "Utils/I18n/I18n.h"
#include "mcu_config.h"

namespace {

constexpr float kCriticalTemperatureC = 65.0f;
constexpr float kClearTemperatureC = 60.0f;
constexpr lv_coord_t kStatusBarHeight = 26;
constexpr lv_coord_t kBannerHeight = 28;

lv_obj_t* warningBanner = nullptr;
lv_obj_t* warningLabel = nullptr;
bool warningActive = false;

void
UpdateWarningText() {
    if (warningLabel == nullptr) {
        return;
    }

    lv_label_set_text_fmt(warningLabel, I18n::Text(I18n::TextId::BatteryTemperatureCritical),
                          systemInfo.powerMonitor.batteryInfo.Temp_f);
}

} // namespace

lv_obj_t*
Page::BatteryWarning_Create(lv_obj_t* parent) {
    warningBanner = lv_obj_create(parent);
    lv_obj_remove_style_all(warningBanner);
    lv_obj_set_size(warningBanner, LV_HOR_RES - 8, kBannerHeight);
    lv_obj_set_pos(warningBanner, 4, kStatusBarHeight + 1);
    lv_obj_clear_flag(warningBanner, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(warningBanner, lv_color_hex(0x8F241E), 0);
    lv_obj_set_style_bg_grad_color(warningBanner, lv_color_hex(0xD9583E), 0);
    lv_obj_set_style_bg_grad_dir(warningBanner, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_bg_opa(warningBanner, LV_OPA_90, 0);
    lv_obj_set_style_border_color(warningBanner, lv_color_hex(0xF6D062), 0);
    lv_obj_set_style_border_width(warningBanner, 1, 0);
    lv_obj_set_style_radius(warningBanner, 3, 0);

    warningLabel = lv_label_create(warningBanner);
    lv_obj_remove_style_all(warningLabel);
    lv_obj_set_width(warningLabel, LV_HOR_RES - 20);
    lv_obj_clear_flag(warningLabel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_label_set_long_mode(warningLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_align(warningLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(warningLabel, lv_color_white(), 0);
    lv_obj_set_style_text_font(warningLabel, ResourcePool::GetFont("oswaldBold_12"), 0);
    lv_obj_center(warningLabel);

    UpdateWarningText();
    lv_obj_add_flag(warningBanner, LV_OBJ_FLAG_HIDDEN);
    return warningBanner;
}

void
Page::BatteryWarning_Update() {
    if (warningBanner == nullptr) {
        return;
    }

    const float temperatureC = systemInfo.powerMonitor.batteryInfo.Temp_f;
    const bool shouldBeActive =
        warningActive ? temperatureC >= kClearTemperatureC : temperatureC >= kCriticalTemperatureC;

    if (shouldBeActive != warningActive) {
        warningActive = shouldBeActive;
        if (warningActive) {
            UpdateWarningText();
            lv_obj_clear_flag(warningBanner, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(warningBanner);
        } else {
            lv_obj_add_flag(warningBanner, LV_OBJ_FLAG_HIDDEN);
        }
        return;
    }

    if (warningActive) {
        UpdateWarningText();
    }
}

void
Page::BatteryWarning_ApplyLanguage() {
    UpdateWarningText();
}
