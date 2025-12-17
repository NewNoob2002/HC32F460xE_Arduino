/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */

#include <elog.h>
#include <stdio.h>

#include "lvgl/lvgl.h"
#include "../../../Platform/support/support.h"
#if defined(HC32F460)
#include "SEGGER_RTT.h"
#else
#include <windows.h>
static HANDLE output_lock = NULL;
#endif
char buf[32];
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;
#if defined(HC32F460)
    /* add your code here */
    SEGGER_RTT_Init();
#else
    output_lock = CreateMutex(NULL, FALSE, NULL);
#endif

    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {
    /* add your code here */
	#if defined(HC32F460)
	#else
    CloseHandle(output_lock);
	#endif
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    /* add your code here */
#if defined(HC32F460)
    /* add your code here */
    SEGGER_RTT_Write(0, log, size);
#else
    printf("%.*s", size, log);
#endif

}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    /* add your code here */
#if defined(HC32F460)

#else
    WaitForSingleObject(output_lock, INFINITE);
#endif
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    /* add your code here */
#if defined(HC32F460)

#else
    ReleaseMutex( output_lock );
#endif
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    /* add your code here */
    //		MakeTimeString(lv_tick_get(), buf, 32);
#if defined(HC32F460)
    lv_snprintf(buf, 32, "%d", lv_tick_get());
    return buf;
#else
    static char cur_system_time[24] = { 0 };
    static SYSTEMTIME currTime;

    GetLocalTime(&currTime);
    snprintf(cur_system_time, 24, "%02d-%02d %02d:%02d:%02d.%03d", currTime.wMonth, currTime.wDay,
            currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);

    return cur_system_time;
#endif
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    /* add your code here */
#if defined(HC32F460)
    return "";
#else
    static char cur_process_info[10] = { 0 };
    snprintf(cur_process_info, 10, "pid:%04ld", GetCurrentProcessId());

    return cur_process_info;
#endif
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    /* add your code here */
#if defined(HC32F460)
    return "";
#else
    static char cur_thread_info[10] = { 0 };

    snprintf(cur_thread_info, 10, "tid:%04ld", GetCurrentThreadId());

    return cur_thread_info;
#endif
}
