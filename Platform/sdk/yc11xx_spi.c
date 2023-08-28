/**
* @file yc11xx_spi.c
* @author duanchu.he
* @brief SPI driver
* @version 0.1
* @date 2021-07-06
*
*
*/
#include "yc11xx_spi.h"
#include "reg_addr.h"
#include "reg_struct.h"

/**
 * @brief Initializes the SPI according to the specified parameters in the SPI_InitStruct.
 *
 * @param SPI_InitStruct pointer to a SPI_InitTypeDef structure that contains
  *         the configuration information for the SPI.
 */
void SPI_Init(SPI_InitTypeDef *spi_initstruct)
{
    _ASSERT(Is_SPI_TxRx_Sequence(spi_initstruct->TxRxSeq));
    _ASSERT(Is_SPI_CPHA(spi_initstruct->Cpha));
    _ASSERT(Is_SPI_CPOL(spi_initstruct->Cpol));
    _ASSERT(Is_SPI_Hw_Start_Sel(spi_initstruct->SpiHwStartSel));

    SPI_CTRLRegDef	spictrl_handle;
    SPI_FIFO_CTRLRegDef	spififoCtrl_handle;

    CoreReg_ClkControl(REG_CLOCK_OFF_SPI, ENABLE);

    HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);

    spictrl_handle.clk_div = spi_initstruct->ClkDiv;
    spictrl_handle.cpha = spi_initstruct->Cpha;
    spictrl_handle.cpol = spi_initstruct->Cpol;
    spictrl_handle.fifo_sw_en = spi_initstruct->FifoSwEn;
    spictrl_handle.ncs_dly = spi_initstruct->NcsDly;
    spictrl_handle.rx_adj_clk = spi_initstruct->RxAdjClk;
    spictrl_handle.rx_adj_en = spi_initstruct->RxAdjEn;
    spictrl_handle.trx_dly = spi_initstruct->TxRxDly;
    spictrl_handle.tx_rx_seq = spi_initstruct->TxRxSeq;
    spictrl_handle.spi_hw_start_sel = spi_initstruct->SpiHwStartSel;

    HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);

    spififoCtrl_handle.rx_fifo_waterlevel = spi_initstruct->RxFifoWaterlevel;
    spififoCtrl_handle.tx_fifo_waterlevel = spi_initstruct->TxFifoWaterlevel;

    HWRITEL_STRUCT(CORE_SPI_FIFO_CTRL, &spififoCtrl_handle);
}


/**
 * @brief Enables or disables the HPI hardware start.
 *
 * @param new_state DISABLE or ENABLE
 */
void SPI_HwStartCmd(FunctionalState new_state)
{
	_ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_CTRLRegDef	spictrl_handle;

    HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);

    spictrl_handle.spi_hw_start_en = new_state;

    HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
}

/**
 * @brief Enables or disables the SPI interface.
 *
 * @param new_state new state of the SPI interface.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_EnableCmd(FunctionalState new_state)
{
	_ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_CTRLRegDef	spictrl_handle;

    HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);

    spictrl_handle.spi_en = new_state;

    HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
}

/**
 * @brief Enables or disables the SPI DMA ping-pang display.
 *
 * @param new_state new state of the SPI DMA ping-pang display.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_PingpangEnableCmd(FunctionalState new_state)
{
	_ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_DMARegDef	spidma_handle;

    HREADL_STRUCT(CORE_SPI_DMA, &spidma_handle);

    spidma_handle.dma_pingpang_en = new_state;

    HWRITEL_STRUCT(CORE_SPI_DMA, &spidma_handle);
}

/**
 * @brief Enables or disables the SPI .
 *
 * @param new_state new state of the SPI.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_Start(FunctionalState new_state)
{
	_ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_CTRLRegDef	spictrl_handle;

    HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);

    spictrl_handle.spi_start = new_state;

    HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
}

/**
 * @brief Enables or disables the SPI DMA.
 *
 * @param new_state new state of the SPI DMA.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_DmaEnableCmd(FunctionalState new_state)
{
	 _ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_DMARegDef	spidma_handle;

    HREADL_STRUCT(CORE_SPI_DMA, &spidma_handle);

    spidma_handle.dma_en = new_state;

    HWRITEL_STRUCT(CORE_SPI_DMA, &spidma_handle);
}

/**
 * @brief Enables or disables the SPI DMA auto start.
 *
 * @param new_state new state of the SPI DMA auto start.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_DmaAutoStartCmd(FunctionalState new_state)
{
	_ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_DMARegDef	spidma_handle;

    HREADL_STRUCT(CORE_SPI_DMA, &spidma_handle);

    spidma_handle.dma_start_auto = new_state;

    HWRITEL_STRUCT(CORE_SPI_DMA, &spidma_handle);
}

/**
 * @brief Enables or disables the SPI FIFO auto start.
 *
 * @param new_state new state of the SPI FIFO auto start.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_FifoAutoStartCmd(FunctionalState new_state)
{
	_ASSERT(IS_FUNCTIONAL_STATE(new_state));

    SPI_FIFORegDef	spififo_handle;

    HREADL_STRUCT(CORE_SPI_FIFO, &spififo_handle);

    spififo_handle.fifo_auto_rst = new_state;

    HWRITEL_STRUCT(CORE_SPI_FIFO, &spififo_handle);
}

/**
 * @brief Writes data to the FIFO of TX
 *
 * @param tx_data To write txdata
 */

void SPI_WriteTxFifoData(uint8_t tx_data)
{
    SPI_FIFOTDATARegDef	spififodata_handle;

    spififodata_handle.tx_fifo_data = tx_data;

    HWRITE_STRUCT(CORE_SPI_FIFOTDATA, &spififodata_handle);
}

/**
 * @brief Writes data to the FIFO of TX
 *
 * @param tx_data To write txdata
 */

void SPI_WriteRxFifoData(uint8_t rx_data)
{
    SPI_FIFORDATARegDef	spififodata_handle;

    spififodata_handle.rx_fifo_data = rx_data;

    HWRITE_STRUCT(CORE_SPI_FIFORDATA, &spififodata_handle);
}


/**
 * @brief READ data to the FIFO of RX
 *
 * @param rx_data To read rxdata
 */

uint8_t SPI_ReadRxFifoData(void)
{
    SPI_FIFORDATARegDef	spififodata_handle;

    HREAD_STRUCT(CORE_SPI_FIFORDATA, &spififodata_handle);

    return spififodata_handle.rx_fifo_data;
}
/**
 * @brief Decrease the RXFIFO depth by one
 *
 */
void SPI_ReduceOneByteRxFifo(void)
{
    SPI_FIFORegDef	spififo_handle;

    HREADW_STRUCT(CORE_SPI_FIFO, &spififo_handle);

    spififo_handle.rx_fifo_rd = 1;

    HWRITEW_STRUCT(CORE_SPI_FIFO, &spififo_handle);
}

void SPI_ReduceOneByteTxFifo(void)
{
    SPI_FIFORegDef	spififo_handle;

    HREADW_STRUCT(CORE_SPI_FIFO, &spififo_handle);

    spififo_handle.tx_fifo_rd = 1;

    HWRITEW_STRUCT(CORE_SPI_FIFO, &spififo_handle);
}

/**
 * @brief Enables or disables the SPI interface interrupts.
 *
 * @param irq_mis irq_mis interrupt type
 * @param new_state new state of the specified SPI interrupts.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_ITConfig(SPI_IRQ_MIS irq_mis, FunctionalState new_state)
{
    SPI_IRQMENRegDef	spiirq_handle = {0};

    switch(irq_mis)
    {
    case DMA_Done_MIS:
        spiirq_handle.dma_done_en = new_state;
        break;
    case TxFifo_Low_MIS:
        spiirq_handle.tx_fifo_lo_en = new_state;
        break;
    case TxFifo_Overflow_MIS:
        spiirq_handle.tx_fifo_ovfl_en = new_state;
        break;
    case RxFifo_High_MIS:
        spiirq_handle.rx_fifo_hi_en = new_state;
        break;
    case RxFifo_Overflow_MIS:
        spiirq_handle.rx_fifo_ovfl_en = new_state;
        break;
    case Start_Over_Quick_MIS:
        spiirq_handle.start_over_quick_en = new_state;
        break;
    }

    HWRITE_STRUCT(CORE_SPI_IRQMEN, &spiirq_handle);
}

/**
 * @brief Clears the SPI's interrupt pending bits.
 *
 * @param irq_ris irq_ris interrupt type
 * @param new_state new state of the specified SPI interrupts.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_ClearFlagStatus(SPI_IRQ_RIS irq_ris, FunctionalState new_state)
{
    SPI_IRQRSRegDef	spiirq_handle = {0};

    switch(irq_ris)
    {
    case DMA_Done_RIS:
        spiirq_handle.dma_done_ris = new_state;
        break;
    case TxFifo_Low_RIS:
        spiirq_handle.tx_fifo_lo_ris = new_state;
        break;
    case TxFifo_Overflow_RIS:
        spiirq_handle.tx_fifo_ovfl_ris = new_state;
        break;
    case RxFifo_High_RIS:
        spiirq_handle.rx_fifo_hi_ris = new_state;
        break;
    case RxFifo_Overflow_RIS:
        spiirq_handle.rx_fifo_ovfl_ris = new_state;
        break;
    case Start_Over_Quick_RIS:
        spiirq_handle.start_over_quick_ris = new_state;
        break;
    }

    HWRITE_STRUCT(CORE_SPI_IRQRS, &spiirq_handle);
}

/**
 * @brief Clears the SPI's interrupt pending bits.
 *
 * @param irq_mis irq_mis interrupt type
 * @param new_state new state of the specified SPI interrupts.
  *         This parameter can be: ENABLE or DISABLE.
 */
void SPI_ClearITStatus(SPI_IRQ_MIS irq_mis, FunctionalState new_state)
{
    SPI_IRQMSRegDef	spiirq_handle;

    switch(irq_mis)
    {
    case DMA_Done_MIS:
        spiirq_handle.dma_done_mis = new_state;
        break;
    case TxFifo_Low_MIS:
        spiirq_handle.tx_fifo_lo_mis = new_state;
        break;
    case TxFifo_Overflow_MIS:
        spiirq_handle.tx_fifo_ovfl_mis = new_state;
        break;
    case RxFifo_High_MIS:
        spiirq_handle.rx_fifo_hi_mis = new_state;
        break;
    case RxFifo_Overflow_MIS:
        spiirq_handle.rx_fifo_ovfl_mis = new_state;
        break;
    case Start_Over_Quick_MIS:
        spiirq_handle.start_over_quick_mis = new_state;
        break;
    }

    HWRITE_STRUCT(CORE_SPI_IRQMS, &spiirq_handle);
}

/**
 * @brief Checks whether the SPI interface  is set or not.
 *
 * @param irq_mis irq_mis interrupt type
 * @return FlagStatus SET or RESET
 */
uint8_t SPI_GetITStatus(SPI_IRQ_MIS irq_mis)
{
    SPI_IRQMSRegDef	spiirq_handle;
    uint8_t bit_status = 0;

    HREAD_STRUCT(CORE_SPI_IRQMS, &spiirq_handle);

    switch(irq_mis)
    {
    case DMA_Done_MIS:
        bit_status = spiirq_handle.dma_done_mis;
        break;
    case TxFifo_Low_MIS:
        bit_status = spiirq_handle.tx_fifo_lo_mis;
        break;
    case TxFifo_Overflow_MIS:
        bit_status = spiirq_handle.tx_fifo_ovfl_mis;
        break;
    case RxFifo_High_MIS:
        bit_status = spiirq_handle.rx_fifo_hi_mis;
        break;
    case RxFifo_Overflow_MIS:
        bit_status = spiirq_handle.rx_fifo_ovfl_mis;
        break;
    case Start_Over_Quick_MIS:
        bit_status = spiirq_handle.start_over_quick_mis;
        break;
    }

    return  bit_status;
}

/**
 * @brief Checks whether the SPI  flag is set or not.
 *
 * @param irq_ris irq_ris interrupt type
 * @return FlagStatus SET or RESET
 */
uint8_t SPI_GetFlagStatus(SPI_IRQ_RIS irq_ris)
{
    SPI_IRQRSRegDef	spiirq_handle;
    uint8_t bit_status = 0;

    HREAD_STRUCT(CORE_SPI_IRQRS, &spiirq_handle);

    switch(irq_ris)
    {
    case DMA_Done_RIS:
        bit_status = spiirq_handle.dma_done_ris;
        break;
    case TxFifo_Low_RIS:
        bit_status = spiirq_handle.tx_fifo_lo_ris;
        break;
    case TxFifo_Overflow_RIS:
        bit_status = spiirq_handle.tx_fifo_ovfl_ris;
        break;
    case RxFifo_High_RIS:
        bit_status = spiirq_handle.rx_fifo_hi_ris;
        break;
    case RxFifo_Overflow_RIS:
        bit_status = spiirq_handle.rx_fifo_ovfl_ris;
        break;
    case Start_Over_Quick_RIS:
        bit_status = spiirq_handle.start_over_quick_ris;
        break;
    }

    return  bit_status;
}

/**
 * @brief Transmits one data via SPI DMA.
 *
 * @param data the data you want transmit.
 */
void SPI_SendData(uint8_t data)
{
    SPI_SendAndReceiveData(&data, 1, 0, 0);
}

/**
 * @brief Transmits datas via SPI DMA.
 *
 * @param txbuff pointer to a buf that contains the data you want send.
 * @param txlen the buf length
 */
void SPI_SendBuff(uint8_t *tx_buff, uint32_t tx_len)
{
    SPI_SendAndReceiveData(tx_buff, tx_len, 0, 0);
}

/**
 * @brief Send and recerive data.
 *
 * @param txbuff pointer to a txbuff  that contains the data you want send.
 * @param txlen the length of send datas
 * @param rxbuff pointer to a txbuff  that contains the data you want receive.
 * @param rxlen the length of receive datas
 */
void SPI_SendAndReceiveData(uint8_t *tx_buff, uint32_t tx_len, uint8_t *rx_buff, uint32_t rx_len)
{
    SPI_CTRLRegDef	spictrl_handle;
    SPI_DMA_TX_LENRegDef	spidmatx_handle;
    SPI_DMA_RX_LENRegDef	spidmarx_handle;
    SPI_DMA_TX_SADDRRegDef	spidmatxsaddr_handle;
    SPI_DMA_RX_SADDRRegDef	spidmarxsaddr_handle;

    if(reg_check_ram_m0(tx_buff) || reg_check_ram_m0(rx_buff))
    {
        HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
        spictrl_handle.mram_sel = 1;
        HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
    }
    else
    {
        HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
        spictrl_handle.mram_sel = 0;
        HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
    }

    spidmatxsaddr_handle.tx_start_addr = (uint32_t)tx_buff

    		;
    HWRITEL_STRUCT(CORE_SPI_DMA_TX_SADDR, &spidmatxsaddr_handle);

    spidmarxsaddr_handle.rx_start_addr = (uint32_t)rx_buff;
    HWRITEL_STRUCT(CORE_SPI_DMA_RX_SADDR, &spidmarxsaddr_handle);

    spidmatx_handle.tx_len = tx_len;
    HWRITEL_STRUCT(CORE_SPI_DMA_TX_LEN, &spidmatx_handle);

    spidmarx_handle.rx_len = rx_len;
    HWRITEL_STRUCT(CORE_SPI_DMA_RX_LEN, &spidmarx_handle);

    HREADL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);
    spictrl_handle.spi_start = 1;
    HWRITEL_STRUCT(CORE_SPI_CTRL, &spictrl_handle);

    while (!(SPI_GetFlagStatus(DMA_Done_RIS)));
    SPI_ClearFlagStatus(DMA_Done_RIS, ENABLE);
}



