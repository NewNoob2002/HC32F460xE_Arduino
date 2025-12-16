#include "DialplateView.h"
#include <cstdio>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

using namespace Page;

void DialplateView::Create(lv_obj_t* root)
{
    TopInfo_Create(root);
    BtnCont_Create(root);

    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

#define ANIM_OPA_DEF(start_time, obj) \
    ANIM_DEF(start_time, obj, opa_scale, LV_OPA_TRANSP, LV_OPA_COVER)

    const lv_coord_t y_tar_top = lv_obj_get_y(ui.topInfo.cont);
    // const lv_coord_t h_satellite_icon = lv_obj_get_height(ui.topInfo.icon_satellite);
    // const lv_coord_t h_number_cont = lv_obj_get_height(ui.topInfo.satellite_used->getCont());
    const lv_coord_t h_tar_btn = lv_obj_get_height(ui.btnCont.btnRec);

    const lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, ui.topInfo.cont, y, -lv_obj_get_height(ui.topInfo.cont), y_tar_top),

        // ANIM_DEF(200, ui.topInfo.icon_satellite, height, 0, h_satellite_icon),
        // ANIM_DEF(300, ui.topInfo.satellite_used->getCont(), height, 0, h_number_cont),

        ANIM_DEF(500, ui.btnCont.btnMap, height, 0, h_tar_btn),
        ANIM_DEF(600, ui.btnCont.btnRec, height, 0, h_tar_btn),
        ANIM_DEF(700, ui.btnCont.btnMenu, height, 0, h_tar_btn),
        LV_ANIM_TIMELINE_WRAPPER_END
    };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void DialplateView::Delete()
{
    if(ui.anim_timeline)
    {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void DialplateView::TopInfo_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, 60);

    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x333333), 0);

    lv_obj_set_style_radius(cont, 27, 0);
    lv_obj_set_y(cont, 26);
    ui.topInfo.cont = cont;


    lv_obj_t *icon_satellite = lv_label_create(cont);
    lv_obj_set_style_text_font(icon_satellite, ResourcePool::GetFont("satellite"), 0);
    lv_obj_set_style_text_color(icon_satellite, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_label_set_text(icon_satellite, CUSTOM_SYMBOL_SATELLITE);
    lv_obj_align(icon_satellite, LV_ALIGN_LEFT_MID, 20, 0);
    ui.topInfo.icon_satellite = icon_satellite;

    const lv_font_t *font_large = ResourcePool::GetFont("rajdhaniBold_40");
    const lv_font_t *font_small = ResourcePool::GetFont("rajdhaniBold_20");

    ui.topInfo.satellite_used = new numberFlow(font_large, 2);
    ui.topInfo.satellite_used->create(cont);
    ui.topInfo.satellite_used->setValue(0);
    ui.topInfo.satellite_used->setAlignTo(icon_satellite, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_obj_t *separator = lv_label_create(cont);
    lv_obj_remove_style_all(separator);
    lv_obj_set_style_text_font(separator, ResourcePool::GetFont("oswaldBold_18"), 0);
    lv_label_set_text(separator, "/");
    lv_obj_align_to(separator, ui.topInfo.satellite_used->getCont(), LV_ALIGN_OUT_RIGHT_MID, 0, 10);

    ui.topInfo.satellite_tacked =  new numberFlow(font_small, 2);
    ui.topInfo.satellite_tacked->create(cont);
    ui.topInfo.satellite_tacked->setValue(0);
    ui.topInfo.satellite_tacked->setAlignTo(separator, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_obj_t *icon_mode = lv_label_create(cont);
    lv_obj_set_style_text_font(icon_mode, ResourcePool::GetFont("dialplate"), 0);
    lv_obj_set_style_text_color(icon_mode, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_label_set_text(icon_mode, CUSTOM_SYMBOL_ROVER);
    lv_obj_align(icon_mode, LV_ALIGN_RIGHT_MID, -20, 0);
}

void DialplateView::BtnCont_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_HOR_RES, 40);
    lv_obj_align_to(cont, ui.topInfo.cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_place(
        cont,
        LV_FLEX_PLACE_SPACE_AROUND,
        LV_FLEX_PLACE_CENTER,
        LV_FLEX_PLACE_CENTER
    );*/

    ui.btnCont.cont = cont;

    ui.btnCont.btnMap = Btn_Create(cont, ResourcePool::GetImage("locate"), -80);
    ui.btnCont.btnRec = Btn_Create(cont, ResourcePool::GetImage("start"), 0);
    ui.btnCont.btnMenu = Btn_Create(cont, ResourcePool::GetImage("menu"), 80);
}

lv_obj_t* DialplateView::Btn_Create(lv_obj_t* par, const void* img_src, const lv_coord_t x_ofs)
{
    lv_obj_t* obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 40, 31);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, 0);
    lv_obj_set_style_bg_img_src(obj, img_src, 0);

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED);
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xbbbbbb), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(obj, 9, 0);

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
    lv_obj_set_style_transition(obj, &tran, LV_STATE_PRESSED);
    lv_obj_set_style_transition(obj, &tran, LV_STATE_FOCUSED);

    lv_obj_update_layout(obj);

    return obj;
}

void DialplateView::AppearAnimStart(const bool reverse) const
{
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
