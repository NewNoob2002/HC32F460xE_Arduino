#include "Startup.h"

using namespace Page;

Startup::Startup()
= default;

Startup::~Startup()
= default;

void Startup::onCustomAttrConfig()
{
    SetCustomCacheEnable(false);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void Startup::onViewLoad()
{
    View.Create(_root);
}

void Startup::onViewDidLoad()
{
    lv_timer_t *timer = lv_timer_create(onTimer, 1000, this);
    lv_timer_resume(timer);
}

void Startup::onViewWillAppear()
{
}

void Startup::onViewDidAppear()
{

}

void Startup::onViewWillDisappear()
{

}

void Startup::onViewDidDisappear()
{
}

void Startup::onViewUnload()
{

}

void Startup::onViewDidUnload()
{
}

void Startup::onTimer(lv_timer_t* timer)
{
    auto* instance = static_cast<Startup*>(timer->user_data);
    instance->View.Update();
}

void Startup::onEvent(lv_event_t* event)
{
}
