#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include <string.h>
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
    Voice_Keynum = 5,
    Power_Keynum = 1,
    Mini_Keynum = 33,
    Mini_Col = 2,
    Home_Keynum = 17,
    Home_Col = 4,
    CONN_PARAM = 99,
};

typedef struct
{
    uint8_t keyvalue[3];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;
static bool voice_key_state;
static bool voice_send_state;
static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};

static const uint8_t MIC_CLOSE_BUF[20] = {0x00, 0x00, 0x00, 0x80, 0x00};
static const uint8_t MIC_OPEN_BUF[20] = {0x00, 0x00, 0x00, 0x80, 0x01};
#if (Project_key == 804)
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0x00, 0, 0},

    {0x20, 0x00, 0x00, 3, 134},// 1 power
    {0x00, 0x80, 0x00, 3, 134}, //disnep 
    {0x01, 0x00, 0x00, 3, 134}, //ok
    {0x02, 0x00, 0x00, 3, 134},//up
    {0x40, 0x00, 0x00, 3, 134}, // voice

    {0x00, 0x02, 0x00, 3, 134}, //disnep 
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},
    {0x00, 0x00, 0x00, 0, 0}, // 

    {0x00, 0x00, 0x00, 0, 0},//11
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x08, 3, 134},//back
    {0x00, 0x00, 0x00, 0, 0},
    {0x00, 0x40, 0x00, 3, 134},//prime vide

    {0x00, 0x04, 0x00, 3, 134},//netflix
    {0x00, 0x00, 0x04, 3, 134},//back
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},
    {0x00, 0x00, 0x00, 0, 0}, //  

    {0x00, 0x01, 0x00, 3, 134},//vol-
    {0x80, 0x00, 0x00, 3, 134},//vol+
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},
    {0x00, 0x00, 0x00, 0, 0}, // 

    {0x00, 0x00, 0x00, 0, 0},//26
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},
    {0x00, 0x00, 0x00, 0, 0}, //

    {0x08, 0x00, 0x00, 3, 134},//31zuo
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x40, 3, 134},//you
    {0x04, 0x00, 0x00, 3, 134},//you
    {0x10, 0x00, 0x00, 3, 134},//you

    {0x00, 0x00, 0x00, 0, 0},//36
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},  
    {0x00, 0x00, 0x00, 0, 0},
    {0x00, 0x00, 0x00, 0, 0}, //      
};
#endif
static const uint8_t scan_rsp_data_buf[] = 
{0x0A, 0x09, 0x58, 0x69, 0x61, 0x6F, 0x6D, 0x69, 0x20, 0x52, 0x43};
static const uint8_t adv_data_buf[] = 
{0x02 ,0x01 ,0x05 ,0x03 ,0xFF ,0x00 ,0x00 
,0x06 ,0x08 ,0x4D ,0x49 ,0x20 ,0x52 ,0x43 ,0x03 ,0x02 ,0x12 
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
static const uint8_t power_adcpcm_adv_data_buf[] = 
{
0x02 ,0x01 ,0x05 ,0x03 ,0xFF ,0x00 ,0x01 
,0x06 ,0x08 ,0x4D ,0x49 ,0x20 ,0x52 ,0x43 ,0x03 ,0x02 ,0x12 ,0x18 ,0x04
,0x0D ,0x04 ,0x05 ,0x00 ,0x02 ,0x0A ,0x00 ,0x04 
,0xFE ,0xEe ,0xd2 ,0x5f};
static const uint8_t power_adcpcm_bangzi_adv_data_buf[] = 
{
0x02 ,0x01 ,0x05 ,0x03 ,0xFF ,0x00 ,0x01 
,0x06 ,0x08 ,0x4D ,0x49 ,0x20 ,0x52 ,0x43 ,0x03 ,0x02 ,0x12 ,0x18 ,0x04
,0x0D ,0x04 ,0x05 ,0x00 ,0x02 ,0x0A ,0x00 ,0x04 
,0xFE ,0xEe ,0x6C,0XCB};
static uint8_t header_idx = 0;
static const uint8_t msbc_header_byte_table[] = {0x08, 0x38, 0xc8, 0xf8};
static const uint8_t mic_open_before_buf[20] = {0x01};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_third;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_second;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum;
const uint8_t product_key_s[] = {22,2,60};
static uint8_t key_pressed_timernum = 0xFF;
static uint8_t encrypt_report_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint8_t vioce_timernum = 0xFF;
static uint8_t adv_start_timernum = 0xFF;
static uint8_t vioce_send_timernum = 0xFF;
static uint8_t sleep_mirc4_timernum = 0xFF;
static bool encrypt_state;
static bool first_pair;
static bool tx_power_switch = true;
static uint32_t sleep_time_state;
static bool dis_encrypt_state;
static bool conn_param_state;
static uint8_t wake_up_state;
static bool mic_open_state;
static uint8_t List_state;
static  uint8_t  E_mibox_mi4;
static bool adv_state;
static  uint8_t report_handle89_1_adcpcm_value[] = {0x27, 0x17, 0x32, 0xb9, 0xD3, 0x04, 0x39, 0x19, 0xa0, 0x20, 0x00, 0x00};
static  uint8_t report_handle92_1_adcpcm_value[] = {0x00,0x00,0x00,0x00,0x00,0x20};
static  uint8_t report_handle98_1_adcpcm_value[] = {0x02};

uint8_t get_List_state(void){
    return List_state;
}

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

static void adv_start_handle(void){
    if(bt_check_le_connected()){
        IPC_TxControlCmd(BT_CMD_STOP_ADV);
    }
    else
    {
        if(List_state == ADCPCM_MODE)
        {
            struct bt_le_adv_param bt_adv_param;
            bt_adv_param.Type = ADV_TYPE_NOMAL;
            bt_adv_param.adv_max_interval = 0x10;
            IPC_TxControlCmd(BT_CMD_STOP_ADV);
            bt_set_le_state(BLE_ADV);
                if(adv_state){
                    adv_state = false;
                    bt_start_le_adv(&bt_adv_param,power_adcpcm_bangzi_adv_data_buf, sizeof(power_adcpcm_adv_data_buf));
                }
                else{
                    adv_state = true;
                    bt_start_le_adv(&bt_adv_param,power_adcpcm_adv_data_buf, sizeof(power_adcpcm_adv_data_buf));
                }
            swtimer_restart(adv_start_timernum);
        }
    }
    
}

static void voice_send_handle(void)
{
    if(key_pressed_num == 1 && voice_send_state == true) {
        voice_send_state = false;
        uint8_t hid_send_buf[3];
        memset(hid_send_buf,0,sizeof(hid_send_buf));
        ATT_sendNotify(ADCPCM_KEYNUM_HANDLE, (void*)hid_send_buf, KeyBuf[Voice_Keynum].key_send_len);
    }
}

static void sleep_mirc_handle(void)
{
    sleep_time_state++;
    if(sleep_time_state == 75) {
        swtimer_stop(sleep_mirc4_timernum);
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
    }
    DEBUG_LOG_STRING("sleep_mirc_handle = %d\r\n",sleep_time_state);
}

static void key_pressed_handle(void)
{
    bool le_connected_state = bt_check_le_connected();

    if (key_pressed_num == 2)
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
                List_state = 0;
                flash_Erase(device_tag,sizeof(addr_inf_t));

                if(E_mibox_mi4) {
                    E_mibox_mi4 = 0;
                    flash_Erase(device_mibox4_state,sizeof(addr_inf_t));
                }                
                return ;
            }
            else
            {
                swtimer_restart(key_pressed_timernum);
            }
        }
    }

}

void update_voice_packet(uint8_t *pkt)
{
    pkt[0] = 0x01;
    pkt[1] = msbc_header_byte_table[header_idx++ % 4];
    pkt[ENCODE_OUTPUT_MSBC_SIZE - 1] = 0x00;
}

void action_after_mic_close(void)
{
    if(List_state == ADCPCM_MODE)
    {
        voice_key_state = false;
        uint8_t zero[20];
        memset(zero, 0, sizeof(zero));
        ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, zero, sizeof(zero));        
    }
    else
    {
        mic_open_state = false;
        uint8_t zero[20];
        memset(zero, 0, sizeof(zero));
        ATT_sendNotify(AUDIO_MIC_OPEN_BEFORE_HANDLE, zero, sizeof(zero));
    }
}

static void encrypt_handle(void)
{
    if (bt_check_le_connected())
    {
        if(keynum != Power_Keynum)
        {
            if(List_state == ADCPCM_MODE)
            {
                uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];

                memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                memcpy((void *)hid_send_buf, (void *)KeyBuf[keynum].keyvalue, 3);

                DEBUG_LOG_STRING("att send  [%x] [%x] [%d] \r\n",KeyBuf[keynum].keyvalue[0],KeyBuf[keynum].keyvalue[1],KeyBuf[keynum].key_send_len);
                if (keynum == Voice_Keynum && !voice_key_state)
                {
                    app_sleep_lock_set(AUDIO_LOCK, true);
                    uint8_t voice_send[3] = {0x00, 0x00, 0x01};
                    ATT_sendNotify(ADCPCM_KEYNUM_HANDLE, (void*)voice_send, 3);
                    voice_key_state = true;
                    voice_send_state = true;
                    voice_status.search = true;
                    voice_status_change();
                    swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                    swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
                    
                }
                else if(keynum != Voice_Keynum) {
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
                if (key_pressed_num == 0)
                {
                    if(keynum == Voice_Keynum && voice_send_state == true) {
                        voice_send_state = false;
                        swtimer_stop(vioce_send_timernum);
                        ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                    }
                    else if(keynum != Voice_Keynum) {
                        ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                    }
                }

            }
            else
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
					swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
                }

                if (key_pressed_num == 0)
                {
                    memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                    ATT_sendNotify(KeyBuf[keynum].handle, (void *)hid_send_buf, KeyBuf[keynum].key_send_len);
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

            if(List_state == ADCPCM_MODE)
            {
                if(keynum == Voice_Keynum && voice_send_state == true) {
                    voice_send_state = false;
                    swtimer_stop(vioce_send_timernum);
                    ATT_sendNotify(ADCPCM_KEYNUM_HANDLE, (void*)hid_send_buf, 3);
                }
                else if(keynum != Voice_Keynum) {
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
            }
            else
            {
                ATT_sendNotify(KeyBuf[keynum].handle, hid_send_buf, KeyBuf[keynum].key_send_len);
            }

            if (wake_up_state != 0)
            {
                wake_up_state = 0;
            }
        }
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] +
                 key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] +
                 key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];
        factory_KeyProcess(keynum==Voice_Keynum?0xff:keynum);
        if (le_connected_state && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            if(List_state == MSBC_MODE)
            {
                memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                memcpy((void *)hid_send_buf, (void *)KeyBuf[keynum].keyvalue, KeyBuf[keynum].key_send_len); 
                DEBUG_LOG_STRING("MSBC_MODE att send  [%x] [%x] [%d] \r\n",KeyBuf[keynum].keyvalue[0],KeyBuf[keynum].keyvalue[1],KeyBuf[keynum].key_send_len);              
                if (keynum == Voice_Keynum)
                {
                    ATT_sendNotify(AUDIO_MIC_OPEN_BEFORE_HANDLE, (uint8_t *)mic_open_before_buf, sizeof(mic_open_before_buf));
                }

                ATT_sendNotify(KeyBuf[keynum].handle, hid_send_buf, KeyBuf[keynum].key_send_len);
                if (keynum == Voice_Keynum)
                {
                    mic_open_state = true;
                    mic_open();
                    swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
                }
            }
            else
            {
                memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                memcpy((void *)hid_send_buf, (void *)KeyBuf[keynum].keyvalue, KeyBuf[keynum].key_send_len);

                DEBUG_LOG_STRING("ADCPCM_MODE att send  [%x] [%x] [%d] \r\n",KeyBuf[keynum].keyvalue[0],KeyBuf[keynum].keyvalue[1],KeyBuf[keynum].key_send_len);

                if (keynum == Voice_Keynum && !voice_key_state)
                {
                    app_sleep_lock_set(AUDIO_LOCK, true);
                    uint8_t voice_send[3] = {0x00, 0x00, 0x01};
                    ATT_sendNotify(ADCPCM_KEYNUM_HANDLE, (void*)voice_send, 3);
                    voice_key_state = true;
                    voice_send_state = true;
                    voice_status.search = true;
                    voice_status_change();
                    swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                    swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
                    
                }
                else if(keynum != Voice_Keynum) {
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
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
            if(List_state == ADCPCM_MODE){
                DEBUG_LOG_STRING("ADCPCM_MODE adv\r\n");
                adv_state = true;
                bt_start_le_adv(&bt_adv_param, power_adcpcm_adv_data_buf, sizeof(power_adcpcm_adv_data_buf));
                swtimer_start(adv_start_timernum, 1000, TIMER_START_ONCE);                

            }
            else{
                DEBUG_LOG_STRING("MSBC_MODE adv\r\n");
                bt_start_le_adv(&bt_adv_param, power_adv_data_buf, sizeof(power_adv_data_buf));
            }
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
        if(List_state == ADCPCM_MODE){
            memcpy((void *)Att_adcpcm_List[BAT_ATTLIST_INDEX].dataPtr, (void *)&batlevel, 1);
        }
        else{
            memcpy((void *)Att_msbc_List[BAT_ATTLIST_INDEX].dataPtr, (void *)&batlevel, 1);
        }
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
        DEBUG_LOG_STRING(" is_sleep %d\r\n", is_sleep);
        if(conn_param_state && E_mibox_mi4) {
            return;
        }

        if(!conn_param_state)
        {
            conn_param_state = true;
            if(!E_mibox_mi4){
                swtimer_start(low_power_timernum, 100, TIMER_START_REPEAT);
            }
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
    if(E_mibox_mi4 == 1){
        DEBUG_LOG_STRING("mi box 4\r\n");
        return;
    }
    if (HREADW(mem_le_slave_latency) == CONN_PARAM || HREADW(mem_le_slave_latency) == 49 )
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
    if (table->handle == 89)
    {
        DEBUG_LOG_STRING("READ 89\r\n");
        memcpy((void *)&senddata[1], report_handle89_1_adcpcm_value, sizeof(report_handle89_1_adcpcm_value));
        ATT_Send(senddata, sendlen + 1);
    }
    else if(table->handle == 92){
        DEBUG_LOG_STRING("READ 92\r\n");
        memcpy((void *)&senddata[1], report_handle92_1_adcpcm_value, sizeof(report_handle92_1_adcpcm_value));
        ATT_Send(senddata, sendlen + 1);
    }
    else if(table->handle == 98){
        DEBUG_LOG_STRING("READ 98\r\n");        
        memcpy((void *)&senddata[1], report_handle98_1_adcpcm_value, sizeof(report_handle98_1_adcpcm_value));
        ATT_Send(senddata, sendlen + 1);
    }
    else
    {
        memcpy((void *)&senddata[1], table->dataPtr, sendlen);
        ATT_Send(senddata, sendlen + 1);
    }

}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);
    factory_WriteDataParse(table->handle, data, len); 
    if(List_state == ADCPCM_MODE)
    {
        if (table->handle == AUDIO_CMD_ADCPCM_HANDLE)
        {
            if(!memcmp(MIC_OPEN, data, 1) && voice_key_state == false){
                DEBUG_LOG_STRING("keep voice open\r\n");
                uint8_t open_error[3] = {0x0C,0X0F,0x01};
                ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, (void*)open_error, 3);
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
    else
    {
        if (table->handle == AUDIO_CTOL_MSBC_HANDLE)
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
}

void LE_LTK_LOST(void)
{
    DEBUG_LOG_BT("BT_EVT_LE_LTK_LOST \r\n");
    if (bt_check_le_connected())
    {
        bt_send_le_disconnect(0x06);
        Bt_ClearRemoteDevInfo();
    }
    bt_set_le_state(BLE_IDLE);
    List_state = 0;
    
    flash_Erase(device_tag,sizeof(addr_inf_t));
    if(E_mibox_mi4) {
        E_mibox_mi4 = 0;
        flash_Erase(device_mibox4_state,sizeof(addr_inf_t));
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
    if(List_state == ADCPCM_MODE){
        // ATT_SendExchangeMtuReq();
    }
    if(E_mibox_mi4) {
	    swtimer_start(sleep_mirc4_timernum, 1000, TIMER_START_REPEAT);
	}
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    first_pair = true;

    app_sleep_timer_set(PAIR_DONE_DELAY);
    led_off(LED_NUM);
    flash_write(device_tag, &List_state, sizeof(List_state),STATE_INF);
    if(E_mibox_mi4) {
        flash_write(device_mibox4_state, &E_mibox_mi4, sizeof(E_mibox_mi4),STATE_INF);
    }
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
    if(E_mibox_mi4){
        swtimer_stop(sleep_mirc4_timernum);
    }
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
    swtimer_stop(adv_start_timernum);
    bt_set_le_state(BLE_CONNECTED);
    bt_disable_le_tx_md();

    if (bt_check_save_connect_info())
    {
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    }
}

void LE_Master_Feature(uint8_t *data, uint8_t len)
{
    uint8_t Master_Feature_adcpcm[3] = {0xFD, 0X7F, 0X7F};
    uint8_t Master_Feature_mibo_m4[2] = {0xFF,0X03};
    if(!List_state)
    {
        if(!memcmp(data,Master_Feature_adcpcm,sizeof(Master_Feature_adcpcm))){
            List_state = ADCPCM_MODE;
            DEBUG_LOG_STRING("adcpcm \r\n");
        }
        else if(!memcmp(data,Master_Feature_mibo_m4,sizeof(Master_Feature_mibo_m4))){
            List_state = MSBC_MODE;
            E_mibox_mi4 = 1;
            if(!Bt_CheckIsPaired()) {
                bt_send_security_request();
                DEBUG_LOG_STRING("bt_send_security_request \r\n");
            }
        }
        else{
            List_state = MSBC_MODE;
            DEBUG_LOG_STRING("msbc \r\n");
        }
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
        bt_set_auth_for_security_req(AUTH_BOND); /// BONING
        bt_set_le_mtu_size(251);
        bt_update_dle();
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
        adv_start_timernum = swtimer_add(adv_start_handle);
        vioce_send_timernum = swtimer_add(voice_send_handle);
        sleep_mirc4_timernum = swtimer_add(sleep_mirc_handle);
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
        if (!flash_record_exist(device_tag)) {
            uint16_t len = sizeof(List_state);
            flash_read(device_tag, (uint8_t*)&List_state, len);
        }

        if (!flash_record_exist(device_mibox4_state)) {
            uint16_t len = sizeof(E_mibox_mi4);
            flash_read(device_mibox4_state, (uint8_t*)&E_mibox_mi4, len);
        }

        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
    else
    {
        app_queue_reset();

        if (key_wakeup_get())
        {
            // wake_up_state++;
            // if (wake_up_state == 100)
            // {
            //     prepare_before_sleep();
            //     enter_deep_sleep();
            // }
            // sleep_time_state = 0;
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
            DEBUG_LOG_STRING("KEEP CONNECT \r\n");
        }
    }
}
