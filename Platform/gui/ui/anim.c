#include "anim.h"

uint8_t BATT_USAGE_HEIGHT = 8;
uint8_t BATT_USAGE_WIDTH = 18;

/*Status Battery Function*/
void StatusBar_ConBattSetOpa(lv_obj_t *obj, int32_t opa)
{
	lv_obj_set_style_opa(obj, opa, 0);
}

void StatusBar_onAnimOpaFinish(lv_anim_t *a)
{
	lv_obj_t *obj = (lv_obj_t *)a->var;
	StatusBar_ConBattSetOpa(obj, LV_OPA_COVER);
	StatusBar_AnimCreate(obj);
}

void StatusBar_onAnimWidthFinish(lv_anim_t *a)
{
	lv_anim_t a_opa;
	lv_anim_init(&a_opa);
	lv_anim_set_var(&a_opa, a->var);
	lv_anim_set_exec_cb(&a_opa, (lv_anim_exec_xcb_t)StatusBar_ConBattSetOpa);
	lv_anim_set_ready_cb(&a_opa, StatusBar_onAnimOpaFinish);
	lv_anim_set_values(&a_opa, LV_OPA_COVER, LV_OPA_TRANSP);
	lv_anim_set_early_apply(&a_opa, true);
	lv_anim_set_delay(&a_opa, 500);
	lv_anim_set_time(&a_opa, 500);
	lv_anim_start(&a_opa);
}

void StatusBar_AnimSetWidth(void *var, int32_t v)
{
	lv_obj_set_width((lv_obj_t *)var, v);
}

void StatusBar_AnimCreate(lv_obj_t *contBatt)
{
	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_var(&a, contBatt);
	lv_anim_set_exec_cb(&a, StatusBar_AnimSetWidth);
	lv_anim_set_values(&a, 0, BATT_USAGE_WIDTH);
	lv_anim_set_time(&a, 1000);
	lv_anim_set_ready_cb(&a, StatusBar_onAnimWidthFinish);
	lv_anim_start(&a);
}

void PowerOff_AnimCreate(lv_obj_t *contBatt)
{
	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_var(&a, contBatt);
	lv_anim_set_exec_cb(&a, StatusBar_AnimSetWidth);
	lv_anim_set_values(&a, 0, BATT_USAGE_WIDTH);
	lv_anim_set_time(&a, 1000);
	lv_anim_set_ready_cb(&a, StatusBar_onAnimWidthFinish);
	lv_anim_start(&a);
}

void bar_anim_cb(void * obj, int32_t v)
{
    lv_bar_set_value((lv_obj_t*) obj, v, LV_ANIM_OFF);
    
    lv_label_set_text_fmt(objects.page_poweroff.percentage_label, "%d%%", v);
    
    if(v >= 100) {
        lv_label_set_text(objects.page_poweroff.label, "Sync Done. Power Off.");
        lv_obj_set_style_bg_color(objects.page_poweroff.bar, lv_color_hex(0x00AA00), LV_PART_INDICATOR);
    }
}

void poweroff_anim_create()
{
  lv_anim_init(&objects.page_poweroff.anim);
  lv_anim_set_var(&objects.page_poweroff.anim, objects.page_poweroff.bar);
  lv_anim_set_exec_cb(&objects.page_poweroff.anim, (lv_anim_exec_xcb_t)bar_anim_cb);
  lv_anim_set_time(&objects.page_poweroff.anim, 8000);
  lv_anim_set_values(&objects.page_poweroff.anim, 0, 100);
	lv_anim_start(&objects.page_poweroff.anim);
}