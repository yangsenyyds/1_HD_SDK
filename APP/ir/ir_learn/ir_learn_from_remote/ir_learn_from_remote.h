#ifndef IR_LEARN_FROM_REMOTE_H
#define IR_LEARN_FROM_REMOTE_H
#include "yc11xx.h"
#include "remote_control.h"
#define INPUT_MARK 0

#ifndef RAW_BUFFER_LENGTH
#define RAW_BUFFER_LENGTH (126)
#endif

enum{
    IR_LEARN_SUCE,
    IR_LEARN_OVERTIME,
    IR_LEARN_FAIL,
    IR_LEARN_BACK_KEYNUM,
};

typedef struct IRDATA {
    uint8_t freq;
    uint8_t len;
    uint16_t repeat;
    uint16_t repeat_stop;
    uint16_t crc;
    uint32_t rawbuf[RAW_BUFFER_LENGTH];
} ir_receive_param_t;

extern uint32_t tempStartTimer;
extern ir_receive_param_t irparams;

extern uint8_t ir_receive(void);
#endif