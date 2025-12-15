#include "StartupModel.h"

using namespace Page;

void StartupModel::Init()
{
    account = new Account("StartupModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
}

void StartupModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}


void StartupModel::SetStatusBarAppear(const bool en, const bool delay) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
    info.cmd = DataProc::STATUS_BAR_CMD_APPEAR;
    info.param.appear = en;
    info.param.delay = delay;
    account->Notify("StatusBar", &info, sizeof(info));
}
