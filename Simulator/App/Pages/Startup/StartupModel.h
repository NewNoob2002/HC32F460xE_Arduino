#ifndef STARTUP_MODEL_H
#define STARTUP_MODEL_H

#include "Common/DataProc/DataProc.h"

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

    private:
        Account *account;
    };
}

#endif
