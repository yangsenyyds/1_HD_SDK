#include "yc11xx_sbc.h"
#include "yc11xx.h"
#include "btreg.h"
#include "reg_addr.h"
#include "reg_struct.h"
#include "yc11xx_bt_interface.h"
#include "yc11xx_systick.h"

#define MEDIA_SBC_SRC_BUFFER_SIZE (0x400)

void Audio_SbcVoiceFilterClkOff(void)
{
	HWORW(CORE_CLOCK_OFF, REG_CLOCK_OFF_VOICE_FILTER);
}
void Audio_SbcVoiceFilterClkOn(void)
{
	HWCORW(CORE_CLOCK_OFF, REG_CLOCK_OFF_VOICE_FILTER);
}

void Audio_SbcFarrowFilterDisable(void)
{
    TONE_CTRLRegDef toneCtrl;
    HREAD_STRUCT(CORE_TONE_CTRL, &toneCtrl);
    toneCtrl.tone_sbc_ff_en = 0;
    HWRITE_STRUCT(CORE_TONE_CTRL, &toneCtrl);
}

void Audio_SbcFarrowFilterEnable(void)
{
    TONE_CTRLRegDef toneCtrl;
    HREAD_STRUCT(CORE_TONE_CTRL, &toneCtrl);
    toneCtrl.tone_sbc_ff_en = 1;
    HWRITE_STRUCT(CORE_TONE_CTRL, &toneCtrl);
}


void Audio_SbcFarrowFilterSbcDisable(void)
{
    SBC_SCTRLRegDef sbcSctrl;
    HREAD_STRUCT(CORE_SBC_SCTRL, &sbcSctrl);
    sbcSctrl.sbc_ff_48k_en = 0;
    HWRITE_STRUCT(CORE_SBC_SCTRL, &sbcSctrl);
}

void Audio_SbcFarrowFilterSbcEnable(void)
{
    SBC_SCTRLRegDef sbcSctrl;
    HREAD_STRUCT(CORE_SBC_SCTRL, &sbcSctrl);
    sbcSctrl.sbc_ff_48k_en = 1;
    HWRITE_STRUCT(CORE_SBC_SCTRL, &sbcSctrl);
}


void Audio_SbcFarrowFilterClose(void)
{
    Audio_SbcVoiceFilterClkOff();

    Audio_SbcFarrowFilterDisable();
    Audio_SbcFarrowFilterSbcDisable();
}

void Audio_SbcFarrowFilter44Kto48K(void)
{
    Audio_SbcVoiceFilterClkOn();

    Audio_SbcFarrowFilterSbcDisable();
    Audio_SbcFarrowFilterEnable();
}
void Audio_SbcFarrowFilter44Kto44K(void)
{
    Audio_SbcVoiceFilterClkOn();

    Audio_SbcFarrowFilterEnable();
    Audio_SbcFarrowFilterSbcEnable();
}
void Audio_SbcFarrowFilter48Kto48K(void)
{
    Audio_SbcVoiceFilterClkOn();

    Audio_SbcFarrowFilterEnable();
    Audio_SbcFarrowFilterSbcEnable();
}


void Audio_SbcFarrowFilterClear(void)
{
    SBC_CLEARRegDef sbcClearReg = {0};
    sbcClearReg.sbc_farrow_filter_clr = 1;
    HWRITE_STRUCT(CORE_SBC_CLEAR, &sbcClearReg);
    SysTick_DelayUs(1);
    HWRITE(CORE_SBC_CLEAR, 0);
}





void Audio_SbcBusyWait(void)
{
	while(((HREAD(CORE_MISC_STATUS)) & BIT4) != 0);
}
void Audio_SbcClkOff(void)
{
	CoreReg_ClkControl(REG_CLOCK_OFF_SBC,DISABLE);
}
void Audio_SbcClkOn(void)
{
	CoreReg_ClkControl(REG_CLOCK_OFF_SBC,ENABLE);
}
void Audio_SbcStop(void)
{
    Audio_SbcClkOn();

	uint8_t temp;
	HWRITE(CORE_SBC_CTRL, 0);
	HWRITEW(CORE_SBC_SWP, 0);
	HWRITE(CORE_SBC_CLEAR, 0xff);
	SysTick_DelayUs(1);
	HWRITE(CORE_SBC_CLEAR, 0);
	Audio_SbcBusyWait();

    SBC_SCTRLRegDef sbcSctrl;
    HREAD_STRUCT(CORE_SBC_SCTRL, &sbcSctrl);
    sbcSctrl.sbc_sctrl_reserved = 0;
    sbcSctrl.cache_ctrl = 0;
    sbcSctrl.sbc_ff_48k_en = 0;
    sbcSctrl.msbc_dec_en = 0;
    sbcSctrl.sbc_eq_en = 0;
    HWRITE_STRUCT(CORE_SBC_SCTRL, &sbcSctrl);

    TONE_CTRLRegDef toneCtrl;
    HREAD_STRUCT(CORE_TONE_CTRL, &toneCtrl);
    toneCtrl.tone_ff_dither = 0;
    toneCtrl.tone_sbc_ff_en = 0;
    HWRITE_STRUCT(CORE_TONE_CTRL, &toneCtrl);

	Audio_SbcClkOff();
}

void Audio_SbcClear(void)
{
	Audio_SbcClkOn();

    SBC_CTRLRegDef sbcCtrl = {0};
    sbcCtrl.sbc_block_en = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbcCtrl);

    // Must clear this.
    HWRITEW(CORE_SBC_SWP, 0);

    SBC_CLEARRegDef sbcClearReg = {0};
    sbcClearReg.sbc_msbc_x_wp_clr = 1;
    sbcClearReg.sbc_v_offset_clr = 1;
    sbcClearReg.sbc_sat_record_clr = 1;
    sbcClearReg.sbc_error_clr = 1;
    sbcClearReg.sbc_v_matrix_clr = 1;
    sbcClearReg.sbc_farrow_filter_clr = 1;
	HWRITE_STRUCT(CORE_SBC_CLEAR, &sbcClearReg);
	SysTick_DelayUs(1);
	HWRITE(CORE_SBC_CLEAR, 0x0);

	Audio_SbcBusyWait();
}

void Audio_SbcSrcMramSelect(bool isMram)
{
    SBC_CTRLRegDef sbcCtrl;
    HREAD_STRUCT(CORE_SBC_CTRL, &sbcCtrl);
    sbcCtrl.sbc_src_bit16 = isMram;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbcCtrl);
}

void Audio_SbcDstMramSelect(bool isMram)
{
    SBC_CTRLRegDef sbcCtrl;
    HREAD_STRUCT(CORE_SBC_CTRL, &sbcCtrl);
    sbcCtrl.sbc_dst_bit16 = isMram;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbcCtrl);
}
void Audio_SbcBlockControl(bool isEnable)
{
    SBC_CTRLRegDef sbcCtrl;
    HREAD_STRUCT(CORE_SBC_CTRL, &sbcCtrl);
    sbcCtrl.sbc_block_en = isEnable;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbcCtrl);
}

void Audio_SbcHwCheckError(void)
{
    SBC_STATUSRegDef sbcStatus;
    HREAD_STRUCT(CORE_SBC_STATUS, &sbcStatus);
    if(sbcStatus.sbc_error == 0)
    {
        return;
    }

    //Audio_SbcInit();
}
uint8_t Audio_SbcErrorAndReinit(void)
{
    uint8_t userFlag=0;
    if((HREAD(CORE_SBC_STATUS)&0xc0)==0)
    {
        return userFlag;
    }
    HWRITE(mem_sbc_error,HREAD(mem_sbc_error)+1);
    userFlag=1;
    return userFlag;
}

void Audio_SbcSetStreamOutSingleLeft(void)
{
    SBC_CTRL2RegDef sbcCtrl2;
    HREAD_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
    sbcCtrl2.sbc_ch0_op_en = 0;
    sbcCtrl2.sbc_ch1_op_en = 1;
    sbcCtrl2.sbc_ch_mux_en = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
}


void Audio_SbcSetStreamOutSingleRight(void)
{
    SBC_CTRL2RegDef sbcCtrl2;
    HREAD_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
    sbcCtrl2.sbc_ch0_op_en = 1;
    sbcCtrl2.sbc_ch1_op_en = 0;
    sbcCtrl2.sbc_ch_mux_en = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
}


void Audio_SbcSetStreamOutLeftRightMix(void)
{
    SBC_CTRL2RegDef sbcCtrl2;
    HREAD_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
    sbcCtrl2.sbc_ch0_op_en = 0;
    sbcCtrl2.sbc_ch1_op_en = 1;
    sbcCtrl2.sbc_ch_mux_en = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
}

void Audio_SbcSetStreamOutFull(void)
{
    SBC_CTRL2RegDef sbcCtrl2;
    HREAD_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
    sbcCtrl2.sbc_ch0_op_en = 0;
    sbcCtrl2.sbc_ch1_op_en = 0;
    sbcCtrl2.sbc_ch_mux_en = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbcCtrl2);
}


void Audio_SbcUpdateStreamOutTws(void)
{
    uint8_t hspRole = HREAD(mem_hsp_role);
    if(Bt_CheckTwsHSPRoleLeft())
    {
        Audio_SbcSetStreamOutSingleLeft();
    }
    else
    {
        Audio_SbcSetStreamOutSingleRight();
    }
}


void Audio_SbcUpdateStreamOut(void)
{
    uint8_t audioOutSetting = HREAD(mem_audio_output_setting);
    switch(audioOutSetting)
    {
        case AUDIO_OUTPUT_FULLMODE:
            Audio_SbcSetStreamOutFull();
            break;
        default:
            if(Bt_CheckTwsInPlayWork())
            {
                Audio_SbcUpdateStreamOutTws();
            }
            else
            {
                Audio_SbcSetStreamOutLeftRightMix();
            }
            break;
    }
}


void Audio_SbcInit(uint32_t bufferStartAddr, uint16_t bufferLen)
{
    bool isMram = reg_check_ram_m0(bufferStartAddr);
	Audio_SbcClear();

	HWRITE(CORE_SBC_CTRL2, 0);

	HWRITEW(CORE_SBC_SRC_START, 0);
	HWRITEW(CORE_SBC_SWP, 0);

	HWRITEW(CORE_SBC_SRC_LEN, MEDIA_SBC_SRC_BUFFER_SIZE);

	HWRITEW(CORE_SBC_DST_START, bufferStartAddr);
	HWRITEW(CORE_SBC_DST_LEN, bufferLen);

    Audio_SbcDstMramSelect(isMram);

    SBC_CLEARRegDef sbcClearReg = {0};
    sbcClearReg.sbc_src_addr_clr = 1;
    sbcClearReg.sbc_dst_addr_clr = 1;
	HWRITE_STRUCT(CORE_SBC_CLEAR, &sbcClearReg);
	SysTick_DelayUs(1);
	HWRITE(CORE_SBC_CLEAR, 0x0);

	Audio_SbcHwCheckError();

    Audio_SbcUpdateStreamOut();
}

uint32_t Audio_SbcGetDstAddr(void)
{
    SBC_CTRLRegDef sbcCtrl;
	uint32_t addr = HREADW(CORE_SBC_DST_START);
    HREAD_STRUCT(CORE_SBC_CTRL, &sbcCtrl);

    if(sbcCtrl.sbc_dst_bit16)
    {
        return reg_map_m0(addr);
    }

    return reg_map(addr);
}


