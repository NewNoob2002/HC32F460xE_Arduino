#ifndef STARTUP_VIEW_H
#define STARTUP_VIEW_H

#include "../Page.h"

namespace Page
{

class StartupView
{
public:
    void Create(lv_obj_t* root);
    void Delete();
    void Update() const;
public:
    struct
    {
        struct
        {
            lv_obj_t* img;
            lv_obj_t* objUsage;
            lv_obj_t* label;
        } battery;
        lv_obj_t *arc;
        lv_obj_t *arc_percent;
        lv_anim_t arc_anim;
        lv_obj_t *time;
    } ui;

private:
};

}

#endif // STARTUP_VIEW_H
