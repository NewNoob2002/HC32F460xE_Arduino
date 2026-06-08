#include "HAL.h"
#include "Arduino.h"
#include "bq40z50.h"
#include "mp2762a.h"
#include "ots.h"

#define FORCE_SHUTDOWN() (systemInfo.powerMonitor.Force_ShutDown)

en_flag_status_t softwareReset = RESET;

void ots_callback(void)
{
	systemInfo.powerMonitor.batteryInfo.fOtsTemp = OTS_CalculateTemp();
}

void HAL::Power_Init()
{
    softwareReset = RMU_GetStatus(RMU_FLAG_SW);
    if (softwareReset == SET) {
        pinMode(POWER_CONTROL_PIN, OUTPUT, HIGH);
        digitalWrite(POWER_CONTROL_PIN, HIGH);
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
    Bat_Comp_Init(&systemInfo.powerMonitor.batteryInfo.Compensate);
		OtsInitConfig(ots_callback);
}

void HAL::Power_OnCheck()
{
    if (!systemInfo.online_device.mp2762) {
        Charger_Control_GPIO_Init();
        USB_Switch_GPIO_Init();
        USB_Switch_GPIO_Control(1);
    }
    while (true) {
        HAL_Update();
        if (systemInfo.powerMonitor.panel_power_on) {
            CORE_DEBUG_PRINTF("MCU PowerDone");
            break;
        }
        if (softwareReset) {
            CORE_DEBUG_PRINTF("softwareRst PowerDone");
            break;
        }
    }
    systemInfo.powerMonitor.panel_power_on = 1;
    CORE_DEBUG_PRINTF("Power: Done");
    digitalWrite(POWER_LED_PIN, HIGH);
    digitalWrite(FUNCTION_LED_PIN, HIGH);
    digitalWrite(POWER_CONTROL_PIN, HIGH);
    if (!systemInfo.online_device.mp2762) {
        uint8_t typec_power = digitalRead(CHARGER_ADC_DETECT_PIN);
        if (!typec_power) {
            USB_Switch_GPIO_Control(0);
        }
    }
		delay_ms(1000);
}

void HAL::Power_Shutdown(bool en)
{
    if (!en) {
        CORE_DEBUG_PRINTF("Shutdown Req");
        systemInfo.powerMonitor.ShutdownReq = 1;
    } else {
        CORE_DEBUG_PRINTF("Shutdown Done");
        digitalWrite(POWER_CONTROL_PIN, LOW);
    }
}

void HAL::Power_PowerOffMonitor()
{
    static uint8_t POWER_GET_REASON = 0;
    if (systemInfo.powerMonitor.ShutdownReq || systemInfo.powerMonitor.LowBatteryPowerOff) {
        digitalWrite(CHARGE_LED_PIN, LOW);
        digitalWrite(POWER_LED_PIN, HIGH);
        digitalWrite(FUNCTION_LED_PIN, HIGH);
        systemInfo.powerMonitor.poweroff_flag            = 1;
        systemInfo.powerMonitor.ShutdownReq              = 0;
        systemInfo.powerMonitor.LowBatteryPowerOff       = 0;
        systemInfo.powerMonitor.pannel_shutdown_req_time = millis();
    }
    if (systemInfo.powerMonitor.ShutdownEnsure || FORCE_SHUTDOWN()) {
        if (!POWER_GET_REASON) {
            POWER_GET_REASON                                 = 1;
            systemInfo.powerMonitor.pannel_shutdown_req_time = millis();
            digitalWrite(CHARGE_LED_PIN, LOW);
            digitalWrite(POWER_LED_PIN, HIGH);
            digitalWrite(FUNCTION_LED_PIN, HIGH);
            CORE_DEBUG_PRINTF("POWER OFF .................Power Off Cause: %s", Power_GetPowerOffCause());
        }
        if (millis() - systemInfo.powerMonitor.pannel_shutdown_req_time >= 8000) {
            Power_Shutdown(true);
            delay_ms(1000);
            NVIC_SystemReset();
        }
    } else if (systemInfo.powerMonitor.poweroff_flag) {
        if (millis() - systemInfo.powerMonitor.pannel_shutdown_req_time >= 20000) {
            Power_Shutdown(true);
            delay_ms(1000);
            NVIC_SystemReset();
        }
    }
}

bool HAL::Power_ShutdownEnsure()
{
    if (systemInfo.powerMonitor.ShutdownEnsure) {
        systemInfo.powerMonitor.ShutdownEnsure = 0;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownForce()
{
    if (systemInfo.powerMonitor.Force_ShutDown) {
        systemInfo.powerMonitor.Force_ShutDown = 0;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownLinux()
{
    if (systemInfo.powerMonitor.LinuxPowerOff) {
        systemInfo.powerMonitor.LinuxPowerOff = 0;
        return true;
    }
    return false;
}

bool HAL::Power_ShutdownLowBattery()
{
    if (systemInfo.powerMonitor.LowBatteryPowerOff) {
        systemInfo.powerMonitor.LowBatteryPowerOff = 0;
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

void HAL::Power_Update()
{
    WatchDog_Feed();
		OtsStart();
    Bat_Comp_CalcOffset(&systemInfo.powerMonitor.batteryInfo);
    Power_GetInfo(&systemInfo.powerMonitor);
}

void HAL::Power_EnableCharger(BatteryInfo_t *pBatteryState)
{
	if(pBatteryState->ChargerDisable)
	{
		if(systemInfo.online_device.mp2762)
		{
			//To do MP2762
			uint8_t cfg0 = mp2762enableCharger();
			cfg0 &= (1 <<4);
			if(cfg0)pBatteryState->ChargerDisable = 0;
		}
		else
		{
		}
	}
}

void HAL::Power_DisableCharger(BatteryInfo_t *pBatteryState)
{
	if(pBatteryState->ChargerDisable == 0)
	{
		if(systemInfo.online_device.mp2762)
		{
				uint8_t cfg0 = mp2762disableCharger();
				cfg0 &= (1 << 4);
				if(!cfg0)pBatteryState->ChargerDisable = 1;
		}
		else
		{
			if(pBatteryState->chargeStatus != notCharge)
			{
				pBatteryState->chargeStatus = notCharge;
				Charge_Enable_Switch(0);
				pBatteryState->ChargerDisable = 1;
			}
		}
	}
}

void HAL::Power_GetInfo(Power_Monitor_t *info)
{
    if (info == nullptr) return;

    if (millis() - systemInfo.powerMonitor.BatteryLastHandleTime >= 3000) {
        systemInfo.powerMonitor.BatteryLastHandleTime = millis();
        if (!systemInfo.online_device.bq40z50) {
						digitalToggle(POWER_LED_PIN);
        } else {
            checkBatteryInfo(&info->batteryInfo);
        }
    }
		info->batteryInfo.ChargerDetect = digitalRead(CHARGER_CTRL_PIN);
		BatteryTemp_Monitor(&info->batteryInfo);   //Battery Temperature Check
    if (systemInfo.online_device.mp2762) {
				ChargerMp2762_update(&info->batteryInfo);
    } else {
				Charger_Control_Monitor(&info->batteryInfo);
    }
}

void HAL::WatchDog_Feed()
{
    if (millis() - systemInfo.powerMonitor.WatchDogLastFeedTime >= WATCHDOG_FEED_TIME) {
        systemInfo.powerMonitor.WatchDogLastFeedTime = millis();
        digitalToggle(WATCHDOG_FEED_PIN);
    }
}

bool ChagrerMp2762_begin(pBatteryInfo_t p_batteryState)
{
    // Reset all configure
    mp2762registerReset();
    // Set Charger Current
    mp2762setFastChargeVoltageMv(6600);
    // Setting precharge current to 880mA
    mp2762setPrechargeCurrentMa(880);
    // Setting fast charge current to 1600mA
    p_batteryState->ChargerCurrent = mp2762setFastChargeCurrentMa(1600);
    // get charge status
    uint8_t charge_status        = mp2762getChargeStatus();
    p_batteryState->chargeStatus = static_cast<Charger_Status_t>(charge_status);

    return true;
}

void ChargerMp2762_update(pBatteryInfo_t p_batteryState)
{
    const uint8_t charge_status = mp2762getChargeStatus();
    if (CM_VALUE_IN_RANGE(charge_status, 0, 2)) {
        p_batteryState->chargeStatus = static_cast<Charger_Status_t>(charge_status);
    }
		mp2762updateConfig0Status(&p_batteryState->mp2762_cfg0);
}

void checkBatteryInfo(pBatteryInfo_t p_batteryState)
{
    uint16_t batteryLevelPercent = bq40z50getRelativeStateOfCharge();
    float batteryVoltage         = (bq40z50getVoltageMv() / 1000.0);
    uint16_t batteryTempC        = bq40z50getTemperatureC(&p_batteryState->fTemp);

    p_batteryState->Actual_Percent = batteryLevelPercent;
    batteryLevelPercent            = batteryLevelPercent + p_batteryState->Compensate.comp_offset;

    p_batteryState->Percent           = CM_SET_VALUE_IN_RANGE(batteryLevelPercent, 0, 100);
    p_batteryState->Processed_Percent = batteryLevelPercent;
    p_batteryState->Voltage           = CM_SET_VALUE_IN_RANGE((uint16_t)batteryVoltage, 6, 9);
    p_batteryState->Temp              = CM_SET_VALUE_IN_RANGE(batteryTempC, 1, 65535);
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
    digitalWrite(USB_SWITCH_PIN, state == 1 ? HIGH : LOW);
}

void Charge_Enable_Switch(uint8_t state)
{
    digitalWrite(CHARGER_ENABLE_PIN, state == 1 ? HIGH : LOW);
}

void Charge_Current_Select(uint16_t select)
{
    if (3000 == select)
        digitalWrite(CHARGER_SWITCH_PIN, HIGH);
    else
        digitalWrite(CHARGER_SWITCH_PIN, LOW);
}

void Charger_Control_Monitor(BatteryInfo_t *batteryState)
{
    if (batteryState->ChargerDetect)
		{
			if(batteryState->ChargerPlugCount < 5)batteryState->ChargerPlugCount++;
		}
    else
		{
			batteryState->ChargerPlugCount = 0;
		}
		
		if(batteryState->ChargerDisable){
			return;
		}

    if (batteryState->ChargerPlugCount >= 3) {
        Charge_Enable_Switch(1);
        Charge_Current_Select(3000);

        if (digitalRead(CHARGER_ENABLE_PIN) == 1)
            batteryState->chargeStatus = fastCharge;
        else
            batteryState->chargeStatus = normalCharge;
    } else {
        batteryState->chargeStatus = notCharge;
        Charge_Enable_Switch(0);
    }
}

void BatteryTemp_Monitor(pBatteryInfo_t pBatteryState)
{
//	if(pBatteryState->fTemp >= BATTERY_TERMINATE_TEMP || pBatteryState->fOtsTemp >= BATTERY_TERMINATE_TEMP)
	if(pBatteryState->fTemp >= BATTERY_TERMINATE_TEMP)
	{
		if(pBatteryState->ChargerOverTempCount<= 50)pBatteryState->ChargerOverTempCount++;
		if(pBatteryState->ChargerOverTempCount>= 20)pBatteryState->isOverTemp = 1;
	}
	else
	{
		pBatteryState->ChargerOverTempCount = 0;
		pBatteryState->isOverTemp = 0;
	}
	
	if(pBatteryState->isOverTemp)
	{
		HAL::Power_DisableCharger(pBatteryState);
	}
	else
	{
		HAL::Power_EnableCharger(pBatteryState);
	}
}

void Bat_Comp_Init(Battery_Compensate_t *bat_comp)
{
    if (bat_comp == nullptr) return;
    bat_comp->is_comp_updated = 0;
    bat_comp->last_soc        = 0;
    bat_comp->stable_timer    = 0;

    uint8_t offset        = Flash_Read_CompOffset();
    bat_comp->comp_offset = offset;
}

void Bat_Comp_CalcOffset(BatteryInfo_t *batteryState)
{
    if (batteryState == nullptr || batteryState->Compensate.is_comp_updated == 1) return;

    uint8_t IsCharge = batteryState->chargeStatus != notCharge;

    if (IsCharge) {
        if (batteryState->Actual_Percent == batteryState->Compensate.last_soc) {
            batteryState->Compensate.stable_timer += 500; // 500ms

            if (batteryState->Compensate.stable_timer >= STABLE_TIME_MS) {
                batteryState->Compensate.comp_offset = 100 - batteryState->Actual_Percent;
                if (batteryState->Compensate.comp_offset < 10) {
                    Flash_Write_CompOffset(batteryState->Compensate.comp_offset);
                    batteryState->Compensate.is_comp_updated = 1;
                }
                batteryState->Compensate.stable_timer = 0;
            }
        } else {
            batteryState->Compensate.stable_timer = 0;
            batteryState->Compensate.last_soc     = batteryState->Actual_Percent;
        }
    } else {
        batteryState->Compensate.stable_timer    = 0;
        batteryState->Compensate.is_comp_updated = 0;
    }
}

uint8_t Flash_Write_CompOffset(uint8_t offset)
{
    /* EFM_FWMC write enable */
    int32_t ret   = LL_ERR;
    ret           = FlashEraseSector(FLASH_OFFSET_ADDR);
    uint32_t temp = offset;
    if (ret == LL_OK) {
        ret = FlashWriteData(FLASH_OFFSET_ADDR, (uint8_t *)&temp, 4);
        if (ret == LL_OK) {
            return 1;
        }
    }
    EFM_FWMC_Cmd(DISABLE);
    return 0;
}

uint8_t Flash_Read_CompOffset(void)
{
    uint32_t flash_data = *(__IO uint32_t *)FLASH_OFFSET_ADDR;

    if (flash_data != 0xFFFFFFFF) {
        return (uint8_t)flash_data;
    } else
        return 0;
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