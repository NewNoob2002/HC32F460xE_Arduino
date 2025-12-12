#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif
const char* MakeTimeString(uint32_t ms, char* buf, uint16_t len);
#ifdef __cplusplus
}
#endif

#endif