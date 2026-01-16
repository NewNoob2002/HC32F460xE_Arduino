#include "DataProc.h"
#include "../HAL/HAL.h"
#if defined(_WIN32)
#else
#include "Arduino.h"
#endif

static void onTimer(Account *account)
{
#if defined(_WIN32)
#else
    Power_Monitor_t power;
    HAL::Power_GetInfo(&power);
    HAL::Power_PowerOffMonitor();

    account->Commit(&power, sizeof(power));
    account->Publish();

    DataProc::Led_Info_t info;
    DATA_PROC_INIT_STRUCT(info);
		info.cmd = DataProc::LED_CMD_BLINK;
    if (power.batteryInfo.chargeStatus != 0x00) {
        info.param.powerLed_blink_interval = 0;
        digitalWrite(POWER_LED_PIN, LOW);
        bool isFullCharge = (power.batteryInfo.Percent >= 100);
				if(isFullCharge)
				{
					log_d("Charing Full");
					info.param.charge_blink_interval = 0;
					digitalWrite(CHARGE_LED_PIN, HIGH);
				}
				else
				{
					log_d("Charing NotFull");
					if(power.batteryInfo.chargeStatus == 0x01)
					{
						info.param.charge_blink_interval = 300;
					}
					else
					{
						info.param.charge_blink_interval = 1000;
					}
				}

    }
		else
		{
//			log_d("Not Chager");
			info.param.charge_blink_interval = 0;
      digitalWrite(CHARGE_LED_PIN, LOW);
			if(power.batteryInfo.Percent >= 14)
			{
				digitalWrite(POWER_LED_PIN, HIGH);
				info.param.powerLed_blink_interval = 0;
			}
			else if(power.batteryInfo.Percent < 14 && power.batteryInfo.Percent >= 9)
			{
				info.param.powerLed_blink_interval = 1000;
			}
			else if(power.batteryInfo.Percent < 9 && power.batteryInfo.Percent >= 0)
			{
				info.param.powerLed_blink_interval = 300;
				if(systemInfo.recordInfo.record_status)
				{
					systemInfo.recordInfo.record_status = On_Off_Status_OFF;
					systemInfo.recordInfo.record_op = 1;
				}
				systemInfo.powerMonitor.batteryInfo.LowBatteryCount ++;
				if (30 * 10 == systemInfo.powerMonitor.batteryInfo.LowBatteryCount)
        {
					systemInfo.powerMonitor.LowBatteryPowerOff = true;
					systemInfo.powerMonitor.batteryInfo.LowBatteryCount = 0 ;
        }
			}
		}
		account->Notify("Led", &info, sizeof(info));
#endif
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

    Power_Monitor_t powerInfo;
    HAL::Power_GetInfo(&powerInfo);
    memcpy(param->data_p, &powerInfo, param->size);

    return Account::RES_OK;
}

DATA_PROC_INIT_DEF(Power)
{
    account->Subscribe("Led");
    account->SetEventCallback(onEvent);
    account->SetTimerPeriod(500);
}
