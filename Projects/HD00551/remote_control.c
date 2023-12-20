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

typedef struct
{
    uint8_t keyvalue;
    uint8_t Ir_TypeDef;
} IRBuf_TypeDef;

enum
{
    Voice_Keynum = 30,
    Voice_Col = 5,
    Vol__Keynum = 44,
    Vol_Col = 3,

    ir_state_01 = 101,
    ir_state_C4 = 104,
    ir_state_1A = 111,
    ir_state_77 = 177,
    ir_state_97 = 197,
    ir_state_A4 = 113,
    ir_state_081F = 108,
};
#if (Project_key == 551)//400
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},

    {0x25, 0x01}, // 1
    {0x15, 0x01},
    {0x00, 0x08},
    {0x7C, 0x1A},
    {0x02, 0x08},

    {0x01, 0x08}, // 6
    {0x47, 0xC4},
    {0x2A, 0xC4},
    {0x32, 0x97},
    {0x10, 0x97},

    {0x0C, 0x97}, // 11
    {0x00, 0x01},
    {0X01, 0x01},
    {0X02, 0x01}, // NETFLIX
    {0X03, 0x01},

    {0X04, 0x01}, // 16
    {0X05, 0x01},
    {0X06, 0x01},
    {0X07, 0x01},
    {0X08, 0x01},

    {0x09, 0x01}, // 21
    {0x0A, 0x01},
    {0X0B, 0x01},
    {0X24, 0x97},
    {0X25, 0x97},

    {0X26, 0x97}, // 26
    {0X27, 0x97},
    {0X48, 0xC4},
    {0X15, 0x97},
    {0X00, 0X00},

    {0X5B, 0xA4}, // 31
    {0X4B, 0xC4},
    {0X74, 0x01},
    {0X34, 0x01},
    {0X65, 0x01},

    {0X33, 0x01}, // 36
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X24, 0x01},

    {0X12, 0x01}, // 41
    {0X10, 0x01},
    {0X14, 0x01}, //
    {0X13, 0x01},
    {0X11, 0x01},

    {0X00, 0x00}, // 46
    {0X17, 0x01},
    {0X28, 0x97},
    {0X3A, 0x01},
    {0X1B, 0x97},

    {0X1A, 0x97}, // 51
    {0X1C, 0x97},
    {0X3C, 0x97},
    {0X19, 0x97},
    {0X3D, 0x97},

    {0X4D, 0xC4}, // 56
    {0X18, 0x97},
    {0X65, 0x1A},
};
#elif (Project_key == 553)//421j
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},

    {0x3A, 0x01}, // 1
    {0x15, 0x01},
    {0x00, 0x08},
    {0x7C, 0x1A},
    {0x02, 0x08},

    {0x03, 0x08}, // 6
    {0x47, 0xC4},
    {0x01, 0x08},
    {0x32, 0x97},
    {0x10, 0x97},

    {0x0C, 0xC4}, // 11
    {0x00, 0x01},
    {0X01, 0x01},
    {0X02, 0x01}, // NETFLIX
    {0X03, 0x01},

    {0X04, 0x01}, // 16
    {0X05, 0x01},
    {0X06, 0x01},
    {0X07, 0x01},
    {0X08, 0x01},

    {0x09, 0x01}, // 21
    {0x0A, 0x01},
    {0X0B, 0x01},
    {0X24, 0x97},
    {0X25, 0x97},

    {0X26, 0x97}, // 26
    {0X27, 0x97},
    {0X5B, 0xA4},
    {0X48, 0xC4},
    {0X00, 0X00},

    {0X25, 0x01}, // 31
    {0X4B, 0xC4},
    {0X74, 0x01},
    {0X34, 0x01},
    {0X65, 0x01},

    {0X33, 0x01}, // 36
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X24, 0x01},

    {0X12, 0x01}, // 41
    {0X10, 0x01},
    {0X15, 0x01}, //
    {0X13, 0x01},
    {0X11, 0x01},

    {0X14, 0x01}, // 46
    {0X17, 0x01},
    {0X28, 0x97},
    {0X0C, 0x97},
    {0X1B, 0x97},

    {0X1A, 0x97}, // 51
    {0X1C, 0x97},
    {0X3C, 0x97},
    {0X19, 0x97},
    {0X3D, 0x97},

    {0X4D, 0xC4}, // 56
    {0X18, 0x97},
    {0X65, 0x1A},
};
#endif
static const uint8_t adv_data_buf[] = {
    0x13, 0x09, 0x53, 0x4F, 0x4E, 0x59, 0x20, 0x54, 0x56, 0x20, 0x52, 0x43,
	 0x20, 0x4D, 0x49, 0x43, 0x20, 0x30, 0x30, 0x31, 0x03, 0x19, 0x80, 0x01, 
	 0x02, 0x01, 0x05, 0x03, 0x03, 0x12, 0x18};
static const uint8_t scan_rsp_data_buf[] = {};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;

static uint8_t vioce_timernum = 0xFF;
static uint8_t key_pressed_timernum = 0xFF;
static uint8_t audio_data_timernum = 0xFF;
static uint8_t voice_send_data_timernum = 0xFF;
static uint8_t encrypt_report_timernum = 0xFF;
static uint16_t key_pressed_time;
static uint8_t keynum;
static uint8_t keynum_second;
static bool tx_power_switch = true;
static uint8_t ir_state;
static bool led_state;
static bool voice_state;
static bool mic_open_already;
static bool key_mic_open;
static bool encrypt_state;
static uint16_t keyscan_state_cnt;
static bool ks_state;
static bool dis_encrypt_state;
static bool voice_send_state;
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

static void audio_data_handle(void)
{
    // DEBUG_LOG_STRING("audio star t:%d,%d,%d\r\n", keyscan_state_cnt, ks_state, mic_open_already);
    if (!voice_state && bt_check_le_connected() && encrypt_state && mic_open_already)
    {
        voice_state = true;
        audio_start_timer();
        DEBUG_LOG_STRING("audio star timer\r\n");
    }

    if (!ks_state && (keyscan_state_cnt++ > KEYSCAN_STC_CNT_MAX))
    {
        keyscan_state_cnt = 0;
        // mic_close();
        SysTick_DelayMs(100);
        enter_deep_sleep();
    }
}

void action_after_mic_close(void)
{
    key_mic_open = false;
    led_off(LED_1);
}

static void voice_send_handle(void)
{
    // if(key_pressed_num == 1 && voice_send_state == true) {
    if(voice_send_state == true) {
        voice_send_state = false;
        uint8_t hid_send_buf[2] = {0x00, 0x00};
        ATT_sendNotify(31, (void *)hid_send_buf, 2);
    }
}

static void encrypt_handle(void)
{
    //  DEBUG_LOG_STRING("BAT keynum: %d \r\n", keynum);
    if (bt_check_le_connected() && keynum == Voice_Keynum)
	{
        set_key_press_state(false);
        voice_send_state = true;
        uint8_t hid_send_buf[2] = {0x21, 0x02};
        ATT_sendNotify(31, (void *)hid_send_buf, 2);
        app_sleep_lock_set(AUDIO_LOCK, true);
        swtimer_start(voice_send_data_timernum, 400, TIMER_START_ONCE);
        swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
        mic_open_already = true;
        led_on(LED_1, 0, 0);
    }
}

static void key_pressed_handle(void)
{
    if (key_pressed_num == 2)
    {
        if (keynum == Voice_Keynum && keynum_second == Vol__Keynum)
        {
            key_pressed_time++;

            if (key_pressed_time == 3)
            {
                key_pressed_time = 0;
                led_state = true;
                led_on(LED_1, 200, 60000);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10, true);
                return;
            }
            swtimer_restart(key_pressed_timernum);
        }
    }
    else if (key_pressed_num == 0 || key_pressed_num == 1)
    {
        if (ir_data[keynum].Ir_TypeDef != 0x00)
        {
            
            if (ir_data[keynum].Ir_TypeDef == 0x97 && ir_state_97 != ir_state)
            {
                ir_state = ir_state_97;
                ir_type_init(UPD6124_D7C8_70, CUSTOM_97_00_A);
            }
            else if (ir_data[keynum].Ir_TypeDef == 0x01 && ir_state_01 != ir_state)
            {
                ir_state = ir_state_01;
                ir_type_init(UPD6124_D7C5_69, CUSTOM_01_00_A);
            }
            else if (ir_data[keynum].Ir_TypeDef == 0x1A && ir_state_C4 != ir_state)
            {
                ir_state = ir_state_C4;
                ir_type_init(UPD6124_D7C8_70, CUSTOM_1A_00_A);
            }
            else if (ir_data[keynum].Ir_TypeDef == 0xC4 && ir_state_1A != ir_state)
            {
                ir_state = ir_state_1A;
                ir_type_init(UPD6124_D7C8_70, CUSTOM_C4_00_A);
            }
            else if (ir_data[keynum].Ir_TypeDef == 0xA4 && ir_state_A4 != ir_state)
            {
                ir_state = ir_state_A4;
                ir_type_init(UPD6124_D7C8_70, CUSTOM_A4_00_A);
            }
            else if (ir_data[keynum].Ir_TypeDef == 0x77 && ir_state_77 != ir_state)
            {
                ir_state = ir_state_77;
                ir_type_init(UPD6124_D7C8_70, CUSTOM_77_00_A);
            }
            else if (ir_data[keynum].Ir_TypeDef == 0x08 && ir_state_081F != ir_state)
            {
                DEBUG_LOG_STRING("ir_state_081F\r\n");
                ir_state = ir_state_081F;
                ir_type_init(UPD6124_D7C13_71, CUSTOM_081F_0000_A);
            }

            ir_comm_send(ir_data[keynum].keyvalue);
            DEBUG_LOG_STRING("ir_comm_send\r\n");
            if (key_pressed_num == 1)
            {
                set_key_press_state(true);
            }
        }
    }
}

static void keyvalue_handle(key_report_t *key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    ks_state = true;

    if (key_pressed_num == 0)
    {
        ks_state = false;
        keyscan_state_cnt = 0;
        if (bt_check_le_connected() && keynum == Voice_Keynum && encrypt_state && voice_send_state == true)
        {
            voice_send_state = false;
            uint8_t hid_send_buf[2] = {0x00, 0x00};
            ATT_sendNotify(31, (void *)hid_send_buf, 2);
            swtimer_stop(voice_send_data_timernum);
            DEBUG_LOG_STRING("mc................\r\n");
        }
        if (!led_state)
        {
            // led_off(LED_1);
        }
        set_key_press_state(false);
    }
    else if (key_pressed_num == 1)
    {
        keynum = 0;
        for (uint8_t i = 0; i < KEY_ROW_NUM; i++)
        {
            keynum += key_report->keynum_report_buf[i];
        }
        factory_KeyProcess(keynum==Voice_Keynum?0xff:keynum);
        DEBUG_LOG_STRING("KEY [%d][%d][%d][%d][%d][%d][%d][%d]\r\n",
        key_report->keynum_report_buf[0], key_report->keynum_report_buf[1], 
        key_report->keynum_report_buf[2], key_report->keynum_report_buf[3],
        key_report->keynum_report_buf[4], key_report->keynum_report_buf[5], 
        key_report->keynum_report_buf[6], key_report->keynum_report_buf[7]);
        if (bt_check_le_connected() && keynum == Voice_Keynum && encrypt_state)
        {
            voice_send_state = true;
            uint8_t hid_send_buf[2] = {0x21, 0x02};
            ATT_sendNotify(31, (void *)hid_send_buf, 2);
            app_sleep_lock_set(AUDIO_LOCK, true);
            swtimer_start(voice_send_data_timernum, 100, TIMER_START_ONCE);
            swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
            mic_open_already = true;
            led_on(LED_1, 0, 0);
        }
        else
        {
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
        }
        if (!led_state)
        {
            // led_on(LED_1, 0, 0);
        }
    }
    else if (key_pressed_num == 2)
    {
        if (key_report->keynum_report_buf[Voice_Col] == Voice_Keynum && key_report->keynum_report_buf[Vol_Col] == Vol__Keynum)
        {
            keynum = Voice_Keynum;
            keynum_second = Vol__Keynum;
            swtimer_start(key_pressed_timernum, 1000, TIMER_START_ONCE);
        }
    }
}

void action_after_led_blk(void)
{
    if (led_state)
    {
        led_state = false;
        stop_adv();
        Bt_SndCmdPwroff();
    }
}

static void power_handle(uint8_t batlevel)
{
    // DEBUG_LOG_STRING("BAT LEVEL: %d \r\n", batlevel);

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
    DEBUG_LOG_STRING("is_sleep  %d\r\n", is_sleep);
    if (is_sleep)
    {
        bt_le_conn_updata_param_req(0x08, 0x08, 99, 400);
    }
    else
    {
        bt_le_conn_updata_param_req(0x08, 0x08, 0, 400);
    }
}

void stop_adv(void)
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
    uint16_t sendlen = (table->dataLen >= (HREADW(mem_mtu_size)) - 1) ? ((HREADW(mem_mtu_size)) - 1) : table->dataLen;

    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);

    memcpy((void *)&senddata[1], table->dataPtr, sendlen);
    ATT_Send(senddata, sendlen + 1);
}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);
    factory_WriteDataParse(table->handle, data, len);
    if (table->handle == VOICE_AUDIO_CMD_HANDLE)
    {
        if (data[0] == 0x01)
        {
        }
        else if (data[0] == 0x00)
        {
            // DEBUG_LOG_STRING("597\r\n");
            // mic_close();
            led_off(LED_1);            
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

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if (HREADW(mem_le_slave_latency) == 99)
    {
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
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
    keyscan_state_cnt = 0;
    update_conn_param(false);

    dis_encrypt_state = false;
    mic_open_already = true;

    encrypt_state = true;
    if (keynum == Voice_Keynum)
	{
        led_on(LED_1, 0, 0);
        swtimer_start(encrypt_report_timernum, 200, TIMER_START_ONCE); 
    }
            
}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
    led_state = false;

    led_off(LED_1);
    swtimer_stop(encrypt_report_timernum);
    keynum = 0;
    led_on(LED_1,200,1200);
}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);
    encrypt_state = false;
    bt_set_tx_power(TX_POWER_5DB);
    Bt_HciFifo_Init();
    app_sleep_lock_set(LATENCY_LOCK, false);
    bt_set_le_state(BLE_IDLE);

    System_ChangeXtal24M();
    swtimer_stop(voice_send_data_timernum);
   
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

    if (Bt_CheckIsPaired())
    {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else
    {
        start_adv(ADV_TYPE_NOMAL, 0x10, true);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    set_key_press_state(false);
    bt_set_lpm_overhead_wake_time(0x0a);
    bt_set_le_state(BLE_CONNECTED);
    if(bt_check_save_connect_info())
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
		bt_set_le_mtu_size(251);
        bt_set_ce_length_num(0x0F); // 小包用  大包用0x08
        software_timer_start(SYSTEM_CURRENT_CLOCK, 10);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(UPD6121F_LIAN_66, CUSTOM_01_00_A);

        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        vioce_timernum = swtimer_add(action_after_mic_close);
        key_pressed_timernum = swtimer_add(key_pressed_handle);
        audio_data_timernum = swtimer_add(audio_data_handle);
        voice_send_data_timernum = swtimer_add(voice_send_handle);
        encrypt_report_timernum = swtimer_add(encrypt_handle);
        swtimer_start(audio_data_timernum, 200, TIMER_START_REPEAT);
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
        mic_open();
    }
    else
    {
        app_queue_reset();

        if (key_wakeup_get())
        {
            remote_control_reinit();
            swtimer_start(audio_data_timernum, 200, TIMER_START_REPEAT);
            DEBUG_LOG_STRING("WAKE UP \r\n");
            mic_open();
        }
        else
        {
            enter_low_sleep();
#ifdef SLEEP_ONE_HOUR
            // sleep_time_state++;
            // if(sleep_time_state > 3800){
            //     bt_send_le_disconnect(0x13);
            // }                   
#endif
            DEBUG_LOG_STRING("KEEP CONNECT \r\n");
        }
    }
}
