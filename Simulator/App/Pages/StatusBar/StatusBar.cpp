//
// Created by gtc on 2025/12/15.
//

#include "StatusBar.h"
#include "../Page.h"

struct
{
    lv_obj_t* cont;

    struct
    {
        lv_obj_t* img;
        lv_obj_t* label;
    } satellite;

    lv_obj_t* imgSD;

    lv_obj_t* labelClock;

    lv_obj_t* labelRec;

    struct
    {
        lv_obj_t* img;
        lv_obj_t* objUsage;
        lv_obj_t* label;
    } battery;
} ui;

lv_obj_t* Page::StatusBar_Create(lv_obj_t* par) {
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
}