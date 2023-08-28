#ifndef SOFTWARE_I2C_H
#define SOFTWARE_I2C_H
#include "yc11xx.h"

extern void IIC_NAck(void);
extern void IIC_Ack(void);
extern uint8_t IIC_WaitAck(void);
extern uint8_t IIC_ReadByte(uint8_t ack);
extern void IIC_SendByte(uint8_t ucByte);
extern void IIC_Stop(void);
extern void IIC_Start(void);
extern void IIC_DeInit(void);
extern void IIC_Init(void); 
extern uint8_t IIC_CheckDevice(uint8_t address);

#endif
