#ifndef _YC11XX_NEC_H_
#define _YC11XX_NEC_H_
#include "yc11xx.h"


typedef enum {
    NEC_Disable,
    NEC_Enable
} NEC_IsDetectEnable;

typedef enum {
    Gain_Min,
	Gain_Level1,
	Gain_Level2,
	Gain_Max,
} NEC_GainLevel;

typedef struct
{
    uint32_t StartBuff;                 /*!< nec start address. */
    uint32_t EndBuff;                   /*!< nec end address. */
    uint8_t *NecReadPtr;                /*!< nec read pointer. */
    uint8_t ClkDivNum;                  /*!< nec detect clk divide number. */
    NEC_IsDetectEnable DetectEn;        /*!< nec detect enable. */
} NEC_Config;

void NEC_RxLearnInit(NEC_Config config);
void NEC_SimulationInit(FunctionalState state, NEC_GainLevel gain_level);
uint16_t NEC_RxBuffLen(void);
void NEC_SetReadPtr(uint16_t *ptr);

#endif
