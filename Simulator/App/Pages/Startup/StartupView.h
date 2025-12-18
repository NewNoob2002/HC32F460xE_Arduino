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
            lv_obj_t *arc;
            lv_obj_t *arc_percent;
            lv_anim_t arc_anim;

            lv_obj_t *btnPress;
        } ui;
    private:
        mutable makeTime_t time = {};
    };
}

#endif // STARTUP_VIEW_H
