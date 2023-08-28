/**
 * @file yc11xx_audio_dac.c
 * @brief Only 12.288M and 11.2896M frequencies are allowed in DAC frequency selection
 * @date 2021-07-06
 */
#include "yc11xx_audio_dac.h"
#include "reg_addr.h"
#include "reg_struct.h"
#include "yc11xx_systick.h"
#include "btreg.h"

#include "yc_debug.h"

const int coef_hbf1[33] =
{
    -1, 4, -8, 17, -31, 53, -87,
        137, -209, 308, -444, 626, -865, 1173,
        -1566, 2061, -2677, 3437, -4370, 5506, -6887,
        8567, -10618, 13143, -16299, 20336, -25691, 33184,
        -44586, 64510, -109897, 333316, 524287
    };
const int coef_hbf2[16] =
{
    10, -47, 149, -383, 857, -1725,
    3202, -5577, 9236, -14737, 22989, -35790,
    57717, -105603, 331847, 524287
};
const int coef_cic_comp[6] = {1311, 224, -9738, -4407, 41196, 73902};
uint8_t gDacStartFlag = 0;

static uint8_t DAC_ChkSum = 0;
extern DAC_InitTypeDef dacparaminit;


void AUDIO_DacClkSet(void)
{
    DAC_SELRegDef dacSel;
    AUDIO_DIV_CLK_SELRegDef dacClkDiv;
    AUDIO_CLKSELDef dacClkSource;

    HREADW_STRUCT(CORE_AUDIO_CLKSEL, &dacClkSource);
    HREAD_STRUCT(CORE_AUDIO_DIV_CLK_SEL, &dacClkDiv);
    HREADW_STRUCT(CORE_DAC_SEL, &dacSel);
#ifndef CLOCK_HALVED
    if(dacparaminit.DacClk == DAC_Clk8k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide3;
        dacSel.dac_clk_sel = DAC_Mclk4;
    }
    else if(dacparaminit.DacClk == DAC_Clk11p025)
    {
        dacClkSource.dac_clksel = DAC_Clk11p2896M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide2;
        dacSel.dac_clk_sel = DAC_Mclk4;
    }
    else if(dacparaminit.DacClk == DAC_Clk12)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide2;
        dacSel.dac_clk_sel = DAC_Mclk4;
    }
    else if(dacparaminit.DacClk == DAC_Clk16k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide3;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk == DAC_Clk22p05)
    {
        dacClkSource.dac_clksel = DAC_Clk11p2896M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk4;
    }
    else if(dacparaminit.DacClk == DAC_Clk24k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk4;
    }
    else if(dacparaminit.DacClk == DAC_Clk32k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide3;
        dacSel.dac_clk_sel = DAC_Mclk;
    }
    else if(dacparaminit.DacClk == DAC_Clk44p1k)
    {
        dacClkSource.dac_clksel = DAC_Clk11p2896M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk == DAC_Clk48k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide2;
        dacSel.dac_clk_sel = DAC_Mclk;
    }
#else
    if(dacparaminit.DacClk == DAC_Clk8k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide3;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk == DAC_Clk11p025)
    {
        dacClkSource.dac_clksel = DAC_Clk11p2896M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide2;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk == DAC_Clk12)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide2;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk == DAC_Clk16k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide3;
        dacSel.dac_clk_sel = DAC_Mclk;
    }
    else if(dacparaminit.DacClk == DAC_Clk22p05)
    {
        dacClkSource.dac_clksel = DAC_Clk11p2896M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk == DAC_Clk24k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk2;
    }
    else if(dacparaminit.DacClk==DAC_Clk32k)
    {
    	//use 32k��clk need select 12.288M
   		_ASSERT_FAULT();
    }
    else if(dacparaminit.DacClk == DAC_Clk44p1k)
    {
        dacClkSource.dac_clksel = DAC_Clk11p2896M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk;
    }
    else if(dacparaminit.DacClk == DAC_Clk48k)
    {
        dacClkSource.dac_clksel = DAC_Clk12p288M;
        dacClkDiv.dac_divclk_sel = DAC_ClkDivide1;
        dacSel.dac_clk_sel = DAC_Mclk;
    }
#endif
    HWRITE_STRUCT(CORE_AUDIO_DIV_CLK_SEL, &dacClkDiv);
    HWRITEW_STRUCT(CORE_AUDIO_CLKSEL, &dacClkSource);
    HWRITEW_STRUCT(CORE_DAC_SEL, &dacSel);
}
/**
 * @brief DAC initialization function, you need to configure the initialization structure DAC to start
 * @param  dacParam:DAC mode config
 *                      SoundTrack:stereo/mono
 *                      LeftChannelConfig:DAC_LChannelSelLeft/DAC_LChannelSelRight/DAC_LChannelSelLeftRightMix/DAC_LChannelSelLeftRightNegation/DAC_LChannelClose
 *                      RightChannelConfig:DAC_RChannelSelLeft/DAC_RChannelSelRight/DAC_RChannelSelLeftRightMix/DAC_RChannelSelLeftRightNegation/DAC_RChannelClose
 *                      DacClk:DAC_Clk8k/DAC_Clk16k/DAC_Clk24k/DAC_Clk44p1k/DAC_Clk48k
 *                      DacStartAddr:Enter the start address of DAC playback
 *                      DacBufferSize:BUFF LEN
 *                      DacWorkMode:DAC_WorkContinuous/DAC_WorkWptr
 *                      DacZeroNumSel:0~7
 *                      DateMode:DAC_Date16Bit\DAC_Date24Bit
 *                      FadeEn:DAC_FadeEnable/DAC_FadeDisable
 *                      FadeStep:dac vol step of Channel LR
 *                      Vol:0~10
 */
void AUDIO_DacInit(DAC_InitTypeDef dac_param)
{
    DAC_AnaConfig ana_cfg;
    DAC_SELRegDef dac_sel;
    AUDIO_CLKSELDef dac_clksource;
    AUDIO_DIV_CLK_SELRegDef dac_clkdiv;
    DAC_CTRLRegDef dac_ctrl;
    DAC_VOL_LRegDef dac_volleft;
    DAC_VOL_RRegDef dac_volright;
    DAC_VOL_PRegDef dac_volrompt;
    DAC_DITHER_POWRegDef dac_dither;
    uint16_t dac_offsetl;
    uint16_t dac_offsetr;

    if(dac_param.DacBufferSize <= 1 || dac_param.DacStartAddr == NULL || dac_param.DacZeroNumSel > 7)
    {
        return;
    }

    if(dac_param.LeftChannelConfig == DAC_LChannelClose)
    {
        ana_cfg = DAC_AnaRight;
    }
    else if(dac_param.RightChannelConfig == DAC_RChannelClose)
    {
        ana_cfg = DAC_AnaLeft;
    }
    else
    {
        ana_cfg = DAC_AnaStereo;
    }


    CoreReg_ClkControl(REG_CLOCK_OFF_AUDIO_DAC, true);


    AUDIO_DacClkSet();
    AUDIO_DacCoefConfig();

    HREAD24BIT_STRUCT(CORE_DAC_VOL_L, &dac_volleft);
    dac_volleft.dac_vol_target_val_l = 0;
    HWRITE24BIT_STRUCT(CORE_DAC_VOL_L, &dac_volleft);

    HREAD24BIT_STRUCT(CORE_DAC_VOL_R, &dac_volright);
    dac_volright.dac_vol_target_val_r = 0;
    HWRITE24BIT_STRUCT(CORE_DAC_VOL_R, &dac_volright);

    HREAD24BIT_STRUCT(CORE_DAC_VOL_P, &dac_volrompt);
    dac_volrompt.dac_voll_target_val_p = 0;
    HWRITE24BIT_STRUCT(CORE_DAC_VOL_P, &dac_volrompt);

    HREAD24BIT_STRUCT(CORE_DAC_DITHER_POW, &dac_dither);
    dac_dither.dac_dither_power = 0x4FFDF3;
    HWRITE24BIT_STRUCT(CORE_DAC_DITHER_POW, &dac_dither);

    AUDIO_DacSelectChannel(dac_param);
    AUDIO_DacMonoSteroConfig(dac_param.SoundTrack);
    HREAD_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
    if(reg_check_ram_m0(dac_param.DacStartAddr))
    {
        dac_ctrl.dac_mram_sel = 1;
    }
    else
    {
        dac_ctrl.dac_mram_sel = 0;
    }
    if(dac_param.DacWorkMode == DAC_WorkContinuous)
    {
        dac_ctrl.dac_conti_mode = 1;
    }
    else
    {
        dac_ctrl.dac_conti_mode = 0;
    }
    dac_ctrl.dac_mute_lr = 0;
    HWRITE_STRUCT(CORE_DAC_CTRL, &dac_ctrl);

    HREADW_STRUCT(CORE_DAC_SEL, &dac_sel);
    if(dac_param.DateMode == DAC_Date24Bit)
    {
        dac_sel.dac_dat_wid_mode = 1;
    }
    else
    {
        dac_sel.dac_dat_wid_mode = 0;
    }

    dac_sel.dac_dither_en = 0;

    dac_sel.dac_dsm_outdis_mode = 1;
    dac_sel.dac_zero_num_sel = dac_param.DacZeroNumSel;

    dac_sel.dac_scramble_en = 1;
    dac_sel.dac_scramble_mode = 2;

    HWRITEW_STRUCT(CORE_DAC_SEL, &dac_sel);

    HWRITEW(CORE_DAC_SOURCE_ADDRESS, dac_param.DacStartAddr);
    HWRITEW(CORE_DAC_WPTR, dac_param.DacStartAddr);
    HWRITEW(CORE_DAC_LEN, dac_param.DacBufferSize - 1);
    dac_ctrl.dac_filter_en = 1;
    dac_ctrl.dac_sdm_en = 1;
    dac_ctrl.dac_en = 1;
    HWRITE_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
    
    dac_offsetl = HREADW(mem_efuse_dac_pop_val_l);
    dac_offsetr = HREADW(mem_efuse_dac_pop_val_r);
   //DEBUG_LOG_STRING("mem_efuse_dac_pop_val_l = %04x    mem_efuse_dac_pop_val_r = %04x  \r\n",
   //mem_efuse_dac_pop_val_l,mem_efuse_dac_pop_val_r);

    //Simulation configuration

    if(AUDIO_DacIsAnaOpen())
    {
        DEBUG_LOG_STRING("DacInit_noANA\n");
        //AUDIO_DacVolConfig(dacParam.Vol);
        HWRITE(mem_dac_start_ok_flag, 1);
        return;
    }
    DEBUG_LOG_STRING("DacInit_ANA\n");

    if(dac_param.OutMode == DAC_DifferentialOutputNoCapacitance || dac_param.OutMode == DAC_DifferentialOutputHaveCapacitance)
    {
        HWRITEW(CORE_DAC_DC_CALI_L, dac_offsetl);
        HWRITEW(CORE_DAC_DC_CALI_R, dac_offsetr);
    }
    else
    {
        dac_offsetl = dac_offsetl * 2;
        dac_offsetr = dac_offsetr * 2;
        HWRITEW(CORE_DAC_DC_CALI_L, dac_offsetl);
        HWRITEW(CORE_DAC_DC_CALI_R, dac_offsetr);
    }

    wait_lpodelay(2000);    //Wait for the digital to send the offset to the analogm  //65 point 48K

    if(dac_param.OutMode == DAC_DifferentialOutputNoCapacitance || dac_param.OutMode == DAC_DifferentialOutputHaveCapacitance)
    {
        AUDIO_DacAnaDiffConfig(ana_cfg, dac_param.OutMode);
    }
    else if(dac_param.OutMode == DAC_SingleEndOutputNoCapacitance || dac_param.OutMode == DAC_SingleEndOutputHaveCapacitance)
    {
        AUDIO_DacAnaSEConfig(ana_cfg, dac_param.OutMode);
    }
    else if(dac_param.OutMode == DAC_VcomOutputNoCapacitance || dac_param.OutMode == DAC_VcomOutputHaveCapacitance)
    {
        AUDIO_DacAnaVcomConfig(ana_cfg, dac_param.OutMode);
    }
    HWRITE(mem_dac_start_ok_flag, 1);
}

/**
 * @Description: Filter Parameters Write to Register Function
 * @param {*}None
 * @return {*}None
 */
void AUDIO_DacCoefConfig(void)
{
    ADDA_COEF_WR_CTRLRegDef dac_coef;

    HREAD_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
    dac_coef.dac_coef_wr_en = 1;
    HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);

    for(int i = 0; i < 33; i++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, coef_hbf1[i] >> 0);
        HWRITE(CORE_ADDA_COEF_WDATA_1, coef_hbf1[i] >> 8);
        HWRITE(CORE_ADDA_COEF_WDATA_2, coef_hbf1[i] >> 16);
		//DEBUG_LOG_STRING("CORE_ADDA_COEF_WDATA_0 :%04x\n",HREAD24BIT(CORE_ADDA_COEF_WDATA_0));

        dac_coef.dac_coef_strobe = 1;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);

        wait_lpodelay(5);

        dac_coef.dac_coef_strobe = 0;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
        wait_lpodelay(5);
        DAC_ChkSum += ((uint8_t)coef_hbf1[i] + (uint8_t)(coef_hbf1[i] >> 8) + (uint8_t)(coef_hbf1[i] >> 16));
    }
    for(int i = 0; i < 16; i++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, coef_hbf2[i] >> 0);
        HWRITE(CORE_ADDA_COEF_WDATA_1, coef_hbf2[i] >> 8);
        HWRITE(CORE_ADDA_COEF_WDATA_2, coef_hbf2[i] >> 16);
		//DEBUG_LOG_STRING("CORE_ADDA_COEF_WDATA_1 :%04x\n",HREAD24BIT(CORE_ADDA_COEF_WDATA_0));

        dac_coef.dac_coef_strobe = 1;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
        wait_lpodelay(5);

        dac_coef.dac_coef_strobe = 0;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
        wait_lpodelay(5);
        DAC_ChkSum += ((uint8_t)coef_hbf2[i] + (uint8_t)(coef_hbf2[i] >> 8) + (uint8_t)(coef_hbf2[i] >> 16));
    }
    for(int i = 0; i < 6; i++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, coef_cic_comp[i] >> 0);
        HWRITE(CORE_ADDA_COEF_WDATA_1, coef_cic_comp[i] >> 8);
        HWRITE(CORE_ADDA_COEF_WDATA_2, coef_cic_comp[i] >> 16);
		//DEBUG_LOG_STRING("CORE_ADDA_COEF_WDATA_2 :%04x\n",HREAD24BIT(CORE_ADDA_COEF_WDATA_0));

        dac_coef.dac_coef_strobe = 1;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
        wait_lpodelay(5);

        dac_coef.dac_coef_strobe = 0;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
        wait_lpodelay(5);
        DAC_ChkSum += ((uint8_t)coef_cic_comp[i] + (uint8_t)(coef_cic_comp[i] >> 8) + (uint8_t)(coef_cic_comp[i] >> 16));
    }
    dac_coef.dac_coef_wr_en = 0;
    dac_coef.dac_coef_strobe = 0;
    HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &dac_coef);
}

/**
 * @brief Clear RAMP rising completion signal
 */
void AUDIO_DacClearRampDown(void)
{
    RAMP_CTRLRegDef dac_clearRamp;
    HREAD_STRUCT(CORE_RAMP_CTRL, &dac_clearRamp);
    dac_clearRamp.ramp_down_finish_clr = 1;
    HWRITE_STRUCT(CORE_RAMP_CTRL, &dac_clearRamp);

    wait_lpodelay(1);
    HREAD_STRUCT(CORE_RAMP_CTRL, &dac_clearRamp);
    dac_clearRamp.ramp_down_finish_clr = 0;
    HWRITE_STRUCT(CORE_RAMP_CTRL, &dac_clearRamp);
}

/**
 * @Description: Select Left and Right Channels
 * @param {DAC_InitParamDef} config
 *                      SoundTrack:stereo/mono
 *                      LeftChannelConfig:DAC_LChannelSelLeft/DAC_LChannelSelRight/DAC_LChannelSelLeftRightMix/DAC_LChannelSelLeftRightNegation/DAC_LChannelClose
 *                      RightChannelConfig:DAC_RChannelSelLeft/DAC_RChannelSelRight/DAC_RChannelSelLeftRightMix/DAC_RChannelSelLeftRightNegation/DAC_RChannelClose
 *                      DacClk:DAC_Clk8k/DAC_Clk16k/DAC_Clk24k/DAC_Clk44p1k/DAC_Clk48k
 *                      DacStartAddr:Enter the start address of DAC playback
 *                      DacBufferSize:BUFF LEN
 *                      DacWorkMode:DAC_WorkContinuous/DAC_WorkWptr
 *                      DacDsmDitherEn:DAC_DSMDitherDisable/DAC_DSMDitherEnable
 *                      DSMOutdisMode:DAC_DSMModeNormal/DAC_DSMModeByNumSel
 *                      DacZeroNumSel:0~7
 *                      DateMode:DAC_Date16Bit\DAC_Date24Bit
 *                      DWAEn:DAC_ScrambleDWADisable/DAC_ScrambleDWAEnable
 *                      DWAMode:DAC_ScrambleDWANormal/DAC_ScrambleDWARandom/DAC_ScrambleDWARButterfly
 *                      FadeEn:DAC_FadeEnable/DAC_FadeDisable
 *                      FadeStep:dac vol step of Channel LR
 *                      Vol:0~10
 * @return {*}
 */
void AUDIO_DacSelectChannel(DAC_InitTypeDef config)
{
    uint8_t temp;
    DAC_SEL2RegDef dac_sel2;
    DAC_SELRegDef dac_sel;
    HREADW_STRUCT(CORE_DAC_SEL2, &dac_sel2);
    HREADW_STRUCT(CORE_DAC_SEL, &dac_sel);
    dac_sel2.dac_ana_clk_en = 1;
    if(config.LeftChannelConfig == DAC_LChannelClose)
    {
        dac_sel2.dac_clk_en_l = 0;
    }
    else
    {
        dac_sel2.dac_clk_en_l = 1;
        dac_sel2.dac_din_l_sel = config.LeftChannelConfig;
        dac_sel.dac_vol_fade_dis_l = 0;
    }
    if(config.RightChannelConfig == DAC_RChannelClose)
    {
        dac_sel2.dac_clk_en_r = 0;
    }
    else
    {
        dac_sel2.dac_clk_en_r = 1;
        dac_sel2.dac_din_r_sel = config.RightChannelConfig;
        dac_sel.dac_vol_fade_dis_r = 0;
    }
    HWRITEW_STRUCT(CORE_DAC_SEL2, &dac_sel2);
    HWRITEW_STRUCT(CORE_DAC_SEL, &dac_sel);
}

/**
 * @Description: Mono-Dual Channel Selection
 * @param {DAC_SoundTrackDev} mono_flag    stereo/mono
 * @return {*}
 */
void AUDIO_DacMonoSteroConfig(DAC_SoundTrackDev mono_flag)
{
    DAC_SELRegDef dac_sel;
    HREADW_STRUCT(CORE_DAC_SEL, &dac_sel);
    if(mono_flag == stereo)
    {
        dac_sel.dac_dma_mono_mode = 0;
    }
    else
    {
        dac_sel.dac_dma_mono_mode = 1;
    }
    HWRITEW_STRUCT(CORE_DAC_SEL, &dac_sel);
}

void AUDIO_DacAnaStop(void)
{
    codec_vmid_en0RegDef vmid_en0;
    au_dac_en2RegDef dac_en2;
    au_dac_en1RegDef dac_en1;
    au_dac_en3RegDef dac_en3;
    codec_vmid_cfg1RegDef vmid_cfg1;
    codec_vmid_cfg2RegDef vmid_cfg2;
    codec_ldo_en0RegDef lodEn0;

    memset(&vmid_en0, 0, sizeof(vmid_en0));                         //HWRITE(0x8b05,0x01);
    vmid_en0.da_vmid_discharge = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    memset(&dac_en2, 0, sizeof(dac_en2));                           //HWRITE(0x8b16,0x3c);
    dac_en2.da_dac_mute_l_en = 1;
    dac_en2.da_drv_mute_l_en = 1;
    dac_en2.da_drv_ostg_cc_l_en = 1;
    dac_en2.da_drv_ostg_l_en = 1;
    HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

    memset(&dac_en1, 0, sizeof(dac_en1));                           //HWRITE(0x8b15,0x3c);
    dac_en1.da_dac_mute_r_en = 1;
    dac_en1.da_drv_mute_r_en = 1;
    dac_en1.da_drv_ostg_cc_r_en = 1;
    dac_en1.da_drv_ostg_r_en = 1;
    HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);

    memset(&dac_en3, 0, sizeof(dac_en3));                           //HWRITE(0x8b17,0x40);
    dac_en3.da_drv_r1_open = 1;
    HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);

    memset(&vmid_cfg1, 0, sizeof(vmid_cfg1));                       //HWRITE(0x8b07,0x60);
    vmid_cfg1.rg_vmid_discharge_cc = 1;
    vmid_cfg1.rg_vmid_discharge_auto = 1;
    vmid_cfg1.rg_vmid_isel_msb = 0x1f;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);

    memset(&vmid_cfg2, 0, sizeof(vmid_cfg2));                       //HWRITE(0x8b08,0x04);
    vmid_cfg2.rg_vmid_vref_sel = 1;
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    memset(&lodEn0, 0, sizeof(lodEn0));                             //HWRITE(0x8b00,0x00);
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lodEn0);

    CoreReg_ClkControl(REG_CLOCK_OFF_AUDIO_DAC, false);
}

/**
 * @brief DAC stopped working
 */
void AUDIO_DacDigitalStop(void)
{
    DAC_CTRLRegDef dac_ctrl;

    HREAD_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
    dac_ctrl.dac_en = 0;
    HWRITE_STRUCT(CORE_DAC_CTRL, &dac_ctrl);

    HWRITE(mem_dac_start_ok_flag, 0);

    AUDIO_DacVolConfig(20, 0);
    // 0x1000 to 0x0 1step need near 505ms
    // 0x1000 to 0x0 2step need near 255ms
    // 0x1000 to 0x0 100step need near 6ms
    wait_lpodelay(30000);

}

/**
 * @brief DAC working status
 * @return Return true at work
 */
bool AUDIO_DacCheckEnable(void)
{
    DAC_CTRLRegDef dac_ctrl;
    HREAD_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
    return dac_ctrl.dac_en;
}

/**
 * @brief DAC playback is complete
 * @return Complete return true
 */
bool AUDIO_DacCheckEmpty(void)
{
    return (HREADW(CORE_DAC_WPTR) == ((HREADW(CORE_DAC_READ_PTR))));
}

/**
 * @brief View DAC working in CM0 or BT
 * @return Return true when working in CM0 core
 */
bool AUDIO_DacCheckBufferMram(void)
{
    DAC_CTRLRegDef dac_ctrl;
    HREAD_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
    return dac_ctrl.dac_mram_sel;
}

/**
 * @brief View the data size unplayed
 * @return uint16_t
 */
uint16_t AUDIO_DacGetBufferSize(void)
{
    uint16_t dac_rptr, dac_wptr;

    // Check adc buffer already for cvsd work
    dac_rptr = HREADW(CORE_DAC_READ_PTR);
    dac_wptr = HREADW((CORE_DAC_WPTR));
    if(dac_wptr >= dac_rptr)
    {
        return (dac_wptr - dac_rptr);
    }
    else
    {
        return (HREADW(CORE_DAC_LEN) + 1 - (dac_rptr  - dac_wptr));
    }
}

/**
 * @brief Check the size of the data that has been played
 * @return uint16_t
 */
uint16_t AUDIO_DacGetRemindBufferSize(void)
{
    return HREADW(CORE_DAC_LEN) + 1 - AUDIO_DacGetBufferSize();
}

/**
 * @brief DAC mute processing
 * @param  The DAC will be muted when the incoming true is muted, and the DAC will have sound only after unmuting.
 *           mute:true/false
 */
void AUDIO_DacMuteConfig(bool mute)
{
    DAC_CTRLRegDef dac_ctrl;
    HREAD_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
    dac_ctrl.dac_mute_lr = mute;
    HWRITE_STRUCT(CORE_DAC_CTRL, &dac_ctrl);
}

/**
 * @brief  Volume adjustment function, including left and right ears and prompt tone
 * @param    Vol:  Incoming 0-10
 * @return  :Whether the setting is successful
 */
bool AUDIO_DacVolConfig(uint8_t step, uint8_t Vol)
{
    DAC_VOL_LRegDef dac_volleft;
    DAC_VOL_RRegDef dac_volright;
    DAC_VOL_PRegDef dac_volrompt;

    if(Vol > 16)
    {
        return ERROR;
    }
    uint16_t realVol = AUDIO_DAC_VOL_MAX * Vol / 16;

    HREAD24BIT_STRUCT(CORE_DAC_VOL_L, &dac_volleft);
    if(dac_volleft.dac_vol_step_l == step
        && dac_volleft.dac_vol_target_val_l == realVol)
    {
        return SUCCESS;
    }
    dac_volleft.dac_vol_step_l = step;
    dac_volleft.dac_vol_target_val_l = realVol;
    HWRITE24BIT_STRUCT(CORE_DAC_VOL_L, &dac_volleft);

    HREAD24BIT_STRUCT(CORE_DAC_VOL_R, &dac_volright);
    dac_volright.dac_vol_step_r = step;
    dac_volright.dac_vol_target_val_r = realVol;
    HWRITE24BIT_STRUCT(CORE_DAC_VOL_R, &dac_volright);

    HREAD24BIT_STRUCT(CORE_DAC_VOL_P, &dac_volrompt);
    dac_volrompt.dac_vol_step_p = step;
    dac_volrompt.dac_voll_target_val_p = realVol;
    HWRITE24BIT_STRUCT(CORE_DAC_VOL_P, &dac_volrompt);
    return SUCCESS;
}

/**
 * @Description: Write wptr function in DAC write pointer mode
 * @param {uint8_t*} wptrBuff閿涙uff address
 * @return None
 */
void AUDIO_DacSetWptr(uint8_t *wptrBuff)
{
    HWRITEW(CORE_DAC_WPTR, wptrBuff);
}

void AUDIO_DacAnaVcomNoCapacitanceConfig(DAC_AnaConfig config)
{
    au_dac_en3RegDef dac_en3;
    codec_ldo_en0RegDef lod_en0;
    codec_vmid_cfg2RegDef vmid_cfg2;
    au_dac_cfg3RegDef dac_cfg3;
    codec_vmid_en0RegDef vmid_en0;
    au_dac_en0RegDef dac_en0;
    au_dac_en1RegDef dac_en1;
    au_dac_en2RegDef dac_en2;
    codec_vmid_cfg1RegDef vmid_cfg;
    codec_vmid_cfg0RegDef vmid_cfg0;

    memset(&dac_en3, 0, sizeof(dac_en3));                           //HWRITE(0x8b17,0x05);
    if(config == DAC_AnaStereo)
    {
        dac_en3.da_input_sel_l = 1;
        dac_en3.da_input_sel_r = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en3.da_input_sel_l = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en3.da_input_sel_r = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x75);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    vmid_cfg2.rg_vmid_iadjust = 0x0e;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    memset(&dac_cfg3, 0, sizeof(dac_cfg3));                         //HWRITE(0x8b1b,0x7f);
    dac_cfg3.rg_vrefgen_ccsel_int = 1;
    dac_cfg3.rg_vrefgen_ccsel_intopa = 1;
    dac_cfg3.rg_vrefgen_ccsel_drv = 1;
    dac_cfg3.rg_drv_ccn_sel = 3;
    dac_cfg3.rg_drv_ccp_sel = 3;
    HWRITE_STRUCT(CORE_au_dac_cfg3, &dac_cfg3);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x31);
    vmid_en0.da_vmid_discharge = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    vmid_en0.da_vmid_bg_en = 1 ;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_ldo_en0, &lod_en0);                     //HWRITE(0x8b00,0x20);  // mic ldo/isolation LDO enable
    lod_en0.da_mic_reg_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lod_en0);

    memset(&dac_en0, 0, sizeof(dac_en0));                           //HWRITE(0x8b14,0x07);
    memset(&dac_en2, 0, sizeof(dac_en2));                           //HWRITE(0x8b16,0x1c);
    memset(&dac_en1, 0, sizeof(dac_en1));
    if(config == DAC_AnaStereo)
    {
        dac_en0.da_drv_single_end_l_en = 1;
        dac_en0.da_drv_single_end_r_en = 1;
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_drv_mute_l_en = 1;
        dac_en2.da_dac_mute_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 1;
        dac_en1.da_drv_mute_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en0.da_drv_single_end_l_en = 1;
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_drv_mute_l_en = 1;
        dac_en2.da_dac_mute_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en0.da_drv_single_end_r_en = 1;
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 1;
        dac_en1.da_drv_mute_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    lod_en0.da_ldo_daccore_en = 1;                                  //HWRITE(0x8b00,0x2c);
    lod_en0.da_ldohp_dacdrv_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lod_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg);                  //HWRITE(0x8b07,0x7f);
    vmid_cfg.rg_vmid_isel_msb = 0x1f;
    vmid_cfg.rg_vmid_discharge_cc = 1;
    vmid_cfg.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg);

    vmid_en0.da_vmid_bias_en = 1;                                   //HWRITE(0x8b05,0x35);
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    wait_lpodelay(10000);

    if(config == DAC_AnaStereo)                                     //HWRITE(0x8b16,0xad);
    {
        dac_en2.da_drv_ostg_cc_l_en = 0;
        dac_en2.da_drv_l_en = 1;
        dac_en2.da_drv_ostg_l_en = 1;
        dac_en2.da_vrefgen_drv_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_drv_ostg_cc_r_en = 0;
        dac_en1.da_drv_r_en = 1;
        dac_en1.da_drv_ostg_r_en = 1;
        dac_en1.da_vrefgen_drv_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en2.da_drv_ostg_cc_l_en = 0;
        dac_en2.da_drv_l_en = 1;
        dac_en2.da_drv_ostg_l_en = 1;
        dac_en2.da_vrefgen_drv_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en1.da_drv_ostg_cc_r_en = 0;
        dac_en1.da_drv_r_en = 1;
        dac_en1.da_drv_ostg_r_en = 1;
        dac_en1.da_vrefgen_drv_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    wait_lpodelay(3);

    vmid_en0.da_vmid_discharge = 0;                                 //HWRITE(0x8b05,0x36);
    vmid_en0.da_vmid_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    if(config == DAC_AnaStereo)                                     //HWRITE(0x8b16,0xf3);
    {
        dac_en2.da_dac_int_l_en = 1;
        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_dac_mute_l_en = 0;
        dac_en2.da_drv_mute_l_en = 0;
        dac_en2.da_vrefgen_dac_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_dac_int_r_en = 1;
        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 0;
        dac_en1.da_drv_mute_r_en = 0;
        dac_en1.da_vrefgen_dac_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en2.da_dac_int_l_en = 1;
        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_dac_mute_l_en = 0;
        dac_en2.da_drv_mute_l_en = 0;
        dac_en2.da_vrefgen_dac_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en1.da_dac_int_r_en = 1;
        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 0;
        dac_en1.da_drv_mute_r_en = 0;
        dac_en1.da_vrefgen_dac_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x02);
    vmid_cfg0.rg_vmid_isel_lsb = 2;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);
}

void AUDIO_DacAnaVcomConfig(DAC_AnaConfig config, DAC_OutputMode vmid_cfg)
{
    CoreReg_ClkControl(REG_CLOCK_OFF_AUDIO_DAC, true);
    switch(vmid_cfg)
    {
    case DAC_VcomOutputNoCapacitance:
        AUDIO_DacAnaVcomNoCapacitanceConfig(config);
        break;
    case DAC_VcomOutputHaveCapacitance:
        break;
    default:
        break;
    }
}

void AUDIO_DacAnaDiffNoCapacitanceConfig(DAC_AnaConfig config)
{
    au_dac_en3RegDef dac_en3;
    codec_ldo_en0RegDef lod_en0;
    codec_vmid_cfg2RegDef vmid_cfg2;
    au_dac_cfg3RegDef dac_cfg3;
    codec_vmid_en0RegDef vmid_en0;
    au_dac_en0RegDef dac_en0;
    au_dac_en1RegDef dac_en1;
    au_dac_en2RegDef dac_en2;
    codec_vmid_cfg1RegDef vmid_cfg;
    codec_vmid_cfg0RegDef vmid_cfg0;

    memset(&dac_en3, 0, sizeof(dac_en3));                           //HWRITE(0x8b17,0x05);
    if(config == DAC_AnaStereo)
    {
        dac_en3.da_input_sel_l = 1;
        dac_en3.da_input_sel_r = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en3.da_input_sel_l = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en3.da_input_sel_r = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x75);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    vmid_cfg2.rg_vmid_iadjust = 0x0e;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    memset(&dac_cfg3, 0, sizeof(dac_cfg3));                         //HWRITE(0x8b1b,0x7f);
    dac_cfg3.rg_vrefgen_ccsel_int = 1;
    dac_cfg3.rg_vrefgen_ccsel_intopa = 1;
    dac_cfg3.rg_vrefgen_ccsel_drv = 1;
    dac_cfg3.rg_drv_ccn_sel = 3;
    dac_cfg3.rg_drv_ccp_sel = 3;
    HWRITE_STRUCT(CORE_au_dac_cfg3, &dac_cfg3);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x31);
    vmid_en0.da_vmid_discharge = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    vmid_en0.da_vmid_bg_en = 1 ;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_ldo_en0, &lod_en0);                     //HWRITE(0x8b00,0x20);  // mic ldo/isolation LDO enable
    lod_en0.da_mic_reg_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lod_en0);

    memset(&dac_en0, 0, sizeof(dac_en0));                           //HWRITE(0x8b14,0x07);
    memset(&dac_en2, 0, sizeof(dac_en2));                           //HWRITE(0x8b16,0x1c);
    memset(&dac_en1, 0, sizeof(dac_en1));
    if(config == DAC_AnaStereo)
    {
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_drv_mute_l_en = 1;
        dac_en2.da_dac_mute_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 1;
        dac_en1.da_drv_mute_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_drv_mute_l_en = 1;
        dac_en2.da_dac_mute_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 1;
        dac_en1.da_drv_mute_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    lod_en0.da_ldo_daccore_en = 1;                                  //HWRITE(0x8b00,0x2c);
    lod_en0.da_ldohp_dacdrv_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lod_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg);                  //HWRITE(0x8b07,0x7f);
    vmid_cfg.rg_vmid_isel_msb = 0x1f;
    vmid_cfg.rg_vmid_discharge_cc = 1;
    vmid_cfg.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg);

    vmid_en0.da_vmid_bias_en = 1;                                   //HWRITE(0x8b05,0x35);
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    wait_lpodelay(10000);

    if(config == DAC_AnaStereo)                                     //HWRITE(0x8b16,0xad);
    {
        dac_en2.da_drv_ostg_cc_l_en = 0;
        dac_en2.da_drv_l_en = 1;
        dac_en2.da_drv_ostg_l_en = 1;
        dac_en2.da_vrefgen_drv_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_drv_ostg_cc_r_en = 0;
        dac_en1.da_drv_r_en = 1;
        dac_en1.da_drv_ostg_r_en = 1;
        dac_en1.da_vrefgen_drv_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en2.da_drv_ostg_cc_l_en = 0;
        dac_en2.da_drv_l_en = 1;
        dac_en2.da_drv_ostg_l_en = 1;
        dac_en2.da_vrefgen_drv_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en1.da_drv_ostg_cc_r_en = 0;
        dac_en1.da_drv_r_en = 1;
        dac_en1.da_drv_ostg_r_en = 1;
        dac_en1.da_vrefgen_drv_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    wait_lpodelay(3);

    vmid_en0.da_vmid_discharge = 0;                                 //HWRITE(0x8b05,0x36);
    vmid_en0.da_vmid_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    if(config == DAC_AnaStereo)                                     //HWRITE(0x8b16,0xf3);
    {
        dac_en2.da_dac_int_l_en = 1;
        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_dac_mute_l_en = 0;
        dac_en2.da_drv_mute_l_en = 0;
        dac_en2.da_vrefgen_dac_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_dac_int_r_en = 1;
        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 0;
        dac_en1.da_drv_mute_r_en = 0;
        dac_en1.da_vrefgen_dac_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en2.da_dac_int_l_en = 1;
        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_dac_mute_l_en = 0;
        dac_en2.da_drv_mute_l_en = 0;
        dac_en2.da_vrefgen_dac_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en1.da_dac_int_r_en = 1;
        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 0;
        dac_en1.da_drv_mute_r_en = 0;
        dac_en1.da_vrefgen_dac_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x02);
    vmid_cfg0.rg_vmid_isel_lsb = 2;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);
}

void AUDIO_DacAnaDiffConfig(DAC_AnaConfig config, DAC_OutputMode vmid_cfg)
{
    CoreReg_ClkControl(REG_CLOCK_OFF_AUDIO_DAC, true);
    switch(vmid_cfg)
    {
    case DAC_DifferentialOutputNoCapacitance:
        AUDIO_DacAnaDiffNoCapacitanceConfig(config);
        break;
    case DAC_DifferentialOutputHaveCapacitance:
        break;
    default:
        break;
    }
}

void AUDIO_DacAnaSENoCapacitanceConfig(DAC_AnaConfig config)
{
    au_dac_en3RegDef dac_en3;
    codec_ldo_en0RegDef lod_en0;
    codec_vmid_cfg2RegDef vmid_cfg2;
    au_dac_cfg3RegDef dac_cfg3;
    codec_vmid_en0RegDef vmid_en0;
    au_dac_en0RegDef dac_en0;
    au_dac_en1RegDef dac_en1;
    au_dac_en2RegDef dac_en2;
    codec_vmid_cfg1RegDef vmid_cfg;
    codec_vmid_cfg0RegDef vmid_cfg0;

    memset(&dac_en3, 0, sizeof(dac_en3));                           //HWRITE(0x8b17,0x05);
    if(config == DAC_AnaStereo)
    {
        dac_en3.da_input_sel_l = 1;
        dac_en3.da_input_sel_r = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en3.da_input_sel_l = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en3.da_input_sel_r = 1;
        HWRITE_STRUCT(CORE_au_dac_en3, &dac_en3);
    }

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x75);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    vmid_cfg2.rg_vmid_iadjust = 0x0e;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    memset(&dac_cfg3, 0, sizeof(dac_cfg3));                         //HWRITE(0x8b1b,0x7f);
    dac_cfg3.rg_vrefgen_ccsel_int = 1;
    dac_cfg3.rg_vrefgen_ccsel_intopa = 1;
    dac_cfg3.rg_vrefgen_ccsel_drv = 1;
    dac_cfg3.rg_drv_ccn_sel = 3;
    dac_cfg3.rg_drv_ccp_sel = 3;
    HWRITE_STRUCT(CORE_au_dac_cfg3, &dac_cfg3);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x31);
    vmid_en0.da_vmid_discharge = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    vmid_en0.da_vmid_bg_en = 1 ;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_ldo_en0, &lod_en0);                     //HWRITE(0x8b00,0x20);  // mic ldo/isolation LDO enable
    lod_en0.da_mic_reg_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lod_en0);

    memset(&dac_en0, 0, sizeof(dac_en0));                           //HWRITE(0x8b14,0x07);
    memset(&dac_en2, 0, sizeof(dac_en2));                           //HWRITE(0x8b16,0x1c);
    memset(&dac_en1, 0, sizeof(dac_en1));
    if(config == DAC_AnaStereo)
    {
        dac_en0.da_drv_single_end_l_en = 1;
        dac_en0.da_drv_single_end_r_en = 1;
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_drv_mute_l_en = 1;
        dac_en2.da_dac_mute_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 1;
        dac_en1.da_drv_mute_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en0.da_drv_single_end_l_en = 1;
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_drv_mute_l_en = 1;
        dac_en2.da_dac_mute_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en0.da_drv_single_end_r_en = 1;
        dac_en0.da_dac_ibias_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en0, &dac_en0);

        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 1;
        dac_en1.da_drv_mute_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    lod_en0.da_ldo_daccore_en = 1;                                  //HWRITE(0x8b00,0x2c);
    lod_en0.da_ldohp_dacdrv_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &lod_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg);                  //HWRITE(0x8b07,0x7f);
    vmid_cfg.rg_vmid_isel_msb = 0x1f;
    vmid_cfg.rg_vmid_discharge_cc = 1;
    vmid_cfg.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg);

    vmid_en0.da_vmid_bias_en = 1;                                   //HWRITE(0x8b05,0x35);
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    wait_lpodelay(10000);

    if(config == DAC_AnaStereo)                                     //HWRITE(0x8b16,0xad);
    {
        dac_en2.da_drv_ostg_cc_l_en = 0;
        dac_en2.da_drv_l_en = 1;
        dac_en2.da_drv_ostg_l_en = 1;
        dac_en2.da_vrefgen_drv_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_drv_ostg_cc_r_en = 0;
        dac_en1.da_drv_r_en = 1;
        dac_en1.da_drv_ostg_r_en = 1;
        dac_en1.da_vrefgen_drv_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en2.da_drv_ostg_cc_l_en = 0;
        dac_en2.da_drv_l_en = 1;
        dac_en2.da_drv_ostg_l_en = 1;
        dac_en2.da_vrefgen_drv_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en1.da_drv_ostg_cc_r_en = 0;
        dac_en1.da_drv_r_en = 1;
        dac_en1.da_drv_ostg_r_en = 1;
        dac_en1.da_vrefgen_drv_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    wait_lpodelay(3);

    vmid_en0.da_vmid_discharge = 0;                                 //HWRITE(0x8b05,0x36);
    vmid_en0.da_vmid_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    if(config == DAC_AnaStereo)                                     //HWRITE(0x8b16,0xf3);
    {
        dac_en2.da_dac_int_l_en = 1;
        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_dac_mute_l_en = 0;
        dac_en2.da_drv_mute_l_en = 0;
        dac_en2.da_vrefgen_dac_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);

        dac_en1.da_dac_int_r_en = 1;
        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 0;
        dac_en1.da_drv_mute_r_en = 0;
        dac_en1.da_vrefgen_dac_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }
    else if(config == DAC_AnaLeft)
    {
        dac_en2.da_dac_int_l_en = 1;
        dac_en2.da_drv_ostg_cc_l_en = 1;
        dac_en2.da_dac_mute_l_en = 0;
        dac_en2.da_drv_mute_l_en = 0;
        dac_en2.da_vrefgen_dac_l_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en2, &dac_en2);
    }
    else if(config == DAC_AnaRight)
    {
        dac_en1.da_dac_int_r_en = 1;
        dac_en1.da_drv_ostg_cc_r_en = 1;
        dac_en1.da_dac_mute_r_en = 0;
        dac_en1.da_drv_mute_r_en = 0;
        dac_en1.da_vrefgen_dac_r_en = 1;
        HWRITE_STRUCT(CORE_au_dac_en1, &dac_en1);
    }

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x02);
    vmid_cfg0.rg_vmid_isel_lsb = 2;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);
}

void AUDIO_DacAnaSEConfig(DAC_AnaConfig config, DAC_OutputMode vmid_cfg)
{
    CoreReg_ClkControl(REG_CLOCK_OFF_AUDIO_DAC, true);
    switch(vmid_cfg)
    {
    case DAC_SingleEndOutputNoCapacitance:
        AUDIO_DacAnaSENoCapacitanceConfig(config);
        break;
    case DAC_SingleEndOutputHaveCapacitance:
        break;
    default:
        break;
    }
}

bool AUDIO_DacIsAnaOpen()
{
    codec_vmid_indRegDef vmid_inreg;                          //Determine whether the DAC simulation is powered on
    memset(&vmid_inreg, 0, sizeof(vmid_inreg));
    HREAD_STRUCT(CORE_codec_vmid_ind, &vmid_inreg);
    return vmid_inreg.ad_vmid_ok_12;
}

void AUDIO_HighPerformanceTest()
{
    //    Tests ADC+DAC high performance mode-specific functions.
    //    For internal debug.
    //    Configure the simulation before calling this letter.
    HWRITE(0x804c, 0x88);
    HWRITE(0x804d, 0x9f);
    HWRITE(0x804e, 0x26);
    HWRITE(0x804f, 0x01);
    HWRITE(0x8005, 0x20);

    HWRITE(0x8b01, 0x79);
    HWRITE(0x8b02, 0x7d);
    HWRITE(0x8b04, 0x3e);
#if 1
    HWRITE(0x8b08, 0x05);
#else
    HWRITE(0x8b08, 0x01);  //01 performs better. SNR differs by 2 dB
#endif
}

/**
 * @Description: Mix processing initialization function
 * @param {HINTTONE_InitParamDef} config
 *                              HintToneVol:HINTTONE_HintToneVol0/HINTTONE_HintToneVol1/HINTTONE_HintToneVol2/HINTTONE_HintToneVol3
 *                              MainVol:HINTTONE_MainVol0/HINTTONE_MainVol1/HINTTONE_MainVol2/HINTTONE_MainVol3
 *                              WorkMode:HINTTONE_WorkWptr/HINTTONE_WorkContinuous
 *                              LRPlay:HINTTONE_LeftEarPlay/HINTTONE_RightEarPlay/HINTTONE_LREarPlay
 *                              StarAddr:Buff start address
 *                              BuffLen:buff len
 * @return {None}
 */
void HINTTONE_Init(HINTTONE_InitTypeDef hinttone_config)
{
    TISHI_CTRLRegDef hinttone_ctrl;
    RAMP_CTRLRegDef hinttone_ctrlpamp;

    HREAD_STRUCT(CORE_TISHI_CTRL, &hinttone_ctrl);
    hinttone_ctrl.tishi_trunc = hinttone_config.HintToneVol;

    if(hinttone_config.HintToneVol == HINTTONE_HintToneVol2 || hinttone_config.HintToneVol == HINTTONE_HintToneVol3)
    {
        HREAD_STRUCT(CORE_RAMP_CTRL, &hinttone_ctrlpamp);
        hinttone_ctrlpamp.ramp_tishi_trunc_hi = hinttone_config.HintToneVol >> 1;
        HWRITE_STRUCT(CORE_RAMP_CTRL, &hinttone_ctrlpamp);
    }

    hinttone_ctrl.tishi_cont_mode = hinttone_config.WorkMode;

    hinttone_ctrl.main_trunc = hinttone_config.MainVol;
    if(reg_check_ram_m0(hinttone_config.StarAddr))
    {
        hinttone_ctrl.tishi_mram_sel = 1;
    }
    else
    {
        hinttone_ctrl.tishi_mram_sel = 0;
    }
    if(hinttone_config.LRPlay == HINTTONE_LeftEarPlay)
    {
        hinttone_ctrl.tishi_l_en = 1;
        hinttone_ctrl.tishi_r_en = 0;
    }
    else if(hinttone_config.LRPlay == HINTTONE_RightEarPlay)
    {
        hinttone_ctrl.tishi_l_en = 0;
        hinttone_ctrl.tishi_r_en = 1;
    }
    else if(hinttone_config.LRPlay == HINTTONE_LREarPlay)
    {
        hinttone_ctrl.tishi_l_en = 1;
        hinttone_ctrl.tishi_r_en = 1;
    }
    HWRITE_STRUCT(CORE_TISHI_CTRL, &hinttone_ctrl);

    HWRITEW(CORE_TISHI_SADDR, hinttone_config.StarAddr);;

    if(hinttone_config.WorkMode == HINTTONE_WorkWptr)
    {
        HWRITEW(CORE_TISHI_WPTR, hinttone_config.StarAddr);
    }

    HWRITEW(CORE_TISHI_LEN, hinttone_config.BuffLen - 1);

    hinttone_ctrl.tishi_en = 1;
    HWRITE_STRUCT(CORE_TISHI_CTRL, &hinttone_ctrl);
}

/**
 * @Description: Stop mixing
 * @param {None}
 * @return {None}
 */
void HINTTONE_Stop(void)
{
    TISHI_CTRLRegDef hinttone_ctrl;
    HREAD_STRUCT(CORE_TISHI_CTRL, &hinttone_ctrl);
    hinttone_ctrl.tishi_en = 0;
    HWRITE_STRUCT(CORE_TISHI_CTRL, &hinttone_ctrl);
}

/**
 * @Description: Check whether the audio playback is complete
 * @param {None}
 * @return {bool} Return true after completion
 */
bool HINTTONE_CheckEmpty(void)
{
    return HREADW(CORE_TISHI_RPTR) == HREADW(CORE_TISHI_WPTR);
}

/**
 * @Description: Update wptr function in mixed write pointer mode
 * @param {uint8_t*} wptrBuff閿涙瓊ew wptr
 * @return {None}
 */
void HINTTONE_SetWptr(uint8_t *hinttone_wptrbuff)
{
    HWRITEW(CORE_TISHI_WPTR, hinttone_wptrbuff);
}

uint16_t HINTTONE_DacGetBufferSize(void)
{
    uint16_t rPtr, wPtr;

    // Check adc buffer already for cvsd work
    rPtr = HREADW(CORE_TISHI_RPTR);
    wPtr = HREADW((CORE_TISHI_WPTR));
    if(wPtr >= rPtr)
    {
        return (wPtr - rPtr);
    }
    else
    {
        return (HREADW(CORE_TISHI_LEN) + 1 - (rPtr  - wPtr));
    }
}

uint16_t HINTTONE_DacGetRemindBufferSize(void)
{
    return HREADW(CORE_TISHI_LEN) + 1 - HINTTONE_DacGetBufferSize();
}
