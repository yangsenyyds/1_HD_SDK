/**
 * @file yc11xx_qspi.h
 * @author duanchu.he
 * @brief QSPI driver
 * @version 0.1
 * @date 2021-07-06
 *
 *
 */
#ifndef __YC11XX_QSPI_H__
#define __YC11XX_QSPI_H__

#include "yc11xx.h"

#define QSPI_WAITTIMEOUT 		(0x100000)

#define QSPI_MODE_1_BIT        0
#define QSPI_MODE_2_BIT        1
#define QSPI_MODE_4_BIT        2


typedef enum
{
    QSPI1 = 0,
    QSPI2 = 1,
} QSPI_NUM;

typedef struct
{
    uint16_t 	txaddr;
    uint16_t	txlen;
    uint16_t 	rxaddr;
    uint16_t	rxlen;
    uint8_t		wslen;
    uint8_t		delay;
    uint16_t 	cmdaddr;
    uint8_t 	cmdlen;
} QSPI_AddrTypeDef;

void QSPI_AddrConfig(QSPI_NUM qspix, QSPI_AddrTypeDef *qspi_handle);
void QSPI_SetMode (QSPI_NUM qspix, uint8_t val);
void QSPI_SetInc (QSPI_NUM qspix, FunctionalState new_state);
void QSPI_SetClk (QSPI_NUM qspix, FunctionalState new_state);
void QSPI_SetInterrupt(QSPI_NUM qspix, FunctionalState new_state);
void QSPI_SetStart (QSPI_NUM qspix,  FunctionalState new_state);
void QSPI_WaitDone (QSPI_NUM qspix);

#endif
