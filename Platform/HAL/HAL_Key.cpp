#include "HAL.h"
#include "Arduino.h"
#include "ButtonEvent/ButtonEvent.h"
#include "lvgl_screen.h"

static ButtonEvent PowerKey;
static ButtonEvent FuncKey;
static volatile uint8_t Shutdown_powerkey = 0;
static volatile uint8_t Shutdown_force    = 0;

static void PowerKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_DOUBLE_CLICKED: {
            if (screen_flag == 2) {
                change_workstatus();
                systemInfo.radioInfo.radio_change_flag = 1;
            }
            break;
        }
        case ButtonEvent::EVENT_RELEASED:
            Shutdown_powerkey                        = 0;
            systemInfo.powerMonitor.PushKey_ShutDown = false;
            break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT: {
            if (!systemInfo.powerMonitor.panel_power_on || screen_flag == 6) return;
            Shutdown_powerkey++;
            if (Shutdown_powerkey >= 8)
                systemInfo.powerMonitor.PushKey_ShutDown = true;
            break;
        }
				default:
            break;
    }
}

static void FuncKey_callback(ButtonEvent *btn, int event)
{
    switch (event) {
        case ButtonEvent::EVENT_DOUBLE_CLICKED:
            if (screen_flag == 4) {
                if (systemInfo.recordInfo.record_status == 1) {
                    systemInfo.recordInfo.record_status      = On_Off_Status_OFF;
                    systemInfo.recordInfo.record_op          = 1;
                    systemInfo.recordInfo.record_change_flag = 1;
                } else {
                    systemInfo.recordInfo.record_status      = On_Off_Status_ON;
                    systemInfo.recordInfo.record_op          = 1;
                    systemInfo.recordInfo.record_change_flag = 1;
                }
            }
            if (screen_flag == 2) {
                workstatus++;
                screnn2_time       = 0;
                work_status_enable = 0;
                if (workstatus >= 6) workstatus = 1;
                workstatus_change = 1;
            }
            break;
        case ButtonEvent::EVENT_SHORT_CLICKED:
            last_screen_flag   = screen_flag++;
            screen_flag_change = 1;
            if (screen_flag > SCREEN_PAGE_NUM) screen_flag = 1;
            break;
        case ButtonEvent::EVENT_RELEASED:
            Shutdown_force                        = 0;
            systemInfo.powerMonitor.PushKey_ShutDown = false;
            break;
        case ButtonEvent::EVENT_LONG_PRESSED_REPEAT: {
            Shutdown_force++;
            if (Shutdown_force >= 8)
                systemInfo.powerMonitor.Force_ShutDown = true;
            break;
        }
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