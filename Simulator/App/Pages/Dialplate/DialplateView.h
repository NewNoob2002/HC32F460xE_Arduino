#ifndef DIALPLATE_VIEW_H
#define DIALPLATE_VIEW_H

#include "../Page.h"

namespace Page
{

class DialplateView
{

public:
    struct
    {
        struct
        {
            lv_obj_t* cont;
            lv_obj_t* icon_satellite;
            numberFlow* satellite_used;
            numberFlow* satellite_tacked;
        } topInfo;

        struct
        {
            lv_obj_t* cont;
            lv_obj_t* btnMap;
            lv_obj_t* btnRec;
            lv_obj_t* btnMenu;
        } btnCont;

        lv_anim_timeline_t* anim_timeline;
    } ui;

    void Create(lv_obj_t* root);
    void Delete();
    void AppearAnimStart(bool reverse = false) const;

private:
    void TopInfo_Create(lv_obj_t* par);
    void BtnCont_Create(lv_obj_t* par);
    static lv_obj_t* Btn_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs);
};

}

#endif // DIALPLATE_VIEW_H
