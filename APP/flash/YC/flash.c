#include "flash.h"
#include "yc11xx_dev_qspi.h"
// #include "yc_debug.h"

static void write(uint32_t addr, uint16_t len, uint8_t* buf)
{
    uint16_t soc_len;
    uint8_t soc_i = len / 256 + 1;
    uint8_t addr_len = addr % 256;
    uint32_t soc_addr = addr - addr_len;

    if (addr_len + len > 256 && len < 256) {
        soc_i++;
    }
    for (uint8_t i = 0; i < soc_i; i++)
    {
        uint8_t data[256];

        QSPI_ReadFlashData(soc_addr, sizeof(data), (uint8_t*)data);
        if (addr_len + len < 256) {
            memcpy((void*)&(data[addr_len]), (void*)buf, len);
        }
        else {
            soc_len = 256 - addr_len;
            memcpy((void*)&(data[addr_len]), (void*)buf, soc_len);
        }
        QSPI_EraseFlashPage(soc_addr);
        QSPI_WriteFlashData(soc_addr, sizeof(data), (uint8_t*)data);

        addr_len = 0;       //偏移量
        soc_addr += 256;    //扇区
        len -= soc_len;     //buf 长度
        buf += soc_len;
    }
}

FALSH_ERROR_TypeDef flash_record_exist(FlashRecordAddr_TypeDef record_addr)
{
    addr_inf_t inf;
    QSPI_ReadFlashData(record_addr, sizeof(inf), (uint8_t *)&inf);
    if (inf.addr != 0xFFFFFFFF) {
        return NO_ERROR;
    }
    return NO_RECORD_EXIST;
}

void flash_Erase(FlashRecordAddr_TypeDef record_addr,uint16_t len)
{
    uint8_t data[len];
    memset(data, 0xFFFF, sizeof(data));
    write(record_addr, sizeof(data), data);
}

FALSH_ERROR_TypeDef flash_read(FlashRecordAddr_TypeDef record_addr, uint8_t *buf, uint16_t len)
{
    if(record_addr >= addr_base && record_addr <= addr_base+256)
	{
        addr_inf_t inf;
        if (flash_record_exist(record_addr)) {
            return NO_RECORD_EXIST;
        }
        QSPI_ReadFlashData(record_addr, sizeof(inf), (uint8_t*)&inf);
        if (!(inf.len >> 15)) {
            if (inf.len <= len) {
                QSPI_ReadFlashData(inf.addr, inf.len, buf);
                return NO_ERROR;
            }
            else {
                return ERROR_READ_LEN;
            }
        }
        else {
            QSPI_ReadFlashData(record_addr, sizeof(uint32_t), buf);
            return NO_ERROR;
        }
    }else{
        uint8_t data_exist;

        QSPI_ReadFlashData(record_addr, sizeof(data_exist), (uint8_t *)&data_exist);
        if(data_exist != 0xFF){
            QSPI_ReadFlashData(record_addr, len, buf);
            return NO_ERROR;
        }

        return NO_RECORD_EXIST;
    }
}

void flash_write(FlashRecordAddr_TypeDef record_addr, uint8_t *buf, uint16_t len, Inf_TypedDef type)
{
    addr_inf_t inf;

    if (type == DATA_INF)
    {
        if (record_addr == addr_base) {
            inf.addr = addr_base + 256;
            inf.len = len;
        }
        else {
            QSPI_ReadFlashData(record_addr - 0x8, sizeof(inf), (uint8_t*)&inf);
            if (inf.addr >= addr_base) {
                inf.addr = inf.addr + inf.len;
                inf.len = len;
            }
        }

        write(record_addr, sizeof(inf), (uint8_t*)&inf);
        write(inf.addr, len, buf);
    }
    else if(type == CHUNK_INF){
        write(record_addr, len, buf);
    }
    else
    {
        inf.addr = buf[0];
        inf.addr |= buf[1]<<8;
        inf.addr |= buf[2]<<16;
        inf.addr |= buf[3]<<24;
        inf.len |= 0x1 << 15;
        write(record_addr, sizeof(inf), (uint8_t *)&inf);
    }
}


