#include "HAL.h"
#include "Arduino.h"
#include "bq40z50.h"
#include "mp2762a.h"

#define FORCE_SHUTDOWN() (systemInfo.powerMonitor.Force_ShutDown)

static bool POWER_GET_REASON = false;

void HAL::Power_Init()
{
    uint16_t reg_rmu   = *((unsigned short *)(CM_RMU_BASE));
    bool softwareReset = reg_rmu & RMU_RSTF0_SWRF;
		pinMode(POWER_LED_PIN, OUTPUT);
    pinMode(FUNCTION_LED_PIN, OUTPUT);
    pinMode(CHARGE_LED_PIN, OUTPUT);

    pinMode(WATCHDOG_FEED_PIN, OUTPUT);
    if (softwareReset) {
				pinMode(POWER_CONTROL_PIN, OUTPUT, HIGH);
        CORE_DEBUG_PRINTF("Power: GetResetause:Software Reset");
    } else{
				pinMode(POWER_CONTROL_PIN, OUTPUT);
        CORE_DEBUG_PRINTF("Power: GetResetause:PowerOn Reset");
			  CORE_DEBUG_PRINTF("Power: Waiting Keep Press...");
    }
}

void HAL::Power_OnCheck()
{
		uint16_t reg_rmu   = *((unsigned short *)(CM_RMU_BASE));
    bool softwareReset = reg_rmu & RMU_RSTF0_SWRF;
	  while (true) {
				HAL_Update();
        lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_HIDDEN);
        lv_timer_handler();
        if (systemInfo.powerMonitor.panel_power_on) {
            CORE_DEBUG_PRINTF("MCU PowerDone");
            break;
        }
        if (softwareReset) {
            CORE_DEBUG_PRINTF("softwareRst PowerDone");
            break;
        }
    }
    CORE_DEBUG_PRINTF("Power: Done");
    digitalWrite(POWER_LED_PIN, HIGH);
    digitalWrite(FUNCTION_LED_PIN, HIGH);
    digitalWrite(POWER_CONTROL_PIN, HIGH);
}

void HAL::Power_Shutdown(bool en)
{
    if (!en) {
        CORE_DEBUG_PRINTF("Shutdown Req");
        systemInfo.powerMonitor.ShutdownReq = true;
    } else {
        CORE_DEBUG_PRINTF("Shutdown Done");
        digitalWrite(POWER_CONTROL_PIN, LOW);
    }
}

void HAL::Power_PowerOffMonitor()
{
		if(POWER_GET_REASON)
			return;
    if (systemInfo.powerMonitor.ShutdownReq || systemInfo.powerMonitor.LowBatteryPowerOff) {
        digitalWrite(CHARGE_LED_PIN, LOW);
        digitalWrite(POWER_LED_PIN, HIGH);
        digitalWrite(FUNCTION_LED_PIN, HIGH);
        systemInfo.powerMonitor.poweroff_flag = 1;
        systemInfo.powerMonitor.ShutdownReq   = false;
    }
		if ((systemInfo.powerMonitor.ShutdownEnsure) || FORCE_SHUTDOWN() || systemInfo.powerMonitor.LowBatteryPowerOff) {
        if(!POWER_GET_REASON)
				{
					POWER_GET_REASON = true;
					CORE_DEBUG_PRINTF("POWER OFF .................Power Off Cause: %s", Power_GetPowerOffCause());
				}
    }
}

bool HAL::Power_ShutdownEnsure(){
	if(systemInfo.powerMonitor.ShutdownEnsure)
	{
		systemInfo.powerMonitor.ShutdownEnsure = false;
		return true;
	}
	return false;
}

bool HAL::Power_ShutdownForce() {
    if(systemInfo.powerMonitor.Force_ShutDown)
    {
        systemInfo.powerMonitor.Force_ShutDown = false;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownLinux()
{
	if(systemInfo.powerMonitor.LinuxPowerOff)
  {
		systemInfo.powerMonitor.LinuxPowerOff = false;
		return true;
  }
	return false;
}

bool HAL::Power_ShutdownLowBattery() {
    if(systemInfo.powerMonitor.LowBatteryPowerOff)
    {
        systemInfo.powerMonitor.LowBatteryPowerOff = false;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownSoftReset()
{
		if(systemInfo.powerMonitor.reset_flag)
    {
        systemInfo.powerMonitor.reset_flag = false;
        return true;
    }
    return false;
}

void HAL::Power_Update()
{
    WatchDog_Feed();
		Power_GetInfo(&systemInfo.powerMonitor);
}

void HAL::Power_GetInfo(Power_Monitor_t *info)
{
    if (millis() - systemInfo.powerMonitor.BatteryLastHandleTime >= 3000) {
        systemInfo.powerMonitor.BatteryLastHandleTime = millis();
        if (!systemInfo.online_device.bq40z50) {

        } else {
            checkBatteryInfo(&info->batteryInfo);
        }
    }
    if (!systemInfo.online_device.mp2762) {
        /*
         *V1.3 Board do
         */
    } else {
        charger_update(&info->batteryInfo);
    }
}

void HAL::WatchDog_Feed()
{
    if (millis() - systemInfo.powerMonitor.WatchDogLastFeedTime >= WATCHDOG_FEED_TIME) {
        systemInfo.powerMonitor.WatchDogLastFeedTime = millis();
        digitalToggle(WATCHDOG_FEED_PIN);
    }
}

bool chagrer_begin(pBatteryInfo_t p_batteryState)
{
    // Reset all configure
    mp2762registerReset();
    // Set Charger Current
    mp2762setFastChargeVoltageMv(6600);
    // Setting precharge current to 880mA
    mp2762setPrechargeCurrentMa(880);
    // Setting fast charge current to 1600mA
    mp2762setFastChargeCurrentMa(1600);
    // get charge status
    uint8_t charge_status        = mp2762getChargeStatus();
    p_batteryState->chargeStatus = static_cast<Charger_Status_t>(charge_status);

    return true;
}

void charger_update(pBatteryInfo_t p_batteryState)
{
    const uint8_t charge_status = mp2762getChargeStatus();
    //		Serial.printf("typec: %d, charge_status:%d\n", typec_detect, charge_status);
    if (CM_VALUE_IN_RANGE(charge_status, 0, 2)) {
        p_batteryState->chargeStatus = static_cast<Charger_Status_t>(charge_status);
    }
}

void checkBatteryInfo(pBatteryInfo_t p_batteryState)
{
    uint16_t batteryLevelPercent = bq40z50getRelativeStateOfCharge();
    float batteryLevelPercent_f  = bq40z50getRelativeStateOfCharge_float();

    uint16_t batteryVoltage_mV = bq40z50getVoltageMv();
    uint16_t batteryVoltage    = batteryVoltage_mV / 1000;
    float batteryVoltage_f     = batteryVoltage_mV / 1000.0;

    uint16_t batteryTempC = bq40z50getTemperatureC();
    float batteryTempC_f  = batteryTempC / 10.0 - 273.15;

    p_batteryState->Actual_Percent = batteryLevelPercent;

    p_batteryState->Percent = CM_SET_VALUE_IN_RANGE(batteryLevelPercent, 0, 100);
    p_batteryState->Voltage = CM_SET_VALUE_IN_RANGE(batteryVoltage, 6, 9);
    p_batteryState->Temp    = CM_SET_VALUE_IN_RANGE(batteryTempC, 1, 65535);

    p_batteryState->Percent_f = CM_SET_VALUE_IN_RANGE(batteryLevelPercent_f, 0.0, 100.0);
    p_batteryState->Voltage_f = CM_SET_VALUE_IN_RANGE(batteryVoltage_f, 6.0, 8.4);
    p_batteryState->Temp_f    = CM_SET_VALUE_IN_RANGE(batteryTempC_f, 0.0, 100.0);
}

const char *HAL::Power_GetPowerOffCause()
{
    if (systemInfo.powerMonitor.LinuxPowerOff) {
        return "Shutdown Board";
    } else if (systemInfo.powerMonitor.LowBatteryPowerOff) {
        return "Shutdown LowBattery";
    } else if (FORCE_SHUTDOWN())
        return "Shutdown Force";
    return "Shutdown PushKey";
}