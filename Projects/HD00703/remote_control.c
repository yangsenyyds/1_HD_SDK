#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_bt_interface.h"
#include "hci_dtm_test.h"
#include "yc_ota.h"
#include "att.h"
#include "yc_debug.h"
#include "app_config.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "software_timer.h"
#include "vbat.h"
#include "led.h"
#include "ir_send.h"
#include "ir_load.h"
#include "ir_lib.h"
#include "flash.h"
#include "keyscan.h"
#include "audio.h"


#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
enum{
#if (Project_key == 703 )//560
    Voice_Col = 2,
    Voice_Keynum = 21,
#elif (Project_key == 704)//440
    Voice_Col = 3,
    Voice_Keynum = 10,
#elif (Project_key == 702)//v570
    Voice_Col = 2,
    Voice_Keynum = 21,
#endif
    Mute_Col = 1,
    Mute_Keynum = 26,
    CONN_PARAM = 100,
};

typedef struct {
    uint8_t keyvalue[1];
    uint8_t key_send_len;
    uint16_t handle;
}KeyBuf_TypeDef;
#if (Project_key == 703)//560
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0},

    {0x2F, 8, 47},//1
    {0x66, 8, 47},
    {0x68, 8, 47},
    {0xEB, 8, 47},
    {0xEA, 8, 47},

    {0x00, 8, 47},//6
    {0x69, 8, 47},
    {0x6A, 8, 47},
    {0x6B, 8, 47},
    {0x4A, 8, 47},   

    {0xF7, 8, 47}, //11
    {0x00, 8, 47}, 
    {0x41, 8, 47},
    {0x52, 8, 47},//netflix
    {0x50, 8, 47}, 

    {0x28, 8, 47},//16
    {0x4F, 8, 47},
    {0x00, 8, 47},
    {0x51, 8, 47},
    {0x29, 8, 47},

    {0xF0, 8, 47},//21
    {0x39, 8, 47},
    {0x80, 8, 47},
    {0x00, 8, 47},
    {0x80, 8, 47},

    {0x7F, 8, 47},//26
    {0x81, 8, 47},
};
static const uint8_t ir_data[] = {
    0x00, // 0

    0x2F,//1
    0x08,
    0x51,
    0xEB,
    0xEA,

    0x00,//6
    0x1F,
    0xF8,
    0x5A,
    0X2D,

    0XF7,//11
    0X00,
    0X4F,
    0X45,
    0X47,

    0X44,//16
    0X48,
    0X00,
    0X46,
    0X4A,

    0X40,//21
    0X39,
    0X02,
    0X00,
    0X02,

    0X09,//26
    0X03,
    
};
static uint8_t scan_rsp_data_buf[] = {0x03, 0x19, 0x80, 0x01};
static const uint8_t adv_data_buf[] = {
 0x02 ,0x01 ,0x05 ,0x03 ,0x03 ,0x12 ,0x18 ,0x03 ,0x19 
,0x80 ,0x01 ,0x0B ,0x09 ,0x58 ,0x52 ,0x54 ,0x32 ,0x36 
,0x30 ,0x76 ,0x35 ,0x36 ,0x30 ,0x05 ,0xFF ,0x5D ,0x00 
,0x04 ,0x00
};
#elif (Project_key == 704)//v440
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0},

    {0x2F, 8, 47},//1
    {0x66, 8, 47},
    {0xEC, 8, 47},
    {0xEB, 8, 47},
    {0xEA, 8, 47},

    {0x00, 8, 47},//6
    {0x6F, 8, 47},
    {0xF9, 8, 47},
    {0x42, 8, 47},
    {0xF0, 8, 47},//VOICE

    {0xF7, 8, 47}, //11
    {0x00, 8, 47}, 
    {0x41, 8, 47},
    {0x52, 8, 47},//netflix
    {0x50, 8, 47}, 

    {0x28, 8, 47},//16
    {0x4F, 8, 47},
    {0x00, 8, 47},
    {0x51, 8, 47},
    {0x29, 8, 47},

    {0x4A, 8, 47},//21 HOME
    {0x39, 8, 47},
    {0x80, 8, 47},
    {0x00, 8, 47},
    {0x80, 8, 47},

    {0x7F, 8, 47},//26
    {0x81, 8, 47},
};
static const uint8_t ir_data[] = {
    0x00, // 0

    0x2F,//1
    0x08,
    0xEC,
    0xEB,
    0xEA,

    0x00,//6
    0x6F,
    0xF9,
    0x42,
    0X40,//VOICE

    0XF7,//11
    0X00,
    0X4F,
    0X45,
    0X47,

    0X44,//16
    0X48,
    0X00,
    0X46,
    0X4A,

    0X2D,//21
    0X39,
    0X02,
    0X00,
    0X02,

    0X09,//26
    0X03,
    
};
static uint8_t scan_rsp_data_buf[] = {0x03, 0x19, 0x80, 0x01};
static const uint8_t adv_data_buf[] = {
 0x02 ,0x01 ,0x05 ,0x03 ,0x03 ,0x12 ,0x18 ,0x03 ,0x19 
,0x80 ,0x01 ,0x0B ,0x09 ,0x58 ,0x52 ,0x54 ,0x32 ,0x36 
,0x30 ,0x76 ,0x34 ,0x34 ,0x30 ,0x05 ,0xFF ,0x5D ,0x00 
,0x04 ,0x00
};
#elif (Project_key == 702)//v570
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0},

    {0x2F, 8, 47},//1
    {0x66, 8, 47},
    {0x68, 8, 47},
    {0xEB, 8, 47},
    {0xEA, 8, 47},

    {0x00, 8, 47},//6
    {0x69, 8, 47},
    {0x6A, 8, 47},
    {0x6B, 8, 47},
    {0x6F, 8, 47},   

    {0xF9, 8, 47}, //11
    {0x00, 8, 47}, 
    {0x42, 8, 47},
    {0x52, 8, 47},//netflix
    {0x50, 8, 47}, 

    {0x28, 8, 47},//16
    {0x4F, 8, 47},
    {0x00, 8, 47},
    {0x51, 8, 47},
    {0x29, 8, 47},

    {0xF0, 8, 47},//21
    {0x39, 8, 47},
    {0x4A, 8, 47},
    {0x00, 8, 47},
    {0x80, 8, 47},

    {0x7F, 8, 47},//26
    {0x81, 8, 47},
    {0xF7, 8, 47},
    {0x41, 8, 47},
};
static const uint8_t ir_data[] = {
    0x00, // 0

    0x2F,//1
    0x08,
    0x51,
    0xEB,
    0xEA,

    0x00,//6
    0x1F,
    0xF8,
    0x5A,
    0X6F,//VOICE

    0XF9,//11
    0X00,
    0X42,
    0X45,
    0X47,

    0X44,//16
    0X48,
    0X00,
    0X46,
    0X4A,

    0X40,//21
    0X39,
    0X2D,
    0X00,
    0X02,

    0X09,//26
    0X03,
    0XF7,
    0X4F,
};
static uint8_t scan_rsp_data_buf[] = {0x03, 0x19, 0x80, 0x01};
static const uint8_t adv_data_buf[] = {
 0x02 ,0x01 ,0x05 ,0x03 ,0x03 ,0x12 ,0x18 ,0x03 ,0x19 
,0x80 ,0x01 ,0x0B ,0x09 ,0x58 ,0x52 ,0x54 ,0x32 ,0x36 
,0x30 ,0x76 ,0x35 ,0x37 ,0x30 ,0x05 ,0xFF ,0x5D ,0x00 
,0x04 ,0x00
};
#endif


MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_second;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum;

static bool tx_freq_flag;
static bool adv_flag;
static bool ir_learn_flag;

static uint8_t ota_cnt;
static uint8_t ota_buf[2];

static uint8_t key_pressed_timernum = 0xFF;
static uint8_t encrypt_report_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint8_t led_state;
static bool encrypt_state;
static bool tx_power_switch = true;
static uint32_t sleep_time_state;
static bool dis_encrypt_state;
static bool conn_param_state;
static uint8_t wake_up_state;
uint16_t first_packet_cut_cnt = 0;
static const uint8_t msbc_header_h11_byte_1_table[] = { 0x08, 0xc8 };
static const uint8_t msbc_header_h21_byte_1_table[] = { 0x38, 0xf8 };
static uint8_t header_idx = 0;

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

static void stop_adv(void)
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

static void start_adv(enum advType type, uint16_t adv_interval, bool timeout)
{
    struct bt_le_adv_param bt_adv_param;
    bt_adv_param.Type = type;
    bt_adv_param.adv_max_interval = adv_interval;

    if (bt_adv_param.Type == ADV_TYPE_NOMAL)
    {
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
    else if (bt_adv_param.Type == ADV_TYPE_DIRECT)
    {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        if (timeout) {
            app_sleep_lock_set(ADV_LOCK, true);
            app_sleep_timer_set(DIRECT_ADV_TIME);
        }
    }
}

void update_voice_packet(uint8_t *pkt)
{
        if(header_idx > 1)
        {
            header_idx = 0;
        }
        pkt[0] = 0x01;
        pkt[1] = msbc_header_h11_byte_1_table[header_idx];
        pkt[59] = 0x00;
        pkt[60] = 0x01;
        pkt[61] = msbc_header_h21_byte_1_table[header_idx++];
        pkt[119] = 0x00;
}

static void key_pressed_handle(void)
{
    bool le_connected_state = bt_check_le_connected();
    if (keynum == Voice_Keynum && keynum_second == Mute_Keynum)
    {
        if(key_pressed_num == 2){
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                keynum_second = 0;
                key_pressed_time = 0;
                led_state = true;
                led_on(LED_1,200,60000);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10, true);
                return ;
            }else{
                swtimer_restart(key_pressed_timernum);
            }
        }
        else{
            keynum_second = 0;
            key_pressed_time = 0;
        }
    }
    else if (key_pressed_num == 1 || key_pressed_num == 0)
    {
        ir_comm_send(ir_data[keynum]);

        if(key_pressed_num == 1) {
            set_key_press_state(true);
        }
    }
}

static void keyvalue_handle(key_report_t *key_report)
{
    bool le_connected_state = bt_check_le_connected();
    key_pressed_num = key_report->key_press_cnt;
    DEBUG_LOG_STRING("KEYNUM [%d][%d][%d][%d][%d][%d] \r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1], key_report->keynum_report_buf[2],
    key_report->keynum_report_buf[3], key_report->keynum_report_buf[4], key_report->keynum_report_buf[5]);

    if (key_pressed_num == 0)
    {
        if (le_connected_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }
            ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            if(wake_up_state != 0) {
                wake_up_state = 0;
            }
		}else{
            set_key_press_state(false);
        }
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] +
                 key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] +
                 key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];
        if(le_connected_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 1);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }
            if(keynum == Voice_Keynum) {
                if(!led_state) {
                    led_on(LED_1,100,0);
                }
            }
            ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
        }        
        else {
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
        }
    }
    else if (key_pressed_num == 2)
	{
        if (key_report->keynum_report_buf[Voice_Col] == Voice_Keynum && key_report->keynum_report_buf[Mute_Col] == Mute_Keynum)
        {
            led_off(LED_1);
            set_key_press_state(false);
            keynum = Voice_Keynum;
            keynum_second = Mute_Keynum;
            swtimer_start(key_pressed_timernum,1000,TIMER_START_ONCE);
        }        
    }
    else if (key_pressed_num == 3)
    {
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

void action_after_led_blk(void)
{
}

void action_after_mic_close(void)
{
    led_off(LED_1);
}

void update_conn_param(bool is_sleep)
{
    if (is_sleep) {
        if(!conn_param_state) {
            conn_param_state = true;
            swtimer_start(low_power_timernum, 100, TIMER_START_REPEAT);
        }   
        bt_le_conn_updata_param_req(0x0c, 0x0c, CONN_PARAM, 500);
    }
    else {
        bt_le_conn_updata_param_req(0x0c, 0x0c, 0, 500);
    } 
}

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if(HREADW(mem_le_slave_latency) == CONN_PARAM) {
        swtimer_stop(low_power_timernum);
        if(!conn_param_state) {
            conn_param_state = true;
        }        
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
    }
}

void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset)
{
    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);
    uint8_t senddata[251] = {ATTOP_READ_BLOB_RESPONSE};

    uint16_t sendlen = ((table->dataLen - offset) >= ((HREADW(mem_mtu_size)) - 1)) ? ((HREADW(mem_mtu_size)) - 1) : (table->dataLen - offset);

    memcpy(senddata + 1, table->dataPtr + offset, sendlen);
    ATT_Send(senddata, sendlen + 1);
    
}

void Read_Parse(const ATT_TABLE_TYPE *table)
{
    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);
    uint8_t senddata[251] = {ATTOP_READ_RESPONSE};
    
    uint16_t sendlen = (table->dataLen >= (HREADW(mem_mtu_size)) - 1)? ((HREADW(mem_mtu_size)) - 1) : table->dataLen;

    memcpy((void *)&senddata[1], table->dataPtr, sendlen);
    ATT_Send(senddata, sendlen+1);
    
}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);
    if(table->handle == AUDIO_CTOL_HANDLE){
        if(data[0] == 0x01){
            mic_open();
        }else if(data[0] == 0x00){
            mic_close();
        }
        
    }
    else if (table->handle == OTA_WRITE_HANDLE)
    {
        if (app_sleep_check()) {
            software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        }
        app_sleep_lock_set(APP_LOCK, true);
        app_sleep_lock_set(ADV_LOCK, false);
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

void ENCRYPT_DONE(void)
{
    DEBUG_LOG_STRING("ENCRYPT_DONE \r\n");
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
    swtimer_start(encrypt_report_timernum, 500, TIMER_START_ONCE);
    update_conn_param(false);
    dis_encrypt_state = false;

}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
    led_off(LED_NUM);
    led_on(LED_1, 200, 1200);

}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);
    encrypt_state = false;
    conn_param_state = false;
    bt_set_tx_power(TX_POWER_5DB);
    bt_set_le_state(BLE_IDLE);
    app_sleep_lock_set(LATENCY_LOCK, false);
    System_ChangeXtal24M();
    Bt_HciFifo_Init();
  
    if (reason == 0x13 || reason == 0x16 || dis_encrypt_state) {
        DEBUG_LOG_STRING("dis_encrypt_state %d \r\n",dis_encrypt_state);
        enter_deep_sleep();
        return;
    }
    else if(!dis_encrypt_state && Bt_CheckIsPaired()) {
        DEBUG_LOG_STRING("disconnect dis_encrypt_state %d \r\n",dis_encrypt_state);
        dis_encrypt_state = true;
    }
    else if(reason == 0x16) {
        Bt_SndCmdPwroff();
    }
    
    app_queue_reset();
    remote_control_reinit();
    
    if(Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x10, true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x30, true);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_lpm_overhead_wake_time(0x10);
    bt_set_le_state(BLE_CONNECTED);
    bt_disable_le_tx_md();

    led_off(LED_2);
    adv_flag = false;
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

    if(!Lpm_GetWakeFlag())
    {
        bt_renew_scan_rsp((void *)scan_rsp_data_buf, sizeof(scan_rsp_data_buf));
    }

    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x10, true);
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
        bt_set_ce_length_num(0x08);
        if(Bt_CheckIsPaired()) {
            bt_update_mtu();
         }
        bt_set_auth_for_security_req(AUTH_BOND); /// BONING
        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(UPD6121G2_68, CUSTOM_04_FB_A);
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        low_power_timernum = swtimer_add(low_power_handle);
        key_pressed_timernum = swtimer_add(key_pressed_handle);
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
