#ifndef __RECORDCONFIG_MODEL_H
#define __RECORDCONFIG_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page {

class RecordConfigModel {
public:
    RecordConfigModel() = default;

    ~RecordConfigModel() = default;

    typedef enum {
        REC_START = DataProc::RECORDER_CMD_START,
        REC_STOP = DataProc::RECORDER_CMD_STOP
    } RecCmd_t;

public:
    void Init();

    void Deinit();

    void RecorderCommand(RecCmd_t cmd) const;

    void SetStatusBarStyle(DataProc::StatusBar_Style_t style) const;

private:
    Account* account{};

private:
    static int onEvent(Account* account, Account::EventParam_t* param);
};

}

#endif
