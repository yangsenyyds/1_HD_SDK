#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "yc11xx_gpio.h"


/* WATCH_DOG */
// #define FUNCTION_WATCH_DOG

/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE   (GPIO_9)

extern void app_task_run(void);
extern void app_init(void);

#endif
