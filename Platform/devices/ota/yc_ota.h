#ifndef YC_OTA_H
#define YC_OTA_H
#include "yc11xx.h"


#define OTA_PACKET_LEN 0x64
#define REVERSE_3BYTE_DEFINE(a) ((a>>16)+(a&0xff00)+((a&0xff)<<16))

#define CODE_BURN_FLASHADDR1    (0x1000)
#define CODE_BURN_FLASHADDR2    (0x40000)
#define CODE_OFFSET_LEN         (3)
#define SERIAL_NAME_LEN         (16)

#define	OTA_EVT_START  	   0xc0
#define	OTA_EVT_DATA       0xc1
#define	OTA_EVT_END        0Xc2
#define	OTA_EVT_RESET      0Xff

#define OTA_EVT 0xBA
#define OTA_CMD 0xAB

#define OTA_END_CHECK_OK        0X55
#define OTA_END_CHECK_FAIL      0XFF

enum OTA_EVT_ERROR_CODE{
    OTA_NO_ERROR,
    OTA_APK_ERROR,
    OTA_FLASHADDR_ERROR,
    OTA_PACKET_ID_ERROR,
    OTA_PACKET_LEN_ERROR,
    OTA_DATA_ERROR,
};

extern void OTA_RESET(void);
extern uint8_t OTA_EVT_HANDLE(uint8_t len, uint8_t *dataPtr);    
#endif
