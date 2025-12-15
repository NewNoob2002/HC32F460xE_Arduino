#pragma once
#include "lvgl/src/font/lv_font.h"
#include "symbol/symbol_unicode.h"

namespace ResourcePool
{

void Init();
lv_font_t* GetFont(const char* name);
const void* GetImage(const char* name);

}
