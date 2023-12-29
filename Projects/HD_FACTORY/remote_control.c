#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_bt_interface.h"
#include "yc11xx_dev_qspi.h"
#include "hci_dtm_test.h"
#include "yc_ota.h"
#include "att.h"
#include "yc_debug.h"
#include "app_config.h"
#include "app_queue.h"
#include "software_timer.h"
#include "keyscan.h"
#include "ir_factory.h"
#include "led.h"
#include "audio.h"
#include "SecretKey.h"
static const uint8_t scan_rsp_data_buf[] = {0x00};
static uint8_t adv_data_buf[] = {
0X02 ,0X01 ,0X05 ,0X03 ,0X19 ,0X80 ,0X01 ,0X13 
,0X09 ,0X52 ,0X43 ,0X55 ,0X20 ,0X46 ,0X61 ,0X63 
,0X54 ,0X65 ,0X73 ,0X74 ,0X20 ,0X48 ,0X44 ,0X30 
,0X35 ,0X35 ,0X37
};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_third;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_second;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum;

static bool tx_freq_flag;
static uint8_t key_pressed_timernum = 0xFF;
static uint8_t remote_namber[4];
static bool SecretKey_Check(void)
{
    // uint8_t adcbuf[12];

    // uint32_t secretkey_SN_Addr = 0 ;
    // uint8_t secretkey_SN[16];
    // PublicKey_TypeE secretkey_Type;
    // uint8_t secretkey_Gen[16];
    // uint8_t secretkey_Ori[16];
    // for (uint8_t i = 0; i < 12; i++) {
    //     adcbuf[i] = HREAD(mem_0_3_6v_adc_io_data + i);
    // }
    // QSPI_ReadFlashData(SecretKey_Addr, 16, secretkey_Ori);

    // QSPI_ReadFlashData(0, 3, (uint8_t *)&secretkey_SN_Addr);

    // secretkey_SN_Addr = REVERSE_3BYTE_DEFINE((secretkey_SN_Addr & 0x00ffffff)) - 4;
    // DEBUG_LOG_STRING("secretkey_SN_Addr = %x \r\n",secretkey_SN_Addr);

    // QSPI_ReadFlashData(secretkey_SN_Addr-0xc, 16, secretkey_SN);
    //     for(uint8_t i = 0; i < 16; i++){
    //     DEBUG_LOG_STRING("secretkey_Ori = %x \r\n",secretkey_SN[i]);
    // }
    // switch ((secretkey_SN[12] << 8) + secretkey_SN[13])
    // {
    // case 0x1228:
    //     secretkey_Type = Key_1228;
    //     break;
    // case 0x3527:
    //     secretkey_Type = Key_3527;
    //     break;
    // case 0x5815:
    //     secretkey_Type = Key_5815;
    //     break;
    
    // default:
    //     return false;
    // }
    // SecretKey_Generate(secretkey_Type, adcbuf, 12, secretkey_Gen);

    // return (memcmp((void *)secretkey_Ori, (void *)secretkey_Gen, 16) == 0) ? true : false;
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

    QSPI_ReadFlashData(secretkey_SN_Addr - 12, 4, remote_namber);
    DEBUG_LOG_STRING("remote_namber %x %x %x %x\r\n",remote_namber[0], remote_namber[1], remote_namber[2], remote_namber[3]);
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

static void start_adv(enum advType type, uint16_t adv_interval)
{
    struct bt_le_adv_param bt_adv_param;
    bt_adv_param.Type = type;
    bt_adv_param.adv_max_interval = adv_interval;

    if (bt_adv_param.Type == ADV_TYPE_NOMAL)
    {
        bt_set_le_state(BLE_ADV);
        // adv_data_buf[25] = 1;
        
        memcpy(&adv_data_buf[23],remote_namber,sizeof(remote_namber));
        bt_start_le_adv(&bt_adv_param, adv_data_buf, sizeof(adv_data_buf));
    }
    else if (bt_adv_param.Type == ADV_TYPE_DIRECT)
    {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
    }
}

static void Quit_FacTestMode(void)
{
    uint8_t datatemp[3];
    uint32_t code_start_addr = CODE_BURN_FLASHADDR2 + CODE_OFFSET_LEN + SERIAL_NAME_LEN;
    datatemp[0] = (uint8_t)(code_start_addr >> 16);
    datatemp[1] = (uint8_t)((code_start_addr & 0xff00) >> 8);
    datatemp[2] = (uint8_t)code_start_addr;

    QSPI_SectorEraseFlash(0);
    QSPI_WritePageFlashData(0, 3, datatemp);

    SYS_RESET();
    while(1);
}

static void key_pressed_handle(void)
{
    if (key_pressed_num == 2)
    {
        if (keynum == 2 && keynum_second == KEY_COL_NUM + 1)
        {
            DEBUG_LOG_STRING("QUIT FAC_TEST: key_pressed_time[%d] \r\n", key_pressed_time);
            if (key_pressed_time < 5) {
                key_pressed_time++;
                swtimer_restart(key_pressed_timernum);
            }
            else if (key_pressed_time >= 5)
            {
                DEBUG_LOG_STRING("QUIT FAC_TEST \r\n");
                Quit_FacTestMode();
            }
        }
    }
    else if (key_pressed_num == 3)
    {
        if (keynum == 1 && keynum_second == 2 && keynum_third == 3)
        {
            DEBUG_LOG_STRING("HCI_DTM_TEST: key_pressed_time[%d] \r\n", key_pressed_time);
            if (key_pressed_time < 10) {
                key_pressed_time++;
                swtimer_restart(key_pressed_timernum);
            }
            else if (key_pressed_time >= 10)
            {
                tx_freq_flag = true;
                stop_adv();
                hci_freq_tx_test();
                DEBUG_LOG_STRING("FREQ TX TEST \r\n");
            }
        }
    }
    else {
        key_pressed_time = 0;
    }
}

static void keyvalue_handle(key_report_t *key_report)
{
    bool le_connected_state = bt_check_le_connected();
    key_pressed_num = key_report->key_press_cnt;
    DEBUG_LOG_STRING("KEY[%d][%d][%d][%d][%d][%d] NUM[%d] \r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1], key_report->keynum_report_buf[2],
    key_report->keynum_report_buf[3], key_report->keynum_report_buf[4], key_report->keynum_report_buf[5], key_report->key_press_cnt);

    if (key_pressed_num == 0)
    {
#ifdef AUDIO_TEST_MODE
        mic_close();
#else
        if (le_connected_state) {
            uint8_t sendbuf[] = {FRAME_HEADER, RCU << 4 + PC, CMD_KEY, 0x00, 0x01, 0x00, key_pressed_num};
            ATT_sendNotify(DONGLE_SEND_HANDLE, sendbuf, sizeof(sendbuf));
        }
        else {
            set_ir_factory_press(false);
        }
		
        led_off(LED_NUM);
#endif
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] + key_report->keynum_report_buf[2]
             + key_report->keynum_report_buf[3] + key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];

#ifdef AUDIO_TEST_MODE
        if (keynum == 3) {
            voice_status.mode = STANDARD_DATA;
            mic_open();
        }
        else if (keynum == 24) {
            voice_status.mode = PCM_DATA;
            mic_open();
        }
        else if(keynum == 11) {
            voice_status.mode = ENCODE_DATA;
            mic_open();
        }
#else
        if (le_connected_state) {
            uint8_t sendbuf[] = {FRAME_HEADER, RCU << 4 + PC, CMD_KEY, 0x00, 0x01, 0x00, keynum};
            ATT_sendNotify(DONGLE_SEND_HANDLE, sendbuf, sizeof(sendbuf));
        }
        else if (keynum == 4 && tx_freq_flag) {
            tx_freq_flag = false;
            hci_freq_test_end();
            start_adv(ADV_TYPE_NOMAL, 0x30);
            DEBUG_LOG_STRING("STOP FREQ TX TEST \r\n");
        }
        else {
            set_ir_factory_press(true);
            ir_factory_send(keynum);
        }

        led_on(LED_1, 0, 0);
        led_on(LED_2, 0, 0);
        led_on(LED_3, 0, 0);
#endif
    }
    else if (key_pressed_num == 2)
    {
        if (key_report->keynum_report_buf[1] == 2 && key_report->keynum_report_buf[0] == KEY_COL_NUM + 1)
        {
            keynum = 2;
            keynum_second = KEY_COL_NUM + 1;
            key_pressed_time = 0;
            swtimer_start(key_pressed_timernum, UNIT_TIME_1S, TIMER_START_ONCE);
        }
        else {
            key_pressed_time = 0;
            swtimer_stop(key_pressed_timernum);
        }
    }
    else if (key_pressed_num == 3)
    {
        if (key_report->keynum_report_buf[0] == 1 && key_report->keynum_report_buf[1] == 2 && key_report->keynum_report_buf[2] == 3)
        {
            keynum = 1;
            keynum_second = 2;
            keynum_third = 3;
            key_pressed_time = 0;
            swtimer_start(key_pressed_timernum, UNIT_TIME_1S, TIMER_START_ONCE);
        }
        else {
            key_pressed_time = 0;
            swtimer_stop(key_pressed_timernum);
        }
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
    if (table->handle == DONGLE_SEND_HANDLE)
    {
        uint16_t cmd_id = data[0] + (data[1] << 8);
        uint16_t data_len = data[2] + (data[3] << 8);

        switch (cmd_id)
        {
        case CMD_NAME_CHECK:
        {
            uint8_t SerialName[16];
            uint8_t sendbuf[22] = {FRAME_HEADER, RCU << 4 + PC, CMD_NAME_CHECK, 0x00, 0x10, 0x00};

            QSPI_ReadFlashData(CODE_BURN_FLASHADDR2 + CODE_OFFSET_LEN, 16, SerialName);
            memcpy((void *)&sendbuf[6], SerialName, 16);
            ATT_sendNotify(DONGLE_SEND_HANDLE, sendbuf, 22);
        }
            break;
        
        case CMD_VOICE:
        {
            if (data[4] == 0x00) {
                mic_close();
                led_off(LED_NUM);
            }
            else if (data[4] == 0x01)
            {
                mic_open();
                led_on(LED_1, 0, 0);
                led_on(LED_2, 0, 0);
                led_on(LED_3, 0, 0);
            }
        }
            break;
        
        case CMD_LED:
            led_on(LED_1, data[4], data[5]);
            break;

        case CMD_QUIT_FAC:
            Quit_FacTestMode();
            break;
        
        default:
            break;
        }
    }

    else if (table->handle == OTA_WRITE_HANDLE)
    {
        OS_ENTER_CRITICAL();
        if(OTA_EVT_HANDLE(len, data) != OTA_NO_ERROR) {
            OTA_RESET();
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

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
}

void ENCRYPT_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("ENCRYPT FAIL! REASON : %x \r\n", reason);
}

void ENCRYPT_DONE(void)
{
    DEBUG_LOG_STRING("ENCRYPT_DONE \r\n");
}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);
    bt_set_tx_power(TX_POWER_5DB);
    Bt_HciFifo_Init();
    
    start_adv(ADV_TYPE_NOMAL, 0x30);
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_lpm_overhead_wake_time(0x08);
    bt_set_le_state(BLE_CONNECTED);
    bt_set_le_mtu_size(251);
    bt_disable_le_tx_md();
    Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
}

void Dev_PowerOn(void)
{
    DEBUG_LOG_STRING("DEVICE POWER ON \r\n");

    IPC_TxControlCmd(BT_CMD_FIRST_POWER_ON);         
    
    Flash_LoadFlashRecord();
    Bt_ReadAddrFromFlash();
    /* Load the factory burn address  */
    Flash_LoadFacStoreInfo();
    Bt_ReadFacStoreInfoFromFlash();
     
    if(!Lpm_GetWakeFlag())
    {
        bt_renew_scan_rsp((void *)scan_rsp_data_buf, sizeof(scan_rsp_data_buf));
    }

    start_adv(ADV_TYPE_NOMAL, 0x30);
}

void app_task_run(void)
{
    app_queue_task();
}

void app_init(void)
{
    DEBUG_LOG_STRING("HD FACTORY MODE \r\n");
    if (!Lpm_GetWakeFlag())
    {
        Lpm_EnableSet(LPM_DISABLE);
        Lpm_LockLpm(LPM_ALL_LOCK);
        bt_set_ce_length_num(0x08);
        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        GPIO_Init(IR_IO, GPIO_Mode_Out_Low);
        led_init();
        voice_report_init();
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
SecretKey_Check();
        key_pressed_timernum = swtimer_add(key_pressed_handle);
        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
}
