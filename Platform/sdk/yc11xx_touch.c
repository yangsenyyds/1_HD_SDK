/*
 * Copyright (c) 2006-2020, YICHIP Development Team
 * @file     touch.c
 * @brief    source file for setting timer
 *
 * Change Logs:
 * Date            Author             Version        Notes
 * 2021-4-26       WJJ            V 1.0.0         the first version
 * note:  temporary version  revision
 */
#include "yc_debug.h"
#include "yc11xx_touch.h"
#include "yc11xx_systick.h"
#include "yc11xx.h"
/**
 * @brief  	Touch Register initialization
 * @param   touch_init_struct:Register configuration
 * @return  None
 */
void TOUCH_Init(TOUCH_InitTypeDef *touch_init_struct)
{
    uint32_t touch_ctrl = 0;
    TOUCH_PadProtect(touch_init_struct->TS_FIR_CTRL.pad_sel_tch1 ,touch_init_struct->TS_FIR_CTRL.pad_sel_tch2);

    touch_ctrl = TS_BL1_DEFAULT;
    TOUCH_WriteReg(Ts_Bl1, touch_ctrl);

    touch_ctrl = TS_BL2_DEFAULT;
    TOUCH_WriteReg(Ts_Bl2, touch_ctrl);

    TOUCH_WriteReg(Ts_Bl4, TS_BL4_DEFAULT);
    touch_ctrl = ((touch_init_struct->TS_BL4_CTRL.ts_ch1_baseline_cfg << 0)  |	\
                    (touch_init_struct->TS_BL4_CTRL.ts_ch2_baseline_cfg << 12));
    TOUCH_WriteReg(Ts_Bl4, touch_ctrl);
    touch_ctrl = ((touch_init_struct->TS_BL3_CTRL.ts_ch1_fdl << 0) 			|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch1_tfrt_cfg << 4)		|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch1_tfrt_en << 7)		|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch1_track_en << 13)		|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch1_baseline_wr << 14)	|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch2_fdl << 16) 			|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch2_tfrt_cfg << 20)		|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch2_tfrt_en << 23)		|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch2_track_en << 29)		|	\
                    (touch_init_struct->TS_BL3_CTRL.ts_ch2_baseline_wr << 30)	|	\
                    TS_BL3_DEFAULT);
    TOUCH_WriteReg(Ts_Bl3, touch_ctrl);


    touch_ctrl = ((touch_init_struct->TS_CMP1_CTRL.ts_ch1_dr_cfg << 0)		|	\
                    (touch_init_struct->TS_CMP1_CTRL.ts_ch1_dt_cfg << 4)		|	\
                    (touch_init_struct->TS_CMP1_CTRL.ts_ch1_rth_cfg << 8)		|	\
                    (touch_init_struct->TS_CMP1_CTRL.ts_ch1_tth_cfg << 16)		|	\
                    TS_CMP1_DEFAULT);
    TOUCH_WriteReg(Ts_Cmp1, touch_ctrl);

    touch_ctrl = ((touch_init_struct->TS_CMP2_CTRL.ts_ch2_dr_cfg << 0)		|	\
                    (touch_init_struct->TS_CMP2_CTRL.ts_ch2_dt_cfg << 4)		|	\
                    (touch_init_struct->TS_CMP2_CTRL.ts_ch2_rth_cfg << 8)		|	\
                    (touch_init_struct->TS_CMP2_CTRL.ts_ch2_tth_cfg << 16)		|	\
                    TS_CMP2_DEFAULT);
    TOUCH_WriteReg(Ts_Cmp2, touch_ctrl);

    touch_ctrl = ((touch_init_struct->TS_CAL1_CTRL.ts_ch1_lsl << 0)			|	\
                    (touch_init_struct->TS_CAL1_CTRL.ts_ch1_usl << 10)			|	\
                    (touch_init_struct->TS_CAL1_CTRL.ts_ch1_tl << 20)			|	\
                    TS_CAL1_DEFAULT);
    TOUCH_WriteReg(Ts_Cal1, touch_ctrl);
    DEBUG_LOG_STRING("Ts_Cal1\r\n");

    touch_ctrl = ((touch_init_struct->TS_CAL2_CTRL.ts_ch2_lsl << 0)			|	\
                    (touch_init_struct->TS_CAL2_CTRL.ts_ch2_usl << 10)			|	\
                    (touch_init_struct->TS_CAL2_CTRL.ts_ch2_tl << 20)			|	\
                    TS_CAL2_DEFAULT);
    TOUCH_WriteReg(Ts_Cal2, touch_ctrl);

    touch_ctrl = ((touch_init_struct->TS_CAL3_CTRL.ts_ch1_ibc_init << 4)		|	\
                    (touch_init_struct->TS_CAL3_CTRL.ts_ch1_cdc_init << 8)		|	\
                    (touch_init_struct->TS_CAL3_CTRL.ts_ch2_ibc_init << 20)	|	\
                    (touch_init_struct->TS_CAL3_CTRL.ts_ch2_cbc_init << 24)	|	\
                    TS_CAL3_DEFAULT);
    TOUCH_WriteReg(Ts_Cal3, touch_ctrl);

    touch_ctrl = ((touch_init_struct->TS_CAL4_CTRL.ts_ch1_cal_err_th << 0) 	|	\
                    (touch_init_struct->TS_CAL4_CTRL.ts_ch2_cal_err_th << 8)	|	\
                    TS_CAL4_DEFAULT);
    TOUCH_WriteReg(Ts_Cal4, touch_ctrl);

    touch_ctrl = ((touch_init_struct->TS_FIR_CTRL.ts_ch1_ffi_cfg << 0)       | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch1_avg_mode << 3)        | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch1_sfi_cfg << 4)         | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch1_en << 7)              | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch2_ffi_cfg << 8)         | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch2_avg_mode << 11)   | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch2_sfi_cfg << 12)    | \
                    (touch_init_struct->TS_FIR_CTRL.ts_ch2_en << 15)             | \
                    (touch_init_struct->TS_FIR_CTRL.ts_clk_div << 16)            | \
                    (touch_init_struct->TS_FIR_CTRL.pad_sel_tch2 << 26)             | \
                    (touch_init_struct->TS_FIR_CTRL.pad_sel_tch1 << 28)            | \
                    TS_FIR_DEFAULT);
    TOUCH_WriteReg(Ts_Fir, touch_ctrl);

    touch_ctrl = (
                       (touch_init_struct->TS_ANA_CTRL.lpm_ts_ldo_en << 4)              |  \
                       TS_ANA_DEFAULT);
    TOUCH_WriteReg(Ts_Ana, touch_ctrl);

    wait_lpodelay(100);

    touch_ctrl = (
                       (touch_init_struct->TS_ANA_CTRL.tch2_en << 29)             | \
                       (touch_init_struct->TS_ANA_CTRL.ts_en << 30)              |  \
                       (touch_init_struct->TS_ANA_CTRL.lpm_ts_ldo_en << 4)              |  \
                       TS_ANA_DEFAULT);
    TOUCH_WriteReg(Ts_Ana, touch_ctrl);
}

/**
 * @brief    Touch enable switch
 * @param    newstate: ENABLE/DISABLE
 * @return   None
 */
void TOUCH_Set( FunctionalState newstate)
{
    uint32_t touch_ctrl = 0;
    touch_ctrl = TOUCH_ReadReg(Ts_Ana);
    if(newstate == ENABLE)
    {
        touch_ctrl = (newstate << 30 | touch_ctrl);
        TOUCH_WriteReg(Ts_Ana, touch_ctrl);
        touch_ctrl = TOUCH_ReadReg(Ts_Fir) & ~ ((BIT7) | (BIT15));
        touch_ctrl = touch_ctrl | (newstate << 7) | (newstate << 15);
        TOUCH_WriteReg(Ts_Fir, touch_ctrl);
    }
    else
    {
        touch_ctrl = ((touch_ctrl) & ~ (1 << 30));
        TOUCH_WriteReg(Ts_Ana, touch_ctrl);
        touch_ctrl = TOUCH_ReadReg(Ts_Fir) & ~ ((BIT7) | (BIT15));
        touch_ctrl = touch_ctrl | (newstate << 7) | (newstate << 15);
        TOUCH_WriteReg(Ts_Fir, touch_ctrl);
    }
}


/**
 * @brief 	 Protect the gpios of touch pad away from leakaging electricity and wrong wakeup
 * @param    pad1/pad2: Ch1_Pad/Ch2_Pad
 * @return   None
 */
void TOUCH_PadProtect(Touch_CH1_PAD_SELCT ch1, Touch_CH2_PAD_SELCT ch2)
{
	switch(ch1)
	{
		case pad0_gpio33:
			GPIO_Init(GPIO_33,GPIO_Mode_Analog);
		break;
		case pad1_gpio12:
			GPIO_Init(GPIO_12,GPIO_Mode_Analog);
		break;
		case pad2_gpio9:
			GPIO_Init(GPIO_9,GPIO_Mode_Analog);
		break;
		case pad_shutdown2:
			GPIO_Init(GPIO_33,GPIO_Mode_Analog);
			GPIO_Init(GPIO_12,GPIO_Mode_Analog);
			GPIO_Init(GPIO_9,GPIO_Mode_Analog);
		break;

	}
	switch(ch2)
	{
		case pad0_gpio34:
			GPIO_Init(GPIO_34,GPIO_Mode_Analog);
		break;
		case pad1_gpio13:
			GPIO_Init(GPIO_13,GPIO_Mode_Analog);
		break;
		case pad2_gpio10:
			GPIO_Init(GPIO_10,GPIO_Mode_Analog);
		break;
		case pad_shutdown1:
			GPIO_Init(GPIO_34,GPIO_Mode_Analog);
			GPIO_Init(GPIO_10,GPIO_Mode_Analog);
			GPIO_Init(GPIO_13,GPIO_Mode_Analog);
		break;
	}
}


/**
 * @brief    Change pads when sixchannel function turn on
 * @param    pad1/pad2: Ch1_Pad/Ch2_Pad
 * @return   None
 */
void TOUCH_PadChange(uint8_t pad1, uint8_t pad2)
{
    TOUCH_InitTypeDef   touch_init_struct;
    uint32_t touch_ctrl = 0;
    touch_ctrl = ((TOUCH_ReadReg(Ts_Fir) & ~ (0x0f << 26) )| (pad2 << 26) | (pad1 << 28));
    TOUCH_WriteReg(Ts_Fir, touch_ctrl);
}

/**
 * @brief    Update baseline in different pads
 * @param    baseLine1/baseLine2: Ch1_baseLine/Ch2_baseLine
 * @return   None
 */
void TOUCH_BaselineChange(uint16_t  baseLine1, uint16_t baseLine2)
{
    uint32_t touch_ctrl = 0;
    if(baseLine1 > 0)
    {
        touch_ctrl = TOUCH_ReadReg(Ts_Bl4) & ~ (0xfff);
        touch_ctrl = (touch_ctrl | baseLine1);
        TOUCH_WriteReg(Ts_Bl4, touch_ctrl);

        touch_ctrl = TOUCH_ReadReg(Ts_Bl3) & ~ (BIT14);
        TOUCH_WriteReg(Ts_Bl3, touch_ctrl);
        touch_ctrl = (touch_ctrl | (BIT14));
        TOUCH_WriteReg(Ts_Bl3, touch_ctrl);
    }
    if(baseLine2 > 0)
    {
        touch_ctrl = TOUCH_ReadReg(Ts_Bl4) & ~ (0xfff << 12);
        touch_ctrl = (touch_ctrl | (baseLine2 << 12));
        TOUCH_WriteReg(Ts_Bl4, touch_ctrl);

        touch_ctrl = TOUCH_ReadReg(Ts_Bl3) & ~ (BIT30);
        TOUCH_WriteReg(Ts_Bl3, touch_ctrl);
        touch_ctrl = (touch_ctrl | (BIT30));
        TOUCH_WriteReg(Ts_Bl3, touch_ctrl);
    }
}

/**
 * @brief    Writes the parameters to the corresponding register
 * @param    touch_reg :The corresponding register name
 * @param    touch_ctrl :The value to write to the register
 * @return   None
 */
void TOUCH_WriteReg(Touch_Reg_TypeDef touch_reg, uint32_t touch_ctrl)
{
    OS_ENTER_CRITICAL();
    SetLockLpmWriteReg();

    HWRITEW(CORE_LPM_TS_CTRL, 0x0000);
    HWRITEL(CORE_LPM_REG, touch_ctrl);
    HWRITEW(CORE_LPM_TS_CTRL, 1 << touch_reg);

    SysTick_DelayMs(1);
    SetReleaseLpmWriteReg();
    OS_EXIT_CRITICAL();
}
/**
 * @brief    Read the value of the relevant register
 * @param    touch_reg: The register to read
 * @return
 */
uint32_t TOUCH_ReadReg(Touch_Reg_TypeDef touch_reg)
{
    uint32_t touch_readreg = 0;
    HWRITEW(CORE_LPM_TS_RD_MUX, 0x0000);
    /* Location 0x81f2  write 1 to read 804C-804f to the corresponding register */
    if(touch_reg > 7)
        HWRITEW(CORE_LPM_TS_RD_MUX, 1 << (touch_reg + 1));/* 0x81f2 bit 0 not read*/
    else
        HWRITEW(CORE_LPM_TS_RD_MUX, 1 << touch_reg);

    touch_readreg = HREADL(CORE_LPM_TS_CFG_STATUS);

    return touch_readreg;
}
/**
 * @brief    Read the status of touch:touch or relaese
 * @param    TOUCH_Chx_Num	touch_chx  :Ts_Ch1/Ts_Ch2
 * @return   the status of Ts_Ch1/Ts_Ch2,Returns 1 if touched, 0 otherwise
 */
TOUCH_Status TOUCH_ReadStatus(TOUCH_Chx_Num	touchx)
{
    uint8_t status = 0;

    status = HREAD(CORE_LPM_TS_STATUS);
    if(touchx == Ts_Ch1)
    {
        if(status & (1 << ch1_ts_status))
            return Ts_Touch_1;
        else
            return Ts_Release;
    }
    else
    {
        if(status & (1 << ch2_ts_tsatus))
            return Ts_Touch_2;
        else
            return Ts_Release;
    }
}
/**
 * @brief    Read the base value
 * @param    TOUCH_Chx_Num	touch_chx  :Ts_Ch1/Ts_Ch2
 * @return   Get the reference value for channel 1 or channel 2
 */
uint32_t TOUCH_ReadBaseline(TOUCH_Chx_Num touchx)
{
    uint32_t touch_baseline = 0;

    if(touchx != Ts_Ch2)
        touch_baseline = ((HREAD(CORE_LPM_TS_BASELINE + 1) & 0x0f) << 8) | \
                          HREAD(CORE_LPM_TS_BASELINE);
    else
        touch_baseline = ((HREAD(CORE_LPM_TS_BASELINE + 2) << 4) | \
                           (HREAD(CORE_LPM_TS_BASELINE + 1) & 0xf0) >> 4);
    return touch_baseline;
}
/**
 * @brief    Read the filter output data
 * @param    TOUCH_Chx_Num	ouch_chx  :Ts_Ch1/Ts_Ch2
 * @return   Read the value of filter output channel 1 or channel 2
 */
uint32_t TOUCH_ReadFirData(TOUCH_Chx_Num touchx)
{
    uint32_t touch_firdata = 0;
    if(touchx != Ts_Ch2)
    {
        touch_firdata = ((HREAD(CORE_LPM_TS_FIR2_DATA + 1) & 0x0f) << 8) |	 HREAD(CORE_LPM_TS_FIR2_DATA);
    }
    else
    {
        touch_firdata = ((HREAD(CORE_LPM_TS_FIR2_DATA + 2) << 4) | \
                          ((HREAD(CORE_LPM_TS_FIR2_DATA + 1) & 0xf0) >> 4));
    }
    return touch_firdata;
}

/**
 * @brief    select the clock source for touch
 * @param    TOUCH_Clk_Select	clk_sel: xo_32k/osc_32k
 * @return   None
 */
void  TOUCH_ChangeClk(TOUCH_Clk_Select clk_sel)
{
    uint32_t touch_clk = 0;
    OS_ENTER_CRITICAL();
    SetLockLpmWriteReg();
    HWRITE(CORE_LPM_RD_MUX,2);
    touch_clk = HREADL(CORE_LPM_STATUS_OTHERS);
    CoreReg_LpmWriteXtal32kCtrl((touch_clk &~ (TS_CLkBit_CLR << 24)) | (clk_sel << 25)|(clk_sel << 24));
    wait_lpodelay(1000);
    SetReleaseLpmWriteReg();
    OS_EXIT_CRITICAL();
}

/**
 * @brief    select the current source for touch
 * @param    TOUCH_Ele_Select ele_select : ele_32k/ele_bg
 * @return   None
 */
void TOUCH_Ibsel(TOUCH_Ele_Select ele_select)
{
    int value = 0;
    value = (TOUCH_ReadReg(Ts_Fir) &~ (TS_FirCLk_Bit)) | (ele_select << 24);
    TOUCH_WriteReg(Ts_Fir, value);
    value = TOUCH_ReadReg(Ts_Fir);
}


