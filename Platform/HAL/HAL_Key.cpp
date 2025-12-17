#include "HAL.h"
#include "Arduino.h"
#include "ButtonEvent/ButtonEvent.h"

static ButtonEvent PowerKey;
static ButtonEvent FuncKey;
static volatile uint8_t ForceShutdown_powerkey = 0;
static volatile uint8_t ForceShutdown_funckey  = 0;

static bool EncoderEnable           = true;
static volatile int32_t EncoderDiff = 0;
static bool EncoderDiffDisable      = false;

static void PowerKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_PRESSING:
            if (!systemInfo.powerMonitor.panel_power_on) {
                systemInfo.powerMonitor.PowerKey_PressCount++;
            } else if ((systemInfo.online_device.eg25_board || systemInfo.eg25_overtime) && !systemInfo.powerMonitor.Force_ShutDown_Funckey) {
                systemInfo.powerMonitor.PowerKey_PressCount++;
            }
            CORE_DEBUG_PRINTF("[PowerKey] PRESSING\n");
            break;
        case ButtonEvent::EVENT_CLICKED:
            EncoderDiff++;
						CORE_DEBUG_PRINTF("[PowerKey] Single Click\n");
						break;
				case ButtonEvent::EVENT_DOUBLE_CLICKED:
						EncoderDiff--;
						CORE_DEBUG_PRINTF("[PowerKey] Double Click\n");
						break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
            ForceShutdown_powerkey++;
						CORE_DEBUG_PRINTF("[PowerKey] Long_Repeat, %d\n", ForceShutdown_powerkey);
            if (ForceShutdown_powerkey >= 4)
                systemInfo.powerMonitor.Force_ShutDown_Powerkey = true;
            break;
        case ButtonEvent::EVENT_RELEASED:
            systemInfo.powerMonitor.PowerKey_PressCount     = 0;
            ForceShutdown_powerkey                          = 0;
            systemInfo.powerMonitor.Force_ShutDown_Powerkey = false;
						CORE_DEBUG_PRINTF("[PowerKey] Realse\n");
            break;
    }
}

static void FuncKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_DOUBLE_CLICKED:
            CORE_DEBUG_PRINTF("[FuncKey] Double Click\n");
            break;
        case ButtonEvent::EVENT_CLICKED:
            CORE_DEBUG_PRINTF("[FuncKey] Single Click\n");
            break;
        case ButtonEvent::EVENT_RELEASED:
            CORE_DEBUG_PRINTF("[FuncKey] Realse\n");
            ForceShutdown_funckey                          = 0;
            systemInfo.powerMonitor.Force_ShutDown_Funckey = false;
            break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
            ForceShutdown_funckey++;
            CORE_DEBUG_PRINTF("[FuncKey] Long_Repeat, %d\n", ForceShutdown_funckey);
            if (ForceShutdown_funckey >= 5) systemInfo.powerMonitor.Force_ShutDown_Funckey = true;
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
//    FuncKey.EventMonitor(digitalRead(FUNCTION_KEY_PIN) == LOW);
}

int32_t HAL::Encoder_GetDiff()
{
    int32_t diff = EncoderDiff;
    EncoderDiff  = 0;
    return diff;
}

bool HAL::Encoder_GetIsPush()
{
    if (!EncoderEnable) {
        return false;
    }

    return digitalRead(FUNCTION_KEY_PIN) == LOW;
}