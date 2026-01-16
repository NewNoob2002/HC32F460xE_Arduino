#include "SaveConfigModel.h"

using namespace Page;

void SaveConfigModel::Init() {
    account = new Account("SaveConfigModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
}

void SaveConfigModel::Deinit() {
    if (account) {
        delete account;
        account = nullptr;
    }
}

void SaveConfigModel::SetStatusBarAppear(const bool delay) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = true;
    info.param.delay = delay;
    account->Notify("StatusBar", &info, sizeof(info));
}

void SaveConfigModel::SetStatusBarDisappear(const bool delay) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = false;
    info.param.delay = delay;
    account->Notify("StatusBar", &info, sizeof(info));
}
