//
// Created by gtc on 2026/5/9.
//

#include "RecordConfigView.h"

using namespace Page;

constexpr lv_coord_t font_height = 26;

int8_t RecordConfigView::left_roller_index = 0;
int8_t RecordConfigView::mid_roller_index = 0;

static void
lv_anim_label_set_y(void* obj, const int32_t y) {
    lv_obj_set_y(static_cast<lv_obj_t*>(obj), y);
}

void
RecordConfigView::Create(lv_obj_t* root) {
    lv_obj_set_size(root, 294, 100);
    lv_obj_set_align(root, LV_ALIGN_BOTTOM_MID);
    Roller_Create(root);
    BtnCont_Create(root);
    ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
{start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

#define ANIM_OPA_DEF(start_time, obj) \
ANIM_DEF(start_time, obj, opa_scale, LV_OPA_TRANSP, LV_OPA_COVER)

    const lv_coord_t x_tar_top = lv_obj_get_x(ui.roller.cont);
    const lv_coord_t w_up_btn = lv_obj_get_width(ui.roller.left_roller.btnUp);
    const lv_coord_t w_tar_btn = lv_obj_get_width(ui.btnCont.btnRecord);

    const lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, ui.roller.cont, x, -lv_obj_get_width(ui.roller.cont), x_tar_top),

        ANIM_DEF(100, ui.roller.left_roller.btnUp, width, 0, w_up_btn),
        ANIM_DEF(200, ui.roller.left_roller.btnDown, width, 0, w_up_btn),
        ANIM_DEF(300, ui.roller.right_roller.btnUp, width, 0, w_up_btn),
        ANIM_DEF(400, ui.roller.right_roller.btnDown, width, 0, w_up_btn),

        ANIM_DEF(500, ui.btnCont.btnRecord, width, 0, w_tar_btn),
        ANIM_DEF(600, ui.btnCont.btnReturn, width, 0, w_tar_btn),
        LV_ANIM_TIMELINE_WRAPPER_END
    };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void
RecordConfigView::Delete() {
    if (ui.anim_timeline) {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void
RecordConfigView::Roller_Create(lv_obj_t* par) {
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    // lv_obj_set_style_border_color(cont, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont, 1, 0);
    lv_obj_set_size(cont, 230, 90);
    lv_obj_set_align(cont, LV_ALIGN_LEFT_MID);
    ui.roller.cont = cont;

    lv_obj_t* cont_left = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_left);
    lv_obj_set_style_border_color(cont_left, lv_color_white(), 0);
    lv_obj_set_style_border_width(cont_left, 1, 0);
    lv_obj_set_size(cont_left, 90, 30);
    lv_obj_align(cont_left, LV_ALIGN_LEFT_MID, 30, -20);
    ui.roller.left_roller.cont = cont_left;

    const lv_font_t* font = ResourcePool::GetFont("oswaldBold_18");

    lv_obj_t* label_left = lv_label_create(cont_left);
    lv_obj_set_style_text_font(label_left, font, 0);
    lv_label_set_text(label_left,
                      "TRIMTALK\n"
                      "TRIMMK3\n"
                      "TT450S\n"
                      "TRANSEOT\n"
                      "SOUTH\n"
                      "HUACE\n"
                      "SATEL\n"
                      "CSS");
    lv_obj_set_align(label_left, LV_ALIGN_TOP_MID);
    ui.roller.left_roller.label = label_left;

    lv_obj_t* cont_right = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_right);
    lv_obj_set_style_border_color(cont_right, lv_color_white(), 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);
    lv_obj_set_size(cont_right, 90, 30);
    lv_obj_align_to(cont_right, cont_left, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    ui.roller.right_roller.cont = cont_right;

    lv_obj_t* label_right = lv_label_create(cont_right);
    lv_obj_set_style_text_font(label_right, font, 0);
    lv_label_set_text(label_right,
                      "CUSTOM\n"
                      "[1]455.05\n"
                      "[2]456.05\n"
                      "[3]457.05\n"
                      "[4]458.05\n"
                      "[5]459.05\n"
                      "[6]460.05\n"
                      "[7]461.05\n"
                      "[8]462.05\n"
                      "[9]463.05");
    lv_obj_set_align(label_right, LV_ALIGN_TOP_MID);
    ui.roller.right_roller.label = label_right;

    lv_obj_t* cont_upDown1 = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_upDown1);
    // lv_obj_set_style_border_color(cont_upDown1, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont_upDown1, 1, 0);
    lv_obj_set_size(cont_upDown1, 90, 40);
    lv_obj_align_to(cont_upDown1, cont_left, LV_ALIGN_OUT_BOTTOM_MID, -20, 0);

    ui.roller.left_roller.btnUp = Btn_Create(cont_upDown1, ResourcePool::GetImage("up"), -20, 0);
    ui.roller.left_roller.btnDown = Btn_Create(cont_upDown1, ResourcePool::GetImage("down"), 20, 0);

    lv_obj_t* cont_upDown2 = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_upDown2);
    // lv_obj_set_style_border_color(cont_upDown2, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont_upDown2, 1, 0);
    lv_obj_set_size(cont_upDown2, 120, 40);
    lv_obj_align_to(cont_upDown2, cont_upDown1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ui.roller.right_roller.btnUp = Btn_Create(cont_upDown2, ResourcePool::GetImage("up"), -40, 0);
    ui.roller.right_roller.btnDown = Btn_Create(cont_upDown2, ResourcePool::GetImage("down"), 0, 0);
}

void
RecordConfigView::Roller_Style_Init(lv_obj_t* obj) {
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED);
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xbbbbbb), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(obj, 9, 0);

    static lv_style_transition_dsc_t tran;
    static constexpr lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
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
}

void
RecordConfigView::BtnCont_Create(lv_obj_t* par) {
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 50, 99);
    lv_obj_align(cont, LV_ALIGN_RIGHT_MID, -10, 0);
    // lv_obj_set_style_border_color(cont, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont, 1, 0);
    /*lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_place(
        cont,
        LV_FLEX_PLACE_SPACE_AROUND,
        LV_FLEX_PLACE_CENTER,
        LV_FLEX_PLACE_CENTER
    );*/

    ui.btnCont.cont = cont;

    ui.btnCont.btnRecord = Btn_Create(cont, ResourcePool::GetImage("start"), 0, -33);
    ui.btnCont.btnReturn = Btn_Create(cont, ResourcePool::GetImage("reset"), 0, 0);
}

lv_obj_t*
RecordConfigView::Btn_Create(lv_obj_t* par, const void* img_src, const lv_coord_t x_ofs,
                             const lv_coord_t y_ofs) {
    lv_obj_t* obj = lv_obj_create(par);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 35, 26);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_align(obj, LV_ALIGN_CENTER, x_ofs, y_ofs);
    lv_obj_set_style_bg_img_src(obj, img_src, 0);

    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_width(obj, 45, LV_STATE_PRESSED);
    lv_obj_set_style_height(obj, 25, LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x666666), 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xbbbbbb), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff931e), LV_STATE_FOCUSED);
    lv_obj_set_style_radius(obj, 9, 0);

    static lv_style_transition_dsc_t tran;
    static constexpr lv_style_prop_t prop[] = {LV_STYLE_WIDTH, LV_STYLE_HEIGHT, LV_STYLE_PROP_INV};
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

void
RecordConfigView::AppearAnimStart(const bool reverse) const {
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
