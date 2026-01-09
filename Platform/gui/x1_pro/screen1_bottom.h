#ifndef __SCREEN1_BOTTOM_H__
#define __SCREEN1_BOTTOM_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *screen1_bottom;

    extern lv_obj_t *label_satellite_big, *label_sat_num, *label_sat_num_real, *label_wm_big, *label_wm_t, *label_position_big, *label_position_t;

    void lv_screen1_bottom_init(void);

    void lv_singularxyz_satellite(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_satellite(unsigned char type);

    void lv_singularxyz_satellite_num(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_satellite_num(unsigned char value);

    void lv_singularxyz_satellite_num_real(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_satellite_num_real(unsigned char value);

    void lv_singularxyz_workmode(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_workmode(unsigned char mode);

    void lv_singularxyz_workmode_text(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_workmode_text(unsigned char mode);

    void lv_singularxyz_position(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_position(unsigned char state);

    void lv_singularxyz_position_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_position_text(unsigned char state);

    // void lv_singularxyz_update_bottom_main(unsigned char value , unsigned char mode, unsigned char state );
    void lv_singularxyz_update_bottom_main();

#ifdef __cplusplus
}
#endif
#endif
