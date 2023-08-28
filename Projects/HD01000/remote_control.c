#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_bt_interface.h"

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
#include "ir_select.h"
#include "ir_flow.h"

#include "yc_ota.h"
#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
typedef struct {
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

enum{
    POWER_DONE,
    SOURCE_DONE,
    VOL_DONE,
    FLOW_DONE,
    Power_Keynum = 3,
    Source_Keynum = 2,
    Ok_Keynum = 16,
    Voice_Keynum = 19,
    Back_Keynum = 20,
    Flow_Keynum = 21,
    Vol_Keynum = 23,
    Vol__Keynum = 24,
    Mute_Keynum = 25,
    One_Keynum = 31,
    Three_Keynum = 33,
    Mode_Keynum = 1,
    NORMAL = 49,
    One_Three_State = 50,
    Judge_Keynum_State = 51,
    Rolling_Code_State = 52,
    Start_ADV = 53,
    CONN_PARAM = 49,
};


static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x53, 0x00, 2, 25},  //mode
    {0x00, 0x00, 0, 0},
    {0x30, 0x00, 2, 25},  //power
    {0x3E, 0x00, 8, 29},  //red
    {0x3F, 0x00, 8, 29},  //green

    {0x40, 0x00, 8, 29},  //yellow
    {0x41, 0x00, 8, 29},  //blue
    {0xB4, 0x00, 2, 25},  //left kuaijin
    {0xCD, 0x00, 2, 25},  //zanting
    {0xB3, 0x00, 2, 25},  //right kuaijin

    {0x60, 0x00, 2, 25},  //info
    {0xB7, 0x00, 2, 25},  //fangkuai
    {0xB2, 0x00, 2, 25},  //hongdian
    {0x42, 0x00, 2, 25},  //up
    {0x44, 0x00, 2, 25},  //left

    {0x41, 0x00, 2, 25},  //ok
    {0x45, 0x00, 2, 25},  //right
    {0x43, 0x00, 2, 25},  //down
    {0x21, 0x02, 2, 25},  //voice
    {0x24, 0x02, 2, 25},  //back

    {0x23, 0x02, 2, 25},  //flow
    {0x94, 0x00, 2, 25},  //exit
    {0xE9, 0x00, 2, 25},  //vol+
    {0xEA, 0x00, 2, 25},  //vol-
    {0xE2, 0x00, 2, 25},  //mute

    {0x9C, 0x00, 2, 25},  //ch+
    {0x9D, 0x00, 2, 25},  //ch-
    {0x83, 0x00, 2, 25},  //last
    {0xB8, 0x01, 2, 25},  //netflix
    {0x8D, 0x00, 2, 25},  //guide

    {0x1E, 0x00, 8, 29},  //1
    {0x1F, 0x00, 8, 29},  //2
    {0x20, 0x00, 8, 29},  //3
    {0x21, 0x00, 8, 29},  //4
    {0x22, 0x00, 8, 29},  //5

    {0x23, 0x00, 8, 29},  //6
    {0x24, 0x00, 8, 29},  //7
    {0x25, 0x00, 8, 29},  //8
    {0x26, 0x00, 8, 29},  //9
    {0x27, 0x00, 8, 29},  //0
};
static const uint16_t ir_data[] = {
    0x853,  //mode

    0x754,  //mode
    0xFF,
    0x60A,  //power
    0x628,  //red
    0x529,  //green

    0x817,  //yellow 6
    0x727,  //blue
    0x11E,  //left kuaijin
    0x01F,  //zanting
    0x21D,  //right kuaijin

    0xA33,  //infov 11
    0x31C,  //fangkuai
    0xC31,  //hongdian
    0x934,  //up
    0x736,  //left

    0xE11,  //ok 16
    0x637,  //right
    0x835,  //down
    0x655,  //voice
    0xF10,  //back

    0x619,  //flow 21
    0xD12,  //exit
    0x30D,  //vol+
    0x20E,  //vol-
    0x10F,  //mute

    0x50B,  //ch+ //26
    0x40C,  //ch-
    0xC13,  //last
    0x745,  //netflix
    0xD30,  //guide

    0xF01,  //1
    0xE02,  //2
    0xD03,  //3
    0xC04,  //4
    0xB05,  //5

    0xA06,  //6
    0x907,  //7
    0x808,  //8
    0x709,  //9
    0x000,  //
};
static bool voice_key_state;
static bool voice_send_state;
static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};
static const uint8_t adv_data_buf[] = {0x02, 0x01, 0x05, 0x03, 0x19, 0xC1, 0x03, 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18};
static const uint8_t scan_rsp_data_buf[] = {0x0D, 0x09, 0x57, 0x69, 0x6E, 0x6B, 0x20, 0x41, 0x6E, 0x64, 0x72, 0x6F, 0x69, 0x64};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
static bool tx_power_switch = true;
static uint8_t ir_send_timernum = 0xFF;
static uint8_t ir_flow_timernum = 0xFF;
static uint8_t flow_adv_timernum = 0xFF;
static uint8_t vioce_timernum = 0xFF;
static uint8_t vioce_send_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint16_t key_pressed_time;
static uint8_t keynum;
static uint8_t keynume_second;
static uint16_t ir_type = 0;
static uint8_t ir_done;
static uint16_t STATE = NORMAL;
static bool mode_state;

static bool conn_param_state;
static uint32_t sleep_time_state;
static bool encrypt_state;
static uint8_t wake_up_state;
static bool dis_encrypt_state;

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
    DEBUG_LOG_STRING(" secretkey_SN number %x \r\n", (secretkey_SN[0] << 8) + secretkey_SN[1]);
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

static void ir_flow_send(uint8_t keynum)
{
    if (keynum == Power_Keynum &&flow_ir_value[ir_type].keyvalue[0] != 0XFFFF)
    { // POWER
        if (ir_type == 10){
            ir_type_init(flow_ir_value[ir_type].ir_lib,HIGH_CUSTOM_8000_2800_A);
        }
        else if (ir_type == 143){
            ir_type_init(flow_ir_value[ir_type].ir_lib,CUSTOM_18_18_A );
        }
        else{
            if (ir_done != POWER_DONE){
                ir_type_init(flow_ir_value[ir_type].ir_lib,flow_ir_value[ir_type].coust);
                ir_done = POWER_DONE;
            }
        }
        ir_comm_send(flow_ir_value[ir_type].keyvalue[0]);
    }
    else if (keynum == Source_Keynum && flow_ir_value[ir_type].keyvalue[1] != 0XFFFF) // SOUTE
    {
        if (ir_type == 37){
            ir_type_init(SAA3010_38_45, HIGH_CUSTOM_0000_1800_A);
        }
        else if (ir_type == 117){
            ir_type_init(UPD6124_D7C8_70, CUSTOM_77_88_A);
        }
        else if (ir_type == 126){
            ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_2002_0080_A);
        }
        else if (ir_type == 128){
            ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_2002_0480_A);
        } 
        else if (ir_type == 129){
            ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_2002_0280_A);
        }
        else if (ir_type == 145){
            ir_type_init(UPD6121G_64,CUSTOM_18_18_A);
        }
        else if (ir_type == 146){
            ir_type_init(UPD6121G_64,CUSTOM_45_BC_A);
        }
        else{
            if (ir_done != SOURCE_DONE){
                ir_type_init(flow_ir_value[ir_type].ir_lib,flow_ir_value[ir_type].coust);
                ir_done = SOURCE_DONE;
            }
        }
        ir_comm_send(flow_ir_value[ir_type].keyvalue[1]);
    }
    else if ((keynum == Vol_Keynum && flow_ir_value[ir_type].keyvalue[2] != 0XFFFF) || ir_type ==177)  //VOL +
    {
        if (ir_type == 125 ||ir_type == 126 || ir_type == 127 || ir_type == 132 || ir_type == 134 ){
            ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_2002_0080_A);
        }else {
            if (ir_done != VOL_DONE) {
                ir_type_init(flow_ir_value[ir_type].ir_lib,flow_ir_value[ir_type].coust);
                ir_done = VOL_DONE;
            }
        }
        ir_comm_send(flow_ir_value[ir_type].keyvalue[2]);
    }
    else if ((keynum == Vol__Keynum && flow_ir_value[ir_type].keyvalue[3] != 0XFFFF) || ir_type ==177)  //VOL- 
    {
        if (ir_type == 125 ||ir_type == 126 || ir_type == 127 || ir_type == 132 || ir_type == 134 ){
            ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_2002_0080_A);
        }else{
            if (ir_done != VOL_DONE){
                ir_type_init(flow_ir_value[ir_type].ir_lib,flow_ir_value[ir_type].coust);
                ir_done = VOL_DONE;
            }
        }
        ir_comm_send(flow_ir_value[ir_type].keyvalue[3]);
    }
    else if (keynum == Mute_Keynum && flow_ir_value[ir_type].keyvalue[4] != 0XFFFF) //MUTE}
    {
        if (ir_type == 125 ||ir_type == 126 || ir_type == 127 || ir_type == 132 || ir_type == 134 ){
            ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_2002_0080_A);
        }
        else if (ir_type == 174){
            ir_type_init(VICTOR_C8D8_72    ,CUSTOM_43_00_A);
        }
        else{
            if (ir_done != VOL_DONE){
                ir_type_init(flow_ir_value[ir_type].ir_lib,flow_ir_value[ir_type].coust);
                ir_done = VOL_DONE;
            }
        }
        ir_comm_send(flow_ir_value[ir_type].keyvalue[4]);
    } 
}

static void ir_send_handle(void)
{
    if (keynum == Power_Keynum || keynum == Vol_Keynum || keynum == Vol__Keynum || keynum == Mute_Keynum)
    {
        if (mode_state){
            set_key_press_state(true);
            ir_flow_send(keynum);
        }else{
            set_key_press_state(true);
            ir_comm_send(ir_data[keynum]);
        }
    }            
    else if (keynum == Source_Keynum){
        set_key_press_state(true);
        ir_flow_send(keynum);
    }
    else{
        if (keynum == Mode_Keynum){
            set_key_press_state(true);
            ir_comm_send(ir_data[mode_state]);
        }
        else{
            if (!bt_check_le_connected()){
                set_key_press_state(true);
                ir_comm_send(ir_data[keynum]);
            }
        }
    }
}

static void flow_roll_handle(void)
{
    if (STATE == One_Three_State)
    {
        if (key_pressed_num == 2 && keynum == One_Keynum && keynume_second == Three_Keynum)
        {
            key_pressed_time++;
            if (key_pressed_time == 10) {
                DEBUG_LOG_STRING("204 \r\n");
                key_pressed_time = 0;
                STATE = Judge_Keynum_State;
                led_on(LED_1,0,0);
                SysTick_DelayUs(400000);
                led_off(LED_1);
                SysTick_DelayUs(400000);
                led_on(LED_1,0,0);
            }
        }
        else
        {
            STATE = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(ir_flow_timernum);
        }
    }
    else if (STATE == Judge_Keynum_State)
    {
        key_pressed_time++;

        if ((key_pressed_num == 1) && keynum == Power_Keynum)
        {
            key_pressed_time = 0;
            led_off(LED_1);
            STATE = Rolling_Code_State;
            DEBUG_LOG_STRING("221 \r\n");
        }
        else if ((key_pressed_num == 1) && keynum != Power_Keynum)
        {
            DEBUG_LOG_STRING("225 \r\n");
            STATE = NORMAL;
            key_pressed_time = 0;
            led_off(LED_1);
            SysTick_DelayUs(300000);
            led_on(LED_1,200,200);
            swtimer_stop(ir_flow_timernum);
        }

        if (key_pressed_time >= 150)
        {
            STATE = NORMAL;
            key_pressed_time = 0;
            led_off(LED_1);
            swtimer_stop(ir_flow_timernum);
        }
    }
    else if (STATE == Rolling_Code_State)
    {
        if (key_pressed_num == 1 && keynum ==Power_Keynum)
        {
            key_pressed_time++;
            DEBUG_LOG_STRING("258 key_pressed_time =  %d\r\n",key_pressed_time);
            if (key_pressed_time == 20){
                key_pressed_time = 0;
                DEBUG_LOG_STRING("255 \r\n");
                led_on(LED_1,100,200);
                ir_type++;
                ir_flow_send(Power_Keynum);
                DEBUG_LOG_STRING("265 \r\n");
            }
        }
        else
        {
            DEBUG_LOG_STRING("263 \r\n");
            STATE = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(ir_flow_timernum);
            flash_write(FLOW_ROLL_IR, (uint8_t*)&ir_type, sizeof(ir_type),STATE_INF);
        }
    }
}

static void flow_adv_handle(void)
{
    if (key_pressed_num == 2 && keynum == Ok_Keynum && keynume_second == Flow_Keynum)
    {
        key_pressed_time++;
        if (key_pressed_time == 3){
            key_pressed_time = 0;
            
            led_on(LED_2,0,0);
            SysTick_DelayMs(400);
            led_off(LED_2);
            SysTick_DelayMs(400);
            led_on(LED_2,100,30000);
            
            STATE = Start_ADV;
            ir_comm_send(ir_data[Voice_Keynum]);
            start_adv(ADV_TYPE_NOMAL, 0x30, true);
            swtimer_stop(flow_adv_timernum);
        }
    }
    else
    {
        key_pressed_time = 0;
        swtimer_stop(flow_adv_timernum);
    }
}

static void keyvalue_handle(key_report_t* key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    
    if (key_pressed_num == 0)
    {
#ifdef AUDIO_TEST_MODE
        mic_close();
#else
        if (bt_check_le_connected() && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }
            ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
        }

        set_key_press_state(false);
        swtimer_stop(ir_send_timernum);
#endif
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] + key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] + key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];
        DEBUG_LOG_STRING("KEY [%d] \r\n", keynum);

        if (bt_check_le_connected() && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }

            ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            led_on(LED_2, 50, 50);

            if (keynum == Voice_Keynum) {
                // voice_status.search = true;
                // app_sleep_lock_set(AUDIO_LOCK, true);
                // voice_status_change();
                // swtimer_start(vioce_timernum, 500, TIMER_START_ONCE);                
                voice_key_state = true;
                voice_send_state = true;
                voice_status.search = true;
                app_sleep_lock_set(AUDIO_LOCK, true);
                voice_status_change();
                swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                swtimer_start(vioce_timernum, 10000, TIMER_START_ONCE);
            }
        }

        if (STATE == NORMAL)
        {
            if (keynum == Power_Keynum || keynum == Vol_Keynum || keynum == Vol__Keynum || keynum == Mute_Keynum)
            {
                if (mode_state) {
                    led_on(LED_1,50,50);
                    ir_flow_send(keynum);
                    swtimer_start(ir_send_timernum,100,TIMER_START_ONCE);
                }
                else {
                    if (!bt_check_le_connected()) {
                        led_on(LED_2,50,50);
                        ir_comm_send(ir_data[keynum]);
                        swtimer_start(ir_send_timernum,100,TIMER_START_ONCE);
                    }
                }
            }
            else if (keynum == Source_Keynum) {
                led_on(LED_1,50,50);
                ir_flow_send(keynum);
                swtimer_start(ir_send_timernum, 100, TIMER_START_ONCE);
            }
            else if (keynum == Voice_Keynum)
            {
                if (!bt_check_le_connected())
                {
                    led_on(LED_2, 100, 100);
                    SysTick_DelayMs(200);
                    led_off(LED_2);
                    SysTick_DelayMs(500);
                    led_on(LED_2, 100, 30000);
                    start_adv(ADV_TYPE_NOMAL, 0x30, true);
                    STATE = Start_ADV;

                    if (ir_done != FLOW_DONE) {
                        ir_type_init(GD_2000_94, CUSTOM_00_00_A);
                        ir_done = FLOW_DONE;
                    }
                    ir_comm_send(ir_data[keynum]);
                }
            }
            else
            {
                if (ir_done != FLOW_DONE) {
                    ir_type_init(GD_2000_94,CUSTOM_00_00_A);
                    ir_done = FLOW_DONE;
                }

                if (keynum == Mode_Keynum)
                {
                    mode_state ^= 0x1;
                    if (mode_state) {
                        led_on(LED_1,50,50);
                    }
                    else {
                        led_on(LED_2,50,50);
                    }

                    if (!bt_check_le_connected()) {
                        ir_comm_send(ir_data[mode_state]);
                        swtimer_start(ir_send_timernum,100,TIMER_START_ONCE);
                    }
                }
                else {
                    if (!bt_check_le_connected()) {
                        led_on(LED_2, 50, 50);
                        ir_comm_send(ir_data[keynum]);
                        swtimer_start(ir_send_timernum,100,TIMER_START_ONCE);
                    }
                }
            }
        }
        else if (STATE == Start_ADV) {
            if (keynum == Back_Keynum) {
                led_on(LED_2,0,0);
                SysTick_DelayMs(500);
                led_off(LED_2);
                STATE = NORMAL;
            }
        }
    }
    else if (key_pressed_num == 2)
    {
        if (key_report->keynum_report_buf[0] == One_Keynum && key_report->keynum_report_buf[2] == Three_Keynum)
        {
            keynum = key_report->keynum_report_buf[0];
            keynume_second = key_report->keynum_report_buf[2];
            DEBUG_LOG_STRING(" FLOW ROLLING START \r\n");
            STATE = One_Three_State;
            swtimer_start(ir_flow_timernum,20,TIMER_START_REPEAT);
        }
        else if (key_report->keynum_report_buf[3] == Ok_Keynum && key_report->keynum_report_buf[2] == Flow_Keynum)
        {
            keynum = key_report->keynum_report_buf[3];
            keynume_second = key_report->keynum_report_buf[2];
            STATE = One_Three_State;
            swtimer_start(flow_adv_timernum,100,TIMER_START_REPEAT);
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
    update_conn_param(false);
    keyscan_start();
    software_timer_start(SYSTEM_CURRENT_CLOCK, 10);
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
        bt_le_conn_updata_param_req(0x10, 0x10, 49, 500);
    }
    else {
        bt_le_conn_updata_param_req(0x09, 0x09, 0, 400);
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
    
    if (table->handle == AUDIO_CMD_HANDLE)
    {
        if(!memcmp(MIC_OPEN, data, 1) && voice_key_state == false){
            DEBUG_LOG_STRING("keep voice open\r\n");
            uint8_t open_error[3] = {0x0c,0X0f,0x01};
            ATT_sendNotify(AUDIO_CTRL_HANDLE, (void*)open_error, 3);
        }
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

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if (HREADW(mem_le_slave_latency) == 49) {
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
    }

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

void ENCRYPT_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("ENCRYPT FAIL! REASON : %x \r\n", reason);
}

void ENCRYPT_DONE(void)
{
    DEBUG_LOG_STRING("ENCRYPT_DONE \r\n");
    dis_encrypt_state = false;
    encrypt_state = true;
    update_conn_param(false);
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
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
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_lpm_overhead_wake_time(0x10);
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
        start_adv(ADV_TYPE_DIRECT, 0x10, true);
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
        bt_set_ce_length_num(0x0F);//0x08
        bt_set_le_mtu_size(251);
        software_timer_start(SYSTEM_CURRENT_CLOCK, 10);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        ir_init(DEFAULT_TYPE, DEFAULT_CUSTOMER);
        voice_report_init();
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        if (!flash_record_exist(FLOW_ROLL_IR)) {
            uint16_t len = sizeof(ir_type);
            flash_read(FLOW_ROLL_IR, (uint8_t*)&ir_type, len);
        }

        ir_send_timernum = swtimer_add(ir_send_handle);
        ir_flow_timernum = swtimer_add(flow_roll_handle);
        flow_adv_timernum = swtimer_add(flow_adv_handle);
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
            DEBUG_LOG_STRING("WAKE UP \r\n");
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
            DEBUG_LOG_STRING("KEEP CONNECT \r\n");
        }
    }
}
