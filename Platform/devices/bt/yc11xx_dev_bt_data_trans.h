#ifndef _YC11XX_DEV_BT_DATA_TRANS_H_
#define _YC11XX_DEV_BT_DATA_TRANS_H_
#include "yc11xx.h"

#define LE_L2CAP_CID_ATT    0x0004
#define LE_L2CAP_CID_SIGNAL 0x0005
#define LE_L2CAP_CID_SMP    0x0006


typedef struct
{
    uint16_t sppTxCnt;
    uint16_t bleTxCnt;
    uint16_t scoTxCnt;
    uint16_t aapTxCnt;
    uint16_t hidTxCnt;
    uint16_t freeTxCnt;

    uint16_t sppRxCnt;
    uint16_t bleRxCnt;
    uint16_t scoRxCnt;
    uint16_t aapRxCnt;
    uint16_t hidRxCnt;
    uint16_t freeRxCnt;
} __attribute__((packed)) BT_HCI_DATA_DEBUG;


typedef struct
{
    uint16_t l2capLength;
    uint16_t l2capCid;
    uint16_t attHandle;
    uint8_t attOpcode;
    uint16_t lenPayload;
} BLE_PacketRcvHeaderTypeDef;

typedef struct
{
    uint16_t lenPayload;
    uint16_t uihType;
} SPP_PacketRcvHeaderTypeDef;


#define RESPIN_MIN_QUEUE_MIN    (6)
#define HCI_QUEUE_MAX           (14)
#define HCI_DATA_BUFFER_SIZE    (251)

typedef struct
{
    uint16_t type;
    uint16_t len;
    uint8_t data[HCI_DATA_BUFFER_SIZE];
} __attribute__((packed)) BT_HCI_DATA;

typedef struct
{
    uint8_t rptr;
    uint8_t wptr;
    uint16_t cnt;
    BT_HCI_DATA queue[HCI_QUEUE_MAX];
} __attribute__((packed)) BT_HCI_DATA_FIFO;


void Bt_BleSendData(const uint8_t *data, uint16_t len);
void Bt_HciFifo_Init(void);
void Bt_HciFifo_DeInit(void);
BT_HCI_DATA *Bt_HciGetSendBufferPtr(void);
void Bt_HciFifoIn(void);
void Bt_HciFifoOut(void);
uint8_t Bt_GetRespinUsedBufferNum(void);
uint8_t Bt_HciGetUsedBufferNum(void);
uint8_t Bt_HciCheckBufferFull(void);
void Bt_HciWaitBufferElement(void);
void Bt_SndToBtData(uint8_t subType, uint32_t payloadPtr);
void Bt_SndDataToRespin(uint8_t type, const  uint8_t *data, uint16_t len);
void Bt_ParseRcvBLEPacket(uint8_t *payloadPtr, BLE_PacketRcvHeaderTypeDef *headerInfo, uint8_t *rcvBuf);
void Bt_BleL2capCallback(uint16_t len, uint8_t *dataPtr);
void Bt_SppCallback(uint16_t len, uint8_t *dataPtr);
void Bt_ParseRcvSPPPacket(int8_t *payloadPtr, SPP_PacketRcvHeaderTypeDef *headerInfo, uint8_t *rcvBuf);
void Bt_DataBufferCallBack(uint8_t len, uint8_t *dataPtr);

#endif //_YC11XX_DEV_BT_DATA_TRANS_H_
