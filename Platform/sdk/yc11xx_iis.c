/**
***************************************************************************
* @file:  yc11xx_iis.c
* @Date:  2021-6-15-17:35:29
* @brief:   I2S driver
**************************************************************************
*/

#include "yc11xx_iis.h"
#include "reg_struct.h"
#include "reg_addr.h"
#include "board_config.h"

/**
 * @Description:Write receive buff address
 * @param {uint8_t} *val:buff address
 * @return {*}None
 */
void IIS_SetDmaRxSaddr (uint8_t *val)
{
    if(val == NULL)
    {
        return;
    }
    HWRITEW(CORE_I2S_RX_SADDR, val);
}

/**
 * @Description: Output buff address
 * @param {uint8_t} *val:buff address
 * @return {*}None
 */
void IIS_SetDmaTxSaddr (uint8_t *val)
{
    if(val == NULL)
    {
        return;
    }
    HWRITEW(CORE_I2S_TX_SADDR, val);
    HWRITEW(CORE_IIS_TX_WRPTR, val);
}

/**
 * @brief       IIS DMA max length, need minus 1
 * @param    val:buff len
 * @return     None
 */
void IIS_SetDmaLen (unsigned short val)
{

    HWRITEW(CORE_I2S_LEN, val);
}



/**
 * @brief    I2S initialization function, need to pass in the structure IIS_ConfigDev, configure TX BUFF or RX BUFF, if not needed, configure it to NULL. Standard mode, left-justified mode, right-justified mode transmits two-channel audio, and PCM mode transmits mono audio. In the buff address, if the address is 0x1000xxxx, it is the CM0 core address, and if the address is 0x1000xxxx, it is the BT core address.
 * @param    Config:IIS mode Config
                       TxRxMode:IIS_TxEnable/IIS_RxEnable
                       IISMode:IIS_StandardMode/IIS_PcmMode/IIS_LeftMode/IIS_RightMode
                       BitModeSel:IIS_16Bit/IIS_24Bit/IIS_32Bit/IIS_64Bit
                       SignificantBit:IIS_SourceData16Bit/I2S_SourceData24Bit
                       MSMode:IIS_SlaveMode/IIS_MasterMode
                       BaudRate:IIS_BaudRate8K/IIS_BaudRate11p025K/IIS_BaudRate12K/IIS_BaudRate16K/IIS_BaudRate22p05K/
                                IIS_BaudRate24K/IIS_BaudRate44p1K/IIS_BaudRate48K
                       BufferaddrTx:IIS send pointer
                       BufferaddrRx:IIS receive pointer
                       Bufferlen:buff len
                       IsWptrMode:IIS_CyclicMode/IIS_WptrMode
 * @return  None
 */

void IIS_Init(IIS_ConfigDev config)
{
    I2S_MRAM_SELRegDef iis_mramSel;
    I2S_CTRLRegDef iis_ctrl;
    AUDIO_DIV_CLK_SELRegDef iis_clkdiv;
    AUDIO_CLKSELDef iis_clksource;

    uint8_t sound_track = 0;

    if((config.BufferAddrTx == NULL && config.BufferAddrRx == NULL) || config.BufferLen == 0 || config.TxRxMode == 0)
    {
        HREAD_STRUCT(CORE_I2S_CTRL, &iis_ctrl);
        iis_ctrl.i2s_en = 0;
        iis_ctrl.i2s_rx_en = 0;
        HWRITE_STRUCT(CORE_I2S_CTRL, &iis_ctrl);
        return;
    }

    if(config.IisMode != IIS_PcmMode)
    {
        sound_track = 2;
    }
    else
    {
        sound_track = 1;
    }

    CoreReg_ClkControl(REG_CLOCK_OFF_I2S, ENABLE);

    HREAD_STRUCT(CORE_I2S_MRAM_SEL, &iis_mramSel);

    if(config.IisMode != IIS_PcmMode)
    {
        iis_mramSel.pcm_one_channel = 0;
    }
    else
    {
        iis_mramSel.pcm_one_channel = 1;
    }
    if((reg_check_ram_m0(config.BufferAddrTx)) || (reg_check_ram_m0(config.BufferAddrRx)) )
    {
        iis_mramSel.i2s_mram_sel = 1;
    }
    else
    {
        iis_mramSel.i2s_mram_sel = 0;
    }
    if(config.IsWptrMode == IIS_WptrMode)
    {
        iis_mramSel.fifo_mode = 1;
    }
    else
    {
        iis_mramSel.fifo_mode = 0;
    }

    HWRITE_STRUCT(CORE_I2S_MRAM_SEL, &iis_mramSel);


    HREAD_STRUCT(CORE_I2S_CTRL, &iis_ctrl);

    if(config.IisMode == IIS_StandardMode)
        iis_ctrl.i2s_mode_sel = IIS_StandardMode;
    else if(config.IisMode == IIS_PcmMode)
        iis_ctrl.i2s_mode_sel = IIS_PcmMode;
    else if(config.IisMode == IIS_LeftMode)
        iis_ctrl.i2s_mode_sel = IIS_LeftMode;
    else
        iis_ctrl.i2s_mode_sel = IIS_RightMode;

    if(config.BitModeSel == IIS_16Bit)
        iis_ctrl.i2s_bit_mode_sel = 0; //IIS_16Bit;
    else if(config.BitModeSel == IIS_24Bit)
        iis_ctrl.i2s_bit_mode_sel = 1; //IIS_24Bit;
    else if(config.BitModeSel == IIS_32Bit)
        iis_ctrl.i2s_bit_mode_sel = 2; //IIS_32Bit;
    else
        iis_ctrl.i2s_bit_mode_sel = 3; //IIS_64Bit;

    if(config.SignificantBit == IIS_SourceData16Bit)
    {
        iis_ctrl.i2s_24bit_mode = 0;
    }
    else
    {
        iis_ctrl.i2s_24bit_mode = 1;
    }

    if(config.MSMode == IIS_MasterMode)
        iis_ctrl.i2s_master_mode = IIS_MasterMode;


    HREAD_STRUCT(CORE_AUDIO_DIV_CLK_SEL, &iis_clkdiv);
    HREADW_STRUCT(CORE_AUDIO_CLKSEL, &iis_clksource);

#ifndef CLOCK_HALVED
    if( config.BaudRate * config.BitModeSel * sound_track == 128 )
    {
        iis_clksource.i2s_clksel = IIS_Clk6p144M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 176 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 63);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 192 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 31);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 256 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 352 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 31);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 384 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 512 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 7);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 704 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 768 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 7);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 1024 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 3);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 1408 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 7);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 1536 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 3);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 2048 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 2);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 2816 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 3);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 3072 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 1);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 5632 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 1);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 6144 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 1);
    }
#else
    if( config.BaudRate * config.BitModeSel * sound_track == 128 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 176 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 31);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 192 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 31);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 256 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 7);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 352 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 384 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 15);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 512 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide3;
        HWRITE(CORE_I2S_CLKDIV, 3);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 704 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 7);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 768 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 7);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 1024 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide2;
        HWRITE(CORE_I2S_CLKDIV, 2);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 1408 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 3);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 1536 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 3);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 2048 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 2);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 2816 )
    {
        iis_clksource.i2s_clksel = IIS_Clk11p2896M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 1);
    }
    else if( config.BaudRate * config.BitModeSel * sound_track == 3072 )
    {
        iis_clksource.i2s_clksel = IIS_Clk12p288M;
        iis_clkdiv.i2s_divclk_sel = IIS_ClkDivide1;
        HWRITE(CORE_I2S_CLKDIV, 1);
    }

#endif
    HWRITE_STRUCT(CORE_AUDIO_DIV_CLK_SEL, &iis_clkdiv);
    HWRITEW_STRUCT(CORE_AUDIO_CLKSEL, &iis_clksource);
    IIS_SetDmaLen(config.BufferLen - 1);

    IIS_SetDmaRxSaddr(config.BufferAddrRx);
    IIS_SetDmaTxSaddr(config.BufferAddrTx);
    HWRITE_STRUCT(CORE_I2S_CTRL, &iis_ctrl);
    IIS_Enable(config);
}


/**
 * @brief  I2S enable function. When TX is turned on, only data is sent. If RX is enabled, both TX and RX are enabled, and the size of TX BUFF is equal to RX BUFF. You can set the TX BUFF address to NULL
 * @param  config IIS enable
 *                  TxRxMode:IIS_TxEnable/IIS_RxEnable
 */
void IIS_Enable(IIS_ConfigDev config)
{
    I2S_CTRLRegDef iis_ctrl;

    HREAD_STRUCT(CORE_I2S_CTRL, &iis_ctrl);
    if(config.TxRxMode == IIS_TxEnable)
    {
        iis_ctrl.i2s_en = 1;
    }
    else if(config.TxRxMode == IIS_RxEnable)
    {
        iis_ctrl.i2s_en = 1;
        iis_ctrl.i2s_rx_en = 1;
    }
    HWRITE_STRUCT(CORE_I2S_CTRL, &iis_ctrl);
}

/**
 * @brief  IIS is used when writing the pointer to determine whether the IIS data has been sent. Return true when finished.
 * @return bool
 */
bool IIS_IsSendComplete(void)
{
    return HREADW(CORE_IIS_TX_WRPTR) == HREADW(CORE_I2S_RDPTR);
}

/**
 * @brief  Get the length of unsent data
 * @return buff size
 */
uint16_t IIS_GetBufferSize(void)
{
    uint16_t rptr, wptr;

    // Check adc buffer already for cvsd work
    rptr = HREADW(CORE_I2S_RDPTR);
    wptr = HREADW((CORE_IIS_TX_WRPTR));
    if(wptr >= rptr)
    {
        return (wptr - rptr);
    }
    else
    {
        return (HREADW(CORE_I2S_LEN) + 1 - (rptr  - wptr));
    }
}

/**
 * @brief get remind buffer size
 * @return buff size
 */
uint16_t IIS_GetRemindBufferSize(void)
{
    return HREADW(CORE_I2S_LEN) + 1 - IIS_GetBufferSize();
}

/**
 * @brief set IIS write ptr
 * @return None
 */
void IIS_SetTxBuff (uint8_t *val)
{
    HWRITEW(CORE_IIS_TX_WRPTR, val);
}

/**
 * @brief get IIS write ptr
 * @return None
 */
uint16_t IIS_GetWptr(void)
{
    return HREADW(CORE_I2S_WRPTR);
}

/**
 * @brief get IIS read ptr
 * @return None
 */
uint16_t IIS_GetRprt(void)
{
    return HREADW(CORE_I2S_RDPTR);
}
