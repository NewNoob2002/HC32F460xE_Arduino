#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>
#include "styles.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*arc_completion_callback_t)(void);
	
typedef struct {
    lv_obj_t *chart;
    lv_chart_series_t *voltage_series;
    lv_chart_series_t *current_series;
} battery_chart_t;
	
// enum declarations
extern uint8_t BATT_USAGE_HEIGHT;
extern uint8_t BATT_USAGE_WIDTH;
extern const uint8_t  radio_protocol_def[6];

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_NONE
};

// Native global variables



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/