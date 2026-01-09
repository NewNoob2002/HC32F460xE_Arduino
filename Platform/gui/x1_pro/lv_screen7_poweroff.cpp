#include "lvgl_screen.h"

lv_obj_t *scr7 = nullptr;
lv_timer_t *scr7_timer = nullptr;

struct {
  struct {
    lv_obj_t *cont;
    struct {
      lv_obj_t *cont;
      lv_obj_t *label;
      lv_obj_t *obj;
      lv_anim_t anim;
    } bar;
    lv_obj_t *btnPress;
  } shutdown;
  struct {
    lv_obj_t *cont;
    lv_obj_t *label;
    struct {
      lv_obj_t *obj;
      lv_anim_t anim;
    } bar;
  } sync;
  lv_anim_timeline_t *anim_timeline;
} ui;


static bool anim_complement_callback_do = false;
extern volatile SharedData_t shared_info;

static void onEvent(lv_event_t *event);

static void lv_anim_obj_set_width(void *obj, int32_t width)
{
    lv_anim_t *anim = lv_anim_get(obj, lv_anim_obj_set_width);
    if (!anim) return;

    lv_obj_set_width((lv_obj_t*)obj, width);

    if (ui.shutdown.bar.label) {
        lv_label_set_text_fmt(ui.shutdown.bar.label, "%d%%", width);
    }
    if (width >= 100) {
        if (!anim_complement_callback_do) {
            anim_complement_callback_do = true;
            lv_obj_fade_out(ui.shutdown.cont, 300, 0);
            lv_obj_clear_flag(ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
            lv_obj_fade_in(ui.sync.cont, 300, 0);
            lv_anim_timeline_start(ui.anim_timeline);
            lv_anim_start(&ui.sync.bar.anim);
            HAL::Power_Shutdown(false);
            HAL::PowerKey_SetEnable(false);
        }
    }
}

static void syncbar_timer_callback(lv_timer_t *timer)
{
//	shared_info.command = CMD_SKIP_DELAY;
//	shared_info.reset_count = 0x2006;
	NVIC_SystemReset();
}

static void syncbar_anim_done_callback(lv_anim_t *a)
{
    HAL::Power_Shutdown(true);
    lv_timer_t *timer = lv_timer_create(syncbar_timer_callback, 1000, nullptr);
    lv_timer_set_repeat_count(timer, 1);
}

void lv_screen7_init(void)
{
		scr7 = lv_obj_create(NULL);

	  const lv_font_t *font = &font_oswaldBold_18;
    const lv_font_t *font_small = &font_oswaldBold_12;
		
		lv_obj_t *root = scr7;
    lv_obj_t *main_cont = lv_obj_create(root);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(main_cont);
    ui.shutdown.cont = main_cont;

    lv_obj_t *label = lv_label_create(main_cont);
    lv_obj_remove_style_all(label);
    lv_obj_set_style_text_font(label, font_small, 0);
    lv_label_set_text(label, "Keep Press Shutdown");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t *cont = lv_obj_create(main_cont);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 100, 10);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 10);
    ui.shutdown.bar.cont = cont;

    lv_obj_t *bar = lv_obj_create(cont);
    lv_obj_remove_style_all(bar);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0xf9181c), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_width(bar, 0);
    lv_obj_set_height(bar, 10);
    lv_obj_align(bar, LV_ALIGN_LEFT_MID, 0, 0);
    ui.shutdown.bar.obj = bar;

    lv_anim_init(&ui.shutdown.bar.anim);
    lv_anim_set_var(&ui.shutdown.bar.anim, bar);
    lv_anim_set_playback_time(&ui.shutdown.bar.anim, 0);
    lv_anim_set_repeat_count(&ui.shutdown.bar.anim, 0);
		lv_anim_set_exec_cb(&ui.shutdown.bar.anim, lv_anim_obj_set_width);

    lv_obj_t *barPercent = lv_label_create(main_cont);
    lv_obj_remove_style_all(barPercent);
    lv_obj_set_style_text_font(barPercent, font_small, 0);
    lv_label_set_text(barPercent, "0%");
    lv_obj_align(barPercent, LV_ALIGN_CENTER, 70, 10);
    ui.shutdown.bar.label = barPercent;

    lv_obj_t *btnPress = lv_obj_create(main_cont);
    lv_obj_remove_style_all(btnPress);
    lv_obj_set_size(btnPress, 50, 30);
    lv_obj_clear_flag(btnPress, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(btnPress, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_set_style_bg_opa(btnPress, LV_OPA_COVER, 0);
    lv_obj_set_style_width(btnPress, 60, LV_STATE_PRESSED);
    lv_obj_set_style_height(btnPress, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xdd3c3b), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(btnPress, 9, 0);

    static lv_style_transition_dsc_t tran;
    static const lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
    lv_style_transition_dsc_init(
        &tran,
        prop,
        lv_anim_path_ease_out,
        200,
        0,
        nullptr
    );
    lv_obj_set_style_transition(btnPress, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(btnPress, &tran, LV_STATE_FOCUSED);
    lv_obj_update_layout(btnPress);
    ui.shutdown.btnPress = btnPress;

    lv_obj_t *label_btn = lv_label_create(btnPress);
    lv_obj_remove_style_all(label_btn);
    lv_obj_set_style_text_font(label_btn, font, 0);
    lv_label_set_text(label_btn, "PRESS");
    lv_obj_center(label_btn);

    /*
     * Second screen;
     */
    lv_obj_t *second_cont = lv_obj_create(root);
    lv_obj_remove_style_all(second_cont);
    lv_obj_set_size(second_cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(second_cont);
    lv_obj_add_flag(second_cont, LV_OBJ_FLAG_HIDDEN);
    ui.sync.cont = second_cont;

    lv_obj_t *sync_label = lv_label_create(second_cont);
    lv_obj_set_style_text_font(sync_label, font, 0);
    lv_obj_set_style_text_color(sync_label, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_label_set_text(sync_label, "Warming: Saving SystemConfig");
    lv_obj_center(sync_label);
    ui.sync.label = sync_label;

    lv_obj_t *bar_cont = lv_obj_create(second_cont);
    lv_obj_remove_style_all(bar_cont);
    lv_obj_set_style_border_color(bar_cont, lv_color_hex(0x8BCA93), 0);
    lv_obj_set_style_border_width(bar_cont, 2, 0);
    lv_obj_set_size(bar_cont, 102, 12);
    lv_obj_align(bar_cont, LV_ALIGN_CENTER, 0, 40);

    lv_obj_t *sync_bar = lv_obj_create(bar_cont);
    lv_obj_remove_style_all(sync_bar);
    lv_obj_set_size(sync_bar, 0, 10);
    lv_obj_set_style_bg_color(sync_bar, lv_color_hex(0x8BCA93), 0);
    lv_obj_set_style_bg_opa(sync_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(sync_bar, LV_OPA_COVER, 0);
    lv_obj_align(sync_bar, LV_ALIGN_LEFT_MID, 0, 0);
    ui.sync.bar.obj = sync_bar;

    lv_anim_init(&ui.sync.bar.anim);
    lv_anim_set_var(&ui.sync.bar.anim, sync_bar);
    lv_anim_set_exec_cb(&ui.sync.bar.anim, LV_ANIM_EXEC(width));
    lv_anim_set_values(&ui.sync.bar.anim, 0, 100);
    lv_anim_set_time(&ui.sync.bar.anim, 8000);
    lv_anim_set_playback_time(&ui.sync.bar.anim, 0);
    lv_anim_set_repeat_count(&ui.sync.bar.anim, 0);
		lv_anim_set_deleted_cb(&ui.sync.bar.anim, syncbar_anim_done_callback);

    ui.anim_timeline = lv_anim_timeline_create();
#define ANIM_DEF(start_time, obj, attr, start, end) \
{start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    const lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, bar_cont, width, 0, lv_obj_get_style_width(bar_cont, 0)),
        ANIM_DEF(300, sync_label, y, lv_obj_get_style_height(second_cont, 0), lv_obj_get_y(sync_label)),
        LV_ANIM_TIMELINE_WRAPPER_END
    };

    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
		
		lv_obj_add_event_cb(btnPress, onEvent, LV_EVENT_ALL, nullptr);
}

static void onTimerUpdate(lv_timer_t *timer)
{
	    if (HAL::Power_ShutdownForce()) {
				Led_Charge_switch(0);
				Led_Power_switch(1);
				Led_Function_switch(1);

        lv_obj_set_style_text_color(ui.sync.label, lv_palette_main(LV_PALETTE_RED), 0);
        lv_label_set_text(ui.sync.label, "Info: PowerKey Shutdown");
				lv_obj_clear_flag(ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
				lv_obj_fade_in(ui.sync.cont, 300, 0);
        lv_anim_timeline_start(ui.anim_timeline);
        lv_anim_start(&ui.sync.bar.anim);
        HAL::PowerKey_SetEnable(false);
				CORE_DEBUG_PRINTF("POWER OFF .................Power Off Cause: PowerKey Shutdown");
    } else if (HAL::Power_ShutdownLinux()) {
				Led_Charge_switch(0);
				Led_Power_switch(1);
				Led_Function_switch(1);

        lv_label_set_text(ui.sync.label, "Info: Linux Shutdown");
				lv_obj_clear_flag(ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
				lv_obj_fade_in(ui.sync.cont, 300, 0);
        lv_anim_timeline_start(ui.anim_timeline);
        lv_anim_start(&ui.sync.bar.anim);
        HAL::PowerKey_SetEnable(false);
				CORE_DEBUG_PRINTF("POWER OFF .................Power Off Cause: Linux Shutdown");
    } else if (HAL::Power_ShutdownLowBattery()) {
				Led_Charge_switch(0);
				Led_Power_switch(1);
				Led_Function_switch(1);

        lv_obj_set_style_text_color(ui.sync.label, lv_palette_main(LV_PALETTE_RED), 0);
        lv_label_set_text(ui.sync.label, "Warming: LowBattery Shutdown");
				lv_obj_clear_flag(ui.sync.cont, LV_OBJ_FLAG_HIDDEN);
				lv_obj_fade_in(ui.sync.cont, 300, 0);
        lv_anim_timeline_start(ui.anim_timeline);
        lv_anim_start(&ui.sync.bar.anim);
        HAL::PowerKey_SetEnable(false);
				CORE_DEBUG_PRINTF("POWER OFF .................Power Off Cause: LowBattery Shutdown");
    }
		CORE_DEBUG_PRINTF("scr7_timer");
}

static void onEvent(lv_event_t *event)
{
    if (const lv_event_code_t code = lv_event_get_code(event); code == LV_EVENT_SHORT_CLICKED) {
        CORE_DEBUG_PRINTF("ShutdownKey Short Pressed");
    } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESSED) {
			
        lv_anim_t anim                  = ui.shutdown.bar.anim;
        const int16_t current_width     = lv_obj_get_width(ui.shutdown.bar.obj);
        constexpr uint32_t boot_time_ms = 2000; // 设定长按关机时间为2秒
        const uint32_t remaining_time   = boot_time_ms * (100 - current_width) / 100;
        switch (code) {
            case LV_EVENT_PRESSED: {
                lv_anim_del(ui.shutdown.bar.obj, lv_anim_obj_set_width);
                lv_anim_set_path_cb(&anim, lv_anim_path_linear);
                lv_anim_set_values(&anim, current_width, 100);
                lv_anim_set_time(&anim, remaining_time);
                lv_anim_start(&anim);
                break;
            }
            case LV_EVENT_RELEASED: {
                if (current_width < 100) {
                    lv_anim_del(ui.shutdown.bar.obj, lv_anim_obj_set_width);

                    lv_anim_set_values(&anim, current_width, 0);
                    lv_anim_set_time(&anim, 300);
                    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
                    lv_anim_start(&anim);
                }
            }
            default:;
        }
    }
}

void lv_screen7_appear()
{
	if(scr7_timer == nullptr)
	{
		scr7_timer = lv_timer_create(onTimerUpdate, 1000, nullptr);
		CORE_DEBUG_PRINTF("scr7_timer_create");
	}
	else
	{
		lv_timer_resume(scr7_timer);
		lv_timer_ready(scr7_timer);
		CORE_DEBUG_PRINTF("scr7_timer_resume");
	}
	lv_group_t *group = lv_group_get_default();
  LV_ASSERT_NULL(group);
  lv_group_add_obj(group, ui.shutdown.btnPress);
  lv_group_focus_obj(ui.shutdown.btnPress);
	lv_obj_clear_flag(ui.shutdown.cont, LV_OBJ_FLAG_HIDDEN);
	lv_scr_load_anim(scr7, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
	CORE_DEBUG_PRINTF("lv_screen7_appear");
}

void lv_screen7_appear_flag()
{
	if(scr7_timer == nullptr)
	{
		scr7_timer = lv_timer_create(onTimerUpdate, 1000, nullptr);
		CORE_DEBUG_PRINTF("scr7_timer_create");
	}
	else
	{
		lv_timer_resume(scr7_timer);
		lv_timer_ready(scr7_timer);
		CORE_DEBUG_PRINTF("scr7_timer_resume");
	}
	lv_group_t *group = lv_group_get_default();
  LV_ASSERT_NULL(group);
  lv_group_add_obj(group, ui.shutdown.btnPress);
  lv_group_focus_obj(ui.shutdown.btnPress);
	lv_obj_add_flag(ui.shutdown.cont, LV_OBJ_FLAG_HIDDEN);
	lv_scr_load_anim(scr7, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
	CORE_DEBUG_PRINTF("lv_screen7_appear");
}

void lv_screen7_disappear(void)
{
	lv_group_t *group = lv_group_get_default();
  LV_ASSERT_NULL(group);
	lv_group_remove_all_objs(group);
	if(scr7_timer)
	{
		CORE_DEBUG_PRINTF("lv_screen7_timer_stop");
		lv_timer_pause(scr7_timer);
	}
	CORE_DEBUG_PRINTF("lv_screen7_disappear");
}