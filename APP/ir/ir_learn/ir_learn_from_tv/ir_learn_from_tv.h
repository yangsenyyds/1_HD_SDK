#ifndef IR_LEARN_FROM_TV_H
#define IR_LEARN_FROM_TV_H
#include <stdlib.h>
#include <ctype.h>
#include "yc11xx.h"


extern void ir_tv_learn_send(uint16_t keynum);
extern void ir_learn_data_fill(void);
extern uint8_t *get_ir_learn_data_buf_addr(uint16_t offset);
extern void ir_learn_data_clr(void);
extern void ir_learn_reinit(void);
extern void ir_learn_init(void);

#endif
