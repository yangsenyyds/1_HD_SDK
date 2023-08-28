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
#include "ir_learn_from_remote.h"
#include "flash.h"
#include "keyscan.h"
#include "audio.h"

#include "yc11xx_pwm.h"

enum{
    NORMAL = 0,
    IR_RECEIVE_JUDGE_STATE = 1,
    IR_RECEIVE_STATE = 2,
    LED_STATE_NORMAL = 0,
    LED_STATE_SUCCESS = 1,
    LED_STATE_FAIL = 2,
    LED_STATE_CONTINU = 3,
    LED_STATE_PAIR = 4,
    PZ_OCN = 0,
    SA00_RC5 = 1,
    NEC6122 = 2,
    Set_ir_keynum = 1,
    Set_ir_Row = 0,
    PWR_KEYNUM = 2,
    VOL_UP_KEYNUM = 5,
    VOL_DOWN_KEYNUM = 6,
    AVTV_KEYNUM = 4,
    One_keynum = 22,
    One_Row = 3,
    Two_keynum = 23,
    Two_Row = 4,
    Three_keynum = 24,
    Three_Row = 5,
    Voice_Keynum = 16,
    OK_Keynum = 13,
    Server_Keynum = 8,
    xing_Keynum = 31,
    xing_Row = 0,
    jing_Keynum = 30,
    jing_Row = 5,
};

typedef struct {
    uint8_t keyvalue[4];
    uint8_t key_send_len;
    uint16_t handle;
}KeyBuf_TypeDef;

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0x00, 0x00, 0, 0},

    {0xff, 0xff, 0xff, 0xff, 8, 125},//1
    {0xff, 0xff, 0xff, 0xff, 8, 125},
    {0x66, 0x00, 0xff, 0xff, 2, 125},
    {0xff, 0xff, 0xff, 0xff, 8, 125},
    {0xff, 0xff, 0xff, 0xff, 8, 125},

    {0xff, 0xff, 0x00, 0x00, 8, 125},//6
    {0x00, 0x0c, 0x00, 0x9b, 4, 121},
    {0x00, 0x0c, 0x03, 0x08, 4, 121},
    {0xe2, 0x00, 0x00, 0x00, 2, 125},
    {0x52, 0x00, 0x00, 0x00, 2, 125},

    {0x50, 0x00, 0xff, 0xff, 2, 125},//11
    {0x4f, 0x00, 0xff, 0xff, 2, 125},
    {0x28, 0x00, 0xff, 0xff, 2, 125},
    {0x51, 0x00, 0xff, 0xff, 2, 125},
    {0x02, 0x24, 0xff, 0xff, 2, 125},

    {0x00, 0x0C, 0x00, 0x40, 4, 121},//16
    {0x02, 0x23, 0xff, 0xff, 2, 125},//
    {0x00, 0x0c, 0x00, 0x69, 4, 121},//
    {0x00, 0x0c, 0x00, 0x6a, 4, 121},//
    {0x00, 0x0c, 0x00, 0x6c, 4, 121},//

    {0x00, 0x0c, 0x00, 0x6b, 4, 121},//21
    {0x1e, 0x00, 0xff, 0xff, 2, 125},//
    {0x1f, 0x00, 0xff, 0xff, 2, 125},//
    {0x20, 0x00, 0xff, 0xff, 2, 125},//
    {0x21, 0x00, 0xff, 0xff, 2, 125},//

    {0x22, 0x00, 0xff, 0xff, 2, 125},//26
    {0x23, 0x00, 0xff, 0xff, 2, 125},//
    {0x24, 0x00, 0xff, 0xff, 2, 125},//
    {0x25, 0x00, 0xff, 0xff, 2, 125},//
    {0x00, 0x0c, 0x03, 0x0a, 4, 121},//

    {0x00, 0x0c, 0x03, 0x09, 4, 121},//31
    {0x27, 0x00, 0xff, 0xff, 2, 125},//
    {0x26, 0x00, 0xff, 0xff, 2, 125},//
    {0x02, 0x27, 0xff, 0xff, 2, 125},//
    {0x00, 0x0c, 0x03, 0x07, 4, 121},//服务

    {0x00, 0x0c, 0x03, 0x06, 4, 121},//36 确定

};

static const uint8_t adv_data_buf[] = {
 0x10 ,0x09 
,0x4F ,0x43 ,0x4E ,0x20 
,0x54 ,0x56 ,0x20 ,0x52 ,0x45 ,0x4D ,0x4F 
,0x54 ,0x45 ,0x30,0X31 
,0x03 ,0x19 ,0x80 ,0x01 
,0x02 ,0x01 ,0x05 ,0x03 ,0x03 ,0x12 ,0x18};

static const uint8_t scan_rsp_data_buf[] = {
 0x10 ,0x09 ,0x4F ,0x43 ,0x4E ,0x20 
,0x54 ,0x56 ,0x20 ,0x52 ,0x45 ,0x4D ,0x4F 
,0x54 ,0x45 ,0x30 ,0X31 };


static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint16_t key_pressed_time;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t keynum_second;
 static uint8_t keynum;

static uint8_t vioce_timernum = 0xFF;
static uint8_t vioce_send_timernum = 0xFF;
static uint8_t adv_timernum = 0xFF;
static uint8_t ir_receive_timernum = 0xFF;
static uint8_t key_pressed_timernum = 0xFF;
static bool tx_power_switch = true;

static uint8_t ir_receive_keynum[4];
static uint8_t ir_state_keynum;
static uint8_t continu_ir_receive;
static uint8_t ir_format;
static uint8_t ir_format_state = 0xff;
static uint8_t send_state;
static uint8_t led_state;
static uint8_t led_on_off_i;

static bool voice_key_state;
static bool voice_send_state;
static bool voice_led_state;
static bool encrypt_state;
static const uint16_t PZ_OCN_ir_keynum[] = {
    0xFFFF,
    
    0xFFFF,//1
    0xFFFF,
    0xF00F,//POWER
    0xFFFF,
    0xFFFF,

    0xFFFF,//6
    0xF609,
    0x6028,
    0xF30C,
    0xD42B,

    0xD629,//11
    0xD728,
    0xD32C,
    0xDA25,
    0xF40B,

    0xE41B,//16
    0xF50A,
    0x3E42,
    0x585A,
    0x605C,

    0x4F32,//21
    0xDE21,
    0xDC23,
    0xDD22,
    0xE21D,

    0xE11E,//26
    0xE01F,
    0xDF20,
    0xEE11,
    0xEA15,

    0xEB14,//31
    0xEC13,
    0xED12,
    0xE718,
    0x5237,

    0XC23D,
};

static const uint8_t SA00_RC5_ir_keynum[] = {
    0XFF,

    0XFF,//1
    0XFF,
    0X50,
    0XFF,
    0XFF,

    0XFF,//6
    0X5E,
    0X61,
    0X51,
    0X63,

    0X65,//11
    0X66,
    0X67,
    0X64,
    0X55,

    0X60,//16
    0X5D,
    0X6C,
    0X6D,
    0X6E,

    0X6F,//21
    0X71,
    0X72,
    0X73,
    0X74,

    0X75,//26
    0X76,
    0X77,
    0X78,
    0X4A,

    0X49,//31
    0X70,
    0X79,
    0X6B,
    0X7e,//服务

    0X7f,//36 ok
};

static const uint8_t NEC6122_ir_keynum[] = {
    0XFF,

    0XFF,//1
    0XFF,
    0X1A,
    0XFF,
    0XFF,

    0XFF,//6
    0X24,
    0X03,
    0X1E,
    0X54,

    0X44,//11
    0X5C,
    0X4C,
    0X50,
    0X0E,

    0X0B,//16
    0X23,
    0X45,
    0X4D,
    0X55,

    0X5D,//21
    0X49,
    0X51,
    0X59,
    0X0D,

    0X15,//26
    0X1D,
    0X09,
    0X11,
    0X1C,

    0X0C,//31
    0X14,
    0X19,
    0X41,
    0X3C,

    0X25,//36
};

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
    app_sleep_lock_set(ADV_LOCK, true);

    if (bt_adv_param.Type == ADV_TYPE_NOMAL)
    {
        if(bt_get_le_state() == BLE_DIRECT_ADV) {
            stop_adv();
        }     
        bt_set_le_state(BLE_ADV);
        bt_start_le_adv(&bt_adv_param, adv_data_buf, sizeof(adv_data_buf));
        if (timeout) {
            app_sleep_timer_set(SHUTDOWN_TIME);
        }
    }
    else if (bt_adv_param.Type == ADV_TYPE_DIRECT)
    {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        if (timeout) {
            app_sleep_timer_set(DIRECT_ADV_TIME);
        }
    }
}

static void voice_send_handle(void)
{
    if(key_pressed_num == 1) {
        voice_send_state = false;
        uint8_t hid_send_buf[2] = {0x0,0x0};
        ATT_sendNotify(125, (void*)hid_send_buf, 2);
    }
}

static void adv_handle(void)
{
    if (bt_check_le_connected()) return;
    else {
    	stop_adv();
    	start_adv(ADV_TYPE_NOMAL, 0x30, true);
    }
}

static void ir_receive_handle(void){
    if(continu_ir_receive == false)
    {
        tempStartTimer = 0;
ir_fail:
        memset(&irparams,0,sizeof(irparams));
        keyscan_stop();
        key_wakeup_set();
        DEBUG_LOG_STRING("receive \r\n");
        System_ChangeDPLL(CLOCK_DPLL_192M_multiple);
        OS_ENTER_CRITICAL();

        uint8_t JUDE_receive = ir_receive();

        OS_EXIT_CRITICAL();
        System_ChangeXtal24M();
        keyscan_start();
        DEBUG_LOG_STRING("JUDE_receive =%d  \r\n",JUDE_receive);

        if(JUDE_receive == IR_LEARN_OVERTIME)
        {
            swtimer_stop(ir_receive_timernum);
            led_state = LED_STATE_NORMAL;            
            led_on(LED_1,200, 600);
            send_state = NORMAL;
        }
        else if(JUDE_receive == IR_LEARN_SUCE)
        {
            key_pressed_time = 0;
            continu_ir_receive = true;
            send_state = IR_RECEIVE_JUDGE_STATE;
            led_state = LED_STATE_SUCCESS;
            led_on(LED_1, 200, 600);
            DEBUG_LOG_STRING("200  \r\n");

        }
        else if(JUDE_receive == IR_LEARN_FAIL){
            led_state = LED_STATE_FAIL;
            led_on(LED_1, 0, 0);
            goto ir_fail;
        }
    }
    else{
        if(key_pressed_time++ > 300)
        {
            swtimer_stop(ir_receive_timernum);
            key_pressed_time = 0;     
            led_on(LED_1,200, 600);
            send_state = NORMAL;
        }
    }
}

static void key_pressed_handle(void)
{
    bool le_connected_state = bt_check_le_connected();

    if (key_pressed_num == 1 || key_pressed_num == 0)
    {
        if(keynum == Set_ir_keynum)
        {
            DEBUG_LOG_STRING("key_pressed_time = %d \r\n",key_pressed_time);
            if (key_pressed_time >= 3) {
                if(key_pressed_num == 0) {
                    key_pressed_time = 0;
                    swtimer_start(ir_receive_timernum, 100, TIMER_START_REPEAT);
                }else {
                    swtimer_restart(key_pressed_timernum);
                    return ;
                }
            }

            if (key_pressed_time < 3 && key_pressed_num == 1)
            {
                key_pressed_time++;
                if(key_pressed_time >= 3){
                    send_state = IR_RECEIVE_STATE;
                    PWM_DisConfig(LED_1_PIN);
                    led_on(LED_1,0,0);
                }
                swtimer_restart(key_pressed_timernum);
            }
            else if(key_pressed_num == 0) {
                key_pressed_time = 0;
            }
            
        }
        else if(keynum == PWR_KEYNUM || keynum == VOL_UP_KEYNUM || keynum == VOL_DOWN_KEYNUM || keynum == AVTV_KEYNUM)
        {
            if(keynum == PWR_KEYNUM && !flash_read(HD_POWER_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                ir_remote_learn_send(irparams);
                DEBUG_LOG_STRING("pwr\r\n");
            }
            else if(keynum == VOL_UP_KEYNUM && !flash_read(HD_VOL_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                ir_remote_learn_send(irparams);
                DEBUG_LOG_STRING("vol +\r\n");
            }
            else if(keynum == VOL_DOWN_KEYNUM && !flash_read(HD_VOL__DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                ir_remote_learn_send(irparams);
                DEBUG_LOG_STRING("vol -\r\n");
            }
            else if(keynum == AVTV_KEYNUM && !flash_read(HD_AVTV_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                ir_remote_learn_send(irparams);
                DEBUG_LOG_STRING("av tv\r\n");
            }

            if(key_pressed_num == 1){
                set_key_press_state(true);
            }
        }
        else if(keynum == OK_Keynum || keynum == Server_Keynum)
        {
            key_pressed_time++;
            DEBUG_LOG_STRING("2 key_pressed_time = %d \r\n",key_pressed_time);
            if(key_pressed_time > 30)
            {
                DEBUG_LOG_STRING("2  1\r\n");
                bool le_connected_state = bt_check_le_connected();
                if(!le_connected_state)
                {
                    if(ir_format == PZ_OCN) {
                        if(ir_format != ir_format_state) {
                            ir_format_state = ir_format;
                            ir_type_init(DVB_40BIT_100,CUSTOM_434F_004E_A);
                        }
                    }
                    else if(ir_format == SA00_RC5) {
                        if(ir_format != ir_format_state) {
                            ir_format_state = ir_format;
                            ir_type_init(SAA3010_38_45,CUSTOM_0000_B000_A);
                        }
                    }
                    else if(ir_format == NEC6122) {
                        if(ir_format != ir_format_state) {
                            ir_format_state = ir_format;
                            ir_type_init(UPD6121G2_68,CUSTOM_00_7F_A);
                        }
                    }

                    if(keynum == OK_Keynum)
                    {
                        if(ir_format == PZ_OCN) {
                            ir_comm_send(PZ_OCN_ir_keynum[36]);
                        }
                        else if(ir_format == SA00_RC5) {
                            ir_comm_send(SA00_RC5_ir_keynum[36]<<2);
                        }
                        else if(ir_format == NEC6122) {
                            ir_comm_send(NEC6122_ir_keynum[36]);
                        }
                    }
                    else if(keynum == Server_Keynum)
                    {
                        if(ir_format == PZ_OCN) {
                            ir_comm_send(PZ_OCN_ir_keynum[35]);
                        }
                        else if(ir_format == SA00_RC5) {
                            ir_comm_send(SA00_RC5_ir_keynum[35]<<2);
                        }
                        else if(ir_format == NEC6122) {
                            ir_comm_send(NEC6122_ir_keynum[35]);
                        }
                    }
                    // set_key_press_state(true);
                    
                }else{
                    if(keynum == OK_Keynum)
                    {
                        keynum = 36;
                    }
                    else if(keynum == Server_Keynum)
                    {
                        keynum = 35;
                    }
                    uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
                    memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                    memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, KeyBuf[keynum].key_send_len);
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
                return;
            }
            else if(key_pressed_num == 0 && key_pressed_time < 30) 
            {
                bool le_connected_state = bt_check_le_connected();
                if(!le_connected_state)
                {
                    if(ir_format == PZ_OCN) {
                        if(ir_format != ir_format_state) {
                            ir_format_state = ir_format;
                            ir_type_init(DVB_40BIT_100,CUSTOM_434F_004E_A);
                        }
                        ir_single_send(PZ_OCN_ir_keynum[keynum], 0);
                    }
                    else if(ir_format == SA00_RC5) {
                        if(ir_format != ir_format_state) {
                            ir_format_state = ir_format;
                            ir_type_init(SAA3010_38_45,CUSTOM_0000_B000_A);
                        }
                        ir_single_send(SA00_RC5_ir_keynum[keynum]<<2, 0);
                    }
                    else if(ir_format == NEC6122) {
                        if(ir_format != ir_format_state) {
                            ir_format_state = ir_format;
                            ir_type_init(UPD6121G2_68,CUSTOM_00_7F_A);
                        }
                        ir_single_send(NEC6122_ir_keynum[keynum], 0);
                    }
                    
                }
                else{
                    uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
                    memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                    memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, KeyBuf[keynum].key_send_len);
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
            }
            else {
                DEBUG_LOG_STRING("long \r\n");
                swtimer_restart(key_pressed_timernum);
            }
        }
        else if(!le_connected_state)
        {
            if(ir_format == PZ_OCN) {
                if(ir_format != ir_format_state) {
                    ir_format_state = ir_format;
                    ir_type_init(DVB_40BIT_100,CUSTOM_434F_004E_A);
                }
                ir_comm_send(PZ_OCN_ir_keynum[keynum]);
            }
            else if(ir_format == SA00_RC5) {
                if(ir_format != ir_format_state) {
                    ir_format_state = ir_format;
                    ir_type_init(SAA3010_38_45,CUSTOM_0000_B000_A);
                }
                ir_comm_send(SA00_RC5_ir_keynum[keynum]<<2);
            }
            else if(ir_format == NEC6122) {
                if(ir_format != ir_format_state) {
                    ir_format_state = ir_format;
                    ir_type_init(UPD6121G2_68,CUSTOM_00_7F_A);
                }
                ir_comm_send(NEC6122_ir_keynum[keynum]);
            }
            
            if(key_pressed_num == 1) {
                set_key_press_state(true);
            }
        }
    }
    else if (key_pressed_num == 2)
    {
        if (keynum == xing_Keynum && keynum_second == jing_Keynum)
		{
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                led_state = LED_STATE_PAIR;
                key_pressed_time = 0;
                led_on(LED_1,200,60000);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10,true);
                return ;
            }
            swtimer_restart(key_pressed_timernum);

        }
    }
    else
    {
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
        if(send_state == NORMAL)
        {
            if (le_connected_state && encrypt_state && !(keynum == Set_ir_keynum || keynum == PWR_KEYNUM || keynum == VOL_UP_KEYNUM || keynum == VOL_DOWN_KEYNUM || keynum == AVTV_KEYNUM))
            {
                uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];

                if(keynum == Voice_Keynum && voice_send_state == true) {
                    swtimer_stop(vioce_send_timernum);
                    uint8_t hid_send_buf[2] = {0x0,0x0};
                    ATT_sendNotify(125, (void*)hid_send_buf, 2);
                    voice_send_state = false;
                    DEBUG_LOG_STRING("1 \r\n");
                }else if(keynum != Voice_Keynum){
                    led_off(LED_NUM);
                    memset((void *)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                    ATT_sendNotify(KeyBuf[keynum].handle, hid_send_buf, KeyBuf[keynum].key_send_len);
                    DEBUG_LOG_STRING("2  \r\n");
                }
            }else {
                set_key_press_state(false);
                if(led_state != LED_STATE_PAIR){
                    led_off(LED_NUM);
                }
            }

        }

    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] +
                    key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] +
                    key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5] ;
        if(send_state == NORMAL)
        {
            if(keynum == Set_ir_keynum) {
                key_pressed_time = 0;
                if(led_state != LED_STATE_PAIR){
                    PWM_Config(LED_1_PIN, 1000, 50, 0);
                }
                swtimer_start(key_pressed_timernum, 1000, TIMER_START_ONCE);
            }
            else if(keynum == PWR_KEYNUM || keynum == VOL_UP_KEYNUM || keynum == VOL_DOWN_KEYNUM || keynum == AVTV_KEYNUM){
                if(led_state != LED_STATE_PAIR){
                    led_on(LED_1,100,0);
                }
                swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);                
            }
            else if(keynum == OK_Keynum || keynum == Server_Keynum){
                if(led_state != LED_STATE_PAIR){
                    led_on(LED_1,100,0);
                }
                key_pressed_time = 0;
                swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
            }
            else if (le_connected_state && encrypt_state)
            {                
                uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];

                memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, KeyBuf[keynum].key_send_len);
                if (keynum == Voice_Keynum && !voice_status.mic_open_flag)
                {
                    uint8_t voice_send[2] = {0x21,0x02};
                    ATT_sendNotify(125, (void*)voice_send, 2);
                    voice_key_state = true;
                    voice_send_state = true;
                    voice_status.search = true;
                    voice_status_change();
                    app_sleep_lock_set(AUDIO_LOCK, true);

                    swtimer_start(vioce_timernum, 8000, TIMER_START_ONCE);
                    swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                    led_on(LED_1,0,0);
                
                }
                else if(keynum != Voice_Keynum) {
                    led_on(LED_1,100,0);
                    ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                }
            }
            else {
                if(led_state != LED_STATE_PAIR){
                    led_on(LED_1,100,0);
                }
                swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
            }


        }
        else if(send_state == IR_RECEIVE_STATE)
        {
            if(keynum == Set_ir_keynum)
            {
                swtimer_stop(ir_receive_timernum);
                continu_ir_receive = false;
                key_pressed_time = 0;
                send_state = NORMAL;
                DEBUG_LOG_STRING("Set_ir_keynum ok\r\n");
                led_state = LED_STATE_NORMAL;
                led_off(LED_1);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
            else {
                DEBUG_LOG_STRING("else  ok\r\n");
            }
        }
        else if(send_state == IR_RECEIVE_JUDGE_STATE)
        {
            if(keynum == PWR_KEYNUM)
            {
                led_state = LED_STATE_CONTINU;
                led_on(LED_1, 200, 600);
                DEBUG_LOG_STRING("PWR ok\r\n");
                flash_write(HD_POWER_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);

                key_pressed_time = 0;
                continu_ir_receive = 2;
                ir_receive_keynum[ir_state_keynum++] = keynum;
                send_state = IR_RECEIVE_STATE;
            }
            else if(keynum == VOL_UP_KEYNUM)
            {
                led_state = LED_STATE_CONTINU;
                led_on(LED_1, 200, 600);
                DEBUG_LOG_STRING("VOL_H ok\r\n");
                flash_write(HD_VOL_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);

                key_pressed_time = 0;
                continu_ir_receive = 2;
                ir_receive_keynum[ir_state_keynum++] = keynum;
                send_state = IR_RECEIVE_STATE;
            }
            else if(keynum == VOL_DOWN_KEYNUM)
            {
                led_state = LED_STATE_CONTINU;
                led_on(LED_1, 200, 600);
                DEBUG_LOG_STRING("VOL_L ok\r\n");
                flash_write(HD_VOL__DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);

                key_pressed_time = 0;
                continu_ir_receive = 2;
                ir_receive_keynum[ir_state_keynum++] = keynum;
                send_state = IR_RECEIVE_STATE;
            }
            else if(keynum == AVTV_KEYNUM)
            {
                led_state = LED_STATE_CONTINU;
                led_on(LED_1, 200, 600);
                DEBUG_LOG_STRING("AVTV ok\r\n");
                flash_write(HD_AVTV_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);

                key_pressed_time = 0;
                continu_ir_receive = 2;
                ir_receive_keynum[ir_state_keynum++] = keynum;
                send_state = IR_RECEIVE_STATE;
            }
            else if(keynum == Set_ir_keynum)
            {
                swtimer_stop(ir_receive_timernum);

                continu_ir_receive = false;
                key_pressed_time = 0;
                send_state = NORMAL;
                DEBUG_LOG_STRING("Set_ir_keynum ok\r\n");
                led_state = LED_STATE_NORMAL;
                led_off(LED_1);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
        }

    }
    else if (key_pressed_num == 2)
	{
        led_off(LED_NUM);
        set_key_press_state(false);
        if (key_report->keynum_report_buf[One_Row] == One_keynum && key_report->keynum_report_buf[Set_ir_Row] == Set_ir_keynum)
		{
            DEBUG_LOG_STRING("000\r\n");
            ir_format = 0;
            led_on(LED_1, 0, 200);
            flash_write(ir_format_tag, (uint8_t *)&ir_format, sizeof(ir_format), STATE_INF);
        }
        else if (key_report->keynum_report_buf[Two_Row] == Two_keynum && key_report->keynum_report_buf[Set_ir_Row] == Set_ir_keynum)
		{
            DEBUG_LOG_STRING("111\r\n");
            ir_format = 1;
            led_on(LED_1, 0, 200);
            flash_write(ir_format_tag, (uint8_t *)&ir_format, sizeof(ir_format), STATE_INF);
        }
        else if (key_report->keynum_report_buf[Three_Row] == Three_keynum && key_report->keynum_report_buf[Set_ir_Row] == Set_ir_keynum)
		{
            DEBUG_LOG_STRING("222\r\n");
            ir_format = 2;
            led_on(LED_1, 0, 200);
            flash_write(ir_format_tag, (uint8_t *)&ir_format, sizeof(ir_format), STATE_INF);
        }
        else if (key_report->keynum_report_buf[xing_Row] == xing_Keynum && key_report->keynum_report_buf[jing_Row] == jing_Keynum)
		{
            key_pressed_time = 0;    
            keynum = xing_Keynum;
            keynum_second = jing_Keynum;
            swtimer_start(key_pressed_timernum, 1000, TIMER_START_ONCE);
            DEBUG_LOG_STRING("* # \r\n");
        }
    }
    else if (key_pressed_num == 3)
    {
        led_off(LED_NUM);
        set_key_press_state(false);
    }
}

void action_after_mic_close(void)
{
    led_off(LED_1);
    voice_key_state = false;
}

void action_after_led_blk(void)
{
    if(led_state == LED_STATE_SUCCESS || led_state == LED_STATE_FAIL) {
        led_state = LED_STATE_NORMAL;
        led_on(LED_1, 0, 0);
    }else if(led_state == LED_STATE_CONTINU) {
        led_state = LED_STATE_NORMAL;
        continu_ir_receive =false;
        led_on(LED_1, 0, 0);
    }else if(led_state == LED_STATE_PAIR){
        led_state = LED_STATE_NORMAL;
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
            ATT_sendNotify(OCN_BAT_REPORT_HANDLE, &batlevel, 1);
        }
    }
    else {
        memcpy((void *)Att_List[BAT_ATTLIST_INDEX].dataPtr, (void *)&batlevel, 1);
        memcpy((void *)Att_List[OCN_BAT_ATTLIST_INDEX].dataPtr, (void *)&batlevel, 1);
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
    DEBUG_LOG_STRING("is_sleep  %d\r\n",is_sleep);
    if (is_sleep) {
        bt_le_conn_updata_param_req(0x08, 0x08, 99, 400);
    }
    else {
        bt_le_conn_updata_param_req(0x08, 0x08, 0, 400);
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
            enter_deep_sleep();
            voice_led_state = true;
        }
        else{
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

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if(HREADW(mem_le_slave_latency) == 99) {
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
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
    encrypt_state =true;
}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");

    app_sleep_timer_set(PAIR_DONE_DELAY);
    if(bt_check_save_connect_info()){
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
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
    if(voice_key_state){
        led_off(LED_NUM);
        voice_key_state = false;
        swtimer_stop(vioce_send_timernum);
        swtimer_stop(vioce_timernum);
    }    
    if (reason == 0x13) {
        enter_deep_sleep();
        return;
    }
    
    app_queue_reset();
    remote_control_reinit();
    
    if(Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x10, false);
        swtimer_start(adv_timernum, 3000, TIMER_START_ONCE);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x30, true);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    bt_set_lpm_overhead_wake_time(0x0b);
    bt_set_le_state(BLE_CONNECTED);
    bt_set_le_mtu_size(251);
    bt_disable_le_tx_md();
    Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    ATT_SendExchangeMtuReq();
    update_conn_param(false);
    led_state = LED_STATE_NORMAL;
    led_off(LED_NUM);
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

    if(Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x10, false);
        swtimer_start(adv_timernum, 2000, TIMER_START_ONCE);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x30, true);
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
        bt_set_ce_length_num(0x0f);
        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();

        GPIO_Init(IR_RCV_PIN, GPIO_Mode_Out_High);

        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);

        key_pressed_timernum = swtimer_add(key_pressed_handle);
        ir_receive_timernum = swtimer_add(ir_receive_handle);
        adv_timernum = swtimer_add(adv_handle);
        vioce_send_timernum = swtimer_add(voice_send_handle);
        vioce_timernum = swtimer_add(mic_close);

        if (!flash_record_exist(ir_format_tag)) {
            uint16_t len = sizeof(ir_format);
            flash_read(ir_format_tag, (void *)&ir_format, len);
        }
        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
    else
    {
        app_queue_reset();

        if (key_wakeup_get()) {
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP \r\n");
        }
        else {
            enter_low_sleep();
            DEBUG_LOG_STRING("KEEP CONNECT \r\n");
        }
    }
}
