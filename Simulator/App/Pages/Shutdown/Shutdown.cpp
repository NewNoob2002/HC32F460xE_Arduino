#include "Shutdown.h"

#include "HAL.h"

using namespace Page;

static bool anim_complement_callback_do = false;

static void lv_anim_obj_set_width(void *obj, const int32_t width) {
    lv_anim_t *anim = lv_anim_get(obj, lv_anim_obj_set_width);
    if (anim == nullptr) return;

    const auto *instance = static_cast<Shutdown *>(lv_anim_get_user_data(anim));
    LV_ASSERT_NULL(instance);

    lv_obj_set_width(static_cast<lv_obj_t *>(obj), width);

    if (instance->View.ui.shutdown.bar.label) {
        lv_label_set_text_fmt(instance->View.ui.shutdown.bar.label, "%d%%", width);
    }
    if (width >= 100) {
        if (!anim_complement_callback_do) {
            anim_complement_callback_do = true;
            lv_obj_fade_out(instance->View.ui.shutdown.cont, 300, 0);
            HAL::Power_Shutdown(false);
            instance->pageManager->Push("Pages/SaveConfig");
#if defined(_WIN32)
#else
            HAL::Encoder_SetEnable(false);
#endif
        }
    }
}

void Shutdown::onCustomAttrConfig() {
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(true);
}

void Shutdown::onViewLoad() {
    LV_LOG_USER("ViewLoad");
    View.Create(_root);
    lv_anim_set_exec_cb(&View.ui.shutdown.bar.anim, lv_anim_obj_set_width);
    lv_anim_set_user_data(&View.ui.shutdown.bar.anim, this);

    AttachEvent(View.ui.shutdown.btnPress);
}

void Shutdown::onViewDidLoad() {
    LV_LOG_USER("onViewDidLoad");
}

void Shutdown::onViewWillAppear() {
    LV_LOG_USER("onViewWillAppear");
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_add_obj(group, View.ui.shutdown.btnPress);
    lv_group_focus_obj(View.ui.shutdown.btnPress);
}

void Shutdown::onViewDidAppear() {
    LV_LOG_USER("onViewDidAppear");
}

void Shutdown::onViewWillDisappear() {
    LV_LOG_USER("onViewWillDisappear");
}

void Shutdown::onViewDidDisappear() {
    LV_LOG_USER("onViewDidDisappear");
}

void Shutdown::onViewUnload() {
    LV_LOG_USER("onViewUnload");
    anim_complement_callback_do = false;
    View.Delete();
}

void Shutdown::onViewDidUnload() {
    LV_LOG_USER("onViewDidUnload");
}

void Shutdown::AttachEvent(lv_obj_t *obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Shutdown::onEvent(lv_event_t *event) {
    const auto *instance = static_cast<Shutdown *>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);
    if (const lv_obj_t *obj = lv_event_get_target(event); obj != instance->View.ui.shutdown.btnPress) return;

    if (const lv_event_code_t code = lv_event_get_code(event); code == LV_EVENT_SHORT_CLICKED) {
        instance->pageManager->Pop();
    } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESSED) {
#if defined(HC32F460)
        if (!systemInfo.online_device.eg25_board && !systemInfo.eg25_overtime) return;
#endif
        lv_anim_t anim = instance->View.ui.shutdown.bar.anim;
        const int16_t current_width = lv_obj_get_width(instance->View.ui.shutdown.bar.obj);
        constexpr uint32_t boot_time_ms = 2000; // 设定长按关机时间为2秒
        const uint32_t remaining_time = boot_time_ms * (100 - current_width) / 100;
        switch (code) {
            case LV_EVENT_PRESSED: {
                lv_anim_del(instance->View.ui.shutdown.bar.obj, lv_anim_obj_set_width);
                lv_anim_set_path_cb(&anim, lv_anim_path_linear);
                lv_anim_set_values(&anim, current_width, 100);
                lv_anim_set_time(&anim, remaining_time);
                lv_anim_start(&anim);
                break;
            }
            case LV_EVENT_RELEASED: {
                if (current_width < 100) {
                    lv_anim_del(instance->View.ui.shutdown.bar.obj, lv_anim_obj_set_width);

                    lv_anim_set_values(&anim, current_width, 0);
                    lv_anim_set_time(&anim, 300);
                    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
                    lv_anim_start(&anim);
                }
                break;
            }
            default: ;
        }
    }
}
