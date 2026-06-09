#ifndef STAR_MAP_VIEW_H
#define STAR_MAP_VIEW_H

#include "../Page.h"

namespace Page {

class StarMapView {
public:
    struct {
        lv_obj_t* radar_bg;
        lv_obj_t* radar_inner;
        lv_obj_t* line_h;
        lv_obj_t* line_v;
        lv_obj_t* dots[16];

        lv_obj_t* info_cont;
        lv_obj_t* label_title;

        struct {
            lv_obj_t* cont;
            lv_obj_t* dot;
            lv_obj_t* label_name;
            lv_obj_t* label_val;
        } constell[4]; // GPS, BDS, GLONASS, GALILEO
    } ui;

    void Create(lv_obj_t* root);
    void Delete();
    void ApplyLanguage() const;
    void UpdateValues(int gps, int bds, int gln, int gal);
};

}

#endif // STAR_MAP_VIEW_H
