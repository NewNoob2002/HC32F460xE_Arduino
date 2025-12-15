//
// Created by gtc on 2025/12/15.
//

#ifndef LVGL_HARDWARE_CHECK_MODEL_H
#define LVGL_HARDWARE_CHECK_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page
{

    class HardwareCheckModel {
    public:
        HardwareCheckModel() {
            account = nullptr;
        }
        ~HardwareCheckModel() = default;
        void Init();
        void Deinit();
        void SetStatusBarAppear(bool en) const;
        void SetStatusBarStyle(DataProc::StatusBar_Style_t style) const;

    private:
        Account* account;
    };
};

#endif //LVGL_HARDWARE_CHECK_MODEL_H