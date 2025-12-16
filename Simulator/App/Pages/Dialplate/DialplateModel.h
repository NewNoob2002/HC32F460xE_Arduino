#ifndef __DIALPLATE_MODEL_H
#define __DIALPLATE_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page
{

class DialplateModel
{
public:
    DialplateModel() = default;
    ~DialplateModel()= default;
    typedef enum
    {
        REC_START    = DataProc::RECORDER_CMD_START,
        REC_STOP     = DataProc::RECORDER_CMD_STOP
    } RecCmd_t;

public:
    void Init();
    void Deinit();

    bool GetGPSInfo(PositionInfo_t* gps) const;

    void RecorderCommand(RecCmd_t cmd) const;
    void SetStatusBarStyle(DataProc::StatusBar_Style_t style) const;

private:
    Account* account{};

private:
    static int onEvent(Account* account, Account::EventParam_t* param);
};

}

#endif
