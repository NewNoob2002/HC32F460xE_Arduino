#ifndef STARTUP_VIEW_H
#define STARTUP_VIEW_H

#include "../Page.h"

namespace Page {
    class StartupView {
    public:

        void Create(lv_obj_t *root);

        void Delete() const;

        void Update();

        struct {
            numberFlow *satellite;
            numberFlow_clock *clock;
            struct {
                lv_obj_t *img;
                lv_obj_t *objUsage;
                numberFlow *percent;
            } battery;
            lv_obj_t *arc;
            lv_obj_t *arc_percent;
            lv_anim_t arc_anim;
        } ui;
    private:
        mutable makeTime_t time = {};

    private:
    };
}

#endif // STARTUP_VIEW_H
