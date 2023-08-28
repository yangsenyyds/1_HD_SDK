#include "yc11xx_dev_bt_data_trans.h"
#include "btreg.h"
#include "yc_debug.h"
#include "yc11xx_dev_bt_aap.h"
#include "malloc.h"
#include "yc11xx_ipc.h"
//#include "yc11xx_dev_bt_sco.h"
// #include "yc_ota.h"
#include "att.h"

BT_HCI_DATA_FIFO *gpHciDataFifo = NULL;
BT_HCI_DATA_DEBUG gHciDataDebug;


void Bt_HciFifo_Init(void)
{
    if(gpHciDataFifo != NULL) {
        Memory_Free(gpHciDataFifo);
    }
    
    gpHciDataFifo = (BT_HCI_DATA_FIFO *)Memory_Malloc(sizeof(BT_HCI_DATA_FIFO));
    memset((void *)gpHciDataFifo, 0, sizeof(BT_HCI_DATA_FIFO));
    memset((void *)&gHciDataDebug, 0, sizeof(BT_HCI_DATA_DEBUG));
}

void Bt_HciFifo_DeInit(void)
{
    if(gpHciDataFifo != NULL) {
        Memory_Free(gpHciDataFifo);
    }
}

BT_HCI_DATA *Bt_HciGetSendBufferPtr(void)
{
    BT_HCI_DATA_FIFO *pFifo = gpHciDataFifo;
    return &(pFifo->queue[pFifo->wptr]);
}

void Bt_HciFifoIn(void)
{
    BT_HCI_DATA_FIFO *pFifo = gpHciDataFifo;
    if(pFifo->cnt >= HCI_QUEUE_MAX) return;

    pFifo->cnt++;
    pFifo->wptr++;
    if(pFifo->wptr == HCI_QUEUE_MAX) {
        pFifo->wptr = 0;
    }
}

void Bt_HciFifoOut(void)
{
    BT_HCI_DATA_FIFO *pFifo = gpHciDataFifo;
    if(pFifo->cnt == 0) return;

    pFifo->cnt--;
    pFifo->rptr++;
    if(pFifo->rptr == HCI_QUEUE_MAX) {
        pFifo->rptr = 0;
    }
}

uint8_t Bt_GetRespinUsedBufferNum(void)
{
    return HREAD(mem_alloc_big_cur_len);
}

uint8_t Bt_HciGetUsedBufferNum(void)
{
    BT_HCI_DATA_FIFO *pFifo = gpHciDataFifo;
    return pFifo->cnt;
}

uint8_t Bt_HciCheckBufferFull(void)
{
    BT_HCI_DATA_FIFO *pFifo = gpHciDataFifo;
    if ((pFifo->cnt >= HCI_QUEUE_MAX) /*|| (Bt_GetRespinUsedBufferNum() <= RESPIN_MIN_QUEUE_MIN)*/)
    {
        DEBUG_LOG_STRING("HCI QUEUE IS FULL \r\n");
        return 1;
    }
    return 0;
}

void Bt_HciWaitBufferElement(void)
{
    while(Bt_HciCheckBufferFull());
}

void Bt_SndToBtData(uint8_t subType, uint32_t payloadPtr)
{
    OS_ENTER_CRITICAL();
    uint8_t data[4];
    data[0] = subType;
    data[1] = payloadPtr;
    data[2] = payloadPtr >> 8;
    data[3] = payloadPtr >> 16;
    IPC_TxCommon(IPC_TYPE_CM0_TO_BT_DATA, data, sizeof(data));
    OS_EXIT_CRITICAL();
    
    switch(subType)
    {
    case IPC_DATA_SUBTYPE_BLE:
        gHciDataDebug.bleTxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_SPP:
        gHciDataDebug.sppTxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_SCO:
        gHciDataDebug.scoTxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_AAP:
        gHciDataDebug.aapTxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_HID:
        gHciDataDebug.hidTxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_FREE:
        gHciDataDebug.freeTxCnt ++;
        break;

    default:
        break;
    }
}

void Bt_SndDataToRespin(uint8_t type, const uint8_t *data, uint16_t len)
{
    BT_HCI_DATA *pHciData = Bt_HciGetSendBufferPtr();

    pHciData->data[0] = len;
    pHciData->data[1] = len >> 8;

    memcpy((pHciData->data + 2), data, len);
    pHciData->len = len;
    pHciData->type = type;
    // Only send data ptr.
    Bt_SndToBtData(type, (uint32_t)pHciData->data);
    Bt_HciFifoIn();
}

void Bt_BleSendData(const uint8_t *data, uint16_t len)
{
    if(Bt_HciCheckBufferFull())
    {
        DEBUG_LOG_STRING("Bt_BleSendData ERROR \r\n");
        return;
    }
  
    Bt_SndDataToRespin(IPC_DATA_SUBTYPE_BLE, data, len);
}

void Bt_SppSendData(const uint8_t *data, uint16_t len)
{
    if(Bt_HciCheckBufferFull())
    {
        DEBUG_LOG_STRING("Bt_SppSendData ---- Snd Queue Full.");
        return;
    }
    Bt_SndDataToRespin(IPC_DATA_SUBTYPE_SPP, data, len);
}

void Bt_ParseRcvBLEPacket(uint8_t *payloadPtr, BLE_PacketRcvHeaderTypeDef *headerInfo, uint8_t *rcvBuf)
{
    // Parse packet
    uint16_t offset, l2capLength, l2capCid, attHandle, lenPayload;
    uint8_t attOpcode;
    uint32_t addrPayload;

    offset = 0;
    l2capLength = HREADW(payloadPtr + offset);

    
    offset += 2;
    l2capCid = HREADW(payloadPtr + offset);
    
    offset += 2;
    lenPayload = l2capLength;
    addrPayload = (uint32_t) payloadPtr + offset;
    
    if(lenPayload != 0)
    {
        xmemcpy(rcvBuf, (uint8_t *)reg_map(addrPayload), lenPayload);
    }

    headerInfo->l2capLength = l2capLength;
    headerInfo->l2capCid = l2capCid;
}

void Bt_ParseBLEL2capPacket(uint16_t len, uint8_t *dataPtr, uint8_t cid)
{
    switch(cid)
    {
    case  LE_L2CAP_CID_ATT:
        ATT_Parse(len, dataPtr);
        break;
    case LE_L2CAP_CID_SIGNAL:
        break;
    case LE_L2CAP_CID_SMP:
        break;
        
    default:
        break;
    }
}

void Bt_SppCallback(uint16_t len, uint8_t *dataPtr) {}

void Bt_ParseRcvSPPPacket(int8_t *payloadPtr, SPP_PacketRcvHeaderTypeDef *headerInfo, uint8_t *rcvBuf)
{
    // Parse packet
    uint16_t temp, lenPayload;
    uint8_t uihType, payloadStartOffset, lenExt;
    uint32_t addrPayload;

    //get packet start addr
    addrPayload = (uint32_t)payloadPtr;

    //get seq
    uihType = HREAD(addrPayload + 5);
    lenPayload = HREAD(addrPayload + 6);
    if(lenPayload & 0x01)
    {
        lenExt = 0;
    }
    else
    {
        lenExt = 1;
    }
    if(lenExt)
    {
        lenPayload = HREADW(addrPayload + 6);
    }
    lenPayload = lenPayload >> 1;
    if(uihType == 0xEF)//RFCOMM_FRAME_TYPE_UIH
    {
        if(!lenExt)
        {
            payloadStartOffset = 7;
        }
        else
        {
            payloadStartOffset = 8;
        }
    }
    else
    {
        if(!lenExt)
        {
            payloadStartOffset = 8;
        }
        else
        {
            payloadStartOffset = 9;
        }
    }

    addrPayload = addrPayload + payloadStartOffset;
    xmemcpy(rcvBuf, (uint8_t *)reg_map(addrPayload), lenPayload);

    headerInfo->lenPayload = lenPayload;
    headerInfo->uihType = uihType;
}

void Bt_ScoCallback(uint8_t *dataPtr) {}

void Bt_DataBufferCallBack(uint8_t len, uint8_t *dataPtr)
{
    uint32_t payloadPtr;
    uint8_t subType;
    uint16_t conn_handle;
    BLE_PacketRcvHeaderTypeDef headerInfoBle;
    SPP_PacketRcvHeaderTypeDef headerInfoSpp;
    AAP_PacketRcvHeaderTypeDef *headerInfoAapPtr;
    //uint16_t lenPayload;
    uint8_t rcvBuf[628];
    subType = dataPtr[0];
    payloadPtr = reg_map(dataPtr[1] | (dataPtr[2] << 8) | (dataPtr[3] << 16));

    switch(subType)
    {
    case IPC_DATA_SUBTYPE_BLE:
        Bt_ParseRcvBLEPacket((uint8_t *)payloadPtr, &headerInfoBle, rcvBuf);
        Bt_SndToBtData(IPC_DATA_SUBTYPE_FREE, payloadPtr);
        Bt_ParseBLEL2capPacket(headerInfoBle.l2capLength, rcvBuf,headerInfoBle.l2capCid);
        break;
    case IPC_DATA_SUBTYPE_SPP:
        Bt_ParseRcvSPPPacket((uint8_t *)payloadPtr, &headerInfoSpp, rcvBuf);
        Bt_SndToBtData(IPC_DATA_SUBTYPE_FREE, payloadPtr);
        Bt_SppCallback(headerInfoSpp.lenPayload, rcvBuf);
        break;
    case IPC_DATA_SUBTYPE_SCO:
        xmemcpy(rcvBuf, (uint8_t *)payloadPtr, 70);
        Bt_SndToBtData(IPC_DATA_SUBTYPE_FREE, payloadPtr);
        Bt_ScoCallback(rcvBuf);
        break;
    case IPC_DATA_SUBTYPE_AAP:
        App_EvtAAPCallBack((AAP_PacketRcvHeaderTypeDef *)payloadPtr);
        Bt_SndToBtData(IPC_DATA_SUBTYPE_FREE, payloadPtr);
        break;
    case IPC_DATA_SUBTYPE_HID:
        Bt_SndToBtData(IPC_DATA_SUBTYPE_FREE, payloadPtr);
        break;
    case IPC_DATA_SUBTYPE_MEDIA:
        //Bt_SndToBtData(IPC_DATA_SUBTYPE_FREE, payloadPtr);
#ifdef FUNCTION_MEDIA_DEAL_IN_M0
        Media_QueueInsert(payloadPtr);
#endif
        break;
    case IPC_DATA_SUBTYPE_FREE:
        /************************release tx buffer *****************************/
        Bt_HciFifoOut();
        break;

    default:
        break;
    }


    switch(subType)
    {
    case IPC_DATA_SUBTYPE_BLE:
        gHciDataDebug.bleRxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_SPP:
        gHciDataDebug.sppRxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_SCO:
        gHciDataDebug.scoRxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_AAP:
        gHciDataDebug.aapRxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_HID:
        gHciDataDebug.hidRxCnt ++;
        break;
    case IPC_DATA_SUBTYPE_FREE:
        gHciDataDebug.freeRxCnt ++;
        break;

    default:
        break;
    }
}
