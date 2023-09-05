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
#include "flash.h"
#include "keyscan.h"
#include "audio.h"

#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
enum
{
    Voice_Keynum = 2,
    Power_Keynum = 1,
    Mini_Keynum = 10,
    Mini_Col = 3,
    Home_Keynum = 14,
    Home_Col = 1,
    CONN_PARAM = 99,
};

typedef struct
{
    uint8_t keyvalue[3];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

static const uint8_t MIC_CLOSE_BUF[20] = {0x00, 0x00, 0x00, 0x80, 0x00};
static const uint8_t MIC_OPEN_BUF[20] = {0x00, 0x00, 0x00, 0x80, 0x01};
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0x00, 0, 0},

    {0x20, 0x00, 0x00, 3, 134}, // 1
    {0x40, 0x00, 0x00, 3, 134},  //
    {0x02, 0x00, 0x00, 3, 134},  //
    {0x01, 0x00, 0x00, 3, 134},  //
    {0x00, 0x00, 0x00, 0, 0}, // 

    {0x00, 0x00, 0x00, 0, 0}, // 6
    {0x08, 0x00, 0x00, 3, 134}, //
    {0x10, 0x00, 0x00, 3, 134},  //
    {0x04, 0x00, 0x00, 3, 134}, //
    {0x00, 0x02, 0x00, 3, 134}, //

    {0x00, 0x00, 0x00, 0, 0},  //11
    {0x00, 0x00, 0x00, 0, 0},  //12
    {0x00, 0x00, 0x08, 3, 134},  //13
    {0x00, 0x00, 0x04, 3, 134},  //
    {0x00, 0x04, 0x00, 3, 134}, //

    {0x00, 0x08, 0x00, 3, 134},
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},  

    {0x80, 0x00, 0x00, 3, 134},  
    {0x00, 0x01, 0x00, 3, 134},  


};

static const uint8_t scan_rsp_data_buf[] = 
{0x0A, 0x09, 0x58, 0x69, 0x61, 0x6F, 0x6D, 0x69, 0x20, 0x52, 0x43};
static const uint8_t adv_data_buf[] = 
{0x02 ,0x01 ,0x05 ,0x03 ,0xFF ,0x00 ,0x00 
,0x06 ,0x08 ,0x4D ,0x49 ,0x20 ,0x52 ,0x43 ,0x03 ,0x03 ,0x12 
,0x18 ,0x04 ,0x0D ,0x04 ,0x05 ,0x00 ,0x02 ,0x0A ,0x00 };//配对广播

// {
// 0x02, 0x01, 0x05, 0x03, 0xFF, 0x00, 0x01, 
// 0x06, 0x08, 0x4D, 0x49, 0x20, 0x52, 0x43, 0x03, 0x02, 0x12, 
// 0x18, 0x04, 0x0D, 0x04, 0x05, 0x00, 0x02, 0x0A, 0x00};

static const uint8_t power_adv_data_buf[] = 
{
0x02 ,0x01 ,0x05 ,0x03 ,0xFF ,0x00 ,0x01 
,0x06 ,0x08 ,0x4D ,0x49 ,0x20 ,0x52 ,0x43 ,0x03 ,0x03 ,0x12 ,0x18 ,0x04
,0x0D ,0x04 ,0x00 ,0x01 ,0x02 ,0x0A ,0x00 ,0x04 
,0xFE ,0xEC ,0x01 ,0x85};
// {
// 0x02, 0x01, 0x05, 0x03, 0xFF, 0x00, 0x01, 
// 0x06, 0x08, 0x4D, 0x49, 0x20, 0x52, 0x43, 0x03, 0x02, 0x12, 0x18, 0x04, 
// 0x0D, 0x04, 0x05, 0x00, 0x02, 0x0A, 0x00, 0x04, 0xFE, 0XEC, 0X01, 0X85};//开机广播
static uint8_t header_idx = 0;
static const uint8_t msbc_header_byte_table[] = {0x08, 0x38, 0xc8, 0xf8};
static const uint8_t mic_open_before_buf[20] = {0x01};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_third;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_second;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum;

static uint8_t key_pressed_timernum = 0xFF;
static uint8_t encrypt_report_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint8_t vioce_timernum = 0xFF;
static uint8_t adv_stop_timernum = 0xFF;
static bool encrypt_state;
static bool first_pair;
static bool tx_power_switch = true;
static uint32_t sleep_time_state;
static bool dis_encrypt_state;
static bool conn_param_state;
static uint8_t wake_up_state;
static bool mic_open_state;
static  uint8_t report_handle89_1_value[] = {0x27, 0x17, 0x32, 0xb9, 0xD3, 0x04, 0x38, 0x19, 0xa0, 0x20, 0x00, 0x00};
static bool SecretKey_Check(void)
{
    uint8_t adcbuf[12];
    uint8_t secretkey_Ori[16];
    uint32_t secretkey_SN_Addr;
    uint8_t secretkey_SN[2];
    PublicKey_TypeE secretkey_Type;
    uint8_t secretkey_Gen[16];

    for (uint8_t i = 0; i < 12; i++)
    {
        adcbuf[i] = HREAD(mem_0_3_6v_adc_io_data + i);
    }

    DEBUG_LOG_STRING("secretkey_Ori:\r\n");
    QSPI_ReadFlashData(SecretKey_Addr, 16, secretkey_Ori);
    QSPI_ReadFlashData(0, 3, (uint8_t *)&secretkey_SN_Addr);
    secretkey_SN_Addr = REVERSE_3BYTE_DEFINE(secretkey_SN_Addr) - 4;
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

static void low_power_handle(void)
{
    if (HREADW(mem_le_slave_latency) != CONN_PARAM && bt_check_le_connected())
    {
        if (key_pressed_num == 0)
        {
            sleep_time_state++;
            if (sleep_time_state == 600)
            {
                bt_send_le_disconnect(0x13);
                sleep_time_state = 0;
            }
        }
        else
        {
            sleep_time_state = 0;
        }
        DEBUG_LOG_STRING("low_power_handle : %d \r\n", sleep_time_state);
    }
    else
    {
        sleep_time_state = 0;
        swtimer_stop(low_power_timernum);
    }
}

static void stop_adv(void)
{
    if (bt_check_le_connected())
        return;
    else
    {
        if (bt_get_le_state() == BLE_DIRECT_ADV)
        {
            IPC_TxControlCmd(BT_CMD_STOP_DIRECT_ADV);
        }
        else
        {
            IPC_TxControlCmd(BT_CMD_STOP_ADV);
        }

        if (Bt_CheckIsPaired())
        {
            bt_set_le_state(BLE_PAIRED);
        }
        else
        {
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
        if (bt_get_le_state() == BLE_DIRECT_ADV)
        {
            stop_adv();
        }
        bt_set_le_state(BLE_ADV);
        bt_start_le_adv(&bt_adv_param, adv_data_buf, sizeof(adv_data_buf));
        if (timeout)
        {
            app_sleep_lock_set(ADV_LOCK, true);
            app_sleep_timer_set(SHUTDOWN_TIME);
        }
    }
    else if (bt_adv_param.Type == ADV_TYPE_DIRECT)
    {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        if (timeout)
        {
            app_sleep_lock_set(ADV_LOCK, true);
            app_sleep_timer_set(DIRECT_ADV_TIME);
        }
    }
}

static void adv_stop_handle(void){
    start_adv(ADV_TYPE_NOMAL, 0x10, true);
}
static void key_pressed_handle(void)
{
    bool le_connected_state = bt_check_le_connected();

    if (key_pressed_num == 0 || key_pressed_num == 1)
    {
    }
    else if (key_pressed_num == 2)
    {
        if (keynum == Home_Keynum && keynum_second == Mini_Keynum)
        {
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                keynum_second = 0;
                key_pressed_time = 0;
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                IPC_TxControlCmd(BT_CMD_STOP_ADV);
                bt_set_le_state(BLE_IDLE);
                start_adv(ADV_TYPE_NOMAL, 0x10,true);
                return ;
            }else{
                swtimer_restart(key_pressed_timernum);
            }
        }
    }
    else if (key_pressed_num == 3)
    {
    }
}

void update_voice_packet(uint8_t *pkt)
{
    pkt[0] = 0x01;
    pkt[1] = msbc_header_byte_table[header_idx++ % 4];
    pkt[ENCODE_OUTPUT_SIZE - 1] = 0x00;
}

void action_after_mic_close(void)
{
    mic_open_state = false;
    uint8_t zero[20];
    memset(zero, 0, sizeof(zero));
    ATT_sendNotify(AUDIO_MIC_OPEN_BEFORE_HANDLE, zero, sizeof(zero));
}

static void encrypt_handle(void)
{
    if (bt_check_le_connected())
    {
        if(keynum != Power_Keynum)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            if (keynum == Voice_Keynum)
            {
                ATT_sendNotify(AUDIO_MIC_OPEN_BEFORE_HANDLE, (uint8_t *)mic_open_before_buf, sizeof(mic_open_before_buf));
            }
            memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void *)hid_send_buf, (void *)KeyBuf[keynum].keyvalue, 3);
            ATT_sendNotify(KeyBuf[keynum].handle, hid_send_buf, KeyBuf[keynum].key_send_len);
            if (keynum == Voice_Keynum) {
                mic_open_state = true;
                mic_open();
            }

            if (key_pressed_num == 0)
            {
                memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                ATT_sendNotify(KeyBuf[keynum].handle, (void *)hid_send_buf, KeyBuf[keynum].key_send_len);

                if (keynum == 2)
                {
                    mic_close();
                    led_on(LED_3, 200, 1200);
                }
            }
        }
    }

    encrypt_state = true;
}

static void keyvalue_handle(key_report_t *key_report)
{
    bool le_connected_state = bt_check_le_connected();
    key_pressed_num = key_report->key_press_cnt;
    DEBUG_LOG_STRING("KEYNUM [%d][%d][%d][%d][%d][%d] \r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1], key_report->keynum_report_buf[2],
                     key_report->keynum_report_buf[3], key_report->keynum_report_buf[4], key_report->keynum_report_buf[5]);
    if (key_pressed_num == 0)
    {
        if (le_connected_state && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];

            memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            ATT_sendNotify(KeyBuf[keynum].handle, hid_send_buf, KeyBuf[keynum].key_send_len);
            if (wake_up_state != 0)
            {
                wake_up_state = 0;
            }
        }

        set_key_press_state(false);
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] +
                 key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] +
                 key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];

        if (le_connected_state && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            if (keynum == Voice_Keynum)
            {
                ATT_sendNotify(AUDIO_MIC_OPEN_BEFORE_HANDLE, (uint8_t *)mic_open_before_buf, sizeof(mic_open_before_buf));
            }
            memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void *)hid_send_buf, (void *)KeyBuf[keynum].keyvalue, 3);
            ATT_sendNotify(KeyBuf[keynum].handle, hid_send_buf, KeyBuf[keynum].key_send_len);
            if (keynum == Voice_Keynum){
                mic_open_state = true;
                mic_open();
            }
        }
        else if (!le_connected_state && keynum == Power_Keynum)
        {
            struct bt_le_adv_param bt_adv_param;
            bt_adv_param.Type = ADV_TYPE_NOMAL;
            bt_adv_param.adv_max_interval = 0x10;
            IPC_TxControlCmd(BT_CMD_STOP_ADV);
            bt_set_le_state(BLE_IDLE);
            bt_set_le_state(BLE_ADV);
            bt_start_le_adv(&bt_adv_param, power_adv_data_buf, sizeof(power_adv_data_buf));
            app_sleep_lock_set(ADV_LOCK, true); 
            app_sleep_timer_set(DIRECT_ADV_TIME);
        }
    }
    else if (key_pressed_num == 2)
    {
        if (key_report->keynum_report_buf[Home_Col] == Home_Keynum && key_report->keynum_report_buf[Mini_Col] == Mini_Keynum && !le_connected_state)
        {    
            DEBUG_LOG_STRING("S111 \r\n");
            key_pressed_time =  0;
            keynum = Home_Keynum;
            keynum_second = Mini_Keynum;
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

    if (batlevel <= 20 && tx_power_switch)
    {
        tx_power_switch = false;
        bt_set_tx_power(TX_POWER_5DB);
    }

    if (batlevel <= 5)
    {
        if (bt_check_le_connected())
        {
            ATT_sendNotify(BAT_REPORT_HANDLE, &batlevel, 1);
        }
    }
    else
    {
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
    if (is_sleep)
    {
        if (!conn_param_state)
        {
            conn_param_state = true;
            swtimer_start(low_power_timernum, 100, TIMER_START_REPEAT);
        }
        bt_le_conn_updata_param_req(0x0A, 0x0A, CONN_PARAM, 1500);
    }
    else
    {
        bt_le_conn_updata_param_req(0x0A, 0x0A, 0, 1500);
    }
}

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d\r\n", HREADW(mem_le_slave_latency));
    if (HREADW(mem_le_slave_latency) == CONN_PARAM || HREADW(mem_le_slave_latency) == 49)
    {
        swtimer_stop(low_power_timernum);
        if (!conn_param_state)
        {
            conn_param_state = true;
        }
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
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
    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);
    uint8_t senddata[251] = {ATTOP_READ_RESPONSE};
    uint16_t sendlen = (table->dataLen >= (HREADW(mem_mtu_size)) - 1) ? ((HREADW(mem_mtu_size)) - 1) : table->dataLen;
    if (table->handle == 89 && !first_pair )
    {
        memcpy((void *)&senddata[1], report_handle89_1_value, sizeof(report_handle89_1_value));
        ATT_Send(senddata, sendlen + 1);
    }else
    {
        memcpy((void *)&senddata[1], table->dataPtr, sendlen);
        ATT_Send(senddata, sendlen + 1);
    }

}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);

    if (table->handle == AUDIO_CTOL_HANDLE)
    {
        if (!memcmp(MIC_OPEN_BUF, data, sizeof(MIC_OPEN_BUF)))
        {
            if(!mic_open_state) {
                mic_open();
            }
        }
        else if (!memcmp(MIC_CLOSE_BUF, data, sizeof(MIC_CLOSE_BUF)))
        {
            mic_close();
        }
    }
    else if (table->handle == OTA_WRITE_HANDLE)
    {
        if (app_sleep_check())
        {
            software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        }
        app_sleep_lock_set(APP_LOCK, true);
        app_sleep_lock_set(ADV_LOCK, false);
        OS_ENTER_CRITICAL();
        if (OTA_EVT_HANDLE(len, data) != OTA_NO_ERROR)
        {
            OTA_RESET();
            app_sleep_lock_set(APP_LOCK, false);
        }
        OS_EXIT_CRITICAL();
    }
    else if (table->dataLen >= len)
    {
        memcpy((void *)table->dataPtr, (void *)data, len);
    }
    else
    {
        DEBUG_LOG_STRING("ATT WRITE DATA ERROR, HANDLE: %d, WRITE LEN: %d \r\n", table->handle, len);
    }
}

void LE_LTK_LOST(void)
{
    DEBUG_LOG_BT("BT_EVT_LE_LTK_LOST \r\n");
    if (bt_check_le_connected())
    {
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
    dis_encrypt_state = false;
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    first_pair = true;

    app_sleep_timer_set(PAIR_DONE_DELAY);
    led_off(LED_NUM);
    // led_on(LED_3, 200, 1200);
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

    if (reason == 0x13 || reason == 0x16 || dis_encrypt_state)
    {
        DEBUG_LOG_STRING("dis_encrypt_state %d \r\n", dis_encrypt_state);
        enter_deep_sleep();
    return;
    }
    else if (!dis_encrypt_state && Bt_CheckIsPaired())
    {
        DEBUG_LOG_STRING("disconnect dis_encrypt_state %d \r\n", dis_encrypt_state);
        dis_encrypt_state = true;
    }
    else if (reason == 0x16)
    {
        Bt_SndCmdPwroff();
    }

    app_queue_reset();
    remote_control_reinit();

    if (Bt_CheckIsPaired())
    {
        start_adv(ADV_TYPE_DIRECT, 0x10, true);
    }
    else
    {
        start_adv(ADV_TYPE_NOMAL, 0x30, true);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_lpm_overhead_wake_time(0x10);
    bt_set_le_state(BLE_CONNECTED);
    bt_disable_le_tx_md();

    if (bt_check_save_connect_info())
    {
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

    if (Bt_CheckIsPaired())
    {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else
    {
        start_adv(ADV_TYPE_NOMAL, 0x10, true);
        app_sleep_lock_set(ADV_LOCK, true);
        app_sleep_timer_set(DIRECT_ADV_TIME);
    }
}

void tx_power_switch_set(bool switch_enable)
{
    tx_power_switch = switch_enable;
}

void app_task_run(void)
{
    if (tx_power_switch)
    {
        bt_dynamic_switch_power();
    }
    app_queue_task();
}

void app_init(void)
{
    if (!Lpm_GetWakeFlag())
    {

        bt_set_ce_length_num(0x08);
        bt_set_le_mtu_size(251);
        bt_update_dle();
        bt_update_mtu();
        bt_update_slave_feature();
        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);

        low_power_timernum = swtimer_add(low_power_handle);
        encrypt_report_timernum = swtimer_add(encrypt_handle);
        key_pressed_timernum = swtimer_add(key_pressed_handle);
        vioce_timernum = swtimer_add(mic_close);
        adv_stop_timernum = swtimer_add(adv_stop_handle);
        if (!SecretKey_Check())
        {
        #ifdef SecretKey_Check_enable
                    DEBUG_LOG_STRING("SecretKey_Check fail \r\n");
        #ifdef FUNCTION_WATCH_DOG
                    IWDG_Disable(WDT);  // bt watch dog
                    IWDG_Disable(WDT2); // riscv watch dog
        #endif
                    while (1)
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
            if (wake_up_state == 100)
            {
                prepare_before_sleep();
                enter_deep_sleep();
            }
            sleep_time_state = 0;
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP %d\r\n", wake_up_state);
        }
        else
        {
#ifdef SLEEP_ONE_HOUR
            sleep_time_state++;
            if (sleep_time_state > 3800)
            {
                bt_send_le_disconnect(0x13);
            }
#endif
            enter_low_sleep();
            DEBUG_LOG_STRING("keep connect \r\n");
        }
    }
}
