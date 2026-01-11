#include "lvgl_screen.h"

lv_obj_t *scr1 = nullptr; /// main
lv_timer_t *scr1_timer = nullptr;

//struct {
//  struct {
//    lv_obj_t *cont;
//    struct {
//      lv_obj_t *obj;
//      lv_anim_t anim;
//    } bar;
//    lv_obj_t *btnPress;
//  } shutdown;
//} ui;

void lv_screen1_init(void)
{
	scr1 = lv_obj_create(NULL);

	lv_screen1_top_init();
	lv_screen1_bottom_init();
}

void lv_screen1_update()
{
	lv_singularxyz_update_top_status();
	lv_singularxyz_update_bottom_main();
	lv_update_battery(systemInfo.powerMonitor.batteryInfo.Percent);
}

static void OnTimer(lv_timer_t *timer)
{
	lv_screen1_update();
	CORE_DEBUG_PRINTF("Screen1_timer");
}

void lv_screen1_appear(void)
{
	if(scr1_timer == nullptr)
	{
		CORE_DEBUG_PRINTF("lv_screen1_timer_create");
		scr1_timer = lv_timer_create(OnTimer, 1000, nullptr);
		lv_timer_ready(scr1_timer);
	}
	else
	{
		CORE_DEBUG_PRINTF("lv_screen1_timer_resume");
		lv_timer_resume(scr1_timer);
	}
	lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, false);
	CORE_DEBUG_PRINTF("lv_screen1_appear");
}

void lv_screen1_disappear()
{
	if(scr1_timer)
	{
		CORE_DEBUG_PRINTF("lv_screen1_timer_stop");
		lv_timer_pause(scr1_timer);
	}
	CORE_DEBUG_PRINTF("lv_screen1_disappear");
}