//
// Created by gtc on 2026/5/9.
//

#include "RecordConfig.h"

using namespace Page;

void
RecordConfig::onCustomAttrConfig() {
    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void
RecordConfig::onViewLoad() {
    PageBase::onViewLoad();
    View.Create(_root);
    //lv_obj_fade_in(_root, 300, 0);
    AttachEvent(View.ui.roller.left_roller.btnUp);
    AttachEvent(View.ui.roller.left_roller.btnDown);
    AttachEvent(View.ui.roller.right_roller.btnUp);
    AttachEvent(View.ui.roller.right_roller.btnDown);

    AttachEvent(View.ui.btnCont.btnRecord);
    AttachEvent(View.ui.btnCont.btnReturn);
}

void
RecordConfig::onViewDidLoad() {
    PageBase::onViewDidLoad();
}

void
RecordConfig::onViewWillAppear() {
    PageBase::onViewWillAppear();

    lv_indev_wait_release(lv_indev_get_act());
    lv_group_t* group = lv_group_get_default();
    LV_ASSERT_NULL(group);

    lv_group_set_wrap(group, true);

    lv_group_add_obj(group, View.ui.roller.left_roller.btnUp);
    lv_group_add_obj(group, View.ui.roller.left_roller.btnDown);
    lv_group_add_obj(group, View.ui.roller.right_roller.btnUp);
    lv_group_add_obj(group, View.ui.roller.right_roller.btnDown);

    lv_group_add_obj(group, View.ui.btnCont.btnRecord);
    lv_group_add_obj(group, View.ui.btnCont.btnReturn);

    if (lastFocus) {
        lv_group_focus_obj(lastFocus);
    } else {
        lv_group_focus_obj(View.ui.btnCont.btnRecord);
    }

    View.AppearAnimStart();
}

void
RecordConfig::onViewDidAppear() {
    PageBase::onViewDidAppear();
    if (timer == nullptr) {
        PM_LOG_INFO("RecordConfig::Timer Create");
        timer = lv_timer_create(onTimerUpdate, 20000, this);
        lv_timer_ready(timer);
    } else {
        PM_LOG_INFO("RecordConfig::Timer Resume");
        lv_timer_resume(timer);
        lv_timer_ready(timer);
    }
}

void
RecordConfig::onViewWillDisappear() {
    PageBase::onViewWillDisappear();
    lv_group_t* group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lastFocus = lv_group_get_focused(group);
    lv_group_remove_all_objs(group);
    if (timer) {
        PM_LOG_INFO("RecordConfig::Timer Pause");
        lv_timer_pause(timer);
    }
}

void
RecordConfig::onViewDidDisappear() {
    PageBase::onViewDidDisappear();
}

void
RecordConfig::onViewUnload() {
    PageBase::onViewUnload();
    View.Delete();
    if (lastFocus) {
        lastFocus = nullptr;
    }
}

void
RecordConfig::onViewDidUnload() {
    PageBase::onViewDidUnload();
}

void
RecordConfig::onBtnClicked(const lv_obj_t* btn, const lv_event_code_t& code) {
    if (code == LV_EVENT_SHORT_CLICKED) {
        if (btn == View.ui.btnCont.btnRecord) {}
        else if (btn == View.ui.btnCont.btnReturn) {
            pageManager->Pop();
        }
    } else if (code == LV_EVENT_LONG_PRESSED) {
        if (btn == View.ui.btnCont.btnRecord) {}
    }
}

void
RecordConfig::AttachEvent(lv_obj_t* obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void
RecordConfig::onTimerUpdate(lv_timer_t* timer) {
    // auto* instance = static_cast<RecordConfig*>(timer->user_data);
    // instance->View.Update();
    PM_LOG_INFO("RecordConfig::onTimerUpdate");
}

void
RecordConfig::onEvent(lv_event_t* event) {
    auto* instance = static_cast<RecordConfig*>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);

    const lv_obj_t* obj = lv_event_get_current_target(event);
    const lv_event_code_t code = lv_event_get_code(event);

    instance->onBtnClicked(obj, code);
}
