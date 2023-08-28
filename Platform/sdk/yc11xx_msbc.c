/**
***************************************************************************
* @file:  yc11xx_msbc.c
* @Date:  2021-8-10-11:15:58
* @brief:  MSBC basic driver
**************************************************************************
*/

#include "yc11xx_msbc.h"
#include "btreg.h"
#include "yc11xx.h"
#include "reg_addr.h"
#include "reg_struct.h"
#include "yc11xx_sbc.h"

void MSBC_Init(void)
{
	SBC_CTRLRegDef sbc_ctrl;
	SBC_CLEARRegDef sbc_clearreg;

	MSBC_ClockOn();

	HREAD_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

	// before clear, should not change other params.
	//memset(&sbc_ctrl, 0, sizeof(sbc_ctrl));

	sbc_ctrl.sbc_block_en = 1;

	HWRITE_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

	HREAD_STRUCT(CORE_SBC_CLEAR, &sbc_clearreg);

	// before clear, should not change other params.
	//memset(&sbc_ctrl, 0, sizeof(sbc_ctrl));

	sbc_clearreg.sbc_msbc_x_wp_clr = 1;
	sbc_clearreg.sbc_v_offset_clr = 1;
	sbc_clearreg.sbc_sat_record_clr = 1;
	sbc_clearreg.sbc_error_clr = 1;
	sbc_clearreg.sbc_v_matrix_clr = 1;
	sbc_clearreg.sbc_farrow_filter_clr = 1;

	HWRITE_STRUCT(CORE_SBC_CLEAR, &sbc_clearreg);

	wait_lpodelay(1);

	HWRITE(CORE_SBC_CLEAR, 0x0);

	MSBC_BusyEait();
}

/**
 * @brief  :Check to see if decoding is complete and update status
 * @param    void
 * @return  :1:sbc decoder is busy
 */
uint8_t MSBC_UpdateCacheWait(void)
{
    MISC_STATUSRegDef misc_status;

    HREAD_STRUCT(CORE_MISC_STATUS, &misc_status);

    return misc_status.sbc_decoder_busy;
}

/**
 * @brief  :Load the data in the buffer
 * @param    void
 * @return  :
 */
uint8_t MSBC_LoadCacheWait(void)
{
    if(((HREADW(CORE_SBC_SWP)) == (HREADW(CORE_SBC_SOURCE_READ_PTR))))
        return 0;
    else
        return 1;
}

/**
 * @brief  :MSBC encode/decode stop
 * @param    void
 * @return  None
 */
void MSBC_EncDecStop(void)
{
    SBC_SCTRLRegDef sbc_sctrl;

    HWRITE(CORE_SBC_CTRL, 0);

    HREAD_STRUCT(CORE_SBC_SCTRL, &sbc_sctrl);

    sbc_sctrl.sbc_sctrl_reserved = 0;
    sbc_sctrl.cache_ctrl = 0;
    sbc_sctrl.sbc_ff_48k_en = 0;
    sbc_sctrl.msbc_dec_en = 0;
    sbc_sctrl.sbc_eq_en = 0;

    HWRITE_STRUCT(CORE_SBC_SCTRL, &sbc_sctrl);

    HWRITEW(CORE_SBC_SWP, 0 );
}

/**
 * @brief  :MSBC starts coding
 * @param    sbcOutAddr : The start address of the SBC data after the MSBC coding is complete
 * @param    sbc_outsize :MSBC Length of SBC data after encoding
 * @param    pcm_inptr :The start address of the PCM data to be encoded
 * @param    pcm_insize:Length of PCM data to be encoded
 * @param    cache_bufferaddr  :MSBC code is buffer space
 * @return  :NONE
 */
void MSBC_EncodeStart(uint32_t sbcOutAddr, uint16_t sbc_outsize , uint32_t pcm_inptr, uint16_t pcm_insize, uint32_t cache_bufferaddr)
{
    SBC_CTRLRegDef sbc_ctrl;
    SBC_CLEARRegDef sbc_clearreg;
    SBC_CTRL2RegDef sbc_ctrl2;

    bool isOutMRAM = reg_check_ram_m0(sbcOutAddr);
    bool isPCMInMRAM = reg_check_ram_m0(pcm_inptr);
    bool isCacheInMRAM = reg_check_ram_m0(cache_bufferaddr);

    HWRITEW(CORE_SBC_SRC_START, pcm_inptr);
    HWRITEW(CORE_SBC_DST_START, sbcOutAddr);

    HREAD_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

    sbc_ctrl.sbc_src_bit16 = isPCMInMRAM;
    sbc_ctrl.sbc_dst_bit16 = isOutMRAM;
    sbc_ctrl.msbc_en = 1;

    HWRITE_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

    HWRITEW(CORE_SBC_SRC_LEN, pcm_insize + 1);
    HWRITEW(CORE_SBC_DST_LEN, sbc_outsize);

    HREAD_STRUCT(CORE_SBC_CLEAR, &sbc_clearreg);
    sbc_clearreg.sbc_src_addr_clr = 1;
    sbc_clearreg.sbc_dst_addr_clr = 1;
    HWRITE_STRUCT(CORE_SBC_CLEAR, &sbc_clearreg);

    HWRITE(CORE_SBC_CLEAR, 0);

    HREAD_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);
    sbc_ctrl.sbc_block_en = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

    HWRITEW(CORE_SBC_CACHE_CFG0, cache_bufferaddr);

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_addr = isCacheInMRAM;
    sbc_ctrl2.sbc_cache_load = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_load = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);// generate signal

    while(MSBC_UpdateCacheWait());

    HWRITEW(CORE_SBC_SWP, (pcm_insize));

    while(MSBC_LoadCacheWait());

    HWRITEW(CORE_SBC_CACHE_CFG0, cache_bufferaddr);   // Update data

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_update = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_update = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);// generate signal

    while(MSBC_UpdateCacheWait());

    MSBC_EncDecStop();
}

/**
 * @brief  :MSBC starts decoding
 * @param    sbcInAddr : The start address of the SBC data to be decoded
 * @param    sbc_insize :Length of SBC data to decode
 * @param    pcmOutPtr :Start address of PCM data output after MSBC decoding
 * @param    pcm_outsize:Length of PCM data after MSBC decoding
 * @param    cache_bufferaddr  :MSBC decode is buffer space
 * @return  :NONE
 */
void MSBC_DecodeStart(uint32_t sbc_inaddr, uint16_t sbc_insize, uint32_t pcm_outptr, uint16_t pcm_outsize , uint32_t cache_bufferaddr)
{
    SBC_CTRLRegDef sbc_ctrl;
    SBC_CLEARRegDef sbc_clearreg;
    SBC_SCTRLRegDef sbc_sctrl;
    SBC_CTRL2RegDef sbc_ctrl2;

    bool isInMRAM = reg_check_ram_m0(sbc_inaddr);
    bool isPCMOutMRAM = reg_check_ram_m0(pcm_outptr);
    bool isCacheInMRAM = reg_check_ram_m0(cache_bufferaddr);

    HWRITEW(CORE_SBC_SRC_START, sbc_inaddr);
    HWRITEW(CORE_SBC_DST_START, pcm_outptr);

    HREAD_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);
    sbc_ctrl.sbc_src_bit16 = isInMRAM;
    sbc_ctrl.sbc_dst_bit16 = isPCMOutMRAM;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

    HWRITEW(CORE_SBC_SRC_LEN, sbc_insize + 1);
    HWRITEW(CORE_SBC_DST_LEN, pcm_outsize);

    HREAD_STRUCT(CORE_SBC_CLEAR, &sbc_clearreg);
    sbc_clearreg.sbc_src_addr_clr = 1;
    sbc_clearreg.sbc_dst_addr_clr = 1;
    HWRITE_STRUCT(CORE_SBC_CLEAR, &sbc_clearreg);
    HWRITE(CORE_SBC_CLEAR, 0);


    HREAD_STRUCT(CORE_SBC_SCTRL, &sbc_sctrl);
    sbc_sctrl.msbc_dec_en = 1;
    HWRITE_STRUCT(CORE_SBC_SCTRL, &sbc_sctrl);

    HREAD_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);
    sbc_ctrl.sbc_block_en = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL, &sbc_ctrl);

    HWRITEW(CORE_SBC_CACHE_CFG0, cache_bufferaddr ); // Load data

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_addr = isCacheInMRAM;
    sbc_ctrl2.sbc_cache_load = 1;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_load = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);// generate signal

    while(MSBC_UpdateCacheWait());


    HWRITEW(CORE_SBC_SWP, sbc_insize);// Wait sbc ready

    while(MSBC_LoadCacheWait())
    {
        SBC_STATUSRegDef sbcStatus;

        HREAD_STRUCT(CORE_SBC_STATUS, &sbcStatus);

        if(sbcStatus.sbc_error != 0)
        {
            HWRITE(0x4ff8, HREAD(0x4ff8) + 1);
            break;
        }
    }


    HWRITEW(CORE_SBC_CACHE_CFG0, cache_bufferaddr);  // Update data

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_update = 1;
    sbc_ctrl2.sbc_cache_addr = isCacheInMRAM;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);

    HREAD_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);
    sbc_ctrl2.sbc_cache_update = 0;
    HWRITE_STRUCT(CORE_SBC_CTRL2, &sbc_ctrl2);// generate signal
    while(MSBC_UpdateCacheWait());

    MSBC_EncDecStop();
}

/**
 * @brief  :open SBC clock
 * @param    void
 * @return  None
 */
void MSBC_ClockOn(void)
{
    CoreReg_ClkControl(REG_CLOCK_OFF_SBC, ENABLE); //open voice clock
}
/**
 * @brief  :close sbc clock
 * @param    void
 * @return  None
 */
void MSBC_ClockOff(void)
{
    CoreReg_ClkControl(REG_CLOCK_OFF_SBC, DISABLE); //open voice clock
}

/**
 * @brief  :Check whether the MSBC is busy
 * @param    void
 * @return  None
 */
void MSBC_BusyEait(void)
{
    while(((HREAD(CORE_MISC_STATUS)) & BIT4) != 0);
}









