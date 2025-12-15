//
// Created by guoti on 2025/12/14.
//

#ifndef LVGL_NUMBERFLOW_CLOCK_H
#define LVGL_NUMBERFLOW_CLOCK_H

#include "numberFlow.h"

class numberFlow_clock {
public:
    explicit numberFlow_clock(const lv_font_t *font);

    ~numberFlow_clock();

    void create(lv_obj_t *parent);

    void setPos(lv_align_t align, lv_coord_t x, lv_coord_t y) const;

    void setTime(uint32_t hour_val, uint32_t minute_val, uint32_t second_val) const;

    [[nodiscard]] lv_obj_t *getCont() const {
        return cont;
    };
private:
    const lv_font_t *font;
    lv_obj_t *cont; // 主容器
    numberFlow *hour; // 小时（2位）
    numberFlow *minute; // 分钟（2位）
    numberFlow *second; // 秒（2位）
    lv_obj_t *separator1; // 第一个冒号
    lv_obj_t *separator2; // 第二个冒号
};

#endif //LVGL_NUMBERFLOW_CLOCK_H
