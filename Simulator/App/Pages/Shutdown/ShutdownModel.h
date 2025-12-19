#ifndef SHUTDOWN_MODEL_H
#define SHUTDOWN_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page {
    class ShutdownModel {
    public:
        ShutdownModel() : account(nullptr) {
        }

        ~ShutdownModel() {
            delete account;
        }

        void Init();

        void Deinit();

        void SetStatusBarAppear(bool delay) const;

        void SetStatusBarDisappear(bool delay) const;
    private:
        Account *account;
        static int onEvent(Account *account, Account::EventParam_t *param);
    };
}

#endif
