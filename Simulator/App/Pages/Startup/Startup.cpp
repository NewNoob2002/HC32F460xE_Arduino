#include "Startup.h"

#include <cstdio>
#include <functional>

using namespace Page;

static std::function<void()> completion_callback = nullptr;


static void arc_complement_callback() {
    printf("arc_complement_callback\n");
}

static void arc_angle_anim(void *obj, const int32_t v) {
    // 获取当前正在执行的动画对象
    lv_anim_t *anim = lv_anim_get(static_cast<lv_obj_t *>(obj), arc_angle_anim);
    if (anim == nullptr) return;

    const auto *instance = static_cast<Startup *>(lv_anim_get_user_data(anim));
    if (instance == nullptr) return;

    // 更新 arc 的值
    lv_arc_set_value(static_cast<lv_obj_t *>(obj), v);

    // 通过实例访问 ui 结构体
    lv_label_set_text_fmt(instance->View.ui.arc_percent, "%d%%", v);

    if (v >= 98) {
        lv_obj_set_style_arc_color(
            static_cast<lv_obj_t *>(obj), lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
        if (v >= 100) {
            if (completion_callback != nullptr) {
                completion_callback();
            }
        }
    }
}

Startup::Startup()
= default;

Startup::~Startup()
= default;

void Startup::onCustomAttrConfig() {
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void Startup::onViewLoad() {
    View.Create(_root);
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_HIDDEN);
    completion_callback = arc_complement_callback;
    lv_anim_set_exec_cb(&View.ui.arc_anim, arc_angle_anim);
    lv_anim_set_user_data(&View.ui.arc_anim, this);
    // lv_anim_set_values(&ui.arc_anim, 0, 100);
    // lv_anim_start(&ui.arc_anim);
}

void Startup::onViewDidLoad() {
    lv_timer_t *timer = lv_timer_create(onTimer, 10, this);
    lv_timer_resume(timer);
}

void Startup::onViewWillAppear() {
}

void Startup::onViewDidAppear() {
}

void Startup::onViewWillDisappear() {
}

void Startup::onViewDidDisappear() {
}

void Startup::onViewUnload() {
}

void Startup::onViewDidUnload() {
}

void Startup::onTimer(lv_timer_t *timer) {
    auto *instance = static_cast<Startup *>(timer->user_data);
    instance->View.Update();
}

void Startup::onEvent(lv_event_t *event) {
}
