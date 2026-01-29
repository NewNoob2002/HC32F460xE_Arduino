#include "HAL.h"
#include "Arduino.h"
#include "ButtonEvent/ButtonEvent.h"

static ButtonEvent PowerKey;
static ButtonEvent FuncKey;
static volatile uint8_t ForceShutdown_count = 0;
static volatile uint16_t PowerKeyPressCount = 0;

static void PowerKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_PRESSING:
            CORE_DEBUG_PRINTF("[PowerKey] PRESSING\n");
            {
                PowerKeyPressCount++;
                if (PowerKeyPressCount >= 200) {
                    PowerKeyPressCount = 0;
                    if (!systemInfo.powerMonitor.panel_power_on) {
                        systemInfo.powerMonitor.panel_power_on = 1;
                    } else {
                        PowerKeyPressCount = 0;
                        HAL::Power_Shutdown(false);
                    }
                }
            }
            break;
        case ButtonEvent::EVENT_CLICKED:

            CORE_DEBUG_PRINTF("[PowerKey] Single Click\n");
            break;
        case ButtonEvent::EVENT_DOUBLE_CLICKED:
            CORE_DEBUG_PRINTF("[PowerKey] Double Click\n");
            break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
            break;
    }
}

static void FuncKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_DOUBLE_CLICKED:
            CORE_DEBUG_PRINTF("[FuncKey] Double Click\n");
            {
                systemInfo.recordInfo.record_op = 1;
                if (systemInfo.recordInfo.record_status == 1) {
                    systemInfo.recordInfo.record_status = 0;
                } else {
                    systemInfo.recordInfo.record_status = 1;
                }
            }
            break;
        case ButtonEvent::EVENT_CLICKED:
            CORE_DEBUG_PRINTF("[FuncKey] Single Click\n");
            break;
        case ButtonEvent::EVENT_RELEASED:
            CORE_DEBUG_PRINTF("[FuncKey] Realse\n");
            ForceShutdown_count = 0;
            break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT:
            ForceShutdown_count++;
            if (ForceShutdown_count >= 10)
                systemInfo.powerMonitor.Force_ShutDown = true;
            break;
    }
}

void HAL::Key_Init()
{
    CORE_DEBUG_PRINTF("KEY: Init");
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