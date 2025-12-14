//
// Created by guoti on 2025/12/14.
//

#include "numberFlow_clock.h"

numberFlow_clock::numberFlow_clock(const lv_font_t *font) : font(font),
    cont(nullptr),
    hour(nullptr),
    minute(nullptr),
    second(nullptr),
    separator1(nullptr),
    separator2(nullptr) {
    // 创建三个数字流组件
    hour = new numberFlow(font, 2);
    minute = new numberFlow(font, 2);
    second = new numberFlow(font, 2);
}

numberFlow_clock::~numberFlow_clock() {
    // 删除数字流组件
    if (hour) {
        delete hour;
        hour = nullptr;
    }
    if (minute) {
        delete minute;
        minute = nullptr;
    }
    if (second) {
        delete second;
        second = nullptr;
    }
    // 删除容器会自动删除所有子对象（包括分隔符）
    if (cont && lv_obj_is_valid(cont)) {
        lv_obj_del(cont);
        cont = nullptr;
    }
}

void numberFlow_clock::create(lv_obj_t *parent) {
    const uint16_t font_width = lv_font_get_glyph_width(font, '0', '\0');
    const lv_coord_t font_height = font->line_height;
    const uint16_t colon_width = lv_font_get_glyph_width(font, ':', '\0');

    // 创建主容器
    cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    // 计算总宽度：2位数字 + 冒号 + 2位数字 + 冒号 + 2位数字
    const lv_coord_t total_width = font_width * 2 + colon_width + font_width * 2 + colon_width + font_width * 2;
    lv_obj_set_size(cont, total_width, font_height);

    // 创建小时数字流
    hour->create(cont);
    lv_obj_t *hour_cont = hour->getCont();
    lv_obj_set_align(hour_cont, LV_ALIGN_TOP_LEFT); // 清除居中对齐
    lv_obj_set_pos(hour_cont, 0, 0);

    // 创建第一个冒号分隔符
    separator1 = lv_label_create(cont);
    lv_label_set_text_static(separator1, ":");
    lv_obj_set_style_text_font(separator1, font, 0);
    lv_obj_set_style_text_color(separator1, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_pad_all(separator1, 0, 0);
    lv_obj_set_align(separator1, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(separator1, font_width * 2, 0);

    // 创建分钟数字流
    minute->create(cont);
    lv_obj_t *minute_cont = minute->getCont();
    lv_obj_set_align(minute_cont, LV_ALIGN_TOP_LEFT); // 清除居中对齐
    lv_obj_set_pos(minute_cont, font_width * 2 + colon_width, 0);

    // 创建第二个冒号分隔符
    separator2 = lv_label_create(cont);
    lv_label_set_text_static(separator2, ":");
    lv_obj_set_style_text_font(separator2, font, 0);
    lv_obj_set_style_text_color(separator2, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_pad_all(separator2, 0, 0);
    lv_obj_set_align(separator2, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(separator2, font_width * 2 + colon_width + font_width * 2, 0);

    // 创建秒数字流
    second->create(cont);
    lv_obj_t *second_cont = second->getCont();
    lv_obj_set_align(second_cont, LV_ALIGN_TOP_LEFT); // 清除居中对齐
    lv_obj_set_pos(second_cont, font_width * 2 + colon_width + font_width * 2 + colon_width, 0);
}

void numberFlow_clock::setPos(const lv_align_t align, const lv_coord_t x, const lv_coord_t y) const {
    if (cont == nullptr) return;
    lv_obj_align(cont, align, x, y);
}

void numberFlow_clock::setTime(const int hour_val, const int minute_val, const int second_val) const {
    if (hour) {
        hour->setValue(hour_val);
    }
    if (minute) {
        minute->setValue(minute_val);
    }
    if (second) {
        second->setValue(second_val);
    }
}
