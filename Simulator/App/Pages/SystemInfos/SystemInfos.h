#ifndef __SYSTEM_INFOS_PRESENTER_H
#define __SYSTEM_INFOS_PRESENTER_H

#include "SystemInfosModel.h"
#include "SystemInfosView.h"


namespace Page {

class SystemInfos : public PageBase {
  public:
    SystemInfos();

    ~SystemInfos() override;

    void onCustomAttrConfig() override;

    void onViewLoad() override;

    void onViewDidLoad() override;

    void onViewWillAppear() override;

    void onViewDidAppear() override;

    void onViewWillDisappear() override;

    void onViewDidDisappear() override;

    void onViewUnload() override;

    void onViewDidUnload() override;

    void onLanguageChanged() override;

  private:
    void Update() const;

    void AttachEvent(lv_obj_t* obj);

    static void onTimerUpdate(lv_timer_t* timer);

    static void onEvent(lv_event_t* event);

    SystemInfosView View;
    SystemInfosModel Model;
    lv_timer_t* timer{};
};

} // namespace Page

#endif
