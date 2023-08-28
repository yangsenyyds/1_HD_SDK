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
#include "keyscan.h"
#include "vbat.h"
#include "led.h"
#include "audio.h"
#include "ir_load.h"
#include "ir_send.h"
#include "ir_lib.h"

#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
enum {
    Pwr_Keynum = 2,
    Voice_Keynum = 1,
    Back_Keynum = 17,
    Back_Row = 1,
    Pause_Keynum = 13,
    Pause_Row = 2,
    CONN_PARAM = 128,

};

static const uint8_t keyvalue_buf[] = {
    0x00,

    0xa0,   // 
    0x02,   // POWER 1
    0xce,   //...
    0x07,   // ch+
    0x12,   // up

    0x60,  // right 6
    0xd2,  // ok
    0x62,  // Pause
    0x0f,  // left
    0x4f,  // 中间

    0x68,  // 123 11
    0x65,  // home
    0xb9,  // back
    0x0b,  // down
    0x10,  // samsung tv //上3
    // 0x73,//globo play客户提供的是73 上3

    0x61,  // ch- 16
    0x58,  // 7
   0xb4,  // samsungtv plus //上3
    0xf3,  // netflix  //上1
    0xDF,  // disney //上 2  

    0x79,  // vol- 21 
    0xbc,   //rakuten tv //下1
    0xdf,   //Disney 是DF //下2
    0x37,   //www//下3
};

static const uint8_t scan_rsp_data_buf[] = {0x13, 0x09, 0x53, 0x6D, 0x61, 0x72, 0x74, 0x20, 0x43, 0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x20, 0x32, 0x30, 0x31, 0x36};
static const uint8_t adv_data_buf[] = {0x02, 0x01, 0x05, 0x03, 0x19, 0x80, 0x01, 0x07, 0x02, 0x12, 0x18, 0x0F, 0x18, 0x0A, 0x18};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_secnd;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum;

static uint8_t key_pressed_timernum = 0xFF;
static uint8_t param_accepted_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static bool tx_power_switch = true;
bool encrypt_state;
static bool dis_encrypt_state;
static uint8_t conn_param_state;
static uint8_t hid_send_buf[KEY_SEND_LEN];
static uint32_t sleep_time_state;
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

static void param_accepted_handle(void){
    if(key_pressed_num != 0) {
        swtimer_restart(param_accepted_timernum);
    }else {
        if(HREADW(mem_le_slave_latency) == 0) {
            bt_send_le_disconnect(0x13);
        }
    }
}



static void key_pressed_handle(void)
{
    if(key_pressed_num == 1 || key_pressed_num == 0)
    {
        if(!bt_check_le_connected())
        {
            if (keynum == Voice_Keynum) {
                if (!Bt_CheckIsPaired()) {
                    ir_comm_send(0xFD);
                    if(key_pressed_num == 1) {
                        set_key_press_state(true);
                    }
                }
            }
            else if(keynum != Pwr_Keynum){
                ir_comm_send(keyvalue_buf[keynum]);
                if(key_pressed_num == 1) {
                    set_key_press_state(true);
                }
            }
        }
        else {
            if(key_pressed_num == 1 && encrypt_state && Bt_HciGetUsedBufferNum() <= 1) {
                DEBUG_LOG_STRING("hid_send_buf [%d] \r\n", hid_send_buf[0]);
                ATT_sendNotify(KEY_SEND_HANDLE, (void *)hid_send_buf, KEY_SEND_LEN);
                swtimer_restart(key_pressed_timernum);
            }
        }
        if(key_pressed_num == 0) {
            key_pressed_time = 0;
        }
    }
    else if (key_pressed_num == 2 && keynum == Back_Keynum && keynum_secnd == Pause_Keynum)
    {
        key_pressed_time++;

        if (key_pressed_time >= 3)
        {
            keynum_secnd = 0;
            key_pressed_time = 0;

            if(!bt_check_le_connected()) {
            // {
            //     uint8_t hid_send_buf[KEY_SEND_LEN];
                
            //     memset(hid_send_buf,0x0,sizeof(hid_send_buf));
            //     hid_send_buf[0] = 0xD1;
            //     ATT_sendNotify(KEY_SEND_HANDLE, (void *)hid_send_buf, KEY_SEND_LEN);
            //     SysTick_DelayMs(100);
            //     bt_send_le_disconnect(0x13);
            //     SysTick_DelayMs(100);
            // }
                led_on(LED_1, 100, 600);
                ir_comm_send(0xD1);
                set_key_press_state(true);
                
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10,true);
            }
        }
        else {
            swtimer_restart(key_pressed_timernum);
        }
    }
}

static void keyvalue_handle(key_report_t *key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    if (key_pressed_num == 0)
    {
        if (keynum == Pwr_Keynum) {
            set_key_press_state(false);
        }
        else if (bt_check_le_connected() && encrypt_state)
        {
            memset((void *)hid_send_buf, 0, sizeof(hid_send_buf));
            ATT_sendNotify(KEY_SEND_HANDLE, (void *)hid_send_buf, KEY_SEND_LEN);

            if (keynum == Voice_Keynum) {
                mic_close();
            }
        }
        else {
            set_key_press_state(false);
        }
        if(wake_up_state != 0){
            wake_up_state = 0;
        }
        led_off(LED_1);
    }
    else if (key_pressed_num == 1)
    {
        keynum = 0;
        for(uint8_t i = 0; i < KEY_ROW_NUM; i++) {
            keynum += key_report->keynum_report_buf[i];
        }
        // DEBUG_LOG_STRING("KeyNum [%d] \r\n", keynum);
        DEBUG_LOG_STRING("KEY [%d][%d][%d][%d][%d] \r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1], key_report->keynum_report_buf[2],key_report->keynum_report_buf[3]
        ,key_report->keynum_report_buf[4]);
        if (keynum == Pwr_Keynum) {
            ir_single_send(0xE6,1);
            ir_comm_send(keyvalue_buf[Pwr_Keynum]);
            set_key_press_state(true);
        }
        else if (bt_check_le_connected() && encrypt_state)
        {
            memset((void *)hid_send_buf, 0, sizeof(hid_send_buf));
            memcpy((void *)hid_send_buf, (void *)&keyvalue_buf[keynum], 1);
            ATT_sendNotify(KEY_SEND_HANDLE, (void *)hid_send_buf, KEY_SEND_LEN);
            
            if (keynum == Voice_Keynum) {
                mic_open();
            }
            else {
                swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
            }
        }
        else
        {
            key_pressed_time = 0;
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
            
        }

        led_on(LED_1, 0, 0);
    }
    else if (key_pressed_num == 2)
    {
        set_key_press_state(false);
        led_off(LED_1);
        if (key_report->keynum_report_buf[Back_Row] == Back_Keynum && key_report->keynum_report_buf[Pause_Row] == Pause_Keynum)
        {
            keynum = key_report->keynum_report_buf[Back_Row];
            keynum_secnd = key_report->keynum_report_buf[Pause_Row];
            key_pressed_time = 0; 
            swtimer_start(key_pressed_timernum, UNIT_TIME_1S, TIMER_START_ONCE);
        }
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
        bt_le_conn_updata_param_req(0x08, 0x08, 128, 500);
    }
    else {
        bt_le_conn_updata_param_req(0x08, 0x08, 0, 200);
    }
}

void CONN_PARAM_ACCEPTED(void)
{
    uint8_t le_slave_param = HREADW(mem_le_slave_latency);
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", le_slave_param);

    if(le_slave_param == CONN_PARAM) {
        swtimer_stop(param_accepted_timernum);
        swtimer_stop(low_power_timernum);
        if(!conn_param_state) {
            conn_param_state = true;
        }
        sleep_time_state = 0;
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
    }else if(le_slave_param != 0) {
        if(!conn_param_state) {
            conn_param_state = true;
        }
        swtimer_stop(param_accepted_timernum);
        swtimer_stop(low_power_timernum);
        bt_send_le_disconnect(0x13);
    }else {
        swtimer_start(param_accepted_timernum,30000,TIMER_START_ONCE);
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
    
    if (table->handle == 121) {
        if (len == 3 && data[0] == 0x31 && data[1] == 0x01) {
            uint8_t sendbuf[] = {0x33, 0x17, 0x00, 0x0E, 0x00, 0x00, 0x02, 0x00, 0x02, 0x31, 0xB4, 0x01, 0x13, 0x00, 0xA2, 0x81, 0x17, 0x12, 0x01};
            ATT_sendNotify(147, sendbuf, sizeof(sendbuf));
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

void ENCRYPT_DONE(void)
{
    DEBUG_LOG_STRING("ENCRYPT_DONE \r\n");
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
    encrypt_state = true;
    dis_encrypt_state = false;
    if(bt_check_save_connect_info()){
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    }
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
    update_conn_param(false);

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
    if (reason == 0x13 || reason == 0x16 || dis_encrypt_state) {
        DEBUG_LOG_STRING("dis_encrypt_state %d \r\n",dis_encrypt_state);
        enter_deep_sleep();
        return;
    }
    else if(!dis_encrypt_state && Bt_CheckIsPaired()){
        DEBUG_LOG_STRING("disconnect dis_encrypt_state %d \r\n",dis_encrypt_state);
        dis_encrypt_state = true;
    }
    else if(reason == 0x06) {
        ir_comm_send(0xD1);
        set_key_press_state(true);
        start_adv(ADV_TYPE_NOMAL, 0x30,true);
        return;
    }

    app_queue_reset();
    remote_control_reinit();
    
    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x10,true);
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
    start_adv(ADV_TYPE_NOMAL, 0x30,true);
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_le_state(BLE_CONNECTED);
    bt_set_lpm_overhead_wake_time(0x10);
    bt_disable_le_tx_md();
    set_key_press_state(false);
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
        bt_update_dle();
        bt_set_ce_length_num(0x08);//小包用  大包用0x08
        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        ir_init(TC901_D8B8_55, CUSTOM_07_07_A);
        voice_report_init();
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        bt_set_auth_for_security_req(AUTH_BOND);

        key_pressed_timernum = swtimer_add(key_pressed_handle);
        param_accepted_timernum = swtimer_add(param_accepted_handle);
        
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
