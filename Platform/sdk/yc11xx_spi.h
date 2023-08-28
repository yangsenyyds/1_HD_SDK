/**
* @file yc11xx_spi.h
* @author duanchu.he
* @brief SPI driver
* @version 0.1
* @date 2021-07-06
*
*
*/

#ifndef __YC11XX_SPI_H
#define __YC11XX_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "yc11xx.h"

#define SPI_TxRx_Sequence_Concurrent      (0)
#define SPI_TxRx_Sequence_Tx_First        (1)
#define Is_SPI_TxRx_Sequence(SEQUENCE)    (((SEQUENCE) == SPI_TxRx_Sequence_Concurrent) ||\
                                             ((SEQUENCE)  == SPI_TxRx_Sequence_Tx_First))

#define SPI_CPHA_First_Edge               (0)
#define SPI_CPHA_Second_Edge              (1)
#define Is_SPI_CPHA(EDGE)                 (((EDGE) == SPI_CPHA_First_Edge) ||\
                                             ((EDGE)  == SPI_CPHA_Second_Edge))

#define SPI_CPOL_Low                      (0)
#define SPI_CPOL_High                     (1)
#define Is_SPI_CPOL(PHASE)                (((PHASE) == SPI_CPOL_Low) ||\
                                             ((PHASE)  == SPI_CPOL_High))

#define SPI_Hw_Start_Sel_DMA0             (0<<2)
#define SPI_Hw_Start_Sel_DMA1             (1<<2)
#define Is_SPI_Hw_Start_Sel(SELECT)       (((SELECT) == SPI_Hw_Start_Sel_DMA0) ||\
                                             ((SELECT)  == SPI_Hw_Start_Sel_DMA1))

#define SPI_ClkDiv_0						0
#define SPI_ClkDiv_1						1
#define SPI_ClkDiv_2						2
#define SPI_ClkDiv_3						3
#define SPI_ClkDiv_4						4
#define SPI_ClkDiv_5						5
#define SPI_ClkDiv_6						6
#define SPI_ClkDiv_7						7
#define SPI_ClkDiv_8						8
#define SPI_ClkDiv_9						9
#define SPI_ClkDiv_10						10
#define SPI_ClkDiv_11						11
#define SPI_ClkDiv_12						12
#define SPI_ClkDiv_13						13
#define SPI_ClkDiv_14						14
#define SPI_ClkDiv_15						15


/**
 * @brief SPI_IRQ_MIS
  *				  Start_Over_Quick_RIS: Start over quick raw interrupt status
  * 			  RxFifo_Overflow_RIS: Rx fifo overflow raw interrupt status
  * 			  RxFifo_High_RIS: Rx fifo high raw interrupt status
  * 			  TxFifo_Overflow_RIS: Tx fifo overflow raw interrupt status
  * 			  TxFifo_Low_RIS: Tx fifo low raw interrupt status
  * 			  DMA_Done_RIS: DMA done raw interrupt status
 */
typedef enum
{
    DMA_Done_MIS = 0,
    TxFifo_Low_MIS,
    TxFifo_Overflow_MIS,
    RxFifo_High_MIS,
    RxFifo_Overflow_MIS,
    Start_Over_Quick_MIS,
} SPI_IRQ_MIS;

/**
 * @brief SPI_IRQ_RIS
  *			      Start_Over_Quick_MIS: Start over quick mask interrupt status
  * 			  RxFifo_Overflow_MIS: Rx fifo overflow mask interrupt status
  * 			  RxFifo_High_MIS: Rx fifo high mask interrupt status
  * 			  TxFifo_Overflow_MIS: Tx fifo overflow mask interrupt status
  * 			  TxFifo_Low_MIS: Tx fifo low mask interrupt status
  * 			  DMA_Done_MIS: DMA done mask interrupt status
 */
typedef enum
{
    DMA_Done_RIS = 0,
    TxFifo_Low_RIS,
    TxFifo_Overflow_RIS,
    RxFifo_High_RIS,
    RxFifo_Overflow_RIS,
    Start_Over_Quick_RIS,
} SPI_IRQ_RIS;



typedef struct
{
    uint8_t RxAdjClk;                     /*!< Specifies the received phase adjustment value,
                                                 the sampling time is delayed (RxAdjClk + 1)*spi_clk clock cycles,
                                                 shall not be greater than ClkDiv value.
 												This parameter can be a value between 0x00 and 0x0F */

    uint8_t ClkDiv;                       /*!< Specifies the clock frequency division factor, division value is (ClkDiv + 1)*2.
 												This parameter can be a value between 0x00 and 0x0F */

    uint8_t TxRxDly;                      /*!< Specifies the send and receive protection interval, interval value is spi_clk*(TxRxDly + 1)*4.
 												This parameter can be a value between 0x00 and 0x0F */

    uint8_t NcsDly;                        /*!< Specifies the delay time of NCS,delay value is spi_clk*(NcsDly + 1).
 												This parameter can be a value between 0x00 and 0x07 */

    uint16_t RxAdjEn;                       /*!< Specifies the Receive phase adjustment enable switch.
 												This parameter can be a value between 0x00 and 0x01 */

    uint16_t TxRxSeq;                       /*!< Specifies the tx and rx sequence control:
 												This parameter can be a value of @ref SPI_TxRx_Sequence */

    uint16_t Cpha;                          /*!< Specifies the clock active edge for the bit capture.
                                                 This parameter can be a value of @ref SPI_Clock_Phase */

    uint16_t Cpol;                          /*!< Specifies the serial clock steady state.
                                                 This parameter can be a value of @ref SPI_Clock_Polarity */

    uint16_t SpiHwStartSel;                 /*!< Specifies the DCMI DMA select of lines irq.
                                                 This parameter can be a value of @ref SPI_Hw_Start_Sel */

    uint8_t RxFifoWaterlevel;              /*!< Specifies the rx fifo waterlevel.
 												This parameter can be a value between 0x00 and 0x07 */

    uint8_t TxFifoWaterlevel;              /*!< Specifies the tx fifo waterlevel.
 												This parameter can be a value between 0x00 and 0x07 */

    uint8_t		FifoSwEn;						/*!< Specifies the fifo_sw_en. */
    uint16_t	FifoTxLen;						/*!< Specifies the fifo_tx_len. */
    uint16_t	FifoRxLen;						/*!< Specifies the fifo_rx_len. */
} SPI_InitTypeDef;

void SPI_Init(SPI_InitTypeDef *spi_initstruct);
void SPI_HwStartCmd(FunctionalState new_state);
void SPI_EnableCmd(FunctionalState new_state);
void SPI_PingpangEnableCmd(FunctionalState new_state);
void SPI_Start(FunctionalState new_state);
void SPI_DmaEnableCmd(FunctionalState new_state);
void SPI_DmaAutoStartCmd(FunctionalState new_state);
void SPI_FifoAutoStartCmd(FunctionalState new_state);
void SPI_WriteTxFifoData(uint8_t tx_data);
void SPI_WriteRxFifoData(uint8_t rx_data);
uint8_t SPI_ReadRxFifoData(void);
void SPI_ReduceOneByteRxFifo(void);
void SPI_ReduceOneByteTxFifo(void);
void SPI_ITConfig(SPI_IRQ_MIS irq_mis, FunctionalState new_state);
void SPI_ClearFlagStatus(SPI_IRQ_RIS irq_ris, FunctionalState new_state);
void SPI_ClearITStatus(SPI_IRQ_MIS irq_mis, FunctionalState new_state);
uint8_t SPI_GetITStatus(SPI_IRQ_MIS irq_mis);
uint8_t SPI_GetFlagStatus(SPI_IRQ_RIS irq_ris);
void SPI_SendData(uint8_t data);
void SPI_SendBuff(uint8_t *tx_buff, uint32_t tx_len);
void SPI_SendAndReceiveData(uint8_t *tx_buff, uint32_t tx_len, uint8_t *rx_buff, uint32_t rx_len);

#endif
