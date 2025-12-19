#include "HAL.h"
#include "Arduino.h"
#include "ButtonEvent/ButtonEvent.h"

static ButtonEvent PowerKey;
static ButtonEvent FuncKey;
static volatile uint8_t ForceShutdown_count= 0;

static bool EncoderEnable           = true;
static volatile int32_t EncoderDiff = 0;
static bool EncoderDiffDisable      = false;

static void PowerKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_PRESSING:
//            CORE_DEBUG_PRINTF("[PowerKey] PRESSING\n");
            break;
        case ButtonEvent::EVENT_CLICKED:

//						CORE_DEBUG_PRINTF("[PowerKey] Single Click\n");
						break;
				case ButtonEvent::EVENT_DOUBLE_CLICKED:
//						CORE_DEBUG_PRINTF("[PowerKey] Double Click\n");
						break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
            break;
    }
}

static void FuncKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_DOUBLE_CLICKED:
						EncoderDiff++;
//            CORE_DEBUG_PRINTF("[FuncKey] Double Click\n");
            break;
        case ButtonEvent::EVENT_CLICKED:
						EncoderDiff--;
//            CORE_DEBUG_PRINTF("[FuncKey] Single Click\n");
            break;
        case ButtonEvent::EVENT_RELEASED:
//            CORE_DEBUG_PRINTF("[FuncKey] Realse\n");
						ForceShutdown_count = 0;
            break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
					  ForceShutdown_count++;
            if (ForceShutdown_count >= 5)
                systemInfo.powerMonitor.Force_ShutDown = true;
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
//    PowerKey.EventMonitor(digitalRead(POWER_KEY_PIN) == HIGH);
    FuncKey.EventMonitor(digitalRead(FUNCTION_KEY_PIN) == LOW);
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

    return digitalRead(POWER_KEY_PIN) == HIGH;
}