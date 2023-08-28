#ifndef MOUSE_WHEEL_H
#define MOUSE_WHEEL_H
#include "yc11xx.h"

typedef enum {
    WHEEL_DIR_UP = 0x00,
    WHEEL_DIR_DOWN = 0x01,
    WHEEL_DIR_IDLE = 0xff,
}WHEEL_DIR_E;


extern WHEEL_DIR_E get_wheel_dir(void);
extern void mouse_wheel_switch(bool onoff);
extern void mouse_wheel_deinit(void);
extern void mouse_wheel_init(void);

#endif
