#include "delay.h"
#include "lvgl_screen.h"

#define BATT_USAGE_WIDTH 18
#define BATT_USAGE_HEIGHT 8

lv_obj_t *scr6 = nullptr;
lv_timer_t *scr6_timer = nullptr;

struct {
	struct {
        lv_obj_t *img;
        lv_obj_t *objUsage;
        lv_obj_t *percent;
    } battery;
			
  lv_obj_t *arc;
  lv_obj_t *arc_percent;
  lv_anim_t arc_anim;

  lv_obj_t *btnPress;
	
} ui;

static void StatusBar_ConBattSetOpa(lv_obj_t *obj, int32_t opa);

static void StatusBar_onAnimOpaFinish(lv_anim_t *a);

static void StatusBar_AnimCreate(lv_obj_t *contBatt);

static void StatusBar_onAnimWidthFinish(lv_anim_t *a);

static void StatusBar_ConBattSetOpa(lv_obj_t *obj, int32_t opa) {
    lv_obj_set_style_opa(obj, opa, 0);
}

static void StatusBar_onAnimOpaFinish(lv_anim_t *a) {
    auto *obj = static_cast<lv_obj_t *>(a->var);
    StatusBar_ConBattSetOpa(obj, LV_OPA_COVER);
    StatusBar_AnimCreate(obj);
}

static void StatusBar_onAnimWidthFinish(lv_anim_t *a) {
    lv_anim_t a_opa;
    lv_anim_init(&a_opa);
    lv_anim_set_var(&a_opa, a->var);
    lv_anim_set_exec_cb(&a_opa, reinterpret_cast<lv_anim_exec_xcb_t>(StatusBar_ConBattSetOpa));
    lv_anim_set_ready_cb(&a_opa, StatusBar_onAnimOpaFinish);
    lv_anim_set_values(&a_opa, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_early_apply(&a_opa, true);
    lv_anim_set_delay(&a_opa, 500);
    lv_anim_set_time(&a_opa, 500);
    lv_anim_start(&a_opa);
}

static void StatusBar_AnimCreate(lv_obj_t *contBatt) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, contBatt);
    lv_anim_set_exec_cb(&a, LV_ANIM_EXEC(width));
    lv_anim_set_values(&a, 0, BATT_USAGE_WIDTH);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_ready_cb(&a, StatusBar_onAnimWidthFinish);
    lv_anim_start(&a);
}

static void lv_anim_arc_set_value(void * obj, int32_t value)
{
    lv_anim_t *anim = lv_anim_get(obj, lv_anim_arc_set_value);
    if(!anim) return;

    lv_arc_set_value(static_cast<lv_obj_t *>(obj), value);

    if(ui.arc_percent) {
        lv_label_set_text_fmt(ui.arc_percent, "%d%%", value);
    }

    if (value >= 100) {

        if(!systemInfo.powerMonitor.panel_power_on)
        {
            systemInfo.powerMonitor.panel_power_on = true;
						systemInfo.powerMonitor.pannel_power_on_time = lv_tick_get();
						lv_arc_set_value(static_cast<lv_obj_t *>(obj), 0);
						lv_label_set_text_fmt(ui.arc_percent, "0%");
						lv_screen6_disappear();
						lv_screen8_appear();
        }
    }
}

static void onEvent(lv_event_t *event) {
    if (systemInfo.powerMonitor.panel_power_on) return;

    const lv_event_code_t code = lv_event_get_code(event);

    // 获取当前进度，用于断点续传或回退
    const int16_t current_value = lv_arc_get_value(ui.arc);

    if (code == LV_EVENT_PRESSED) {
				CORE_DEBUG_PRINTF("StartUp Press");
        lv_anim_del(ui.arc, lv_anim_arc_set_value);

        lv_anim_set_values(&ui.arc_anim, current_value, 100);

        uint32_t boot_time_ms = 2000; // 设定长按开机时间为2秒
        uint32_t remaining_time = boot_time_ms * (100 - current_value) / 100;

        lv_anim_set_time(&ui.arc_anim, remaining_time);
        lv_anim_set_path_cb(&ui.arc_anim, lv_anim_path_linear);
        lv_anim_start(&ui.arc_anim);
    }
    // === 松开或焦点丢失：回退清零 ===
    else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {

        // 停止充能动画
        lv_anim_del(ui.arc, lv_anim_arc_set_value);

        // 如果还没满，开始回退动画：从当前值回到 0
        if (current_value < 100) {
            lv_anim_set_values(&ui.arc_anim, current_value, 0);

            // 回退速度可以快一点，比如固定 300ms 或者 500ms
            lv_anim_set_time(&ui.arc_anim, 300);
            lv_anim_set_path_cb(&ui.arc_anim, lv_anim_path_ease_out);
            lv_anim_start(&ui.arc_anim);
        }
    }
}

static void OnTimer(lv_timer_t *timer)
{
	    // /* battery */
		lv_label_set_text_fmt(ui.battery.percent, "%d", systemInfo.powerMonitor.batteryInfo.Percent);
		
    const bool Is_BattCharging =  systemInfo.powerMonitor.batteryInfo.chargeStatus != notCharge;
    lv_obj_t *contBatt = ui.battery.objUsage;
    static bool Is_BattChargingAnimActive = false;
    if (Is_BattCharging) {
        if (!Is_BattChargingAnimActive) {
            lv_obj_set_style_bg_color(contBatt, lv_color_hex(0x4CAF50), 0);
            StatusBar_AnimCreate(contBatt);
            Is_BattChargingAnimActive = true;
        }
    } else {
        if (Is_BattChargingAnimActive) {
            lv_anim_del(contBatt, nullptr);
            StatusBar_ConBattSetOpa(contBatt, LV_OPA_COVER);
            Is_BattChargingAnimActive = false;
        }
        lv_color_t battery_color;
        if (const uint16_t battery_percentage =  systemInfo.powerMonitor.batteryInfo.Percent; battery_percentage > 50) {
            battery_color = lv_color_hex(0x4CAF50); // 绿色
        } else if (battery_percentage > 20 && battery_percentage <= 50) {
            battery_color = lv_color_hex(0xFF9800); // 橙色
        } else if (battery_percentage <= 20) {
            battery_color = lv_color_hex(0xF44336); // 红色
        }
        lv_obj_set_style_bg_color(contBatt, battery_color, 0);
        const lv_coord_t width = lv_map( systemInfo.powerMonitor.batteryInfo.Percent, 0, 100, 0, BATT_USAGE_WIDTH);
        lv_obj_set_width(contBatt, width);
    }
}

void lv_screen6_init(void)
{
	scr6 = lv_obj_create(NULL);
	
	lv_obj_t *root = scr6;
	
	  lv_obj_t *img = lv_img_create(scr6);
		lv_obj_remove_style_all(img);
		lv_label_set_recolor(img, true);
		lv_obj_set_style_text_font(img, &font_oswaldBold_18, 0);
		lv_obj_set_style_text_color(img, lv_color_white(), 0);
		lv_label_set_text(img, LV_SYMBOL_BATTERY_EMPTY);
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, -40, 15);
    ui.battery.img = img;

    lv_obj_t *obj = lv_obj_create(img);
    lv_obj_remove_style_all(obj);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4CAF50), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_size(obj, BATT_USAGE_WIDTH, BATT_USAGE_HEIGHT);
    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 2, 0);
    ui.battery.objUsage = obj;
	
		lv_obj_t *label = lv_label_create(scr6);
		lv_obj_set_style_text_font(label, &font_oswaldBold_18, 0);
		lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
		lv_obj_align_to(label, img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
		ui.battery.percent = label;
		
	constexpr lv_coord_t arc_size = 126 * 0.6;
    lv_obj_t *arc = lv_arc_create(root);
    lv_obj_set_size(arc, arc_size, arc_size);
    lv_obj_center(arc);
    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 0);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, nullptr, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x505050), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    ui.arc = arc;

    lv_obj_t *label_percent = lv_label_create(root);
    lv_label_set_text(label_percent, "0%");
    lv_obj_center(label_percent);
    lv_obj_set_style_text_font(label_percent, &font_oswaldBold_18, 0);
    ui.arc_percent = label_percent;

    lv_anim_init(&ui.arc_anim);
    lv_anim_set_var(&ui.arc_anim, ui.arc);
    lv_anim_set_time(&ui.arc_anim, 200);
    lv_anim_set_playback_time(&ui.arc_anim, 0);
    lv_anim_set_repeat_count(&ui.arc_anim, 0);
    lv_anim_set_path_cb(&ui.arc_anim, lv_anim_path_linear);
		lv_anim_set_exec_cb(&ui.arc_anim, lv_anim_arc_set_value);

    lv_obj_t* btnPress = lv_obj_create(root);
    lv_obj_remove_style_all(btnPress);
    lv_obj_set_size(btnPress, 55, 40);
    lv_obj_clear_flag(btnPress, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(btnPress, LV_ALIGN_CENTER, 80, 0);

    lv_obj_set_style_bg_opa(btnPress, LV_OPA_COVER, 0);
    lv_obj_set_style_width(btnPress, 60, LV_STATE_PRESSED);
    lv_obj_set_style_height(btnPress, 35, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xdd3c3b), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btnPress, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(btnPress, 9, 0);

    static lv_style_transition_dsc_t tran;
    static constexpr lv_style_prop_t prop[] = { LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
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
    ui.btnPress = btnPress;

    lv_obj_t *label_btn = lv_label_create(btnPress);
    lv_obj_remove_style_all(label_btn);
    lv_obj_set_style_text_font(label_btn, &font_oswaldBold_18, 0);
    lv_label_set_text(label_btn, "PRESS");
    lv_obj_center(label_btn);

    // Firmware version label
    lv_obj_t *label_firmware = lv_label_create(root);
    lv_label_set_text_fmt(label_firmware, "Rev:%s", SOFTWARE_VERSION);
    lv_obj_set_style_text_font(label_firmware, &font_oswaldBold_12, 0);
    lv_obj_set_style_text_color(label_firmware, lv_palette_main(LV_PALETTE_GREY),
                                0);
    lv_obj_align(label_firmware, LV_ALIGN_BOTTOM_LEFT, 10, 0);

    // Compile time label
    lv_obj_t *label_compile_time = lv_label_create(root);
    lv_label_set_text(label_compile_time, __DATE__ " " __TIME__);
    lv_obj_set_style_text_font(label_compile_time, &font_oswaldBold_12, 0);
    lv_obj_set_style_text_color(label_compile_time,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_compile_time, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
		
		lv_obj_add_event_cb(btnPress, onEvent, LV_EVENT_ALL, nullptr);
}

void lv_screen6_appear()
{
  lv_group_t *group = lv_group_get_default();
  LV_ASSERT_NULL(group);
  lv_group_add_obj(group, ui.btnPress);
  lv_group_focus_obj(ui.btnPress);
	if(scr6_timer == nullptr)
	{
		scr6_timer = lv_timer_create(OnTimer, 1000, nullptr);
		lv_timer_ready(scr6_timer);
	}
	lv_scr_load_anim(scr6, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, false);
	CORE_DEBUG_PRINTF("scr6_appear");
}

void lv_screen6_disappear(void)
{
	lv_group_t *group = lv_group_get_default();
  LV_ASSERT_NULL(group);
	lv_group_remove_obj(ui.btnPress);
	if(scr6_timer)
	{
		CORE_DEBUG_PRINTF("scr6_timer_pause");
		lv_timer_del(scr6_timer);
	}
	CORE_DEBUG_PRINTF("scr6_disappear");
}