#ifndef SHUTDOWN_VIEW_H
#define SHUTDOWN_VIEW_H

#include "../Page.h"

namespace Page {
    class ShutdownView {
    public:
        void Create(lv_obj_t *root);
        void Delete();

        struct {
            struct {
                lv_obj_t *cont;
                struct{
                    lv_obj_t *cont;
                    lv_obj_t *label;
                    lv_obj_t *obj;
                    lv_anim_t anim;
                }bar;
                lv_obj_t *btnPress;
            }shutdown;
        } ui;
    };
}

#endif // !SHUTDOWN_VIEW_H
