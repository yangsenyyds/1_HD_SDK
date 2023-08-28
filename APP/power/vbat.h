#ifndef VBAT_H
#define VBAT_H
#include "yc11xx.h"

typedef void (*power_report_cb_t)(uint8_t);

extern void vbat_reinit(void);
extern void vbat_deinit(void);
extern uint8_t vbat_init(power_report_cb_t cb);

#endif
