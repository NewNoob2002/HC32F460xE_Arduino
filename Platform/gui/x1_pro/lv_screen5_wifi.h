#ifndef __screen5_H__
#define __screen5_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *scr5;

    void lv_screen5_init(void);

    void lv_screen5_appear(void);
    void lv_screen5_disappear();

    void lv_singularxyz_router(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_router(unsigned char state);

    void lv_singularxyz_router_mode_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_router_mode_text(unsigned char mode);

    void lv_singularxyz_router_ssid_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_router_ssid_text(const char *ssid);

    void lv_singularxyz_router_ip_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs);
    void lv_update_router_ip_text(const char *ip);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
