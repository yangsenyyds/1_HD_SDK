#ifndef MOUSE_H
#define MOUSE_H
#include "yc11xx.h"
#include "remote_control.h"
#include "software_i2c.h"
typedef struct {
#if XY_TYPE
    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;
#elif ORI_TYPE
    uint16_t ax;
    uint16_t ay;
    uint16_t az;

    uint16_t gx;
    uint16_t gy;
    uint16_t gz;
#endif
}sensor_data_t;

typedef void (*mouse_report_cb_t)(void *);

extern void mouse_off(void);
extern void mouse_on(void);

extern void mouse_lpm_int_mode(void);
extern void mouse_turn_off(void);
extern void mouse_turn_on(void);
extern void mouse_reinit(void);
extern void mouse_deinit(void);
extern void mouse_init(mouse_report_cb_t cb);
extern uint8_t get_mouse_int_state(void);
extern void mouse_int_mode(void);
extern void check_mouse_chip_id(void);
uint16_t get_acc_lsb_div(void);
uint16_t get_gyro_lsb_div(void);
uint8_t get_axis_moving_state(void);

#endif
