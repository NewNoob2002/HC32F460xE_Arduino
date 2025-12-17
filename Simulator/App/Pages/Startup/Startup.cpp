#include "Startup.h"
#include <cstdio>
#include <functional>

using namespace Page;

static std::function<void(void *e)> completion_callback = nullptr;
static bool arc_complement_callback_do = false;

static void arc_complement_callback(void *e) {
    systemInfo.powerMonitor.panel_power_on = true;
		if(!arc_complement_callback_do)
    {
			arc_complement_callback_do = true;
			systemInfo.powerMonitor.panel_power_on = true;
			const auto *instance = static_cast<Startup *>(e);
			instance->pageManager->Push("Pages/HardwareCheck");
		}
}

static void arc_angle_anim(void *obj, const int32_t v) {
    lv_anim_t *anim = lv_anim_get(static_cast<lv_obj_t *>(obj), arc_angle_anim);
    LV_ASSERT_NULL(anim);

    auto *instance = static_cast<Startup *>(lv_anim_get_user_data(anim));
    LV_ASSERT_NULL(instance);

    lv_arc_set_value(static_cast<lv_obj_t *>(obj), v);
    lv_label_set_text_fmt(instance->View.ui.arc_percent, "%d%%", v);

    if (v >= 98) {
        lv_obj_set_style_arc_color(
            static_cast<lv_obj_t *>(obj), lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
        if (v >= 100) {
            if (completion_callback != nullptr) {
                completion_callback(instance);
            }
        }
    }
}


void Startup::onCustomAttrConfig() {
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void Startup::onViewLoad() {
    Model.Init();
    View.Create(_root);
    completion_callback = arc_complement_callback;
    lv_anim_set_exec_cb(&View.ui.arc_anim, arc_angle_anim);
    lv_anim_set_user_data(&View.ui.arc_anim, this);
}

void Startup::onViewDidLoad() {
    timer = lv_timer_create(onTimer, 10, this);
    lv_timer_resume(timer);
}

void Startup::onViewWillAppear() {
}

void Startup::onViewDidAppear() {
}

void Startup::onViewWillDisappear() {
    lv_timer_del(timer);
    Model.SetStatusBarAppear(false, false);
}

void Startup::onViewDidDisappear() {
}

void Startup::onViewUnload() {
    View.Delete();
    Model.Deinit();
}

void Startup::onViewDidUnload() {
}

void Startup::onTimer(lv_timer_t *timer) {
    auto *instance = static_cast<Startup *>(timer->user_data);
    instance->View.Update();
		static bool StatusBarAppear = false;
    if (lv_tick_get() >= 10000 && !StatusBarAppear) {
				StatusBarAppear = true;
        instance->Model.SetStatusBarAppear(true, true);
    }
}

void Startup::onEvent(lv_event_t *event) {
}
