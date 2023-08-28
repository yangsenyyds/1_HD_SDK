

/**
 * Copyright 2016, yichip Semiconductor(shanghai office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
#ifndef _YC11XX_DEV_QSPI_H_
#define _YC11XX_DEV_QSPI_H_
#include "yc11xx.h"
#include "yc11xx_qspi0.h"

#define LOCK_FLASH_AREA_0h_FFFh 		0x0064
#define LOCK_FLASH_AREA_0h_1FFFh 		0x0068
#define LOCK_FLASH_AREA_0h_3FFFh 		0x006c
#define LOCK_FLASH_AREA_0h_7FFFh		0x0070
#define LOCK_FLASH_AREA_0h_FFFFh		0x0024
#define LOCK_FLASH_AREA_0h_1FFFFh		0x0028
#define LOCK_FLASH_AREA_0h_3FFFFh		0x002c
#define LOCK_FLASH_AREA_0h_7FFFFh		0x0030
#define LOCK_FLASH_AREA_ALL				0x003e

#define LOCK_FLASH_AREA_0h_6FFFFh       0x4004
#define LOCK_FLASH_AREA_0h_5FFFFh       0x4008
#define LOCK_FLASH_AREA_10000h_7FFFFh   0x4020
#define LOCK_FLASH_AREA_20000h_7FFFFh   0x4028
#define LOCK_FLASH_AREA_40000h_7FFFFh   0x402c
#define LOCK_FLASH_AREA_0h_7EFFFh       0x4044
#define LOCK_FLASH_AREA_0h_7DFFFh       0x4048
#define LOCK_FLASH_AREA_0h_7BFFFh       0x404c
#define LOCK_FLASH_AREA_0h_77FFFh       0x4050

void __attribute__((noinline)) SetLockQSPI(void);
void __attribute__((noinline)) SetReleaseQSPI(void);

uint8_t QSPI_ReadFlashDataToXRAM(uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf);
uint8_t QSPI_ReadFlashData(uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf);
uint8_t QSPI_WriteFlashData(uint32_t flash_addr, uint32_t len, uint8_t *tbuf);
uint8_t QSPI_SectorEraseFlash(uint32_t flash_addr);
uint8_t QSPI_EraseFlashPage(uint32_t flash_addr);
void QSPI_WritePageFlashData(uint32_t addr, uint32_t len, uint8_t *buf);
void QSPI_FlashWriteLock(bool lock, uint16_t area);
#endif
