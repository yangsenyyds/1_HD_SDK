#include "led.h"
#include "software_timer.h"
#include "app_sleep.h"
#include "yc11xx_gpio.h"
#include "app_config.h"
#include "yc_debug.h"
#ifndef BLK_UNIT_TIME
#define BLK_UNIT_TIME (100)
#endif

typedef struct {
    bool on_off : 1;
    bool blk_on_off : 1;
    bool blk_constant : 1;
    bool dir : 1;
    uint16_t time;
    uint16_t count;
    uint16_t blk_once_time;
    uint32_t blk_total_time;
} led_state_t;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT led_state_t led[LED_NUM];
static uint8_t led_blk_timernum = 0XFF;

WEAK void action_after_led_blk(void){}
WEAK void app_sleep_lock_set(DEV_LOCK_TypeDef dev_lock, bool state){}

static void led_task(void)
{
#ifdef LED_1_PIN
    if (led[LED_1].blk_on_off)
    {
        if (led[LED_1].blk_constant)
        {
            led[LED_1].time += BLK_UNIT_TIME;
            if (led[LED_1].time >= led[LED_1].blk_once_time)
            {
                led[LED_1].time = 0;
                led[LED_1].dir ^= 0x01;
                if (led[LED_1].dir) {
                    GPIO_Init(LED_1_PIN, LED_ON);
                }
                else {
                    GPIO_Init(LED_1_PIN, LED_OFF);
                }
            }
        }
        else
        {
            led[LED_1].time += BLK_UNIT_TIME;
            if (led[LED_1].time >= led[LED_1].blk_once_time)
            {
                led[LED_1].count++;
                led[LED_1].time = 0;
                DEBUG_LOG_STRING("55 %d  %d\r\n",led[LED_1].count * led[LED_1].blk_once_time,led[LED_1].blk_total_time);
                if (led[LED_1].count * led[LED_1].blk_once_time >= led[LED_1].blk_total_time) {
                    memset((void *)&led[LED_1], 0, sizeof(led_state_t));
                    GPIO_Init(LED_1_PIN, LED_OFF);
                }
                else
                {
                    led[LED_1].dir ^= 0x01;
                    if (led[LED_1].dir) {
                        GPIO_Init(LED_1_PIN, LED_ON);
                    }
                    else {
                        GPIO_Init(LED_1_PIN, LED_OFF);
                    }
                }
            }
        }
    }
#endif

#ifdef LED_2_PIN
    if (led[LED_2].blk_on_off)
    {
        if (led[LED_2].blk_constant)
        {
            led[LED_2].time += BLK_UNIT_TIME;
            if (led[LED_2].time >= led[LED_2].blk_once_time)
            {
                led[LED_2].time = 0;
                led[LED_2].dir ^= 0x01;
                if (led[LED_2].dir) {
                    GPIO_Init(LED_2_PIN, LED_ON);
                }
                else {
                    GPIO_Init(LED_2_PIN, LED_OFF);
                }
            }
        }
        else
        {
            led[LED_2].time += BLK_UNIT_TIME;
            if (led[LED_2].time >= led[LED_2].blk_once_time)
            {
                led[LED_2].count++;
                led[LED_2].time = 0;

                if (led[LED_2].count * led[LED_2].blk_once_time >= led[LED_2].blk_total_time) {
                    memset((void *)&led[LED_2], 0, sizeof(led_state_t));
                    GPIO_Init(LED_2_PIN, LED_OFF);
                }
                else
                {
                    led[LED_2].dir ^= 0x01;
                    if (led[LED_2].dir) {
                        GPIO_Init(LED_2_PIN, LED_ON);
                    }
                    else {
                        GPIO_Init(LED_2_PIN, LED_OFF);
                    }
                }
            }
        }
    }
#endif

#ifdef LED_3_PIN
    if (led[LED_3].blk_on_off)
    {
        if (led[LED_3].blk_constant)
        {
            led[LED_3].time += BLK_UNIT_TIME;
            if (led[LED_3].time >= led[LED_3].blk_once_time)
            {
                led[LED_3].time = 0;
                led[LED_3].dir ^= 0x01;
                if (led[LED_3].dir) {
                    GPIO_Init(LED_3_PIN, LED_ON);
                }
                else {
                    GPIO_Init(LED_3_PIN, LED_OFF);
                }
            }
        }
        else
        {
            led[LED_3].time += BLK_UNIT_TIME;
            if (led[LED_3].time >= led[LED_3].blk_once_time)
            {
                led[LED_3].count++;
                led[LED_3].time = 0;

                if (led[LED_3].count * led[LED_3].blk_once_time >= led[LED_3].blk_total_time) {
                    memset((void *)&led[LED_3], 0, sizeof(led_state_t));
                    GPIO_Init(LED_3_PIN, LED_OFF);
                }
                else
                {
                    led[LED_3].dir ^= 0x01;
                    if (led[LED_3].dir) {
                        GPIO_Init(LED_3_PIN, LED_ON);
                    }
                    else {
                        GPIO_Init(LED_3_PIN, LED_OFF);
                    }
                }
            }
        }
    }
#endif

    for (lednum_t lednum = LED_1; lednum < LED_NUM; lednum++) {
        if (led[lednum].blk_on_off) {
            swtimer_restart(led_blk_timernum);
            return;
        }
    }
    DEBUG_LOG_STRING("171 \r\n");
    app_sleep_lock_set(LED_LOCK, false);
    action_after_led_blk();
}

void led_off(lednum_t lednum)
{
    switch (lednum)
    {
#ifdef LED_1_PIN
    case LED_1:
        memset((void *)&led[LED_1], 0, sizeof(led_state_t));
        GPIO_Init(LED_1_PIN, LED_OFF);
        break;
#endif
#ifdef LED_2_PIN
    case LED_2:
        memset((void *)&led[LED_2], 0, sizeof(led_state_t));
        GPIO_Init(LED_2_PIN, LED_OFF);
        break;
#endif
#ifdef LED_3_PIN
    case LED_3:
        memset((void *)&led[LED_3], 0, sizeof(led_state_t));
        GPIO_Init(LED_3_PIN, LED_OFF);
        break;
#endif
    case LED_NUM:
#ifdef LED_1_PIN
        memset((void *)&led[LED_1], 0, sizeof(led_state_t));
        GPIO_Init(LED_1_PIN, LED_OFF);
#endif
#ifdef LED_2_PIN
        memset((void *)&led[LED_2], 0, sizeof(led_state_t));
        GPIO_Init(LED_2_PIN, LED_OFF);
#endif
#ifdef LED_3_PIN
        memset((void *)&led[LED_3], 0, sizeof(led_state_t));
        GPIO_Init(LED_3_PIN, LED_OFF);
#endif
        break;
    
    default:
        break;
    }

    for (lednum_t lednum = LED_1; lednum < LED_NUM; lednum++) {
        if (led[lednum].on_off || led[lednum].blk_on_off) {
            return;
        }
    }
    app_sleep_lock_set(LED_LOCK, false);
}

void led_on(lednum_t lednum, uint16_t blk_once_time, uint32_t blk_total_time)
{
    switch (lednum)
    {
#ifdef LED_1_PIN
    case LED_1:
        GPIO_Init(LED_1_PIN, LED_ON);
        break;
#endif
#ifdef LED_2_PIN
    case LED_2:
        GPIO_Init(LED_2_PIN, LED_ON);
        break;
#endif
#ifdef LED_3_PIN
    case LED_3:
        GPIO_Init(LED_3_PIN, LED_ON);
        break;
#endif

    default:
        break;
    }

    if (blk_once_time != 0 || blk_total_time != 0)
    {
        memset(&led[lednum], 0, sizeof(led_state_t));
        led[lednum].blk_on_off = true;
        led[lednum].dir = true;

        if (blk_once_time > 0)
        {
            led[lednum].blk_once_time = blk_once_time;

            if (blk_total_time == 0) {
                led[lednum].blk_constant = true;
            }
            else {
                led[lednum].blk_constant = false;
                led[lednum].blk_total_time = blk_total_time;
            }
        }
        else {
            led[lednum].blk_constant = false;
            led[lednum].blk_total_time = blk_total_time;
            led[lednum].blk_once_time = blk_total_time;
        }
        swtimer_start(led_blk_timernum, BLK_UNIT_TIME, TIMER_START_ONCE);
    }
    else {
        swtimer_stop(led_blk_timernum);
        memset((void *)&led[lednum], 0, sizeof(led_state_t));
        led[lednum].on_off = true;
    }

    app_sleep_lock_set(LED_LOCK, true);
}

void led_reinit(void)
{
    memset(led, 0, sizeof(led));
}

void led_deinit(void)
{
#ifdef LED_1_PIN
    GPIO_Init(LED_1_PIN, LED_OFF);
#endif
#ifdef LED_2_PIN
    GPIO_Init(LED_2_PIN, LED_OFF);
#endif
#ifdef LED_3_PIN
    GPIO_Init(LED_3_PIN, LED_OFF);
#endif
}

uint8_t led_init(void)
{
#ifdef LED_1_PIN
    GPIO_Init(LED_1_PIN, LED_OFF);
#endif
#ifdef LED_2_PIN
    GPIO_Init(LED_2_PIN, LED_OFF);
#endif
#ifdef LED_3_PIN
    GPIO_Init(LED_3_PIN, LED_OFF);
#endif

    led_blk_timernum = swtimer_add(led_task);
    if (led_blk_timernum > TIMER_NUM) {
        return led_blk_timernum;
    }

    return 0;
}
