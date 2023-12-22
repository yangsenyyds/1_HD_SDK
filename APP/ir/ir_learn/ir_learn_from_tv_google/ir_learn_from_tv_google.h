#ifndef IR_LEARN_FROM_TV_GOOGLE_H
#define IR_LEARN_FROM_TV_GOOGLE_H
#include <stdlib.h>
#include "yc11xx.h"

extern void ir_tv_learn_send(uint16_t keynum);
extern void ir_learn_data_fill(uint8_t *tv_ir_learn_data);
extern void ir_learn_data_clr(void);
extern void ir_learn_init(void);
extern bool get_ir_learn_state(void);
#endif