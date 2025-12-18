#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include "ShutdownView.h"
#include "ShutdownModel.h"

namespace Page {
    class Shutdown : public PageBase {
    public:
        typedef struct {
            uint16_t time;
            lv_color_t color;
        } Param_t;

        Shutdown() = default;

        ~Shutdown() override = default;

        void onCustomAttrConfig() override;

        void onViewLoad() override;

        void onViewDidLoad() override;

        void onViewWillAppear() override;

        void onViewDidAppear() override;

        void onViewWillDisappear() override;

        void onViewDidDisappear() override;

        void onViewUnload() override;

        void onViewDidUnload() override;

        ShutdownView View{};
        ShutdownModel Model{};
    private:
        void Update();

        void AttachEvent(lv_obj_t *obj);

        static void onTimerUpdate(lv_timer_t *timer);

        static void onEvent(lv_event_t *event);

        lv_timer_t *timer{};
    };
}

#endif
