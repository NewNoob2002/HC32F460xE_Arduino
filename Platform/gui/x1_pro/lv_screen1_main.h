#ifndef __SCREEN1_H__
#define __SCREEN1_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *scr1;

    void lv_screen1_init(void);
    void lv_screen1_appear(void);
    void lv_screen1_disappear();

#ifdef __cplusplus
}
#endif

#endif
