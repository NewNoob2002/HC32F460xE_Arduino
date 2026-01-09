#include "HAL.h"
#include "Arduino.h"
#include "ButtonEvent/ButtonEvent.h"

static ButtonEvent PowerKey;
static ButtonEvent FuncKey;
static volatile uint8_t ForceShutdown_powerkey = 0;

static void PowerKey_callback(ButtonEvent *btn, int event)
{
	switch (event)
	{
	case ButtonEvent::EVENT_PRESSING:
			{
				break;
			}
	case ButtonEvent::EVENT_RELEASED:
		ForceShutdown_powerkey = 0;
		systemInfo.powerMonitor.Force_ShutDown = false;
		break;
	case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
	{
		ForceShutdown_powerkey++;
		if (ForceShutdown_powerkey >= 8)
			systemInfo.powerMonitor.Force_ShutDown = true;
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
				break;
		default:
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