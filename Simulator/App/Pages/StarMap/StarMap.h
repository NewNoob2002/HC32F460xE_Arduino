#ifndef STAR_MAP_H
#define STAR_MAP_H

#include "StarMapView.h"

namespace Page {

class StarMap final : public PageBase {
  public:
    StarMap();
    ~StarMap() override;

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
    void Update();
#if defined(LVGL_SIMULATOR)
    static void GenerateTestData();
#endif
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);

    StarMapView View;
    lv_timer_t* timer{};
};

} // namespace Page

#endif // STAR_MAP_H
