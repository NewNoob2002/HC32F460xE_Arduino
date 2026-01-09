#ifndef __screen6_H__
#define __screen6_H__

#include "lvgl.h"
#include "mcu_define.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef void (*arc_completion_callback_t)(void);


extern lv_obj_t * scr6;


void lv_screen6_init(void);
void lv_screen6_appear(void);
void lv_screen6_disappear(void);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif
