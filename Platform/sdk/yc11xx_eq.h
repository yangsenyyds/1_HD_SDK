#ifndef __YC11xx_EQ_H__
#define __YC11xx_EQ_H__
#include "yc11xx.h"
#include "reg_addr.h"
#include "reg_struct.h"
extern int EqIIRCoef[51];

typedef enum
{
    EQ_16Bit,
    EQ_24Bit
} EQ_BitModeSelTypeDev;

typedef enum
{
    EQ_Mono,
    EQ_Stereo
} EQ_NumChannel;

void EQ_Init(uint8_t isMRam,EQ_BitModeSelTypeDev dataBit,EQ_NumChannel channel);
void EQ_CoefInit(void);
void EQ_Work(uint8_t *srcAddr, uint8_t *dstAddr, uint16_t len);
void EQ_Off(void);
uint16_t EQ_GetDmaRptr(void);
uint16_t EQ_GetDmaWptr(void);
void EQ_WorkEndCheck(void);



#endif
