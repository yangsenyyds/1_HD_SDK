/*
File Name    : system.c
Author       : Yichip
Version      : V1.0
Date         : 2019/11/13
Description  : none.
*/
#include "yc11xx.h"
#include "btreg.h"
#include "reg_addr.h"
#include "yc11xx_systick.h"


uint16_t gOsEnterCriticalCnt;
/*************************COMMON FUNC***************************/
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
void Toolmemcpy(uint8_t *dest, const uint8_t *src, uint16_t len)
{
    for(int i = 0; i < len; i++)
    {
        HWRITE((int)(dest + i), (HREADTool((int)src + i)));
    }
    return;
}
void xmemcpy(uint8_t *dest, const uint8_t *src, uint16_t len)
{
    for(int i = 0; i < len; i++)
    {
        HWRITE((int)(dest + i), (HREAD((int)src + i)));
    }
    return;
}

uint16_t xstrlen(const char *s)
{
    int n;
    for(n = 0; *s != '\0'; s++)
    {
        n++;
    }
    return n;
}

uint8_t hexchar2hex(char hexchar)
{
    if(hexchar >= '0' && hexchar <= '9')
        return hexchar - '0';
    if(hexchar >= 'a' && hexchar <= 'f')
        return hexchar - 'a' + 10;
    if(hexchar >= 'A' && hexchar <= 'F')
        return hexchar - 'A' + 10;
    return -1;
}

void error_handle()
{
    OS_ENTER_CRITICAL();
    while(1);
}
void _assert_handler(const char *file, int line, const char *func)
{
    while (1);
}

void whileDelay(int delayValue)
{
    volatile int delayMS;
    for (int k = 0; k < delayValue; k++)
    {
        delayMS = 20000;
        while(delayMS--);
    }
}
void whileDelayshort(int delayValue)
{
    volatile int delayMS;
    for (int k = 0; k < delayValue; k++)
    {
        delayMS = 2000;
        while(delayMS--);
    }
}

bool xramcmp(volatile uint8_t *op1, volatile uint8_t *op2, int len)
{
    for(int i = 0; i < len; i++)
    {
        if (HREAD((int)op1 + i) != HREAD((int)op2 + i))
            return false;
    }
    return true;
}

void xramcpy(volatile uint8_t *des, volatile uint8_t *src, int len)
{
    for(int i = 0; i < len; i++)
    {
        HWRITE(((int)des + i), (HREAD((int)src + i)));
    }
}


uint32_t math_abs(int32_t value)
{
    return (value < 0) ? (-value) : value;
}
uint32_t CoreReg_ClkControl(CORE_ClockOffEnumDef clkIndex, bool isEnable)
{
    uint16_t temp;

    if(clkIndex & 0x10000)
    {
        temp = HREADW(CORE_CLKOFF2);

        if (isEnable)
        {
            temp &= ~(uint8_t)clkIndex;
        }
        else
        {
            temp |= (uint8_t)clkIndex;
        }

        HWRITEW(CORE_CLKOFF2, temp);
    }
    else
    {
        temp = HREADW(CORE_CLOCK_OFF);

        if (isEnable)
        {
            temp &= ~clkIndex;
        }
        else
        {
            temp |= clkIndex;
        }

        HWRITEW(CORE_CLOCK_OFF, temp);
    }
}

void __attribute__((noinline)) SetLockLpmWriteReg(void)
{
    HWRITE_INLINE(mem_lpm_reg_lock_flag_cm0, 1);// I'm interested
    HWRITE_INLINE(mem_lpm_reg_lock_victim, DOUBLE_LOCK_VICTIM_CM0);// you go first

    while(((HREAD_INLINE(mem_lpm_reg_lock_flag_respin)) != 0)
            && ((HREAD_INLINE(mem_lpm_reg_lock_victim)) == DOUBLE_LOCK_VICTIM_CM0));// wait
}

void __attribute__((noinline)) SetReleaseLpmWriteReg(void)
{
    HWRITE_INLINE(mem_lpm_reg_lock_flag_cm0, 0);
}

void CoreReg_LpmWriteCommon(uint32_t writeVal, uint16_t writeLpmReg, uint8_t writeLpmVal)
{
    OS_ENTER_CRITICAL();
    SetLockLpmWriteReg();
    HWRITEL(CORE_LPM_REG, writeVal);
    HWOR(writeLpmReg, writeLpmVal);
    wait_lpodelay(1000);
    SetReleaseLpmWriteReg();
    OS_EXIT_CRITICAL();
}
void CoreReg_LpmWrite(uint32_t writeVal, uint8_t writeLpmVal)
{
    CoreReg_LpmWriteCommon(writeVal, CORE_LPM_WR, writeLpmVal);
}
void CoreReg_LpmWriteCtrl(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_CTRL);
}
void CoreReg_LpmWriteCtrl2(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_CTRL2);
}
void CoreReg_LpmWriteGpioLowWake(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_GPIOLOW_WAKE);
}
void CoreReg_LpmWriteGpioHighWake(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_GPIOHIGH_WAKE);
}
void CoreReg_LpmWriteCounter(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_COUNTER);
}
void CoreReg_LpmWriteBuckCfg(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_BUCK_CFG);
}
void CoreReg_LpmWriteChargeCtrl(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_CHARGE_CTRL);
}
void CoreReg_LpmWriteXtal32kCtrl(uint32_t writeVal)
{
    CoreReg_LpmWrite(writeVal, LPMREG_SEL_XTAL32K_CTRL);
}

void CoreReg_LpmWrite2(uint32_t writeVal, uint8_t writeLpmVal)
{
    CoreReg_LpmWriteCommon(writeVal, CORE_LPM_WR2, writeLpmVal);
}
void CoreReg_LpmWrite2EnterScanMode(uint32_t writeVal)
{
    CoreReg_LpmWrite2(writeVal, LPMREG2_SEL_ENTER_SCAN_MODE);
}
void CoreReg_LpmWrite2ChargeLowHighWakeup(uint32_t writeVal)
{
    CoreReg_LpmWrite2(writeVal, LPMREG2_SEL_CHARGE_LOW_HIGH_WAKEUP_REG);
}
void CoreReg_LpmWrite2LdoControl(uint32_t writeVal)
{
    CoreReg_LpmWrite2(writeVal, LPMREG2_SEL_LDO_CONTROL);
}
void CoreReg_LpmWrite2Gpio2TouchWake(uint32_t writeVal)
{
    CoreReg_LpmWrite2(writeVal, LPMREG2_SEL_GPIO2_TOUCH_WAKE);
}
void CoreReg_LpmWrite2IceWdtLongRst(uint32_t writeVal)
{
    CoreReg_LpmWrite2(writeVal, LPMREG2_SEL_ICE_WDT_LONG_RST_MODE);
}

uint32_t CoreReg_LpmGetIceWdtLongRst(void)
{
    uint32_t val = 0;
    /* here has error*/
    val = HREADL(CORE_ICE_WDT_MODE);
    return val;
}

void CoreReg_LpmAppRegRead(LPM_AppCtrlRegDef *lpmCtrl)
{
    HREADL_STRUCT(CORE_BUCK_CTRL, lpmCtrl);
}
void CoreReg_LpmAppRegWrite(LPM_AppCtrlRegDef *lpmCtrl)
{
    CoreReg_LpmWriteBuckCfg(*(uint32_t *)(lpmCtrl));
}
void CoreReg_LpmAppRegFirstPowerOnControl(bool isPowerOn)
{
    LPM_AppCtrlRegDef lpmCtrl;
    CoreReg_LpmAppRegRead(&lpmCtrl);
    lpmCtrl.user_bit_second_power_on = isPowerOn;
    CoreReg_LpmAppRegWrite(&lpmCtrl);
}
void CoreReg_LpmAppRegChargePowerOffControl(bool isChargePowerOff)
{
    LPM_AppCtrlRegDef lpmCtrl;
    CoreReg_LpmAppRegRead(&lpmCtrl);
    lpmCtrl.user_bit_charge_power_off = isChargePowerOff;
    CoreReg_LpmAppRegWrite(&lpmCtrl);
}

/**
 * @brief   Adc 12M  clock config
 * @param    init:
 * @param    frac :
 * @return  None
 */
void Audio_AdcDSMDIV_12MConfig(short init, int frac)
{
    //    DEBUG_LOG_STRING("dsmdiv_12m_cfg, init: 0x%x, frac: 0x%x.\r\n", init, frac);
    HWRITE(CORE_DSMDIV_12M_INT,   init);
    HWRITE24BIT(CORE_DSMDIV_12M_FRAC,   frac);
    //cfg_update
    HWCOR(CORE_DSMDIV_CTRL, BIT7);
    HWOR(CORE_DSMDIV_CTRL, BIT7);
}
/**
 * @brief  Adc 12M  clock enable
 * @param    en:     enable dsmdiv_12m
 * @param    dither_en  :dither_enable
 * @param    div2 :   0: output div1:  1: output div2
 * @return  None
 */
void Audio_AdcDSMDIV_12MEnable(short en, short dither_en, short div2)
{
    short cfg;
    //clr_cfg_update, add for default value load
    HWCOR(CORE_DSMDIV_CTRL, BIT7);

    //cfg_update
    cfg = HREAD(CORE_DSMDIV_CTRL);
    cfg = (cfg & 0x0f) + (((1 << 3) + (div2 << 2) + (dither_en << 1) + (en << 0)) << 4);
    HWRITE(CORE_DSMDIV_CTRL, cfg);
}
/**
 * @brief  Adc 11M  clock config
 * @param    init
 * @param    frac
 * @return  None
 */
void Audio_AdcDSMDIV_11MConfig(short init, int frac)
{
    HWRITE(CORE_DSMDIV_11M_INT,   init);
    HWRITE24BIT(CORE_DSMDIV_11M_FRAC,   frac);
    //cfg_update
    HWCOR(CORE_DSMDIV_CTRL, BIT3);
    HWOR(CORE_DSMDIV_CTRL, BIT3);
}
/**
 * @brief  Adc 11M  clock enable
 * @param    en:     enable dsmdiv_11m
 * @param    dither_en  :dither_enable
 * @param    div2 :   0: output div1:  1: output div2
 * @return  None
 */
void Audio_AdcDSMDIV_11MEnable(short en, short dither_en, short div2)
{
    short cfg;
    //clr_cfg_update, add for default value load
    HWCOR(CORE_DSMDIV_CTRL, BIT3);
    //cfg_update
    cfg = HREAD(CORE_DSMDIV_CTRL);
    cfg = (cfg & 0xf0) + (((1 << 3) + (div2 << 2) + (dither_en << 1) + (en << 0)));

    HWRITE(CORE_DSMDIV_CTRL, cfg);
}
/**
 * @brief   Adc clock initialize
 * @param    void
 * @return  None
 */
void Audio_ClkInit(void)//Init_Audio_Clk
{
#ifdef FUNCTION_FPGA_WORK
    // In FPGA source is 192M
    // 192/12.288 = 15.625, 0x0f is <init>, 0.625*2^24=0xA00000
    Audio_AdcDSMDIV_12MConfig(0x0f, 0x0A00000);
    //en, dither_en, div2
    Audio_AdcDSMDIV_12MEnable(1, 0, 0);
    // 192/11.2896 = 17.006802721088435374149659863946, 0x11 is <init>, 0.006802721088435374149659863946*2^24=0x1BDD3
    Audio_AdcDSMDIV_11MConfig(0x11, 0x001BDD3);
    //en, dither_en, div2
    Audio_AdcDSMDIV_11MEnable(1, 0, 0);
#else
    // In Real Work source is 384M
#ifndef CLOCK_HALVED
    // 384/12.288 = 31.25, 0x1f is <init>, 0.25*2^24=0x400000
    Audio_AdcDSMDIV_12MConfig(0x1f, 0x400000);
    //en, dither_en, div2
    Audio_AdcDSMDIV_12MEnable(1, 0, 0);
    // 384/11.2896 = 34.01360544217687, 0x22 is <init>, 0.01360544217687*2^24=0x37ba5
    Audio_AdcDSMDIV_11MConfig(0x22, 0x037ba5);
    //en, dither_en, div2
    Audio_AdcDSMDIV_11MEnable(1, 0, 0);
#else
    // 384/6.144  = 62.5 , 0x3E is <init>, 0.5 *2^24=0x800000
    Audio_AdcDSMDIV_12MConfig(0x3e, 0x800000);
    //en, dither_en, div2
    Audio_AdcDSMDIV_12MEnable(1, 0, 0);
    // 384/5.6448  = 68.02721088435374 , 0x44 is <init>, 0.02721088435374 *2^24=0x06f74a
    Audio_AdcDSMDIV_11MConfig(0x44, 0x06f74a);
    //en, dither_en, div2
    Audio_AdcDSMDIV_11MEnable(1, 0, 0);
#endif

#endif
}

void wait_lpodelay(uint32_t nus)
{
    uint16_t cpu_mhz = System_WorkClockGet()/MHz/2;
	lpo_delay(cpu_mhz * nus);
}

void SYS_RESET(void)
{
    HWRITE(CORE_CMD1, 0x03);
}
