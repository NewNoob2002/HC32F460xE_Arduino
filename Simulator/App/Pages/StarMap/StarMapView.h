#ifndef STAR_MAP_VIEW_H
#define STAR_MAP_VIEW_H

#include "../Page.h"

namespace Page {

class StarMapView {
  public:
    struct {
        lv_obj_t* label_title;
        lv_obj_t* activity_indicator;
        lv_obj_t* divider;

        struct {
            lv_obj_t* cont;
            lv_obj_t* accent;
            lv_obj_t* label_name;
            lv_obj_t* label_val;
        } constell[7];
    } ui;

    bool activity_indicator_on = false;

    void Create(lv_obj_t* root);
    void Delete();
    void ApplyLanguage() const;
    void UpdateActivityIndicator();
    void UpdateValues(int gps, int bds, int gln, int gal, int sbas, int qzss, int irnss);
};

} // namespace Page

#endif // STAR_MAP_VIEW_H
