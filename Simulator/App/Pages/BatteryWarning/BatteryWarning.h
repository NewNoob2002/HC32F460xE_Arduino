#ifndef BATTERY_WARNING_H
#define BATTERY_WARNING_H

#include "lvgl.h"

namespace Page {

lv_obj_t* BatteryWarning_Create(lv_obj_t* parent);
void BatteryWarning_Update();
void BatteryWarning_ApplyLanguage();

} // namespace Page

#endif // BATTERY_WARNING_H
