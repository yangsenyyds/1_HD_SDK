#include "flash.h"
#include "yc11xx_dev_qspi.h"
#include "yc_debug.h"

static uint16_t CRC16(uint8_t *buf, uint16_t len)
{
    uint16_t i,j = 0;
    uint16_t crc = 0xffff;

    while (len > 0)
    {
        crc ^= buf[j++];
        for (i = 0; i < 8; ++i)
        {
            if ((crc & 0x01) != 0) {
                crc >>= 1;
                crc ^= 0xa001;
            }
            else {
                crc >>= 1;
            }
        }

        len--;
    }

    return crc;
}

static void write(uint32_t addr, uint16_t len, uint8_t* buf)
{
    uint8_t soc_i;
 
    uint16_t soc_len;

    uint8_t addr_len = addr % 256;
    uint32_t soc_addr = addr - addr_len;
    soc_i = len / 256 + 1;
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
    uint16_t crc  = 0;
    addr_inf_t inf;
    memset((uint8_t *)&inf, 0, sizeof(inf));
    if(record_addr >= addr_base && record_addr <= addr_base+256)
    {
        
        if (flash_record_exist(record_addr)) {
            return NO_RECORD_EXIST;
        }
        QSPI_ReadFlashData(record_addr, sizeof(inf), (uint8_t*)&inf);
        crc = CRC16((uint8_t *)&inf , sizeof(inf) - 2);
        if(crc != inf.crc) {
            return ERROR_CRC_DATA;
        }
        else if (!(inf.len >> 15))
        {
            if (inf.len <= len) {
                uint16_t crc_read;

                QSPI_ReadFlashData(inf.addr, inf.len -2, buf);
                QSPI_ReadFlashData(record_addr + inf.len -2, sizeof(crc_read), (uint8_t *)&crc_read);
                crc = CRC16(buf , inf.len - 2);
                if(crc == crc_read){
                    return NO_ERROR;
                }else{
                    return ERROR_CRC_DATA;
                }
            }
            else {
                return ERROR_READ_LEN;
            }
        }
        else 
        {
            memcpy((void *)buf, (void *)&inf.addr,4);
            return NO_ERROR;
        }
    }
    else
    {
        uint16_t crc_read;

        QSPI_ReadFlashData(record_addr, len -2, buf);
        QSPI_ReadFlashData(record_addr + len -2, sizeof(crc_read), (uint8_t *)&crc_read);
        if(crc_read != 0xFFFF)
        {
            crc = CRC16(buf , len - 2);
            if(crc == crc_read) {
                return NO_ERROR;
            }else {
                return ERROR_CRC_DATA;
            }
        }

        return NO_RECORD_EXIST;
    }
}

void flash_write(FlashRecordAddr_TypeDef record_addr, uint8_t *buf, uint16_t len, Inf_TypedDef type)
{
    addr_inf_t inf;
    memset((uint8_t *)&inf, 0, sizeof(inf));
    uint16_t crc = 0;
    if (type == DATA_INF)
    {
        if (record_addr == addr_base) {
            inf.addr = addr_base + 256;
            inf.len = len + 2;

        }
        else {
            QSPI_ReadFlashData(record_addr - 0x8, sizeof(inf), (uint8_t*)&inf);
            if (inf.addr >= addr_base) {
                inf.addr = inf.addr + inf.len;
                inf.len = len + 2;
            }
        }
        crc = CRC16((uint8_t *)&inf ,sizeof(inf) - 2);
        inf.crc = crc;
        write(record_addr, sizeof(inf), (uint8_t*)&inf);

        crc = CRC16(buf ,len);
        write(inf.addr, len, buf);
        write(inf.addr + len, 2, (uint8_t *)&crc);
    }
    else if(type == CHUNK_INF) {
        write(record_addr, len - 2, buf);

        crc = CRC16(buf ,len - 2);
        write(record_addr + len - 2, 2, (uint8_t *)&crc);
    }
    else
    {
        inf.addr = (uint32_t)buf[0];
        inf.addr |= (uint32_t)buf[1]<<8;
        inf.addr |= (uint32_t)buf[2]<<16;
        inf.addr |= (uint32_t)buf[3]<<24;

        inf.len  |= 0x1 << 15;
        crc = CRC16((uint8_t *)&inf ,sizeof(inf) - 2);
        inf.crc = crc;
        write(record_addr, sizeof(inf), (uint8_t *)&inf);
        
    }
}


