/*
		 * Copyright (c) 2006-2021, YICHIP Development Team
		 * @file     YC_STM32_iwdg.c
		 * @brief    source file for setting wdg
		 *
		 * Change Logs:
		 * Date           Author      	Version        Notes
		 * 2021-02-08     Yichip 	V1.0.0         the first version
		 */
/* Includes ------------------------------------------------------------------*/
#include "yc11xx_iwdg.h"
#include "reg_addr.h"
/* ---------------------- IWDG registers bit mask ----------------------------*/

/**
 * @brief  Initialize the WDT.
 * @param  wdtx: Select the WDT.WDT,WDT2
 *         MODE: Reset and interrupt MODEs.RESET_MODE,INTR_MODE
 *         SETLOAD: Different time intervals.
 * @return None
 */
void IWDG_Init(IWDG_InitTypeDef *iwdg_init_struct)
{
	_ASSERT(IS_WDTX(iwdg_init_struct->WDTx));
	_ASSERT(IS_WDT_MODE(iwdg_init_struct->MODE));
    uint8_t en_rst = HREAD(CORE_SYSTEM_CTRL);
    if (RESET_MODE == iwdg_init_struct->MODE)
    {
        en_rst &= ~(BIT1);
		HWRITE(CORE_WDT_COUNTER_BASE(iwdg_init_struct->WDTx), iwdg_init_struct->SETLOAD);
    }
    else if (INTR_MODE == iwdg_init_struct->MODE)
    {
        en_rst |= BIT1;
        HWRITE(CORE_WDT_COUNTER_BASE(iwdg_init_struct->WDTx), iwdg_init_struct->SETLOAD);
    }
    HWRITE(CORE_SYSTEM_CTRL, en_rst);
}

/**
 * @brief  set IWDG Reload value
 * @param  Reload:Invalid parameter
 * @return None
 */
void IWDG_SetReload(IWDG_InitTypeDef *iwdg_init_struct)
{
	HWRITE(CORE_WDT_COUNTER_BASE(iwdg_init_struct->WDTx), iwdg_init_struct->SETLOAD);
	wait_lpodelay(90);
}

/**
 * @brief  feeding watchdog.
 * @param  None
 * @return None
 */
void IWDG_ReloadCounter(IWDG_InitTypeDef *iwdg_init_struct)
{
    IWDG_SetReload(iwdg_init_struct);
}

/**
 * @brief  enable WDT.
 * @param  iwdg_init_struct:init
 * @return None
 */
void IWDG_Enable(IWDG_NumTypeDef wdtx)
{
	_ASSERT(IS_WDTX(wdtx));
    uint32_t wdt_en = 0;
    wdt_en = CoreReg_LpmGetIceWdtLongRst();
    wdt_en |= (wdt_en_bit(wdtx));
    CoreReg_LpmWrite2IceWdtLongRst(wdt_en);
}
/**
 * @brief  disable WDT.
 * @param  IWDG_NumTypeDef:wdtx
 * @return None
 */
void IWDG_Disable(IWDG_NumTypeDef wdtx)
{
	_ASSERT(IS_WDTX(wdtx));
    uint32_t wdt_en;
    wdt_en = CoreReg_LpmGetIceWdtLongRst();
    wdt_en &= ~(wdt_en_bit(wdtx));
    CoreReg_LpmWrite2IceWdtLongRst(wdt_en);
}

/**
 * @brief  start watchdog.
 * @param  WDT_init_struct:init
 * @return None
 */
void IWDG_Start(IWDG_InitTypeDef *iwdg_init_struct)
{
	_ASSERT(IS_WDTX(iwdg_init_struct->WDTx));
    IWDG_Disable(iwdg_init_struct->WDTx);
    WDT_IntrClear(iwdg_init_struct->WDTx);
    IWDG_Init(iwdg_init_struct);
    IWDG_Enable(iwdg_init_struct->WDTx);
}

/**
  * @brief  Checks whether the specified IWDG flag is set or not.
  * @param  IWDG_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg IWDG_FLAG_PVU: Prescaler Value Update on going
  *     @arg IWDG_FLAG_RVU: Reload Value Update on going
  * @return The new state of IWDG_FLAG (SET or RESET).
  */
void WDT_IntrClear(IWDG_NumTypeDef wdtx)
{
	_ASSERT(IS_WDTX(wdtx));
    uint8_t wdt_en;
    wdt_en = HREAD((CORE_SYS_EVENT_CLEAN));
    wdt_en &= wdt_intr_clr(wdtx);
    HWRITE((CORE_SYS_EVENT_CLEAN), wdt_en);
    wdt_en = HREAD((CORE_SYS_EVENT_CLEAN));
}

/**
  * @brief  Watchdog interrupt MODE enable.
  * @param  None
  * @return None
  */
void WDT_IntrEn(void)
{
    uint8_t en_rst;
    en_rst = HREAD(CORE_SYSTEM_CTRL);
    en_rst &= ~ WDT_INTR_BIT;
    HWRITE(CORE_SYSTEM_CTRL, en_rst);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
