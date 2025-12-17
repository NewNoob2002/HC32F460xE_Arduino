#ifndef __LIVEMAP_PRESENTER_H
#define __LIVEMAP_PRESENTER_H

#include "WorkSettingsView.h"
#include "WorkSettingsModel.h"

enum Protocol_index {
    TRIMTALK,
    TRIMMK3,
    TT450S,
    TRANSEOT,
    SOUTH,
    HUACE,
    SATEL,
    CCS,
    PROTOCOL_MAX
};

namespace Page
{

class WorkSettings : public PageBase
{
public:
    WorkSettings();
    ~WorkSettings() override;

    void onCustomAttrConfig() override;
    void onViewLoad() override;
    void onViewDidLoad() override;
    void onViewWillAppear() override;
    void onViewDidAppear() override;
    void onViewWillDisappear() override;
    void onViewDidDisappear() override;
    void onViewUnload() override;
    void onViewDidUnload() override;

    void onBtnClicked(lv_obj_t* btn) const;

private:
    WorkSettingsView View{};
    WorkSettingsModel Model;
    lv_obj_t* lastFocus{};
    void AttachEvent(lv_obj_t* obj);
    static void onEvent(lv_event_t* event);
};

}

#endif
