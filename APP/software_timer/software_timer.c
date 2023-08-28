#include "software_timer.h"
#include "app_queue.h"
#include "yc_nvic.h"
#include "yc_debug.h"


enum timeout_param {
    TIMEOUT_FLAG,
    TIMEOUT_CURR,
    TIMEOUT_SET,
    TIMEOUT_MODE,
    TIMEOUT_PARAM_NUM
};

enum timeout_flag {
    TIMEOUT_END,
    TIMEOUT_START,
};

static uint16_t task_func_timeout[TIMER_NUM][TIMEOUT_PARAM_NUM];
static task_func_t task_func_array[TIMER_NUM];
static uint8_t timer_num;

static void swtimer_task(void)
{
    for (uint8_t i = 0; i < timer_num; i++)
    {
        if (task_func_timeout[i][TIMEOUT_FLAG] == TIMEOUT_START)
        {
            if (task_func_timeout[i][TIMEOUT_MODE] == TIMER_START_ONCE)
            {
                if (task_func_timeout[i][TIMEOUT_CURR] == task_func_timeout[i][TIMEOUT_SET]) {
                    task_func_timeout[i][TIMEOUT_FLAG] = TIMEOUT_END;
                    app_queue_insert(task_func_array[i]);
                }
                else {
                    OS_ENTER_CRITICAL(); /// add 20230522
                    task_func_timeout[i][TIMEOUT_CURR]++;
                    OS_EXIT_CRITICAL();
                }
            }
            else if (task_func_timeout[i][TIMEOUT_MODE] == TIMER_START_REPEAT)
            {
                if (task_func_timeout[i][TIMEOUT_CURR] == task_func_timeout[i][TIMEOUT_SET]) {
                    task_func_timeout[i][TIMEOUT_CURR] = 0;
                    app_queue_insert(task_func_array[i]);
                }
                else {
                    OS_ENTER_CRITICAL();
                    task_func_timeout[i][TIMEOUT_CURR]++;
                    OS_EXIT_CRITICAL();
                }
            }
        }
    }
}

uint8_t swtimer_restart(uint8_t num)
{
    if (num >= timer_num) {
        return TIMER_OVER_RANGE;
    }
    if (task_func_array[num] == NULL) {
        return TIMER_NOT_EXIST;
    }

    task_func_timeout[num][TIMEOUT_FLAG] = TIMEOUT_START;
    task_func_timeout[num][TIMEOUT_CURR] = 0;

    return 0;
}

uint8_t swtimer_stop(uint8_t num)
{
    if (num >= timer_num) {
        return TIMER_OVER_RANGE;
    }
    if (task_func_array[num] == NULL) {
        return TIMER_NOT_EXIST;
    }

    task_func_timeout[num][TIMEOUT_FLAG] = TIMEOUT_END;

    return 0;
}

uint8_t swtimer_start(uint8_t num, uint16_t timeout, swtimer_mode_t mode)
{
    if ((num >= timer_num) || (timeout < TIMER_UNIT_MS)) /// add 20230522
    {
        return TIMER_OVER_RANGE;
    }
    if (task_func_array[num] == NULL) {
        return TIMER_NOT_EXIST;
    }

    OS_ENTER_CRITICAL();
    task_func_timeout[num][TIMEOUT_FLAG] = TIMEOUT_START;
    task_func_timeout[num][TIMEOUT_CURR] = 0;
    task_func_timeout[num][TIMEOUT_SET] = timeout / TIMER_UNIT_MS - 1; /// add 20230522
    task_func_timeout[num][TIMEOUT_MODE] = mode;
    OS_EXIT_CRITICAL();

    return 0;
}

uint8_t swtimer_delete(uint8_t num)
{
    if (num >= timer_num) {
        return TIMER_OVER_RANGE;
    }
    if (task_func_array[num] == NULL) {
        return TIMER_NOT_EXIST;
    }

    task_func_array[num] = NULL;
    task_func_timeout[num][TIMEOUT_FLAG] = TIMEOUT_END;

    return 0;
}

uint8_t swtimer_add(task_func_t cb)
{
    if (timer_num >= TIMER_NUM) {
        return TIMER_OVER_RANGE;
    }

    if (cb == NULL) {
        return TIMER_CB_IS_NULL;
    }
    else
    {
        for (uint8_t i = 0; i <= timer_num; i++)
        {
            if (task_func_array[i] == cb) {
                return i;
            }
            else if (task_func_array[i] == NULL)
            {
                task_func_array[i] = cb;
                if (i == timer_num) {
                    timer_num++;
                }
                return i;
            }
        }  
    }
}

void software_timer_stop(void)
{
    NVIC_Configuration(rv_tm_IRQn, 14, DISABLE);
}

void software_timer_start(SYSTEM_CLOCK_TypeDef systemClk, uint8_t timer_unit_ms)
{
    SysTick_IrqInit(systemClk, timer_unit_ms * 1000, swtimer_task);
}
