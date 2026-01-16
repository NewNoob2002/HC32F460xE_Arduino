#ifndef STARTUP_MODEL_H
#define STARTUP_MODEL_H

#include "Common/DataProc/DataProc.h"
#include "HAL/HAL.h"

namespace Page {
    class StartupModel {
    public:
        StartupModel() : account(nullptr) {
        }

        ~StartupModel() {
            delete account;
        }

        void Init();

        void Deinit();

        void SetStatusBarAppear(bool en, bool delay) const;

        static void SetEncoderEnable(const bool en) {
#if defined(_WIN32)
#else
            HAL::Encoder_SetEnable(en);
#endif
        }

    private:
        Account *account;
    };
}

#endif
