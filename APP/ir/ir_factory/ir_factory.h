#ifndef IR_FACTORY_H
#define IR_FACTORY_H
#include <string.h>
#include "yc11xx.h"


extern void ir_factory_send(uint16_t irnum);
extern void set_ir_factory_press(bool press);

#endif