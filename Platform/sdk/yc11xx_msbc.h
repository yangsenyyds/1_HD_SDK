#ifndef _YC11XX_MSBC_H_
#define _YC11XX_MSBC_H_

#include "yc11xx.h"

void 	MSBC_Init(void);
uint8_t MSBC_LoadCacheWait(void);
uint8_t MSBC_UpdateCacheWait(void);
void 	MSBC_EncDecStop(void);
void 	MSBC_EncodeStart(uint32_t sbcOutAddr, uint16_t sbc_outsize , uint32_t pcm_inptr, uint16_t pcm_insize, uint32_t cache_bufferaddr);
void 	MSBC_DecodeStart(uint32_t sbc_inaddr, uint16_t sbc_insize, uint32_t pcm_outptr, uint16_t pcm_outsize , uint32_t cache_bufferaddr);
void 	MSBC_ClockOn(void);
void 	MSBC_ClockOff(void);
void 	MSBC_BusyEait(void);

#endif
