#ifndef MOUSE_H
#define MOUSE_H
#include "yc11xx.h"
#include "remote_control.h"
#include "software_i2c.h"

// 定义MPU9250内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
//#define	GYRO_CONFIG		0x27	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
//#define	ACCEL_CONFIG	0x28	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define WHO_AM_I_REG_ADDR   (0X75) //CHIP ID

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		  0x75	//IIC地址寄存器(默认数值0x68，只读)

#define MPU_6500_SENSOR_ADDR   (0X68 << 1)
//****************************

// #define	GYRO_ADDRESS   0xD0	  //陀螺地址
#define	GYRO_ADDRESS   (0X68 << 1)  //陀螺地址
#define MAG_ADDRESS    0x18   //磁场地址
// #define ACCEL_ADDRESS  0xD0 
#define ACCEL_ADDRESS       (0X68 << 1)

//************************************

void READ_MPU9250_ACCEL(void);
void READ_MPU9250_GYRO(void);
void READ_MPU9250_MAG(void);
static int Init_MPU9250(void);
#endif