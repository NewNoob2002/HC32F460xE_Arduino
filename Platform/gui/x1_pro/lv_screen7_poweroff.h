#ifndef __SCREEN7_H__
#define __SCREEN7_H__

#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif
	
void lv_screen7_init(void);
void lv_screen7_appear();
void lv_screen7_disappear();
void lv_screen7_appear_flag();
	
void lv_anim_obj_set_width(void *obj, int32_t width);
#ifdef __cplusplus
}
#endif

#endif
