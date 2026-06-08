#ifndef SAVECONFIG_VIEW_H
#define SAVECONFIG_VIEW_H

#include "../Page.h"

namespace Page {
class SaveConfigView {
public:
    void Create(lv_obj_t* root);

    void Delete();

    void ApplyLanguage() const;

    void SetPowerOffCause(I18n::TextId causeId) const;

    struct {
        struct {
            lv_obj_t* cont;
            lv_obj_t* label;
            lv_obj_t* brandCont;
            lv_obj_t* brandShine;
            lv_obj_t* percentLabel;
            lv_obj_t* dots[3];
            lv_anim_t dotAnims[3];
            lv_anim_t shineAnim;

            struct {
                lv_obj_t* track;
                lv_obj_t* obj;
                lv_anim_t anim;
            } bar;
        } sync;

        lv_anim_timeline_t* anim_timeline;
    } ui;
};
}

#endif // !SHUTDOWN_VIEW_H
