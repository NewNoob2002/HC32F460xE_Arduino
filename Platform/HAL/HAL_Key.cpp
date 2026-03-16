#include "HAL.h"
#include "Arduino.h"
#include "multi_button.h"

// static ButtonEvent PowerKey;
// static ButtonEvent FuncKey;

static Button PowerKey, FuncKey;

static volatile uint8_t ForceShutdown_count = 0;
static volatile uint16_t PowerKeyPressCount = 0;

static void PowerKey_Pressing_Callback(Button *btn)
{
    CORE_DEBUG_PRINTF("[PowerKey] PRESSING\n");
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

static void PowerKey_Realse_Callback(Button *btn)
{
    CORE_DEBUG_PRINTF("[PowerKey] REALSE\n");
    PowerKeyPressCount = 0;
}

static void FuncKey_DoubleClick_Callback(Button *btn)
{
    CORE_DEBUG_PRINTF("[FuncKey] Double Click\n");
    systemInfo.recordInfo.record_op = 1;
    if (systemInfo.recordInfo.record_status == 1) {
        systemInfo.recordInfo.record_status = 0;
    } else {
        systemInfo.recordInfo.record_status = 1;
    }
}

static void FuncKey_LongPressRepeat_Callback(Button *btn)
{
    CORE_DEBUG_PRINTF("[FuncKey] LongPressRepeat\n");
    ForceShutdown_count++;
    if (ForceShutdown_count >= 10)
        systemInfo.powerMonitor.Force_ShutDown = true;
}

static void FuncKey_Realse_Callback(Button *btn)
{
    CORE_DEBUG_PRINTF("[FuncKey] REALSE\n");
    ForceShutdown_count = 0;
}

uint8_t read_button_gpio(uint8_t button_id)
{
    switch (button_id) {
        case 1:
            return digitalRead(POWER_KEY_PIN);
        case 2:
            return digitalRead(FUNCTION_KEY_PIN);
        default:
            return 0;
    }
}

void HAL::Key_Init()
{
    CORE_DEBUG_PRINTF("KEY: Init");
    pinMode(POWER_KEY_PIN, INPUT_PULLUP);
    pinMode(FUNCTION_KEY_PIN, INPUT_PULLUP);

    // Initialize button 1 (active high for simulation)
    button_init(&PowerKey, read_button_gpio, 1, 1);

    // Attach event handlers for button 1
    button_attach(&PowerKey, BTN_LONG_PRESS_HOLD, PowerKey_Pressing_Callback);
    button_attach(&PowerKey, BTN_PRESS_UP, PowerKey_Realse_Callback);

    // Initialize button 2 (active high for simulation)
    button_init(&FuncKey, read_button_gpio, 0, 2);

    // Attach event handlers for button 2
    button_attach(&FuncKey, BTN_DOUBLE_CLICK, FuncKey_DoubleClick_Callback);
    button_attach(&FuncKey, BTN_LONG_PRESS_START, FuncKey_LongPressRepeat_Callback);
    button_attach(&FuncKey, BTN_PRESS_UP, FuncKey_Realse_Callback);

    // Start button processing
    button_start(&PowerKey);
    button_start(&FuncKey);
}

void HAL::Key_Update()
{
    button_ticks();
    //    PowerKey.EventMonitor(digitalRead(POWER_KEY_PIN) == HIGH);
    //    FuncKey.EventMonitor(digitalRead(FUNCTION_KEY_PIN) == LOW);
}