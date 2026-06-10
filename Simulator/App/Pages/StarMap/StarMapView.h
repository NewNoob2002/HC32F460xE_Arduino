#ifndef STAR_MAP_VIEW_H
#define STAR_MAP_VIEW_H

#include "../Page.h"

namespace Page {

class StarMapView {
  public:
    struct {
        lv_obj_t* label_title;
        lv_obj_t* divider;

        struct {
            lv_obj_t* cont;
            lv_obj_t* accent;
            lv_obj_t* label_name;
            lv_obj_t* label_val;
        } constell[7];
    } ui;

    void Create(lv_obj_t* root);
    void Delete();
    void ApplyLanguage() const;
    void UpdateValues(int gps, int bds, int gln, int gal, int sbas, int qzss, int irnss);
};

} // namespace Page

#endif // STAR_MAP_VIEW_H
