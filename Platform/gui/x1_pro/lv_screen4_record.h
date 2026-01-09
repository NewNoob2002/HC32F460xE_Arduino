#ifndef __SCREEN4_H__
#define __SCREEN4_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *scr4;

    extern lv_obj_t *label_record_big, *label_record_rsc_t, *label_record_name_t, *label_record_type_t, *label_record_time_t;

    extern unsigned char record_status;
    extern float record_RSC;
    extern char record_name[16];
    extern char record_type;
    extern char record_spacetime;

    void lv_screen4_init(void);

    void lv_screen4_appear(void);
		void lv_screen4_disappear(void);
    void lv_screen4_update();

    void lv_singularxyz_record(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_record();

    void lv_singularxyz_record_rsc_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_record_rsc_text();

    void lv_singularxyz_record_name_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_record_name_text();

    void lv_singularxyz_record_type_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_record_type_text();

    void lv_singularxyz_record_time_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_record_time_text();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SCREEN6_H__
