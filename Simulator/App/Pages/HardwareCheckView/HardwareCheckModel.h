//
// Created by gtc on 2025/12/15.
//

#ifndef LVGL_HARDWARE_CHECK_MODEL_H
#define LVGL_HARDWARE_CHECK_MODEL_H

#include "Common/DataProc/DataProc_Def.h"
#include "HAL/HAL.h"
#include "Utils/DataCenter/Account.h"

namespace Page {
class HardwareCheckModel {
  public:
    HardwareCheckModel() { account = nullptr; }

    ~HardwareCheckModel() = default;

    void Init();

    void Deinit();

    void SetStatusBarAppear(bool en) const;

    void SetStatusBarStyle(DataProc::StatusBar_Style_t style) const;

    static void
    SetEncoderEnable(const bool en) {
#if defined(LVGL_SIMULATOR) || defined(_WIN32)
#else
        HAL::Encoder_SetEnable(en);
#endif
    }

  private:
    Account* account;
};
}; // namespace Page

#endif //LVGL_HARDWARE_CHECK_MODEL_H
