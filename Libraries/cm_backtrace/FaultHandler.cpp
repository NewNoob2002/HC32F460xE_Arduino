#include <hc32_ll.h>
#include <Arduino.h>
#include <cm_backtrace.h>

void FaultHandle_Init()
{
    cm_backtrace_init("test", "1.0.0", "1.0.0");
}

extern "C" {
void cmb_printf(const char *__restrict __format, ...)
{
    char printf_buff[256];

    va_list args;
    va_start(args, __format);
    int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), __format, args);
    va_end(args);

    Serial.print(printf_buff);
}
}