#include "WorkSettings.h"

using namespace Page;

WorkSettings::WorkSettings() = default;

WorkSettings::~WorkSettings() = default;

void WorkSettings::onCustomAttrConfig() {
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
    SetCustomCacheEnable(true);
    PageBase::onCustomAttrConfig();
}

void WorkSettings::onViewLoad() {
    PageBase::onViewLoad();
    View.Create(_root);
    lv_obj_fade_in(_root, 300, 0);
}

void WorkSettings::onViewDidLoad() {
    PageBase::onViewDidLoad();
    AttachEvent(View.ui.roller.left_roller.btnUp);
    AttachEvent(View.ui.roller.left_roller.btnDown);
    AttachEvent(View.ui.roller.mid_roller.btnUp);
    AttachEvent(View.ui.roller.mid_roller.btnDown);
    AttachEvent(View.ui.roller.btnReset);

    AttachEvent(View.ui.btnCont.btnBase);
    AttachEvent(View.ui.btnCont.btnRover);
    AttachEvent(View.ui.btnCont.btnNtrip);
}

void WorkSettings::onViewWillAppear() {
    PageBase::onViewWillAppear();
    if (timer == nullptr) {
        PM_LOG_INFO("WorkSettings::Create");
        timer = lv_timer_create(onTimerUpdate, 20000, this);
        lv_timer_ready(timer);
    }
    else {
        PM_LOG_INFO("WorkSettings::Resume");
        lv_timer_resume(timer);
        lv_timer_ready(timer);
    }

    lv_indev_wait_release(lv_indev_get_act());
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);

    lv_group_set_wrap(group, true);

    lv_group_add_obj(group, View.ui.roller.left_roller.btnUp);
    lv_group_add_obj(group, View.ui.roller.left_roller.btnDown);
    lv_group_add_obj(group, View.ui.roller.mid_roller.btnUp);
    lv_group_add_obj(group, View.ui.roller.mid_roller.btnDown);
    lv_group_add_obj(group, View.ui.roller.btnReset);
    lv_group_add_obj(group, View.ui.btnCont.btnBase);
    lv_group_add_obj(group, View.ui.btnCont.btnRover);
    lv_group_add_obj(group, View.ui.btnCont.btnNtrip);

    if (lastFocus) {
        lv_group_focus_obj(lastFocus);
    } else {
        lv_group_focus_obj(View.ui.btnCont.btnBase);
    }

    View.AppearAnimStart();
}

void WorkSettings::onViewDidAppear() {
    PageBase::onViewDidAppear();
}

void WorkSettings::onViewWillDisappear() {
    PageBase::onViewWillDisappear();
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lastFocus = lv_group_get_focused(group);
    lv_group_remove_all_objs(group);
}

void WorkSettings::onViewDidDisappear() {
    PageBase::onViewDidDisappear();
    if (timer) {
        PM_LOG_INFO("WorkSettings::Pause");
        lv_timer_pause(timer);
    }
}

void WorkSettings::onViewUnload() {
    PageBase::onViewUnload();
    View.Delete();
    if (lastFocus) {
        lastFocus = nullptr;
    }
}

void WorkSettings::onViewDidUnload() {
    PageBase::onViewDidUnload();
}

void WorkSettings::onBtnClicked(const lv_obj_t *btn) const {
    if (btn == View.ui.btnCont.btnBase) {
        const uint8_t protocol_index = RadioProtocol[WorkSettingsView::Roller_GetIndex(View.ui.roller.left_roller.label)];
        const uint8_t channel_index = WorkSettingsView::Roller_GetIndex(View.ui.roller.mid_roller.label);
        if (channel_index != 0) {
            PM_LOG_INFO("btnBase, pro:%d, freq:%d", protocol_index, channel_index);
            systemInfo.work_mode = autobase_mode;
            systemInfo.radioInfo.radio_status = On_Off_Status_ON;
            systemInfo.ntripInfo.gprs_status = On_Off_Status_OFF;
            systemInfo.ntripInfo.NtripServer_status = On_Off_Status_OFF;
            systemInfo.radioInfo.radio_mode = radio_mode_tx;
            systemInfo.radioInfo.radio_protocol = protocol_index;
            systemInfo.radioInfo.radio_channel = channel_index;
            systemInfo.radioInfo.radio_change_flag = 1;
        }
        pageManager->Pop();
    } else if (btn == View.ui.btnCont.btnRover) {
        const uint8_t protocol_index = RadioProtocol[WorkSettingsView::Roller_GetIndex(View.ui.roller.left_roller.label)];
        const uint8_t channel_index = WorkSettingsView::Roller_GetIndex(View.ui.roller.mid_roller.label);
        if (channel_index != 0) {
            PM_LOG_INFO("btnRover, pro:%d, freq:%d", protocol_index, channel_index);
            systemInfo.work_mode = rover_mode;
            systemInfo.radioInfo.radio_status = On_Off_Status_ON;
            systemInfo.ntripInfo.gprs_status = On_Off_Status_OFF;
            systemInfo.ntripInfo.NtripServer_status = On_Off_Status_OFF;
            systemInfo.radioInfo.radio_mode = radio_mode_rx;
            systemInfo.radioInfo.radio_protocol = protocol_index;
            systemInfo.radioInfo.radio_channel = channel_index;
            systemInfo.radioInfo.radio_change_flag = 1;
        }
        pageManager->Pop();
    } else if (btn == View.ui.btnCont.btnNtrip) {
        systemInfo.work_mode = autobase_mode;
        systemInfo.radioInfo.radio_status = On_Off_Status_OFF;
        systemInfo.ntripInfo.gprs_status = On_Off_Status_ON;
        systemInfo.ntripInfo.NtripServer_status = On_Off_Status_ON;
        systemInfo.radioInfo.radio_change_flag = 1;
        pageManager->Pop();
    } else if (btn == View.ui.roller.left_roller.btnUp) {
        this->View.Roller_up(View.ui.roller.left_roller.label);
    } else if (btn == View.ui.roller.left_roller.btnDown) {
        this->View.Roller_down(View.ui.roller.left_roller.label);
    } else if (btn == View.ui.roller.mid_roller.btnUp) {
        this->View.Roller_up(View.ui.roller.mid_roller.label);
    } else if (btn == View.ui.roller.mid_roller.btnDown) {
        this->View.Roller_down(View.ui.roller.mid_roller.label);
    } else if (btn == View.ui.roller.btnReset) {
        pageManager->Pop();
    }
}

void WorkSettings::AttachEvent(lv_obj_t *obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void WorkSettings::onTimerUpdate(lv_timer_t *timer) {
    const auto *instance = static_cast<WorkSettings *>(timer->user_data);
    instance->View.Update();
    PM_LOG_INFO("WorkSettings::onTimerUpdate");
}

void WorkSettings::onEvent(lv_event_t *event) {
    const auto *instance = static_cast<WorkSettings *>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);

    const lv_obj_t *obj = event->current_target;

    if (const lv_event_code_t code = lv_event_get_code(event); code == LV_EVENT_SHORT_CLICKED) {
        instance->onBtnClicked(obj);
    }
}
