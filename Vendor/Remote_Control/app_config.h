#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include "app_sleep.h"
#include "remote_control.h"

/* ATTRIBUTE */
#define MEMORY_NOT_PROTECT_UNDER_LPM_ATT  __attribute__ ((section(".memory_protect_under_lpm")))
#define APP_DEBUGE_INFO_ATT  __attribute__ ((section(".debug_info")))

/* Compiler Related Definitions */
#ifdef __CC_ARM                     /* ARM Compiler */
#include <stdarg.h>
#define SECTION(x)                  __attribute__((section(x)))
#define YC_UNUSED                   __attribute__((unused))
#define YC_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define WEAK						__weak
#elif defined (__GNUC__)            /* GNU GCC Compiler */
#define SECTION(x)                  __attribute__((section(x)))
#define YC_UNUSED                   __attribute__((unused))
#define YC_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define WEAK                        __attribute__((weak))
#else
#error not supported tool chain
#endif

/* HEAP */
#define HEAP_BUFFER_SIZE    (0x2000)

#endif