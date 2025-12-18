#include "Shutdown.h"

using namespace Page;

static void lv_anim_obj_set_width(void *obj, int32_t width) {
    static bool anim_complement_callback_do = false;
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
            lv_obj_set_style_arc_color(static_cast<lv_obj_t *>(obj), lv_color_hex(0x70e958), LV_PART_MAIN);
#if defined(HC32F460)
#else
            if (instance->View.ui.shutdown.cont)
                lv_obj_fade_out(instance->View.ui.shutdown.cont, 300, 0);
            instance->Model.SetStatusBarDisappear(false);
            lv_obj_clear_flag(instance->View.ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
            lv_obj_fade_in(instance->View.ui.sync.cont, 300, 0);

            lv_anim_timeline_start(instance->View.ui.anim_timeline);
            lv_anim_start(&instance->View.ui.sync.bar.anim);
#endif
        }
    }
}

static void syncbar_timer_callback(lv_timer_t *timer) {
    const auto *instance = static_cast<Shutdown *>(timer->user_data);
    LV_ASSERT_NULL(instance);
    systemInfo.powerMonitor.panel_power_on = false;
    instance->pageManager->BackHome();
}

static void syncbar_anim_done_callback(lv_anim_t *a) {
    auto *instance = static_cast<Shutdown *>(lv_anim_get_user_data(a));
    LV_ASSERT_NULL(instance);
    lv_label_set_text(instance->View.ui.sync.label, "Sync Done");
    lv_timer_t *timer = lv_timer_create(syncbar_timer_callback, 1000, instance);
    lv_timer_set_repeat_count(timer, 1);
}

void Shutdown::onCustomAttrConfig() {
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(true);
}

void Shutdown::onViewLoad() {
    LV_LOG_USER("ViewLoad");
    Model.Init();
    View.Create(_root);
    lv_anim_set_exec_cb(&View.ui.shutdown.bar.anim, lv_anim_obj_set_width);
    lv_anim_set_user_data(&View.ui.shutdown.bar.anim, this);

    lv_anim_set_deleted_cb(&View.ui.sync.bar.anim, syncbar_anim_done_callback);
    lv_anim_set_user_data(&View.ui.sync.bar.anim, this);

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
    if (timer != nullptr) {
        lv_timer_del(timer);
        timer = nullptr;
    }
}

void Shutdown::onViewUnload() {
    LV_LOG_USER("onViewUnload");
    View.Delete();
    Model.Deinit();
}

void Shutdown::onViewDidUnload() {
    LV_LOG_USER("onViewDidUnload");
}

void Shutdown::AttachEvent(lv_obj_t *obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Shutdown::Update() {
}

void Shutdown::onTimerUpdate(lv_timer_t *timer) {
    auto *instance = static_cast<Shutdown *>(timer->user_data);

    instance->Update();
}

void Shutdown::onEvent(lv_event_t *event) {
    const auto *instance = static_cast<Shutdown *>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);
    if (const lv_obj_t *obj = lv_event_get_target(event); obj != instance->View.ui.shutdown.btnPress)return;

    if (const lv_event_code_t code = lv_event_get_code(event); code == LV_EVENT_SHORT_CLICKED) {
        instance->pageManager->Pop();
        PM_LOG_INFO("code:%d", code);
    } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESSED) {
        PM_LOG_INFO("code:%d", code);
        lv_anim_t anim = instance->View.ui.shutdown.bar.anim;
        const int16_t current_width = lv_obj_get_width(instance->View.ui.shutdown.bar.obj);
        constexpr uint32_t boot_time_ms = 2000; // 设定长按开机时间为2秒
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
