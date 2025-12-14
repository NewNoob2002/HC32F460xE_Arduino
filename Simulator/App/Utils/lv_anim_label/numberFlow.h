#ifndef LVGL_NUMBER_FLOW_H
#define LVGL_NUMBER_FLOW_H

#include <vector>
#include "lvgl.h"

class numberFlow {
public:
    explicit numberFlow(const lv_font_t *default_Font, const int NUMBER_SIZE) : cont(nullptr),
        font(default_Font),
        number_size(NUMBER_SIZE),
        last_value(-1) // 初始化为-1，确保第一次会更新
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

    void setValue(int target_value) const;

private:
    lv_obj_t *cont;
    const lv_font_t *font;
    uint16_t number_size;
    std::vector<lv_obj_t *> digit_labels;
    mutable int last_value; // 记录上一次的值，避免重复动画
    mutable std::vector<int> last_digits; // 记录每个数字位的上一次值

    // 内部辅助函数：执行动画
    void animateDigit(int digit_index, int target_val) const;
};

#endif //LVGL_NUMBER_FLOW_H
