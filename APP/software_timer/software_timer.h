#ifndef SOFTWARE_TIMER
#define SOFTWARE_TIMER
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_systick.h"

#include "remote_control.h"

enum swtimer_error_code {
    TIMER_INIT_ERROR= 0XF0,
    TIMER_CB_IS_NULL,
    TIMER_OVER_RANGE,
    TIMER_ALREADY_EXIST,
    TIMER_NOT_EXIST,
};

typedef enum {
    TIMER_START_ONCE,
    TIMER_START_REPEAT,
}swtimer_mode_t;

typedef void (*task_func_t)(void);

extern uint8_t swtimer_restart(uint8_t num);
extern uint8_t swtimer_stop(uint8_t num);
extern uint8_t swtimer_start(uint8_t num, uint16_t timeout, swtimer_mode_t mode);
extern uint8_t swtimer_delete(uint8_t num);
extern uint8_t swtimer_add(task_func_t cb);
extern void software_timer_stop(void);
extern void software_timer_start(SYSTEM_CLOCK_TypeDef systemClk, uint8_t timer_unit_ms);

#endif
