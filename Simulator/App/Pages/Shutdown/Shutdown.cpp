#include "Shutdown.h"

#include "HAL.h"

using namespace Page;

static bool anim_complement_callback_do = false;

static void lv_anim_obj_set_width(void *obj, int32_t width) {
    lv_anim_t *anim = lv_anim_get(obj, lv_anim_obj_set_width);
    if (!anim) return;

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
            lv_obj_clear_flag(instance->View.ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
            lv_obj_fade_in(instance->View.ui.sync.cont, 300, 0);
            lv_anim_timeline_start(instance->View.ui.anim_timeline);
            lv_anim_start(&instance->View.ui.sync.bar.anim);
            HAL::Power_Shutdown(false);
#if defined(_WIN32)
#else
            HAL::Encoder_SetEnable(false);
#endif
        }
    }
}

static void syncbar_timer_callback(lv_timer_t *timer) {
    const auto *instance = static_cast<Shutdown *>(timer->user_data);
    LV_ASSERT_NULL(instance);
    memset(&systemInfo, 0, sizeof(systemInfo));
#if defined(_WIN32)
    LV_LOG_INFO("Shutdonw done\n");
#else
    shared_info.reset_count = 0x2026;
    shared_info.command = CMD_SKIP_DELAY;
    NVIC_SystemReset();
#endif
}

static void syncbar_anim_done_callback(lv_anim_t *a) {
    auto *instance = static_cast<Shutdown *>(lv_anim_get_user_data(a));
    LV_ASSERT_NULL(instance);
    HAL::Power_Shutdown(true);
    lv_timer_t *timer = lv_timer_create(syncbar_timer_callback, 1000, instance);
    lv_timer_set_repeat_count(timer, 1);
}

// static void softreset_anim_done_callback(lv_anim_t *a) {
// #if defined(_WIN32)
//     PM_LOG_INFO("SoftReset");
// #else
//     NVIC_SystemReset();
// #endif
// }

void Shutdown::onCustomAttrConfig() {
    SetCustomCacheEnable(true);
}

void Shutdown::onViewLoad() {
    View.Create(_root);
    lv_anim_set_exec_cb(&View.ui.shutdown.bar.anim, lv_anim_obj_set_width);
    lv_anim_set_user_data(&View.ui.shutdown.bar.anim, this);

    lv_anim_set_deleted_cb(&View.ui.sync.bar.anim, syncbar_anim_done_callback);
    lv_anim_set_user_data(&View.ui.sync.bar.anim, this);

    AttachEvent(View.ui.shutdown.btnPress);
}

void Shutdown::onViewDidLoad() {
    timer = lv_timer_create(onTimerUpdate, 1000, this);
}

void Shutdown::onViewWillAppear() {
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_add_obj(group, View.ui.shutdown.btnPress);
    lv_group_focus_obj(View.ui.shutdown.btnPress);
}

void Shutdown::onViewDidAppear() {
}

void Shutdown::onViewWillDisappear() {
}

void Shutdown::onViewDidDisappear() {
    if (timer != nullptr) {
        lv_timer_del(timer);
        timer = nullptr;
    }
}

void Shutdown::onViewUnload() {
    anim_complement_callback_do = false;
    View.Delete();
}

void Shutdown::onViewDidUnload() {
}

void Shutdown::AttachEvent(lv_obj_t *obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Shutdown::Update() const {
#if defined(_WIN32)
#else
    if (HAL::Power_ShutdownForce()) {
        lv_obj_fade_out(View.ui.shutdown.cont, 100, 0);
        Model.SetStatusBarDisappear(false);
        lv_obj_set_style_text_color(View.ui.sync.label, lv_palette_main(LV_PALETTE_RED), 0);
        lv_label_set_text(View.ui.sync.label, "Warming: Force Shutdown");
        lv_obj_clear_flag(View.ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_fade_in(View.ui.sync.cont, 300, 0);
        lv_anim_timeline_start(View.ui.anim_timeline);
        lv_anim_start(&View.ui.sync.bar.anim);
        HAL::Encoder_SetEnable(false);
    } else if (HAL::Power_ShutdownLinux()) {
        lv_obj_fade_out(View.ui.shutdown.cont, 100, 0);
        Model.SetStatusBarDisappear(false);
        lv_label_set_text(View.ui.sync.label, "Info: Linux Shutdown");
        lv_obj_clear_flag(View.ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_fade_in(View.ui.sync.cont, 300, 0);
        lv_anim_timeline_start(View.ui.anim_timeline);
        lv_anim_start(&View.ui.sync.bar.anim);
        HAL::Encoder_SetEnable(false);
    } else if (HAL::Power_ShutdownLowBattery()) {
        lv_obj_fade_out(View.ui.shutdown.cont, 100, 0);
        Model.SetStatusBarDisappear(false);
        lv_obj_set_style_text_color(View.ui.sync.label, lv_palette_main(LV_PALETTE_RED), 0);
        lv_label_set_text(View.ui.sync.label, "Warming: LowBattery Shutdown");
        lv_obj_clear_flag(View.ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_fade_in(View.ui.sync.cont, 300, 0);
        lv_anim_timeline_start(View.ui.anim_timeline);
        lv_anim_start(&View.ui.sync.bar.anim);
        HAL::Encoder_SetEnable(false);
    }
#endif
}

void Shutdown::onTimerUpdate(lv_timer_t *timer) {
    auto *instance = static_cast<Shutdown *>(timer->user_data);
    instance->Update();
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
            }
            default: ;
        }
    }
}
