//
// Created by gtc on 2025/12/15.
//

#include "HardwareCheckModel.h"

using namespace Page;

void HardwareCheckModel::Init() {
    account = new Account("StartupModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
}

void HardwareCheckModel::Deinit() {
    if (account)
    {
        delete account;
        account = nullptr;
    }
}

void HardwareCheckModel::SetStatusBarAppear(const bool en) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    info.param.delay = true;
    account->Notify("StatusBar", &info, sizeof(info));
}

void HardwareCheckModel::SetStatusBarStyle(const DataProc::StatusBar_Style_t style) const {
        DataProc::StatusBar_Info_t info;
        DATA_PROC_INIT_STRUCT(info);
        info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
        info.param.style = style;
        account->Notify("StatusBar", &info, sizeof(info));
}
