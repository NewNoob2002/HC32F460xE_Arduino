#include "lvgl_screen.h"

lv_obj_t *screen1_bottom = NULL;

lv_obj_t *label_satellite_big, *label_sat_num, *label_sat_num_real, *label_wm_big, *label_wm_t, *label_position_big, *label_position_t;

void lv_screen1_bottom_init(void)
{
	screen1_bottom = lv_label_create(scr1);
	lv_label_set_text(screen1_bottom, "");
	lv_obj_set_pos(screen1_bottom, 0, 45); // x,y

	lv_obj_align(screen1_bottom, LV_ALIGN_TOP_MID, 0, 0);
	lv_obj_set_width(screen1_bottom, lv_pct(100));
	lv_obj_set_height(screen1_bottom, 120);

	lv_singularxyz_satellite(screen1_bottom, 5 + screen_destition, 10);

	lv_singularxyz_satellite_num_real(screen1_bottom, 1, 130 + screen_destition, 20);
	lv_update_satellite_num_real(0);

	lv_singularxyz_satellite_num(screen1_bottom, 1, -165 + screen_destition, 11);
	lv_update_satellite_num(0);

	lv_singularxyz_workmode(screen1_bottom, 1, 175 + screen_destition, 2);
	lv_update_workmode(1);
	lv_singularxyz_workmode_text(screen1_bottom, 1, 42 + screen_destition, 30);
	lv_update_workmode_text(1);
	lv_singularxyz_position(screen1_bottom, 1, 238 + screen_destition, 2);
	lv_update_position(0);
	lv_singularxyz_position_text(screen1_bottom, 107 + screen_destition, 30);
	lv_update_position_text(0);
}

void lv_singularxyz_satellite(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_satellite_big = lv_label_create(obj);
	lv_obj_remove_style_all(label_satellite_big);
	lv_label_set_text(label_satellite_big, LV_SYMBOL_SATELLITE);
	lv_obj_align(label_satellite_big, LV_ALIGN_LEFT_MID, x_ofs, y_ofs);
	lv_obj_set_style_text_font(label_satellite_big, &lv_font_singularxyz_48, 0);
	lv_obj_set_style_text_color(label_satellite_big, lv_palette_main(LV_PALETTE_INDIGO), 0);
}

void lv_singularxyz_satellite_num(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_sat_num = lv_label_create(obj);
	lv_obj_remove_style_all(label_sat_num);
	
	lv_obj_align(label_sat_num, LV_ALIGN_RIGHT_MID, x_ofs, y_ofs);
	lv_obj_set_style_text_font(label_sat_num, &font_rajdhaniBold_40, 0);
	lv_obj_set_style_text_color(label_sat_num, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_satellite_num(unsigned char value)
{
	lv_label_set_text_fmt(label_sat_num, "%d", value);
}

void lv_singularxyz_satellite_num_real(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_sat_num_real = lv_label_create(obj);
	lv_obj_remove_style_all(label_sat_num_real);
	
	lv_obj_align(label_sat_num_real, LV_ALIGN_LEFT_MID, x_ofs, y_ofs);
	lv_obj_set_style_text_font(label_sat_num_real, &font_rajdhaniBold_20, 0);
	lv_obj_set_style_text_color(label_sat_num_real, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_satellite_num_real(unsigned char value)
{
	lv_label_set_text_fmt(label_sat_num_real, "/%d", value);
}

void lv_singularxyz_workmode(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{

	label_wm_big = lv_label_create(obj);
	lv_obj_remove_style_all(label_wm_big);
	
	lv_obj_align(label_wm_big, LV_ALIGN_LEFT_MID, x_ofs, y_ofs);
	
	lv_obj_set_style_text_font(label_wm_big, &lv_font_singularxyz_32, 0);

	lv_obj_set_style_text_color(label_wm_big, lv_palette_main(LV_PALETTE_CYAN), 0);

}

void lv_update_workmode(unsigned char mode)
{
	if (0 == mode)
		lv_label_set_text(label_wm_big, LV_SYMBOL_ROVER);
	else if (1 == mode)
		lv_label_set_text(label_wm_big, LV_SYMBOL_BASE);
	else if (2 == mode)
		lv_label_set_text(label_wm_big, LV_SYMBOL_ROVER);
	else if (3 == mode)
		lv_label_set_text(label_wm_big, LV_SYMBOL_BASE);
}

void lv_singularxyz_workmode_text(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_wm_t = lv_label_create(obj);
	lv_obj_remove_style_all(label_wm_t);
	
	lv_obj_align(label_wm_t, LV_ALIGN_CENTER, x_ofs, y_ofs);
	lv_obj_set_style_text_font(label_wm_t, &font_oswaldBold_18, 0);
	lv_obj_set_style_text_color(label_wm_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_workmode_text(unsigned char mode)
{
	if (0 == mode || 2 == mode)
	{
		lv_label_set_text(label_wm_t, "Rover");
	}
	else if (1 == mode || 3 == mode)
	{
		lv_label_set_text(label_wm_t, "Base");
	}
}

void lv_singularxyz_position(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_position_big = lv_label_create(obj);
	lv_obj_remove_style_all(label_position_big);
	
	lv_label_set_text(label_position_big, LV_SYMBOL_POSITION);
	lv_obj_align(label_position_big, LV_ALIGN_LEFT_MID, x_ofs, y_ofs);
	lv_obj_set_style_text_font(label_position_big, &lv_font_singularxyz_32, 0);
}

void lv_update_position(unsigned char state)
{
	if (0 == state) // none
		lv_obj_set_style_text_color(label_position_big, lv_palette_main(LV_PALETTE_RED), 0);
	else if (1 == state) /// single
		lv_obj_set_style_text_color(label_position_big, lv_palette_main(LV_PALETTE_YELLOW), 0);
	else if (2 == state) /// fix
		lv_obj_set_style_text_color(label_position_big, lv_palette_main(LV_PALETTE_GREEN), 0);
	else if (3 == state) /// float
		lv_obj_set_style_text_color(label_position_big, lv_palette_main(LV_PALETTE_YELLOW), 0);
	else if (4 == state) /// FIXEDPOS
		lv_obj_set_style_text_color(label_position_big, lv_palette_main(LV_PALETTE_BLUE), 0);
}

void lv_singularxyz_position_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	label_position_t = lv_label_create(obj);
	lv_obj_remove_style_all(label_position_t);
	
	lv_obj_align(label_position_t, LV_ALIGN_CENTER, x_ofs, y_ofs);
	lv_obj_set_style_text_font(label_position_t, &font_oswaldBold_18, 0);
	lv_obj_set_style_text_color(label_position_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_position_text(unsigned char state)
{
	if (0 == state) /// None
	{
		lv_label_set_text(label_position_t, "None");
	}
	else if (1 == state) /// Single
	{
		lv_label_set_text(label_position_t, "Single");
	}
	else if (2 == state) /// RTD
		lv_label_set_text(label_position_t, "RTD");
	else if (4 == state) /// fix
	{
		lv_label_set_text(label_position_t, "Fix");
	}
	else if (5 == state) /// float
	{
		lv_label_set_text(label_position_t, "Float");
	}
	else if (7 == state) /// FIXEDPOS
	{
		lv_label_set_text(label_position_t, "Fixed");
	}
}

void lv_singularxyz_update_bottom_main()
{
	lv_update_satellite_num(systemInfo.positionInfo.satellite_number_used); //
	lv_update_satellite_num_real(systemInfo.positionInfo.satellite_number_track);
	lv_update_workmode(systemInfo.work_mode);
	lv_update_workmode_text(systemInfo.work_mode);
	lv_update_position(systemInfo.positionInfo.coordinate_status);
	lv_update_position_text(systemInfo.positionInfo.coordinate_status);
}
