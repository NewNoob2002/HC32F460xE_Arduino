#ifndef __SCREEN2_H__
#define __SCREEN2_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *scr2;

    extern long int screnn2_time;

    extern int work_status_enable;

    void lv_screen2_init(void);
    void lv_screen2_appear(void);
    void lv_screen2_disappear();

    void lv_singularxyz_workstatus_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_workstatus_text();

    void lv_singularxyz_radio_mode_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_radio_mode_text(int mode_flag);
    void lv_singularxyz_radio_protocol_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_radio_protocol_text(int protocol_flag);
    void lv_singularxyz_radio_chanel_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_radio_chanel_text(int channel_flag);

    void update_screen2_radio(int show_flag);

    void update_screen2_ntrip(int show_flag);
    void lv_singularxyz_ntrip_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_ntrip_text(int show_flag);
    void lv_singularxyz_ntrip_ip_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_ntrip_ip_text(int show_flag);
    void lv_singularxyz_ntrip_point_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_ntrip_point_text(int show_flag);

    void query_workstatus();
    void change_workstatus();

#ifdef __cplusplus
}
#endif

#endif
