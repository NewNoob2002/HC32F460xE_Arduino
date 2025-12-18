#include "WorkSettingsView.h"
#include <cmath>

using namespace Page;

static void lv_anim_label_set_y(void *obj, const int32_t y) {
    lv_obj_set_y(static_cast<lv_obj_t *>(obj), y);
}

void WorkSettingsView::Create(lv_obj_t *root) {
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
    const lv_coord_t w_tar_btn = lv_obj_get_width(ui.btnCont.btnBase);

    const lv_anim_timeline_wrapper_t wrapper[] =
    {
        ANIM_DEF(0, ui.roller.cont, x, -lv_obj_get_width(ui.roller.cont), x_tar_top),

        ANIM_DEF(100, ui.roller.left_roller.btnUp, width, 0, w_up_btn),
        ANIM_DEF(200, ui.roller.left_roller.btnDown, width, 0, w_up_btn),
        ANIM_DEF(300, ui.roller.mid_roller.btnUp, width, 0, w_up_btn),
        ANIM_DEF(400, ui.roller.mid_roller.btnDown, width, 0, w_up_btn),
        ANIM_DEF(500, ui.roller.btnReset, width, 0, w_up_btn),

        ANIM_DEF(500, ui.btnCont.btnBase, width, 0, w_tar_btn),
        ANIM_DEF(600, ui.btnCont.btnRover, width, 0, w_tar_btn),
        ANIM_DEF(700, ui.btnCont.btnNtrip, width, 0, w_tar_btn),
        LV_ANIM_TIMELINE_WRAPPER_END
    };
    lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void WorkSettingsView::Delete() {
    if (ui.anim_timeline) {
        lv_anim_timeline_del(ui.anim_timeline);
        ui.anim_timeline = nullptr;
    }
}

void WorkSettingsView::Roller_Create(lv_obj_t *par) {
    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);
    // lv_obj_set_style_border_color(cont, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont, 1, 0);
    lv_obj_set_size(cont, 230, 90);
    lv_obj_set_align(cont, LV_ALIGN_LEFT_MID);
    ui.roller.cont = cont;

    lv_obj_t *cont_left = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_left);
    lv_obj_set_style_border_color(cont_left, lv_color_white(), 0);
    lv_obj_set_style_border_width(cont_left, 1, 0);
    lv_obj_set_size(cont_left, 90, 30);
    lv_obj_align(cont_left, LV_ALIGN_LEFT_MID, 10, -20);
    ui.roller.left_roller.cont = cont_left;

    const lv_font_t *font = ResourcePool::GetFont("oswaldBold_18");

    lv_obj_t *label_left = lv_label_create(cont_left);
    lv_obj_set_style_text_font(label_left, font, 0);
    lv_label_set_text(label_left, "TRIMTALK\n"
                                  "TRIMMK3\n"
                                  "TT450S\n"
                                  "TRANSEOT\n"
                                  "SOUCH\n"
                                  "HUACE\n"
                                  "SATAL\n"
                                  "CSS");
    lv_obj_set_align(label_left, LV_ALIGN_TOP_MID);
    ui.roller.left_roller.label = label_left;

    lv_obj_t *cont_right = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_right);
    lv_obj_set_style_border_color(cont_right, lv_color_white(), 0);
    lv_obj_set_style_border_width(cont_right, 1, 0);
    lv_obj_set_size(cont_right, 90, 30);
    lv_obj_align_to(cont_right, cont_left, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    ui.roller.mid_roller.cont = cont_right;

    lv_obj_t *label_right = lv_label_create(cont_right);
    lv_obj_set_style_text_font(label_right, font, 0);
    lv_label_set_text(label_right, "[1]455.05\n"
                      "[2]456.05\n"
                      "[3]457.05\n"
                      "[4]458.05\n"
                      "[5]459.05\n"
                      "[6]460.05\n"
                      "[7]461.05\n"
                      "[8]462.05\n"
                      "[9]463.05");
    lv_obj_set_align(label_right, LV_ALIGN_TOP_MID);
    ui.roller.mid_roller.label = label_right;

    lv_obj_t *cont_upDown1 = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_upDown1);
    // lv_obj_set_style_border_color(cont_upDown1, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont_upDown1, 1, 0);
    lv_obj_set_size(cont_upDown1, 90, 40);
    lv_obj_align_to(cont_upDown1, cont_left, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    ui.roller.left_roller.btnUp = Btn_Create(cont_upDown1, ResourcePool::GetImage("up"), -20, 0);
    ui.roller.left_roller.btnDown = Btn_Create(cont_upDown1, ResourcePool::GetImage("down"), 20, 0);

    lv_obj_t *cont_upDown2 = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_upDown2);
    // lv_obj_set_style_border_color(cont_upDown2, lv_color_white(), 0);
    // lv_obj_set_style_border_width(cont_upDown2, 1, 0);
    lv_obj_set_size(cont_upDown2, 120, 40);
    lv_obj_align_to(cont_upDown2, cont_upDown1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ui.roller.mid_roller.btnUp = Btn_Create(cont_upDown2, ResourcePool::GetImage("up"), -40, 0);
    ui.roller.mid_roller.btnDown = Btn_Create(cont_upDown2, ResourcePool::GetImage("down"), 0, 0);

    ui.roller.btnReset = Btn_Create(cont_upDown2, ResourcePool::GetImage("reset"), 40, 0);
}

void WorkSettingsView::Roller_Style_Init(lv_obj_t *obj) {
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

void WorkSettingsView::BtnCont_Create(lv_obj_t *par) {
    lv_obj_t *cont = lv_obj_create(par);
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

    ui.btnCont.btnBase = Btn_Create(cont, ResourcePool::GetImage("base"), 0, -33);
    ui.btnCont.btnRover = Btn_Create(cont, ResourcePool::GetImage("rover"), 0, 0);
    ui.btnCont.btnNtrip = Btn_Create(cont, ResourcePool::GetImage("ntrip"), 0, 33);
}

lv_obj_t *WorkSettingsView::Btn_Create(lv_obj_t *par, const void *img_src, const lv_coord_t x_ofs,
                                       const lv_coord_t y_ofs) {
    lv_obj_t *obj = lv_obj_create(par);
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

void WorkSettingsView::Roller_UpDown(lv_obj_t *obj, const bool down) {
    constexpr lv_coord_t font_height = 26;

    // 先删除该 label 上可能存在的旧动画，避免冲突
    lv_anim_del(obj, lv_anim_label_set_y);

    const lv_coord_t current_y = lv_obj_get_y(obj);
    const lv_coord_t max_y = lv_obj_get_height(obj);

    lv_coord_t target_y = 0;

    if (!down) {
        target_y = current_y + font_height;
        if (target_y > 0) {
            target_y = -max_y + font_height;
        }
    } else {
        target_y = current_y - font_height;
        if (target_y <= -max_y) {
            target_y = 0;
        }
    }
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, current_y, target_y);
    lv_anim_set_time(&a, 300);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&a, lv_anim_label_set_y);
    lv_anim_start(&a);
}

uint8_t WorkSettingsView::Roller_GetIndex(const lv_obj_t *obj) {
    const lv_coord_t current_y = lv_obj_get_y(obj);
    return abs(current_y) / 26;
}

void WorkSettingsView::AppearAnimStart(const bool reverse) const {
    lv_anim_timeline_set_reverse(ui.anim_timeline, reverse);
    lv_anim_timeline_start(ui.anim_timeline);
}
