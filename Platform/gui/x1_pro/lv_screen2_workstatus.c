#include "lvgl_screen.h"

lv_obj_t *scr2 = nullptr; /// main
lv_timer_t *scr2_timer = nullptr;

lv_obj_t *screen2_ws, *label_radio_mode_t, *label_radio_protocol_t, *label_radio_channel_t;

lv_obj_t *screen2_ntripText, *screen2_ntrip_ipText, *screen2_ntrip_pointText;

long int screnn2_time = 0;
int last_workstatus = 0;
int workstatus = 0;
int work_status_enable = 0;

void lv_screen2_init(void)
{
    scr2 = lv_obj_create(nullptr);
    if (RootDefaultStyle)
    {
        lv_obj_add_style(scr2, RootDefaultStyle, LV_PART_MAIN);
    }
    lv_singularxyz_workstatus_text(scr2, 20 + screen_destition, 25);

    lv_singularxyz_radio_mode_text(scr2, 0, 85 + screen_destition, 22);
    lv_update_radio_mode_text(0);
    lv_singularxyz_radio_protocol_text(scr2, 0, 85 + screen_destition, 47);
    lv_update_radio_protocol_text(0);
    lv_singularxyz_radio_chanel_text(scr2, 0, 85 + screen_destition, 72);
    lv_update_radio_chanel_text(0);

    lv_singularxyz_ntrip_text(scr2, 1, 85 + screen_destition, 22);
    lv_update_ntrip_text(0);
    lv_singularxyz_ntrip_ip_text(scr2, 1, 85 + screen_destition, 47);
    lv_update_ntrip_ip_text(0);
    lv_singularxyz_ntrip_point_text(scr2, 1, 85 + screen_destition, 72);
    lv_update_ntrip_point_text(0);

    query_workstatus();
}

void lv_screen2_update()
{
    screnn2_time += 1000;
    if (workstatus_change || screnn2_time >= 5000)
    {
        screnn2_time = 0;
        workstatus_change = 0;
        query_workstatus();
    }
    lv_update_workstatus_text();
}

static void OnTimer(lv_timer_t *timer)
{
    lv_screen2_update();
    CORE_DEBUG_PRINTF("Screen2_timer");
}

void lv_screen2_appear(void)
{
    if (scr2_timer == nullptr)
    {
        CORE_DEBUG_PRINTF("lv_screen2_timer_create");
        scr2_timer = lv_timer_create(OnTimer, 1000, nullptr);
        lv_timer_ready(scr2_timer);
    }
    else
    {
        CORE_DEBUG_PRINTF("lv_screen2_timer_resume");
        lv_timer_resume(scr2_timer);
    }
    lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    CORE_DEBUG_PRINTF("lv_screen2_appear");
}

void lv_screen2_disappear()
{
    if (scr2_timer)
    {
        CORE_DEBUG_PRINTF("lv_screen2_timer_stop");
        lv_timer_pause(scr2_timer);
    }
    CORE_DEBUG_PRINTF("lv_screen2_disappear");
}

void query_workstatus()
{
    if (systemInfo.work_mode == 0) // Rover
    {
        if (systemInfo.radioInfo.radio_status == 0) // radio off
        {
            if (systemInfo.ntripInfo.gprs_status == 1 && systemInfo.ntripInfo.NtripClient_status == 1 && systemInfo.ntripInfo.NtripServer_status == 0) // Network Phone enable
            {
                workstatus = 1;
                work_status_enable = 1;
            }
            else // Network Phone disable
            {
                workstatus = 1;
                work_status_enable = 0;
            }
        }
        else if (systemInfo.radioInfo.radio_status == 1) // radio On
        {
            if (systemInfo.radioInfo.radio_mode == 0) // radio trans
            {
                workstatus = 1;
                work_status_enable = 0;
            }
            else if (systemInfo.radioInfo.radio_mode == 1) // radio receive
            {
                if (systemInfo.ntripInfo.gprs_status == 0 && systemInfo.ntripInfo.NtripClient_status == 0 && systemInfo.ntripInfo.NtripServer_status == 0) // ��̨�ƶ�վ
                {
                    workstatus = 2;
                    work_status_enable = 1;
                }
                else
                {
                    workstatus = 1;
                    work_status_enable = 0;
                }
            }
            else if (systemInfo.radioInfo.radio_mode == 2) // radio bridge
            {
                if (systemInfo.ntripInfo.gprs_status == 1 && systemInfo.ntripInfo.NtripClient_status == 1 && systemInfo.ntripInfo.NtripServer_status == 0) // RTK�Ž�
                {
                    workstatus = 5;
                    work_status_enable = 1;
                }
                else // ��Ч�����Ĭ����ʾ�����ƶ�վ
                {
                    workstatus = 1;
                    work_status_enable = 0;
                }
            }
        }
    }
    else if (systemInfo.work_mode == 1) // base
    {
        if (systemInfo.radioInfo.radio_status == 0) // radio off
        {
            if (systemInfo.ntripInfo.gprs_status == 1 && systemInfo.ntripInfo.NtripClient_status == 0 && systemInfo.ntripInfo.NtripServer_status == 1) // NtripServer On
            {
                workstatus = 3;
                work_status_enable = 1;
            }
            else // NtripServer Off
            {
                workstatus = 3;
                work_status_enable = 0;
            }
        }
        else if (systemInfo.radioInfo.radio_status == 1) // Radio On
        {
            if (systemInfo.radioInfo.radio_mode == 0) // Radio tans
            {
                if (systemInfo.ntripInfo.gprs_status == 0 && systemInfo.ntripInfo.NtripClient_status == 0 && systemInfo.ntripInfo.NtripServer_status == 0) // Radio Base On
                {
                    workstatus = 4;
                    work_status_enable = 1;
                }
            }
            else // Radio Base Off
            {
                workstatus = 3;
                work_status_enable = 0;
            }
        }
    }
    else if (systemInfo.work_mode == 2) // Single
    {
        workstatus = 1;
        work_status_enable = 0;
    }
    else if (systemInfo.work_mode == 3) // autoBase
    {
        workstatus = 1;
        work_status_enable = 0;
    }
}

void change_workstatus()
{
    if (workstatus == 1) // �����ƶ�վ
    {
        systemInfo.work_mode = 0;
        systemInfo.radioInfo.radio_status = 0;
        systemInfo.ntripInfo.gprs_status = 1;
        systemInfo.ntripInfo.NtripClient_status = 1;
        systemInfo.ntripInfo.NtripServer_status = 0;
    }

    else if (workstatus == 2) // ��̨�ƶ�վ
    {
        systemInfo.work_mode = 0;
        systemInfo.radioInfo.radio_status = 1;
        systemInfo.radioInfo.radio_mode = 1;
        systemInfo.ntripInfo.gprs_status = 0;
        systemInfo.ntripInfo.NtripClient_status = 0;
        systemInfo.ntripInfo.NtripServer_status = 0;
    }

    else if (workstatus == 3) // �����վ
    {
        systemInfo.work_mode = 1;
        systemInfo.radioInfo.radio_status = 0;
        systemInfo.ntripInfo.gprs_status = 1;
        systemInfo.ntripInfo.NtripClient_status = 0;
        systemInfo.ntripInfo.NtripServer_status = 1;
    }

    else if (workstatus == 4) // ��̨��վ
    {
        systemInfo.work_mode = 1;
        systemInfo.radioInfo.radio_status = 1;
        systemInfo.radioInfo.radio_mode = 0;
        systemInfo.ntripInfo.gprs_status = 0;
        systemInfo.ntripInfo.NtripClient_status = 0;
        systemInfo.ntripInfo.NtripServer_status = 0;
    }

    else if (workstatus == 5) // RTK�Ž�
    {
        systemInfo.work_mode = 0;
        systemInfo.radioInfo.radio_status = 1;
        systemInfo.radioInfo.radio_mode = 2;
        systemInfo.ntripInfo.gprs_status = 1;
        systemInfo.ntripInfo.NtripClient_status = 1;
        systemInfo.ntripInfo.NtripServer_status = 0;
    }
}

void lv_singularxyz_workstatus_text(lv_obj_t *obj, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    screen2_ws = lv_label_create(obj);
    lv_obj_remove_style_all(screen2_ws);
    lv_obj_align(screen2_ws, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_text_font(screen2_ws, &lv_workstatus_icon, 0);
}

void lv_update_workstatus_text()
{
    // query_workstatus();
    if (workstatus == 1) // �����ƶ�վ
    {
        lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);
        lv_label_set_text(screen2_ws, LV_SYMBOL_workstatus1); // �����ƶ�վ
        update_screen2_radio(0);
        update_screen2_ntrip(1);
    }

    else if (workstatus == 2) // ��̨�ƶ�վ
    {
        lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);
        lv_label_set_text(screen2_ws, LV_SYMBOL_workstatus2); // ��̨�ƶ�վ
        update_screen2_radio(1);
        update_screen2_ntrip(0);
    }

    else if (workstatus == 3) // �����վ
    {
        lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_label_set_text(screen2_ws, LV_SYMBOL_workstatus4); // �����վ
        update_screen2_radio(0);
        update_screen2_ntrip(1);
    }

    else if (workstatus == 4) // ��̨��վ
    {
        lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_label_set_text(screen2_ws, LV_SYMBOL_workstatus5); // ��̨��վ
        update_screen2_radio(1);
        update_screen2_ntrip(0);
    }

    else if (workstatus == 5) // RTK�Ž�
    {
        lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_label_set_text(screen2_ws, LV_SYMBOL_workstatus3); // RTK�Ž�
        update_screen2_radio(1);
        update_screen2_ntrip(0);
    }
    else
    {
        lv_obj_set_style_text_color(screen2_ws, lv_palette_main(LV_PALETTE_LIGHT_GREEN), 0);
        lv_label_set_text(screen2_ws, LV_SYMBOL_workstatus1); // �����ƶ�վ
        update_screen2_radio(0);
        update_screen2_ntrip(1);
    }
}

void update_screen2_radio(int show_flag)
{
    lv_update_radio_mode_text(show_flag);
    lv_update_radio_protocol_text(show_flag);
    lv_update_radio_chanel_text(show_flag);
}

void lv_singularxyz_radio_mode_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_radio_mode_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_radio_mode_t);

    lv_obj_align(label_radio_mode_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_radio_mode_t, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(label_radio_mode_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_radio_mode_text(int mode_flag)
{
    if (work_status_enable)
        lv_obj_set_style_text_color(label_radio_mode_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_radio_mode_t, lv_palette_main(LV_PALETTE_GREY), 0);

    if (mode_flag)
    {
        if (workstatus == 2)
            lv_label_set_text(label_radio_mode_t, GET_TEXT(TEXT_MODE_RADIO_RECEIVE));
        else if (workstatus == 4)
            lv_label_set_text(label_radio_mode_t, GET_TEXT(TEXT_MODE_RADIO_TRANSMIT));
        else if (workstatus == 5)
            lv_label_set_text(label_radio_mode_t, GET_TEXT(TEXT_MODE_RADIO_BRIDGE));
    }
    else
        lv_label_set_text(label_radio_mode_t, "");
}

void lv_singularxyz_radio_protocol_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_radio_protocol_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_radio_protocol_t);

    lv_obj_align(label_radio_protocol_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_radio_protocol_t, &font_oswaldBold_18, 0);

    if (1 == state)
        lv_obj_set_style_text_color(label_radio_protocol_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_radio_protocol_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_radio_protocol_text(int protocol_flag)
{
    if (work_status_enable)
        lv_obj_set_style_text_color(label_radio_protocol_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_radio_protocol_t, lv_palette_main(LV_PALETTE_GREY), 0);

    if (protocol_flag)
    {
        if (systemInfo.radioInfo.radio_protocol == 1)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:TRIMTALK", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 2)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:TRIMMK3", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 4)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:TT450S", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 5)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:TRANSEOT", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 9)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:SOUTH", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 10)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:HUACE", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 13)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:SATEL", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else if (systemInfo.radioInfo.radio_protocol == 16)
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:CCS", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
        else
            lv_label_set_text_fmt(label_radio_protocol_t, "%s:", GET_TEXT(TEXT_MODE_RADIO_PROTOCOL));
    }
    else
        lv_label_set_text_fmt(label_radio_protocol_t, "");
}

void lv_singularxyz_radio_chanel_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    label_radio_channel_t = lv_label_create(obj);
    lv_obj_remove_style_all(label_radio_channel_t);

    lv_obj_align(label_radio_channel_t, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_radio_channel_t, &font_oswaldBold_18, 0);

    lv_obj_set_style_text_color(label_radio_channel_t, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_radio_chanel_text(int chanel_flag)
{
    if (work_status_enable)
        lv_obj_set_style_text_color(label_radio_channel_t, lv_palette_main(LV_PALETTE_GREEN), 0);
    else
        lv_obj_set_style_text_color(label_radio_channel_t, lv_palette_main(LV_PALETTE_GREY), 0);

    if (chanel_flag)
    {
        lv_label_set_text_fmt(label_radio_channel_t, "%s: %d", GET_TEXT(TEXT_MODE_RADIO_CHANNEL), systemInfo.radioInfo.radio_channel);
    }
    else
    {
        lv_label_set_text_fmt(label_radio_channel_t, "");
    }
}

void update_screen2_ntrip(int show_flag)
{
    lv_update_ntrip_text(show_flag);
    lv_update_ntrip_ip_text(show_flag);
    lv_update_ntrip_point_text(show_flag);
}

void lv_singularxyz_ntrip_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    screen2_ntripText = lv_label_create(obj);
    lv_obj_remove_style_all(screen2_ntripText);

    lv_obj_align(screen2_ntripText, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(screen2_ntripText, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(screen2_ntripText, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_ntrip_text(int show_flag)
{
    if (show_flag)
    {
        if (work_status_enable)
            lv_obj_set_style_text_color(screen2_ntripText, lv_palette_main(LV_PALETTE_GREEN), 0);
        else
            lv_obj_set_style_text_color(screen2_ntripText, lv_palette_main(LV_PALETTE_GREY), 0);

        if (workstatus == 1)
        {
            lv_label_set_text_fmt(screen2_ntripText, GET_TEXT(TEXT_MODE_NTRIP_CLIENT));
        }
        else if (workstatus == 3)
        {
            lv_label_set_text_fmt(screen2_ntripText, GET_TEXT(TEXT_MODE_NTRIP_SERVER));
        }
    }
    else
    {
        lv_label_set_text_fmt(screen2_ntripText, "");
    }
}

void lv_singularxyz_ntrip_ip_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    screen2_ntrip_ipText = lv_label_create(obj);
    lv_obj_remove_style_all(screen2_ntrip_ipText);

    lv_obj_align(screen2_ntrip_ipText, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(screen2_ntrip_ipText, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(screen2_ntrip_ipText, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_ntrip_ip_text(int show_flag)
{
    if (show_flag)
    {
        if (work_status_enable)
            lv_obj_set_style_text_color(screen2_ntrip_ipText, lv_palette_main(LV_PALETTE_GREEN), 0);
        else
            lv_obj_set_style_text_color(screen2_ntrip_ipText, lv_palette_main(LV_PALETTE_GREY), 0);

        if (workstatus == 1)
            lv_label_set_text_fmt(screen2_ntrip_ipText, "IP: %d.%d.%d.%d", systemInfo.ntripInfo.NtripClient_IP[0], systemInfo.ntripInfo.NtripClient_IP[1], systemInfo.ntripInfo.NtripClient_IP[2], systemInfo.ntripInfo.NtripClient_IP[3]);
        else if (workstatus == 3)
            lv_label_set_text_fmt(screen2_ntrip_ipText, "IP: %d.%d.%d.%d", systemInfo.ntripInfo.NtripServer_IP[0], systemInfo.ntripInfo.NtripServer_IP[1], systemInfo.ntripInfo.NtripServer_IP[2], systemInfo.ntripInfo.NtripServer_IP[3]);
    }
    else
    {
        lv_label_set_text_fmt(screen2_ntrip_ipText, "");
    }
}

void lv_singularxyz_ntrip_point_text(lv_obj_t *obj, unsigned char state, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    screen2_ntrip_pointText = lv_label_create(obj);
    lv_obj_remove_style_all(screen2_ntrip_pointText);

    lv_obj_align(screen2_ntrip_pointText, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_text_font(screen2_ntrip_pointText, &font_oswaldBold_18, 0);
    lv_obj_set_style_text_color(screen2_ntrip_pointText, lv_palette_main(LV_PALETTE_GREY), 0);
}

void lv_update_ntrip_point_text(int show_flag)
{
    if (show_flag)
    {
        if (work_status_enable)
            lv_obj_set_style_text_color(screen2_ntrip_pointText, lv_palette_main(LV_PALETTE_GREEN), 0);
        else
            lv_obj_set_style_text_color(screen2_ntrip_pointText, lv_palette_main(LV_PALETTE_GREY), 0);

        if (workstatus == 1)
            lv_label_set_text_fmt(screen2_ntrip_pointText, "MP: %s", systemInfo.ntripInfo.NtripClient_Mountpoint);
        else if (workstatus == 3)
            lv_label_set_text_fmt(screen2_ntrip_pointText, "MP: %s", systemInfo.ntripInfo.NtripServer_Mountpoint);
    }
    else
    {
        lv_label_set_text_fmt(screen2_ntrip_pointText, "");
    }
}
