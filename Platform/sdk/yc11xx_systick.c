/*
 * Copyright (c) 2006-2020, YICHIP Development Team
 * @file     yc_timer.c
 * @brief    source file for setting timer
 *
 * Change Logs:
 * Date            Author             Version        Notes
 * 2021-3-15       zhouduo            V1.0.0         the first version
 * 2021-07-15      Lichengrun         V1.1.0         the formal version
 * note:  temporary version,under revision
 */

#include "yc11xx_systick.h"
#include "reg_addr.h"
#include "yc_nvic.h"
#include "yc_debug.h"


static SysTickIRQ_CB_Handle g_systickIrqCallback;
static uint32_t g_systick_unitUs;

/**
 * @brief  systick enable,interrupt enable ,load value
 * @param    reload_value
 * @return
 */
uint32_t SysTick_Config(uint32_t reload_value)
{
    if ((reload_value - 1UL) > SysTick_LOAD_RELOAD_Msk)
    {
        return (1UL);   /* Reload value impossible */
    }

    reload_value -= 1;
    *INITCPU_SYST_CSR &= ~ (((uint32_t)1) << INITCPU_SYST_CSR_ENABLE);
    *INITCPU_SYST_RVR = reload_value;
    *INITCPU_SYST_CVR = reload_value; //clear the current value

    *INITCPU_SYST_CSR |= ((INITCPU_SYSTICK_SYSCLOCK << INITCPU_SYST_CSR_CLKSOURCE) | \
                          (1 << INITCPU_SYST_CSR_ENABLE) | (1 << INITCPU_SYST_CSR_TICKINT));
    return 0;
}
/**
 * @brief  change the system DPLL to normal  DPLL
 * @param  None
 * @return None
 */
uint32_t SysTick_TimerGet1usTicks(void)
{
    uint8_t clock_xtal_choose = HREAD(CORE_CLOCK_SELECT) & 0x03;
    uint8_t tmpVal = (HREAD(CORE_CLOCK_CTRL) >> 1) & 0x03;
    uint8_t rtnVal;

    if(0 == clock_xtal_choose) {
        return SYSTEM_XTAL_CLOCK_24M;
    }
    
    switch(tmpVal)
    {
    case CLOCK_DPLL_48M_multiple:
    {
        rtnVal = SYSTEM_DPLL_CLOCK_48M;
        break;
    }
    case CLOCK_DPLL_153M_multiple:
    {
        rtnVal = SYSTEM_DPLL_CLOCK_153M;
        break;
    }
    case CLOCK_DPLL_96M_multiple:
    {
        rtnVal = SYSTEM_DPLL_CLOCK_96M;
        break;
    }
    case CLOCK_DPLL_192M_multiple:
    {
        rtnVal = SYSTEM_DPLL_CLOCK_192M;
        break;
    }

    default:
        break;
    }

    return rtnVal;
}

/**
 * @brief  Initialize the system clock with.
 * @param  system_clk can be CLOCK_48M_multiple.
 * @param  uint_us: One interrupt  in a period of us.
 * @param  callback: function pointer.
 * @return None
 */
void SysTick_Init(void)
{
    g_systick_unitUs = SYSTICK_DEFAULT_COUNTER;
    SysTick_Config(SysTick_TimerGet1usTicks() * g_systick_unitUs);
}

/**
 * @brief  Initialize the system clock.
 * @param  system_clk can be CLOCK_48M_multiple.
 * @param  uint_us: One interrupt in a period of us.
 * @param  callback: function pointer.
 * @return None
 */
void SysTick_IrqInit(uint32_t system_clk, uint32_t uint_us, SysTickIRQ_CB_Handle callback)
{
    g_systickIrqCallback = callback;
    g_systick_unitUs = uint_us;

    if(g_systickIrqCallback != NULL) {
        NVIC_Configuration(rv_tm_IRQn, 14, ENABLE);
    }

    if(CLOCK_XTAL_24M_multiple == system_clk) {
        System_LdoCoreVsel(0);
        SysTick_Config(SysTick_TimerGet1usTicks() * g_systick_unitUs);
    }
    else {
        System_ChangeDPLL(system_clk);
    }
}

bool SysTick_CheckInFastDpll(void)
{
    uint8_t tmp = HREAD(CORE_CLOCK_CTRL);
    uint8_t system_clk = (tmp >> 1) & 0x03;
    return (system_clk != CLOCK_DPLL_48M_multiple);
}

void System_LdoCoreVsel(uint8_t svel)
{
    LPM_DaLdoRegDef lpmldoctr;
    uint32_t lpmtempval = 0;
    uint8_t *ptr = NULL;
    
    //step1: read 
    HREADL_STRUCT(CORE_LDO_CTRL, &lpmldoctr);
    ptr = (uint8_t *)&lpmldoctr;
    lpmldoctr.da_ldo_dig_core_vsel = 0x07 & svel ;
    lpmtempval = *ptr + (*(ptr+1) << 8) + (*(ptr+2) << 16) + (*(ptr+3) << 24);

    //step2: write
    CoreReg_LpmWrite2LdoControl(lpmtempval);
}

/**
 * @brief  Modify the system DPLL.
 * @param  system_clk: system DPLL only can be  CLOCK_48M_multiple.
 * @return None
 */
void System_ChangeDPLL(SYSTEM_CLOCK_TypeDef system_clk)
{
    uint8_t tmp = HREAD(CORE_CLOCK_CTRL);
    uint8_t tmpVal = system_clk;
    
    tmp &= 0xf9;
    tmp |= tmpVal << 1;

    System_LdoCoreVsel(4);

    HWRITE(CORE_CLOCK_CTRL, tmp);
    HWRITE(CORE_CLOCK_SELECT, 0x05);
    SysTick_Config(SysTick_TimerGet1usTicks() * g_systick_unitUs);
    wait_lpodelay(2); //Delay a while for work stable
}

/**
 * @brief  Modify the system DPLL.
 * @param  system_clk: system DPLL only can be  CLOCK_48M_multiple.
 * @return None
 */
void System_ChangeXtal24M(void)
{
    HWRITE(CORE_CLOCK_SELECT, 0x04);
    SysTick_Config(SysTick_TimerGet1usTicks() * g_systick_unitUs);
    wait_lpodelay(2);

    System_LdoCoreVsel(0);
}

uint32_t System_WorkClockGet(void)
{
    uint32_t rtnVal = 0;
    uint8_t clock_choose = HREAD(CORE_CLOCK_SELECT)  & 0x03;
    uint8_t mcu_clock_sel =( HREAD(CORE_CLOCK_CTRL)  & 0x06) >> 1;
    
    switch(clock_choose)
    {
        case CLOCK_SELECT_XTAL:
            rtnVal = SYSTEM_XTAL_CLOCK_24M*MHz;
            break;
        case CLOCK_SELECT_DPLL:
           if(CLOCK_DPLL_48M_multiple == mcu_clock_sel)
           {
                rtnVal = SYSTEM_DPLL_CLOCK_48M*MHz;
           }
           else if(CLOCK_DPLL_153M_multiple == mcu_clock_sel)
           {
                rtnVal = SYSTEM_DPLL_CLOCK_153M*MHz;
           }
           else if(CLOCK_DPLL_96M_multiple == mcu_clock_sel)
           {
                rtnVal = SYSTEM_DPLL_CLOCK_96M*MHz;
           }
           else
           {
                rtnVal = SYSTEM_DPLL_CLOCK_192M*MHz;
           }
      
            break;
       case CLOCK_SELECT_RC:
            break;
       case CLOCK_SELECT_LPOCLOCK:
            break;
     default:
            break;
    }

    return rtnVal;
}

void SysTick_DelayMs(uint32_t nms)
{
    SysTick_DelayUs(nms * 1000);
}

/**
  * @brief  Perform a certain amount of time delay.
  * @param  nus:Prescribed length.
  * @return None
  */
void SysTick_DelayUs(uint32_t nus)
{
    uint32_t ticks = 0;
    uint32_t told = 0, tnow = 0, tcnt = 0;
    ticks = nus * SysTick_TimerGet1usTicks();

    uint32_t reload = (SysTick_TimerGet1usTicks() * g_systick_unitUs) - 1;
    told = *(INITCPU_SYST_CVR);
    
    while(1)
    {
        tnow = *(INITCPU_SYST_CVR);
        
        if(tnow != told)
        {
            if(tnow < told) {
                tcnt += told - tnow;
            }
            else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            
            if(tcnt >= ticks) {
                break;
            }
        }
    }
}

/**
  * @brief  Interrupt handler function.
  * @param  None
  * @return None
  */
void RV_TM_IRQHandler(void)
{
    if(g_systickIrqCallback != NULL) {
        g_systickIrqCallback();
    }
}






