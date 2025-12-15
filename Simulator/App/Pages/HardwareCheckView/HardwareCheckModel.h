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
        void Init();
        void Deinit();
        void SetStatusBarAppear(bool en);

    private:
        // Account* account;
    };
};

#endif //LVGL_HARDWARE_CHECK_MODEL_H