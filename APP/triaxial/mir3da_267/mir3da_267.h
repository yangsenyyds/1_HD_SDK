#ifndef MIR3DA_267_H
#define MIR3DA_267_H
#include "yc11xx.h"
#include "software_i2c.h"
#include <stdbool.h>
#include <string.h>

#define ARM_BIT_8               0

#if ARM_BIT_8
//如下数据类型是在8位机上定义的，在其它平台（比如32位）可能存在差别，需要根据实际情况修改 。 
typedef unsigned char    u8_m;                   /* 无符号8位整型变量*/
typedef signed   char    s8_m;                   /* 有符号8位整型变量*/
typedef unsigned int     u16_m;                  /* 无符号16位整型变量*/
typedef signed   int     s16_m;                  /* 有符号16位整型变量*/
typedef unsigned long    u32_m;                  /* 无符号32位整型变量*/
typedef signed   long    s32_m;                  /* 有符号32位整型变量*/
typedef float            fp32_m;                 /* 单精度浮点数（32位长度）*/
typedef double           fp64_m;                 /* 双精度浮点数（64位长度）*/
#else
//如下数据类型是在32位机上定义的，在其它平台（比如8位）可能存在差别，需要根据实际情况修改 。 
typedef unsigned char    u8_m;                   /* 无符号8位整型变量*/
typedef signed   char    s8_m;                   /* 有符号8位整型变量*/
typedef unsigned short   u16_m;                  /* 无符号16位整型变量*/
typedef signed   short   s16_m;                  /* 有符号16位整型变量*/
typedef unsigned int     u32_m;                  /* 无符号32位整型变量*/
typedef signed   int     s32_m;                  /* 有符号32位整型变量*/
typedef float            fp32_m;                 /* 单精度浮点数（32位长度）*/
typedef double           fp64_m;                 /* 双精度浮点数（64位长度）*/
#endif

/*******************************************************************************
Macro definitions - Register define for Gsensor asic
********************************************************************************/
#define REG_SPI_I2C                 0x00
#define REG_CHIP_ID                 0x01
#define REG_ACC_X_LSB               0x02
#define REG_ACC_X_MSB               0x03
#define REG_ACC_Y_LSB               0x04
#define REG_ACC_Y_MSB               0x05
#define REG_ACC_Z_LSB               0x06
#define REG_ACC_Z_MSB               0x07
#define REG_FIFO_STATUS             0x08
#define REG_MOTION_FLAG             0x09
#define REG_STEPS_MSB               0x0D
#define REG_STEPS_LSB               0x0E
#define REG_RESOLUTION_RANGE        0x0F
#define REG_MODE_ODR                0x10
#define REG_MODE_AXIS               0x11
#define REG_SWAP_POLARITY           0x12
#define REG_FIFO_CTRL               0x14
#define REG_INT_SET0                0x15
#define REG_INT_SET1                0x16
#define REG_INT_SET2                0x17
#define REG_INT_MAP1                0x19
#define REG_INT_MAP2                0x1A
#define REG_INT_MAP3                0x1B
#define REG_INT_CONFIG              0x20
#define REG_INT_LATCH               0x21
#define REG_TAP_QUIET               0x29
#define REG_TAP_DUR                 0x2A
#define REG_TAP_THS                 0x2B
#define REG_RESET_STEP              0x2E
#define REG_STEP_FILTER             0x33
#define REG_ACTIVE_DUR              0x38
#define REG_ACTIVE_X_THS            0x39
#define REG_ACTIVE_Y_THS            0x3A
#define REG_ACTIVE_Z_THS            0x3B
/*********** INTERNAL REGISTER ************/
#define REG_ENGINEERING_MODE        0x7F
#define REG_SENS_COMP               0x8C
#define REG_MEMS_OPTION             0x8F
#define REG_CHIP_INFO               0xC0

// #define abs(x)          (((x) > 0) ? (x) : (-(x)))

// extern u8_m i2c_addr;
extern s8_m mir3da_open_active_interrupt(void);
extern s8_m mir3da_close_active_interrupt(void);
extern s8_m mir3da_init(void);

#endif