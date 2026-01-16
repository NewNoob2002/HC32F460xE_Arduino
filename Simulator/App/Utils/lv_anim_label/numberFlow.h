#ifndef LVGL_NUMBER_FLOW_H
#define LVGL_NUMBER_FLOW_H

#include <vector>
#include "lvgl.h"

class numberFlow {
public:
    explicit numberFlow(const lv_font_t *default_Font, const int NUMBER_SIZE, const bool hide = false) :
        cont(nullptr),
        font(default_Font),
        number_size(NUMBER_SIZE),
        last_value(-1),
        Hidden_high_position(hide)
    {
    };

    ~numberFlow() {
        // 删除容器会自动删除所有子对象，不需要单独删除 digit_labels 中的指针
        if (this->cont && lv_obj_is_valid(this->cont)) {
            lv_obj_del(this->cont);
        }
        digit_labels.clear();
    }

    void create(lv_obj_t *parent);

    void setPos(lv_align_t align, lv_coord_t x, lv_coord_t y) const;

    void setAlignTo(const lv_obj_t *base, lv_align_t align, lv_coord_t x, lv_coord_t y) const;

    void setValue(uint32_t target_value) const;

    // 获取容器指针，用于布局
    lv_obj_t *getCont() const { return cont; }

private:
    lv_obj_t *cont;
    const lv_font_t *font;
    uint16_t number_size;
    std::vector<lv_obj_t *> digit_labels;
    mutable uint32_t last_value; // 记录上一次的值，避免重复动画
    mutable std::vector<uint32_t> last_digits; // 记录每个数字位的上一次值
    bool Hidden_high_position;
    // 内部辅助函数：执行动画
    void animateDigit(uint32_t digit_index, uint32_t target_val) const;
    // 内部辅助函数：隐藏/显示数字位
    void setDigitVisibility(uint32_t digit_index, bool visible) const;
};

#endif //LVGL_NUMBER_FLOW_H
