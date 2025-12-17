#include "ShutdownModel.h"

using namespace Page;

uint32_t ShutdownModel::GetData()
{
    return lv_tick_get();
}
