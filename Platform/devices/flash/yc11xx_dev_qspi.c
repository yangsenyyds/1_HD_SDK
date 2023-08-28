#include "yc11xx_dev_qspi.h"
#include "reg_addr.h"
#include "btreg.h"

void __attribute__((noinline)) SetLockQSPI(void)
{
    HWRITE_INLINE(mem_qspi_lock_flag_cm0, 1);
    HWRITE_INLINE(mem_qspi_lock_victim, DOUBLE_LOCK_VICTIM_CM0);

    while(((HREAD_INLINE(mem_qspi_lock_flag_respin)) != 0)
            && ((HREAD_INLINE(mem_qspi_lock_victim)) == DOUBLE_LOCK_VICTIM_CM0));// wait
}

void __attribute__((noinline)) SetReleaseQSPI(void)
{
    HWRITE_INLINE(mem_qspi_lock_flag_cm0, 0);
}


#define FLASH_TRANMIT_BUFFER (0x100)
// We don't suggest use this to work for big memory copy to xram
uint8_t  QSPI_ReadFlashDataToXRAM(uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf)
{
    uint8_t ret, i, buckCnt;
    uint8_t tempBuffer[FLASH_TRANMIT_BUFFER];
    uint32_t reserveCnt;
    buckCnt = rlen / FLASH_TRANMIT_BUFFER;
    reserveCnt = rlen - (buckCnt * FLASH_TRANMIT_BUFFER);
    OS_ENTER_CRITICAL();
    SetLockQSPI( );
    for(i = 0; i < buckCnt; i ++)
    {
        ret = qspi0_flash_read(W25X_FASTREAD_DUAL1, flash_addr + (i * FLASH_TRANMIT_BUFFER), FLASH_TRANMIT_BUFFER, tempBuffer);
        xmemcpy(rbuf + (i * FLASH_TRANMIT_BUFFER), tempBuffer, FLASH_TRANMIT_BUFFER);
    }
    if(reserveCnt != 0)
    {
        ret = qspi0_flash_read(W25X_FASTREAD_DUAL1, flash_addr + (i * FLASH_TRANMIT_BUFFER), reserveCnt, tempBuffer);
        xmemcpy(rbuf + (i * FLASH_TRANMIT_BUFFER), tempBuffer, reserveCnt);
    }
    SetReleaseQSPI( );
    OS_EXIT_CRITICAL();

    return ret;
}

uint8_t QSPI_ReadFlashData(uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf)
{
    uint8_t ret;
    OS_ENTER_CRITICAL();
    SetLockQSPI( );
    ret = qspi0_flash_read(W25X_FASTREAD_DUAL1, flash_addr, rlen, rbuf);
    SetReleaseQSPI( );
    OS_EXIT_CRITICAL();
    
    return ret;
}

uint8_t QSPI_WriteFlashData(uint32_t flash_addr, uint32_t len, uint8_t *tbuf)
{
    uint8_t ret;

    OS_ENTER_CRITICAL();
    QSPI_FlashWriteLock(false, LOCK_FLASH_AREA_ALL);
    SetLockQSPI();
    ret = qspi0_flash_write(flash_addr, len, tbuf);
    SetReleaseQSPI();
    QSPI_FlashWriteLock(true, LOCK_FLASH_AREA_ALL);
    OS_EXIT_CRITICAL();

    return ret;
}

uint8_t QSPI_SectorEraseFlash(uint32_t flash_addr)
{
    uint8_t ret;

    OS_ENTER_CRITICAL();
    QSPI_FlashWriteLock(false, LOCK_FLASH_AREA_ALL);
    SetLockQSPI();
    ret = qspi0_flash_sectorerase(flash_addr);
    SetReleaseQSPI();
    QSPI_FlashWriteLock(true, LOCK_FLASH_AREA_ALL);
    OS_EXIT_CRITICAL();
    return ret;
}

uint8_t QSPI_EraseFlashPage(uint32_t flash_addr)
{
    uint8_t ret;

    OS_ENTER_CRITICAL();
    QSPI_FlashWriteLock(false, LOCK_FLASH_AREA_ALL);
    SetLockQSPI();
    ret = qspi0_flash_blockerasepage(flash_addr);
    SetReleaseQSPI();
    QSPI_FlashWriteLock(true, LOCK_FLASH_AREA_ALL);
    OS_EXIT_CRITICAL();

    return ret;
}

void QSPI_WritePageFlashData(uint32_t addr, uint32_t len, uint8_t *buf)
{
    uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    QSPI_FlashWriteLock(false, LOCK_FLASH_AREA_ALL);
    
    Addr = addr % 256; //本页开始的地址
    count = 256 - Addr; //第一页剩余可写的空间
    NumOfPage = len / 256; //数据可以写多少页
    NumOfSingle = len % 256; //最后一页的数据有多少
    if(Addr == 0)//在页的起始位置开始写
    {
        if(NumOfPage == 0)
        {
            QSPI_WriteFlashData(addr, len, buf);
        }
        else
        {
            while(NumOfPage--)
            {
                QSPI_WriteFlashData(addr, 256, buf);
                addr += 256;
                buf += 256;
            }
            QSPI_WriteFlashData(addr, NumOfSingle, buf);
        }
    }
    else//不在起始地址
    {
        if(NumOfPage == 0)
        {
            if(NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                QSPI_WriteFlashData(addr, count, buf);
                addr += count;
                buf += count;
                QSPI_WriteFlashData(addr, temp, buf);
            }
            else
            {
                QSPI_WriteFlashData(addr, len, buf);
            }
        }
        else//多页写且地址不在起始地址
        {
            len -= count;
            NumOfPage = len / 256;
            NumOfSingle = len % 256;
            QSPI_WriteFlashData(addr, count, buf);
            addr += count;
            buf += count;

            while(NumOfPage--)
            {
                QSPI_WriteFlashData(addr, 256, buf);
                addr += 256;
                buf += 256;
            }

            if(NumOfSingle != 0)
            {
                QSPI_WriteFlashData(addr, NumOfSingle, buf);
            }
        }
    }

    QSPI_FlashWriteLock(true, LOCK_FLASH_AREA_ALL);
}

void QSPI_FlashWriteLock(bool lock, uint16_t area)
{
    OS_ENTER_CRITICAL();
    SetLockQSPI();
    qspi0_flashwrite_lock(lock, area);
    SetReleaseQSPI();
    OS_EXIT_CRITICAL();
}
