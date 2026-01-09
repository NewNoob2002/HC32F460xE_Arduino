#include "lvgl_screen.h"

lv_obj_t *scr3 = nullptr; /// main
lv_timer_t *scr3_timer = nullptr;

lv_obj_t *label_coordinate_big, *label_coordinate_lon_t, *label_coordinate_lat_t, *label_coordinate_alt_t;

void lv_screen3_init(void)
{
	scr3 = lv_obj_create(NULL);

	lv_singularxyz_coordinate(scr3, 2, 0 + screen_destition, 30);
	lv_update_coordinate();
	lv_singularxyz_coordinate_lon_text(scr3, 90 + screen_destition, 25);
	lv_update_coordinate_lon_text(); // ����
	lv_singularxyz_coordinate_lat_text(scr3, 90 + screen_destition, 50);
	lv_update_coordinate_lat_text(); // ��γ
	lv_singularxyz_coordinate_alt_text(scr3, 90 + screen_destition, 75);
	lv_update_coordinate_alt_text(); // �߳�
}

void lv_screen3_update()
{
		lv_update_coordinate();
		lv_update_coordinate_lon_text();
		lv_update_coordinate_lat_text();
		lv_update_coordinate_alt_text();
}

static void OnTimer(lv_timer_t *timer)
{
	lv_screen3_update();
	CORE_DEBUG_PRINTF("Screen3_timer");
}

void lv_screen3_appear(void)
{
	if(scr3_timer == nullptr)
	{
		CORE_DEBUG_PRINTF("lv_screen3_timer_create");
		scr3_timer = lv_timer_create(OnTimer, 1000, nullptr);
		lv_timer_ready(scr3_timer);
	}
	else
	{
		CORE_DEBUG_PRINTF("lv_screen3_timer_resume");
		lv_timer_resume(scr3_timer);
	}
	lv_scr_load_anim(scr3, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
	CORE_DEBUG_PRINTF("lv_screen3_appear");
}

void lv_screen3_disappear()
{
	if(scr3_timer)
	{
		CORE_DEBUG_PRINTF("lv_screen3_timer_stop");
		lv_timer_pause(scr3_timer);
	}
	CORE_DEBUG_PRINTF("lv_screen3_disappear");
}

void lv_singularxyz_coordinate(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_coordinate_big = lv_label_create(obj);
	lv_obj_remove_style_all(label_coordinate_big);
	
	lv_obj_align(label_coordinate_big, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
	lv_obj_set_style_text_color(label_coordinate_big, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_text_font(label_coordinate_big, &lv_font_singularxyz_53, 0);
	lv_label_set_text(label_coordinate_big, LV_SYMBOL_COORDINATE);
}

void lv_update_coordinate()
{
	if (systemInfo.positionInfo.coordinate_status == 0) // none
		lv_obj_set_style_text_color(label_coordinate_big, lv_palette_main(LV_PALETTE_RED), 0);
	else if (systemInfo.positionInfo.coordinate_status == 1) /// single
		lv_obj_set_style_text_color(label_coordinate_big, lv_palette_main(LV_PALETTE_YELLOW), 0);
	else if (systemInfo.positionInfo.coordinate_status == 4) /// fix
		lv_obj_set_style_text_color(label_coordinate_big, lv_palette_main(LV_PALETTE_GREEN), 0);
	else if (systemInfo.positionInfo.coordinate_status == 5) /// float
		lv_obj_set_style_text_color(label_coordinate_big, lv_palette_main(LV_PALETTE_YELLOW), 0);
	else if (systemInfo.positionInfo.coordinate_status == 7) /// FIXEDPOS
		lv_obj_set_style_text_color(label_coordinate_big, lv_palette_main(LV_PALETTE_BLUE), 0);
}

void lv_singularxyz_coordinate_lon_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_coordinate_lon_t = lv_label_create(obj);
	lv_obj_remove_style_all(label_coordinate_lon_t);
	
	lv_obj_align(label_coordinate_lon_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
	lv_obj_set_style_text_color(label_coordinate_lon_t, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_text_font(label_coordinate_lon_t, &font_oswaldBold_18, 0);
	lv_label_set_text(label_coordinate_lon_t, LV_SYMBOL_COORDINATE);
	
}

void lv_update_coordinate_lon_text()
{
	lv_label_set_text_fmt(label_coordinate_lon_t, "B:%0.6f", systemInfo.positionInfo.coordinate_lon);
}

void lv_singularxyz_coordinate_lat_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_coordinate_lat_t = lv_label_create(obj);
	lv_obj_remove_style_all(label_coordinate_lat_t);
	
	lv_obj_align(label_coordinate_lat_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
	lv_obj_set_style_text_color(label_coordinate_lat_t, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_text_font(label_coordinate_lat_t, &font_oswaldBold_18, 0);
	lv_label_set_text(label_coordinate_lat_t, LV_SYMBOL_COORDINATE);
}

void lv_update_coordinate_lat_text()
{
	lv_label_set_text_fmt(label_coordinate_lat_t, "L:%0.6f", systemInfo.positionInfo.coordinate_lat);
}

void lv_singularxyz_coordinate_alt_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_coordinate_alt_t = lv_label_create(obj);
	lv_obj_remove_style_all(label_coordinate_alt_t);
	
	lv_obj_align(label_coordinate_alt_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
	lv_obj_set_style_text_color(label_coordinate_alt_t, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_text_font(label_coordinate_alt_t, &font_oswaldBold_18, 0);
	lv_label_set_text(label_coordinate_alt_t, LV_SYMBOL_COORDINATE);
}

void lv_update_coordinate_alt_text()
{
	lv_label_set_text_fmt(label_coordinate_alt_t, "H:%0.6f", systemInfo.positionInfo.coordinate_alt);
}
