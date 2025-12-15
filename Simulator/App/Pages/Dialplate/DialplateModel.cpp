#include "DialplateModel.h"

using namespace Page;

void DialplateModel::Init() {
    account = new Account("DialplateModel", DataProc::Center(), 0, this);
    account->Subscribe("SportStatus");
    account->Subscribe("Recorder");
    account->Subscribe("StatusBar");
    account->Subscribe("GPS");
    account->Subscribe("MusicPlayer");
    account->SetEventCallback(onEvent);
}

void DialplateModel::Deinit() {
    if (account) {
        delete account;
        account = nullptr;
    }
}

bool DialplateModel::GetGPSReady() {
    PositionInfo_t gps;
    if (account->Pull("GPS", &gps, sizeof(gps)) != Account::RES_OK) {
        return false;
    }
    return (gps.satellite_number_track > 0);
}

int DialplateModel::onEvent(Account *account, Account::EventParam_t *param) {
    if (param->event != Account::EVENT_PUB_PUBLISH) {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    DialplateModel *instance = (DialplateModel *) account->UserData;

    return Account::RES_OK;
}

void DialplateModel::RecorderCommand(RecCmd_t cmd) {
    RecordInfo_t recInfo;
    DATA_PROC_INIT_STRUCT(recInfo);
    recInfo.cmd = static_cast<DataProc::Recorder_Cmd_t>(cmd);
    account->Notify("Recorder", &recInfo, sizeof(recInfo));

    DataProc::StatusBar_Info_t statInfo;
    DATA_PROC_INIT_STRUCT(statInfo);
    statInfo.cmd = DataProc::STATUS_BAR_CMD_SET_LABEL_REC;

    switch (cmd) {
        case REC_START:
            statInfo.param.record_active = true;
            break;
        case REC_STOP:
            statInfo.param.record_active = false;
            break;
        default:
            break;
    }

    account->Notify("StatusBar", &statInfo, sizeof(statInfo));
}

void DialplateModel::PlayMusic(const char *music) {
    DataProc::MusicPlayer_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.music = music;
    account->Notify("MusicPlayer", &info, sizeof(info));
}

void DialplateModel::SetStatusBarStyle(DataProc::StatusBar_Style_t style) {
    DataProc::StatusBar_Info_t info;
    DATA_PROC_INIT_STRUCT(info);

    info.cmd = DataProc::STATUS_BAR_CMD_SET_STYLE;
    info.param.style = style;

    account->Notify("StatusBar", &info, sizeof(info));
}
