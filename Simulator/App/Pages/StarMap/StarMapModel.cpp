#include "StarMapModel.h"

using namespace Page;

void
StarMapModel::Init() {
    account = new Account("StarMapModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
}

void
StarMapModel::Deinit() {
    if (account) {
        delete account;
        account = nullptr;
    }
}

void
StarMapModel::SetStatusBarStyle(const DataProc::StatusBar_Style_t style) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}
