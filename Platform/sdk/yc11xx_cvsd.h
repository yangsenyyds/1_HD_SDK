#ifndef _YC11XX_CVSD_H_
#define _YC11XX_CVSD_H_

#include "yc11xx.h"
#include "reg_addr.h"
#include "reg_struct.h"

void CVSD_ClkEnable(void);
void CVSD_ClkDisable(void);
void CVSD_WaitCvsdFilterClrDone(void);
void CVSD_CoefInit(void);
void CVSD_Start(void);
void CVSD_Stop(void);
void CVSD_OutInit(uint32_t cvsd_outbufferaddr, uint16_t cvsd_outbuffersize, uint32_t cvsd_pcmInbufferaddr, uint16_t cvsd_pcmInbuffersize);
void CVSD_InInit(uint32_t cvsd_inbufferaddr, uint16_t cvsdI_inbuffersize, uint32_t cvsd_pcmoutbufferaddr, uint16_t cvsd_pcmoutbuffersize);
void CVSD_Init(uint32_t cvsd_inbufferaddr, uint16_t cvsdI_inbuffersize, uint32_t cvsd_pcmoutbufferaddr, uint16_t cvsd_pcmoutbuffersize, uint32_t cvsd_outbufferaddr, uint16_t cvsd_outbuffersize, uint32_t cvsd_pcmInbufferaddr, uint16_t cvsd_pcmInbuffersize, uint16_t cvsd_grplength);
bool CVSD_CheckWorkStart(void);
bool CVSD_CheckEnable(void);
bool CVSD_CheckWorkInprocess(void);
bool CVSD_CheckInUseMRam(void);
bool CVSD_CheckOutUseMRam(void);
uint32_t CVSD_GetInAddr(void);
uint32_t CVSD_GetOutAddr(void);
void CVSD_WorkEnable(void);
void CVSD_WorkDisable(void);


#endif
