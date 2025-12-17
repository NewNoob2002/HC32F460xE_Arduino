#include "DataProc.h"
#include "../HAL/HAL.h"

typedef enum
{
    GPS_STATUS_DISCONNECT,
    GPS_STATUS_UNSTABLE,
    GPS_STATUS_CONNECT,
} GPS_Status_t;

static GPS_Status_t nowStatus = GPS_STATUS_DISCONNECT;

static void onTimer(Account* account)
{
    const PositionInfo_t gpsInfo = systemInfo.positionInfo;

    // if (const int coordinate_status = gpsInfo.coordinate_status; coordinate_status >= 4)
    // {
    //     nowStatus = GPS_STATUS_CONNECT;
    // }
    // else if (coordinate_status == 1)
    // {
    //     nowStatus = GPS_STATUS_UNSTABLE;
    // }
    // else if (coordinate_status == 0)
    // {
    //     nowStatus = GPS_STATUS_DISCONNECT;
    // }
    account->Commit(&gpsInfo, sizeof(gpsInfo));
    account->Publish();
}

static int onEvent(Account* account, Account::EventParam_t* param)
{
    if (param->event == Account::EVENT_TIMER)
    {
        onTimer(account);
        return Account::RES_OK;
    }

    if (param->event != Account::EVENT_SUB_PULL)
    {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    if (param->size != sizeof(PositionInfo_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    param->data_p = &systemInfo.positionInfo;

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(GPS)
{
    account->SetEventCallback(onEvent);
    account->SetTimerPeriod(1000);
}
