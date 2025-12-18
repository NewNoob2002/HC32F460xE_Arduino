#include "Dialplate.h"

#include <cstdio>
#include <cstdlib>

using namespace Page;

Dialplate::Dialplate()
    : recState(RECORD_STATE_STOP)
      , lastFocus(nullptr) {
}

Dialplate::~Dialplate()
= default;

void Dialplate::onCustomAttrConfig() {
    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void Dialplate::onViewLoad() {
    Model.Init();
    View.Create(_root);

    AttachEvent(View.ui.btnCont.btnMap);
    AttachEvent(View.ui.btnCont.btnRec);
    AttachEvent(View.ui.btnCont.btnMenu);
    AttachEvent(View.ui.btnCont.btnShutdown);
}

void Dialplate::onViewDidLoad() {
}

void Dialplate::onViewWillAppear() {
    lv_indev_wait_release(lv_indev_get_act());
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);

    lv_group_set_wrap(group, false);

    lv_group_add_obj(group, View.ui.btnCont.btnMap);
    lv_group_add_obj(group, View.ui.btnCont.btnRec);
    lv_group_add_obj(group, View.ui.btnCont.btnMenu);
    lv_group_add_obj(group, View.ui.btnCont.btnShutdown);

    if (lastFocus) {
        lv_group_focus_obj(lastFocus);
    } else {
        lv_group_focus_obj(View.ui.btnCont.btnRec);
    }

    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_TRANSP);

    Update();

    View.AppearAnimStart();
}

void Dialplate::onViewDidAppear() {
    timer = lv_timer_create(onTimerUpdate, 1000, this);
}

void Dialplate::onViewWillDisappear() {
    lv_group_t *group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lastFocus = lv_group_get_focused(group);
    lv_group_remove_all_objs(group);
    lv_timer_del(timer);
    // View.AppearAnimStart(true);
}

void Dialplate::onViewDidDisappear() {
}

void Dialplate::onViewUnload() {
    Model.Deinit();
    View.Delete();
    if (lastFocus) {
        lastFocus = nullptr;
    }
}

void Dialplate::onViewDidUnload() {
}

void Dialplate::AttachEvent(lv_obj_t *obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Dialplate::Update() {
    const PositionInfo_t positionInfo = systemInfo.positionInfo;
    View.ui.topInfo.satellite_used->setValue(positionInfo.satellite_number_used);
    View.ui.topInfo.satellite_tacked->setValue(positionInfo.satellite_number_track);
    if (systemInfo.recordInfo.record_status == 0 && recState == RECORD_STATE_START) {
        onRecord(false);
    } else if (systemInfo.recordInfo.record_status == 1 && recState == RECORD_STATE_STOP) {
        onRecord(true);
    }
    switch (systemInfo.work_mode) {
        case rover_mode:
        case single_mode:
            lv_label_set_text(View.ui.topInfo.icon_mode, CUSTOM_SYMBOL_ROVER);
            if (systemInfo.radioInfo.radio_status)
                lv_obj_set_style_text_color(View.ui.topInfo.icon_radio, lv_palette_main(LV_PALETTE_GREEN), 0);
            else {
                lv_obj_set_style_text_color(View.ui.topInfo.icon_radio, lv_color_white(), 0);
            }
            break;
        case base_mode:
        case autobase_mode:
            lv_label_set_text(View.ui.topInfo.icon_mode, CUSTOM_SYMBOL_BASE);
            if (systemInfo.radioInfo.radio_status)
                lv_obj_set_style_text_color(View.ui.topInfo.icon_radio, lv_palette_main(LV_PALETTE_BLUE), 0);
            else {
                lv_obj_set_style_text_color(View.ui.topInfo.icon_radio, lv_color_white(), 0);
            }
            break;
    }
}

void Dialplate::onTimerUpdate(lv_timer_t *timer) {
    auto *instance = static_cast<Dialplate *>(timer->user_data);
    instance->Update();
}

void Dialplate::onBtnClicked(lv_obj_t *btn) const {
    if (btn == View.ui.btnCont.btnMap) {
        pageManager->Push("Pages/WorkSettings");
    } else if (btn == View.ui.btnCont.btnMenu) {
        pageManager->Push("Pages/SystemInfos");
    } else if (btn == View.ui.btnCont.btnShutdown) {
#if defined(HC32F460)
        HAL::Power_Shutdown();
#endif
        pageManager->Push("Pages/Shutdown");
    }
}

void Dialplate::onRecord(const bool longPress) {
    switch (recState) {
        case RECORD_STATE_START:
            if (!longPress) {
                Model.RecorderCommand(DialplateModel::REC_STOP);
                SetBtnRecImgSrc("start");
                systemInfo.recordInfo.record_status = 0;
                systemInfo.panel_operation_flag = 1;
                recState = RECORD_STATE_STOP;
            }
            break;
        case RECORD_STATE_STOP:
            if (longPress) {
                Model.RecorderCommand(DialplateModel::REC_START);
                SetBtnRecImgSrc("stop");
                systemInfo.recordInfo.record_status = 1;
                systemInfo.panel_operation_flag = 1;
                recState = RECORD_STATE_START;
            }
            break;
        default:
            break;
    }
}

void Dialplate::SetBtnRecImgSrc(const char *srcName) const {
    lv_obj_set_style_bg_img_src(View.ui.btnCont.btnRec, ResourcePool::GetImage(srcName), 0);
}

void Dialplate::onEvent(lv_event_t *event) {
    auto *instance = static_cast<Dialplate *>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    const lv_event_code_t code = lv_event_get_code(event);


    if (code == LV_EVENT_SHORT_CLICKED) {
        instance->onBtnClicked(obj);
    }

    if (obj == instance->View.ui.btnCont.btnRec) {
        if (code == LV_EVENT_SHORT_CLICKED) {
            instance->onRecord(false);
        } else if (code == LV_EVENT_LONG_PRESSED) {
            instance->onRecord(true);
        }
    }
}
