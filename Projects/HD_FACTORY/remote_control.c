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

static const uint8_t scan_rsp_data_buf[] = {0x00};
static const uint8_t adv_data_buf[] = {0x02, 0x01, 0x05, 0x03, 0x03, 0x12, 0x18, 0x03, 0x19, 0x80, 0x01, 0x0B, 0x09, 0x48, 0x44, 0x5f, 0x46, 0x41, 0x43, 0x5f, 0x52, 0x43, 0x55, 0x05, 0xFF, 0x71, 0x01, 0x04, 0x1E};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_third;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_second;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum;

static bool tx_freq_flag;
static uint8_t key_pressed_timernum = 0xFF;

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

        key_pressed_timernum = swtimer_add(key_pressed_handle);
        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
}
