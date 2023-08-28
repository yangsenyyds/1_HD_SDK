#include "yc11xx_sdio.h"
#include "reg_addr.h"

/**
 * @brief  :SD set  Rx sample
 * @param    cmd:enable or disable
 * @return  None
 */
void SDIO_SetRxSample (FunctionalState cmd)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg (SDIO_CLK_CTRL);
    tmp &= (~(1 << 4)); // clear bit[4]

    if(cmd)
        tmp |= cmd;

    SDIO_WriteReg (SDIO_CLK_CTRL, tmp);
}

/**
 * @brief  £»SD set  POS sample
 * @param    cmd :enable or disable
 * @return  None
 */
void SDIO_SetTxSample (FunctionalState cmd)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg (SDIO_CLK_CTRL);
    tmp = tmp & 0xdf; // clear bit[5]

    if(cmd)
        tmp |= cmd;

    SDIO_WriteReg (SDIO_CLK_CTRL, tmp);
}

/**
 * @brief  :sd clk frequency select
 * @param    SDCLK_Frequency   :clk select
 * @return  None
 */

void SDIO_SetClkDiv (SDCLK_Fre_InitTypeDef SDCLK_Frequency)
{
    SDIO_WriteReg(SDIO_CLK_DIV, SDCLK_Frequency);
}

/**
 * @brief  :setting SD send transtion mode
 * @param    val  :   mode value
 * @return  None
 */
void SDIO_SetTranstionMode (uint8_t val)
{
    SDIO_WriteReg(SDIO_TRANS_MODE, val);// direct set reg
}

/**
 * @brief  :get SD care response error interruot
 * @param    vp:intterupt num
 * @return  None
 */
void SDIO_GetNormalInterupt (uint8_t *vp)
{
    *vp = SDIO_ReadReg (SDIO_NORM_IRQ);
}

/**
 * @brief  :get SD care response error interruot
 * @param    vp :intterupt num
 * @return  None
 */
void SDIO_GetErrorInterrupt (uint8_t *vp)
{
    *vp = SDIO_ReadReg (SDIO_ERR_IRQ);
}

void SDIO_GetCmdStatus (uint8_t *vp)
{
    *(vp) = SDIO_ReadReg (SDIO_CMD_STATUS);
}

void SDIO_GetDataStatus (uint8_t *vp)
{
    *(vp) = SDIO_ReadReg (SDIO_DAT_STATUS);
}

//Software Reset
/**
 * @brief  :Program software reset
 * @param    void
 * @return  None
 */
void SDIO_SoftwareReset (void)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_SW_RST);

    tmp = tmp & (~(1 << 0)); // clear bit[0]
    tmp = tmp | 0x01;
    SDIO_WriteReg(SDIO_SW_RST, tmp);

    tmp = tmp & (~(1 << 0)); // clear bit[0]
    tmp = tmp | 0x00;
    SDIO_WriteReg(SDIO_SW_RST, tmp);
}

/**
 * @brief  :Example Set the internal clock of the SD card
 * @param    cmd    :enable or diable
 * @return  None
 */
void SDIO_SetClkCmd (FunctionalState cmd)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg (SDIO_CLK_CTRL);
    tmp = tmp & (~(1 << 0)); // clear bit[0]

    if(cmd)
        tmp = tmp | cmd;

    SDIO_WriteReg(SDIO_CLK_CTRL, tmp);
}

/**
 * @brief  :Set the response type of the SD card
 * @param    val :response type
 * @return  None
 */
void SDIO_SetResponseType (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_TRANS_MODE);
    tmp = tmp & 0xfc; // clear bit[1:0], resp_type
    tmp = tmp | val; // set bit[1:0], resp_type

    SDIO_WriteReg(SDIO_TRANS_MODE, tmp);
}

/**
 * @brief  £»command index, trigger one command transceive
 * @param    val
 * @return  None
 */
void SDIO_SetCmd (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = val & 0x3f; // bit[5:0]
    SDIO_WriteReg(SDIO_COMMAND, tmp);
}

/**
 * @brief  :sd card cmd
 * @param    val
 * @return  None
 */
void SDIO_SetCmdArg (uint32_t val)
{
    uint8_t tmp = 0;

    tmp = val & 0xff;
    SDIO_WriteReg(SDIO_CMD_ARG_B0, tmp);

    tmp = (val >> 8) & 0xff;
    SDIO_WriteReg(SDIO_CMD_ARG_B1, tmp);

    tmp = (val >> 16) & 0xff;
    SDIO_WriteReg(SDIO_CMD_ARG_B2, tmp);

    tmp = (val >> 24) & 0xff;
    SDIO_WriteReg(SDIO_CMD_ARG_B3, tmp);
}


/**
 * @brief
 * @param    val
 * @return  None
 */
void SDIO_SetCmdIndexChk (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_TRANS_MODE);
    tmp = tmp & (~(1 << 3)); // clear bit[3], cmd_idx_chk_en
    tmp = tmp | val; // set bit[3], cmd_idx_chk_en
    SDIO_WriteReg(SDIO_TRANS_MODE, tmp);
}

/**
 * @brief  :set block count
 * @param    val  :block num
 * @return  None
 */
void SDIO_SetBlockCount (uint16_t val)
{
    uint8_t tmp = 0;

    tmp = val & 0xff;
    SDIO_WriteReg(SDIO_BLK_CNT_L, tmp);

    tmp = (val >> 8) & 0xff;
    SDIO_WriteReg(SDIO_BLK_CNT_H, tmp);
}

// cmd_crc_chk
/**
 * @brief
 * @param    val
 * @return  None
 */
void SDIO_SetCmdCRCChk (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_TRANS_MODE);
    tmp = tmp & (~(1 << 2)); // clear bit[2], cmd_crc_chk_en
    tmp = tmp | val; // set bit[2], cmd_crc_chk_en
    SDIO_WriteReg(SDIO_TRANS_MODE, tmp);
}
/**
 * @brief  :get SD card response
 * @param    vp
 * @return  None
 */
void SDIO_GetResponse32 (uint8_t *vp)
{
    uint8_t tmp = 0;

    for (tmp = 0; tmp < 4; tmp ++)
    {
        *(vp + tmp) = SDIO_ReadReg (SDIO_RSP_B0 + tmp);
    }
}

// trans_dir
void SDIO_SetTranstioDir (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_TRANS_MODE);
    tmp = tmp & 0xdf; // clear bit[5], trans_dir
    tmp = tmp | val; // set bit[5], trans_dir
    SDIO_WriteReg(SDIO_TRANS_MODE, tmp);
}

// dat_present
/**
 * @brief  :set sd card data response
 * @param    val :    data command
 * @return  None
 */
void SDIO_SetDatResponse (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_TRANS_MODE);
    tmp = tmp & 0xef; // clear bit[4], dat_pres
    tmp = tmp | val; // set bit[4], dat_pres
    SDIO_WriteReg(SDIO_TRANS_MODE, tmp);
}

/**
 * @brief  :set sd card timeout count
 * @param    val
 * @return  None
 */
void SDIO_SetTimeoutCount (uint8_t val)
{
    SDIO_WriteReg(SDIO_TIMEOUT_CNT, val);
}

/**
 * @brief  :set sd card block size
 * @param    val:block size value
 * @return  None
 */
void SDIO_SetBlockSize (uint16_t val)
{
    uint8_t tmp = 0;

    tmp = val & 0xff;
    SDIO_WriteReg(SDIO_BLK_SIZE_L, tmp);
    tmp = (val >> 8) & 0xff;
    SDIO_WriteReg(SDIO_BLK_SIZE_H, tmp);
}
/**
 * @brief
 * @param    val
 * @return  None
 */
void SDIO_SetBusWidth (uint8_t val)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_TRANS_MODE);
    tmp = tmp & (~(1 << 6)); // clear bit[6], bus_width
    tmp = tmp | val; // set bit[6], bus_width
    SDIO_WriteReg(SDIO_TRANS_MODE, tmp);
}

/**
 * @brief  setting sd card DMA start address
 * @param    val   :DNA address
 * @return  None
 */
void SDIO_SetDmaStartAddress (uint16_t val)
{
    uint8_t tmp = 0;

    tmp = val & 0xff;
    SDIO_WriteReg (SDIO_DMA_SADDR_L, tmp);
    tmp = (val >> 8) & 0xff;

    SDIO_WriteReg (SDIO_DMA_SADDR_H, tmp);
}
/**
 * @brief  setting sd card DMA bufeer length
 * @param    val   :DMA length
 * @return  None
 */
void SDIO_SetDmaBufferLength (uint16_t val)
{
    uint8_t tmp = 0;

    tmp = val & 0xff;
    SDIO_WriteReg (SDIO_DMA_LEN_L, tmp);
    tmp = (val >> 8) & 0xff;

    SDIO_WriteReg (SDIO_DMA_LEN_H, tmp);
}

// dma_hw_start
/**
 * @brief  :set DMA write start
 * @param    cmd
 * @return  None
 */
void SDIO_SetWtiteDmaStart (FunctionalState cmd)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_DMA_CTRL);
    tmp = tmp & 0xfe;

    tmp |= cmd;
    SDIO_WriteReg (SDIO_DMA_CTRL, tmp);
}

/**
 * @brief  :Whether to select MRAM
 * @param    cmd:0/1
 * @return  None
 */
void SDIO_SetSelectMram (FunctionalState cmd)
{
    uint8_t tmp = 0;

    tmp = SDIO_ReadReg(SDIO_DMA_CTRL);
    tmp = tmp & 0xef; // clear bit[4]
    tmp = tmp | cmd;

    SDIO_WriteReg (SDIO_DMA_CTRL, tmp);
}

/**
 * @brief  :Writes data to the registers of the SD card
 * @param    addr :reg address
 * @param    val :write data
 * @return  None
 */
void SDIO_WriteReg(uint8_t addr, uint8_t val)
{
    HWRITE(CORE_ADDA_COEF_WDATA_0, addr);
    HWRITE(CORE_ADDA_COEF_WDATA_1, val);
    HWRITE(CORE_SD_REG_WR, 1);
}
/**
 * @brief  Writes data to the registers of the SD card
 * @param    addr :red address
 * @return
 */
uint8_t SDIO_ReadReg (uint8_t addr)
{
    HWRITE(CORE_ADDA_COEF_WDATA_0, addr);
    return HREAD(CORE_SD_REG_RDATA);
}




