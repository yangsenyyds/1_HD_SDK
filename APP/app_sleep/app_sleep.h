#ifndef APP_SLEEP_H
#define APP_SLEEP_H
#include <string.h>
#include "yc11xx.h"
#include "remote_control.h"

typedef enum {
    ADV_LOCK,
    APP_LOCK,
    KEY_LOCK,
    LED_LOCK,
    IR_LOCK,
    AUDIO_LOCK,
    MOUSE_LOCK,
    LATENCY_LOCK,
    DEV_LOCK_NUM
}DEV_LOCK_TypeDef;

/*
WEAK void Action_After_Prepare_Sleep(void){} //在每次进入lpm之前
WEAK void Action_After_Enter_Deep_Sleep(void){}//每次进入深度休眠之前
*/
extern void prepare_before_sleep(void);
extern void enter_deep_sleep(void);
extern void enter_low_sleep(void);
extern bool app_sleep_check(void);
extern void app_sleep_lock_set(DEV_LOCK_TypeDef dev_lock, bool state);
extern void app_sleep_timer_set(uint16_t time);
extern uint8_t app_sleep_init(void);

#endif
