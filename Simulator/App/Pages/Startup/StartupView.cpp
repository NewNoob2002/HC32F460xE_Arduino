#include "StartupView.h"
#include "../../Platform/Config/mcu_config.h"
#include "src/core/lv_obj_style.h"
#include "src/misc/lv_area.h"
#include "src/misc/lv_color.h"

using namespace Page;

#define COLOR_ORANGE lv_color_hex(0xff931e)

void StartupView::Create(lv_obj_t *root) {


  lv_obj_t *time = lv_label_create(root);
  lv_label_set_text(time, "0:00:00");
  lv_obj_set_style_text_font(time,
                             ResourcePool::GetFont("oswaldBold_18"), 0);
  lv_obj_set_style_text_color(time, lv_color_hex(0xffffff), 0);
  lv_obj_align(time, LV_ALIGN_TOP_LEFT, 10, 10);
  ui.time = time;

  lv_obj_t *img = lv_img_create(root);
  lv_img_set_src(img, ResourcePool::GetImage("battery"));
  const auto* img_ext = (lv_img_t*)img;
  lv_obj_set_size(img, img_ext->w, img_ext->h);
  lv_obj_align(img, LV_ALIGN_TOP_RIGHT, -10, 10);
  ui.battery.img = img;

  lv_obj_t* obj = lv_obj_create(img);
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

void StartupView::Delete() {}

void StartupView::Update() const{
}
