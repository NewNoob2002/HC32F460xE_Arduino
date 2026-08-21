//
// Created by gtc on 2026/5/9.
//

#ifndef LVGL_RECORDCONFIGVIEW_H
#define LVGL_RECORDCONFIGVIEW_H

#include "../Page.h"

extern const uint8_t RecordConfigType[Redcord_Type_MAX];
extern const uint8_t RecordConfigInterval[Redcord_Interval_MAX];

namespace Page {
class RecordConfigView {
  public:
    struct {
        struct {
            lv_obj_t* cont;

            struct {
                lv_obj_t* cont;
                lv_obj_t* label;

                lv_obj_t* btnCont;
                lv_obj_t* btnUp;
                lv_obj_t* btnDown;
            } left_roller;

            struct {
                lv_obj_t* cont;
                lv_obj_t* label;

                lv_obj_t* btnCont;
                lv_obj_t* btnUp;
                lv_obj_t* btnDown;
            } right_roller;
        } roller;

        struct {
            lv_obj_t* cont;
            lv_obj_t* btnRecord;
            lv_obj_t* btnReturn;
        } btnCont;

        lv_anim_timeline_t* anim_timeline;
    } ui;

    void Create(lv_obj_t* root);

    void Delete();

    void Update() const;

    void ApplyLanguage() const;

    void Roller_Create(lv_obj_t* par);

    static void Roller_Style_Init(lv_obj_t* obj);

    void BtnCont_Create(lv_obj_t* par);

    static lv_obj_t* Btn_Create(lv_obj_t* par, const void* img_src, lv_coord_t x_ofs, lv_coord_t y_ofs);

    static void Roller_toIndex(lv_obj_t* obj, uint8_t index);

    void Roller_up(lv_obj_t* obj) const;

    void Roller_down(lv_obj_t* obj) const;

    static uint8_t Roller_GetIndex(const lv_obj_t* obj);

    void AppearAnimStart(bool reverse = false) const;

  private:
    static int8_t left_roller_index;
    static int8_t right_roller_index;
};

} // namespace Page

#endif //LVGL_RECORDCONFIGVIEW_H
