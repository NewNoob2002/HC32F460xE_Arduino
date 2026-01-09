#include "Arduino.h"
#include "HAL.h"

ledState_t powerLed = {false, 0, 0};
ledState_t chargerLed = {false, 0, 0};
ledState_t functionKeyLed = {false, 0, 0};

static bool lowBattery_flag = false;
static uint32_t lowBattery_tick = 0;

void Led_Init()
{
    pinMode(POWER_LED_PIN, OUTPUT, LOW);
    pinMode(CHARGE_LED_PIN, OUTPUT, LOW);
    pinMode(FUNCTION_LED_PIN, OUTPUT, LOW);
}

// Led Control interface
void Led_Power_switch(const uint8_t level)
{
    digitalWrite(POWER_LED_PIN, level);
}

void Led_Charge_switch(const uint8_t level)
{
    digitalWrite(CHARGE_LED_PIN, level);
}

void Led_Function_switch(const uint8_t level)
{
    digitalWrite(FUNCTION_LED_PIN, level);
}
// Led Update
void Led_Update_Power()
{
    if (powerLed.currentRate > 0)
    {
        const uint32_t now = millis();
        if (now - powerLed.lastToggleTime >= powerLed.currentRate)
        {
            powerLed.lastToggleTime = now;
            digitalToggle(POWER_LED_PIN);
        }
    }
}

// Led Update
void Led_Update_Charge()
{
		if(systemInfo.powerMonitor.ShutdownReq)
			return;
					
    if (chargerLed.currentRate > 0)
    {
        const uint32_t now = millis();
        if (now - chargerLed.lastToggleTime >= chargerLed.currentRate)
        {
            chargerLed.lastToggleTime = now;
            digitalToggle(CHARGE_LED_PIN);
        }
    }

    if (systemInfo.powerMonitor.batteryInfo.chargeStatus != 0x00)
    {
				if(lowBattery_flag)
				{
					lowBattery_flag = false;
				}
				digitalWrite(POWER_LED_PIN, LOW);
        powerLed.currentRate = 0;
        bool isFullCharge = (systemInfo.powerMonitor.batteryInfo.Percent >= 100);
        if (isFullCharge)
        {
            chargerLed.currentRate = 0;
            digitalWrite(CHARGE_LED_PIN, HIGH);
        }
        else
        {
            if (systemInfo.powerMonitor.batteryInfo.chargeStatus == 0x02)
            {
                chargerLed.currentRate = 500;
            }
            else
            {
                chargerLed.currentRate = 1000;
            }
        }
    }
    else
    {
        digitalWrite(CHARGE_LED_PIN, LOW);
        chargerLed.currentRate = 0;
				uint16_t Percent = systemInfo.powerMonitor.batteryInfo.Percent;
			  if (Percent >= 14)
        {
            digitalWrite(POWER_LED_PIN, HIGH);
        }
        else if ((Percent < 14) && (Percent > 9))
        {
            powerLed.currentRate = 1000;
        }
        else if ((Percent <= 9) && (Percent > 0))
        {
            powerLed.currentRate = 300;
            if (systemInfo.recordInfo.record_status)
            {
                systemInfo.recordInfo.record_status = On_Off_Status_OFF;
                systemInfo.recordInfo.record_op = 1;
            }
						if(!lowBattery_flag)
						{
							lowBattery_flag = true;
							lowBattery_tick = millis();
						}
            if (millis() - lowBattery_tick >= 60000)
            {
								lowBattery_tick = millis();
                systemInfo.powerMonitor.LowBatteryPowerOff = true;
            }
        }
    }
}

// Led Update
void Led_Update_Function()
{
		if(systemInfo.powerMonitor.ShutdownReq || !systemInfo.online_device.eg25_board)
			return;
    /*Monitor*/
    if (functionKeyLed.currentRate > 0)
    {
        const uint32_t now = millis();
        if (now - functionKeyLed.lastToggleTime >= functionKeyLed.currentRate)
        {
            functionKeyLed.lastToggleTime = now;
            digitalToggle(FUNCTION_LED_PIN);
        }
    }

		if (systemInfo.recordInfo.record_status)
    {
        functionKeyLed.currentRate = 500;
    }
    else
    {
        digitalWrite(FUNCTION_LED_PIN, LOW);
        functionKeyLed.currentRate = 0;
    }
}
// Led Update
void Led_Update()
{
    Led_Update_Power();
    Led_Update_Charge();
    Led_Update_Function();
}