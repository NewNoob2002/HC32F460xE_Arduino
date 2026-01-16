#ifndef SAVECONFIG_MODEL_H
#define SAVECONFIG_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page {
    class SaveConfigModel {
    public:
        SaveConfigModel() : account(nullptr) {
        }

        ~SaveConfigModel() {
            delete account;
        }

        void Init();

        void Deinit();

        void SetStatusBarAppear(bool delay) const;

        void SetStatusBarDisappear(bool delay) const;
    private:
        Account *account;
    };
}

#endif
