#include <cstdio>
#include "DataProc.h"

using namespace DataProc;

typedef struct
{
    RecordInfo_t recInfo;
    Account* account;
} Recorder_t;

static void Recorder_RecStart(Recorder_t* recorder, const uint16_t time)
{
    LV_LOG_USER("Static record start, time:%d", time);
}

static void Recorder_RecStop(Recorder_t* recorder)
{
    LV_LOG_USER("Static record end");
}

static int onNotify(Recorder_t* recorder, RecordInfo_t* info)
{
    switch (info->cmd)
    {
    case RECORDER_CMD_START:
        Recorder_RecStart(recorder, recorder->recInfo.record_interval);
        break;
    case RECORDER_CMD_STOP:
        Recorder_RecStop(recorder);
        break;
    }

    return 1;
}

static int onEvent(Account* account, Account::EventParam_t* param)
{
    Account::ResCode_t res = Account::RES_UNKNOW;
    auto* recorder = static_cast<Recorder_t *>(account->UserData);;

    switch (param->event)
    {
    case Account::EVENT_SUB_PULL:
        if (param->size == sizeof(RecordInfo_t))
        {
            memcpy(param->data_p, &(recorder->recInfo), param->size);
        }
        else
        {
            res = Account::RES_SIZE_MISMATCH;
        }
        break;

    case Account::EVENT_NOTIFY:
        if (param->size == sizeof(Recorder_t))
        {
            onNotify(recorder, static_cast<RecordInfo_t *>(param->data_p));
            res = Account::RES_OK;
        }
        else
        {
            res = Account::RES_SIZE_MISMATCH;
        }
        break;

    default:
        break;
    }

    return res;
}

DATA_PROC_INIT_DEF(Recorder)
{
    static Recorder_t recorder;
    // memset(&recorder.recInfo, 0, sizeof(recorder.recInfo));
    // memset(&recorder.file, 0, sizeof(recorder.file));
    // recorder.active = false;
    // recorder.account = account;
    // account->UserData = &recorder;

    account->Subscribe("GPS");
    account->SetEventCallback(onEvent);
}
