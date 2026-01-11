#ifndef __SCREEN9_H__
#define __SCREEN9_H__

#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif
	
void lv_screen9_init(void);
void lv_screen9_appear();
void lv_screen9_disappear();
void lv_shutdown_set_src(const char*src);
#ifdef __cplusplus
}
#endif

#endif
