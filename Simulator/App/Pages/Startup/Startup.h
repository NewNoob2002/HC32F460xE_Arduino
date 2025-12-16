#ifndef STARTUP_PRESENTER_H
#define STARTUP_PRESENTER_H

#include "StartupView.h"
#include "StartupModel.h"


namespace Page
{

class Startup final : public PageBase
{
public:
    Startup()
    = default;
    ~Startup() override = default;

    void onCustomAttrConfig() override;
    void onViewLoad() override;
    void onViewDidLoad() override;
    void onViewWillAppear() override;
    void onViewDidAppear() override;
    void onViewWillDisappear() override;
    void onViewDidDisappear() override;
    void onViewUnload() override;
    void onViewDidUnload() override;

    StartupView View{};
    StartupModel Model;

private:
    lv_timer_t* timer{};
    static void onTimer(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);
};

}

#endif
