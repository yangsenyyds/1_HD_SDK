/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YC11XX_IIS_H_
#define __YC11XX_IIS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "yc11xx.h"


typedef enum
{
    IIS_ClkDivide1,
    IIS_ClkDivide2,
    IIS_ClkDivide3,
}IIS_ClkSelDev;

typedef enum
{
    IIS_Clk6p144M,
    IIS_Clk12p288M,
    IIS_Clk11p2896M,
    IIS_Clk4p096M,
    IIS_ClkFromGpio,
}IIS_AudioClkSelDev;

typedef enum
{
    IIS_TxEnable = 1,
    IIS_RxEnable = 3,
}IIS_TREnTypeDev;

typedef enum
{
    IIS_StandardMode,
    IIS_PcmMode,
    IIS_LeftMode,
    IIS_RightMode,
}IIS_ModeTypeDev;


typedef enum
{
    IIS_16Bit = 16,
    IIS_24Bit = 24,
    IIS_32Bit = 32,
    IIS_64Bit = 64,
}IIS_BitModeSelTypeDev;
typedef enum
{
    IIS_SourceData16Bit = 16,
    IIS_SourceData24Bit = 24,
}IIS_SignificantBitDev;
typedef enum
{
    IIS_SlaveMode,
    IIS_MasterMode,
}IIS_MSModeDev;

typedef enum
{
    IIS_BaudRate8K = 8,
    IIS_BaudRate11p025K = 11,
    IIS_BaudRate12K = 12,
    IIS_BaudRate16K = 16,
    IIS_BaudRate22p05K = 22,
    IIS_BaudRate24K = 24,
    IIS_BaudRate32K = 32,
    IIS_BaudRate44p1K = 44,
    IIS_BaudRate48K = 48,
}IIS_BaudRateDev;

typedef enum
{
    IIS_CyclicMode,
    IIS_WptrMode
}IIS_IsWptrMode;

typedef struct
{
    IIS_TREnTypeDev 			TxRxMode;                /*!< Configure sending and receiving. */
    IIS_ModeTypeDev 			IisMode;                 /*!< Configuration mode. */
    IIS_BitModeSelTypeDev		BitModeSel;        /*!< Configuration bit width. */
    IIS_SignificantBitDev 		SignificantBit;    /*!< Configure valid data bits. */
    IIS_MSModeDev 				MSMode;                    /*!< Configure master or slave. */
    IIS_BaudRateDev 			BaudRate;                /*!< Configure audio sample rate. */
    uint8_t 					*BufferAddrTx;                   /*!< Configure the sending buff address. */
    uint8_t 					*BufferAddrRx;                   /*!< Configure the receiving buff address. */
    uint16_t 					BufferLen;                      /*!< Configure the buff length. */
    IIS_IsWptrMode 				IsWptrMode;               /*!< Configure loop mode or FIFO mode. */
} IIS_ConfigDev;

void IIS_Init(IIS_ConfigDev config);
void IIS_Enable(IIS_ConfigDev config);
bool IIS_IsSendComplete(void);
void IIS_SetTxBuff (uint8_t *val);
uint16_t IIS_GetBufferSize(void);
uint16_t IIS_GetRemindBufferSize(void);
uint16_t IIS_GetWptr(void);
uint16_t IIS_GetRprt(void);


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
