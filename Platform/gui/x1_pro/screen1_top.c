#include "lvgl_screen.h"

lv_obj_t *screen1_top = NULL;

/**********************����״̬�����*****************************/
lv_obj_t *screen1_battery, *screen1_wifi, *screen1_bt, *screen1_vol, *screen1_gps, *screen1_sd, *screen1_radio, *screen1_4g;

/*****************************************************************/
/************  ����״̬������   ************/
/*****************************************************************/
void lv_screen1_top_init(void)
{
	screen1_top = lv_label_create(scr1); // lv_label_create
	lv_label_set_text(screen1_top, "");

	lv_obj_set_pos(screen1_top, 0, 0);
	lv_obj_align(screen1_top, LV_ALIGN_TOP_MID, 0, 0);
	lv_obj_set_width(screen1_top, lv_pct(100));
	lv_obj_set_height(screen1_top, 35);

	lv_singularxyz_4g(screen1_top, 10 + screen_destition, 10);
	lv_update_4g();
	lv_singularxyz_radio(screen1_top, 58 + screen_destition, 10);
	lv_update_radio();
	lv_singularxyz_sd(screen1_top, 106 + screen_destition, 10);
	lv_update_sd();
	lv_singularxyz_gps(screen1_top, 154 + screen_destition, 10);
	lv_update_gps();
	lv_singularxyz_wifi(screen1_top, 202 + screen_destition, 10);
	lv_update_wifi();
	lv_singularxyz_battery(screen1_top, 255 + screen_destition, 10);
	lv_update_battery(0);
}

void lv_singularxyz_4g(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	screen1_4g = lv_label_create(obj);
	lv_obj_remove_style_all(screen1_4g);
	lv_label_set_text(screen1_4g, LV_SYMBOL_LTE);
	lv_obj_align(screen1_4g, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
	lv_obj_set_style_text_font(screen1_4g, &lv_font_singularxyz_22, 0);
}

void lv_update_4g()
{
	if (systemInfo.ntripInfo.gprs_status == 1)
		lv_obj_set_style_text_color(screen1_4g, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
	else
		lv_obj_set_style_text_color(screen1_4g, lv_palette_main(LV_PALETTE_RED), 0);
}

void lv_singularxyz_radio(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	screen1_radio = lv_label_create(obj);
	lv_obj_remove_style_all(screen1_radio);
	lv_label_set_text(screen1_radio, LV_SYMBOL_RADIO);
	lv_obj_align(screen1_radio, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
	lv_obj_set_style_text_font(screen1_radio, &lv_font_singularxyz_22, 0);
}

void lv_update_radio()
{
	if (systemInfo.radioInfo.radio_status == 1)
	{
		lv_obj_set_style_text_color(screen1_radio, lv_palette_main(LV_PALETTE_ORANGE), 0);
	}
	else
	{
		lv_obj_set_style_text_color(screen1_radio, lv_palette_main(LV_PALETTE_RED), 0);
	}
}

void lv_singularxyz_sd(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	screen1_sd = lv_label_create(obj);
	lv_obj_remove_style_all(screen1_sd);
	lv_label_set_text(screen1_sd, LV_SYMBOL_SD_CARD);
	lv_obj_align(screen1_sd, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
}

void lv_update_sd()
{
	if (systemInfo.recordInfo.record_status == 1)
	{
		lv_obj_set_style_text_color(screen1_sd, lv_palette_main(LV_PALETTE_PURPLE), 0);
	}
	else
	{
		lv_obj_set_style_text_color(screen1_sd, lv_palette_main(LV_PALETTE_RED), 0);
	}
}

void lv_singularxyz_gps(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	screen1_gps = lv_label_create(obj);
	lv_obj_remove_style_all(screen1_gps);
	lv_label_set_text(screen1_gps, LV_SYMBOL_GPS);
	lv_obj_align(screen1_gps, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
}

void lv_update_gps()
{
	unsigned char type = systemInfo.positionInfo.coordinate_status;
	switch (type)
	{
	case 0:
		lv_obj_set_style_text_color(screen1_gps, lv_palette_main(LV_PALETTE_RED), 0);
		break;
	case 1:
		lv_obj_set_style_text_color(screen1_gps, lv_palette_main(LV_PALETTE_YELLOW), 0);
		break;
	case 2:
		lv_obj_set_style_text_color(screen1_gps, lv_palette_main(LV_PALETTE_GREEN), 0);
		break;
	case 3:
		lv_obj_set_style_text_color(screen1_gps, lv_palette_main(LV_PALETTE_YELLOW), 0);
		break;
	case 4:
		lv_obj_set_style_text_color(screen1_gps, lv_palette_main(LV_PALETTE_BLUE), 0);
		break;
	}
}

void lv_singularxyz_wifi(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	screen1_wifi = lv_label_create(obj);
	lv_obj_remove_style_all(screen1_wifi);
	lv_label_set_text(screen1_wifi, LV_SYMBOL_WIFI);
	lv_obj_align(screen1_wifi, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
}

void lv_update_wifi()
{
	if (systemInfo.wifiInfo.wifi_status == 1)
	{
		lv_obj_set_style_text_color(screen1_wifi, lv_palette_main(LV_PALETTE_TEAL), 0);
	}
	else
	{
		lv_obj_set_style_text_color(screen1_wifi, lv_palette_main(LV_PALETTE_RED), 0);
	}
}

void lv_singularxyz_battery(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
	screen1_battery = lv_label_create(obj);
	lv_obj_remove_style_all(screen1_battery);
	lv_label_set_recolor(screen1_battery, true);
	lv_obj_align(screen1_battery, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
}

void lv_update_battery(unsigned int value)
{
	if (value == 100)
	{
		lv_label_set_text(screen1_battery, LV_SYMBOL_BATTERY_FULL);
		lv_obj_set_style_text_color(screen1_battery, lv_palette_main(LV_PALETTE_GREEN), 0);
	}
	else if ((value < 100) && (value >= 70))
	{
		lv_label_set_text(screen1_battery, LV_SYMBOL_BATTERY_3);
		lv_obj_set_style_text_color(screen1_battery, lv_palette_main(LV_PALETTE_GREEN), 0);
	}
	else if ((value < 70) && (value >= 40))
	{
		lv_label_set_text(screen1_battery, LV_SYMBOL_BATTERY_2);
		lv_obj_set_style_text_color(screen1_battery, lv_palette_main(LV_PALETTE_YELLOW), 0);
	}
	else if ((value < 40) && (value >= 10))
	{
		lv_label_set_text(screen1_battery, LV_SYMBOL_BATTERY_1);
		lv_obj_set_style_text_color(screen1_battery, lv_palette_main(LV_PALETTE_RED), 0);
	}
	else
	{
		lv_label_set_text(screen1_battery, LV_SYMBOL_BATTERY_EMPTY);
		lv_obj_set_style_text_color(screen1_battery, lv_palette_main(LV_PALETTE_RED), 0);
	}
}

void lv_singularxyz_update_top_status()
{
	lv_update_4g();
	lv_update_radio();
	lv_update_sd();
	lv_update_gps();
	lv_update_wifi();
}
