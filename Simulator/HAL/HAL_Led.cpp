#include "HAL.h"
#include "lvgl/lvgl.h"
#include "Common/DataProc/DataProc.h"

static Account *actLed;

static ledState_t powerLed       = {0, 0, false};
static ledState_t chargerLed     = {0, 0, false};
static ledState_t functionKeyLed = {0, 0, false};

static void onTimer(Account *account)
{
    const uint32_t now = lv_tick_get();
    if (powerLed.currentRate > 0 && powerLed.usedByOtherTask == false) {
        if (now - powerLed.lastToggleTime >= powerLed.currentRate) {
            powerLed.lastToggleTime = now;
            // digitalToggle(POWER_LED_PIN);
        }
    }
    if (chargerLed.currentRate > 0 && chargerLed.usedByOtherTask == false) {
        if (now - chargerLed.lastToggleTime >= chargerLed.currentRate) {
            chargerLed.lastToggleTime = now;
            // digitalToggle(CHARGE_LED_PIN);
        }
    }
    if (functionKeyLed.currentRate > 0 && functionKeyLed.usedByOtherTask == false) {
        if (now - functionKeyLed.lastToggleTime >= functionKeyLed.currentRate) {
            functionKeyLed.lastToggleTime = now;
            // digitalToggle(FUNCTION_LED_PIN);
        }
    }
}

static int onEvent(Account *account, Account::EventParam_t *param)
{

    if (param->event == Account::EVENT_TIMER) {
        onTimer(account);
        return Account::RES_OK;
    }

    if (param->event != Account::EVENT_NOTIFY) {
        return Account::RES_UNSUPPORTED_REQUEST;
    }
    if (param->size != sizeof(DataProc::Led_Info_t)) {
        return Account::RES_SIZE_MISMATCH;
    }

    DataProc::Led_Info_t *info = (DataProc::Led_Info_t *)param->data_p;
    switch (info->cmd) {
        case DataProc::LED_CMD_BLINK: {
            if (info->param.powerLed_blink_interval > 0) {
                powerLed.usedByOtherTask = false;
                powerLed.currentRate     = info->param.powerLed_blink_interval;
            }
            if (info->param.powerLed_blink_interval > 0) {
                chargerLed.usedByOtherTask = false;
                chargerLed.currentRate     = info->param.charge_blink_interval;
            }
            if (info->param.powerLed_blink_interval > 0) {

                functionKeyLed.usedByOtherTask = false;
                functionKeyLed.currentRate     = info->param.funckey_blink_interval;
            }

            break;
        }
        case DataProc::LED_CMD_NOT_BLINK:
            if (info->param.powerLed_blink_interval == 0) {
                powerLed.usedByOtherTask = true;
                powerLed.currentRate     = info->param.powerLed_blink_interval;
            }
            if (info->param.powerLed_blink_interval == 0) {
                chargerLed.usedByOtherTask = true;
                chargerLed.currentRate     = info->param.charge_blink_interval;
            }
            if (info->param.powerLed_blink_interval == 0) {

                functionKeyLed.usedByOtherTask = true;
                functionKeyLed.currentRate     = info->param.funckey_blink_interval;
            }
            break;
        default:
            return Account::RES_PARAM_ERROR;
    }
    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(Led)
{
    account->SetEventCallback(onEvent);
    account->SetTimerPeriod(100);

    actLed = account;
}