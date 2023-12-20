#ifndef _YC11XX_SBC_H_
#define _YC11XX_SBC_H_

#include "yc11xx.h"

void Audio_SbcVoiceFilterClkOff(void);
void Audio_SbcVoiceFilterClkOn(void);
void Audio_SbcFarrowFilterDisable(void);
void Audio_SbcFarrowFilterEnable(void);
void Audio_SbcFarrowFilterSbcEnable(void);
void Audio_SbcFarrowFilter44Kto48K(void);
void Audio_SbcFarrowFilter44Kto44K(void);
void Audio_SbcFarrowFilter48Kto48K(void);
void Audio_SbcFarrowFilterClear(void);
void Audio_SbcBusyWait(void);
void Audio_SbcClkOff(void);
void Audio_SbcClkOn(void);
void Audio_SbcStop(void);
void Audio_SbcClear(void);
void Audio_SbcDstMramSelect(bool isMram);
void Audio_SbcBlockControl(bool isEnable);
void Audio_SbcHwCheckError(void);
void Audio_SbcSetStreamOutSingleLeft(void);
void Audio_SbcSetStreamOutSingleRight(void);
void Audio_SbcSetStreamOutLeftRightMix(void);
void Audio_SbcSetStreamOutFull(void);
void Audio_SbcUpdateStreamOutTws(void);
void Audio_SbcUpdateStreamOut(void);
void Audio_SbcInit(uint32_t bufferStartAddr, uint16_t bufferLen);
uint32_t Audio_SbcGetDstAddr(void);
void Audio_SbcSrcMramSelect(bool isMram);
uint8_t Audio_SbcErrorAndReinit(void);
void Audio_SbcFarrowFilterClose(void);
uint16_t SADC_ReadSAdcValue(void);




#endif
