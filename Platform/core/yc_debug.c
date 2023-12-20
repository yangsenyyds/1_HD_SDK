#include "yc_debug.h"
#include "reg_addr.h"
#include "yc11xx_gpio.h"
#include "btreg.h"


#ifdef FUNCTION_CONTROL_DEBUG_ENABLE
MEMORY_NOT_PROTECT_UNDER_LPM_ATT unsigned char uarttxbuf[DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH], uartrxbuf[2];
unsigned char *txptr;

void uart_txBuffer(uint8_t *send_buf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++)
    {
        uart_txChar(*(send_buf + i));
    }
}

void uart_txChar(unsigned char c)
{
    OS_ENTER_CRITICAL();
    while(HREAD(CORE_UART_B_TX_ITEMS) >= sizeof(uarttxbuf) - 10);
    *txptr = c;
    if(++txptr > uarttxbuf + sizeof(uarttxbuf) - 1) txptr = uarttxbuf;
    HWRITEW(CORE_UARTB_TX_WPTR, txptr);
    OS_EXIT_CRITICAL();
}

void init_debugUart(void)
{
    HWOR(CORE_CLOCK_CTRL, BIT0);
    HWCOR(CORE_CLOCK_OFF + 1, 0x80);

    //gpio config
    if(DEBUG_GPIO_DEFINE <= 31)
        HWRITE(CORE_GPIO0_CONFIG + DEBUG_GPIO_DEFINE, GPIO_Mode_Uartb_Txd);
    else
        HWRITE(CORE_GPIO_CONF1 + (DEBUG_GPIO_DEFINE - 32), GPIO_Mode_Uartb_Txd);

    HWRITEW(CORE_UART_B_BAUD_RATE, 0x8034);// 921600
    //HWRITEW(CORE_UARTB_BAUD, 0x81bf); //115200
    HWRITEW(CORE_UARTB_TX_START_ADDR, (uint16_t)(uint32_t)&uarttxbuf);//write start addr
    HWRITEW(CORE_UARTB_TX_WPTR, (uint16_t)(uint32_t)&uarttxbuf); //init write pointer
    HWRITEW(CORE_UARTB_TX_END_ADDR, (uint16_t)((int)&uarttxbuf + sizeof(uarttxbuf) - 1));//write end addr

    HWRITEW(CORE_UARTB_RX_START_ADDR, (uint16_t)(uint32_t)&uartrxbuf); //read start addr
    HWRITEW(CORE_UARTB_RX_RPTR, (uint16_t)(uint32_t)&uartrxbuf);  //init read pointer
    HWRITEW(CORE_UARTB_RX_END_ADDR, (uint16_t)((int)&uartrxbuf + sizeof(uartrxbuf) - 1)); //read end addr

    HWRITE(CORE_UART_B_CONTROL, 0x81);
    txptr = uarttxbuf;

    // Init other function
    init_print_log();
    return;
}

void debugUart_pollTx(void)
{
    // Type_Log print
    print_log_to_uart();
    return;
}

void write_long_value(uint8_t *addr, uint32_t value)
{
    *(addr) = (value >> 0);
    *(addr + 1) = (value >> 8);
    *(addr + 2) = (value >> 16);
    *(addr + 3) = (value >> 24);
}

void print_round_buffer(uint8_t index, uint32_t start_addr, uint32_t end_addr, uint32_t write_addr, uint32_t read_addr)
{
    uint8_t uart_send_buf[DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH];
    uint8_t uart_send_length = 0;
    uint8_t offset = 0;

    *(uart_send_buf + offset + DEBUG_PROTOCL_PAYLOAD_START_OFFSET) = index;
    offset = offset + 1;
    write_long_value((uart_send_buf + offset + DEBUG_PROTOCL_PAYLOAD_START_OFFSET), start_addr);
    offset = offset + 4;
    write_long_value((uart_send_buf + offset + DEBUG_PROTOCL_PAYLOAD_START_OFFSET), end_addr);
    offset = offset + 4;
    write_long_value((uart_send_buf + offset + DEBUG_PROTOCL_PAYLOAD_START_OFFSET), write_addr);
    offset = offset + 4;
    write_long_value((uart_send_buf + offset + DEBUG_PROTOCL_PAYLOAD_START_OFFSET), read_addr);
    offset = offset + 4;
    // Prepare packet
    yichip_debug_uart_protocol(DEBUG_PROTOCL_TYPE_ROUND_BUFFER, offset, uart_send_buf, &uart_send_length);

    uart_txBuffer(uart_send_buf, uart_send_length);
}

/**
  * To avoid uart lost info, we need add protocol to protect it, The input length must limit DEBUG_PROTOCL_PAYLOAD_ALLOW_LENGTH
  * To reduce timing, you should prepare payload into <uart_send_buf> in right point.
  *    1byte      1byte    1byte        nbytes         1byte
  * | 0xAA  | Type  | length  |  Payload   | 0xBB  |
  */
void yichip_debug_uart_protocol(uint8_t type, uint8_t length
                                , uint8_t *uart_send_buf, uint8_t *uart_send_length_ptr)
{
    uint16_t offset = 0;
    *(uart_send_buf + offset) = 0xAA;
    offset ++;
    *(uart_send_buf + offset) = type;
    offset ++;
    *(uart_send_buf + offset) = length;
    offset ++;
    length = length > DEBUG_PROTOCL_PAYLOAD_ALLOW_LENGTH ? DEBUG_PROTOCL_PAYLOAD_ALLOW_LENGTH : length;
    // DO NOTHING, to reduce timing
    //memcpy(uart_send_buf+offset, payload, length);
    offset += length;
    *(uart_send_buf + offset) = 0xBB;
    offset ++;
    *uart_send_length_ptr = offset;
}

// use share memory
static YC_DEBUG_MANAGER g_yc_debug_manager;

void init_print_log(void)
{
    // Set ptr
    g_yc_debug_manager.log_bug_ptr = (uint32_t *)DEBUG_LOG_BUF_START_ADDR;
    g_yc_debug_manager.log_initial_flag_ptr = (uint8_t *)DEBUG_LOG_BUF_INITIAL_FLAG_ADDR;
    g_yc_debug_manager.log_lock_flag_cm0_ptr = (uint8_t *)DEBUG_LOG_BUF_LOCK_FLAG_CM0_ADDR;
    g_yc_debug_manager.log_lock_flag_respin_ptr = (uint8_t *)DEBUG_LOG_BUF_LOCK_FLAG_RESPIN_ADDR;
    g_yc_debug_manager.log_lock_victim_ptr = (uint8_t *)DEBUG_LOG_BUF_LOCK_VICTIM_ADDR;
    g_yc_debug_manager.log_read_index_ptr = (uint16_t *)DEBUG_LOG_BUF_READ_INDEX_ADDR;
    g_yc_debug_manager.log_write_index_ptr = (uint16_t *)DEBUG_LOG_BUF_WRITE_INDEX_ADDR;


    memset((void *)g_yc_debug_manager.log_bug_ptr, 0, LOG_BUFFER_SIZE * 4);
    *(g_yc_debug_manager.log_lock_flag_cm0_ptr) = 0;
    *(g_yc_debug_manager.log_lock_flag_respin_ptr) = 0;
    *(g_yc_debug_manager.log_lock_victim_ptr) = 0;
    *(g_yc_debug_manager.log_read_index_ptr) = 0;
    *(g_yc_debug_manager.log_write_index_ptr) = 0;
    *(g_yc_debug_manager.log_initial_flag_ptr) = DEBUG_LOG_INITIAL_SUCCESS_FLAG;

}

bool print_log_buffer_is_empty(void)
{
    volatile uint16_t *read_index_ptr = g_yc_debug_manager.log_read_index_ptr;
    volatile uint16_t *write_index_ptr = g_yc_debug_manager.log_write_index_ptr;
    uint16_t write_index;
    uint16_t read_index;
    OS_ENTER_CRITICAL();
    // get write lock
    //print_log_get_lock();
    write_index = (*write_index_ptr);
    read_index = (*read_index_ptr);
    //print_log_free_lock();
    OS_EXIT_CRITICAL();

    return (write_index == read_index);
}

void print_log_to_uart(void)
{
    volatile uint16_t *read_index_ptr = g_yc_debug_manager.log_read_index_ptr;
    volatile uint16_t *write_index_ptr = g_yc_debug_manager.log_write_index_ptr;
    uint16_t write_index;
    uint16_t read_index;
    uint16_t offset = 0;
    uint8_t uart_send_buf[DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH];
    uint8_t uart_send_length = 0;

    OS_ENTER_CRITICAL();
    write_index = (*write_index_ptr);
    read_index = (*read_index_ptr);
    // To avoid protocol header lost timing
    while((write_index != read_index)
            && ((offset + 4) < DEBUG_PROTOCL_PAYLOAD_ALLOW_LENGTH))// Here offset must add 4
    {
        //memcpy((void *)(uart_send_buf + DEBUG_PROTOCL_PAYLOAD_START_OFFSET + offset)
        //	, log_bug_ptr + read_index, 4);
        xmemcpy(uart_send_buf + DEBUG_PROTOCL_PAYLOAD_START_OFFSET + offset, (uint8_t *)(g_yc_debug_manager.log_bug_ptr + read_index), 4);
        read_index = (read_index + 1) % LOG_BUFFER_SIZE;
        *read_index_ptr = read_index;
        offset += 4;
    }
    OS_EXIT_CRITICAL();

    // Real Send To Uart
    if(offset != 0)
    {
        // Prepare packet
        yichip_debug_uart_protocol(DEBUG_PROTOCL_TYPE_LOG, offset, uart_send_buf, &uart_send_length);

        uart_txBuffer(uart_send_buf, uart_send_length);
    }
}
uint16_t gHaltLogFlag;
uint16_t gHaltLogValue;
void print_log(uint16_t flag, uint16_t value)
{
    uint8_t uart_send_buf[DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH];
    uint8_t uart_send_length = 0;

    // send respin log point.
    print_log_to_uart();

    *(uint32_t *)(uart_send_buf + DEBUG_PROTOCL_PAYLOAD_START_OFFSET) = (flag << 16) | value;

    // Prepare packet
    yichip_debug_uart_protocol(DEBUG_PROTOCL_TYPE_LOG, 4, uart_send_buf, &uart_send_length);

    uart_txBuffer(uart_send_buf, uart_send_length);
}


void print_log_get_lock(void)
{
    *(g_yc_debug_manager.log_lock_flag_cm0_ptr) = 1;// I'm interested
    *(g_yc_debug_manager.log_lock_victim_ptr) = DEBUG_LOG_LOCK_VICTIM_CM0;// you go first
    while(((*g_yc_debug_manager.log_lock_flag_respin_ptr) != 0)
            && (*(g_yc_debug_manager.log_lock_victim_ptr) == DEBUG_LOG_LOCK_VICTIM_CM0));// wait
    return;
}
void print_log_free_lock(void)
{
    *g_yc_debug_manager.log_lock_flag_cm0_ptr = 0;
    return;
}



//*****************************************************************************
//
//! A simple  MyPrintf function supporting \%c, \%d, \%p, \%s, \%u,\%x, and \%X.
//!
//! \param format is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! \return None.
//
//*****************************************************************************
void print_string_to_uart(uint8_t *buf, int len)
{
    uint8_t uart_send_buf[3];
    uint8_t offset = 0;

    if(len == 0)
    {
        return;
    }

    if(len > 255 - DEBUG_PROTOCL_HEADER_SIZE)
    {
        len = 255 - DEBUG_PROTOCL_HEADER_SIZE;
    }

    *(uart_send_buf + offset) = 0xAA;
    offset ++;
    *(uart_send_buf + offset) = DEBUG_PROTOCL_TYPE_STRING;
    offset ++;
    *(uart_send_buf + offset) = len;

    // send header
    uart_txBuffer(uart_send_buf, DEBUG_PROTOCL_PAYLOAD_START_OFFSET);

    uart_txBuffer(buf, len);

    // send trail
    *(uart_send_buf + 0) = 0xBB;
    uart_txBuffer(uart_send_buf, 1);
}


void print_string(uint8_t* uartSndBuf, uint8_t* uartSndBufUseLen, uint8_t* buf, int len)
{
	for(uint8_t i = 0; i < len; i ++)
	{
		if(*uartSndBufUseLen >= DEBUG_STRING_LOG_SINGLE_LINE_MAX_SIZE)
		{
			return;
		}
		*(uartSndBuf + *uartSndBufUseLen) = buf[i];

		*uartSndBufUseLen = *uartSndBufUseLen+1;
	}
}

int print_string_log_raw(char *str, size_t size, char *format, va_list vaArgP)
{
	// For fast display log.
    debugUart_pollTx();
    uint32_t ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
    char *pcStr, pcBuf[16], cFill;
    uint8_t data_for_float [20] ={0};
    char HexFormat;
    static const int8_t* const g_pcHex1 = "0123456789abcdef";
    static const int8_t* const g_pcHex2 = "0123456789ABCDEF";
    uint8_t* uart_tx_buff = str;
    uint8_t uart_tx_local_ptr = 0;

    while(*format)
    {
        // Find the first non-% character, or the end of the string.
        for(ulIdx = 0; (format[ulIdx] != '%') && (format[ulIdx] != '\0');ulIdx++)
        {}

        // Write this portion of the string.
        if(ulIdx>0)
        {
            print_string(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)format, ulIdx);
        }

        format += ulIdx;

        if(*format == '%')
        {
            format++;

            // Set the digit count to zero, and the fill character to space
            // (i.e. to the defaults).
            ulCount = 0;
            cFill = ' ';

        again:
           switch(*format++)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    if((format[-1] == '0') && (ulCount == 0))
                    {
                        cFill = '0';
                    }

                    ulCount *= 10;
                    ulCount += format[-1] - '0';

                    goto again;
                }

                case 'c':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    print_string(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)&ulValue, 1);
                    break;
                }

                case 'd':
                case 'z':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;

                    if((long)ulValue < 0)
                    {
                        ulValue = -(long)ulValue;
                        ulNeg = 1;
                    }
                    else
                    {
                        ulNeg = 0;
                    }

                    ulBase = 10;
                    goto convert;
                }

                case 's':
                {
                    pcStr = va_arg(vaArgP, char *);

                    for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
                    {}

                    print_string(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)pcStr, ulIdx);

                    if(ulCount > ulIdx)
                    {
                        ulCount -= ulIdx;
                        while(ulCount--)
                        {
                            print_string(uart_tx_buff, &uart_tx_local_ptr, " ", 1);
                        }
                    }
                    break;
                }

                case 'u':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;
                    ulBase = 10;
                    ulNeg = 0;
                    goto convert;
                }


                case 'X':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;
                    ulBase = 16;
                    ulNeg = 0;
                    HexFormat='X';
                    goto convert;
                }

                case 'f':
                {
                   float float_data = 0; 
                   float_data = va_arg(vaArgP, double);
                   uint8_t float_data_len = 0;
   		 uint8_t float_data_buf[40]={0};
   		 float_data_len= get_float_data_array(float_data,float_data_buf);
                    print_string(uart_tx_buff, &uart_tx_local_ptr, float_data_buf, float_data_len);
                    break;
                }

                case 'x':
                case 'p':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;
                    ulBase = 16;
                    ulNeg = 0;
                     HexFormat='x';

        convert:
                    for(ulIdx = 1;
                        (((ulIdx * ulBase) <= ulValue) &&
                         (((ulIdx * ulBase) / ulBase) == ulIdx));
                        ulIdx *= ulBase, ulCount--)
                    {
                    }

                    if(ulNeg)
                    {
                        ulCount--;
                    }

                    if(ulNeg && (cFill == '0'))
                    {
                        pcBuf[ulPos++] = '-';
                        ulNeg = 0;
                    }

                    if((ulCount > 1) && (ulCount < 16))
                    {
                        for(ulCount--; ulCount; ulCount--)
                        {
                            pcBuf[ulPos++] = cFill;
                        }
                    }

                    if(ulNeg)
                    {
                        pcBuf[ulPos++] = '-';
                    }

                    for(; ulIdx; ulIdx /= ulBase)
                    {
                        if(HexFormat=='x')  pcBuf[ulPos++] = g_pcHex1[(ulValue / ulIdx) % ulBase];//x
                        else    pcBuf[ulPos++] = g_pcHex2[(ulValue / ulIdx) % ulBase];//X
                    }

                    print_string(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)pcBuf, ulPos);
                    break;
                }

                case '%':
                {
                    print_string(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)format - 1, 1);
                    break;
                }

                default:
                {
                    print_string(uart_tx_buff, &uart_tx_local_ptr, "ERROR", 5);
                    break;
                }
            }//switch
        }//if
    }//while

    return uart_tx_local_ptr;
}

void print_string_log(char *format, ...)
{
	va_list ap;

	va_start(ap, format);

    uint8_t buf[DEBUG_STRING_LOG_SINGLE_LINE_MAX_SIZE];
    int len;
    len = print_string_log_raw(buf, DEBUG_STRING_LOG_SINGLE_LINE_MAX_SIZE, format, ap);
    if(len != 0)
    {
        print_string_to_uart(buf, len);
    }

    va_end(ap);
}




void print_protocol_to_uart(uint16_t logLevel, uint16_t logType, uint8_t* buf, int len)
{
	uint8_t uart_send_buf[3];
	uint8_t offset = 0;

	if(len == 0)
	{
		return;
	}

	if(len > 255 - DEBUG_PROTOCL_HEADER_SIZE)
	{
		len = 255 - DEBUG_PROTOCL_HEADER_SIZE;
	}

	*(uart_send_buf + offset) = 0xAA;
	offset ++;
	*(uart_send_buf + offset) = DEBUG_PROTOCL_TYPE_PROTOCOL;
	offset ++;
	*(uart_send_buf + offset) = len + 4;

	// send header
	uart_txBuffer(uart_send_buf, DEBUG_PROTOCL_PAYLOAD_START_OFFSET);

	offset = 0;
	*(uart_send_buf + offset) = logLevel& 0xff;
	*(uart_send_buf + offset + 1) = (logLevel >> 8) & 0xff;
	offset += 2;
	*(uart_send_buf + offset) = logType & 0xff;
	*(uart_send_buf + offset + 1) = (logType >> 8) & 0xff;
	offset += 2;
	// send sub header
	uart_txBuffer(uart_send_buf, offset);
	
	uart_txBuffer(buf, len);

	// send trail
	*(uart_send_buf + 0) = 0xBB;
	uart_txBuffer(uart_send_buf, 1);
}

void print_hci_h4_to_uart_raw(uint8_t isRcv, uint8_t type, uint8_t* buf, int len)
{
	uint8_t uart_send_buf[3];
	uint8_t offset = 0;

	if(len == 0)
	{
		return;
	}

	*(uart_send_buf + offset) = 0xAA;
	offset ++;
	*(uart_send_buf + offset) = DEBUG_PROTOCL_TYPE_HCI_H4;
	offset ++;
	*(uart_send_buf + offset) = len + 2;
	offset ++;
	*(uart_send_buf + offset) = (len + 2) >> 8;

	// send header
	uart_txBuffer(uart_send_buf, DEBUG_PROTOCL_BIG_PAYLOAD_START_OFFSET);

	offset = 0;
	*(uart_send_buf + offset) = isRcv;
	offset ++;
	*(uart_send_buf + offset) = type;
	offset ++;
	// send sub header
	uart_txBuffer(uart_send_buf, offset);
	
	uart_txBuffer(buf, len);

	// send trail
	*(uart_send_buf + 0) = 0xBB;
	uart_txBuffer(uart_send_buf, 1);
}

void print_hci_h4_to_uart(uint8_t isRcv, uint8_t* buf, int len)
{
    print_hci_h4_to_uart_raw(isRcv, buf[0], buf+1, len - 1);
}

#endif


void print_string_empty_fuction(char *format, ...)
{

}


void print_format_string_buffer(uint8_t* uartSndBuf, uint8_t* uartSndBufUseLen, uint8_t* buf, int len)
{
	for(uint8_t i = 0; i < len; i ++)
	{
		if(*uartSndBufUseLen >= DEBUG_STRING_LOG_SINGLE_LINE_MAX_SIZE)
		{
			return;
		}
		*(uartSndBuf + *uartSndBufUseLen) = buf[i];

		*uartSndBufUseLen = *uartSndBufUseLen+1;
	}
}

int print_format_string(char *str, size_t size, char *format, va_list vaArgP)
{
    uint32_t ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
    char *pcStr, pcBuf[16], cFill;
    char HexFormat;
    static const int8_t* const g_pcHex1 = "0123456789abcdef";
    static const int8_t* const g_pcHex2 = "0123456789ABCDEF";
    uint8_t* uart_tx_buff = str;
    uint8_t uart_tx_local_ptr = 0;

    while(*format)
    {
        // Find the first non-% character, or the end of the string.
        for(ulIdx = 0; (format[ulIdx] != '%') && (format[ulIdx] != '\0');ulIdx++)
        {}

        // Write this portion of the string.
        if(ulIdx>0)
        {
            print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)format, ulIdx);
        }

        format += ulIdx;

        if(*format == '%')
        {
            format++;

            // Set the digit count to zero, and the fill character to space
            // (i.e. to the defaults).
            ulCount = 0;
            cFill = ' ';

        again:
           switch(*format++)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    if((format[-1] == '0') && (ulCount == 0))
                    {
                        cFill = '0';
                    }

                    ulCount *= 10;
                    ulCount += format[-1] - '0';

                    goto again;
                }

                case 'c':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)&ulValue, 1);
                    break;
                }

                case 'd':
                case 'z':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;

                    if((long)ulValue < 0)
                    {
                        ulValue = -(long)ulValue;
                        ulNeg = 1;
                    }
                    else
                    {
                        ulNeg = 0;
                    }

                    ulBase = 10;
                    goto convert;
                }

                case 's':
                {
                    pcStr = va_arg(vaArgP, char *);

                    for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
                    {}

                    print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)pcStr, ulIdx);

                    if(ulCount > ulIdx)
                    {
                        ulCount -= ulIdx;
                        while(ulCount--)
                        {
                            print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, " ", 1);
                        }
                    }
                    break;
                }

                case 'u':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;
                    ulBase = 10;
                    ulNeg = 0;
                    goto convert;
                }


                case 'X':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;
                    ulBase = 16;
                    ulNeg = 0;
                    HexFormat='X';
                    goto convert;
                }

                case 'x':
                case 'p':
                {
                    ulValue = va_arg(vaArgP, unsigned long);
                    ulPos = 0;
                    ulBase = 16;
                    ulNeg = 0;
                     HexFormat='x';

        convert:
                    for(ulIdx = 1;
                        (((ulIdx * ulBase) <= ulValue) &&
                         (((ulIdx * ulBase) / ulBase) == ulIdx));
                        ulIdx *= ulBase, ulCount--)
                    {
                    }

                    if(ulNeg)
                    {
                        ulCount--;
                    }

                    if(ulNeg && (cFill == '0'))
                    {
                        pcBuf[ulPos++] = '-';
                        ulNeg = 0;
                    }

                    if((ulCount > 1) && (ulCount < 16))
                    {
                        for(ulCount--; ulCount; ulCount--)
                        {
                            pcBuf[ulPos++] = cFill;
                        }
                    }

                    if(ulNeg)
                    {
                        pcBuf[ulPos++] = '-';
                    }

                    for(; ulIdx; ulIdx /= ulBase)
                    {
                        if(HexFormat=='x')  pcBuf[ulPos++] = g_pcHex1[(ulValue / ulIdx) % ulBase];//x
                        else    pcBuf[ulPos++] = g_pcHex2[(ulValue / ulIdx) % ulBase];//X
                    }

                    print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)pcBuf, ulPos);
                    break;
                }

                case '%':
                {
                    print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, (uint8_t*)format - 1, 1);
                    break;
                }

                default:
                {
                    print_format_string_buffer(uart_tx_buff, &uart_tx_local_ptr, "ERROR", 5);
                    break;
                }
            }//switch
        }//if
    }//while

    return uart_tx_local_ptr;
}


uint8_t get_float_data_array(float input_value,uint8_t * output_str)
{    
    uint8_t *output_ptr = output_str;
    float value = input_value;
    int hight =0;
    float low =0.0;
    int i =0;
    int ch[20] = {0};
    float tp = 0.1;
    float num = 0.0;
    int count = 0;
    uint8_t len = 0;
    
    hight = (int)value;
    low = value - hight;
    i = 0;
    while(hight > 0)
    {
        ch[i++] = hight%10;
        hight /=10;
    }
    count = i;
    for(i = 0;i<count;i++)
    {
        *(output_ptr++) = ch[i] +'0';
    }
    len += count;
    *output_ptr++ = '.';
    len ++;
    num = low;
    i = 0;
    while((6 > i) && (num > 0))
    {
        ch[i] = (int)(num*10);
        num = num*10 - (int)(num*10);
        i++;

    }
    count = i;
    for(i = 0;i<count;i++)
    {
        *output_ptr++ = ch[i] +'0';
    }
    *output_ptr ='\0';
    len += count;

    return len;
}


