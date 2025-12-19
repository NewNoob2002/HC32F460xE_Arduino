#include "ShutdownModel.h"

using namespace Page;

void ShutdownModel::Init() {
    account = new Account("ShutdownModel", DataProc::Center(), 0, this);
    account->SetEventCallback(onEvent);
    account->Subscribe("StatusBar");
//    account->Subscribe("Power");
}

void ShutdownModel::Deinit() {
    if (account) {
        delete account;
        account = nullptr;
    }
}

void ShutdownModel::SetStatusBarAppear(const bool delay) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = true;
    info.param.delay = delay;
    account->Notify("StatusBar", &info, sizeof(info));
}

void ShutdownModel::SetStatusBarDisappear(const bool delay) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = false;
    info.param.delay = delay;
    account->Notify("StatusBar", &info, sizeof(info));
}

int ShutdownModel::onEvent(Account *account, Account::EventParam_t *param) {
    if (param->event != Account::EVENT_PUB_PUBLISH) {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    if (param->size != sizeof(Power_Monitor_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    auto *instance = static_cast<ShutdownModel *>(account->UserData);
    LV_ASSERT_NULL(instance);
    return Account::RES_OK;
}
