#ifndef _ATT_H_
#define _ATT_H_
#include <stddef.h>
#include "yc11xx.h"


void ATT_Send(uint8_t *packet, uint8_t len);
void ATT_SendExchangeMtuReq(void); //发送mtu
void ATT_sendWriteReq(uint16_t handle, uint8_t *dataptr,uint8_t len);
void ATT_sendNotify(uint16_t handle, uint8_t *dataptr,uint8_t len);
void ATT_Parse(uint8_t len,uint8_t *dataPtr);

#endif
