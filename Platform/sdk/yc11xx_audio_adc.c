/**
***************************************************************************
* @file:  yc11xx_audio_adc.c
* @Date:  2021-7-14-21:22:02
* @brief:  Audio_adc dirver

* Change Logs:
* Date           Author      Version        Notes
* 2021-07-14     xxx         V1.0.0         the first version
**************************************************************************
*/
#include "yc11xx_audio_adc.h"
#include "reg_addr.h"
#include "reg_struct.h"
#include "yc11xx_gpio.h"
#include "yc11xx.h"
#include "yc11xx_systick.h"

#include "app_config.h"

const int64_t g_adc_cicfircoeffTtablel[] =
{
    322, 564, -126, -698, 412, 971, -984, -1206, 1873, 1134, -3152, -547, 4648, -919, -6141,
    3470, 7097, -7327, -6921, 12411, 4724, -18559, 460, 25322, -10158, -32422, 27278, 40358, -61509, -57063,
    173450, 331522, 173450, -57063, -61509, 40358, 27278, -32422, -10158, 25322, 460, -18559, 4724, 12411, -6921,
    -7327, 7097, 3470, -6141, -919, 4648, -547, -3152, 1134, 1873, -1206, -984, 971, 412, -698,
    -126, 564, 322
};
const int64_t g_adc_halfbandcoeftablel[] =
{
    -430, 0, 643, 0, -1100, 0, 1748, 0, -2640, 0, 3841, 0, -5439, 0, 7567,
        0, -10446, 0, 14484, 0, -20557, 0, 30962, 0, -54150, 0, 166388, 262144, 166388, 0,
        -54150, 0, 30962, 0, -20557, 0, 14484, 0, -10446, 0, 7567, 0, -5439, 0, 3841,
        0, -2640, 0, 1748, 0, -1100, 0, 643, 0, -430
    };

static uint8_t g_adc_chksum = 0;

/**
 * @brief    Adc initional
 * @param    adc_param :   ADC configuration parameter structure
 * @return  None
 */
void AUDIO_AdcInit(AUDIO_ADCInitTypeDef adc_param)
{
    ADC_MIC_CTRLRegDef adc_micctrl;
    AUDIO_CLKSELDef adc_clksource;
    AUDIO_DIV_CLK_SELRegDef adc_clkdiv;
    ADCD_CTRLRegDef adc_ctrl;
    ADC_VOLRegDef adc_vol;
    uint8_t *adc_endaddr;
    bool adc_startaddrismram;
    ADC_FILTER_CTRLRegDef adc_filterctrl;
    ADC_DC_CTRLRegDef adc_dcctrl;
    ADCD_DELAYRegDef adc_delay;

    if((uint32_t *)adc_param.Adc_StartAddr == NULL)
    {
        return;
    }

    CoreReg_ClkControl(REG_CLOCK_OFF_VOICE_FILTER, ENABLE);
    CoreReg_ClkControl(REG_CLOCK_OFF_MIC, ENABLE);

    adc_endaddr = (adc_param.Adc_StartAddr + adc_param.Adc_BufferSize - 1);
    adc_startaddrismram = reg_check_ram_m0(adc_param.Adc_StartAddr);

    //Step1: init adc
    AUDIO_AdcFilterInit();
    // ADC SCAL setting
    HREAD_STRUCT(CORE_ADC_FILTER_CTRL, &adc_filterctrl);
    adc_filterctrl.adc_comp_scale = 4;
    adc_filterctrl.adc_hbf_scale = 4;
    HWRITE_STRUCT(CORE_ADC_FILTER_CTRL, &adc_filterctrl);

    HREAD_STRUCT(CORE_ADC_DC_CTRL, &adc_dcctrl);
    adc_dcctrl.adc_dc_avg_time = 2;
    HWRITE_STRUCT(CORE_ADC_DC_CTRL, &adc_dcctrl);

    //Step2: init adc buffer info
    HWRITEW(CORE_ADCD_SADDR, adc_param.Adc_StartAddr);
    HWRITEW(CORE_ADCD_EADDR, adc_endaddr);

    HREADW_STRUCT(CORE_ADC_VOL + 1, &adc_vol);
    if(adc_param.Adc_VolFadeEnable == ENABLE)
    {
        HWRITE(CORE_ADC_VOL, adc_param.Adc_VolStep);
        adc_vol.adc_vol_fade_dis = 0;
    }
    else
    {
        adc_vol.adc_vol_fade_dis = 1;
    }

    adc_vol.adc_voll_target_val = 0;
    if(adc_param.Adc_MuteChannel == ENABLE)
    {
        adc_vol.adc_mute_en = 1;
    }
    else
    {
        adc_vol.adc_mute_en = 0;
    }

    HWRITEW_STRUCT(CORE_ADC_VOL + 1, &adc_vol);

    HREAD_STRUCT(CORE_ADC_MIC_CTRL, &adc_micctrl);
    HREAD_STRUCT(CORE_AUDIO_DIV_CLK_SEL, &adc_clkdiv);
    HREAD_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
    HREADW_STRUCT(CORE_AUDIO_CLKSEL, &adc_clksource);
    if(adc_param.Mic_Type == MicIsPDMRisingClk)
    {
        adc_micctrl.adc_mic_src = true;
        adc_micctrl.adc_pdmdat_sel = false;
        adc_micctrl.adc_pdmclk_en = true;
    }
    else if(adc_param.Mic_Type == MicIsPDMFallingClk)
    {
        adc_micctrl.adc_mic_src = true;
        adc_micctrl.adc_pdmdat_sel = true;
        adc_micctrl.adc_pdmclk_en = true;
    }
    else
    {
        adc_micctrl.adc_mic_src = false;
        adc_micctrl.adc_anaclk_en = true;
    }

    adc_micctrl.adc_micdat_mode = false;
    adc_micctrl.adc_micclk_inv = true;

    switch(adc_param.Adc_Sample)
    {
#ifndef CLOCK_HALVED
    case ADC_Sample8K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide3;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv4;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample11p025K:
        adc_clksource.mic_clksel = ADCClk11p2896M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv4;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample12K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv4;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample16K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide3;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample22p05K:
        adc_clksource.mic_clksel = ADCClk11p2896M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample24K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample32K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide3;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample44p1K:
        adc_clksource.mic_clksel = ADCClk11p2896M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide1;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample48K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
#else
    case ADC_Sample8K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide3;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample11p025K:
        adc_clksource.mic_clksel = ADCClk11p2896M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample12K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPllDiv2;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample16K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide3;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample22p05K:
        adc_clksource.mic_clksel = ADCClk11p2896M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample24K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide2;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample32K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide3;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample16;
        break;
    case ADC_Sample44p1K:
        adc_clksource.mic_clksel = ADCClk11p2896M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide1;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
    case ADC_Sample48K:
        adc_clksource.mic_clksel = ADCClk12p288M;
        adc_clkdiv.adc_divclk_sel = ADC_ClkDivide1;
        adc_micctrl.adc_micclk_sel = MicClkFromPll;
        adc_ctrl.adcd_cic_ds_sel = Down_sample32;
        break;
#endif
    }
    HWRITE_STRUCT(CORE_AUDIO_DIV_CLK_SEL, &adc_clkdiv);
    HWRITE_STRUCT(CORE_ADC_MIC_CTRL, &adc_micctrl);
    HWRITE_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
    HWRITEW_STRUCT(CORE_AUDIO_CLKSEL, &adc_clksource);

    HREAD_STRUCT(CORE_ADCD_DELAY, &adc_delay);
    adc_delay.adcd_vof_blk_en = 1;
    HWRITE_STRUCT(CORE_ADCD_DELAY, &adc_delay);

    adc_ctrl.adcd_mram_sel = adc_startaddrismram;

    adc_dcctrl.adc_dc_avg_time = 0;
    adc_dcctrl.adc_dc_en = 1;
    HWRITE_STRUCT(CORE_ADC_DC_CTRL, &adc_dcctrl);

    if(adc_param.Adc_RssiConfig == adc_RssiDisable)
    {
        adc_ctrl.adcd_rssi_en = 0;
    }
    else
    {
        adc_ctrl.adcd_rssi_en = 1;
        adc_ctrl.adcd_rssi_avg_time = adc_param.Adc_RssiConfig;
        adc_ctrl.adcd_rssi_clr = 0;
    }
    HWRITE_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
    if(adc_param.Adc_HPFConfig != adc_HPFDisable)
    {
        AUDIO_AdcHPFInit(adc_param.Adc_HPFConfig);
    }
    else
    {
        AUDIO_AdcHPFClr();
    }
    
    if(adc_param.Adc_OutputMode == ADC_SingleEndOutputWithCapacitor)
    {
        AUDIO_AdcAnaSEWithCapacitor();
    }
    else if(adc_param.Adc_OutputMode == ADC_SingleEndOutputNoCapacitor)
    {
        AUDIO_AdcAnaSENoCapacitor();
    }
    else if(adc_param.Adc_OutputMode == ADC_DifferentialOutput)
    {
        AUDIO_AdcAnaDiffConfig();
    }
    else if(adc_param.Adc_OutputMode == ADC_VcomOutput)
    {
        AUDIO_AdcAnaVcomConfig();
    }

    AUDIO_AdcVolConfig(adc_param.Adc_Vol);
}

/**
 * @brief  Adc Filter Init
 * @param    void
 * @return  None
 */
void AUDIO_AdcFilterInit(void)
{
    uint16_t i = 0;
    ADDA_COEF_WR_CTRLRegDef adc_coef;
    HWRITE(CORE_ADDA_COEF_WR_CTRL, 0x10);

    HREAD_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
    adc_coef.vof_coef_wr_en = 1;
    HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);

    for(i = 0; i < 32; i++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, g_adc_cicfircoeffTtablel[i]);
        HWRITE(CORE_ADDA_COEF_WDATA_1, g_adc_cicfircoeffTtablel[i] >> 8);
        HWRITE(CORE_ADDA_COEF_WDATA_2, g_adc_cicfircoeffTtablel[i] >> 16);

        g_adc_chksum += ((uint8_t)g_adc_cicfircoeffTtablel[i] + (uint8_t)(g_adc_cicfircoeffTtablel[i] >> 8) + (uint8_t)(g_adc_cicfircoeffTtablel[i] >> 16));

        adc_coef.vof_coef_strobe = 1;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
        adc_coef.vof_coef_strobe = 0;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
    }
    for(i = 0; i < 28; i++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, g_adc_halfbandcoeftablel[i]);
        HWRITE(CORE_ADDA_COEF_WDATA_1, g_adc_halfbandcoeftablel[i] >> 8);
        HWRITE(CORE_ADDA_COEF_WDATA_2, g_adc_halfbandcoeftablel[i] >> 16);

        g_adc_chksum += ((uint8_t)g_adc_halfbandcoeftablel[i] + (uint8_t)(g_adc_halfbandcoeftablel[i] >> 8) + (uint8_t)(g_adc_halfbandcoeftablel[i] >> 16));

        adc_coef.vof_coef_strobe = 1;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
        adc_coef.vof_coef_strobe = 0;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
    }
    for(i = 0; i < 128; i ++)
    {
        HWRITE24BIT(CORE_ADDA_COEF_WDATA_0, 0);

        adc_coef.vof_coef_strobe = 1;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
        adc_coef.vof_coef_strobe = 0;
        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
    }
    adc_coef.vof_coef_strobe = 0;
    adc_coef.vof_coef_wr_en = 0;
    HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &adc_coef);
}

/**
 * @brief  To set the DMA of ADC
 * @param    void
 * @return  None
 */
void AUDIO_AdcDmaSet(bool state)
{
    ADCD_CTRLRegDef adc_ctrl;
    HREAD_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
    adc_ctrl.adcd_dma_en = state;
    HWRITE_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
}
/**
 * @brief  Check Adc dma enable
 * @param    void
 * @return  1:adc DMA enable,0:adc DMA disable
 */
bool AUDIO_AdcCheckEnable(void)
{
    ADCD_CTRLRegDef adc_ctrl;
    HREAD_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
    return (adc_ctrl.adcd_dma_en);
    //return (HREAD(CORE_ADCD_CTRL) & BIT7) != 0;
}

/**
 * @brief   Check using SRAM or MRAM
 * @param    void
 * @return  1: mram; 0: schedule ram/xram
 */
bool AUDIO_AdcCheckUseMram(void)
{
    ADCD_CTRLRegDef adc_ctrl;
    HREAD_STRUCT(CORE_ADCD_CTRL, &adc_ctrl);
    return adc_ctrl.adcd_mram_sel;
}

/**
 * @brief  stop Adc work
 * @param    void
 * @return  None
 */
void AUDIO_AdcStop(void)
{
    ADCD_DELAYRegDef adc_delay;
    memset(&adc_delay, 0, sizeof(adc_delay));
    HWRITE_STRUCT(CORE_ADCD_DELAY, &adc_delay);
    AUDIO_AdcAnaStop();
}

/**
 * @brief  Gets the lower sixteen bits of the ADC write pointer address
 * @param    void
 * @return  None
 */
uint16_t AUDIO_AdcWptr(void)
{
    return HREADW(CORE_ADCD_ADDR);
}

void AUDIO_AdcHPFInit(AUDIO_ADCHighPassFilter alpha1)
{
    MIC_HPF_CTRLRegDef adc_hpfcon;
    HWRITE(CORE_MIC_HPF_ALPHA, alpha1);
    memset(&adc_hpfcon, 0, sizeof(adc_hpfcon));
    adc_hpfcon.mic_hpf_ctrl_reserved1 = alpha1 >> 8;
    HWRITE_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
    AUDIO_AdcHPFEnable();
}

void AUDIO_AdcHPFEnable()
{
    MIC_HPF_CTRLRegDef adc_hpfcon;
    HREAD_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
    adc_hpfcon.mic_hpf_en = 1;
    adc_hpfcon.mic_hpf_clr = 0;
    HWRITE_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
}

void AUDIO_AdcHPFDisable()
{
    MIC_HPF_CTRLRegDef adc_hpfcon;
    HREAD_STRUCT(CORE_MIC_HPF_ALPHA, &adc_hpfcon);
    adc_hpfcon.mic_hpf_en = 0;
    HWRITE_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
}

void AUDIO_AdcHPFClr()
{
    MIC_HPF_CTRLRegDef adc_hpfcon;
    HREAD_STRUCT(CORE_MIC_HPF_ALPHA, &adc_hpfcon);
    adc_hpfcon.mic_hpf_en = 0;
    HWRITE_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
    adc_hpfcon.mic_hpf_clr = 1;
    HWRITE_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
    adc_hpfcon.mic_hpf_clr = 0;
    HWRITE_STRUCT(CORE_MIC_HPF_CTRL, &adc_hpfcon);
}

bool AUDIO_AdcVolConfig(uint8_t vol)
{
    ADC_VOLRegDef adc_vol;
    if(vol >= AUDIO_ADC_VOL_GEAR)
    {
        return false;
    }
    HREADW_STRUCT(CORE_ADC_VOL + 1, &adc_vol);
    adc_vol.adc_voll_target_val = AUDIO_ADC_VOL_MAX * vol / ((AUDIO_ADC_VOL_GEAR) - 1);
    HWRITEW_STRUCT(CORE_ADC_VOL + 1, &adc_vol);
    return true;
}

void AUDIO_AdcAnaDiffConfig()
{
    codec_ldo_en0RegDef ldo_en0;
    codec_ldo_cfg0RegDef lod_cfg0;
    codec_ldo_cfg1RegDef lod_cfg1;
    codec_ldo_cfg2RegDef lod_cfg2;
    codec_vmid_en0RegDef vmid_en0;
    codec_vmid_cfg0RegDef vmid_cfg0;
    codec_vmid_cfg1RegDef vmid_cfg1;
    codec_vmid_cfg2RegDef vmid_cfg2;
    codec_vmid_cfg3RegDef vmid_cfg3;
    au_adc_en0RegDef adc_en0;
    au_adc_en1RegDef adc_en1;
    au_adc_cfg0RegDef adc_cfg0;
    au_adc_cfg1RegDef adc_cfg1;
    au_adc_cfg3RegDef adc_cfg3;
    au_adc_cfg4RegDef adc_cfg4;
    au_adc_cfg5RegDef adc_cfg5;
    au_adc_cfg6RegDef adc_cfg6;

    HREAD_STRUCT(CORE_codec_ldo_en0, &ldo_en0);                     //HWRITE(0x8b00,0x3e);
    ldo_en0.da_ldo_daccore_en = 1;
    ldo_en0.da_ldohp_dacdrv_en = 1;
    ldo_en0.da_adc_reg_en = 1;
    ldo_en0.da_mic_reg_en = 1;
    ldo_en0.da_adc_pwrsw_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &ldo_en0);

    HREAD_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);                   //HWRITE(0x8b01,0x51);
    lod_cfg0.rg_mic_hvreg_vc = 1;
    lod_cfg0.rg_adc_reg_vol_sel = 2;
    lod_cfg0.rg_adc_reg_ibias_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);

    HREAD_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);                   //HWRITE(0x8b02,0x55);
    lod_cfg1.rg_ibsel_hpldo = 1;
    lod_cfg1.rg_ldo_excap_en = 1;
    lod_cfg1.rg_ldo_voutsel = 2;
    lod_cfg1.rg_ibsel_ldo = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);

    lod_cfg2.rg_ldohp_excap_en = 1;                                 //HWRITE(0x8b04,0x2a);
    lod_cfg2.rg_ldohp_voutsel = 2;
    lod_cfg2.rg_ldohp_ocp_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg2, &lod_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x36);
    vmid_en0.da_vmid_discharge = 0;
    vmid_en0.da_vmid_en = 1;
    vmid_en0.da_vmid_bias_en = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    vmid_en0.da_vmid_bg_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x08);
    vmid_cfg0.rg_vmid_isel_lsb = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);                 //HWRITE(0x8b07,0x68);
    vmid_cfg1.rg_vmid_isel_msb = 8;
    vmid_cfg1.rg_vmid_discharge_cc = 1;
    vmid_cfg1.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x05);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);                 //HWRITE(0x8b09,0x84);
    vmid_cfg3.rg_vmid_bg_trim = 4;
    vmid_cfg3.rg_vmid_bg_rct = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);

    memset(&adc_en0, 0, sizeof(adc_en0));                           //HWRITE(0x8b0b,0xe6);
    adc_en0.da_pga_gm_en = 0;
    adc_en0.da_pga_gm_fst_en = 1;
    adc_en0.da_sdm_dem_en = 1;
    adc_en0.da_sdm_en = 1;
    adc_en0.da_aadc_top_ibias_en = 1;
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    //    wait_lpodelay(100);

    adc_en0.da_pga_gm_fst_en = 0;                                   //HWRITE(0x8b0b,0xe2);
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    memset(&adc_en1, 0, sizeof(adc_en1));                           //HWRITE(0x8b0c,0x0b);
    adc_en1.da_micbias_en = 1;
    adc_en1.da_pga_en = 1;
    adc_en1.da_pga_diff_en = 1;
    adc_en1.da_pga_rbias_en = 0;
    HWRITE_STRUCT(CORE_au_adc_en1, &adc_en1);

    memset(&adc_cfg0, 0, sizeof(adc_cfg0));                         //HWRITE(0x8b0d,0x55);
    adc_cfg0.rg_sdm_cmp_isel = 1;
    adc_cfg0.rg_sdm_ota1_isel = 1;
    adc_cfg0.rg_sdm_ota2_isel = 1;
    adc_cfg0.rg_sdm_ref_isel = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg0, &adc_cfg0);

    memset(&adc_cfg1, 0, sizeof(adc_cfg1));                         //HWRITE(0x8b0e,0x51);
    adc_cfg1.rg_pga_opa2_ibias_sel = 1;
    adc_cfg1.rg_pga_cz_ibias_sel = 2;
    adc_cfg1.rg_pga_gm_ibias_sel = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg1, &adc_cfg1);

    HWRITE(CORE_au_adc_cfg2, 0x05);                                 //HWRITE(0x8b0f,0x05);

    memset(&adc_cfg3, 0, sizeof(adc_cfg3));                         //HWRITE(0x8b10,0x0f);
    adc_cfg3.rg_pga_gc = AdcAnaDiffConfig_VOLTAGE_GAIN;
    HWRITE_STRUCT(CORE_au_adc_cfg3, &adc_cfg3);

    memset(&adc_cfg4, 0, sizeof(adc_cfg4));                         //HWRITE(0x8b11,0x05);
    adc_cfg4.rg_pga_rcfilt_coarse = 1;
    adc_cfg4.rg_pga_dcoc_ibc = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg4, &adc_cfg4);

    memset(&adc_cfg5, 0, sizeof(adc_cfg5));                         //HWRITE(0x8b12,0x4e);
    adc_cfg5.rg_micbias_rc = 6;
    adc_cfg5.rg_micbias_ibias_sel = 1;
    adc_cfg5.rg_pga_rcfilt_fine = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg5, &adc_cfg5);

    memset(&adc_cfg6, 0, sizeof(adc_cfg6));                         //HWRITE(0x8b13,0x40);
    adc_cfg6.rg_micbias_mode = 0;
    adc_cfg6.rg_micbias_vc = 0;
    HWRITE_STRUCT(CORE_au_adc_cfg6, &adc_cfg6);
}

void AUDIO_AdcAnaSENoCapacitor()
{
    codec_ldo_en0RegDef ldo_en0;
    codec_ldo_cfg0RegDef lod_cfg0;
    codec_ldo_cfg1RegDef lod_cfg1;
    codec_ldo_cfg2RegDef lod_cfg2;
    codec_vmid_en0RegDef vmid_en0;
    codec_vmid_cfg0RegDef vmid_cfg0;
    codec_vmid_cfg1RegDef vmid_cfg1;
    codec_vmid_cfg2RegDef vmid_cfg2;
    codec_vmid_cfg3RegDef vmid_cfg3;
    au_adc_en0RegDef adc_en0;
    au_adc_en1RegDef adc_en1;
    au_adc_cfg0RegDef adc_cfg0;
    au_adc_cfg1RegDef adc_cfg1;
    au_adc_cfg3RegDef adc_cfg3;
    au_adc_cfg4RegDef adc_cfg4;
    au_adc_cfg5RegDef adc_cfg5;
    au_adc_cfg6RegDef adc_cfg6;

    memset(&adc_en1, 0, sizeof(adc_en1));                           //HWRITE(0x8b0c,0x0b);
    adc_en1.da_micbias_en = 1;
    adc_en1.da_pga_en = 1;
    adc_en1.da_pga_diff_en = 0;
    adc_en1.da_pga_rbias_en = 1;
    HWRITE_STRUCT(CORE_au_adc_en1, &adc_en1);

    HREAD_STRUCT(CORE_codec_ldo_en0, &ldo_en0);                     //HWRITE(0x8b00,0x3e);
    ldo_en0.da_ldo_daccore_en = 1;
    ldo_en0.da_ldohp_dacdrv_en = 1;
    ldo_en0.da_adc_reg_en = 1;
    ldo_en0.da_mic_reg_en = 1;
    ldo_en0.da_adc_pwrsw_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &ldo_en0);

    HREAD_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);                   //HWRITE(0x8b01,0x51);
    lod_cfg0.rg_mic_hvreg_vc = 1;
    lod_cfg0.rg_adc_reg_vol_sel = 2;
    lod_cfg0.rg_adc_reg_ibias_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);

    HREAD_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);                   //HWRITE(0x8b02,0x55);
    lod_cfg1.rg_ibsel_hpldo = 1;
    lod_cfg1.rg_ldo_excap_en = 1;
    lod_cfg1.rg_ldo_voutsel = 2;
    lod_cfg1.rg_ibsel_ldo = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);

    lod_cfg2.rg_ldohp_excap_en = 1;                                 //HWRITE(0x8b04,0x2a);
    lod_cfg2.rg_ldohp_voutsel = 2;
    lod_cfg2.rg_ldohp_ocp_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg2, &lod_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x36);
    vmid_en0.da_vmid_discharge = 0;
    vmid_en0.da_vmid_en = 1;
    vmid_en0.da_vmid_bias_en = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    vmid_en0.da_vmid_bg_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x08);
    vmid_cfg0.rg_vmid_isel_lsb = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);                 //HWRITE(0x8b07,0x68);
    vmid_cfg1.rg_vmid_isel_msb = 8;
    vmid_cfg1.rg_vmid_discharge_cc = 1;
    vmid_cfg1.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x05);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);                 //HWRITE(0x8b09,0x84);
    vmid_cfg3.rg_vmid_bg_trim = 4;
    vmid_cfg3.rg_vmid_bg_rct = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);

    memset(&adc_en0, 0, sizeof(adc_en0));                           //HWRITE(0x8b0b,0xe6);
    adc_en0.da_pga_gm_en = 1;
    adc_en0.da_pga_gm_fst_en = 1;
    adc_en0.da_sdm_dem_en = 1;
    adc_en0.da_sdm_en = 1;
    adc_en0.da_aadc_top_ibias_en = 1;
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    wait_lpodelay(200);

    adc_en0.da_pga_gm_fst_en = 0;
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);                       //HWRITE(0x8b0b,0xe2);

    memset(&adc_cfg0, 0, sizeof(adc_cfg0));                         //HWRITE(0x8b0d,0x55);
    adc_cfg0.rg_sdm_cmp_isel = 1;
    adc_cfg0.rg_sdm_ota1_isel = 1;
    adc_cfg0.rg_sdm_ota2_isel = 1;
    adc_cfg0.rg_sdm_ref_isel = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg0, &adc_cfg0);

    memset(&adc_cfg1, 0, sizeof(adc_cfg1));                         //HWRITE(0x8b0e,0x51);
    adc_cfg1.rg_pga_opa2_ibias_sel = 1;
    adc_cfg1.rg_pga_cz_ibias_sel = 2;
    adc_cfg1.rg_pga_gm_ibias_sel = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg1, &adc_cfg1);

    HWRITE(CORE_au_adc_cfg2, 0x05);                                 //HWRITE(0x8b0f,0x05);

    memset(&adc_cfg3, 0, sizeof(adc_cfg3));                         //HWRITE(0x8b10,0x0f);
    adc_cfg3.rg_pga_gc = 15;
    HWRITE_STRUCT(CORE_au_adc_cfg3, &adc_cfg3);

    memset(&adc_cfg4, 0, sizeof(adc_cfg4));                         //HWRITE(0x8b11,0x05);
    adc_cfg4.rg_pga_rcfilt_coarse = 1;
    adc_cfg4.rg_pga_dcoc_ibc = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg4, &adc_cfg4);

    memset(&adc_cfg5, 0, sizeof(adc_cfg5));                         //HWRITE(0x8b12,0x4e);
    adc_cfg5.rg_micbias_rc = 6;
    adc_cfg5.rg_micbias_ibias_sel = 1;
    adc_cfg5.rg_pga_rcfilt_fine = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg5, &adc_cfg5);

    memset(&adc_cfg6, 0, sizeof(adc_cfg6));                         //HWRITE(0x8b13,0x40);
    adc_cfg6.rg_micbias_mode = 1;
    adc_cfg6.rg_micbias_vc = 6;
    HWRITE_STRUCT(CORE_au_adc_cfg6, &adc_cfg6);
}

void AUDIO_AdcAnaSEWithCapacitor()
{
    codec_ldo_en0RegDef ldo_en0;
    codec_ldo_cfg0RegDef lod_cfg0;
    codec_ldo_cfg1RegDef lod_cfg1;
    codec_ldo_cfg2RegDef lod_cfg2;
    codec_vmid_en0RegDef vmid_en0;
    codec_vmid_cfg0RegDef vmid_cfg0;
    codec_vmid_cfg1RegDef vmid_cfg1;
    codec_vmid_cfg2RegDef vmid_cfg2;
    codec_vmid_cfg3RegDef vmid_cfg3;
    au_adc_en0RegDef adc_en0;
    au_adc_en1RegDef adc_en1;
    au_adc_cfg0RegDef adc_cfg0;
    au_adc_cfg1RegDef adc_cfg1;
    au_adc_cfg3RegDef adc_cfg3;
    au_adc_cfg4RegDef adc_cfg4;
    au_adc_cfg5RegDef adc_cfg5;
    au_adc_cfg6RegDef adc_cfg6;

    HREAD_STRUCT(CORE_codec_ldo_en0, &ldo_en0);                     //HWRITE(0x8b00,0x3e);
    ldo_en0.da_ldo_daccore_en = 1;
    ldo_en0.da_ldohp_dacdrv_en = 1;
    ldo_en0.da_adc_reg_en = 1;
    ldo_en0.da_mic_reg_en = 1;
    ldo_en0.da_adc_pwrsw_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &ldo_en0);

    HREAD_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);                   //HWRITE(0x8b01,0x51);
    lod_cfg0.rg_mic_hvreg_vc = 1;
    lod_cfg0.rg_adc_reg_vol_sel = 2;
    lod_cfg0.rg_adc_reg_ibias_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);

    HREAD_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);                   //HWRITE(0x8b02,0x55);
    lod_cfg1.rg_ibsel_hpldo = 1;
    lod_cfg1.rg_ldo_excap_en = 1;
    lod_cfg1.rg_ldo_voutsel = 2;
    lod_cfg1.rg_ibsel_ldo = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);

    lod_cfg2.rg_ldohp_excap_en = 1;                                 //HWRITE(0x8b04,0x2a);
    lod_cfg2.rg_ldohp_voutsel = 2;
    lod_cfg2.rg_ldohp_ocp_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg2, &lod_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x36);
    vmid_en0.da_vmid_discharge = 0;
    vmid_en0.da_vmid_en = 1;
    vmid_en0.da_vmid_bias_en = 1;
    vmid_en0.da_vmid_extcap_en = 1;
    vmid_en0.da_vmid_bg_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x08);
    vmid_cfg0.rg_vmid_isel_lsb = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);                 //HWRITE(0x8b07,0x68);
    vmid_cfg1.rg_vmid_isel_msb = 8;
    vmid_cfg1.rg_vmid_discharge_cc = 1;
    vmid_cfg1.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x05);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);                 //HWRITE(0x8b09,0x84);
    vmid_cfg3.rg_vmid_bg_trim = 4;
    vmid_cfg3.rg_vmid_bg_rct = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);

    memset(&adc_en0, 0, sizeof(adc_en0));                           //HWRITE(0x8b0b,0xe6);
    adc_en0.da_pga_gm_en = 0;
    adc_en0.da_pga_gm_fst_en = 1;
    adc_en0.da_sdm_dem_en = 1;
    adc_en0.da_sdm_en = 1;
    adc_en0.da_aadc_top_ibias_en = 1;
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    //    wait_lpodelay(100);

    adc_en0.da_pga_gm_fst_en = 0;                                   //HWRITE(0x8b0b,0xe2);
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    memset(&adc_en1, 0, sizeof(adc_en1));                           //HWRITE(0x8b0c,0x0b);
    adc_en1.da_micbias_en = 1;
    adc_en1.da_pga_en = 1;
    adc_en1.da_pga_diff_en = 0;
    adc_en1.da_pga_rbias_en = 0;
    HWRITE_STRUCT(CORE_au_adc_en1, &adc_en1);

    memset(&adc_cfg0, 0, sizeof(adc_cfg0));                         //HWRITE(0x8b0d,0x55);
    adc_cfg0.rg_sdm_cmp_isel = 1;
    adc_cfg0.rg_sdm_ota1_isel = 1;
    adc_cfg0.rg_sdm_ota2_isel = 1;
    adc_cfg0.rg_sdm_ref_isel = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg0, &adc_cfg0);

    memset(&adc_cfg1, 0, sizeof(adc_cfg1));                         //HWRITE(0x8b0e,0x51);
    adc_cfg1.rg_pga_opa2_ibias_sel = 1;
    adc_cfg1.rg_pga_cz_ibias_sel = 2;
    adc_cfg1.rg_pga_gm_ibias_sel = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg1, &adc_cfg1);

    HWRITE(CORE_au_adc_cfg2, 0x05);                                 //HWRITE(0x8b0f,0x05);

    memset(&adc_cfg3, 0, sizeof(adc_cfg3));                         //HWRITE(0x8b10,0x0f);
    adc_cfg3.rg_pga_gc = AdcAnaSEWithCapacitor_VOLTAGE_GAIN;
    HWRITE_STRUCT(CORE_au_adc_cfg3, &adc_cfg3);

    memset(&adc_cfg4, 0, sizeof(adc_cfg4));                         //HWRITE(0x8b11,0x05);
    adc_cfg4.rg_pga_rcfilt_coarse = 1;
    adc_cfg4.rg_pga_dcoc_ibc = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg4, &adc_cfg4);

    memset(&adc_cfg5, 0, sizeof(adc_cfg5));                         //HWRITE(0x8b12,0x4e);
    adc_cfg5.rg_micbias_rc = 6;
    adc_cfg5.rg_micbias_ibias_sel = 1;
    adc_cfg5.rg_pga_rcfilt_fine = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg5, &adc_cfg5);

    memset(&adc_cfg6, 0, sizeof(adc_cfg6));                         //HWRITE(0x8b13,0x40);
    adc_cfg6.rg_micbias_mode = 0;
    adc_cfg6.rg_micbias_vc = 0;
    HWRITE_STRUCT(CORE_au_adc_cfg6, &adc_cfg6);
}

void AUDIO_AdcAnaVcomConfig()
{
    codec_ldo_en0RegDef ldo_en0;
    codec_ldo_cfg0RegDef lod_cfg0;
    codec_ldo_cfg1RegDef lod_cfg1;
    codec_vmid_en0RegDef vmid_en0;
    codec_vmid_cfg0RegDef vmid_cfg0;
    codec_vmid_cfg1RegDef vmid_cfg1;
    codec_vmid_cfg2RegDef vmid_cfg2;
    codec_vmid_cfg3RegDef vmid_cfg3;
    au_adc_en0RegDef adc_en0;
    au_adc_en1RegDef adc_en1;
    au_adc_cfg0RegDef adc_cfg0;
    au_adc_cfg1RegDef adc_cfg1;
    au_adc_cfg3RegDef adc_cfg3;
    au_adc_cfg4RegDef adc_cfg4;
    au_adc_cfg5RegDef adc_cfg5;

    HREAD_STRUCT(CORE_codec_ldo_en0, &ldo_en0);                     //HWRITE(0x8b00,0x3c);
    ldo_en0.da_ldo_daccore_en = 1;
    ldo_en0.da_ldohp_dacdrv_en = 1;
    ldo_en0.da_adc_reg_en = 1;
    ldo_en0.da_mic_reg_en = 1;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &ldo_en0);

    HREAD_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);                   //HWRITE(0x8b01,0x51);
    lod_cfg0.rg_mic_hvreg_vc = 1;
    lod_cfg0.rg_adc_reg_vol_sel = 2;
    lod_cfg0.rg_adc_reg_ibias_sel = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg0, &lod_cfg0);

    HREAD_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);                   //HWRITE(0x8b02,0x51);
    lod_cfg1.rg_ibsel_hpldo = 1;
    lod_cfg1.rg_ldo_voutsel = 2;
    lod_cfg1.rg_ibsel_ldo = 1;
    HWRITE_STRUCT(CORE_codec_ldo_cfg1, &lod_cfg1);

    HREAD_STRUCT(CORE_codec_vmid_en0, &vmid_en0);                   //HWRITE(0x8b05,0x26);
    vmid_en0.da_vmid_discharge = 0;
    vmid_en0.da_vmid_en = 1;
    vmid_en0.da_vmid_bias_en = 1;
    vmid_en0.da_vmid_bg_en = 1;
    HWRITE_STRUCT(CORE_codec_vmid_en0, &vmid_en0);

    HREAD_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);                 //HWRITE(0x8b06,0x08);
    vmid_cfg0.rg_vmid_isel_lsb = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg0, &vmid_cfg0);

    HREAD_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);                 //HWRITE(0x8b07,0x68);
    vmid_cfg1.rg_vmid_isel_msb = 8;
    vmid_cfg1.rg_vmid_discharge_cc = 1;
    vmid_cfg1.rg_vmid_discharge_auto = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg1, &vmid_cfg1);

    HREAD_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);                 //HWRITE(0x8b08,0x05);
    vmid_cfg2.rg_vmid_bg_fastsetting = 1;
    vmid_cfg2.rg_vmid_vref_sel = 1;
    HWRITE_STRUCT(CORE_codec_vmid_cfg2, &vmid_cfg2);

    HREAD_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);                 //HWRITE(0x8b09,0x84);
    vmid_cfg3.rg_vmid_bg_trim = 4;
    vmid_cfg3.rg_vmid_bg_rct = 8;
    HWRITE_STRUCT(CORE_codec_vmid_cfg3, &vmid_cfg3);

    memset(&adc_en0, 0, sizeof(adc_en0));                           //HWRITE(0x8b0b,0xe0);
    adc_en0.da_sdm_dem_en = 1;
    adc_en0.da_sdm_en = 1;
    adc_en0.da_aadc_top_ibias_en = 1;
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    memset(&adc_en1, 0, sizeof(adc_en1));                           //HWRITE(0x8b0c,0x07);
    adc_en1.da_micbias_en = 1;
    adc_en1.da_pga_en = 1;
    adc_en1.da_pga_diff_en = 0;
    HWRITE_STRUCT(CORE_au_adc_en1, &adc_en1);

    memset(&adc_cfg0, 0, sizeof(adc_cfg0));                         //HWRITE(0x8b0d,0x55);
    adc_cfg0.rg_sdm_cmp_isel = 1;
    adc_cfg0.rg_sdm_ota1_isel = 1;
    adc_cfg0.rg_sdm_ota2_isel = 1;
    adc_cfg0.rg_sdm_ref_isel = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg0, &adc_cfg0);

    memset(&adc_cfg1, 0, sizeof(adc_cfg1));                         //HWRITE(0x8b0e,0x51);
    adc_cfg1.rg_pga_opa2_ibias_sel = 1;
    adc_cfg1.rg_pga_cz_ibias_sel = 2;
    adc_cfg1.rg_pga_gm_ibias_sel = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg1, &adc_cfg1);

    HWRITE(CORE_au_adc_cfg2, 0x05);                                 //HWRITE(0x8b0f,0x05);

    memset(&adc_cfg3, 0, sizeof(adc_cfg3));                         //HWRITE(0x8b10,0x04);
    adc_cfg3.rg_pga_gc = 4;
    HWRITE_STRUCT(CORE_au_adc_cfg3, &adc_cfg3);

    memset(&adc_cfg4, 0, sizeof(adc_cfg4));                         //HWRITE(0x8b11,0x05);
    adc_cfg4.rg_pga_rcfilt_coarse = 1;
    adc_cfg4.rg_pga_dcoc_ibc = 1;
    HWRITE_STRUCT(CORE_au_adc_cfg4, &adc_cfg4);

    memset(&adc_cfg5, 0, sizeof(adc_cfg5));                         //HWRITE(0x8b12,0x4a);
    adc_cfg5.rg_micbias_rc = 2;
    adc_cfg5.rg_micbias_ibias_sel = 1;
    adc_cfg5.rg_pga_rcfilt_fine = 2;
    HWRITE_STRUCT(CORE_au_adc_cfg5, &adc_cfg5);
}

void AUDIO_AdcAnaStop(void)
{
    codec_ldo_en0RegDef ldo_en0;
    au_adc_en0RegDef adc_en0;
    au_adc_en1RegDef adc_en1;

    HREAD_STRUCT(CORE_codec_ldo_en0, &ldo_en0);
    ldo_en0.da_adc_reg_en = 0;
    HWRITE_STRUCT(CORE_codec_ldo_en0, &ldo_en0);

    memset(&adc_en0, 0, sizeof(adc_en0));                           //HWRITE(0x8b0b,0x20);
    adc_en0.da_sdm_dem_en = 1;
    HWRITE_STRUCT(CORE_au_adc_en0, &adc_en0);

    memset(&adc_en1, 0, sizeof(adc_en1));                           //HWRITE(0x8b0c,0x00);
    HWRITE_STRUCT(CORE_au_adc_en1, &adc_en1);
}
