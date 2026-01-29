#include "HAL.h"
#include "Arduino.h"

static ledState_t powerLed       = {0, 0};
static ledState_t chargerLed     = {0, 0};
static ledState_t functionKeyLed = {0, 0};

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
				systemInfo.powerMonitor.batteryInfo.LowBatteryCount = 0;
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
        if (systemInfo.powerMonitor.panel_power_on == 0)
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
                systemInfo.recordInfo.record_status = 0;
                systemInfo.recordInfo.record_op     = 1;
            }
            systemInfo.powerMonitor.batteryInfo.LowBatteryCount++;
            if (systemInfo.powerMonitor.batteryInfo.LowBatteryCount >= 1800) {
                systemInfo.powerMonitor.batteryInfo.LowBatteryCount = 0;
                systemInfo.powerMonitor.LowBatteryPowerOff          = 1;
            }
        }
    }
}

static void Led_Update_Function()
{
    if (systemInfo.powerMonitor.ShutdownReq || systemInfo.powerMonitor.pannel_shutdown_req_time != 0 || !systemInfo.online_device.eg25_board)
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