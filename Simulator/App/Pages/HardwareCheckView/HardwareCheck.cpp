//
// Created by guoti on 2025/12/14.
//

#include "HardwareCheck.h"

using namespace Page;

uint32_t HardwareCheck::last_check_time = 0;

void HardwareCheck::onCustomAttrConfig() {
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_FADE_ON);
}

void HardwareCheck::onViewLoad() {
    Model.Init();
    View.Create(_root);
    timer = lv_timer_create(onTimer, 2000, this);
    lv_timer_resume(timer);
    last_check_time = lv_tick_get();

}

void HardwareCheck::onViewDidLoad() {
    PageBase::onViewDidLoad();
}

void HardwareCheck::onViewWillAppear() {
    PageBase::onViewWillAppear();
}

void HardwareCheck::onViewDidAppear() {
    PageBase::onViewDidAppear();
}

void HardwareCheck::onViewWillDisappear() {
    if (timer) {
        lv_timer_del(timer);
    }
}

void HardwareCheck::onViewDidDisappear() {
    // Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP);
    Model.SetStatusBarAppear(true);
}

void HardwareCheck::onViewUnload() {
    View.Delete();
    // Model.SetEncoderEnable(true);
    Model.Deinit();
}

void HardwareCheck::onViewDidUnload() {
    PageBase::onViewDidUnload();
}

void HardwareCheck::onTimer(lv_timer_t *timer) {
    const auto* instance = static_cast<HardwareCheck *>(timer->user_data);
    instance->View.Update();
    if (lv_tick_get() - last_check_time >= 25000) {
        instance->pageManager->Push("Pages/Dialplate");
    }
}

void HardwareCheck::onEvent(lv_event_t *event) {
}
