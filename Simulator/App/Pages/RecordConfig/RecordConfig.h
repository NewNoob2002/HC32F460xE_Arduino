//
// Created by gtc on 2026/5/9.
//

#ifndef LVGL_RECORDCONFIG_H
#define LVGL_RECORDCONFIG_H

#include "RecordConfigView.h"

namespace Page {
class RecordConfig : public PageBase {
public:
    RecordConfig()
        : recState(RECORD_STATE_STOP), lastFocus(nullptr), timer(nullptr) {}

    ~RecordConfig() override = default;

    void onCustomAttrConfig() override;

    void onViewLoad() override;

    void onViewDidLoad() override;

    void onViewWillAppear() override;

    void onViewDidAppear() override;

    void onViewWillDisappear() override;

    void onViewDidDisappear() override;

    void onViewUnload() override;

    void onViewDidUnload() override;

    void onBtnClicked(const lv_obj_t* btn, const lv_event_code_t& code);

private:
    typedef enum {
        RECORD_STATE_START = 0,
        RECORD_STATE_STOP
    } RecordState_t;

    RecordConfigView View{};
    RecordState_t recState;
    lv_obj_t* lastFocus;
    lv_timer_t* timer;

    void AttachEvent(lv_obj_t* obj);

    static void onTimerUpdate(lv_timer_t* timer);

    static void onEvent(lv_event_t* event);
};
}

#endif //LVGL_RECORDCONFIG_H
