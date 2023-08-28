/**
 * @file yc11xx_eq.c
 * @brief
 * @date 2021-08-09
 */
#include "yc11xx_eq.h"
//#include "yc_debug.h"


int EqIIRCoef[51] = {2097152, 2097152, -4183320, 2086200, 4183320, -2086200, 2097152,
                     -4172328, 2075305, 4172328, -2075305, 2097152, -4150323, 2053683, 4150323, -2053683,
                     2097152, -4106221, 2011096, 4106221, -2011096, 2097152, -4017639, 1928430, 4017639,
                     -1928430, 1982095, -3701043, 1750884, 3701043, -1635827, 2097152, -3444099, 1468442,
                     3444099, -1468442, 2097152, -2663089, 977918, 2663089, -977918, 2097152, -1140315,
                     183477, 1140315, -183477, 2097152, 314355, -1468442, -314355, 1468442
                     };
//static uint8_t coefchecksum=0;

/**
 * @Description: EQ initialization
 * @param {*}:   None
 * @return {*}:  None
 */
void EQ_Init(uint8_t isMRam,EQ_BitModeSelTypeDev dataBit,EQ_NumChannel channel)
{
    CLKOFF2RegDef clockOff;
	EQ_CTRLRegDef eqCtrl;
    HREAD_STRUCT(CORE_CLKOFF2, &clockOff);
    clockOff.eq_clkoff = 0;
    HWRITE_STRUCT(CORE_CLKOFF2, &clockOff);
    EQ_CoefInit();

	HREAD_STRUCT(CORE_EQ_CTRL, &eqCtrl);
	eqCtrl.src_mram_sel = isMRam;
	eqCtrl.dst_mram_sel = isMRam;
	eqCtrl.wid_sel = dataBit;
    eqCtrl.nrof_channels = channel;
    eqCtrl.eq_blken = 1;
    eqCtrl.eq_start = 0;
	HWRITE_STRUCT(CORE_EQ_CTRL, &eqCtrl);
}

/**
 * @Description: EQ work function
 * @param {uint8_t *} srcAddr:Original audio
 * @param {uint8_t *} dstAddr:Audio after EQ processing
 * @param {uint16_t} len:Data length
 * @param {EQ_BitModeSelTypeDev} bit:Effective data bit
 * 								    EQ_16Bit/EQ_24Bit
 * @param {EQ_NumChannel} channel:Stereo or mono
 * 									    EQ_Mono/EQ_Stereo
 * @return {*}None
 */
void EQ_Work(uint8_t *srcAddr, uint8_t *dstAddr, uint16_t len)
{
    EQ_CTRLRegDef eq_ctrl;
    HWRITEW(CORE_EQ_SRC_SADDR, srcAddr);
    HWRITEW(CORE_EQ_DST_SADDR, dstAddr);
    HWRITEW(CORE_EQ_BUF_LEN, len - 1);
    //DEBUG_LOG_STRING("CORE_EQ_SRC_SADDR: %04x    %04x  %d.\r\n" , srcAddr,dstAddr,len-1);

    HREAD_STRUCT(CORE_EQ_CTRL, &eq_ctrl);
    eq_ctrl.eq_start = ENABLE;
    HWRITE_STRUCT(CORE_EQ_CTRL, &eq_ctrl);
	
    //while((HREAD(CORE_MISC_STATUS)&0x80)==0);
    EQ_WorkEndCheck();
	eq_ctrl.eq_start = DISABLE;
    HWRITE_STRUCT(CORE_EQ_CTRL, &eq_ctrl);
}

/**
 * @Description: EQ stopped working
 * @param {*}None
 * @return {*}None
 */
void EQ_Off(void)
{
    EQ_CTRLRegDef eq_ctrl;

    HREAD_STRUCT(CORE_EQ_CTRL, &eq_ctrl);

    eq_ctrl.eq_blken = DISABLE;
    eq_ctrl.eq_start = DISABLE;

    HWRITE_STRUCT(CORE_EQ_CTRL, &eq_ctrl);
}
void EQ_WorkEndCheck(void)
{
    while(HREADW(CORE_EQ_RPTR) != HREADW(CORE_EQ_SRC_SADDR) + HREADW(CORE_EQ_BUF_LEN) + 1);
}
/**
 * @Description: Get EQ read pointer position
 * @param {*}
 * @return {*}uint16_t:Returns the lower sixteen bits of the read pointer address
 */
uint16_t EQ_GetDmaRptr(void)
{
    return HREADW(CORE_EQ_RPTR);
}

/**
 * @Description: Get EQ write pointer position
 * @param {*}
 * @return {*}uint16_t:Returns the lower sixteen bits of the write pointer address
 */
uint16_t EQ_GetDmaWptr(void)
{
    return HREADW(CORE_EQ_WPTR);
}

void EQ_CoefInit(void)
{
    int k = 0;
    //uint8_t coefchecksum=0;
    HWRITE(CORE_ADC_COEF_CTRL, 0x04);
    for (k = 0; k < 51; k++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, (uint8_t)EqIIRCoef[k]);
        HWRITE(CORE_ADDA_COEF_WDATA_1, (uint8_t)(EqIIRCoef[k] >> 8));
        HWRITE(CORE_ADDA_COEF_WDATA_2, (uint8_t)(EqIIRCoef[k] >> 16));
        HWRITE(CORE_ADC_COEF_CTRL, 0x0c);
        HWRITE(CORE_ADC_COEF_CTRL, 0x04);
        //		coefchecksum+=((uint8_t)EqIIRCoef[k]+(uint8_t)(EqIIRCoef[k]>>8)+(uint8_t)(EqIIRCoef[k]>>16));
        //DEBUG_LOG_STRING("coefchecksum: %x %x.\r\n" ,coefchecksum,HREAD(CORE_EQ_COEF_CHKSUM));

    }
    for(k = 0; k < 66; k++)
    {
        HWRITE(CORE_ADDA_COEF_WDATA_0, 0);
        HWRITE(CORE_ADDA_COEF_WDATA_1, 0);
        HWRITE(CORE_ADDA_COEF_WDATA_2, 0);
        HWRITE(CORE_ADC_COEF_CTRL, 0x0c);
        HWRITE(CORE_ADC_COEF_CTRL, 0x04);
    }
    HWRITE(CORE_ADC_COEF_CTRL, 0x00);
}

/**
 * @Description: Get EQ checksum
 * @param {*}
 * @return {*}uint8_t:Get EQ checksum
 */
uint8_t EQ_GetChecksum(void)
{
    return HREAD(CORE_EQ_COEF_CHKSUM);
}
