//
// Created by gtc on 2025/12/15.
//

#ifndef LVGL_STATUSBAR_H
#define LVGL_STATUSBAR_H

#include "lvgl.h"

namespace Page {
    lv_obj_t *StatusBar_Create(lv_obj_t *par);
}

void StatusBar_Appear(bool en, bool delay);
void StatusBar_SetRecord(bool active);
#endif //LVGL_STATUSBAR_H
