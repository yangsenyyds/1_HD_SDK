#ifndef IR_SELECT_H
#define IR_SELECT_H
#include "yc11xx.h"

#define TV (2)
#define AV (3)
#define VOL_H (13)
#define VOL_L (17)
#define METU (16)

typedef void (*ir_brand_init_t)(uint8_t);

extern const uint16_t search_ir_number[];

extern ir_brand_init_t brand_AV_init(uint16_t brand);
extern ir_brand_init_t brand_DVB_init(uint16_t brand);
#endif

