#ifndef YC_BLE_LL_H
#define YC_BLE_LL_H

#include "yc11xx.h"
#define LL_VERSION_OPCODE  0x0c
#define LL_FEATURE_OPCODE 0x08
#define LL_SLAVE_FEATURE_OPCODE  0x0E
#define LL_LENGTH_OPCODE  0x14

//WEAK void LE_Master_Feature(uint8_t *data, uint8_t len){}
void Bt_LmpCallBack(uint8_t len, uint8_t *dataPtr);

#endif
