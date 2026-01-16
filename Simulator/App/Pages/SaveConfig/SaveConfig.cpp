#include "SaveConfig.h"

#include "HAL.h"

using namespace Page;

static void syncbar_timer_callback(lv_timer_t *timer) {
    const auto *instance = static_cast<SaveConfig *>(timer->user_data);
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
    auto *instance = static_cast<SaveConfig *>(lv_anim_get_user_data(a));
    LV_ASSERT_NULL(instance);
    HAL::Power_Shutdown(true);
    lv_timer_t *timer = lv_timer_create(syncbar_timer_callback, 1000, instance);
    lv_timer_set_repeat_count(timer, 1);
}

void SaveConfig::onCustomAttrConfig() {
    LV_LOG_USER("onCustomAttrConfig");
    SetCustomCacheEnable(true);
}

void SaveConfig::onViewLoad() {
    LV_LOG_USER("ViewLoad");
    Model.Init();
    View.Create(_root);
    Model.SetStatusBarDisappear(false);
    lv_label_set_text_fmt(View.ui.sync.label, "Info: %s", HAL::Power_GetPowerOffCause());
    lv_anim_set_deleted_cb(&View.ui.sync.bar.anim, syncbar_anim_done_callback);
    lv_anim_set_user_data(&View.ui.sync.bar.anim, this);
    lv_anim_timeline_start(View.ui.anim_timeline);
    lv_anim_start(&View.ui.sync.bar.anim);
}

void SaveConfig::onViewDidLoad() {
    LV_LOG_USER("onViewDidLoad");
}

void SaveConfig::onViewWillAppear() {
    LV_LOG_USER("onViewWillAppear");
}

void SaveConfig::onViewDidAppear() {
    LV_LOG_USER("onViewDidAppear");
}

void SaveConfig::onViewWillDisappear() {
    LV_LOG_USER("onViewWillDisappear");
}

void SaveConfig::onViewDidDisappear() {
    LV_LOG_USER("onViewDidDisappear");
}

void SaveConfig::onViewUnload() {
    LV_LOG_USER("onViewUnload");
    View.Delete();
    Model.Deinit();
}

void SaveConfig::onViewDidUnload() {
    LV_LOG_USER("onViewDidUnload");
}
