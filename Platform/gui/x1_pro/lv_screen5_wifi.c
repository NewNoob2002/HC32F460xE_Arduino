#include "lvgl_screen.h"

lv_obj_t *scr5 = nullptr; /// main
lv_timer_t *scr5_timer = nullptr;

lv_obj_t *label_router_big, *label_router_mode_t, *label_router_ssid_t, *label_router_ip_t;

// unsigned char wifi_status;
// unsigned char wifi_mode;
// char wifi_IP[4];
// char wifi_SSID[16];

char wifi_ip[4] = {0, 0, 0, 0};

void lv_screen5_init(void)
{
    scr5 = lv_obj_create(NULL);
    if (RootDefaultStyle)
    {
        lv_obj_add_style(scr5, RootDefaultStyle, LV_PART_MAIN);
    }
    lv_singularxyz_router(scr5, 2, 20 + screen_destition, 30);
    lv_update_router(0);
    lv_singularxyz_router_mode_text(scr5, 2, 85 + screen_destition, 25);
    lv_update_router_mode_text(0);
    lv_singularxyz_router_ssid_text(scr5, 2, 85 + screen_destition, 50);
    lv_update_router_ssid_text(" ");
    lv_singularxyz_router_ip_text(scr5, 2, 85 + screen_destition, 75);
    lv_update_router_ip_text(wifi_ip);
}

void lv_screen5_update()
{
    lv_update_router(systemInfo.wifiInfo.wifi_status);
    lv_update_router_mode_text(systemInfo.wifiInfo.wifi_mode);
    lv_update_router_ssid_text(systemInfo.wifiInfo.wifi_ssid);
    lv_update_router_ip_text(systemInfo.wifiInfo.wifi_ip);
}

static void OnTimer(lv_timer_t *timer)
{
    lv_screen5_update();
    CORE_DEBUG_PRINTF("scr5_timer");
}

void lv_screen5_appear(void)
{
    if (scr5_timer == nullptr)
    {
        scr5_timer = lv_timer_create(OnTimer, 1000, nullptr);
        CORE_DEBUG_PRINTF("scr5_timer_create");
    }
    else
    {
        lv_timer_resume(scr5_timer);
        CORE_DEBUG_PRINTF("scr5_timer_resume");
    }
    lv_scr_load_anim(scr5, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    CORE_DEBUG_PRINTF("lv_screen5_appear");
}

void lv_screen5_disappear()
{
    if (scr5_timer)
    {
        CORE_DEBUG_PRINTF("lv_screen5_timer_stop");
        lv_timer_pause(scr5_timer);
    }
    CORE_DEBUG_PRINTF("lv_screen5_disappear");
}

void lv_singularxyz_router(lv_obj_t *obj, unsigned char type, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_router_big = lv_label_create(obj);
    lv_obj_remove_style_all(label_router_big);

    lv_obj_align(label_router_big, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_color(label_router_big, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label_router_big, LV_SYMBOL_ROUTER);
    lv_obj_set_style_text_font(label_router_big, &lv_font_singularxyz_53, 0);
}

void lv_update_router(unsigned char state)
{
    if (0 == state)
        lv_obj_set_style_text_color(label_router_big, lv_palette_main(LV_PALETTE_YELLOW), 0);
    else if (1 == state)
        lv_obj_set_style_text_color(label_router_big, lv_palette_main(LV_PALETTE_GREEN), 0);
}

void lv_singularxyz_router_mode_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_router_mode_t = lv_label_create(obj);

    lv_obj_remove_style_all(label_router_mode_t);
    lv_obj_set_style_text_font(label_router_mode_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_router_mode_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_router_mode_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);

    if (1 == state)
        lv_obj_set_style_text_color(label_router_mode_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_router_mode_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_router_mode_text(unsigned char mode)
{
    if (0 == mode)
    {
        lv_label_set_text_fmt(label_router_mode_t, "%s: AP", GET_TEXT(TEXT_WIFI_MODE));
    }
    else
    {
        lv_label_set_text_fmt(label_router_mode_t, "%s: STA", GET_TEXT(TEXT_WIFI_MODE));
    }
}

void lv_singularxyz_router_ssid_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_router_ssid_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_router_ssid_t);

    lv_obj_set_style_text_font(label_router_ssid_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_router_ssid_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_router_ssid_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);

    if (1 == state)
        lv_obj_set_style_text_color(label_router_ssid_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_router_ssid_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_router_ssid_text(const char *ssid)
{
    lv_label_set_text_fmt(label_router_ssid_t, "%s", ssid); // SSID:
}

void lv_singularxyz_router_ip_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_router_ip_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_router_ip_t);

    lv_obj_set_style_text_font(label_router_ip_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_router_ip_t, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_router_ip_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);

    if (1 == state)
        lv_obj_set_style_text_color(label_router_ip_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_router_ip_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_router_ip_text(const char *ip)
{
    lv_label_set_text_fmt(label_router_ip_t, "IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}
