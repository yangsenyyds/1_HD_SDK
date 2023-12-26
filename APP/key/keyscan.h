#ifndef KEYSCAN_H
#define KEYSCAN_H
#include <string.h>
#include "yc11xx.h"

#include "remote_control.h"

typedef enum {
    KEY_MODE_SINGLE,
    KEY_MODE_CONTINUE,
}key_mode_t;

typedef struct KEY_REPORT {
    uint8_t key_press_cnt;
    uint8_t keynum_scan_buf[KEY_COL_NUM];
    uint8_t keynum_report_buf[KEY_COL_NUM];
}key_report_t;

typedef void (*key_report_cb_t)(key_report_t *);

extern uint8_t key_lock_state;
extern void keyscan_row_cfg_set(void);
extern uint8_t get_key_timernum(void);
extern void key_wakeup_set_high(void);
extern bool key_wakeup_get_high(void);
extern bool key_wakeup_get(void);
extern void key_wakeup_set(void);
extern void keyscan_stop(void);
extern void keyscan_start(void);
extern uint8_t keyscan_init(key_mode_t mode, key_report_cb_t cb);

#endif
