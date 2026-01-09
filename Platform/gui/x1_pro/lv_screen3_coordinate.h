#ifndef __SCREEN3_H__
#define __SCREEN3_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *scr3;

    extern lv_obj_t *label_coordinate_big, *label_coordinate_lon_t, *label_coordinate_lat_t, *label_coordinate_alt_t;

    void lv_screen3_init(void);
    void lv_screen3_appear(void);
    void lv_screen3_disappear();

    void lv_singularxyz_coordinate(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_coordinate();

    void lv_singularxyz_coordinate_lon_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_coordinate_lon_text();

    void lv_singularxyz_coordinate_lat_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_coordinate_lat_text();

    void lv_singularxyz_coordinate_alt_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_coordinate_alt_text();

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __SCREEN3_H__
