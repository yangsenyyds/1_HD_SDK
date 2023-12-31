#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_bt_interface.h"
#include "hci_dtm_test.h"
#include "yc_ota.h"
#include "yc_debug.h"
#include "att.h"
#include "app_config.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "software_timer.h"
#include "vbat.h"
#include "led.h"
#include "keyscan.h"
#include "audio.h"
#include "flash.h"
#include "ir_send.h"
#include "ir_load.h"
#include "ir_lib.h"

#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
typedef struct {
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

enum{
    Power__Keynum = 1,

    Left__Keynum = 18,
    Left__Row = 3,
    HOME__Keynum = 14,
    HOME__Row = 6,
    Voice_Keynum = 33,
    CONN_PARAM = 99,
};
static const uint8_t ir_data[] = {
    0x00,

    0x0C,//1
    0X0D,
    0X01,
    0X02,
    0X03,

    0X04,//6
    0X05,
    0X06,
    0X07,
    0X08,

    0X09,//11
    0X16,
    0X00,
    0X78,
    0X0F,

    0X42,//16
    0X11,
    0X44,//T
    0X46,
    0X45,

    0XFF,//21
    0X64,
    0X43,
    0XB5,
    0XD5,

    0XD6,
    0XE0,
    0XFF,
    0XC2,
    0XAB,

    0X14,
    0X12,
    0X82,
    0X15,
    0XFF,

    0X13,
    0XB4,
    0X40,
    0X61,
    0X41,

    0XB2,
    0XFF,
    0X63,
    0X67,
    0XA0,

    0XA1,
    0XA2,
    0XA3,
};
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x00, 0x00, 4, 57}, // INPUT  1
    {0xE2, 0x00, 4, 57}, // POWER 1
    {0x93, 0x00, 4, 57}, // 1
    {0x61, 0x00, 4, 57}, // 2
    {0x88, 0x00, 4, 57}, // 3

    {0x63, 0x00, 4, 57}, // 4  6
    {0x89, 0x00, 4, 57}, // 5
    {0x65, 0x00, 4, 57}, // 6
    {0x8A, 0x00, 4, 57}, // 7
    {0x8B, 0x00, 4, 57}, // 8

    {0x82, 0x00, 4, 57}, // 9  11
    {0x60, 0x00, 4, 57}, // -
    {0x83, 0x00, 4, 57}, // 0
    {0x23, 0x02, 4, 57}, //CC
    {0x91, 0x00, 4, 57}, // Red

    {0x42, 0x00, 4, 57}, //  16
    {0x40, 0x00, 4, 57}, // 
    {0x44, 0x00, 4, 57}, // 
    {0x41, 0x00, 4, 57}, // 
    {0x45, 0x00, 4, 57}, // 

    {0xFF, 0xFF, 4, 57}, //  21
    {0x24, 0x02, 4, 57}, // 
    {0x43, 0x00, 4, 57}, //
    {0x94, 0x00, 4, 57}, // 
    {0x8E, 0x01, 4, 57}, //     

    {0x5F, 0x02, 4, 57}, //  26
    {0x73, 0x00, 4, 57}, // 
    {0xFF, 0x00, 4, 57}, // 
    {0x92, 0x00, 4, 57}, // 
    {0x97, 0x00, 4, 57}, // 

    {0xE9, 0x00, 4, 57}, //  31
    {0x9C, 0x00, 4, 57}, // 
    {0x21, 0x02, 4, 57}, // 
    {0xEA, 0x00, 4, 57}, // 
    {0xFF, 0x00, 4, 57}, // 

    {0x9D, 0x00, 4, 57}, //  36
    {0xBC, 0x00, 4, 57}, // 
    {0xB4, 0x00, 4, 57}, // 
    {0xB0, 0x00, 4, 57}, // 
    {0xB3, 0x00, 4, 57}, // 

    {0x98, 0x00, 4, 57}, //  41
    {0xFF, 0x00, 4, 57}, // 
    {0xB1, 0x00, 4, 57}, // 
    {0xA0, 0x00, 4, 57}, // 
    {0x69, 0x00, 4, 57}, //

    {0x6A, 0x00, 4, 57}, //  
    {0x6B, 0x00, 4, 57}, //  
    {0x6C, 0x00, 4, 57}, //              
};
static uint8_t adv_data_buf[] = {
 0x02 ,0x01 ,0x06
,0x03 ,0x19 ,0x80 
,0x01 ,0x05 ,0x02 ,0x12 
,0x18 ,0x0F ,0x18
};
static uint8_t scan_rsp_data_buf[] = {
0x0E ,0x09 ,0x54 ,0x56 ,0x20 
,0x42 ,0x4C ,0x45 ,0x20 ,0x52 
,0x65 ,0x6D ,0x6F ,0x74 ,0x65};
static bool voice_key_state;
static bool voice_send_state;
static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;

static bool tx_power_switch = true;
static uint8_t vioce_timernum = 0xFF;
static uint8_t key_pressed_timernum = 0xFF;
static uint8_t vioce_send_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint16_t key_pressed_time;
static uint8_t keynum;
static uint8_t keynum_second;
static uint8_t led_state;
static bool conn_param_state;
static uint32_t sleep_time_state;
static bool encrypt_state;
static bool dis_encrypt_state;
static uint8_t wake_up_state;

static bool SecretKey_Check(void)
{
    uint8_t adcbuf[12];
    uint8_t secretkey_Ori[16];
    uint32_t secretkey_SN_Addr = 0 ;
    uint8_t secretkey_SN[2];
    PublicKey_TypeE secretkey_Type;
    uint8_t secretkey_Gen[16];

    for (uint8_t i = 0; i < 12; i++) {
        adcbuf[i] = HREAD(mem_0_3_6v_adc_io_data + i);
    }
    QSPI_ReadFlashData(SecretKey_Addr, 16, secretkey_Ori);

    QSPI_ReadFlashData(0, 3, (uint8_t *)&secretkey_SN_Addr);
    secretkey_SN_Addr = REVERSE_3BYTE_DEFINE((secretkey_SN_Addr & 0x00ffffff)) - 4;
    QSPI_ReadFlashData(secretkey_SN_Addr, 2, secretkey_SN);
    switch ((secretkey_SN[0] << 8) + secretkey_SN[1])
    {
    case 0x1228:
        secretkey_Type = Key_1228;
        break;
    case 0x3527:
        secretkey_Type = Key_3527;
        break;
    case 0x5815:
        secretkey_Type = Key_5815;
        break;
    
    default:
        return false;
    }
    SecretKey_Generate(secretkey_Type, adcbuf, 12, secretkey_Gen);

    return (memcmp((void *)secretkey_Ori, (void *)secretkey_Gen, 16) == 0) ? true : false;
}

static void low_power_handle(void){
    if(HREADW(mem_le_slave_latency) != CONN_PARAM && bt_check_le_connected()){
        if(key_pressed_num == 0) {
            sleep_time_state++;
            if(sleep_time_state == 600){
                bt_send_le_disconnect(0x13);
                sleep_time_state = 0;
            }
        }else{
            sleep_time_state = 0;
        }
        DEBUG_LOG_STRING("low_power_handle : %d \r\n", sleep_time_state);
    }else{
        sleep_time_state = 0;
        swtimer_stop(low_power_timernum);
    }
}

static void voice_send_handle(void)
{
    if(key_pressed_num == 1 && voice_send_state == true) {
        voice_send_state = false;
        uint8_t hid_send_buf[KeyBuf[Voice_Keynum].key_send_len];
        memset(hid_send_buf,0,sizeof(hid_send_buf));
        ATT_sendNotify(KeyBuf[Voice_Keynum].handle, (void*)hid_send_buf, KeyBuf[Voice_Keynum].key_send_len);
    }
}

static void key_pressed_handle(void)
{
   
    if (key_pressed_num == 1 || key_pressed_num == 0)
    {
        if(keynum == Power__Keynum){
            ir_comm_send(ir_data[keynum]);

            if(key_pressed_num == 1 ) {
                set_key_press_state(true);
            }
        }
        else if(!bt_check_le_connected())
        {

            ir_comm_send(ir_data[keynum]);

            if(key_pressed_num == 1 ) {
                set_key_press_state(true);
            }
        
        }
    }else if(key_pressed_num == 2)
    {
        if (keynum == Left__Keynum && keynum_second == HOME__Keynum)
        {
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                keynum_second = 0;
                key_pressed_time = 0;
                led_off(LED_NUM);
                led_state = true;
                led_on(LED_2,200,60000);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10,true);
                return ;
            }else{
                swtimer_restart(key_pressed_timernum);
            }
        }
    }
}

static void keyvalue_handle(key_report_t* key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    
    if (key_pressed_num == 0)
    {
        if (bt_check_le_connected() && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }

            if(keynum == Voice_Keynum && voice_send_state == true) {
                voice_send_state = false;
                swtimer_stop(vioce_send_timernum);
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }
            else if(keynum != Voice_Keynum){
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                led_off(LED_2);
            }
			if(wake_up_state != 0) {
            	wake_up_state = 0;
        	}			
        }
        if(led_state == 0){
            led_off(LED_1);
        }

        set_key_press_state(false);
    }
    else if (key_pressed_num == 1)
    {
        keynum = 0;
        for(uint8_t i = 0; i < KEY_COL_NUM; i++) {
            keynum +=key_report->keynum_report_buf[i];
        }
        DEBUG_LOG_STRING("KEY [%d][%d][%d][%d][%d] [%d][%d] \r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1], key_report->keynum_report_buf[2],key_report->keynum_report_buf[3]
        ,key_report->keynum_report_buf[4],key_report->keynum_report_buf[5],key_report->keynum_report_buf[6]);
        factory_KeyProcess(keynum==Voice_Keynum?0xff:keynum);
        if(keynum == Power__Keynum) {
            if(bt_check_le_connected() && encrypt_state && led_state == 0){
                led_on(LED_2,100,0);
            }
            else if(led_state == 0) {
                led_on(LED_1,100,0);
            }
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);            
        }
        else if (bt_check_le_connected() && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);
            DEBUG_LOG_STRING("att send  [%x] [%x] [%d] \r\n",KeyBuf[keynum].keyvalue[0],KeyBuf[keynum].keyvalue[1],KeyBuf[keynum].key_send_len);

            if (keynum == Voice_Keynum && !voice_key_state)
            {
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                
                voice_key_state = true;
                voice_send_state = true;
                voice_status.search = true;
                app_sleep_lock_set(AUDIO_LOCK, true);
                voice_status_change();
                swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
                led_state = true;
                led_on(LED_2,0,0);
                
            }
            else if(keynum != Voice_Keynum) {
                if(led_state == 0) {
                    led_on(LED_2,100,0);
                }
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }
            
        }
        else {
            if(led_state == 0) {
                led_on(LED_1,100,0);
            }
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
        }
    }
    else if (key_pressed_num == 2)
    {
        key_pressed_time = 0;
        if (key_report->keynum_report_buf[Left__Row] == Left__Keynum && key_report->keynum_report_buf[HOME__Row] == HOME__Keynum) {
            led_off(LED_NUM);
            if(!bt_check_le_connected()) {
                keynum = Left__Keynum;
                keynum_second = HOME__Keynum;
                swtimer_start(key_pressed_timernum,1000,TIMER_START_ONCE);
            }
        }
    }   
}

void action_after_mic_close(void)
{
    led_state = false;
    led_off(LED_2);
    voice_key_state = false;
}

void action_after_led_blk(void){
    if(led_state == 1){
        led_state = false;
    }
}

static void power_handle(uint8_t batlevel)
{
    DEBUG_LOG_STRING("BAT LEVEL: %d \r\n", batlevel);
    
    if (batlevel <= 20 && tx_power_switch) {
        tx_power_switch = false;
        bt_set_tx_power(TX_POWER_5DB);
    }
    
    if (batlevel <= 5) {
        if (bt_check_le_connected()) {
            ATT_sendNotify(BAT_REPORT_HANDLE, &batlevel, 1);
        }
    }
    else {
        memcpy((void *)Att_List[BAT_ATTLIST_INDEX].dataPtr, (void *)&batlevel, 1);
    }
}

static void remote_control_reinit(void)
{
    keyscan_start();
    software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
    vbat_reinit();
    led_reinit();
    voice_report_reinit();
}

void update_conn_param(bool is_sleep)
{
    if (is_sleep) {
        if(!conn_param_state) {
            conn_param_state = true;
            swtimer_start(low_power_timernum, 100, TIMER_START_REPEAT);
        }
        bt_le_conn_updata_param_req(0x08, 0x08, CONN_PARAM, 400);
    }
    else {
        bt_le_conn_updata_param_req(0x08, 0x08, 0, 400);
    }
}

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if (HREADW(mem_le_slave_latency) == CONN_PARAM) {
        swtimer_stop(low_power_timernum);
        if(!conn_param_state) {
            conn_param_state = true;
        }
        sleep_time_state = 0;
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
    }
}

void stop_adv(void)
{
    if (bt_check_le_connected()) return;
    else
    {
        if(bt_get_le_state() == BLE_DIRECT_ADV) {
            IPC_TxControlCmd(BT_CMD_STOP_DIRECT_ADV);
        }
        else {
            IPC_TxControlCmd(BT_CMD_STOP_ADV);
        }

        if(Bt_CheckIsPaired()) {
            bt_set_le_state(BLE_PAIRED);
        }
        else {
            bt_set_le_state(BLE_IDLE);
        }
    }
}

void start_adv(enum advType type, uint16_t adv_interval, bool timeout)
{
    struct bt_le_adv_param bt_adv_param;
    bt_adv_param.Type = type;
    bt_adv_param.adv_max_interval = adv_interval;

    if (bt_adv_param.Type == ADV_TYPE_NOMAL) {
        if(bt_get_le_state() == BLE_DIRECT_ADV) {
            stop_adv();
        }
        bt_set_le_state(BLE_ADV);
        bt_start_le_adv(&bt_adv_param, adv_data_buf, sizeof(adv_data_buf));
        if (timeout) {
            app_sleep_lock_set(ADV_LOCK, true);
            app_sleep_timer_set(SHUTDOWN_TIME);
        }
    }
    else if(bt_adv_param.Type == ADV_TYPE_DIRECT) {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        if (timeout) {
            app_sleep_lock_set(ADV_LOCK, true);            
            app_sleep_timer_set(DIRECT_ADV_TIME);
        }
    }
}

void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset)
{
    uint8_t senddata[251] = {ATTOP_READ_BLOB_RESPONSE};
    uint16_t sendlen = ((table->dataLen - offset) >= ((HREADW(mem_mtu_size)) - 1)) ? ((HREADW(mem_mtu_size)) - 1) : (table->dataLen - offset);

    DEBUG_LOG_STRING("READ BLOB HANDLE: %d , OFFSET: %d \r\n", table->handle, offset);

    memcpy(senddata + 1, table->dataPtr + offset, sendlen);
    ATT_Send(senddata, sendlen + 1);
}

void Read_Parse(const ATT_TABLE_TYPE *table)
{
    uint8_t senddata[251] = {ATTOP_READ_RESPONSE};
    uint16_t sendlen = (table->dataLen >= (HREADW(mem_mtu_size)) - 1)? ((HREADW(mem_mtu_size)) - 1) : table->dataLen;
    
    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);
    
    memcpy((void *)&senddata[1], table->dataPtr, sendlen);
    ATT_Send(senddata, sendlen+1);
}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);
    factory_WriteDataParse(table->handle, data, len);
    if (table->handle == AUDIO_CMD_HANDLE)
    {
        if(!memcmp(MIC_OPEN, data, 1) && voice_key_state == false)
        {
            DEBUG_LOG_STRING("keep voice open\r\n");
            uint8_t open_error[3] = {0x0c,0X0f,0x01};
            ATT_sendNotify(AUDIO_CTRL_HANDLE, (void*)open_error, 3);
        }
        else
        {
            if (!memcmp(GET_CAPS, data, 1)) {
                voice_status.get_caps = true;
                voice_status_change();
            }
            else if (!memcmp(MIC_OPEN, data, 1)) {
                voice_status.mic_open = true;
                voice_status_change();
                swtimer_stop(vioce_timernum);
            }
            else if (!memcmp(MIC_CLOSE, data, 1)) {
                voice_status.mic_close = true;
                voice_status_change();
            }
        }
    }
    else if (table->handle == OTA_WRITE_HANDLE)
    {
        if (app_sleep_check()) {
            software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        }
        app_sleep_lock_set(APP_LOCK, true);
        app_sleep_lock_set(ADV_LOCK,false);
        OS_ENTER_CRITICAL();
        if(OTA_EVT_HANDLE(len, data) != OTA_NO_ERROR) {
            OTA_RESET();
            app_sleep_lock_set(APP_LOCK, false);
        }
        OS_EXIT_CRITICAL();
    }
    else if (table->dataLen >= len) {
        memcpy((void *)table->dataPtr, (void *)data, len);
    }
    else {
        DEBUG_LOG_STRING("ATT WRITE DATA ERROR, HANDLE: %d, WRITE LEN: %d \r\n", table->handle, len);
    }
}

void LE_LTK_LOST(void)
{
    DEBUG_LOG_BT("BT_EVT_LE_LTK_LOST \r\n");
    if(bt_check_le_connected()) {
        bt_send_le_disconnect(0x06);
        Bt_ClearRemoteDevInfo();
    }
    Bt_ClearDeviceNvdataInfo();
    start_adv(ADV_TYPE_NOMAL, 0x30, true);
}

void ENCRYPT_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("ENCRYPT FAIL! REASON : %x \r\n", reason);
}

void ENCRYPT_DONE(void)
{
    DEBUG_LOG_STRING("ENCRYPT_DONE \r\n");
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
    dis_encrypt_state = false;
    encrypt_state = true;
    update_conn_param(false);
    led_on(LED_2,200,1200);
    ATT_SendExchangeMtuReq();

}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);   
}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);

    encrypt_state = false;
    conn_param_state = false;

    bt_set_tx_power(TX_POWER_5DB);
    Bt_HciFifo_Init();
    app_sleep_lock_set(LATENCY_LOCK, false);
    bt_set_le_state(BLE_IDLE);
    System_ChangeXtal24M();
    if(voice_key_state){
        led_off(LED_NUM);
        voice_key_state = false;
        swtimer_stop(vioce_send_timernum);
        swtimer_stop(vioce_timernum);
    }
    if (reason == 0x13 || reason == 0x16 || dis_encrypt_state) {
        DEBUG_LOG_STRING("dis_encrypt_state %d \r\n",dis_encrypt_state);
        enter_deep_sleep();
        return;
    }
    else if(!dis_encrypt_state && Bt_CheckIsPaired()){
        DEBUG_LOG_STRING("disconnect dis_encrypt_state %d \r\n",dis_encrypt_state);
        dis_encrypt_state = true;
    }

    app_queue_reset();
    remote_control_reinit();
    
    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x08,true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x10,true);
    }
}

void LE_CONNECTED(void)
{
    led_state = false;
    led_off(LED_NUM);
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_lpm_overhead_wake_time(0x0d);
    bt_set_le_state(BLE_CONNECTED);
    bt_disable_le_tx_md();
    if(bt_check_save_connect_info()) {
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    }
}

void Dev_PowerOn(void)
{
    DEBUG_LOG_STRING("DEVICE POWER ON \r\n");

    IPC_TxControlCmd(BT_CMD_FIRST_POWER_ON);
    Flash_LoadFlashRecord();
    Bt_ReadAddrFromFlash();
     
    if (!Lpm_GetWakeFlag())
    {
        bt_renew_scan_rsp((void *)scan_rsp_data_buf, sizeof(scan_rsp_data_buf));
    }

    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x10,true);
    }
}

void tx_power_switch_set(bool switch_enable)
{
    tx_power_switch = switch_enable;
}

void app_task_run(void)
{
    if (tx_power_switch) {
        bt_dynamic_switch_power();
    }
    app_queue_task();
}

void app_init(void)
{
    if (!Lpm_GetWakeFlag())
    {
        bt_set_le_mtu_size(251);        
        bt_update_dle();     
        bt_set_ce_length_num(0x0F);//0x08
        software_timer_start(SYSTEM_CURRENT_CLOCK, 10);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(TC901_54, CUSTOM_0E_0E_A);
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        
        key_pressed_timernum = swtimer_add(key_pressed_handle);
        vioce_timernum = swtimer_add(mic_close);
        vioce_send_timernum = swtimer_add(voice_send_handle);
        low_power_timernum = swtimer_add(low_power_handle);
        if (!SecretKey_Check())
        {
#ifdef SecretKey_Check_enable
            DEBUG_LOG_STRING("SecretKey_Check fail \r\n");
#ifdef FUNCTION_WATCH_DOG
            IWDG_Disable(WDT);   //bt watch dog     
            IWDG_Disable(WDT2);  //riscv watch dog
#endif
            while(1)
            {
                led_on(LED_1, 0, 0);
                SysTick_DelayMs(100);
                led_off(LED_1);
                SysTick_DelayMs(100);
            }
#endif
        }

        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
    else
    {
        app_queue_reset();

        if (key_wakeup_get())
        {
            wake_up_state++;
            if(wake_up_state == 100){
                prepare_before_sleep();
                enter_deep_sleep();                
            }
            sleep_time_state = 0;
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP\r\n" );

        }
        else 
        {
#ifdef SLEEP_ONE_HOUR
            sleep_time_state++;
            if(sleep_time_state > 3800) {
                bt_send_le_disconnect(0x13);
            }
#endif
            enter_low_sleep();
            DEBUG_LOG_STRING("KEEP CONNECT\r\n");
        }
    }
}
