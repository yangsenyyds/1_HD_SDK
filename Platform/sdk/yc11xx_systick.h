#ifndef _YC11XX_SYSTICK_H_
#define _YC11XX_SYSTICK_H_


#include "yc11xx.h"
/**
  *@brief system tick.
  */
#define SYSTEM_CURRENT_CLOCK  CLOCK_XTAL_24M_multiple

#define SysTick_LOAD_RELOAD_Msk            (0xFFFFFFUL /*<< SysTick_LOAD_RELOAD_Pos*/)    /*!< SysTick LOAD: RELOAD Mask */  
#define SYSTEM_XTAL_CLOCK_24M   (24)
#define SYSTEM_DPLL_CLOCK_48M   (48)
#define SYSTEM_DPLL_CLOCK_96M   (96)
#define SYSTEM_DPLL_CLOCK_153M  (153)
#define SYSTEM_DPLL_CLOCK_192M  (192)

#define SYSTICK_DEFAULT_COUNTER  10000

#define CLOCK_SELECT_XTAL 0
#define CLOCK_SELECT_DPLL 1
#define CLOCK_SELECT_RC   2
#define CLOCK_SELECT_LPOCLOCK 3

typedef enum {
  CLOCK_DPLL_48M_multiple,
  CLOCK_DPLL_153M_multiple,
  CLOCK_DPLL_96M_multiple,
  CLOCK_DPLL_192M_multiple,
  CLOCK_XTAL_24M_multiple,
}SYSTEM_CLOCK_TypeDef;

/**
  *@brief Keypad event callback function.
  */
typedef void (*SysTickIRQ_CB_Handle)(void);

void System_ChangeXtal24M(void);
void SysTick_Init(void);
void SysTick_IrqInit(SYSTEM_CLOCK_TypeDef system_clk, uint32_t uint_us, SysTickIRQ_CB_Handle callback);
uint32_t SysTick_Config(uint32_t reload_value);
bool SysTick_CheckInFastDpll(void);
void System_ChangeDPLL(SYSTEM_CLOCK_TypeDef system_clk);
void System_LdoCoreVsel(uint8_t  svel);
void SysTick_ChangeToFastDPLL(void);
void SysTick_ChangeToNormalDPLL(void);
void SysTick_DelayMs(uint32_t nms);
void SysTick_DelayUs(uint32_t nus);
uint32_t System_WorkClockGet(void);
#endif  /* _YC11XX_SYSTICK_H_ */
