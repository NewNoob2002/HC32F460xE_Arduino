#include "HAL.h"
#include "Arduino.h"
#include "ButtonEvent/ButtonEvent.h"

static ButtonEvent PowerKey;
static ButtonEvent FuncKey;
static volatile uint8_t ForceShutdown_powerkey = 0;
static volatile uint8_t ForceShutdown_funckey = 0;

static void PowerKey_callback(ButtonEvent *btn, int event)
{
	switch (event)
	{
	case ButtonEvent::EVENT_PRESSING:
			{
				if(!systemInfo.powerMonitor.panel_power_on)
				{
					systemInfo.powerMonitor.PowerKey_PressCount++;
				}
				else if(( systemInfo.online_device.eg25_board || systemInfo.eg25_overtime) && !systemInfo.powerMonitor.Force_ShutDown_Funckey)
				{
					systemInfo.powerMonitor.PowerKey_PressCount++;
				}
				break;
			}
	case ButtonEvent::EVENT_RELEASED:
		systemInfo.powerMonitor.PowerKey_PressCount = 0;
		ForceShutdown_powerkey = 0;
		systemInfo.powerMonitor.Force_ShutDown_Powerkey = false;
		break;
	case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
	{
		ForceShutdown_powerkey++;
		if (ForceShutdown_powerkey >= 4)
			systemInfo.powerMonitor.Force_ShutDown_Powerkey = true;
		break;
	}
	}
}

static void FuncKey_callback(ButtonEvent *btn, int event)
{
    switch (event)
    {
    case ButtonEvent::EVENT_DOUBLE_CLICKED:
        break;
    case ButtonEvent::EVENT_CLICKED:

    case ButtonEvent::EVENT_RELEASED:
        CORE_DEBUG_PRINTF("[FuncKey] Realse\n");
				ForceShutdown_funckey = 0;
				systemInfo.powerMonitor.Force_ShutDown_Funckey = false;
        break;
		case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
				ForceShutdown_funckey++;
				CORE_DEBUG_PRINTF("[FuncKey] Long_Repeat, %d\n",ForceShutdown_funckey);
				if(ForceShutdown_funckey >= 5)systemInfo.powerMonitor.Force_ShutDown_Funckey = true;
        break;
    }
}

void HAL::Key_Init()
{
	CORE_DEBUG_PRINTF("KEY: Init\n");
  pinMode(POWER_KEY_PIN, INPUT_PULLUP);
	pinMode(FUNCTION_KEY_PIN, INPUT_PULLUP);
	
	PowerKey.EventAttach(PowerKey_callback);
	FuncKey.EventAttach(FuncKey_callback);
}

void HAL::Key_Update()
{
	PowerKey.EventMonitor(digitalRead(POWER_KEY_PIN) == HIGH);
	FuncKey.EventMonitor(digitalRead(FUNCTION_KEY_PIN) == LOW);
}