#ifndef STARTUP_PRESENTER_H
#define STARTUP_PRESENTER_H

#include "StartupView.h"
#include "StartupModel.h"

namespace Page
{

class Startup final : public PageBase
{

public:
    Startup();
    ~Startup() override;

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewUnload();
    virtual void onViewDidUnload();

    StartupView View{};
    StartupModel Model;
private:
    static void onTimer(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);
};

}

#endif
