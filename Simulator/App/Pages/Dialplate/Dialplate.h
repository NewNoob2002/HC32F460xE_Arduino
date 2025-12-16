#ifndef DIALPLATE_PRESENTER_H
#define DIALPLATE_PRESENTER_H

#include "DialplateView.h"
#include "DialplateModel.h"

namespace Page
{

class Dialplate final : public PageBase
{
public:
    Dialplate();
    ~Dialplate() override;

    void onCustomAttrConfig() override;
    void onViewLoad() override;
    void onViewDidLoad() override;
    void onViewWillAppear() override;
    void onViewDidAppear() override;
    void onViewWillDisappear() override;
    void onViewDidDisappear() override;
    void onViewUnload() override;
    void onViewDidUnload() override;

private:
    typedef enum
    {
        RECORD_STATE_START = 0,
        RECORD_STATE_STOP
    } RecordState_t;

private:
    void Update();
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);
    void onBtnClicked(lv_obj_t* btn) const;
    void onRecord(bool longPress);
    void SetBtnRecImgSrc(const char* srcName);

private:
    DialplateView View{};
    DialplateModel Model;
    lv_timer_t* timer{};
    RecordState_t recState;
    PositionInfo_t positionInfo;
    lv_obj_t* lastFocus;
};

}

#endif
