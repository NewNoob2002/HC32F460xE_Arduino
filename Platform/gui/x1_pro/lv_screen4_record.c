#include "lvgl_screen.h"

lv_obj_t *scr4 = nullptr;
lv_timer_t *scr4_timer = nullptr;

lv_obj_t *label_record_big, *label_record_rsc_t, *label_record_name_t, *label_record_type_t, *label_record_time_t;

unsigned char record_status;
float record_RSC;
char record_name[16] = "NEW";
char record_type = 1;
char record_spacetime = 1;

void lv_screen4_init(void)
{
    scr4 = lv_obj_create(NULL);
    if (RootDefaultStyle)
    {
        lv_obj_add_style(scr4, RootDefaultStyle, LV_PART_MAIN);
    }
    lv_singularxyz_record(scr4, 2, 20 + screen_destition, 30);
    lv_update_record();
    lv_singularxyz_record_rsc_text(scr4, 0, 85 + screen_destition, 20);
    lv_update_record_rsc_text();
    lv_singularxyz_record_name_text(scr4, 0, 85 + screen_destition, 45);
    lv_update_record_name_text();
    lv_singularxyz_record_type_text(scr4, 0, 85 + screen_destition, 70);
    lv_update_record_type_text();
    lv_singularxyz_record_time_text(scr4, 0, 85 + screen_destition, 95);
    lv_update_record_time_text();
}

static void OnTimer(lv_timer_t *timer)
{
    lv_screen4_update();
    CORE_DEBUG_PRINTF("Screen4_timer");
}

void lv_screen4_appear(void)
{
    if (scr4_timer == nullptr)
    {
        CORE_DEBUG_PRINTF("lv_screen4_timer_create");
        scr4_timer = lv_timer_create(OnTimer, 1000, nullptr);
        lv_timer_ready(scr4_timer);
    }
    else
    {
        CORE_DEBUG_PRINTF("lv_screen4_timer_resume");
        lv_timer_resume(scr4_timer);
    }
    lv_scr_load_anim(scr4, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    CORE_DEBUG_PRINTF("lv_screen4_appear");
}

void lv_screen4_disappear()
{
    if (scr4_timer)
    {
        CORE_DEBUG_PRINTF("lv_screen4_timer_stop");
        lv_timer_pause(scr4_timer);
    }
    CORE_DEBUG_PRINTF("lv_screen4_disappear");
}

void lv_screen4_update()
{
    lv_update_record();
    lv_update_record_rsc_text();
    lv_update_record_name_text();
    lv_update_record_type_text();
    lv_update_record_time_text();
}

void lv_singularxyz_record(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_record_big = lv_label_create(obj);
    lv_obj_remove_style_all(label_record_big);

    lv_obj_align(label_record_big, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_color(label_record_big, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label_record_big, LV_SYMBOL_RECORD);

    if (0 == type)
        lv_obj_set_style_text_font(label_record_big, &lv_font_singularxyz_28, 0);
    else if (1 == type)
        lv_obj_set_style_text_font(label_record_big, &lv_font_singularxyz_32, 0);
    else
        lv_obj_set_style_text_font(label_record_big, &lv_font_singularxyz_53, 0);
}

void lv_update_record()
{
    if (systemInfo.recordInfo.record_status)
        lv_obj_set_style_text_color(label_record_big, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_record_big, lv_palette_main(LV_PALETTE_YELLOW), 0);
}

void lv_singularxyz_record_rsc_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_record_rsc_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_record_rsc_t);

    lv_obj_align(label_record_rsc_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_record_rsc_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_record_rsc_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label_record_rsc_t, LV_SYMBOL_RECORD);

    if (1 == state)
        lv_obj_set_style_text_color(label_record_rsc_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_record_rsc_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_record_rsc_text()
{
    if (systemInfo.recordInfo.record_status)
        lv_label_set_text_fmt(label_record_rsc_t, "%s: %0.1fMB",  GET_TEXT(TEXT_RECORD_SPACE), systemInfo.recordInfo.record_leftspace);
    else
        lv_label_set_text_fmt(label_record_rsc_t, "%s: -MB", GET_TEXT(TEXT_RECORD_SPACE));
}

void lv_singularxyz_record_name_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_record_name_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_record_name_t);

    lv_obj_align(label_record_name_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_record_name_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_record_name_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label_record_name_t, LV_SYMBOL_RECORD);

    if (1 == state)
        lv_obj_set_style_text_color(label_record_name_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_record_name_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_record_name_text()
{
    if (systemInfo.recordInfo.record_status)
        lv_label_set_text_fmt(label_record_name_t, "%s: %s", GET_TEXT(TEXT_RECORD_NAME), systemInfo.recordInfo.record_name);
    else
        lv_label_set_text_fmt(label_record_name_t, "%s: -", GET_TEXT(TEXT_RECORD_NAME));
}

void lv_singularxyz_record_type_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_record_type_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_record_type_t);

    lv_obj_align(label_record_type_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_record_type_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_record_type_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label_record_type_t, LV_SYMBOL_RECORD);

    if (1 == state)
        lv_obj_set_style_text_color(label_record_type_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_record_type_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_record_type_text()
{
    // Taper
    if (systemInfo.recordInfo.record_status)
    {
        if (systemInfo.recordInfo.record_type == 1)
            lv_label_set_text_fmt(label_record_type_t, "%s: XYZ", GET_TEXT(TEXT_RECORD_TYPE));
        else if (systemInfo.recordInfo.record_type == 2)
            lv_label_set_text_fmt(label_record_type_t, "%s: Rinex 3.02", GET_TEXT(TEXT_RECORD_TYPE));
    }
    else
    {
        lv_label_set_text_fmt(label_record_type_t, "%s: -", GET_TEXT(TEXT_RECORD_TYPE));
    }
}

void lv_singularxyz_record_time_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_record_time_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_record_time_t);

    lv_obj_align(label_record_time_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_record_time_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_record_time_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label_record_time_t, LV_SYMBOL_RECORD);

    if (1 == state)
        lv_obj_set_style_text_color(label_record_time_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_record_time_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_record_time_text()
{
    // Temps
    if (systemInfo.recordInfo.record_status)
    {
        if (systemInfo.recordInfo.record_interval == 0x00)
            lv_label_set_text_fmt(label_record_time_t, "%s: 15min", GET_TEXT(TEXT_RECORD_TIME));
        else
            lv_label_set_text_fmt(label_record_time_t, "%s: %d hour", GET_TEXT(TEXT_RECORD_TIME), systemInfo.recordInfo.record_interval);
    }
    else
    {
        lv_label_set_text_fmt(label_record_time_t, "%s: -", GET_TEXT(TEXT_RECORD_TIME));
    }
}
