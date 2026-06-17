/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __LV_OBJ_EXT_FUNC_H
#define __LV_OBJ_EXT_FUNC_H

#include "lvgl/lvgl.h"

void lv_obj_set_opa_scale(lv_obj_t* obj, int16_t opa);
int16_t lv_obj_get_opa_scale(lv_obj_t* obj);
void lv_label_set_text_add(lv_obj_t * label, const char * text);

#define LV_ANIM_TIME_DEFAULT    400

static inline void lv_anim_exec_obj_set_x(void* obj, int32_t value)
{
    lv_obj_set_x((lv_obj_t*)obj, (lv_coord_t)value);
}

static inline void lv_anim_exec_obj_set_y(void* obj, int32_t value)
{
    lv_obj_set_y((lv_obj_t*)obj, (lv_coord_t)value);
}

static inline void lv_anim_exec_obj_set_width(void* obj, int32_t value)
{
    lv_obj_set_width((lv_obj_t*)obj, (lv_coord_t)value);
}

static inline void lv_anim_exec_obj_set_height(void* obj, int32_t value)
{
    lv_obj_set_height((lv_obj_t*)obj, (lv_coord_t)value);
}

static inline void lv_anim_exec_obj_set_opa_scale(void* obj, int32_t value)
{
    lv_obj_set_opa_scale((lv_obj_t*)obj, (int16_t)value);
}

#define LV_ANIM_EXEC(attr)      lv_anim_exec_obj_set_##attr
void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,
    uint16_t time = LV_ANIM_TIME_DEFAULT,
    uint32_t delay = 0,
    lv_anim_ready_cb_t ready_cb = NULL,
    lv_anim_path_cb_t path_cb = lv_anim_path_ease_out
);
#define LV_OBJ_ADD_ANIM(obj,attr,target,time)\
do{\
    lv_obj_add_anim(\
        (obj), NULL,\
        LV_ANIM_EXEC(attr),\
        lv_obj_get_##attr(obj),\
        (target),\
        (time)\
    );\
}while(0)
#define LV_OBJ_ADD_DELAY_ANIM(obj,attr,target,delay,time)\
do{\
    lv_obj_add_anim(\
        (obj), NULL,\
        LV_ANIM_EXEC(attr),\
        lv_obj_get_##attr(obj),\
        (target),\
        (time),\
        (delay)\
    );\
}while(0)
lv_indev_t* lv_get_indev(lv_indev_type_t type);

#endif
