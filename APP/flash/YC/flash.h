#ifndef FLASH_H
#define FLASH_H
#include <string.h>
#include "yc11xx.h"

#include "remote_control.h"

typedef enum {
    STATE_INF,//状态
    DATA_INF,//连续的数据
    CHUNK_INF,//存储512字节
}Inf_TypedDef;

typedef enum {
    NO_ERROR,
    NO_RECORD_EXIST,
    ERROR_READ_LEN,
    ERROR_CRC_DATA,
}FALSH_ERROR_TypeDef;

typedef struct {
    uint32_t addr;
    uint16_t len;
    uint16_t crc;
} addr_inf_t;

extern FALSH_ERROR_TypeDef flash_record_exist(FlashRecordAddr_TypeDef record_addr);
extern void flash_Erase(FlashRecordAddr_TypeDef record_addr,uint16_t len);
extern FALSH_ERROR_TypeDef flash_read(FlashRecordAddr_TypeDef record_addr, uint8_t* buf, uint16_t len);
extern void flash_write(FlashRecordAddr_TypeDef record_addr, uint8_t* buf, uint16_t len,Inf_TypedDef type);

#endif