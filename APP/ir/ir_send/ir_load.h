#ifndef IR_LOAD_H
#define IR_LOAD_H
#include <string.h>
#include "yc11xx.h"

#include "remote_control.h"
#include "ir_lib.h"

#ifdef IR_RCV_PIN
#include "ir_learn_from_remote.h"
#endif

#ifdef SAMSUNG
#define IR_BUF_SIZE (512)
#else
#define IR_BUF_SIZE (256)
#endif
extern uint8_t *get_ir_pulse_fill(void);
extern void set_ir_pulse_fill(void);
extern void ir_send(void);
#ifdef IR_RCV_PIN
extern void ir_remote_learn_send(ir_receive_param_t irparams);
#endif
extern void ir_single_send(uint16_t irnum, uint8_t repeat);//3
extern void ir_time_send(const uint16_t *time_data);//time date data[0]为载波 data[1]数据长度 data[2]~data[n]为高低电平时间
extern void ir_comm_send(uint16_t irnum);
extern void ir_type_init(ir_type_t type, customer_code_t custom);
extern void ir_init(ir_type_t type, customer_code_t custom);

#endif
