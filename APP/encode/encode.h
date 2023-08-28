

#ifndef ENCODE_H
#define ENCODE_H
#include "yc11xx.h"

//// 编码器方向定义
enum encode_dir_t
{
    ENCODE_DIR_UP = 0x00,          /// 向上滚
    ENCODE_DIR_DOWN = 0x01,        /// 向下滚
    ENCODE_DIR_BLE_UNCHECK = 0xfe, /// BLE未检测成功
    ENCODE_DIR_UNCHECK = 0xff,     /// IR未检测成功
};
extern bool encode_state;
extern void encode_on(void);
extern void encode_off(void);
extern void encode_timer_init(void);
extern uint8_t get_encode_chk_ok_state(void);
extern void encode_timer_deinit(void);
#endif