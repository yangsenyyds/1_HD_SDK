#ifndef _YC11XX_UART_H_
#define _YC11XX_UART_H_

#include "yc11xx.h"

#ifdef __cplusplus
extern "C" {
#endif
//#define UART_BAUDRATE
#define IS_UARTN_BAUDRATE(BAUDRATE_NUM) ((BAUDRATE_NUM > 0) && (BAUDRATE_NUM< 4000000))

#define UART_BAUDRATE_RAM(RAM_RegionUse,BAUDRATE_VALUE) 					((RAM_RegionUse)?(BAUDRATE_VALUE|0X8000):BAUDRATE_VALUE)
#define UART_BAUDRATE_SYS(SYSTEM_FREQUENCY,BAUDRATE_NUM) 					((SYSTEM_FREQUENCY/BAUDRATE_NUM))

#define UART_BIT_ENABLE (1<<0)
#define BAUD_USE_SETTING (1<<7)

#define UART_STATE_TX_FULL (1 << 0)

/** @defgroup UART_Stop_Bits
  * @{
  */
#define UART_StopBits_1                 (0<<3)
#define UART_StopBits_2                 (1<<3)

#define IS_UART_STOPBITS(STOPBITS)		 (((STOPBITS) == UART_StopBits_1) ||  \
							((STOPBITS) == UART_StopBits_2) )
/**
  * @}
  */

/** @defgroup UART_Parity
* @{
*/
#define UART_Parity_None                  (0<<1)
#define UART_Parity_Even                  (2<<1)
#define UART_Parity_Odd                   (3 << 1)

#define IS_UART_PARITY(PARITY)		 ( ((PARITY) == UART_Parity_Even) || ((PARITY) == UART_Parity_Odd)) || \
                                 ((PARITY) == UART_Parity_Odd))
/**
  * @}
  */

/** @defgroup UART_Mode
  * @{
  */
#define UART_Mode_Single_Line        (1<<6)
#define UART_Mode_duplex                      (0<<6)

#define IS_UART_MODE(MODE) 		(((MODE) == UART_Mode_Single_Line) ||\
				((MODE) == UART_Mode_duplex))
/**
  * @}
  */

/** @defgroup UART_Hardware_Flow_Control
  * @{
  */
#define UART_HardwareFlowControl_None       (0<<4)
#define UART_HardwareFlowControl_ENABLE       (1<<4)

#define IS_UART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == UART_HardwareFlowControl_None) || \
                               ((CONTROL) == UART_HardwareFlowControl_ENABLE))


/**
  * @brief  UART channel define
  */
typedef enum
{
    UARTA = 0,
    UARTB = 1,
} UART_TypeDef;

/**
  * @brief  UART CLK Source
  */
typedef enum
{
    UART_XTAL = 0,
    UART_DPLL = 1,
} UART_ClKDef;

#define IS_UARTAB(UARTx)         (UARTx == UARTA)||\
						(UARTx == UARTB)

//5 sram &mram select in funcntion
typedef struct
{
    uint16_t Baudrate;
    uint16_t RxSadr;
    uint16_t RxEadr;
    uint16_t RxRptr;
    uint16_t TxSadr;
    uint16_t TxEadr;
    uint16_t TxWptr;
} UartxRegDef;

#define IS_UART_BUFFER_NULL(BUFFER)        (BUFFER != 0)
#define IS_UART_LEN_ZERO(BUFFERLEN)        (BUFFERLEN != 0)

typedef struct
{
    UartxRegDef rbu;
    uint8_t cbu;
} UartxRegControlBackup;


typedef struct
{
    uint32_t UART_BaudRate;            /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (UART_InitStruct->UART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 8 * (OVR8+1)) + 0.5
                                           Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */
    uint8_t UART_RamSelect;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                             This parameter can be a value of @ref UART_Word_Length */
    uint8_t UART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

    uint8_t UART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

    uint8_t UART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */

    uint8_t UART_Mode;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Mode */

    uint8_t UART_HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */
    uint8_t UART_ClkSource;			/*!< select the clock used by the uart is DPLL or XTAL */

    uint16_t UART_TXLen; /*!< Specifies Tx DMA buff len */

    uint16_t UART_RXLen; /*!< Specifies Rx DMA buff len */

    uint32_t UART_TxBuffer; /*!Tx ring buffer start addr */

    uint32_t UART_RxBuffer; /*!Rx ring buffer start addr */
} UART_InitTypeDef;


void UART_DeInit(UART_TypeDef uartx);
void UART_Init(UART_TypeDef uartx, UART_InitTypeDef *uart_init_struct);
void UART_SendData(UART_TypeDef uartx, uint8_t data);
uint8_t UART_ReceiveData(UART_TypeDef uartx);
uint16_t UART_ReadDatatoBuff(UART_TypeDef uartx, uint8_t *rx_buff, uint16_t rx_size);
void UART_SendDataFromBuff(UART_TypeDef uartx, uint8_t *tx_buff, uint16_t tx_len);
void UART_SendDataFromBuffFast(UART_TypeDef uartx, uint8_t *tx_buff, uint16_t tx_len);
uint16_t UART_GetRxCount(UART_TypeDef uartx);
uint16_t UART_GetTxCount(UART_TypeDef uartx);
void UART_SetRxITNum(UART_TypeDef uartx, uint8_t rx_num);
void UART_SetRxTimeout(UART_TypeDef uartx, uint16_t out_time);


#ifdef __cplusplus
}
#endif

#endif /* _YC11XX_UART_H_ */












