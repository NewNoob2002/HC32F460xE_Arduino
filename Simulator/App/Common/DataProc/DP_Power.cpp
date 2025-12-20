#include "DataProc.h"
#include "../HAL/HAL.h"

static void onTimer(Account *account)
{
    // Power_Monitor_t power;
    // HAL::Power_GetInfo(&power);
    // HAL::Power_PowerOffMonitor();
    //
    // account->Commit(&power, sizeof(power));
    // account->Publish();
    //
    // DataProc::Led_Info_t info;
    // DATA_PROC_INIT_STRUCT(info);
    // if (power.batteryInfo.chargeStatus != 0x00) {
    //     info.param.powerLed_blink_interval = 0;
    //     digitalWrite(POWER_LED_PIN, LOW);
    //     bool isFullCharge = (batteryState.Percent >= 100);
    //         account->Notify("Led",
    // }
}

static int onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event == Account::EVENT_TIMER) {
        onTimer(account);
        return Account::RES_OK;
    }

    if (param->event != Account::EVENT_SUB_PULL) {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    if (param->size != sizeof(BatteryInfo_t)) {
        return Account::RES_SIZE_MISMATCH;
    }

    // Power_Monitor_t powerInfo;
    // HAL::Power_GetInfo(&powerInfo);
    // memcpy(param->data_p, &powerInfo, param->size);

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(Power)
{
    account->Subscribe("Led");
    account->SetEventCallback(onEvent);
    account->SetTimerPeriod(500);
}
