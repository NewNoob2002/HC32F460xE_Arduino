#include "HAL.h"
#include "Arduino.h"
#include "lvgl/lvgl.h"
#include "Common/DataProc/DataProc.h"

static Account *actLed;

static ledState_t powerLed       = {0, 0, false};
static ledState_t chargerLed     = {0, 0, false};
static ledState_t functionKeyLed = {0, 0, false};

static void Led_Update_Charge()
{
    if (systemInfo.powerMonitor.ShutdownReq)
        return;
    const uint32_t now = millis();
    if (chargerLed.currentRate > 0) {
        if (now - chargerLed.lastToggleTime >= chargerLed.currentRate) {
            chargerLed.lastToggleTime = now;
            digitalToggle(CHARGE_LED_PIN);
        }
    }
    if (powerLed.currentRate > 0) {
        if (now - powerLed.lastToggleTime >= powerLed.currentRate) {
            powerLed.lastToggleTime = now;
            digitalToggle(POWER_LED_PIN);
        }
    }

    if (systemInfo.powerMonitor.batteryInfo.chargeStatus != 0x00) {
        CORE_DEBUG_PRINTF("CHARGE\n");
        digitalWrite(POWER_LED_PIN, LOW);
        powerLed.currentRate = 0;
        bool isFullCharge    = (systemInfo.powerMonitor.batteryInfo.Percent >= 100);
        if (isFullCharge) {
            chargerLed.currentRate = 0;
            digitalWrite(CHARGE_LED_PIN, HIGH);
        } else {
            if (systemInfo.powerMonitor.batteryInfo.chargeStatus == 0x02) {
                chargerLed.currentRate = 300;
            } else {
                chargerLed.currentRate = 1000;
            }
        }
    } else {
        if (systemInfo.powerMonitor.panel_power_on == false)
            return;
        digitalWrite(CHARGE_LED_PIN, LOW);
        chargerLed.currentRate = 0;
        if (systemInfo.powerMonitor.batteryInfo.Percent >= 14) {
            digitalWrite(POWER_LED_PIN, HIGH);
        } else if ((systemInfo.powerMonitor.batteryInfo.Percent < 14) && (systemInfo.powerMonitor.batteryInfo.Percent > 9)) {
            powerLed.currentRate = 1000;
        } else if ((systemInfo.powerMonitor.batteryInfo.Percent <= 9) && (systemInfo.powerMonitor.batteryInfo.Percent > 0)) {
            powerLed.currentRate = 300;
            if (systemInfo.recordInfo.record_status) {
                systemInfo.recordInfo.record_status = On_Off_Status_OFF;
                systemInfo.recordInfo.record_op     = 1;
            }
            systemInfo.powerMonitor.batteryInfo.LowBatteryCount++;
            if (systemInfo.powerMonitor.batteryInfo.LowBatteryCount >= 1800) {
                systemInfo.powerMonitor.batteryInfo.LowBatteryCount = 0;
                systemInfo.powerMonitor.LowBatteryPowerOff          = true;
            }
        }
    }
}

static void Led_Update_Function()
{
    if (systemInfo.powerMonitor.ShutdownReq || !systemInfo.online_device.eg25_board)
        return;
    if (systemInfo.recordInfo.record_status) {
        functionKeyLed.currentRate = 500;
    } else {
        digitalWrite(FUNCTION_LED_PIN, LOW);
        functionKeyLed.currentRate = 0;
    }
    /*Monitor*/
    if (functionKeyLed.currentRate > 0) {
        const uint32_t now = millis();
        if (now - functionKeyLed.lastToggleTime >= functionKeyLed.currentRate) {
            functionKeyLed.lastToggleTime = now;
            digitalToggle(FUNCTION_LED_PIN);
        }
    }
}

void HAL::Led_Update()
{
    Led_Update_Charge();
    Led_Update_Function();
}
// static void onTimer(Account *account)
//{
//     if (powerLed.currentRate > 0 && powerLed.usedByOtherTask == false) {
//         const uint32_t now = lv_tick_get();
//         if (now - powerLed.lastToggleTime >= powerLed.currentRate) {
//             powerLed.lastToggleTime = now;
//             digitalToggle(POWER_LED_PIN);
//         }
//     }
//     if (chargerLed.currentRate > 0 && chargerLed.usedByOtherTask == false) {
//         const uint32_t now = lv_tick_get();
//         if (now - chargerLed.lastToggleTime >= chargerLed.currentRate) {
//             chargerLed.lastToggleTime = now;
//             digitalToggle(CHARGE_LED_PIN);
//         }
//     }
//     if (functionKeyLed.currentRate > 0 && functionKeyLed.usedByOtherTask == false) {
//         const uint32_t now = lv_tick_get();
//         if (now - functionKeyLed.lastToggleTime >= functionKeyLed.currentRate) {
//             functionKeyLed.lastToggleTime = now;
//             digitalToggle(FUNCTION_LED_PIN);
//         }
//     }
// }

// static int onEvent(Account *account, Account::EventParam_t *param)
//{

//    if (param->event == Account::EVENT_TIMER) {
//        onTimer(account);
//        return Account::RES_OK;
//    }

//    if (param->event != Account::EVENT_NOTIFY) {
//        return Account::RES_UNSUPPORTED_REQUEST;
//    }
//    if (param->size != sizeof(DataProc::Led_Info_t)) {
//        return Account::RES_SIZE_MISMATCH;
//    }

//    DataProc::Led_Info_t *info = (DataProc::Led_Info_t *)param->data_p;
//    switch (info->cmd) {
//        case DataProc::LED_CMD_BLINK: {
//            if (info->param.powerLed_blink_interval > 0) {
//                powerLed.usedByOtherTask = false;
//            }
//						else
//						{
//							 powerLed.usedByOtherTask = true;
//						}
//						powerLed.currentRate     = info->param.powerLed_blink_interval;
//            if (info->param.powerLed_blink_interval > 0) {
//                chargerLed.usedByOtherTask = false;
//            }
//						else
//						{
//							 chargerLed.usedByOtherTask = true;
//						}
//						 chargerLed.currentRate     = info->param.charge_blink_interval;
//            if (info->param.powerLed_blink_interval > 0) {
//                functionKeyLed.usedByOtherTask = false;
//            }
//						else
//						{
//							functionKeyLed.usedByOtherTask = true;
//						}
//							functionKeyLed.currentRate     = info->param.funckey_blink_interval;
//            break;
//        }
//        case DataProc::LED_CMD_NOT_BLINK:
//            if (info->param.powerLed_blink_interval == 0) {
//                powerLed.usedByOtherTask = true;
//                powerLed.currentRate     = info->param.powerLed_blink_interval;
//            }
//            if (info->param.powerLed_blink_interval == 0) {
//                chargerLed.usedByOtherTask = true;
//                chargerLed.currentRate     = info->param.charge_blink_interval;
//            }
//            if (info->param.powerLed_blink_interval == 0) {

//                functionKeyLed.usedByOtherTask = true;
//                functionKeyLed.currentRate     = info->param.funckey_blink_interval;
//            }
//            break;
//        default:
//            return Account::RES_PARAM_ERROR;
//    }
//    return Account::RES_OK;
//}

// DATA_PROC_INIT_DEF(Led)
//{
//     account->SetEventCallback(onEvent);
//     account->SetTimerPeriod(100);

//    actLed = account;
//}