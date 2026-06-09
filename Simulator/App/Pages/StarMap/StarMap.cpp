#include "StarMap.h"

using namespace Page;

StarMap::StarMap() = default;

StarMap::~StarMap() = default;

void
StarMap::onCustomAttrConfig() {
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void
StarMap::onViewLoad() {
    Model.Init();
    View.Create(_root);
    AttachEvent(_root);
}

void
StarMap::onViewDidLoad() {}

void
StarMap::onViewWillAppear() {
    Model.SetStatusBarStyle(DataProc::STATUS_BAR_STYLE_BLACK);

    timer = lv_timer_create(onTimerUpdate, 1000, this);
    lv_timer_ready(timer);

    lv_obj_set_style_opa(_root, LV_OPA_TRANSP, 0);
    lv_obj_fade_in(_root, 300, 0);
}

void
StarMap::onViewDidAppear() {}

void
StarMap::onViewWillDisappear() {}

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
    Model.Deinit();
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

    View.UpdateValues(gps, bds, gln, gal);
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

    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_PRESSED) {
        instance->pageManager->Pop();
    }

    if (obj == instance->_root) {
        if (code == LV_EVENT_LEAVE) {
            instance->pageManager->Pop();
        }
    }
}
