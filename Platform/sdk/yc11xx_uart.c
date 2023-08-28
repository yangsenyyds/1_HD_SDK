/**
***************************************************************************
* @file:  yc11xx_uart.c
* @Date:  2021-6-4-20:14:05
* @brief: UART driver
*
* Change Logs:
* Date           Author      Version        Notes
* 2021-03-08     zhouduo     V1.0.0      the first version
* 2021-06-15     zhouduo     V1.1.0      the formal version
**************************************************************************
*/
#include "yc11xx_uart.h"
#include "reg_addr.h"
#include "reg_struct.h"


/**
  * @brief  Deinit the UARTx peripheral registers
  *         to their default reset values.
  * @param  UARTx : Select the UART peripheral.UARTA,UARTB
  * @return None
  */
void UART_DeInit(UART_TypeDef uartx)
{
	_ASSERT(IS_UARTAB(uartx));
    UART_CONTROLRegDef uart_ctrl;
    UartxRegDef *uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));
    HREAD_STRUCT(uart_addr, &uart_ctrl);
    uart_ctrl.uart_en = 0;
    HWRITE_STRUCT(uart_addr, &uart_ctrl);
}

/**
  * @brief  Initializes the UARTx peripheral according to the specified
  *         parameters in the uartInitStruct.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @param  uartInitStruct : pointer to a UART_InitTypeDef structure
  *         that contains the configuration information for the specified UART
  *         peripheral.
  * @return None
  */
void UART_Init(UART_TypeDef uartx, UART_InitTypeDef *uart_init_struct)
{
	_ASSERT(IS_UARTAB(uartx));
    _ASSERT(IS_UART_BUFFER_NULL(uart_init_struct->UART_RxBuffer));
    _ASSERT(IS_UART_BUFFER_NULL(uart_init_struct->UART_TxBuffer));
    uint8_t ctr_value = 0;
    UART_CONTROLRegDef uart_ctrl;
    CLOCK_CTRLRegDef clock_slect;
    uint16_t uart_txctrl = CORE_UART_CONTROL_BASE(uartx);
    UartxRegDef *uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));

	//#ifdef
    HREAD_STRUCT(CORE_CLOCK_CTRL, &clock_slect);//TODO:ifdef
    clock_slect.uart_clock_sel = uart_init_struct->UART_ClkSource;
    HWRITE_STRUCT(CORE_CLOCK_CTRL, &clock_slect);
    CoreReg_ClkControl(REG_CLOCK_OFF_UART, ENABLE);

    ctr_value =  uart_init_struct->UART_Mode | \
					uart_init_struct->UART_HardwareFlowControl | \
					uart_init_struct->UART_Parity | \
					uart_init_struct->UART_StopBits | \
					BAUD_USE_SETTING | \
					UART_BIT_ENABLE;
    if(uart_init_struct->UART_ClkSource)
    {
        HWRITEW((uint32_t)(&uart_addr->Baudrate),((UART_BAUDRATE_SYS(SYSCLK_DPLL_48MHZ*MHz,uart_init_struct->UART_BaudRate))|((uart_init_struct->UART_RamSelect) << 15)));
    }else
    {
        HWRITEW((uint32_t)(&uart_addr->Baudrate),((UART_BAUDRATE_SYS(SYSCLK_XTAL_24MHZ*MHz,uart_init_struct->UART_BaudRate))|((uart_init_struct->UART_RamSelect) << 15)));
    }
    HWRITEW((uint32_t)(&uart_addr->RxSadr), uart_init_struct->UART_RxBuffer);
    HWRITEW((uint32_t)(&uart_addr->RxEadr), uart_init_struct->UART_RxBuffer + uart_init_struct->UART_RXLen - 1);
    HWRITEW((uint32_t)(&uart_addr->RxRptr), uart_init_struct->UART_RxBuffer);
    HWRITEW((uint32_t)(&uart_addr->TxSadr), uart_init_struct->UART_TxBuffer);
    HWRITEW((uint32_t)(&uart_addr->TxEadr), uart_init_struct->UART_TxBuffer + uart_init_struct->UART_TXLen - 1);
    HWRITEW((uint32_t)(&uart_addr->TxWptr), uart_init_struct->UART_TxBuffer);

    uart_ctrl.uart_en = 0;
    HWRITE_STRUCT(uart_txctrl, &uart_ctrl);

    HWRITE(uart_txctrl, ctr_value);
}


/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @param  Data : the data to transmit.
  * @return None
  */
void UART_SendData(UART_TypeDef uartx, uint8_t data)
{
	_ASSERT(IS_UARTAB(uartx));

    UartxRegDef *uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));
    uint16_t  wptr = HREADW((uint32_t)(&uart_addr->TxWptr));

    while(HREAD(CORE_UART_STATE_BASE(uartx)) & UART_STATE_TX_FULL );
    if(reg_check_uart_ram(HREADW(CORE_UART_BAUD_BASE(uartx))))
    {
        HWRITE(reg_map_m0(wptr), data); //send from mram
    }
    else
    {
        HWRITE(reg_map(wptr),data);//send from sram
    }
    RB_UPDATE_PTR(wptr, HREADW((uint32_t)(&uart_addr->TxSadr)), HREADW((uint32_t)(&uart_addr->TxEadr)));
    HWRITEW((uint32_t)(&uart_addr->TxWptr), (uint32_t)(wptr));// ring back
}

/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @return The received data.
  */
uint8_t UART_ReceiveData(UART_TypeDef uartx)
{
	_ASSERT(IS_UARTAB(uartx));
    uint8_t rec_data = 0;
    UartxRegDef *uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));
    uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));
    uint16_t  rptr = HREADW((uint32_t)(&uart_addr->RxRptr));
    if(reg_check_uart_ram(HREADW(CORE_UART_BAUD_BASE(uartx))))
    {
    	rec_data = HREADW(reg_map_m0(rptr));//receive from mram
    }
    else
    {
    	rec_data = HREADW(reg_map(rptr));//receive from sram
    }
	RB_UPDATE_PTR(rptr, HREADW((uint32_t)(&uart_addr->RxSadr)), HREADW((uint32_t)(&uart_addr->RxEadr)));
	HWRITEW((uint32_t)(&uart_addr->RxRptr), rptr);
    return rec_data;
}

/**
  * @brief  Saves the received data to a buff
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @param	RxBuff : The buffer where data receive to.
  * @param	RxSize : The length of receive data.
  * @return None
  */
uint16_t UART_ReadDatatoBuff(UART_TypeDef uartx, uint8_t *rx_buff, uint16_t rx_size)
{
	_ASSERT(IS_UARTAB(uartx));

    uint32_t uart_rxitems = CORE_UART_RxItems_BASE(uartx);
    uint16_t rx_len = HREADW(reg_map(uart_rxitems));
    uint16_t rptr = 0;
    uint16_t rec_len = 0;
    UartxRegDef *uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));
	uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));

    if (rx_size != 0)
    {
        if (rx_len < rx_size) return 0;
        else rx_len = rx_size;
    }
    if (0 == rx_len)
    {
        return 0;
    }
    else
    {
        rptr = HREADW((uint32_t)(&uart_addr->RxRptr));
        for(rec_len = 0; rec_len < rx_len; rec_len++ )
        {
        	rx_buff[rec_len] = HREAD(reg_map_m0(rptr));
            RB_UPDATE_PTR(rptr, HREADW((uint32_t)(&uart_addr->RxSadr)), HREADW((uint32_t)(&uart_addr->RxEadr)));
        }
    }
    HWRITEW((uint32_t)(&uart_addr->RxRptr), (rptr));
    return rec_len;
}

/**
  * @brief  Transmits specified length data through the UARTx peripheral.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @param  TxBuff : The buffer where data transmit from.
  * @param	TxLen : The specified length to transmit.
  * @return None
  */
void UART_SendDataFromBuff(UART_TypeDef uartx, uint8_t *tx_buff, uint16_t tx_len)
{
	_ASSERT(IS_UARTAB(uartx));

    while(tx_len--)
    {
        UART_SendData(uartx, *tx_buff++);
    }
}

/**
  * @brief  Transmits specified length data through the UARTx peripheral.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @param  TxBuff : The buffer where data transmit from.
  * @param	TxLen : The specified length to transmit.
  * @return None
  */
void UART_SendDataFromBuffFast(UART_TypeDef uartx, uint8_t *tx_buff, uint16_t tx_len)
{
	_ASSERT(IS_UARTAB(uartx));

    UartxRegDef *uart_addr = (UartxRegDef *)(reg_map(CORE_UART_BAUD_BASE(uartx)));
    uint16_t wptr = HREADW((uint32_t)(&uart_addr->TxWptr));
    uint16_t ram_flag = reg_check_uart_ram(HREADW(CORE_UART_BAUD_BASE(uartx)));
	for(int i = 0;i<tx_len;i++)
	{
		while(HREAD(CORE_UART_STATE_BASE(uartx))& UART_STATE_TX_FULL);
		HWRITE(UART_RAM_SELECT(ram_flag,wptr), tx_buff[i]);
		RB_UPDATE_PTR(wptr, HREADW((uint32_t)(&uart_addr->TxSadr)), HREADW((uint32_t)(&uart_addr->TxEadr)));
	}
    HWRITEW((uint32_t)(&uart_addr->TxWptr), (uint32_t)(wptr)); // ring back
}

/**
  * @brief  Get the length of RX DMA.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @return None.
  */
uint16_t UART_GetRxCount(UART_TypeDef uartx)
{
	_ASSERT(IS_UARTAB(uartx));
    return HREADW(reg_map(CORE_UART_RxItems_BASE(uartx)));
}


/**
  * @brief  Get the length of TX DMA.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @return None.
  */
uint16_t UART_GetTxCount(UART_TypeDef uartx)
{
	_ASSERT(IS_UARTAB(uartx));
    return HREADW(reg_map(CORE_UART_TxItems_BASE(uartx)));
}


/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx : Select the UART peripheral. UARTA,UARTB
  * @param  Num : interrupt Number of data.
  * @return The received data.
  */
void UART_SetRxITNum(UART_TypeDef uartx, uint8_t rx_num)
{
	_ASSERT(IS_UARTAB(uartx));
	HWRITE(reg_map(CORE_UART_CTRL_BASE(uartx)), rx_num);
}

/**
  * @brief  Sets the RX interrupt trigger timeout.
  * @param  time : timeout of trigger interrupt.
  * @return None.
  */
void UART_SetRxTimeout(UART_TypeDef uartx, uint16_t out_time)
{
	_ASSERT(IS_UARTAB(uartx));
	HWRITE(reg_map(CORE_UART_CTRL_BASE(uartx) + 1), out_time);
}

