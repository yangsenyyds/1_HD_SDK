#include "yc11xx_cvsd.h"
#include "yc11xx.h"
#include "btreg.h"
#include "yc11xx_bt_interface.h"
#include "yc_debug.h"


const int coef_cvsd[41] =
{
    0x000c, 0x0012, 0x001b, 0x0024, 0x0029, 0x0028, 0x001d, 0x0007,
    0xffe4, 0xffb9, 0xff8a, 0xff5f, 0xff42, 0xff3c, 0xff53, 0xff8c,
    0xffe4, 0x0050, 0x00c2, 0x0125, 0x0163, 0x0168, 0x0127, 0x009d,
    0xffd5, 0xfee7, 0xfdf7, 0xfd33, 0xfccb, 0xfcea, 0xfdaf, 0xff27,
    0x0149, 0x03f3, 0x06ee, 0x09f6, 0x0cbd, 0x0ef9, 0x106b, 0x10eb, 0x0000
};

/**
 * @Description: CVSD enable
 * @param {*}None
 * @return {*}None
 */
void CVSD_ClkEnable(void)
{
	CoreReg_ClkControl(REG_CLOCK_OFF_VOICE_FILTER, ENABLE);
}

/**
 * @Description: CVSD disenable
 * @param {*}None
 * @return {*}None
 */
void CVSD_ClkDisable(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    cvsd_ctrl.cvsd_grp_en = 0;

    HWRITE_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);
}

/**
 * @Description: Wait to clear the filter parameters
 * @param {*}
 * @return {*}
 */
void CVSD_WaitCvsdFilterClrDone(void)
{
    PERIPHERAL_STATUSRegDef status_reg;

    do
    {
        HREAD_STRUCT(CORE_PERIPHERAL_STATUS, &status_reg);
    }while(status_reg.cvsd_filt_clr_done == 0);
}

/**
 * @Description: Initialization function
 * @param {*}
 * @return {*}
 */
void CVSD_CoefInit(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;
    ADDA_COEF_WR_CTRLRegDef coef_wrctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    cvsd_ctrl.cvsd_filter_clr = 1;

    HWRITE_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    HREAD_STRUCT(CORE_ADDA_COEF_WR_CTRL, &coef_wrctrl);

    memset(&coef_wrctrl, 0, sizeof(coef_wrctrl));
    coef_wrctrl.cvsd_coef_wr_en = 1;

    HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &coef_wrctrl);

    for(uint8_t i = 0; i < 41; i ++)
    {
        HWRITEW(CORE_ADDA_COEF_WDATA_0, coef_cvsd[i]);

        coef_wrctrl.cvsd_coef_strobe = 1;

        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &coef_wrctrl);

        coef_wrctrl.cvsd_coef_strobe = 0;

        HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &coef_wrctrl);
    }

    coef_wrctrl.cvsd_coef_wr_en = 0;
    coef_wrctrl.cvsd_coef_strobe = 0;

    HWRITE_STRUCT(CORE_ADDA_COEF_WR_CTRL, &coef_wrctrl);

    CVSD_WaitCvsdFilterClrDone();
}

/**
 * @Description: start-up
 * @param {*}
 * @return {*}
 */
void CVSD_Start(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    cvsd_ctrl.cvsd_dec_8k_mode_en = 1;

#ifdef FUNCTION_CVSD_OVER_SOFT_8KTO48K

    cvsd_ctrl.cvsd_dec_48k_mode_en = 1;
    cvsd_ctrl.cvsd_ff_64to48k_en = 1;

#else

    cvsd_ctrl.cvsd_dec_48k_mode_en = 0;
    cvsd_ctrl.cvsd_ff_64to48k_en = 0;

#endif

    cvsd_ctrl.cvsd_filter_clr = 0;
    cvsd_ctrl.cvsd_dma_en = 1;

    HWRITE_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);
}

/**
 * @Description:CVSD register clear
 * @param {*}
 * @return {*}
 */
void CVSD_Stop(void)
{
    HWRITE(CORE_CVSD_CONTROL, 0);
}

/**
 * @Description: Configure CVSD buff address
 * @param {uint32_t} cvsd_outbufferaddr:CVSD coded output address
 * @param {uint16_t} cvsd_outbuffersize:Output buff size after CVSD encoding
 * @param {uint32_t} cvsd_pcmInbufferaddr:Address to be CVSD encoded
 * @param {uint16_t} cvsd_pcmInbuffersize:The size of the data to be CVSD encoded
 * @return {*}None
 */
void CVSD_OutInit(uint32_t cvsd_outbufferaddr, uint16_t cvsd_outbuffersize, uint32_t cvsd_pcmInbufferaddr, uint16_t cvsd_pcmInbuffersize)
{
    bool pcm_inmramflag = reg_check_ram_m0(cvsd_pcmInbufferaddr);
    bool cvsd_outmramflag = reg_check_ram_m0(cvsd_outbufferaddr);

    CVSD_CTRL1RegDef cvsd_ctrl1;

    HWRITEW(CORE_PCM_SOURCE_ADDRESS, (uint16_t) cvsd_pcmInbufferaddr);	// PCM In Setting

    HREAD_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    cvsd_ctrl1.cvsd_pcmin_mram_sel = pcm_inmramflag;
    cvsd_ctrl1.cvsd_cvsdout_mram_sel = cvsd_outmramflag;

    HWRITE_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    HWRITEW(CORE_CVSD_DEST_ADDRESS, cvsd_outbufferaddr);
}

/**
 * @Description: Configure CVSD buff address
 * @param {uint32_t} cvsd_inbufferaddr:CVSD data address to decode
 * @param {uint16_t} cvsdI_inbuffersize:CVSD data size to decode
 * @param {uint32_t} cvsd_pcmoutbufferaddr:Data buff address after CVSD decoding
 * @param {uint16_t} cvsd_pcmoutbuffersize:Data buff size after CVSD decoding
 * @return {*}None
 */
void CVSD_InInit(uint32_t cvsd_inbufferaddr, uint16_t cvsdI_inbuffersize, uint32_t cvsd_pcmoutbufferaddr, uint16_t cvsd_pcmoutbuffersize)
{
    bool pcm_outmramflag = reg_check_ram_m0(cvsd_pcmoutbufferaddr);
    bool cvsd_inmramflag = reg_check_ram_m0(cvsd_inbufferaddr);

    CVSD_CTRL1RegDef cvsd_ctrl1;

    HWRITEW(CORE_CVSD_SOURCE_ADDRESS, (uint16_t)cvsd_inbufferaddr); // Init cvsd in buffer addr

    HREAD_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    cvsd_ctrl1.cvsd_8kpcmout_mram_sel = pcm_outmramflag;
    cvsd_ctrl1.cvsd_cvsdin_mram_sel = cvsd_inmramflag;

    HWRITE_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    HWRITEW(CORE_PCM_8K_DEST_ADDRESS, (uint16_t)cvsd_pcmoutbufferaddr); // In cvsd, it have 8K and 48K out
}

/**
 * @Description:Cvsd encoding and decoding initialization function, encoding and decoding at the same time
 * @param {uint32_t} cvsd_inbufferaddr:Buff address before decoding
 * @param {uint16_t} cvsdI_inbuffersize:The size of the data to be decoded
 * @param {uint32_t} cvsd_pcmoutbufferaddr:The decoded buff address
 * @param {uint16_t} cvsd_pcmoutbuffersize:The decoded buff size
 * @param {uint32_t} cvsd_outbufferaddr:Buff address after CVSD encoding
 * @param {uint16_t} cvsd_outbuffersize:The size of the encoded data
 * @param {uint32_t} cvsd_pcmInbufferaddr:Raw data to be encoded
 * @param {uint16_t} cvsd_pcmInbuffersize:The size of the original data to be encoded
 * @param {uint8_t} vsdGrpLength:Group length
 * @return {*}None
 */
void CVSD_Init(uint32_t cvsd_inbufferaddr, uint16_t cvsdI_inbuffersize, uint32_t cvsd_pcmoutbufferaddr, uint16_t cvsd_pcmoutbuffersize, uint32_t cvsd_outbufferaddr, uint16_t cvsd_outbuffersize, uint32_t cvsd_pcmInbufferaddr, uint16_t cvsd_pcmInbuffersize, uint16_t cvsd_grplength)
{
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "Audio_DacInit: 0x%04X", LOG_POINT_A630, HREAD(CORE_CVSD_CONTROL));
	SBC_CTRL2RegDef sbc_ctrl2;

    CVSD_ClkEnable();// Enable clock

    CVSD_OutInit(cvsd_outbufferaddr, cvsd_outbuffersize, cvsd_pcmInbufferaddr, cvsd_pcmInbuffersize); // Init cvsd out, pcm in buffer, and init adc
    CVSD_InInit(cvsd_inbufferaddr, cvsdI_inbuffersize , cvsd_pcmoutbufferaddr, cvsd_pcmoutbuffersize);

    HWRITEW(CORE_CVSD_BUFFER_LEN, cvsd_outbuffersize - 1);

    //cvsd filter enable, it will reuse sbc resource. 1: for cvsd; 0: for sbc/msbc
    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);

    sbc_ctrl2.sbc_cvsd_mux_en = 1;

    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);

    CVSD_CoefInit(); // clear and init cvsd filter

    HWRITEW(CORE_CVSD_GROUP_LENGTH, cvsd_grplength); // cvsd encode and decode will work with the length

    CVSD_Start();
}

/**
 * @Description:Check if the CVSD module is working
 * @param {*}None
 * @return {bool} Is working return true
 */
bool CVSD_CheckEnable(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    return cvsd_ctrl.cvsd_grp_en;
}

/**
 * @Description: Check if dma is working in CVSD
 * @param {*}None
 * @return {bool}Is working return true
 */
bool CVSD_CheckWorkStart(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    return cvsd_ctrl.cvsd_dma_en;
}

/**
 * @Description: Check whether the CVSD work is completed
 * @param {*}None
 * @return {bool}Return true when work is complete
 */
bool CVSD_CheckWorkInprocess(void)
{
    PERIPHERAL_STATUSRegDef status_reg;

    HREAD_STRUCT(CORE_PERIPHERAL_STATUS, &status_reg);

    return (status_reg.cvsd_grp_done == 0);
}

/**
 * @Description: Is the data to be decoded in sram or mram
 * @param {*}None
 * @return {bool}mram returns true
 */
bool CVSD_CheckInUseMRam(void)
{
    CVSD_CTRL1RegDef cvsd_ctrl1;

    HREAD_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    return cvsd_ctrl1.cvsd_cvsdin_mram_sel;
}

/**
 * @Description: Cvsd encoding output ram selection
 * @param {*}None
 * @return {bool}mram returns true
 */
bool CVSD_CheckOutUseMRam(void)
{
    CVSD_CTRL1RegDef cvsd_ctrl1;

    HREAD_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    return cvsd_ctrl1.cvsd_cvsdout_mram_sel;
}

/**
 * @Description: Get the buff address that needs to be decoded
 * @param {*}None
 * @return {uint32_t}Get the buff address that needs to be decoded
 */
uint32_t CVSD_GetInAddr(void)
{
    uint32_t cvsd_indataptr;

    CVSD_CTRL1RegDef cvsd_ctrl1;

    HREAD_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    cvsd_indataptr = HREADW(CORE_CVSD_SOURCE_ADDRESS);

    if(cvsd_ctrl1.cvsd_cvsdin_mram_sel)
    	cvsd_indataptr = reg_map_m0(cvsd_indataptr);
    else
    	cvsd_indataptr = reg_map(cvsd_indataptr);

    return cvsd_indataptr;
}

/**
 * @Description: Get the encoded buff address
 * @param {*}None
 * @return {uint32_t}Get the encoded buff address
 */
uint32_t CVSD_GetOutAddr(void)
{
    uint32_t cvsd_outdataptr;

    CVSD_CTRL1RegDef cvsd_ctrl1;

    HREAD_STRUCT(CORE_CVSD_CTRL1, &cvsd_ctrl1);

    cvsd_outdataptr = HREADW(CORE_CVSD_DEST_ADDRESS);

    if(cvsd_ctrl1.cvsd_cvsdout_mram_sel)
        cvsd_outdataptr = reg_map_m0(cvsd_outdataptr);
    else
        cvsd_outdataptr = reg_map(cvsd_outdataptr);

    return cvsd_outdataptr;
}

/**
 * @Description: CVSD starts to work
 * @param {*}None
 * @return {*}None
 */
void CVSD_WorkEnable(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    cvsd_ctrl.cvsd_grp_en = ENABLE;

    HWRITE_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);
}

/**
 * @Description: CVSD disable
 * @param {*}None
 * @return {*}None
 */
void CVSD_WorkDisable(void)
{
    CVSD_CONTROLRegDef cvsd_ctrl;

    HREAD_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);

    cvsd_ctrl.cvsd_grp_en = DISABLE;

    HWRITE_STRUCT(CORE_CVSD_CONTROL, &cvsd_ctrl);
}


