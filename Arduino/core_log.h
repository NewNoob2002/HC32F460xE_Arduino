#ifndef CORE_LOG_H
#define CORE_LOG_H

#define LOG_LEVEL_NONE    0
#define LOG_LEVEL_ERROR   1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_DEBUG   4

#ifdef CORE_DEBUG
#include "SEGGER_RTT.h"
#define LOG_LEVEL LOG_LEVEL_DEBUG
#else
#define LOG_LEVEL LOG_LEVEL_NONE
#endif

#if (LOG_LEVEL >= LOG_LEVEL_ERROR)
#define LOG_ERROR(fmt, ...)                                                                        \
    {                                                                                              \
        SEGGER_RTT_printf(0, "[E]" fmt "\n", ##__VA_ARGS__);                                       \
    }
#else
#define LOG_ERROR(fmt, ...)
#endif
#if (LOG_LEVEL >= LOG_LEVEL_WARNING)
#define LOG_WARN(fmt, ...)                                                                         \
    {                                                                                              \
        SEGGER_RTT_printf(0, "[W]" fmt "\n", ##__VA_ARGS__);                                       \
    }
#else
#define LOG_WARN(fmt, ...)
#endif
#if (LOG_LEVEL >= LOG_LEVEL_INFO)
#define LOG_INFO(fmt, ...)                                                                         \
    {                                                                                              \
        SEGGER_RTT_printf(0, "[I]" fmt "\n", ##__VA_ARGS__);                                       \
    }
#else
#define LOG_INFO(fmt, ...)
#endif
#if (LOG_LEVEL >= LOG_LEVEL_DEBUG)
#define LOG_DEBUG(fmt, ...)                                                                        \
    {                                                                                              \
        SEGGER_RTT_printf(0, "[D]" fmt "\n", ##__VA_ARGS__);                                       \
    }
#else
#define LOG_DEBUG(fmt, ...)
#endif

#endif

