#ifndef __SCREEN1_TOP_H__
#define __SCREEN1_TOP_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *screen1_top;

    /**********************����״̬�����*****************************/
    extern lv_obj_t *screen1_battery, *screen1_wifi, *screen1_bt, *screen1_vol, *screen1_gps, *screen1_sd, *screen1_radio, *screen1_4g;

    void lv_screen1_top_init(void);

    void lv_singularxyz_4g(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_4g();

    void lv_singularxyz_radio(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_radio();

    void lv_singularxyz_sd(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_sd();

    void lv_singularxyz_gps(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_gps();

    void lv_singularxyz_wifi(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_wifi();

    void lv_singularxyz_battery(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_battery(unsigned int value);

    void lv_singularxyz_update_top_status(void);

#ifdef __cplusplus
}
#endif

#endif
