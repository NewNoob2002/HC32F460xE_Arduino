#include "DataProc.h"
#include "../HAL/HAL.h"

// static void onTimer(Account* account)
// {
//     static Charger_Status_t lastStatus = notCharge;
//
//     BatteryInfo_t power;
//     HAL::Power_GetInfo(&power);
// }

static int onEvent(Account* account, Account::EventParam_t* param)
{
    // if (param->event == Account::EVENT_TIMER)
    // {
    //     onTimer(account);
    //     return Account::RES_OK;
    // }

    if (param->event != Account::EVENT_SUB_PULL)
    {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    if (param->size != sizeof(BatteryInfo_t))
    {
        return Account::RES_SIZE_MISMATCH;
    }

    BatteryInfo_t powerInfo;
    HAL::Power_GetInfo(&powerInfo);
    memcpy(param->data_p, &powerInfo, param->size);

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(Power)
{
    account->SetEventCallback(onEvent);
    // account->SetTimerPeriod(500);
}
