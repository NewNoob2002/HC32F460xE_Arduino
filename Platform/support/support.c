#include "support.h"

const char* MakeTimeString(uint32_t ms, char* buf, uint16_t len)
{
    uint64_t ss = ms / 1000;
    uint64_t mm = ss / 60;
    uint32_t hh = (uint32_t)(mm / 60);

    lv_snprintf(
        buf, len,
        "%d:%02d:%02d",
        hh,
        (uint32_t)(mm % 60),
        (uint32_t)(ss % 60)
    );

    return buf;
}

void MakeTime(uint32_t ms, makeTime_t* time)
{
	  uint64_t ss = ms / 1000;
    uint64_t mm = ss / 60;
    uint32_t hh = (uint32_t)(mm / 60);
	
    time->second 	= ss % 60;
    time->minute  = mm % 60;
    time->hour 		= hh;
}