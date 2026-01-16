#ifndef SAVECONFIG_VIEW_H
#define SAVECONFIG_VIEW_H

#include "../Page.h"

namespace Page {
    class SaveConfigView {
    public:
        void Create(lv_obj_t *root);
        void Delete();

        struct {
            struct {
                lv_obj_t *cont;
                lv_obj_t *label;
                struct {
                    lv_obj_t *obj;
                    lv_anim_t anim;
                }bar;
            }sync;
            lv_anim_timeline_t* anim_timeline;
        } ui;
    };
}

#endif // !SHUTDOWN_VIEW_H
