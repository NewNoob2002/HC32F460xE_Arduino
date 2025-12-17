#include "Shutdown.h"

using namespace Page;

void Shutdown::onCustomAttrConfig()
{
    LV_LOG_USER("begin");
}

void Shutdown::onViewLoad()
{
    LV_LOG_USER("begin");
    View.Create(_root);
}

void Shutdown::onViewDidLoad()
{
    LV_LOG_USER("begin");
}

void Shutdown::onViewWillAppear()
{
    LV_LOG_USER("begin");
}

void Shutdown::onViewDidAppear()
{
    LV_LOG_USER("begin");
}

void Shutdown::onViewWillDisappear()
{
    LV_LOG_USER("begin");
}

void Shutdown::onViewDidDisappear()
{
    LV_LOG_USER("begin");
    lv_timer_del(timer);
}

void Shutdown::onViewUnload()
{
    LV_LOG_USER("begin");
}

void Shutdown::onViewDidUnload()
{
    LV_LOG_USER("begin");
}

void Shutdown::AttachEvent(lv_obj_t* obj)
{
    lv_obj_set_user_data(obj, this);
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Shutdown::Update()
{
    lv_label_set_text_fmt(View.ui.labelTick, "tick = %d save = %d", Model.GetData(), Model.TickSave);
}

void Shutdown::onTimerUpdate(lv_timer_t* timer)
{
    auto* instance = static_cast<Shutdown *>(timer->user_data);

    instance->Update();
}

void Shutdown::onEvent(lv_event_t* event)
{
    auto* instance = static_cast<Shutdown *>(lv_event_get_user_data(event));
    LV_ASSERT_NULL(instance);
}
