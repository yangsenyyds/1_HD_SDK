#ifndef __YC11xx_AAP_H__
#define __YC11xx_AAP_H__
#include "yc11xx.h"

typedef struct
{
    uint16_t lenPayload;
    uint16_t l2capCid;
    uint8_t dataPayload[1];
} __attribute__((packed)) AAP_PacketRcvHeaderTypeDef;

void App_EvtAAPCallBack(AAP_PacketRcvHeaderTypeDef *packet);

#endif
