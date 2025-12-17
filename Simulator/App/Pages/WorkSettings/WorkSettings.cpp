#include "WorkSettings.h"

using namespace Page;

const uint8_t RadioProtocol[PROTOCOL_MAX] = {1, 2, 4, 5, 9, 10, 13, 16};

WorkSettings::WorkSettings() = default;

WorkSettings::~WorkSettings() = default;

void WorkSettings::onCustomAttrConfig() {
    PageBase::onCustomAttrConfig();
}

void WorkSettings::onViewLoad() {
    PageBase::onViewLoad();
    View.Create(_root);

    AttachEvent(View.ui.roller.left_roller.btnUp);
    AttachEvent(View.ui.roller.left_roller.btnDown);
    AttachEvent(View.ui.roller.mid_roller.btnUp);
    AttachEvent(View.ui.roller.mid_roller.btnDown);

    AttachEvent(View.ui.btnCont.btnBase);
    AttachEvent(View.ui.btnCont.btnRover);
    AttachEvent(View.ui.btnCont.btnNtrip);
}

void WorkSettings::onViewDidLoad() {
    PageBase::onViewDidLoad();
}

void WorkSettings::onViewWillAppear() {
    PageBase::onViewWillAppear();
    lv_indev_wait_release(lv_indev_get_act());
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);

    lv_group_set_wrap(group, false);

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
}

void WorkSettings::onViewUnload() {
    PageBase::onViewUnload();
    Model.Deinit();
    View.Delete();
}

void WorkSettings::onViewDidUnload() {
    PageBase::onViewDidUnload();
}

void WorkSettings::onBtnClicked(lv_obj_t *btn) const {
    if (btn == View.ui.btnCont.btnBase) {
        uint8_t protocol_index = RadioProtocol[WorkSettingsView::Roller_GetIndex(View.ui.roller.left_roller.label)];
        uint8_t channel_index = WorkSettingsView::Roller_GetIndex(View.ui.roller.mid_roller.label) + 1;
        PM_LOG_INFO("btnBase, pro:%d, freq:%d", protocol_index, channel_index);
        systemInfo.work_mode = base_mode;
        systemInfo.radioInfo.radio_status = 1;
        systemInfo.radioInfo.radio_mode = 0;
        systemInfo.radioInfo.radio_protocol = protocol_index;
        systemInfo.radioInfo.radio_channel =channel_index;
        systemInfo.panel_operation_flag = 1;
        pageManager->Push("Pages/Dialplate");
    } else if (btn == View.ui.btnCont.btnRover) {
        PM_LOG_INFO("btnRover");
        pageManager->Push("Pages/Dialplate");
    }else if (btn == View.ui.btnCont.btnNtrip) {
        PM_LOG_INFO("btnNtrip");
        pageManager->Push("Pages/Dialplate");
    }else if (btn == View.ui.roller.left_roller.btnUp) {
        PM_LOG_INFO("left.btnUp");
        WorkSettingsView::Roller_UpDown(View.ui.roller.left_roller.label, false);
    }else if (btn == View.ui.roller.left_roller.btnDown) {
        PM_LOG_INFO("left.btnDown");
        WorkSettingsView::Roller_UpDown(View.ui.roller.left_roller.label, true);
    }else if (btn == View.ui.roller.mid_roller.btnUp) {
        PM_LOG_INFO("mid.btnUp");
        WorkSettingsView::Roller_UpDown(View.ui.roller.mid_roller.label, false);
    }else if (btn == View.ui.roller.mid_roller.btnDown) {
        PM_LOG_INFO("mid.btnDown");
        WorkSettingsView::Roller_UpDown(View.ui.roller.mid_roller.label, true);
    }
}

void WorkSettings::AttachEvent(lv_obj_t *obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void WorkSettings::onEvent(lv_event_t *event) {
    auto *instance = static_cast<WorkSettings *>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);

    if (const lv_event_code_t code = lv_event_get_code(event); code == LV_EVENT_SHORT_CLICKED) {
        instance->onBtnClicked(obj);
    }
}
