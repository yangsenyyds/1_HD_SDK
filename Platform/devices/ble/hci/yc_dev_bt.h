#ifndef YC_DEV_BT_H
#define YC_DEV_BT_H
#include "yc11xx.h"

//#define BT_WHITE_LIST_ENABLE
#define LE_WHITE_LIST_MAX_NUM 2

#define BT_ADDR_LE_PUBLIC       0x00
#define BT_ADDR_LE_RANDOM       0x01
#define BT_ADDR_LE_PUBLIC_ID    0x02
#define BT_ADDR_LE_RANDOM_ID    0x03

#define BT_ADDR_IS_RPA(a)     (((a)  & 0xc0) == 0x40)
#define BT_ADDR_IS_NRPA(a)    (((a)  & 0xc0) == 0x00)
#define BT_ADDR_IS_STATIC(a)  (((a)   & 0xc0) == 0xc0)

#define REC_4_MODE_SA 0x35
#define REC_4_MODE_RRPA 0x36
#define REC_4_MODE_RNRPA 0x37

#define PUBLIC_ADDR_TYPE  0
#define RANDOM_ADDR_TYPE 1

#define BT_MODULE_NAME "BT_LOG"
#define BT_LOG
#ifdef BT_LOG
#define DEBUG_LOG_BT(fmt, ...)    do {				     \
		DEBUG_LOG_STRING("%s: "fmt"",BT_MODULE_NAME,  ##__VA_ARGS__);		\
		} while (false);
#else
#define DEBUG_LOG_BT  
#endif


#define BUILD_UINT8(loByte, hiByte ) \
    ((uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define AUTH_NONE 0                                                             /**< No auth requirement. */
#define AUTH_BOND (1 << 0)                                                      /**< Bond flag. */
#define AUTH_MITM (1 << 2)                                                      /**< MITM flag. */
#define AUTH_SEC_CON (1 << 3)                                                   /**< Security connection flag. */
#define AUTH_KEY_PRESS_NOTIFY (1 << 4)                                          /**< Key press notify flag. */
#define AUTH_ALL (AUTH_BOND | AUTH_MITM | AUTH_SEC_CON | AUTH_KEY_PRESS_NOTIFY) /**< All authentication flags are on. */

/*********************************************************** error code relative************************************************************/
#define     ERROE -1
#define     SUCCESS 0

/*********************************************************** adv relative************************************************************/
#define LE_ADV_DATA_MAX_LENGTH 31
#define LE_SCAN_RSP_MAX_LENGTH 31

typedef void (*bt_exit_func)(int para);

enum advType
{
	ADV_TYPE_NOMAL,
	ADV_TYPE_DIRECT,
	ADV_TYPE_NOCONNECT,
};

struct bt_le_adv_param
{
    uint32_t adv_time;
    enum advType Type;
    uint16_t  adv_min_interval;
    uint16_t  adv_max_interval;
};

struct bt_adv_data
{
    uint8_t type;
    uint8_t data_len;
    const uint8_t *data;
};

#define BT_LE_ADV_PARAM_INIT(_adv_time, _type, _min_interval, _max_interval) \
{ \
    .adv_time = (_adv_time), \
    .Type = (_type), \
    .adv_min_interval = (_min_interval), \
    .adv_max_interval = (_max_interval), \
}

#define BT_LE_ADV_PARAM(_options, _int_min, _int_max, _peer) \
	((struct bt_le_adv_param[]) { \
		BT_LE_ADV_PARAM_INIT(_options, _int_min, _int_max, _peer) \
	 })


#define BT_LE_ADV_CONN_PARAM BT_LE_ADV_PARAM(ADV_TYPE_NOMAL | \
					    BT_GAP_ADV_STOP_TIMER, \
					    BT_GAP_ADV_FAST_INT_MIN_1, \
					    BT_GAP_ADV_FAST_INT_MAX_2, NULL)


enum scanType
{
	SCAN_TYPE_PASSIVE,
	SCAN_TYPE_ACTIVE,
};

typedef enum
{
	NONE_FILTER,
	WHITE_LIST_SCAN_FILTER,
	WHITE_LIST_CONNECT_FILTER,
	WHITE_LIST_CONN_SCAN_FILTER,
}LE_WHITE_POLICY;

/*********************************************************** linkkey relative************************************************************/

#define Flash_BLE_ADDR 0x7d100	
#define Flash_BT_STATE_RIGHT (0x35)
#define Flash_BT_STATE_PAIR_FANISH (0x36)
#define Flash_BT_FIRST_PAIR_WORK_READY (0x35)
#define Flash_BT_SMP_PPADDR_VALID (0x35)
#define Flash_FAC_INFO_ADDR (0x7f000)
#define Flash_FAC_STORE_INFO_FLAG (0x55aa)

/*********************************************************** nvram save info************************************************************/
#define REC_4_MODE_SA 0x35
#define REC_4_MODE_RRPA 0x36
#define REC_4_MODE_RNRPA 0x37
#define NV_ITEM_DATA_LEN 0x18

typedef struct
{
	// Total Check sum area crc校验
	uint16_t sCRC;
	// Use for first power up work用于第一次开机工作
	uint8_t sFirstPowerUpFastWork;
	// Link key store section 链接密钥存储部分
	uint8_t sLinkKeyInfoState;//链路密钥信息状态
	uint8_t sPeerBDaddrType;//对端BD地址类型
	uint8_t sPeerBDaddr[6];//对端BD地址
	uint8_t sPowerUpBDaddrValid;//Power Up BD addr有效
	uint8_t sPeerPowerUpBDaddr[6];
	uint8_t sLinkKey[16];//链接密钥
	uint8_t sIrk[16];
	uint8_t ediv[2];
	uint8_t rand[8];
	uint8_t sPairState;
}__attribute__((packed)) BT_REC_INFO;

typedef struct
{
    uint16_t sBt_Start_Flag;
    uint8_t sLocalStoreBDaddr[6];
    uint16_t sCRC;
}__attribute__((packed)) BT_FAC_STORE_INFO;

void Flash_LoadFlashRecord(void);
/*********************************************************** rf relative************************************************************/

/*********************************************************** bt state relative************************************************************/
typedef struct
{
	uint8_t topState;
	uint8_t secondState;
}BLE_STATE;

typedef enum
{
	BLE_IDLE,
	BLE_ADV,
	BLE_DIRECT_ADV,
	BLE_CONNECTING,
	BLE_CONNECTED,
	BLE_PAIRED,
	BLE_ENC_FINISH,
	BLE_FAST_PAIRED,
}BLE_STATE_TYPE;

typedef enum
{
	BLE_CONNECT_ADDR,
	BLE_ENC_INDENTITY_LTK,
	BLE_INDENTITY_INFO_IRK,
	BLE_INDENTITY_ADDRESS,
	BLE_MASTER_INDENTITYCATION_ENC_INFO,
}BLE_DEVICE_INFO_TYPE;

/*********************************************************** Function declaration relative************************************************************/
bool bt_check_save_connect_info(void);
void bt_state_init(void);
void bt_set_le_addr(uint8_t *addr);
void bt_set_local_dle(uint16_t rx_octets, uint16_t rx_time, uint16_t tx_octets, uint16_t tx_time);
void bt_set_ce_length_num(uint8_t txmd_num);
void bt_set_auth_for_security_req(uint8_t auth);
void bt_send_security_request(void);
void bt_update_mtu(void);
void bt_update_slave_feature(void);
void bt_update_dle(void);
void bt_renew_scan_rsp(uint8_t *data,uint8_t len);
void bt_start_le_adv(const struct bt_le_adv_param *param, const uint8_t *ad_data, uint8_t ad_len);
void bt_le_conn_updata_param_req(uint32_t min_le_interval, uint32_t max_le_interval, uint32_t SlaveLatency, uint32_t SupervisionTimeout);
void bt_set_le_mtu_size(uint16_t size);
void bt_set_lpm_overhead_wake_time(uint8_t time);
void bt_set_tx_power(uint8_t power);
void bt_set_adv_random(uint8_t  delay_time);
bool bt_check_lpm_allow(void);
void bt_disable_le_tx_md(void);
void bt_enable_le_tx_md(void);
bool Bt_CheckIsPaired(void);
bool bt_check_le_in_adv(void);
uint8_t bt_disconnect_reason_get(void);
uint8_t bt_pair_fail_reason_get(void);
void bt_connect_enc_protect(int32_t para);
BLE_STATE_TYPE bt_get_le_state(void);
void bt_set_le_state(uint8_t inState);
bool bt_check_le_connected(void);
void bt_send_le_disconnect(uint8_t reason);
uint8_t bt_conn_arg_update_status_get(void);
void bt_enable_le_latecy(void);
void bt_disable_le_latecy(void);
void bt_set_connect_lpm_period(int32_t time);
void Bt_ClearRemoteDevInfo(void);
void Bt_ClearLinkkey(void);
void Bt_ClearNvdataItem(uint8_t item_index);
void bt_dynamic_switch_power(void);
uint8_t Bt_SaveBleDeviceInfoToFlash(BLE_DEVICE_INFO_TYPE type);
void Bt_EvtCallBack(uint8_t len, uint8_t *dataPtr);
void Bt_BleCallBack(uint8_t len, uint8_t *dataPtr);
void bt_white_filter_policy(uint8_t filter_policy);
void bt_add_white_list( volatile BT_REC_INFO *gvRecinfo);
void bt_clear_white_list(uint8_t *addr);
void bt_random_data_get(uint8_t  *random_data, uint8_t len);
void Bt_UpdateDeviceNvdataInfo(void);
void Bt_ClearDeviceNvdataInfo(void);
void Bt_ReadAddrFromFlash(void);
void Flash_LoadFacStoreInfo(void);
void Bt_ReadFacStoreInfoFromFlash(void);
void Flash_UpdateFlashRecordInfo(void);
#endif
