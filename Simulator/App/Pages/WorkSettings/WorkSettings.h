#ifndef LIVEMAP_PRESENTER_H
#define LIVEMAP_PRESENTER_H

#include "WorkSettingsView.h"

namespace Page {
    class WorkSettings : public PageBase {
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

        void onBtnClicked(const lv_obj_t *btn) const;

    private:
        WorkSettingsView View{};
        lv_obj_t *lastFocus{};
        lv_timer_t *timer{};

        void AttachEvent(lv_obj_t *obj);

        static void onTimerUpdate(lv_timer_t *timer);

        static void onEvent(lv_event_t *event);
    };
}

#endif
