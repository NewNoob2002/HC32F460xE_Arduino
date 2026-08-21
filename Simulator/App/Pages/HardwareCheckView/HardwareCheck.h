//
// Created by guoti on 2025/12/14.
//

#ifndef LVGL_HARDWARE_CHECK_H
#define LVGL_HARDWARE_CHECK_H
#include "HardwareCheckModel.h"
#include "HardwareCheckView.h"


namespace Page {
class HardwareCheck final : public PageBase {
  public:
    HardwareCheck() = default;

    ~HardwareCheck() override = default;

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

    HardwareCheckView View{};
    HardwareCheckModel Model;

  private:
    lv_timer_t* timer{};
    static uint32_t first_check_time;

    static void onTimer(lv_timer_t* timer);

    static void onEvent(lv_event_t* event);
};
} // namespace Page

#endif //LVGL_HARDWARE_CHECK_H
