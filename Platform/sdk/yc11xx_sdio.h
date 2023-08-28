#ifndef __YC11XX_SDIO_H_
#define __YC11XX_SDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "yc11xx.h"
#include "MyPrintf.h"

// sd internal reg
#define SDIO_BLK_SIZE_L           						0
#define SDIO_BLK_SIZE_H           						1
#define SDIO_BLK_CNT_L            						2
#define SDIO_BLK_CNT_H            						3
#define SDIO_CMD_ARG_B0           						4
#define SDIO_CMD_ARG_B1           						5
#define SDIO_CMD_ARG_B2           						6
#define SDIO_CMD_ARG_B3           						7
#define SDIO_TRANS_MODE           						8
#define SDIO_COMMAND              						9
#define SDIO_RSP_B0               						10
#define SDIO_RSP_B1               						11
#define SDIO_RSP_B2               						12
#define SDIO_RSP_B3               						13
#define SDIO_RSP_B4               						14
#define SDIO_RSP_B5               						15
#define SDIO_RSP_B6               						16
#define SDIO_RSP_B7               						17
#define SDIO_RSP_B8               						18
#define SDIO_RSP_B9               						19
#define SDIO_RSP_B10              						20
#define SDIO_RSP_B11              						21
#define SDIO_RSP_B12              						22
#define SDIO_RSP_B13              						23
#define SDIO_RSP_B14              						24
#define SDIO_RSP_IDX              						25
#define SDIO_RSP_CRC              						26
#define SDIO_BLK_GAP              						27
#define SDIO_CLK_CTRL             						28
#define SDIO_CLK_DIV              						29
#define SDIO_TIMEOUT_CNT          						30
#define SDIO_SW_RST               						31
#define SDIO_NORM_IRQ             						32
#define SDIO_ERR_IRQ              						33
#define SDIO_NORM_IRQ_EN          						34
#define SDIO_ERR_IRQ_EN           						35
#define SDIO_CMD_STATUS           						36
#define SDIO_DAT_STATUS           						37
#define SDIO_PAD_STATE1           						38
#define SDIO_PAD_STATE2           						39
#define SDIO_DMA_SW_START         						128
#define SDIO_DMA_CTRL             						129
#define SDIO_DMA_SADDR_L          						130
#define SDIO_DMA_SADDR_H          						131
#define SDIO_DMA_LEN_L            						132
#define SDIO_DMA_LEN_H            						133
#define SDIO_DMA_ADDR_L           						134
#define SDIO_DMA_ADDR_H           						135
#define SDIO_DMA_STATUS           						136

typedef enum
{
	SDCLK_Frequency_24M = 0,
	SDCLK_Frequency_12M,
	SDCLK_Frequency_8M,
	SDCLK_Frequency_6M,
	SDCLK_Frequency_4M = 0x05,
	SDCLK_Frequency_3 = 0x07,
	SDCLK_Frequency_2M = 0x0b,
	SDCLK_Frequency_400KB = 0x3d,
	SDCLK_Frequency_200KB = 0x7a,
}SDCLK_Fre_InitTypeDef;

void SDIO_SetRxSample (FunctionalState cmd);
void SDIO_SetTxSample (FunctionalState cmd);
void SDIO_SetClkDiv (SDCLK_Fre_InitTypeDef SDCLK_Frequency);
void SDIO_SetTranstionMode (uint8_t val);
void SDIO_GetNormalInterupt (uint8_t *vp);
void SDIO_GetErrorInterrupt (uint8_t *vp);
void SDIO_GetCmdStatus (uint8_t *vp);
void SDIO_GetDataStatus (uint8_t *vp);
void SDIO_SoftwareReset (void);
void SDIO_SetCmd (uint8_t val);
void SDIO_SetResponseType (uint8_t val);
void SDIO_SetClkCmd (FunctionalState cmd);
void SDIO_SetCmdArg (uint32_t val);
void SDIO_SetCmdIndexChk (uint8_t val);
void SDIO_SetBlockCount (uint16_t val);
void SDIO_SetCmdCRCChk (uint8_t val);
void SDIO_GetResponse32 (uint8_t *vp);
void SDIO_SetTranstioDir (uint8_t val);
void SDIO_SetBlockSize (uint16_t val);
void SDIO_SetDatResponse (uint8_t val);
void SDIO_SetTimeoutCount (uint8_t val);
void SDIO_SetBusWidth (uint8_t val);
void SDIO_SetDmaStartAddress (uint16_t val);
void SDIO_SetDmaBufferLength (uint16_t val);
void SDIO_SetWtiteDmaStart (FunctionalState cmd);
void SDIO_SetSelectMram (FunctionalState cmd);
void SDIO_WriteReg(uint8_t addr, uint8_t val);
uint8_t SDIO_ReadReg (uint8_t addr);

#endif
