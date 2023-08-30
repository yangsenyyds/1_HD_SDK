#include "yc_dev_bt.h"
#include "yc11xx_systick.h "
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_qspi.h"
#include "yc11xx_dev_bt_data_trans.h "
#include "yc11xx_bt_interface.h"
#include "yc_gap.h"
#include "att.h"
#include "btreg.h"
#include "yc_debug.h"

#include "app_config.h"

WEAK void Dev_PowerOn(void){}
WEAK void Dev_WakeUp(void){Dev_PowerOn();}
WEAK void LE_CONNECTED(void){}
WEAK void LE_DISCONNECTED(uint8_t reason){}
WEAK void PAIR_DONE(void){}
WEAK void PAIR_FAIL(uint8_t reason){
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}
WEAK void ENCRYPT_DONE(void){}
WEAK void ENCRYPT_FAIL(uint8_t reason){}
WEAK void CONN_PARAM_ACCEPTED(void){}
WEAK void LE_LTK_LOST(void){}


/* uninitialized variables need power - off protection*/
volatile BT_REC_INFO gRecinfo;
volatile BT_FAC_STORE_INFO gFacStoreinfo;
static BLE_STATE gBLEState;
volatile BT_REC_INFO *gpRecinfo = &gRecinfo;
volatile BT_FAC_STORE_INFO *gFacRecinfo = &gFacStoreinfo;
/***********************************************************ble driver relative************************************************************/
/*check  reconnect*/
bool bt_check_save_connect_info(void)
{
            /*check  reconnect*/
        if((Flash_BT_STATE_RIGHT  == gpRecinfo->sLinkKeyInfoState)
  && xramcmp((volatile uint8_t*)reg_map(mem_le_plap), gpRecinfo->sPeerBDaddr, 6)
  && xramcmp((volatile uint8_t*)reg_map(mem_le_ltk), gpRecinfo->sLinkKey, 16))
        {
            DEBUG_LOG_BT("reconnect info is not need to save \r\n");
             return false;
        }
        else
        DEBUG_LOG_BT("reconnect info is  need to save \r\n");
            return true;

}
static void bt_set_adv_type(enum advType Type)
{
    HWRITE(mem_le_adv_type, Type);
}

static void bt_set_adv_addr_type(uint8_t addr_type)
{
    HWRITE(mem_le_adv_own_addr_type, addr_type);
}

void bt_state_init(void)
{
    gBLEState.topState = BLE_IDLE;
    gBLEState.secondState = BLE_IDLE;

    for (uint8_t i = 0; i < 6; i++) {
        HWRITE(mem_le_lap + i,  HREAD(mem_0_3_6v_adc_io_data + i));
    }
    DEBUG_LOG_BT("ble %x%x%x%x%x%x\r\n",HREAD(mem_0_3_6v_adc_io_data + 5),
    HREAD(mem_0_3_6v_adc_io_data + 4) ,HREAD(mem_0_3_6v_adc_io_data + 3) ,HREAD(mem_0_3_6v_adc_io_data + 2) ,HREAD(mem_0_3_6v_adc_io_data + 1) 
    ,HREAD(mem_0_3_6v_adc_io_data + 0));
}

void bt_set_le_addr(uint8_t *addr)
{
    uint8_t ble_addr[12];
    for(uint8_t  i = 0; i < 12; i++)
    {
        ble_addr[i] = hexchar2hex(*addr);
        addr++;
    }

    for(uint8_t j = 0; j < 12;)
    {
        HWRITE(mem_le_lap + 5 - (j / 2), BUILD_UINT8(ble_addr[j + 1], ble_addr[j]));	//Big endian
        //HWRITE(mem_le_lap+(j/2), BUILD_UINT8(ble_addr[j+1], ble_addr[j]));	//Little endian
        j += 2;
    }
}

void bt_set_local_dle(uint16_t rx_octets, uint16_t rx_time, uint16_t tx_octets, uint16_t tx_time)
{
    HWRITEW(mem_local_rx_max_octets,rx_octets);
    HWRITEW(mem_local_rx_max_octets+2,rx_time);
    HWRITEW(mem_local_rx_max_octets+4,tx_octets);
    HWRITEW(mem_local_rx_max_octets+6,tx_time);
}

void bt_set_ce_length_num(uint8_t txmd_num)
{
    HWRITE(mem_le_ce_num, txmd_num);
}

void bt_set_auth_for_security_req(uint8_t auth)
{
	HWRITE(mem_le_pres_auth, auth);
}

void bt_send_security_request(void)
{
    HWRITE(mem_le_config, (HREAD(mem_le_config) | 0x08));
}

void bt_update_mtu(void)
{
    HWRITE(mem_le_config, (HREAD(mem_le_config) | 0x04));
}

void bt_update_slave_feature(void)
{
    HWRITE(mem_le_config, (HREAD(mem_le_config) | 0x02));
}

void bt_update_dle(void)
{
    HWRITE(mem_le_config, (HREAD(mem_le_config) | 0x01));
}

void bt_renew_scan_rsp(uint8_t *data, uint8_t len)
{
	uint8_t length=0;
	if(len > LE_SCAN_RSP_MAX_LENGTH)
		return;
	while(len--)
	{
		HWRITE(mem_le_scan_data+length,*data);
		data++;
		length++;
	}
	HWRITE(mem_le_scan_data_len,length);
}

void bt_start_le_adv(const struct bt_le_adv_param *param, const uint8_t *ad_data, uint8_t ad_len)
{
    uint8_t len = ad_len;
    uint8_t length = 0;
    uint8_t addr_type = 0;
    
    bt_set_lpm_overhead_wake_time(0x02); /*chip sleep, overhead wakeup time*/
    HWRITEW(mem_le_adv_interval, param->adv_max_interval);
    HWRITEW(mem_lpm_interval,param->adv_max_interval);
    /*set adv data*/

    if(ad_len > LE_ADV_DATA_MAX_LENGTH)
    {
        DEBUG_LOG_BT("ad_len  oversize error  \r\n");
        return;
    }

    while(len--)
    {
        HWRITE(mem_le_adv_data + length, *ad_data++);
        length++;
    }
    
    HWRITE(mem_le_adv_data_len, length);
    
    /* set adv type*/
    switch(param->Type)
    {
    case ADV_TYPE_NOMAL:
        bt_set_adv_addr_type(BT_ADDR_LE_PUBLIC);
        DEBUG_LOG_BT("ADV_TYPE_NOMAL    --------------\r\n");
        bt_set_adv_type(ADV_TYPE_NOMAL);
        IPC_TxControlCmd(BT_CMD_START_ADV);
        break;
    case ADV_TYPE_DIRECT:
        addr_type = (HREAD(mem_le_conn_peer_addr_type)) << 1;
        bt_set_adv_addr_type(addr_type);
        DEBUG_LOG_BT("ADV_TYPE_DIRECT    --------------\r\n");
        bt_set_adv_type(ADV_TYPE_DIRECT);
        IPC_TxControlCmd(BT_CMD_START_DIRECT_ADV);
        break;
    case ADV_TYPE_NOCONNECT:
        bt_set_adv_addr_type(BT_ADDR_LE_PUBLIC);
        DEBUG_LOG_BT("ADV_TYPE_NOCONNECT    --------------\r\n");
        bt_set_adv_type(ADV_TYPE_NOCONNECT);
        IPC_TxControlCmd(BT_CMD_START_ADV);
        break;
    
    default:
        break;
    }
}

/* 
min_le_interval:uint is 1.25ms
max_le_interval:uint is 1.25ms
SlaveLatency: slave latency
SupervisionTimeout : timeout time uint 10ms
 */
void bt_le_conn_updata_param_req(uint32_t min_le_interval, uint32_t max_le_interval, uint32_t SlaveLatency, uint32_t SupervisionTimeout)
{
	uint8_t num[8];
	char i = 0;
	num[0] = min_le_interval & 0xff;
	num[1] = (min_le_interval>>8) & 0xff;
	num[2] = max_le_interval & 0xff;
	num[3] = (max_le_interval>>8) & 0xff;
	num[4] = SlaveLatency & 0xff;
	num[5] = (SlaveLatency>>8) & 0xff;
	num[6] = SupervisionTimeout & 0xff;
	num[7] = (SupervisionTimeout>>8) & 0xff;
	for (i = 0; i < 8; i++)	
	{
		HWRITE(mem_le_interval_min + i, *(num+i));
	}
	IPC_TxAclCmd(BT_CMD_LE_UPDATE_CONN);
}

void bt_set_le_mtu_size(uint16_t size)
{
    HWRITEW(mem_le_local_mtu, size);
}

void bt_disable_le_tx_md(void)
{
    IPC_TxControlCmd(BT_CMD_LE_TX_MD_OFF);
}

void bt_enable_le_tx_md(void)
{
    IPC_TxControlCmd(BT_CMD_LE_TX_MD_ON);
}

bool bt_check_le_in_adv(void)
{
    return (gBLEState.topState == BLE_ADV) ? true : false;
}

BLE_STATE_TYPE bt_get_le_state(void)
{
    return gBLEState.topState;
}

void bt_set_le_state(uint8_t inState)
{
    gBLEState.topState = inState;
}

bool bt_check_le_connected(void)
{
    return (gBLEState.topState == BLE_CONNECTED) ? true : false;
}

uint8_t bt_disconnect_reason_get(void)
{
    return HREAD(mem_le_diconnect_reson) & 0x7f;
}

uint8_t bt_pair_fail_reason_get(void)
{
    return HREAD(mem_le_ll_pairing_fail_reason);
}

void bt_send_le_disconnect(uint8_t reason)
{
    HWRITE(mem_le_diconnect_reson, reason);
    IPC_TxAclCmd(BT_CMD_LE_DISCONNECT);
}

uint8_t bt_conn_arg_update_status_get(void)
{
    return HREAD2(mem_le_l2cap_signaling_conn_param_update_rsp_result);
}

void bt_enable_le_latecy(void)
{
    IPC_TxControlCmd(BT_CMD_ENABLE_LE_LATENCY);
}

void bt_disable_le_latecy(void)
{
    IPC_TxControlCmd(BT_CMD_DISABLE_LE_LATENCY);
}

void bt_connect_enc_protect(int32_t para)
{
    bt_send_le_disconnect(BT_HCI_ERR_CONN_TIMEOUT);
    bt_set_le_state(BLE_IDLE);
    Bt_ClearRemoteDevInfo();
}

/*1.adv enable   2. connect status  3. latecy is on or off*/
bool bt_check_lpm_allow(void)
{
	if(HREAD(mem_le_adv_enable) && Lpm_CheckLpmFlag()) 
        return true;
            
    if(bt_check_le_connected() && (0 != HREAD(mem_lpm_mult)) && Lpm_CheckLpmFlag())
        return true;
		
	return false;
}

/*  0~761   uint is 1us  delay random:  0*delay_time*1 ~761*delay_time*1  */
void bt_set_adv_random(uint8_t  delay_time)
{
    if(delay_time <= 0  ||  delay_time > 0xc)
        delay_time = 3;
        
    HWRITE(mem_le_adv_delay, delay_time);
}

void bt_set_tx_power(uint8_t power)
{
    HWRITE(mem_tx_power, power);
}

/*1.uint is 625us  adv connect different need interface  2.Users do not modify*/
void bt_set_lpm_overhead_wake_time(uint8_t time)
{
    HWRITEW(mem_lpm_overhead,time);
}

/*
Set the LPM interval:
time: The unit is 1.25ms 
*/
void bt_set_connect_lpm_period(int32_t time)
{
    HWRITEW(mem_time_wake_gap, time);
    bt_enable_le_latecy();
}

#define RSSI_BUFF_SIZE  (4)
static uint16_t rssi_buffer[RSSI_BUFF_SIZE];
static uint8_t rssi_counter=0;
#define POWER_SUPLY 0x50
#define POWER_MINUS 0x50
#define TX_POWER_MINUS_5DB 0x04

void bt_dynamic_switch_power(void)
{
	if(HREAD(mem_rssi_update))
	{
		HWRITE(mem_rssi_update, 0);
		rssi_buffer[rssi_counter] = HREAD(mem_rssi_sum);
		rssi_counter+=1;

		if(rssi_counter < RSSI_BUFF_SIZE) return;

		uint8_t average = MedianFilter(rssi_buffer, RSSI_BUFF_SIZE);
		rssi_counter = 0;

		if(average >= POWER_SUPLY) {
		    bt_set_tx_power(TX_POWER_5DB);
			return;
		}
		
		if(average <= POWER_MINUS) {
			bt_set_tx_power(TX_POWER_0DB);
			return;
		}
	}
}

void bt_white_filter_policy(uint8_t filter_policy)
{
	HWRITE(mem_le_white_filter_policy,filter_policy);
}

void bt_add_white_list( volatile BT_REC_INFO *gvRecinfo)
{
	uint8_t  len=HREAD(mem_white_list_num);
	if(len >= LE_WHITE_LIST_MAX_NUM)
	{
		return;
	}

    xramcpy((volatile uint8_t *)reg_map(mem_white_list + 6*len), gvRecinfo->sPeerBDaddr, 6);
    xramcpy((volatile uint8_t *)reg_map(mem_white_list + 6*len+6), gvRecinfo->sIrk, 16);
    HWRITE(mem_white_list_num,len+1);
}

void bt_clear_white_list(uint8_t *addr)
{
	HWRITE(mem_white_list_num,0);
}

/* flash relative */
uint8_t Bt_SaveBleDeviceInfoToFlash(BLE_DEVICE_INFO_TYPE type)
{
    gpRecinfo->sLinkKeyInfoState = Flash_BT_STATE_RIGHT;

    switch(type)
	{
	    case BLE_CONNECT_ADDR:
	        	for(int i=0;i<6;i++)
            	{
            		gpRecinfo->sPeerBDaddr[i] = HREAD(mem_le_plap+i);
            	}
            	gpRecinfo->sPeerBDaddrType = HREAD(mem_le_conn_peer_addr_type);
	        break;
	    case BLE_ENC_INDENTITY_LTK:
	        	for(int i=0;i<16;i++)
            	{
            		gpRecinfo->sLinkKey[i] = HREAD(mem_le_ltk+i);
            	}
	        break;
	        
	    case BLE_INDENTITY_INFO_IRK:
            for(int i=0;i<16;i++) {
                gpRecinfo->sIrk[i] = HREAD(mem_le_irk+i);
            }
	        break;
	    case BLE_INDENTITY_ADDRESS:
	        for(int i=0;i<6;i++) {
            	gpRecinfo->sPeerPowerUpBDaddr[i] = HREAD(mem_smp_ppaddr+i);
            }
	        break;
        case BLE_MASTER_INDENTITYCATION_ENC_INFO:
            for(int i=0;i<2;i++) {
                gpRecinfo->ediv[i] = HREAD(mem_le_ediv+i);
            }
            for(int i=0;i<8;i++) {
                gpRecinfo->rand[i] = HREAD(mem_le_rand+i);
            }
	        break;
    
	    default:
	        break;
	}

	Flash_UpdateFlashRecordInfo();
}

bool Flash_CheckFlashRecordCRC(void)
{
	uint16_t i,crc_value,sum=0;
	uint8_t* addr = (uint8_t*)gpRecinfo;
	
	crc_value = gpRecinfo->sCRC;
	for(i = 2; i< sizeof(BT_REC_INFO); i++)
	{
		sum = sum + *(addr+i);
	}	
	if (crc_value == sum)
	{
		return true;
	}
	for(i = 0; i < sizeof(BT_REC_INFO); i++)
	{
		*(addr+i) = 0xff;
	}
	return false;
}

uint16_t Flash_FacStoreInfoCrc(uint16_t sCRC, uint16_t FAC_INFO_DATA)
{
    sCRC  = (sCRC >> 8) | (sCRC << 8);
    sCRC ^= FAC_INFO_DATA & 0xff;
    sCRC ^= (sCRC & 0xff) >> 4;
    sCRC ^= sCRC << 12;
    sCRC ^= (sCRC & 0xff) << 5;
    sCRC &= 0xffff;
    return sCRC;
}

uint16_t Flash_FacStoreInfoCRCCheck(void)
{
    uint16_t i, sum ;
    uint8_t *addr = (uint8_t *)(gFacRecinfo);

    for(i = 2, sum = 0xffff; i < sizeof(BT_FAC_STORE_INFO); i++)
    {
        sum = Flash_FacStoreInfoCrc(sum, (*(addr + i)));
    }

    return sum;
}

void Bt_ReadFacStoreInfoFromFlash(void)
{
    // Check flag
    if(gFacRecinfo->sBt_Start_Flag == Flash_FAC_STORE_INFO_FLAG)
    {
        //Check crc
        if(Flash_FacStoreInfoCRCCheck() != 0) return;
    
        for(int i = 0; i < 6; i++)
        {
            HWRITE(mem_le_lap + i, gFacRecinfo->sLocalStoreBDaddr[i]);
        }

    }
    else return;
}

void Flash_LoadFacStoreInfo(void)
{
    OS_ENTER_CRITICAL();
    SetLockQSPI();
    QSPI_ReadFlashData(Flash_FAC_INFO_ADDR, sizeof(BT_FAC_STORE_INFO), (uint8_t *)gFacRecinfo);
    SetReleaseQSPI();
    OS_EXIT_CRITICAL();
}

void Flash_CalFlashRecordCRC(void)
{
    uint16_t i, sum = 0;
    uint8_t *addr = (uint8_t *)gpRecinfo;

    for(i = 2; i < sizeof(BT_REC_INFO); i++)
    {
        sum = sum + *(addr + i);
    }

    /*  gpRecinfo.sCRC  = sum  */
    *((uint16_t *)addr) = sum;
}

void Flash_LoadFlashRecord(void)
{
	//QSPI_Init();
	OS_ENTER_CRITICAL();
	SetLockQSPI( );
	QSPI_ReadFlashData(Flash_BLE_ADDR, sizeof(BT_REC_INFO), (uint8_t *)gpRecinfo);
	SetReleaseQSPI( );
	OS_EXIT_CRITICAL();
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"IPC Rx: 0x%04X", LOG_POINT_A101, gpRecinfo->sLinkKeyInfoState);
	// Must check the store info right or not
	Flash_CheckFlashRecordCRC();
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"IPC Rx: 0x%04X", LOG_POINT_A102, gpRecinfo->sLinkKeyInfoState);
}

void Flash_UpdateFlashRecordInfo(void)
{
    Flash_CalFlashRecordCRC();
    OS_ENTER_CRITICAL();
    SetLockQSPI();
    QSPI_SectorEraseFlash(Flash_BLE_ADDR);
    QSPI_WriteFlashData(Flash_BLE_ADDR, sizeof(BT_REC_INFO), (uint8_t *)gpRecinfo);
    SetReleaseQSPI();
    OS_EXIT_CRITICAL();
}

void Bt_ReadAddrFromFlash(void)
{
    int i;
    if (Bt_CheckIsPaired())
    {
        for(i = 0; i < 16; i++)
            HWRITE(mem_le_ltk + i, gpRecinfo->sLinkKey[i]);

        for(i = 0; i < 6; i++)
            HWRITE(mem_le_plap + i, gpRecinfo->sPeerBDaddr[i]);

        for(i = 0; i < 16; i++)
            HWRITE(mem_le_irk + i, gpRecinfo->sIrk[i]);

        HWRITE(mem_le_conn_peer_addr_type, gpRecinfo->sPeerBDaddrType);
        HWRITE(mem_ltk_exists, 1);
        
        #ifdef BT_WHITE_LIST_ENABLE
        bt_add_white_list(gpRecinfo);
        bt_white_filter_policy(WHITE_LIST_SCAN_FILTER);
        #endif
        
        Bt_UpdateDeviceNvdataInfo();
    }
    else {
        bt_set_le_state(BLE_IDLE);
    }
}

/* ipc callback relative */
void Bt_EvtCallBack(uint8_t len, uint8_t *dataPtr)
{
    if (*dataPtr > BT_EVT_100MS_UINT)
    {
        return;
    }

    // DEBUG_LOG_STRING("BT EvtCallBack 0x%x \r\n", *dataPtr);

    switch(*dataPtr)
    {
    case BT_EVT_LE_CONNECTED:
        LE_CONNECTED();
        break;
    case BT_EVT_LE_DISCONNECTED:
        LE_DISCONNECTED(bt_disconnect_reason_get());
        break;
    case BT_EVT_LE_PARSE_CONN_PARAM_ACCEPTED:
        CONN_PARAM_ACCEPTED();
        break;
    case  BT_EVT_LE_PARSE_CONN_PAPA_UPDATE_RSP:
    {
        uint8_t temp = bt_conn_arg_update_status_get();    // temp = 0 is accept; temp = 1 is reject
    }
        break;

    case BT_EVT_RESTART:
        Dev_WakeUp();
        break;
    case BT_EVT_RESET:
        Dev_PowerOn();
        break;
    
    case BT_EVT_ENCRYPTION_CHANGE:
        ENCRYPT_DONE();
        break;
    
    case BT_EVT_LE_DATA_LENGTH_CHANGE:
        break;
    case BT_EVT_LE_ENC_INFO:
        DEBUG_LOG_BT("BT_EVT_LE_ENC_INFO \r\n");
        Bt_SaveBleDeviceInfoToFlash(BLE_ENC_INDENTITY_LTK);
        break;
    case BT_EVT_LE_INDENTITYINFO:
        DEBUG_LOG_BT("BT_EVT_LE_INDENTITYINFO \r\n");
        Bt_SaveBleDeviceInfoToFlash(BLE_INDENTITY_INFO_IRK);
        break;
    case BT_EVT_INDENTITYADDRESS:
        DEBUG_LOG_BT("BT_EVT_INDENTITYADDRESS \r\n");
        Bt_SaveBleDeviceInfoToFlash(BLE_INDENTITY_ADDRESS);
        break;

    case BT_EVT_LE_SMP_PAIRING_COMPlETE:
    case BT_EVT_MASTER_IDENTIFICATION:
        DEBUG_LOG_BT("BT_EVT_MASTER_IDENTIFICATION \r\n");
        gpRecinfo->sPairState = Flash_BT_STATE_PAIR_FANISH;
        Bt_SaveBleDeviceInfoToFlash(BLE_MASTER_INDENTITYCATION_ENC_INFO);
        Bt_UpdateDeviceNvdataInfo();
        PAIR_DONE();
        break;
    case BT_EVT_LE_PAIRING_FAIL:
        PAIR_FAIL(bt_pair_fail_reason_get());
        break;
    
    case BT_EVT_LE_LTK_LOST:
        LE_LTK_LOST();
       break;
    
    default:
        break;
    }

    HWRITE(mem_save_state, gBLEState.topState);
}

void Bt_BleCallBack(uint8_t len, uint8_t *dataPtr)
{
    
}

bool Bt_CheckIsPaired(void)
{
    return gpRecinfo->sPairState == Flash_BT_STATE_PAIR_FANISH;
}

bool Bt_CheckFastPairedWorkOn(void)
{
    return gpRecinfo->sFirstPowerUpFastWork != Flash_BT_FIRST_PAIR_WORK_READY;
}

void Bt_ClearFastPaired(void)
{
    gpRecinfo->sFirstPowerUpFastWork = Flash_BT_FIRST_PAIR_WORK_READY;
    Flash_UpdateFlashRecordInfo();
    HWRITE(mem_le_pairing_state,0);
}

void Bt_ClearAddrToFlash(void)
{
    gpRecinfo->sPairState = 0x00;
    gpRecinfo->sLinkKeyInfoState = 0x00;
    Flash_UpdateFlashRecordInfo();
}

void Bt_ClearLinkkey(void)
{
    for(uint8_t i = 0;  i < 16;  i++)
    {
       HWRITE(mem_le_ltk+i,0);
    }
    HWRITE(mem_ltk_exists,0);
}

void Bt_ClearRemoteDevInfo(void)
{
    DEBUG_LOG_STRING("Clear Remote DevInfo \r\n");
    memset((uint8_t *)&gRecinfo, 0, sizeof(gRecinfo));
    Bt_ClearAddrToFlash();
    Bt_ClearFastPaired();
    Bt_ClearLinkkey();
    bt_set_le_state(BLE_IDLE);
}

/*********************************************************** nvram save info************************************************************/
/**Currently, information about only one device is saved, Save more pairs later when required**/
void Bt_UpdateDeviceNvdataInfo(void)
{
    uint8_t nv_item_num = HREAD(mem_nv_data_number);
    uint8_t nvdata_offset = 0;
     
    if((0 == nv_item_num) || (nv_item_num>4))
    {
        DEBUG_LOG_STRING("nv_item_num is error!   \r\n");
        return;
    }


    if(PUBLIC_ADDR_TYPE  == HREAD(mem_le_conn_peer_addr_type) )
    {
        HWRITE(mem_nv_data,REC_4_MODE_SA);
        nvdata_offset++;
        
        for(uint8_t i = 0; i<6; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i,gpRecinfo->sPeerBDaddr[i]);
        }
       nvdata_offset += 6;

    }
   else if(BT_ADDR_IS_STATIC(gpRecinfo->sPeerBDaddr[5]))
   {
        HWRITE(mem_nv_data,REC_4_MODE_SA);
        nvdata_offset++;
        
        for(uint8_t i = 0; i<6; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i,gpRecinfo->sPeerBDaddr[i]);
        }
        nvdata_offset += 6;
   }
   else if(BT_ADDR_IS_RPA(gpRecinfo->sPeerBDaddr[5]))
   {
        HWRITE(mem_nv_data,REC_4_MODE_RRPA);
        nvdata_offset++;
        
        for(uint8_t i = 0; i<6; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i,gpRecinfo->sPeerBDaddr[i]);
        }
        nvdata_offset += 6;
        
        for(uint8_t i = 0; i<16; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i, gpRecinfo->sIrk[i]);
        }
          nvdata_offset += 16;
        
   }
   else if(BT_ADDR_IS_NRPA(gpRecinfo->sPeerBDaddr[5]))
   {
        HWRITE(mem_nv_data,REC_4_MODE_RNRPA);
        nvdata_offset++;
        
        for(uint8_t i = 0; i<6; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i,gpRecinfo->sPeerBDaddr[i]);
        }
        nvdata_offset += 6;
        

        for(uint8_t i = 0; i<2; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i,gpRecinfo->ediv[i]);
        }
        nvdata_offset += 2;
 
         for(uint8_t i = 0; i<8; i++)
        {
            HWRITE(mem_nv_data+nvdata_offset+i,gpRecinfo->rand[i]);
        }
        nvdata_offset += 8;
   }
   else
   {
        return;
   }
  
}

void Bt_ClearDeviceNvdataInfo(void)
{
    uint8_t nv_item_num = HREAD(mem_nv_data_number);

    if((0 == nv_item_num) ||(nv_item_num>4))
    {
        DEBUG_LOG_STRING("nv_item_num is error!   \r\n");
        return;
    }
    
    for(uint8_t i = 0;  i< nv_item_num;   i++)
    {
        Bt_ClearNvdataItem(i);
    }
}

void Bt_ClearNvdataItem(uint8_t item_index)
{
    uint8_t nvptr_offset = 0;
    nvptr_offset +=  item_index*NV_ITEM_DATA_LEN;
    
    for(uint8_t i = 0; i< NV_ITEM_DATA_LEN; i++)
    {
        HWRITE(mem_nv_data+nvptr_offset+i,0);
    }
}

void bt_random_data_get(uint8_t *random_data, uint8_t len)
{   
    xmemcpy(random_data, (const uint8_t *)reg_map(mem_cm0_random_data),len);
    IPC_TxControlCmd(BT_CMD_UPDATE_RANDOM_DATA);
}
