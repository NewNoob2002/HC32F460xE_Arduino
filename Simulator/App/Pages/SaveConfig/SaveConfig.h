#ifndef SAVECONFIG_H
#define SAVECONFIG_H

#include "SaveConfigView.h"
#include "SaveConfigModel.h"

namespace Page {
class SaveConfig : public PageBase {
public:
    typedef struct {
        uint16_t time;
        lv_color_t color;
    } Param_t;

    SaveConfig() = default;

    ~SaveConfig() override = default;

    void onCustomAttrConfig() override;

    void onViewLoad() override;

    void onViewDidLoad() override;

    void onViewWillAppear() override;

    void onViewDidAppear() override;

    void onViewWillDisappear() override;

    void onViewDidDisappear() override;

    void onViewUnload() override;

    void onViewDidUnload() override;

    void onLanguageChanged() override;

    SaveConfigView View{};
    SaveConfigModel Model{};

private:
    void UpdatePowerOffCauseText() const;
    static I18n::TextId GetPowerOffCauseTextId();
};
}

#endif
