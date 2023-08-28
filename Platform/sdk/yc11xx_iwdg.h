#ifndef __YC11XX_IWDG_H
#define __YC11XX_IWDG_H

#define IWDG_IP(x)				*((volatile int*)(0xE000E400 + x*4))

#include "yc11xx.h"

/**
  * @brief  watch dog number definition
  */
typedef enum
{
    WDT,
    WDT2,
} IWDG_NumTypeDef;
#define IS_WDTX(WDTx)         (WDTx == WDT)||\
						(WDTx == WDT2)
/**
  * @brief  watch dog mode definition
  */
typedef enum
{
    RESET_MODE = 0x02,
    INTR_MODE = 0xfd
} IWDG_ModeTypedef;
#define IS_WDT_MODE(mode)         (mode == RESET_MODE)||\
								(mode == INTR_MODE)

#define WDT_BT_INTR_CLR   0xfe
#define WDT_CM0_INTR_CLR  0xfd

#define WDT_INTR_BIT BIT1

#define WDT_BT_EN_BIT  BIT1
#define WDT_CM0_EN_BIT BIT2

#define wdt_en_bit(x) (BIT1 + x * 2)
#define wdt_intr_clr(x) (0xfe - x * 1)

#define WDT_timer_125ms  0x7F
#define WDT_timer_250ms  0x7e
#define WDT_timer_500ms  0x7c
#define WDT_timer_1_5s   0x74
#define WDT_timer_10s    0x32
#define WDT_timer_Max    0x00 //16.384s

typedef struct
{
    IWDG_NumTypeDef WDTx;
    IWDG_ModeTypedef MODE;
    uint8_t SETLOAD;
} IWDG_InitTypeDef;

void IWDG_Init(IWDG_InitTypeDef *iwdg_init_struct);
void IWDG_SetReload(IWDG_InitTypeDef *iwdg_init_struct);
void IWDG_ReloadCounter(IWDG_InitTypeDef *iwdg_init_struct);
void IWDG_Enable(IWDG_NumTypeDef wdtx);
void IWDG_Disable(IWDG_NumTypeDef wdtx);
void IWDG_Start(IWDG_InitTypeDef *iwdg_init_struct);
void WDT_IntrClear(IWDG_NumTypeDef wdtx);
void WDT_IntrEn(void);

#endif /* _YC11XX_IWDG_H_ */
