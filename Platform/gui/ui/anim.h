#ifndef EEZ_LVGL_UI_ANIM_H
#define EEZ_LVGL_UI_ANIM_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"
#include "vars.h"

void StatusBar_ConBattSetOpa(lv_obj_t *obj, int32_t opa);
void StatusBar_onAnimOpaFinish(lv_anim_t *a);
void StatusBar_onAnimWidthFinish(lv_anim_t *a);
void StatusBar_AnimSetWidth(void *var, int32_t v);
void StatusBar_AnimCreate(lv_obj_t *contBatt);
	
	
void poweroff_anim_create();
#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_FONTS_H*/