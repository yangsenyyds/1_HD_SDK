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

#include "yc11xx_lpm.h"
#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
typedef struct {
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

enum{
    Power_Keynum = 1,
    NETFLIX_Keynum = 19,
    Voice_Keynum = 3,
    OK_Keynum = 8,
    OK_Col = 1,
    Vol__Keynum = 17,
    Vol_Col = 4,
    Vol_Keynum = 16,
    VolCol = 3,
    Back__Keynum = 13,
    Back_Col = 0,
	CONN_PARAM = 99,
};
#if (Project_key == 901)
static const uint8_t ir_data[] = {
    0x00,

    0x59,
    0X87,
    0X22,
    0X82,
    0X80,

    0X06,
    0X5a,
    0X1A,
    0X1B,
    0X16,

    0X11,
    0X0D,
    0X05,
    0X19,
    0X55,

    0X13,
    0X17,
    0X15,//方框
    0X61,
    0X71,

    0X73,
    0X72,
};

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x66, 0x00, 8, 121},
    {0xBB, 0x01, 4, 125},
    {0x21, 0x02, 4, 125},
    {0x2A, 0x02, 4, 125},
    {0x0A, 0x00, 4, 125},

    {0x42, 0x00, 4, 125},//6
    {0x44, 0x00, 4, 125},
    {0x41, 0x00, 4, 125},
    {0x45, 0x00, 4, 125},
    {0x43, 0x00, 4, 125},   

    {0x11, 0x00, 4, 125}, //11
    {0X23, 0x02, 4, 125}, 
    {0x24, 0x02, 4, 125},
    {0xE2, 0X00, 4, 125},//
    {0x4b, 0X00, 8, 121}, 

    {0xE9, 0X00, 4, 125},//16
    {0xEA, 0X00, 4, 125},
    {0x4E, 0X00, 8, 121},
    {0x6F, 0X00, 4, 125},
    {0x07, 0X00, 4, 125},

    {0x06, 0X00, 4, 125},//21
    {0x09, 0X00, 4, 125},
};

#endif
static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};
const uint8_t product_key_s[] = {22,1,134};
static uint8_t adv_data_buf[] = 
{
 0x02 ,0x01 ,0x05 ,0x03 ,0x03 ,0x12 ,0x18 ,0x03 ,0x19 ,0x80 ,0x01 
,0x0b ,0x09 ,0x52 ,0x65 ,0x6d ,0x6f, 0x74, 0x65, 0x42, 0x30, 0x35, 0x31
};
static uint8_t scan_rsp_data_buf[] = {
 0x0B ,0x09 ,0x52 ,0x65 ,0x6D ,0x6F 
,0x74 ,0x65 ,0x42 ,0x30 ,0x35 ,0x31 

};
static uint8_t power_adv_data_buf[] = 
{
 0X02 ,0X01 ,0X05 ,0X03 ,0X19 ,
 0X80 ,0X01 ,0X12 ,0XFF ,0XFF ,
 0XFF ,0X41 ,0X6D ,0x6D ,0X6F ,
 0X67 ,0X69 ,0X63 ,0X01 ,0xFF ,
 0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0X00};
static uint8_t Netflix_adv_data_buf[] = 
{
0x02 ,0x01 ,0x05 ,0x03 ,0x19 ,0x80 ,0x01 ,0x12 ,0xFF 
,0xFF ,0xFF ,0x41 ,0x6D ,0x6D ,0x6F ,0x67 ,0x69 ,0x63 ,0x02 ,0xFF ,0xFF ,0xFF 
,0xFF ,0xFF ,0xFF ,0x00};

// 12 FF FF FF 41 6D 6C 6F 67 69 63 01 D9 
// C2 38 F9 53 1C 09
/*netflix 
02 01 05 03 03 12 18 03 19 80 01 12 FF 
FF FF 41 6D 6C 6F 67 69 63 02 D9 C2 38 
F9 53 1C 08
power 
02 01 05 03 03 12 18 03 19 80 01 12 FF 
FF FF 41 6D 6C 6F 67 69 63 01 D9 C2 38 
F9 53 1C 0A
*/
 static uint8_t key_pressed_num;
static bool tx_power_switch = true;
static uint8_t vioce_timernum = 0xFF;
static uint8_t key_pressed_timernum = 0xFF;
static uint8_t vioce_send_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint8_t key_press_time_timernum = 0xFF;
static uint16_t key_pressed_time;
static uint8_t keynum;
static uint8_t keynum_second;

static uint8_t ir_state;
static uint8_t led_state;
static bool conn_param_state;
static uint8_t updata_connect_state;
static uint32_t sleep_time_state;
/// 0600 04 00 01 30 07
static uint8_t f_buf[] = {0x00, 0x01, 0x30, 0x09};
static uint8_t f_ack[] = {0x07, 0x00, 0x03, 0x00, 0x01, 0x31, 0x09, 0x13}; /// 第3字节为自增ack_idx++
/// 060097 00 01 30 07
static uint8_t second_buf[] = {0x00, 0x01, 0x30, 0x07};
static uint8_t secondbuf_ack[] = {0x08, 0x00, 0x00, 0x00, 0x01, 0x32, 0x07, 0xFC, 0x03}; /// 第3字节为自增ack_idx++
/// 060098 00 01 30 02
static uint8_t thr_buf[] = {0x00, 0x01, 0x30, 0x02};
static uint8_t thr_buf_ack1[] = {0x13, 0x38, 0x00, 0x00, 0x01, 0x01, 0x2E, 0x02, 0x46, 0x3B, 0x20, 0x03, 0x53, 0x4F, 0x4E, 0x59, 0x20, 0x56, 0x52, 0x43}; /// 第3字节为自增ack_idx++
static uint8_t thr_buf_ack2[] = {0x06, 0x10, 0x08, 0x00, 0x00, 0x20, 0x20}; 

static uint8_t ack_idx = 0;

static bool voice_key_state;
static bool voice_send_state;
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

static void key_press_time_handle(void){
    if(key_pressed_num == 1)
    {
        // DEBUG_LOG_STRING("key_pressed_time : %d \r\n", key_pressed_time);
        key_pressed_time++;
        if(key_pressed_time >= 60)
        {
            DEBUG_LOG_STRING(" ok key_pressed_time : %d \r\n", key_pressed_time);
            key_pressed_time = 0;
            if(!bt_check_le_connected())
            {
                set_key_press_state(false);
            }
            else
            {
                uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
                memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);

                if(keynum == Voice_Keynum && voice_send_state == true) {
                    voice_send_state = false;
                    swtimer_stop(vioce_send_timernum);
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }else if(keynum != Voice_Keynum) {
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
            }
            return;
        }
        swtimer_restart(key_press_time_timernum);
    }
    else{
        key_pressed_time = 0;
    }
}

static void low_power_handle(void)
{
    if(HREADW(mem_le_slave_latency) != CONN_PARAM && bt_check_le_connected())
    {
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
    if(key_pressed_num == 0 || key_pressed_num == 1)
    {
        if(!bt_check_le_connected())
        {
            ir_comm_send(ir_data[keynum]);
            if(key_pressed_num == 1) {
                set_key_press_state(true);
            }
        }
    }
    else if(key_pressed_num == 2) {
        if (keynum == OK_Keynum && keynum_second == Vol__Keynum)
        {
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                keynum_second = 0;
                key_pressed_time = 0;
                led_state = true;
                led_on(LED_1,200,60000);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10,true);
                return ;
            }else{
                swtimer_restart(key_pressed_timernum);
            }
        }
        else if(keynum == OK_Keynum && keynum_second == Vol_Keynum)
        {
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                keynum_second = 0;
                key_pressed_time = 0;
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                bt_send_le_disconnect(0x13);
                return ;
            }else{
                swtimer_restart(key_pressed_timernum);
            }
        }
        else if(keynum == OK_Keynum && keynum_second == Back__Keynum)
        {
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                uint8_t feedback[4] = {0x12, 0x00, 0x00, 0x00};
                ATT_sendNotify(125, (void*)feedback, sizeof(feedback));
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

            if(keynum == Voice_Keynum && voice_send_state == true)
            {
                voice_send_state = false;
                swtimer_stop(vioce_send_timernum);
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }else if(keynum != Voice_Keynum)
            {
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }
        }
        else
        {
            set_key_press_state(false);
        }

        if(led_state == 0)
        {
            led_off(LED_1);
        }        
    }
    else if (key_pressed_num == 1)
    {
        keynum = 0;
        for(uint8_t i = 0; i < KEY_COL_NUM; i++) {
            keynum += key_report->keynum_report_buf[i];
        }

        factory_KeyProcess(keynum==Voice_Keynum?0xff:keynum);
        swtimer_start(key_press_time_timernum, 1000, TIMER_START_ONCE);
        // encrypt_state = 1;
        led_state = 0;
        DEBUG_LOG_STRING("KEY [%d][%d][%d][%d][%d][%d][%d]\r\n", key_report->keynum_report_buf[0]
        ,key_report->keynum_report_buf[1],key_report->keynum_report_buf[2],key_report->keynum_report_buf[3],key_report->keynum_report_buf[4],key_report->keynum_report_buf[5],key_report->keynum_report_buf[6]);
        if (bt_check_le_connected() && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }         
            DEBUG_LOG_STRING("att send  [%x] [%x] [%d] \r\n",KeyBuf[keynum].keyvalue[0],KeyBuf[keynum].keyvalue[1],KeyBuf[keynum].key_send_len);

            if (keynum == Voice_Keynum && !voice_key_state)
            {
                // update_conn_param(false);
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                voice_key_state = true;
                voice_send_state = true;
                voice_status.search = true;
                voice_status_change();
                app_sleep_lock_set(AUDIO_LOCK, true);
                led_state = true;
                swtimer_start(vioce_timernum, 30000, TIMER_START_ONCE);
                swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                led_on(LED_1,0,0);
                
            }
            else if(keynum != Voice_Keynum)
            {
                if(led_state == 0) {
                    led_state = 2;
                    led_on(LED_1,0,120);
                }
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                DEBUG_LOG_STRING("424  %d %d\r\n", HREADW(M0_LPM_REG),led_state);
            }
        }
        else
        {
            if(led_state == 0) {
                led_state = 1;
                led_on(LED_1,120,480);
            }
            if(keynum == Power_Keynum && Bt_CheckIsPaired())
            {
                struct bt_le_adv_param bt_adv_param;
                bt_adv_param.Type = ADV_TYPE_NOMAL;
                bt_adv_param.adv_max_interval = 0x10;
                IPC_TxControlCmd(BT_CMD_STOP_ADV);
                bt_set_le_state(BLE_IDLE);
                bt_set_le_state(BLE_ADV);
                flash_read(power_key_mac,(uint8_t *)&power_adv_data_buf,sizeof(power_adv_data_buf));
                power_adv_data_buf[25] += 1;
                flash_write(power_key_mac, (uint8_t *)&power_adv_data_buf, sizeof(power_adv_data_buf),CHUNK_INF);
                DEBUG_LOG_BT("gRecinfo.sPeerBDaddr = %x %x %x %x %x %x\r\n",power_adv_data_buf[15],power_adv_data_buf[16],power_adv_data_buf[17],power_adv_data_buf[18],power_adv_data_buf[19],power_adv_data_buf[20]);
                bt_start_le_adv(&bt_adv_param, power_adv_data_buf, sizeof(power_adv_data_buf));
                app_sleep_lock_set(ADV_LOCK, true); 
                app_sleep_timer_set(DIRECT_ADV_TIME);
            }
            else if(keynum == NETFLIX_Keynum && Bt_CheckIsPaired())
            {
                struct bt_le_adv_param bt_adv_param;
                bt_adv_param.Type = ADV_TYPE_NOMAL;
                bt_adv_param.adv_max_interval = 0x10;
                IPC_TxControlCmd(BT_CMD_STOP_ADV);
                bt_set_le_state(BLE_IDLE);
                bt_set_le_state(BLE_ADV);

                flash_read(netflix_key_mac,(uint8_t *)&Netflix_adv_data_buf,sizeof(Netflix_adv_data_buf));
                Netflix_adv_data_buf[25] += 1;
                flash_write(netflix_key_mac, (uint8_t *)&Netflix_adv_data_buf, sizeof(Netflix_adv_data_buf),CHUNK_INF);

                bt_start_le_adv(&bt_adv_param, Netflix_adv_data_buf, sizeof(Netflix_adv_data_buf));
                app_sleep_lock_set(ADV_LOCK, true); 
                app_sleep_timer_set(DIRECT_ADV_TIME);        
            }
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
        }
    }
    else if (key_pressed_num == 2)
    {
        key_pressed_time = 0;
        if (key_report->keynum_report_buf[OK_Col] == OK_Keynum && key_report->keynum_report_buf[Vol_Col] == Vol__Keynum)
        {
            if(!bt_check_le_connected()){
                keynum = OK_Keynum;
                keynum_second = Vol__Keynum;
                swtimer_start(key_pressed_timernum,1000,TIMER_START_ONCE);
            }
        }
        else if (key_report->keynum_report_buf[OK_Col] == OK_Keynum && key_report->keynum_report_buf[VolCol] == Vol_Keynum)
        {
            if(bt_check_le_connected()){
                keynum = OK_Keynum;
                keynum_second = Vol_Keynum;
                swtimer_start(key_pressed_timernum,1000,TIMER_START_ONCE);
            }
        }
        else if (key_report->keynum_report_buf[OK_Col] == OK_Keynum && key_report->keynum_report_buf[Back_Col] == Back__Keynum)
        {
            if(bt_check_le_connected()){
                keynum = OK_Keynum;
                keynum_second = Back__Keynum;
                swtimer_start(key_pressed_timernum,1000,TIMER_START_ONCE);
            }
        }        
    }   
}

void Action_After_Prepare_Sleep(void)
{
    if(key_pressed_num == 1 && bt_check_le_connected() && encrypt_state){
        key_wakeup_set_high();
    }
}

void action_after_mic_close(void)
{
    led_state = false;
    led_off(LED_1);
    voice_key_state = false;
}

void action_after_led_blk(void)
{
    DEBUG_LOG_STRING("514  %d \r\n", led_state);
    if (led_state == 1)
    {
        led_state = false;
    }
    else if(led_state == 2)
    {
        swtimer_stop(get_key_timernum());
        Lpm_unLockLpm(LPM_ALL_LOCK);
        app_sleep_lock_set(KEY_LOCK, false);
        app_sleep_lock_set(LATENCY_LOCK, true);
        DEBUG_LOG_STRING("521  %d \r\n", HREADW(M0_LPM_REG));
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
        bt_le_conn_updata_param_req(0x08, 0x08, CONN_PARAM, 500);
    }
    else {
        bt_le_conn_updata_param_req(0x08, 0x08, 0, 1);
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
        if(!memcmp(MIC_OPEN, data, 1) && voice_key_state == false) {
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
    set_key_press_state(false);
    update_conn_param(false);
    encrypt_state = true;
    dis_encrypt_state = false;
    // led_on(LED_1,200,1200);

}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
    memcpy((void *)&power_adv_data_buf[19],(void *)gRecinfo.sPeerBDaddr,6);
    memcpy((void *)&Netflix_adv_data_buf[19],(void *)gRecinfo.sPeerBDaddr,6);
    DEBUG_LOG_STRING("mac %x %x %x %x %x %x\r\n",gRecinfo.sPeerBDaddr[0]
    ,gRecinfo.sPeerBDaddr[1],gRecinfo.sPeerBDaddr[2],gRecinfo.sPeerBDaddr[3]
    ,gRecinfo.sPeerBDaddr[4],gRecinfo.sPeerBDaddr[5]);
    flash_write(power_key_mac, (uint8_t *)&power_adv_data_buf, sizeof(power_adv_data_buf),CHUNK_INF);
    flash_write(netflix_key_mac, (uint8_t *)&Netflix_adv_data_buf, sizeof(Netflix_adv_data_buf),CHUNK_INF);
    led_state = 1;
    led_on(LED_1,0,3000);
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
    
    if(voice_key_state){
        led_off(LED_NUM);
        voice_key_state = false;
        swtimer_stop(vioce_send_timernum);
        swtimer_stop(vioce_timernum);
    }    
    System_ChangeXtal24M();
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
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else {
        app_sleep_lock_set(ADV_LOCK, true);
        // start_adv(ADV_TYPE_NOMAL, 0x10,true);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    led_state =false;
    led_off(LED_1);

    bt_set_lpm_overhead_wake_time(0x10);
    bt_set_le_state(BLE_CONNECTED);
    bt_disable_le_tx_md();
	if(bt_check_save_connect_info()){
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
    else{
        app_sleep_lock_set(ADV_LOCK, true);
        app_sleep_timer_set(SHUTDOWN_TIME);
    }
    // else {
    //     start_adv(ADV_TYPE_NOMAL, 0x10, true);
    // }
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
        bt_set_ce_length_num(0x0F);//小包用  大包用0x08
        software_timer_start(SYSTEM_CURRENT_CLOCK, 10);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(UPD6121G2_68, CUSTOM_00_FF_A);
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        
        key_pressed_timernum = swtimer_add(key_pressed_handle);
        vioce_timernum = swtimer_add(mic_close);
        vioce_send_timernum = swtimer_add(voice_send_handle);
		low_power_timernum = swtimer_add(low_power_handle);
        key_press_time_timernum = swtimer_add(key_press_time_handle);
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

        if (key_wakeup_get() && keynum != 1)
        {
            sleep_time_state = 0;
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP %d\r\n" ,wake_up_state);

        }
        else if(key_wakeup_get_high() && keynum == 1){
            sleep_time_state = 0;
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP %d\r\n" ,wake_up_state);            
        }
        else 
        {
#ifdef SLEEP_ONE_HOUR
            sleep_time_state++;
            if(sleep_time_state > 3800){
                bt_send_le_disconnect(0x13);
            }
#endif            
            enter_low_sleep();
            DEBUG_LOG_STRING("KEEP CONNECT\r\n");
        }
    }
}
