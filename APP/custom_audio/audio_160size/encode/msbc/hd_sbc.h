#ifndef HD_SBC_H
#define HD_SBC_H
#include "yc11xx.h"

extern void msbc_encode_process(int16_t *input_buf, uint8_t *output_buf);
extern void msbc_encoder_init(void);

#endif
