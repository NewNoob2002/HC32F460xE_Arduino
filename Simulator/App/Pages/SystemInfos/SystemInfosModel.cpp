#include "SystemInfosModel.h"

using namespace Page;

void SystemInfosModel::Init()
{
    account = new Account("SystemInfosModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
}

void SystemInfosModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}

void SystemInfosModel::GetGPSInfo(PositionInfo_t *gps) const {
    account->Pull("GPS", gps, sizeof(PositionInfo_t));
}

void SystemInfosModel::GetBatteryInfo(BatteryInfo_t *power) const {
    account->Pull("Power", power, sizeof(BatteryInfo_t));
}

void SystemInfosModel::GetRecordInfo(RecordInfo_t *record) const {
    account->Pull("Recorder", record, sizeof(RecordInfo_t));
}

void SystemInfosModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style) const {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}
