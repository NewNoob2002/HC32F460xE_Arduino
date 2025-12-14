#include "StartupView.h"
#include <cstdio>
#include "../../Platform/Config/mcu_config.h"

using namespace Page;

#define COLOR_ORANGE lv_color_hex(0xff931e)

void StartupView::Create(lv_obj_t *root) {
    lv_font_t *font = ResourcePool::GetFont("oswaldBold_18");
    uint16_t font_width = lv_font_get_glyph_width(font, '0', '\0');
    printf("Height:%d, Width:%d\n", font->line_height, font_width);
    lv_obj_t *cont = lv_obj_create(root);
    lv_obj_remove_style_all(cont);
    lv_obj_center(cont);
    lv_obj_set_size(cont, font_width, font->line_height);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_OVERFLOW_VISIBLE); // 确保裁剪子对象

    lv_obj_t *time = lv_label_create(cont);
    lv_label_set_text(time, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9");
    lv_obj_set_style_text_font(time,
                               ResourcePool::GetFont("oswaldBold_18"), 0);
    lv_obj_set_style_text_color(time, lv_color_hex(0xffffff), 0);
    lv_obj_set_align(time, LV_ALIGN_TOP_MID);
    ui.time = time;

    lv_obj_t *img = lv_img_create(root);
    lv_img_set_src(img, ResourcePool::GetImage("battery"));
    const auto *img_ext = reinterpret_cast<lv_img_t *>(img);
    lv_obj_set_size(img, img_ext->w, img_ext->h);
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, -10, 10);
    ui.battery.img = img;

    lv_obj_t *obj = lv_obj_create(img);
    lv_obj_remove_style_all(obj);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x4CAF50), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_size(obj, 16, 8);
    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, -4);
    ui.battery.objUsage = obj;
    // Firmware version label
    lv_obj_t *label_firmware = lv_label_create(root);
    lv_label_set_text_fmt(label_firmware, "Rev:%s", SOFTWARE_VERSION);
    lv_obj_set_style_text_font(label_firmware,
                               ResourcePool::GetFont("oswaldBold_12"), 0);
    lv_obj_set_style_text_color(label_firmware, lv_palette_main(LV_PALETTE_GREY),
                                0);
    lv_obj_align(label_firmware, LV_ALIGN_BOTTOM_LEFT, 10, 0);

    // Compile time label
    lv_obj_t *label_compile_time = lv_label_create(root);
    lv_label_set_text(label_compile_time, __DATE__ " " __TIME__);
    lv_obj_set_style_text_font(label_compile_time,
                               ResourcePool::GetFont("oswaldBold_12"), 0);
    lv_obj_set_style_text_color(label_compile_time,
                                lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_compile_time, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
}

void StartupView::Delete() {
}

void StartupView::Update() const {
    static int i = 0;
    roll_to(ui.time, i++);
    if (i >= 10)i = 0;
}
