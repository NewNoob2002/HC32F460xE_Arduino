#include "HAL.h"
#include "Arduino.h"
#include "bq40z50.h"
#include "mp2762a.h"

#define FORCE_SHUTDOWN() (systemInfo.powerMonitor.Force_ShutDown)

static bool POWER_GET_REASON   = false;
en_flag_status_t softwareReset = RESET;

void HAL::Power_Init()
{
    softwareReset = RMU_GetStatus(RMU_FLAG_SW);
    if (softwareReset == SET) {
        pinMode(POWER_CONTROL_PIN, OUTPUT, HIGH);
        CORE_DEBUG_PRINTF("Power: GetResetause:Software Reset");
    } else {
        pinMode(POWER_CONTROL_PIN, OUTPUT);
        CORE_DEBUG_PRINTF("Power: GetResetause:PowerOn Reset");
        CORE_DEBUG_PRINTF("Power: Waiting Keep Press...");
    }
    pinMode(POWER_LED_PIN, OUTPUT);
    pinMode(FUNCTION_LED_PIN, OUTPUT);
    pinMode(CHARGE_LED_PIN, OUTPUT);
    pinMode(WATCHDOG_FEED_PIN, OUTPUT);
    Led_Init();
}

void HAL::Power_OnCheck()
{
		if(!systemInfo.online_device.mp2762)
		{
			Charger_Control_GPIO_Init();
			USB_Switch_GPIO_Init();
			USB_Switch_GPIO_Control(1);
		}
    while (true) {
        HAL_Update();
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
    CORE_DEBUG_PRINTF("[%d] Power: Done\n", millis());
    digitalWrite(POWER_LED_PIN, HIGH);
    digitalWrite(FUNCTION_LED_PIN, HIGH);
    digitalWrite(POWER_CONTROL_PIN, HIGH);
		if(!systemInfo.online_device.mp2762)
		{
			uint8_t typec_power = digitalRead(CHARGER_ADC_DETECT_PIN);
			if (!typec_power)
			{
				USB_Switch_GPIO_Control(0);
			}
		}
}

void HAL::Power_Shutdown(bool en)
{
    if (!en) {
        CORE_DEBUG_PRINTF("Shutdown Req");
        systemInfo.powerMonitor.ShutdownReq = true;
        digitalWrite(CHARGE_LED_PIN, LOW);
        digitalWrite(POWER_LED_PIN, HIGH);
        digitalWrite(FUNCTION_LED_PIN, HIGH);
        systemInfo.powerMonitor.poweroff_flag = 1;
    } else {
        CORE_DEBUG_PRINTF("Shutdown Done");
        systemInfo.powerMonitor.ShutdownReq = false;
        digitalWrite(POWER_CONTROL_PIN, LOW);
    }
}

void HAL::Power_PowerOffMonitor()
{
    if (!POWER_GET_REASON) {
        POWER_GET_REASON = true;
        CORE_DEBUG_PRINTF("POWER OFF .................Power Off Cause: %s", Power_GetPowerOffCause());
    }
}

bool HAL::Power_ShutdownEnsure()
{
    if (systemInfo.powerMonitor.ShutdownEnsure) {
        systemInfo.powerMonitor.ShutdownEnsure = false;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownForce()
{
    if (systemInfo.powerMonitor.Force_ShutDown) {
        systemInfo.powerMonitor.Force_ShutDown = false;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownLinux()
{
    if (systemInfo.powerMonitor.LinuxPowerOff) {
        systemInfo.powerMonitor.LinuxPowerOff = false;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownLowBattery()
{
    if (systemInfo.powerMonitor.LowBatteryPowerOff) {
        systemInfo.powerMonitor.LowBatteryPowerOff = false;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownSoftReset()
{
    if (systemInfo.powerMonitor.reset_flag) {
        systemInfo.powerMonitor.reset_flag = false;
        return true;
    }
    return false;
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

void HAL::Power_Update()
{
    if (millis() - systemInfo.powerMonitor.BatteryLastHandleTime >= 3000) {
        WatchDog_Feed();
        systemInfo.powerMonitor.BatteryLastHandleTime = millis();
        if (!systemInfo.online_device.bq40z50) {
            while (1) {
                CM_EXECUTE_INTERVAL(digitalToggle(POWER_LED_PIN), 500);
            }
        } else {
            checkBatteryInfo(&systemInfo.powerMonitor.batteryInfo);
        }
    }
    if (millis() - systemInfo.powerMonitor.ChargerLastHandleTime >= 500) {
        systemInfo.powerMonitor.ChargerLastHandleTime = millis();
        if (!systemInfo.online_device.mp2762) {
            Charger_Control_Monitor(&systemInfo.powerMonitor.batteryInfo);
        } else {
            charger_update(&systemInfo.powerMonitor.batteryInfo);
        }
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

void Charger_Control_GPIO_Init(void)
{
    pinMode(CHARGER_ENABLE_PIN, OUTPUT);
    pinMode(CHARGER_SWITCH_PIN, OUTPUT);

    pinMode(CHARGER_CTRL_PIN, INPUT_PULLUP);
    pinMode(CHARGER_CTRL_FAST_PIN, INPUT_PULLUP);
    pinMode(CHARGER_ADC_DETECT_PIN, INPUT_PULLUP);
}

void USB_Switch_GPIO_Init(void)
{
	pinMode(USB_SWITCH_PIN, OUTPUT);
}

void USB_Switch_GPIO_Control(uint8_t state)
{
	digitalWrite(USB_SWITCH_PIN, state==1 ? HIGH:LOW);
}

void Charge_Enable_Switch(uint8_t state)
{
	digitalWrite(CHARGER_ENABLE_PIN, state==1 ? HIGH:LOW);
}

void Charge_Current_Select(uint16_t select)
{
  if(3000 == select)
    digitalWrite(CHARGER_SWITCH_PIN, HIGH);
  else
    digitalWrite(CHARGER_SWITCH_PIN, LOW);
}

void Charger_Control_Monitor(BatteryInfo_t *batteryState)
{
	static uint8_t Charger_Monitor_Count = 0;
  int charge_detect = 0, fast_detect = 0, typec_detect = 0;

  charge_detect = digitalRead(CHARGER_CTRL_PIN);
	fast_detect = digitalRead(CHARGER_CTRL_FAST_PIN);
  typec_detect = digitalRead(CHARGER_ADC_DETECT_PIN);
	//printf("charge_detect: %d, fast_detect:%d, typec_detect:%d\n", charge_detect, fast_detect, typec_detect);

  if (charge_detect)
    Charger_Monitor_Count++;
  else
    Charger_Monitor_Count = 0;
	
  if (Charger_Monitor_Count >= 3)
  {
		Charge_Enable_Switch(1);
		Charge_Current_Select(3000);

		if( digitalRead(CHARGER_ENABLE_PIN) == 1)
			batteryState->chargeStatus = fastCharge;
		else
			batteryState->chargeStatus = normalCharge;
  }
  else
  {
		
		batteryState->chargeStatus = notCharge;
    Charge_Enable_Switch(0);
  }
}