#include "yc11xx_qspi0.h"
#include "reg_addr.h"


void __attribute__((noinline)) prefetch(void *start_addr, void *end_addr)
{
    int addr;
    for(addr = (int)start_addr & ~0x1f; addr < (int)end_addr + 32; addr += 32)
    {
        PREFETCH_LINE(addr);
    }
}

static void qspi0_dma_start(void *src, int srclen, void *dst, int dstlen)
{
    HWRITEW(CORE_QSPI_TXADDR, (int)src);
    HWRITEW(CORE_QSPI_TXLEN, srclen);
    HWRITEW(CORE_QSPI_RXADDR, (int)dst);
    HWRITEW(CORE_QSPI_RXLEN, dstlen);
    HWRITE(CORE_DMA_START, 8);
}

static uint8_t qspi0_dma_wait(void)
{
    int timeout = 0;
    while(!(HREAD(CORE_DMA_STATUS) & 8))
    {
        timeout++;
        if(timeout > DMA_WAIT_TIMEOUT)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}


static uint8_t qspi0_write(uint8_t *tbuf, uint32_t len)
{
    uint8_t ctrl = HREAD_INLINE(CORE_QSPI_CTRL);
    uint8_t delay = HREAD_INLINE(CORE_QSPI_DELAY);
    uint8_t ret = 0;

    HWRITE(CORE_QSPI_CTRL, 0x44);
    HWRITE(CORE_QSPI_DELAY, 0x80);
    qspi0_dma_start(tbuf, len, 0, 0);
    ret = qspi0_dma_wait();
    HWRITE(CORE_QSPI_CTRL, ctrl);
    HWRITE(CORE_QSPI_DELAY, delay);
    return ret;
}


static uint8_t  qspi0_read_write(uint8_t *tbuf, uint32_t tlen, uint8_t *rbuf, uint32_t rlen)
{
    uint8_t ctrl = HREAD_INLINE(CORE_QSPI_CTRL);
    uint8_t delay = HREAD_INLINE(CORE_QSPI_DELAY);
    uint8_t ret = 0;
    HWRITE(CORE_QSPI_CTRL, 0x44);
    HWRITE(CORE_QSPI_DELAY, 0x80);
    qspi0_dma_start(tbuf, tlen, rbuf, rlen);
    ret = qspi0_dma_wait();
    HWRITE(CORE_QSPI_CTRL, ctrl);
    HWRITE(CORE_QSPI_DELAY, delay);
    return ret;
}



static uint8_t  qspi0_flash_cmd(uint8_t cmd)
{
    uint8_t tbuf[1];
    tbuf[0] = cmd;

    return qspi0_write(tbuf, 1);
}


static uint8_t  qspi0_flash_readstatus1(uint8_t *rbuf)
{
    uint8_t tbuf[1];
    tbuf[0] = W25X_READ_STATUS1;
    return qspi0_read_write(tbuf, 1, rbuf, 1);
}

static uint8_t  qspi0_flash_readstatus2(uint8_t *rbuf)
{
    uint8_t tbuf[1];
    tbuf[0] = W25X_READ_STATUS2;
    return qspi0_read_write(tbuf, 1, rbuf, 1);
}


static uint8_t  qspi0_flash_waitfinish(void)
{
    uint32_t timeout = 0;
    uint8_t status1 = 1;

    while(status1 & 1)
    {
        qspi0_flash_readstatus1(&status1);

        if(++timeout > QSPI_BUSYTIMEOUT)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}


static uint8_t  qspi0_flash_writestatus(uint8_t *reg)
{
    uint8_t tbuf[3];
    tbuf[0] = W25X_WRITE_STATUS;
    tbuf[1] = reg[0];
    tbuf[2] = reg[1];

    if(ERROR == qspi0_flash_cmd(W25X_WRITE_ENABLE))
        return ERROR;

    qspi0_write(tbuf, 3);

    return qspi0_flash_waitfinish();
}



static uint8_t  qspi0_flash_quadenable(uint8_t on)
{
    uint8_t reg[2] = {0};

    if(ERROR == qspi0_flash_readstatus1(&reg[0]))
        return ERROR;

    if(ERROR == qspi0_flash_readstatus2(&reg[1]))
        return ERROR;

    if(on)
        reg[1] |= 0x02;
    else
        reg[1] &= 0xfd;

    return qspi0_flash_writestatus(reg);
}


static uint8_t  qspi0_flash_writeenable(void)
{
    return qspi0_flash_cmd(W25X_WRITE_ENABLE);
}



static uint8_t  qspi0_flash_writedisable(void)
{
    return qspi0_flash_cmd(W25X_WRITE_DISABLE);
}

static void  qspi0_flash_precmd(uint8_t *buf, uint8_t cmd, uint32_t addr)
{
    buf[0] = W25X_PAGE_PROGRAM;
    buf[1] = addr >> 16;
    buf[2] = addr >> 8;
    buf[3] = addr;
}

#define BLOCK_UNIT  (256)
static uint8_t  _qspi0_flash_write(uint32_t flash_addr, uint32_t len, uint8_t *tbuf)
{
    uint8_t buf[BLOCK_UNIT + 4] = {0};
    uint32_t packnum = (len + BLOCK_UNIT - 1) / BLOCK_UNIT;
    uint32_t packlen = 0;

    for(uint32_t i = 0; i < packnum; i++)
    {
        packlen = len > BLOCK_UNIT ? BLOCK_UNIT : len;

        qspi0_flash_precmd(buf, W25X_PAGE_PROGRAM, flash_addr + i * BLOCK_UNIT);

        for(uint32_t j = 0; j < packlen; j++)
        {
            buf[4 + j] =  tbuf[j + i * BLOCK_UNIT];
        }

        if(ERROR == qspi0_flash_writeenable())
            return ERROR;

        if(ERROR == qspi0_write(buf, packlen + 4))
            return ERROR;

        if(ERROR == qspi0_flash_waitfinish())
            return ERROR;

        len -= packlen;
    }

    return SUCCESS;
}



static uint8_t  _qspi0_flash_chiperase(void)
{
    if(ERROR == qspi0_flash_writeenable())
        return ERROR;

    if(ERROR ==  qspi0_flash_cmd(W25X_CHIP_ERASE))
        return ERROR;

    return qspi0_flash_waitfinish();
}


static uint8_t  _qspi0_flash_erase(uint8_t cmd, uint32_t flash_addr)
{
    uint8_t tbuf[4] = {0};
    if(ERROR == qspi0_flash_writeenable())
        return ERROR;

    tbuf[0] = cmd;
    tbuf[1] = flash_addr >> 16;
    tbuf[2] = flash_addr >> 8;
    tbuf[3] = flash_addr;

    if(ERROR == qspi0_write(tbuf, 4))
        return ERROR;

    return qspi0_flash_waitfinish();
}


static uint8_t  _qspi0_flash_read(uint8_t mode, uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf)
{
    uint8_t ret = 0;
    uint8_t tbuf[5] = {0};
    uint32_t tlen = 5;
    uint8_t ctrl =  HREAD_INLINE(CORE_QSPI_CTRL);
    uint8_t delay = HREAD_INLINE(CORE_QSPI_DELAY);

    tbuf[0] = mode;
    tbuf[1] = flash_addr >> 16;
    tbuf[2] = flash_addr >> 8;
    tbuf[3] = flash_addr;
    tbuf[4] = 0;

    switch(mode)
    {
    case W25X_FASTREAD_DUAL1 :
        tlen = 4;
        HWRITEW(CORE_QSPI_CTRL, QSPICFG_MODE_3B);
        break;
    case W25X_FASTREAD_DUAL2 :
        HWRITEW(CORE_QSPI_CTRL, QSPICFG_MODE_BB);
        break;
    case W25X_FASTREAD_QUAD1 :
        tlen = 4;
        HWRITEW(CORE_QSPI_CTRL, QSPICFG_MODE_6B);
        break;
    case W25X_FASTREAD_QUAD2 :
        HWRITEW(CORE_QSPI_CTRL, QSPICFG_MODE_EB);
        break;
    case W25X_FASTREAD_QUAD3 :
        HWRITEW (CORE_QSPI_CTRL, QSPICFG_MODE_E7);
        break;
    default :
        tlen = 4;
        HWRITEW(CORE_QSPI_CTRL, 0x44);
        break;
    }
    qspi0_dma_start(tbuf, tlen, rbuf, rlen);
    ret = qspi0_dma_wait();
    HWRITE(CORE_QSPI_CTRL, ctrl);
    HWRITE(CORE_QSPI_DELAY, delay);
    return ret;
}

uint32_t  _qspi0_flash_JEDECID(uint8_t *cmd, uint8_t *revice)
{
    uint8_t tbuf[1];
    uint8_t rbuf[3];
    uint32_t ret;
    tbuf[0] = W25X_JEDEC_DEVICEID;
    qspi0_read_write(cmd, 1, revice, 3);
    ret = (revice[0] << 16) | (revice[1] << 8) | (revice[2] << 0);
    return ret;
}

uint8_t qspi0_flash_sectorerase(uint32_t flash_addr)
{
    prefetch(qspi0_dma_start, qspi0_flash_sectorerase);
    return _qspi0_flash_erase(W25X_SECTOR_ERASE, flash_addr);
}


uint8_t  qspi0_flash_blockerase32k(uint32_t flash_addr)
{
    prefetch(qspi0_dma_start, qspi0_flash_blockerase32k);
    return _qspi0_flash_erase(W25X_BLOCK_ERASE32K, flash_addr);
}

uint8_t  qspi0_flash_blockerase64k(uint32_t flash_addr)
{
    prefetch(qspi0_dma_start, qspi0_flash_blockerase64k);
    return _qspi0_flash_erase(W25X_BLOCK_ERASE64K, flash_addr);
}


uint8_t  qspi0_flash_blockerasepage(uint32_t flash_addr)
{
    prefetch(qspi0_dma_start, qspi0_flash_blockerasepage);
    return _qspi0_flash_erase(FlashCMD_PageErase, flash_addr);
}


uint8_t  qspi0_flash_chiperase(void)
{
    prefetch(qspi0_dma_start, qspi0_flash_chiperase);
    return _qspi0_flash_chiperase();
}

uint8_t  qspi0_flash_write(uint32_t flash_addr, uint32_t len, uint8_t *tbuf)
{
    prefetch(qspi0_dma_start, qspi0_flash_write);
    return _qspi0_flash_write(flash_addr, len, tbuf);
}


uint8_t  qspi0_flash_read(uint8_t mode, uint32_t flash_addr, uint32_t rlen, uint8_t *rbuf)
{
    prefetch(qspi0_dma_start, qspi0_flash_read);
    return _qspi0_flash_read(mode, flash_addr, rlen, rbuf);
}


uint32_t  qspi0_flash_JEDECID(uint8_t *cmd, uint8_t *revice)
{
    prefetch(qspi0_dma_start, qspi0_flash_JEDECID);
    return _qspi0_flash_JEDECID(cmd, revice);
}

void QSPI0_SetInter(FunctionalState state)
{
	if(state != DISABLE) {
        HWOR(CORE_I2S_MRAM_SEL, 1 << 5);
    }
	else {
        HWCOR(CORE_I2S_MRAM_SEL, 1 << 5);
    }
}

void QSPI0_ClearInter(void)
{
	HWOR(CORE_I2S_MRAM_SEL, 1 << 4);
	HWCOR(CORE_I2S_MRAM_SEL, 1 << 4);
}

uint8_t qspi0_flash_arealock(bool lock, uint16_t area)
{
    uint8_t reg[2];

    if(qspi0_flash_readstatus1(&reg[0]) == ERROR) return ERROR;
    if(qspi0_flash_readstatus2(&reg[1]) == ERROR) return ERROR;
        
    reg[0] &= (~0x7c);
    reg[1] &= (~0x7c);
	
    if (lock) {
        reg[0] |= area;
    }
    
    if (area < 0xff) {
        reg[1] &= (~(1<<CMP_BIT));
    }
    else {
        reg[1] |= (1<<CMP_BIT);
    }

    return qspi0_flash_writestatus(reg);
}

uint8_t qspi0_flashwrite_lock(bool lock, uint16_t area)
{
    uint8_t ret = 0;
    
    prefetch(qspi0_dma_start, qspi0_flashwrite_lock);
    ret = qspi0_flash_arealock(lock, area);

    return ret;
}
