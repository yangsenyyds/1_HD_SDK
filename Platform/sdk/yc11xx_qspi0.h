#ifndef __YC11XX_QSPI0_H_
#define __YC11XX_QSPI0_H_

#include "yc11xx.h"


#define QSPI_BUSYTIMEOUT (0x1000000)
#define DMA_WAIT_TIMEOUT (0x10000)

#define W25X_WRITE_ENABLE			    0x06
#define W25X_WRITE_DISABLE			  0x04
#define W25X_READ_STATUS1			    0x05
#define W25X_READ_STATUS2			    0x35
#define W25X_WRITE_STATUS			    0x01
#define W25X_READ_DATA				    0x03
#define W25X_FASTREAD_DATA		    0x0B
#define W25X_FASTREAD_DUAL1		    0x3B
#define W25X_FASTREAD_DUAL2		    0xBB

#define W25X_FASTREAD_QUAD1		    0x6B
#define W25X_FASTREAD_QUAD2		    0xEB
#define W25X_FASTREAD_QUAD3		    0xE7

#define W25X_PAGE_PROGRAM		      0x02
#define W25X_SECTOR_ERASE			    0x20
#define W25X_BLOCK_ERASE32K			  0x52
#define W25X_BLOCK_ERASE64K       0xD8
#define FlashCMD_PageErase  			0x81
#define W25X_CHIP_ERASE			      0xC7
#define W25X_POWER_DOWN			      0xB9
#define W25X_RELEASE_POWERDOWN    0xAB
#define W25X_DEVICEID             0xAB
#define W25X_MANUFACT_DEVICEID    0x90
#define W25X_JEDEC_DEVICEID		    0x9F


#define QSPICFG_DUAL_MODE			(1 << 0)
#define QSPICFG_QUAD_MODE			(2 << 0)
#define QSPICFG_MBYTE				(1 << 4)
#define QSPICFG_MBYTE_CONT		    (1 << 5)
#define QSPICFG_MRAM				(1 << 6)
#define QCSFT_DUMMY                 8


#define QSPICFG_MODE_3B			    QSPICFG_DUAL_MODE | 8 << QCSFT_DUMMY                           | QSPICFG_MRAM
#define QSPICFG_MODE_6B			    QSPICFG_QUAD_MODE | 8 << QCSFT_DUMMY                           | QSPICFG_MRAM
#define QSPICFG_MODE_BB			    QSPICFG_DUAL_MODE | QSPICFG_MBYTE | 0x6000                     | QSPICFG_MRAM
#define QSPICFG_MODE_EB			    QSPICFG_QUAD_MODE | QSPICFG_MBYTE | 0x6000 | 4 << QCSFT_DUMMY  | QSPICFG_MRAM
#define QSPICFG_MODE_E7			    QSPICFG_QUAD_MODE | QSPICFG_MBYTE | 0x6000 | 2 << QCSFT_DUMMY  | QSPICFG_MRAM

#define CMP_BIT				0x06

void __attribute__((noinline)) prefetch(void *start_addr, void *end_addr);
uint8_t  qspi0_flash_sectorerase(uint32_t flash_addr);
uint8_t  qspi0_flash_blockerase32k(uint32_t flash_addr);
uint8_t  qspi0_flash_blockerase64k(uint32_t flash_addr);
uint8_t  qspi0_flash_blockerasepage(uint32_t flash_addr);
uint8_t  qspi0_flash_chiperase(void);
uint8_t  qspi0_flash_write(uint32_t flash_addr, uint32_t len, uint8_t *tbuf);
uint8_t  qspi0_flash_read(uint8_t mode, uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf);
uint32_t  qspi0_flash_jedecid(uint8_t *cmd, uint8_t *revice);
void QSPI0_SetInter(FunctionalState state);
void QSPI0_ClearInter(void);
uint8_t qspi0_flash_arealock(bool lock, uint16_t area);
uint8_t qspi0_flashwrite_lock(bool lock, uint16_t area);
#endif
