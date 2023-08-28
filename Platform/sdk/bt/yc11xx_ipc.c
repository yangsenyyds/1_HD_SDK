/*
 * Copyright 2016, Yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
#include "yc11xx_ipc.h"
#include "yc_debug.h"
#include "app_config.h"

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t ipcReadBuff[IPC_RX_BUFF_LEN] ;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t ipcSendBuff[IPC_TX_BUFF_LEN] ;

static IPCHandleCb_Array_TypeDef ipc_handle_cb_array;

static void ipcRx(uint8_t *Dest, uint16_t Len)
{
    uint32_t RxPtr = HREAD24BIT(IPC_RX_READ_PTR);

    for (uint16_t i = 0; i < Len; i++)
    {
        Dest[i] = HREAD(RxPtr);
        if(++RxPtr >= IPC_RX_END) {
            RxPtr = IPC_RX_HEAD;
        }
    }

    OS_ENTER_CRITICAL();
    HWRITE24BIT(IPC_RX_READ_PTR, RxPtr);
    OS_EXIT_CRITICAL();
}

static void ipcTx(uint8_t *Src, uint8_t Len)
{
    uint32_t TxPtr = HREAD24BIT(IPC_TX_WRITE_PTR);
    
    for (uint16_t i = 0; i < Len; i++)
    {
        uint8_t delay = 100;
        
        HWRITE(TxPtr, Src[i]);
        if(++TxPtr >= IPC_TX_END) {
            TxPtr = IPC_TX_HEAD;
        }
        
        while(delay--);
    }

    OS_ENTER_CRITICAL();
    HWRITE24BIT(IPC_TX_WRITE_PTR, TxPtr);
    OS_EXIT_CRITICAL();
}

bool IPC_IsTxBuffEmpty(void)
{
    uint32_t TxWritePtr = HREAD24BIT(IPC_TX_WRITE_PTR);
    uint32_t TxReadPtr = HREAD24BIT(IPC_TX_READ_PTR);
    uint32_t TxReadPtr1 = HREAD24BIT(IPC_TX_READ_PTR);
    
    if (TxReadPtr != TxReadPtr1) return false;
    if (TxWritePtr == TxReadPtr) return true;
    
    return false;
}

bool IPC_IsRxBuffEmpty(void)
{
    uint32_t RxReadPtr = HREAD24BIT(IPC_RX_READ_PTR);
    uint32_t RxWritePtr = HREAD24BIT(IPC_RX_WRITE_PTR);
    uint32_t RxWritePtr1 = HREAD24BIT(IPC_RX_WRITE_PTR);

    if (RxWritePtr != RxWritePtr1) return true;
    if (RxReadPtr == RxWritePtr) return true;
    
    return false;
}

bool IPC_TxBufferIsEnough(uint8_t Len)
{
    uint32_t TxPtr = HREAD24BIT(IPC_TX_WRITE_PTR);
    uint32_t TxReadPtr = HREAD24BIT(IPC_TX_READ_PTR);
    uint32_t TxReadPtr1 = HREAD24BIT(IPC_TX_READ_PTR);

    if(TxReadPtr != TxReadPtr1) {
        TxReadPtr = HREAD24BIT(IPC_TX_READ_PTR);
    }

    for (uint16_t i = 0; i < Len; i++)
    {
        if(++TxPtr >= IPC_TX_END)
            TxPtr = IPC_TX_HEAD;

        if(TxPtr == TxReadPtr)
            return false;

    }
    return true;
}

void IPC_TxPacket(IPC_DATA_FORMAT *packet)
{
    if (packet->len == 0)
        return;

    if (IPC_MCU_STATE_LMP == HREAD(IPC_MCU_STATE))
    {
        HWRITE(IPC_MCU_STATE, IPC_MCU_STATE_RUNNING);
    }

    if(!IPC_TxBufferIsEnough((packet->len+2)*2))
    {
        _ASSERT_FAULT();
    }

    ipcTx((unsigned char *)packet,  packet->len + 2);
}

void IPC_TxCommon(uint8_t Type, uint8_t *Dt, uint8_t Len)
{
    ipcSendBuff[0] = Type;
    if (Len == 0 || Len >= (IPC_TX_BUFF_LEN - 2))
        return;

    if (IPC_MCU_STATE_LMP == HREAD(IPC_MCU_STATE))
    {
        HWRITE(IPC_MCU_STATE, IPC_MCU_STATE_RUNNING);
    }

    if(!IPC_TxBufferIsEnough(Len * 2))
    {
        _ASSERT_FAULT();
    }

    ipcSendBuff[1] = Len;
    memcpy(&ipcSendBuff[2], Dt, Len);
    ipcTx(ipcSendBuff, Len + 2);
}

void IPC_TxControlCmd(uint8_t Cmd)
{
    uint8_t cmdBuff[IPC_CMD_LEN] = {IPC_TYPE_CMD, 0x01};

    if (IPC_MCU_STATE_LMP == HREAD(IPC_MCU_STATE)) {
        HWRITE(IPC_MCU_STATE, IPC_MCU_STATE_RUNNING);
    }

    if(!IPC_TxBufferIsEnough(IPC_CMD_LEN*2)) {
        _ASSERT_FAULT();
    }
    
    cmdBuff[IPC_CMD_LEN - 1] = Cmd;
    ipcTx(cmdBuff, IPC_CMD_LEN);
}

void IPC_TxAclCmd(uint8_t Cmd)
{
    uint8_t cmdBuff[IPC_CMD_LEN] = {IPC_TYPE_ACL_CMD, 0x01};

    if (IPC_MCU_STATE_LMP == HREAD(IPC_MCU_STATE)) {
        HWRITE(IPC_MCU_STATE, IPC_MCU_STATE_RUNNING);
    }

    if(!IPC_TxBufferIsEnough(IPC_CMD_LEN*2)) {
        _ASSERT_FAULT();
    }
    
    cmdBuff[IPC_CMD_LEN - 1] = Cmd;
    ipcTx(cmdBuff, IPC_CMD_LEN);
}

void IPC_HandleRxPacket(void)
{
    if (IPC_IsRxBuffEmpty()) return;
    
    uint8_t *pbuff = ipcReadBuff;

    ipcRx(ipcReadBuff, 2);
    if(*pbuff > IPC_TYPE_START && *pbuff < IPC_TYPE_NUM) {
        ipcRx(ipcReadBuff + 2, *(pbuff + 1));
        (*(*ipc_handle_cb_array)[*pbuff])(*(pbuff + 1), pbuff + 2);
    }
    else {
        _ASSERT_FAULT();
    }
}

void IpcDefaultCallBack(uint8_t len, uint8_t *dataPtr)
{
    DEBUG_LOG_STRING("IpcDefaultCallBack \r\n");
    while(1);
    return;
}

void IPC_Init(IPCHandleCb_Array_TypeDef cb)
{
    ipc_handle_cb_array = cb;
}
