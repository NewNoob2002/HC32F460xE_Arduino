#include "ShutdownModel.h"

using namespace Page;

void ShutdownModel::Init() {
    account = new Account("ShutdownModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
}

void ShutdownModel::Deinit() {
    if (account)
    {
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
