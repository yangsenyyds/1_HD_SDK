#ifndef LED_H
#define LED_H
#include <string.h>
#include "yc11xx.h"

#include "remote_control.h"

typedef enum {
#ifdef LED_1_PIN
    LED_1,
#endif
#ifdef LED_2_PIN
    LED_2,
#endif
#ifdef LED_3_PIN
    LED_3,
#endif
    LED_NUM
} lednum_t;

extern void led_off(lednum_t lednum);
extern void led_on(lednum_t lednum, uint16_t blk_once_time, uint32_t blk_total_time);
extern void led_reinit(void);
extern void led_deinit(void);
extern uint8_t led_init(void);

#endif
