/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */

/**
  *@file ipc.h
  *@brief ipc support for application.
  */
#ifndef _YC_IPC_H_
#define _YC_IPC_H_
#include <string.h>

#include "yc11xx.h"
#include "btreg.h"

#define BUG_FIX

#ifdef  BUG_FIX
#define FIX_ENTER_LPM	   	  0x01
#define FIX_ENTER_HIBERNATE	0x03
#endif

#define IPC_RX_BUFF_LEN		(254) //for ota, need 200 byte
#define IPC_TX_BUFF_LEN		(40)  //adv need 0x22
#define IPC_BLE_DATA			0x05
#define IPC_SPP_DATA      0x04

#define IPC_CMD_LEN       (3)

/**
  *@brief MCU state.
  */
#define IPC_MCU_STATE_RUNNING   0
#define IPC_MCU_STATE_HIBERNATE 1
#define IPC_MCU_STATE_LMP       2
#define IPC_MCU_STATE_STOP      3

/**
  *@brief MCU phase type.
  */
#define IPC_MCU_PHASE_IDLE	0
#define IPC_MCU_PHASE1_NACK	1
#define IPC_MCU_PHASE1_ACK	2
#define IPC_MCU_PHASE2_NACK	3
#define IPC_MCU_PHASE2_ACK	4

/**
  *@brief IPC buffer address.
  */
#define IPC_TX_BUF_START_ADDR (HREADADDR3(mem_param_ipc_to_bt_buf_start_addr))
#define IPC_TX_BUF_END_ADDR	  (HREADADDR3(mem_param_ipc_to_bt_buf_end_addr))
#define IPC_TX_WPTR_ADDR      (HREADADDR3(mem_param_ipc_to_bt_wptr_addr))
#define IPC_TX_RPTR_ADDR      (HREADADDR3(mem_param_ipc_to_bt_rptr_addr))
#define IPC_RX_WPTR_ADDR      (HREADADDR3(mem_param_ipc_to_m0_wptr_addr))
#define IPC_RX_RPTR_ADDR      (HREADADDR3(mem_param_ipc_to_m0_rptr_addr))
#define IPC_RX_BUF_START_ADDR (HREADADDR3(mem_param_ipc_to_m0_buf_start_addr))
#define IPC_RX_BUF_END_ADDR	  (HREADADDR3(mem_param_ipc_to_m0_buf_end_addr))
#define IPC_MCU_PHASE         mem_ipc_mcu_phase
#define IPC_MCU_STATE         mem_ipc_mcu_state
/**
  *@brief IPC buffer length.
  */
#define IPC_TX_BUF_LEN (IPC_TX_BUF_END_ADDR-IPC_TX_BUF_START_ADDR)
#define IPC_RX_BUF_LEN (IPC_RX_BUF_END_ADDR-IPC_RX_BUF_START_ADDR)

/**
  *@brief IPC tx hardware address.
  */
#define IPC_TX_HEAD	      IPC_TX_BUF_START_ADDR
#define IPC_TX_END	      IPC_TX_BUF_END_ADDR
#define IPC_TX_WRITE_PTR	IPC_TX_WPTR_ADDR
#define IPC_TX_READ_PTR		IPC_TX_RPTR_ADDR

/**
  *@brief IPC rx hardware address.
  */
#define IPC_RX_HEAD	      IPC_RX_BUF_START_ADDR
#define IPC_RX_END	      IPC_RX_BUF_END_ADDR
#define IPC_RX_WRITE_PTR	IPC_RX_WPTR_ADDR
#define IPC_RX_READ_PTR		IPC_RX_RPTR_ADDR

/**
  *@brief IPC command type.
  */
#define IPC_CMD_START_DISCOVERY 		1
#define IPC_CMD_STOP_DISCOVERY 			2
#define IPC_CMD_RECONNECT 				  3
#define IPC_CMD_DISCONNECT 				  4
#define IPC_CMD_ENTER_SNIFF				  5
#define IPC_CMD_SET_PIN_CODE 			  10
#define IPC_CMD_START_INQUIRY 			11
#define IPC_CMD_STOP_INQUIRY 			  12
#define IPC_CMD_START_ADV				    13
#define IPC_CMD_STOP_ADV				    14
#define IPC_CMD_LE_DISCONNECT				17
#define IPC_CMD_START_SCAN				  23
#define IPC_CMD_STOP_SCAN				    24
#define IPC_CMD_ENTER_HIBERNATE 		25
#define IPC_CMD_ROLE_SWITCH 				29
#define IPC_CMD_BB_RECONN_CANCEL 		30
#define IPC_CMD_MTU_EXCHANGE				33
#define IPC_CMD_STOP_24G				    34
#define IPC_CMD_PAIR_24G				    35
#define IPC_CMD_TEST_24G				    36
#define IPC_CMD_TEST_MODE				    37
#define IPC_CMD_UPDATE_SUPERVISION_TO	40
#define IPC_CMD_LE_SET_PINCODE 			  41
#define IPC_CMD_SET_RECONNECT_INIT		42
#define IPC_CMD_START_ADV_REC			    43
#define IPC_CMD_REC_PAGE 				      44
#define IPC_CMD_LOAD_FLASH_PARAMS     45

/**
  *@brief IPC event type.
  */
#define IPC_EVT_NULL					          0x00
#define IPC_EVT_BB_CONNECTED			      0x01
#define IPC_EVT_BB_DISCONNECTED			    0x02
#define IPC_EVT_RECONN_STARTED			    0x03
#define IPC_EVT_RECONN_FAILED			      0x04
#define IPC_EVT_SETUP_COMPLETE			    0x05
#define IPC_EVT_HID_CONNECTED			      0x06
#define IPC_EVT_HID_DISCONNECTED		    0x07
#define IPC_EVT_SPP_CONNECTED			      0x08
#define IPC_EVT_SPP_DISCONNECTED		    0x09
#define IPC_EVT_PINCODE_REQ				      0x0A
#define IPC_EVT_HID_HANDSHAKE			      0x12
#define IPC_EVT_RECONN_PAGE_TIMEOUT     0X13
#define IPC_EVT_LE_CONNECTED			      0x14
#define IPC_EVT_LE_DISCONNECTED			    0x15
#define IPC_EVT_LINKKEY_GENERATE		    0x18
#define IPC_EVT_LE_ENC_INFO				      0x29
#define IPC_EVT_SWITCH_FAIL_MASTER      0x2a
#define IPC_EVT_RESET					          0x2f
#define IPC_EVT_WAKEUP				          0x30
#define IPC_EVT_24G_PAIRING_COMPLETE    0x31
#define IPC_EVT_24G_ATTEMPT_FAIL		    0x32
#define IPC_EVT_HIBERNATE_WAKE			    0x33
#define IPC_EVT_LE_PROTOCOL_CONNECTED	  0x34
#define IPC_EVT_LE_PINCODE_REQ			    0x35
#define IPC_EVT_24G_CHEAK_DONGLE_EXIST	0x36
#define IPC_EVT_RECONNECT_STANDY		    0x37
#define IPC_EVT_24G_ATTEMPT_SUCCESS		  0x38

/**
  *@brief IPC event type define.
  */
#define IPC_EVT_HFP_CONNECTED		      0x60
#define IPC_EVT_HFP_DISCONNECTED	    0x61
#define IPC_EVT_HFP_INDICATORS_UPDATE 0x62
#define IPC_EVT_HFP_UP_TO_MAX_VLM     0x63
#define IPC_EVT_HFP_DOWN_TO_MIN_VLM   0x64

/**
  *@brief IPC type enum.
  */
#define IPC_TYPE_START  0
#define IPC_TYPE_NUM    0xd

#define IPC_TxHidData(dt, len)		IPC_TxCommon(IPC_HID_DATA, (dt), (len))
#define IPC_Tx24GData(dt, len)		IPC_TxCommon(IPC_24G_DATA, (dt), (len))
#define IPC_TxSppData(dt, len)		IPC_TxCommon(IPC_SPP_DATA, (dt), (len))
#define IPC_TxBleData(dt, len)		IPC_TxCommon(IPC_BLE_DATA, (dt), (len))

/**
  *@brief IPC unite bluetooth state.
  */
enum btState
{
  STATE_BT_NULL,
  STATE_BT_CONNECTING,
  STATE_BT_PAIRING,
  STATE_BT_CONNECTED,
  STATE_BT_PAGE,
  STATE_BT_DISCV,
};

/**
  *@brief IPC unite BLE state.
  */
enum blestate
{
  STATE_BLE_NULL,
  STATE_BLE_CONNECTING,
  STATE_BLE_PAIRING,
  STATE_BLE_CONNECTED,
  STATE_BLE_RECONNECTING,
  STATE_BLE_DISCV,
};

/**
  *@brief IPC unite 2.4g state.
  */
enum privprostate
{
  STATE_24G_NULL,
  STATE_24G,
};

/**
  *@brief IPC unit mesh state.
  */
enum meshstate
{
  STATE_NULL,
};

/**
  *@brief BLE format.
  */
typedef struct {
  uint16_t mhandle;
  uint8_t data;
} IPC_BLE_FORMAT;

/**
  *@brief SPP format.
  */
typedef struct {
  uint8_t data;
} IPC_SPP_FORMAT;

/**
  *@brief IPC data format.
  */
typedef struct {
  uint8_t ipctype;
  uint8_t len;
  union
  {
    IPC_BLE_FORMAT uBleData;
  } ipcUnion;
} IPC_DATA_FORMAT;

typedef void (*IPCHandleCb_TypeDef)(uint8_t, uint8_t *);
typedef IPCHandleCb_TypeDef (*IPCHandleCb_Array_TypeDef)[IPC_TYPE_NUM];


bool IPC_IsTxBuffEmpty(void);
bool IPC_IsRxBuffEmpty(void);
bool IPC_TxBufferIsEnough(uint8_t Len);
void IPC_TxPacket(IPC_DATA_FORMAT *packet);
void IPC_TxCommon(uint8_t Type, uint8_t *Dt, uint8_t Len);
void IPC_TxControlCmd(uint8_t Cmd);
void IPC_TxAclCmd(uint8_t Cmd);
void IPC_HandleRxPacket(void);
void IpcDefaultCallBack(uint8_t len, uint8_t *dataPtr);
void IPC_Init(IPCHandleCb_Array_TypeDef cbArrayPtr);

#endif
