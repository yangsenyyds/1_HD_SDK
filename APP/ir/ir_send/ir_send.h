#ifndef IR_SEND_H
#define IR_SEND_H
#include "yc11xx.h"


typedef enum {
    IR_STOP,
    IR_START,
} IR_CODE_VAL;


extern uint8_t get_key_press_state(void);
extern void set_key_press_state(bool state);
extern void ir_set_freq(uint16_t freq);
extern void ir_gpio_send(IR_CODE_VAL codeVal, uint32_t time);
extern void ir_pwm_send(IR_CODE_VAL codeVal, uint32_t time);
extern void set_pwm_freq_div(void);

#endif
