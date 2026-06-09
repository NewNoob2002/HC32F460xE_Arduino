#include "StarMap.h"

using namespace Page;

StarMap::StarMap() = default;

StarMap::~StarMap() = default;

void
StarMap::onCustomAttrConfig() {
    SetCustomCacheEnable(false);
}

void
StarMap::onViewLoad() {
    View.Create(_root);
    AttachEvent(_root);
}

void
StarMap::onViewDidLoad() {}

void
StarMap::onViewWillAppear() {
    timer = lv_timer_create(onTimerUpdate, 1000, this);
    lv_timer_ready(timer);
    lv_group_t* group = lv_group_get_default();
    LV_ASSERT_NULL(group);
    lv_group_add_obj(group, _root);
    lv_group_focus_obj(_root);
}

void
StarMap::onViewDidAppear() {}

void
StarMap::onViewWillDisappear() {
    lv_group_remove_obj(_root);
}

void
StarMap::onViewDidDisappear() {
    if (timer) {
        lv_timer_del(timer);
        timer = nullptr;
    }
}

void
StarMap::onViewUnload() {
    View.Delete();
}

void
StarMap::onViewDidUnload() {}

void
StarMap::onLanguageChanged() {
    View.ApplyLanguage();
    Update();
}

void
StarMap::AttachEvent(lv_obj_t* obj) {
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void
StarMap::Update() {
    int gps = systemInfo.starMapInfo.numberGPS;
    int bds = systemInfo.starMapInfo.numberBDS;
    int gln = systemInfo.starMapInfo.numberGLONASS;
    int gal = systemInfo.starMapInfo.numberGALILEO;
    int sbas = systemInfo.starMapInfo.numberSBAS;
    int qzss = systemInfo.starMapInfo.numberQZSS;
    int irnss = systemInfo.starMapInfo.numberIRNSS;

    View.UpdateValues(gps, bds, gln, gal, sbas, qzss, irnss);
}

void
StarMap::onTimerUpdate(lv_timer_t* timer) {
    auto* instance = static_cast<StarMap*>(timer->user_data);
    LV_ASSERT_NULL(instance);
    instance->Update();
}

void
StarMap::onEvent(lv_event_t* event) {
    const auto* instance = static_cast<StarMap*>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);

    const lv_obj_t* obj = lv_event_get_current_target(event);
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_SHORT_CLICKED) {
        if (obj == instance->_root) {
            instance->pageManager->Pop();
        }
    }
}
