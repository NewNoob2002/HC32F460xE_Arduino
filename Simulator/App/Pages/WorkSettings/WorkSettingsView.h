#ifndef WorkSettings_VIEW_H
#define WorkSettings_VIEW_H

#include "../Page.h"

namespace Page
{

class WorkSettingsView
{
public:
    struct
    {
        lv_obj_t* labelInfo;

        struct {
            lv_obj_t* cont;
            struct {
                lv_obj_t* cont;
                lv_obj_t* label;

                lv_obj_t* btnCont;
                lv_obj_t* btnUp;
                lv_obj_t* btnDown;
            }left_roller;

            struct {
                lv_obj_t* cont;
                lv_obj_t* label;

                lv_obj_t* btnCont;
                lv_obj_t* btnUp;
                lv_obj_t* btnDown;
            }mid_roller;

            lv_obj_t* btnReset;
        }roller;

        struct
        {
            lv_obj_t* cont;
            lv_obj_t* btnBase;
            lv_obj_t* btnRover;
            lv_obj_t* btnNtrip;
        } btnCont;

        lv_anim_timeline_t* anim_timeline;
    } ui;

    void Create(lv_obj_t* root);
    void Delete();

    void Roller_Create(lv_obj_t* par);
    static void Roller_Style_Init(lv_obj_t* obj);
    void BtnCont_Create(lv_obj_t* par);
    static lv_obj_t* Btn_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs, lv_coord_t y_ofs);

    static void Roller_UpDown(lv_obj_t* obj, bool down);
    static uint8_t Roller_GetIndex(const lv_obj_t* obj);

    void AppearAnimStart(bool reverse = false) const;
};

}

#endif // WorkSettings_VIEW_H
