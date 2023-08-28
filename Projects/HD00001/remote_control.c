#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_systick.h"
#include "yc_debug.h"
#include "att.h"
#include "app_config.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "software_timer.h"
#include "vbat.h"
#include "keyscan.h"
#include "led.h"
#include "flash.h"
#include "audio.h"
#include "mouse.h"
#include "hd_math.h"
#include "ir_send.h"
#include "ir_load.h"
#include "ir_lib.h"
#include "ir_select.h"
#include "ir_learn_from_remote.h"
#include "reg_addr.h"
#define mouse_angle_range   (127.0f)
#define mouse_move_range    (35.0f)
#define mouse_ratio         (mouse_angle_range / mouse_move_range)

enum {
    NORMAL,
    AV_PRESS,
    DVB_PRESS,
    JUDGE_PRESS,
    AV_STATE,
    DVB_STATE,
    SEARCH_STATE,
    PAIR_STATE,
    POWER_STATE,
    VOL_STATE,
    FACTORY_DEFAULT,
    IR_RECEIVE_STATE,
    IR_RECEIVE_JUDGE_STATE,
    Down_Keynum = 9,
    Down_Row = 2,
    Back_Keynum = 11,
    Back_Row = 4,
    CH_keynum = 15,
    CH__keynum = 18,
    OK__Keynum = 24,
    OK__Row = 5,
    SEARCH_NUMBER = 475,
};

typedef struct {
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

typedef struct {
    uint8_t MouseNTFBuf[MOUSE_SEND_LEN];
}MouseNTFQue_TypeDef;

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x30, 0x00, 2, 41}, // POWER 1
    {0x00, 0x00, 2, 41}, //
    {0x00, 0x00, 2, 41}, //
    {0x40, 0x00, 2, 41}, // MEHIO
    {0xC0, 0x01, 2, 41}, // YOU >>

    {0x42, 0x00, 2, 41}, // UP  6
    {0x44, 0x00, 2, 41}, // LIFT
    {0x45, 0x00, 2, 41}, // RIGHT
    {0x43, 0x00, 2, 41}, // DOWN
    {0xB4, 0x00, 2, 41}, // R LIFT

    {0x24, 0x02, 2, 41}, // BACK  11
    {0xB3, 0x00, 2, 41}, // R RIGHT
    {0xE9, 0x00, 2, 41}, // VOL +
    {0xCD, 0x00, 2, 41}, // ZAN TING
    {0x9C, 0x00, 2, 41}, // CH +

    {0xE2, 0x00, 2, 41}, // 16  MUTE
    {0xEA, 0x00, 2, 41}, // VOL-
    {0x9D, 0x00, 2, 41}, // CH-
    {0x83, 0x00, 2, 41}, // FANG KUANG
    {0x00, 0x00, 2, 74}, // FEISHU

    {0x21, 0x02, 2, 41}, //21  VOICE
    {0x1E, 0x00, 8, 74}, // 1
    {0x20, 0x00, 8, 74}, // 3
    {0x41, 0x00, 2, 41}, // OK
    {0x1F, 0x00, 8, 74}, // 2

    {0x21, 0x00, 8, 74}, // 26 4
    {0x23, 0x00, 8, 74}, // 6
    {0x22, 0x00, 8, 74}, // 5
    {0x24, 0x00, 8, 74}, // 7
    {0x26, 0x00, 8, 74}, // 9

    {0x25, 0x00, 8, 74}, // 8
    {0x27, 0x00, 8, 74}, // 0
};

static const uint8_t brand_number[] = {
    0XFF,

    0XFF, //1
    0XFF,
    0XFF,
    0XFF,
    0XFF,

    0XFF, //6
    0XFF,
    0XFF,
    0XFF,
    0XFF,

    0XFF, //11
    0XFF,
    0XFF,
    0XFF,
    0XFF,

    0XFF, //16
    0XFF,
    0XFF,
    0XFF,
    0XFF,

    0XFF, //21
    0X01, //1
    0X03, //3
    0XFF,
    0X02, //2

    0X04, //26 4
    0X06, //6
    0X05, //5
    0X07, //7
    0X09, //9

    0X08, //8
    0X00, //0
};

static const uint8_t adv_data_buf[] = {0x02, 0x01, 0x05, 0x03, 0x19, 0xC1, 0x03, 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18};
static const uint8_t scan_rsp_data_buf[] = {0x0D, 0x09, 0x57, 0x69, 0x6E, 0x6B, 0x20, 0x41, 0x6E, 0x64, 0x72, 0x6F, 0x69, 0x64};

static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static euler_angles_t euler_angles_offset;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t mouse_flag;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static bool mouse_on_off;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static bool continu_ir_receive = false;
static bool llcp_updata;

static ir_brand_init_t ir_brand_send;
static ir_brand_init_t ir_judge;
static uint8_t brand_cnt;
static uint8_t ir_done;    // 0是正常发送 1、2、是av dvb按下4s的判断 3、4、是需要4次键号  5 是强连
static uint8_t AV_DVB_VALUE;
static uint16_t brand_code;
static uint8_t ir_state;

static uint8_t ir_receive_keynum[5];
static uint8_t ir_state_keynum;

static bool vol_state = true;
static bool power_state = false;
static uint16_t search_ir;
static uint32_t av_brand_code = 60;
static uint16_t dvb_brand_code = 3220;

static uint16_t key_pressed_time;
static uint8_t key_pressed_num;
static uint8_t keynum_secnd;
static uint8_t keynum;

static uint8_t vioce_timernum = 0xFF;
static uint8_t led_state_timernum = 0xFF;
static uint8_t brand_pressed_timernum = 0xFF;
static uint8_t power_timernum = 0xFF;
static uint8_t power_led_timernum = 0xFF;
static uint8_t pair_done_timernum = 0xFF;
static uint8_t ir_done_timernum = 0xFF;

static bool tx_power_switch;

static void ir_learn_remote_clr(uint8_t keynum){
    switch(keynum)
    {
        case TV:
            flash_Erase(HD_TV_DATA_OLD,sizeof(irparams));
            break;
        case AV:
            flash_Erase(HD_AV_DATA_OLD,sizeof(irparams));
            break;
        case VOL_H:
            flash_Erase(HD_VOL_DATA_OLD,sizeof(irparams));
            break;
        case VOL_L:
            flash_Erase(HD_VOL__DATA_OLD,sizeof(irparams));
            break;
        case METU:
            flash_Erase(HD_MUTE_DATA_OLD,sizeof(irparams));
            break;
        case 0xFF:
            flash_Erase(HD_TV_DATA,sizeof(irparams));
            flash_Erase(HD_AV_DATA,sizeof(irparams));
            flash_Erase(HD_VOL_DATA,sizeof(irparams));
            flash_Erase(HD_VOL__DATA,sizeof(irparams));
            flash_Erase(HD_MUTE_DATA,sizeof(irparams));
            break;
    }

}

static void ir_learn_remote_write(uint8_t keynum){
    switch(keynum)
    {
        case TV:
            flash_read(HD_TV_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams));
            flash_write(HD_TV_DATA,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);
            flash_Erase(HD_TV_DATA_OLD,sizeof(irparams));
            break;
        case AV:
            flash_read(HD_AV_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams));
            flash_write(HD_AV_DATA,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);
            flash_Erase(HD_AV_DATA_OLD,sizeof(irparams));
            break;
        case VOL_H:
            flash_read(HD_VOL_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams));
            flash_write(HD_VOL_DATA,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);
            flash_Erase(HD_VOL_DATA_OLD,sizeof(irparams));
            break;
        case VOL_L:
            flash_read(HD_VOL__DATA_OLD,(uint8_t *)&irparams,sizeof(irparams));
            flash_write(HD_VOL__DATA,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);
            flash_Erase(HD_VOL__DATA_OLD,sizeof(irparams));
            break;
        case METU:
            flash_read(HD_MUTE_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams));
            flash_write(HD_MUTE_DATA,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);
            flash_Erase(HD_MUTE_DATA_OLD,sizeof(irparams));
            break;
    }
}

static void pair_done_handle(void)
{
    ir_state = NORMAL;
    set_key_press_state(false);
    if (bt_check_le_connected()) {
        app_sleep_lock_set(APP_LOCK, false);
    }    
}

static void power_state_handle(void)
{
    set_key_press_state(false);
    SysTick_DelayMs(100);

    ir_brand_send = brand_AV_init(av_brand_code);
    ir_brand_send(TV);

    if(key_pressed_num == 1){
        set_key_press_state(true);
    }

    if (bt_check_le_connected()) {
        app_sleep_lock_set(APP_LOCK, false);
    }
}

static void power_led_state_handle(void)
{
    led_off(LED_2);
    led_on(LED_1, 0, 50);
    ir_state = NORMAL;
    if (bt_check_le_connected()) {
        app_sleep_lock_set(APP_LOCK, false);
    }    
}

static void led_state_handle(void)
{
    if (ir_state != NORMAL) {
        ir_state = NORMAL;
        if (bt_check_le_connected()) {
            app_sleep_lock_set(APP_LOCK, false);
        }
    }
}

static void ir_done_handle(void)
{
    set_key_press_state(false);
}

static void brand_pressed_handle(void)
{
    if (ir_state == AV_PRESS)
    {
        if (key_pressed_num == 2 && keynum == 24 && keynum_secnd == 7)
        {
            key_pressed_time++;

            if (key_pressed_time == 2)
            {
                AV_DVB_VALUE = 0XFF;
                led_off(LED_NUM);
                led_on(LED_1, 100, 400);
                brand_code = 0;
                key_pressed_time = 0;
                ir_state = JUDGE_PRESS;
            }
        }
        else {
            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }
    }
    else if (ir_state == DVB_PRESS)
    {
        DEBUG_LOG_STRING(" DVB_ok\r\n");

        if (key_pressed_num == 2 && keynum == 24 && keynum_secnd == 8)
        {
            key_pressed_time++;
            if (key_pressed_time == 2)
            {
                AV_DVB_VALUE = 0XFF;
                led_off(LED_NUM);
                led_on(LED_2, 100, 400);
                brand_code = 0;
                key_pressed_time = 0;
                ir_state = DVB_STATE;
            }
        }
        else {
            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }            
        }
    }
    else if(ir_state == JUDGE_PRESS) {
        if (!bt_check_le_connected()) {
            app_sleep_lock_set(ADV_LOCK, true);
        }

        key_pressed_time++;
        if (key_pressed_time >= 30){
            led_on(LED_1, 0, 400);
            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }
    }
    else if (ir_state == AV_STATE || ir_state == DVB_STATE)
    {
        if (!bt_check_le_connected()) {
            app_sleep_lock_set(ADV_LOCK, true);
        }
        key_pressed_time++;
        if ((key_pressed_num == 1) && ((AV_DVB_VALUE >= 0) && (AV_DVB_VALUE < 10))) {
            key_pressed_time = 0;
        }

        if (key_pressed_time >= 30)
        {
            if (ir_state == AV_STATE) {
                led_on(LED_1, 0, 400);
            }
            else {
                led_on(LED_2, 0, 400);
            }

            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }
    }
    else if (ir_state == SEARCH_STATE)
    {
        if (!bt_check_le_connected()) {
            app_sleep_lock_set(ADV_LOCK, true);
        }
        if(key_pressed_num == 1 && keynum == CH_keynum){
            key_pressed_time++;
            if(key_pressed_time == 30){
                key_pressed_time = 0; 
                led_on(LED_1, 0, 150);
                ++search_ir;
                if(search_ir > 475){
                    search_ir = 1;
                }
                ir_brand_send = brand_AV_init(search_ir_number[search_ir]);
                ir_brand_send(TV);
            }
        }
        else if(key_pressed_num == 1 && keynum == CH__keynum){
            key_pressed_time++;
            if(key_pressed_time == 30){
                key_pressed_time = 0; 
                led_on(LED_1, 0, 150);
                --search_ir;
                if(search_ir == 0){
                    search_ir = SEARCH_NUMBER;
                }
                ir_brand_send = brand_AV_init(search_ir_number[search_ir]);
                ir_brand_send(TV);
            }
        }
        else if(key_pressed_num == 0){
            key_pressed_time++;
            if(key_pressed_time == 300){
                flash_read(HD_SEARCH_STATE,(uint8_t*)&search_ir, sizeof(search_ir));
                DEBUG_LOG_STRING("ok= %d ok\r\n",key_pressed_time);
                led_on(LED_1, 0, 300);
                ir_state = NORMAL;
                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
        }
    }
    else if (ir_state == PAIR_STATE)
    {
        if (key_pressed_num == 2 && keynum == 11 && keynum_secnd == 4)
        {
            key_pressed_time++;
            DEBUG_LOG_STRING("key_pressed_time = %d\r\n", key_pressed_time);
            if (key_pressed_time == 3)
            {
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x30);
                
                ir_brand_send = brand_DVB_init(3220);
                ir_brand_send(2);
                set_key_press_state(true);
                led_off(LED_NUM);
                led_on(LED_2, 100, 45000);

                key_pressed_time = 0;
                swtimer_start(pair_done_timernum, 200, TIMER_START_ONCE);
                swtimer_stop(brand_pressed_timernum);              
            }
        }
        else {
            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }
    }
    else if (ir_state == POWER_STATE || ir_state == VOL_STATE)
    {
        if ((key_pressed_num == 2 && keynum == 24 && keynum_secnd == 1) || (key_pressed_num == 2 && keynum == 24 && keynum_secnd == 13))
        {
            key_pressed_time++;
            if (key_pressed_time == 3)
            {
                DEBUG_LOG_STRING("ir_state ok\r\n");
                if (ir_state == POWER_STATE)
                {
                    power_state = power_state ^ 0x01;
                    DEBUG_LOG_STRING("power_state = %d\r\n", power_state);
                    flash_write(HD_POWER_STATE, (uint8_t*)&power_state, sizeof(power_state), STATE_INF);
                    if (power_state == 0) {
                        led_on(LED_2, 0, 80);
                        swtimer_start(led_state_timernum, 300, TIMER_START_ONCE);
                    }
                    else {
                        led_on(LED_2, 0, 100);
                        swtimer_start(power_led_timernum, 510, TIMER_START_ONCE);
                    }
                }
                else
                {
                    vol_state = vol_state ^ 0x01;
                    DEBUG_LOG_STRING("502 vol_state = %d\r\n", vol_state);
                    flash_write(HD_VOL_STATE, (uint8_t*)&vol_state, sizeof(vol_state), STATE_INF);
                    led_off(LED_NUM);
                    ir_done = 0;
                    if (vol_state == 0) {
                        led_on(LED_1, 0, 80);
                    }
                    else {
                        led_on(LED_2, 0, 80);
                    }
                    swtimer_start(led_state_timernum, 300, TIMER_START_ONCE);
                }

                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
            }
        }
        else {
            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }
    }
    else if (ir_state == FACTORY_DEFAULT)
    {
        if (key_pressed_num == 2 && keynum == 24 && keynum_secnd == 11)
        {
            key_pressed_time++;
            DEBUG_LOG_STRING("ir_state = %d\r\n", ir_state);
            if (key_pressed_time == 5)
            {
                DEBUG_LOG_STRING("ir_state ok");
                av_brand_code = 60;
                flash_write(HD_AV_IR, (uint8_t*)&av_brand_code, sizeof(av_brand_code), STATE_INF);

                dvb_brand_code = 3220;
                flash_write(HD_DVB_IR, (uint8_t*)&dvb_brand_code, sizeof(dvb_brand_code), STATE_INF);

                power_state = 0;
                flash_write(HD_POWER_STATE, (uint8_t*)&power_state, sizeof(power_state), STATE_INF);

                vol_state = 1;
                flash_write(HD_VOL_STATE, (uint8_t*)&vol_state, sizeof(vol_state), STATE_INF);

                DEBUG_LOG_STRING("ir_DVB ok %d", av_brand_code);
                ir_learn_remote_clr(0XFF);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x30);
                led_on(LED_1, 50, 400);
                led_on(LED_2, 50, 400);

                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                swtimer_start(led_state_timernum, 370, TIMER_START_ONCE);   
            }
        }
        else {
            ir_state = NORMAL;
            key_pressed_time = 0;
            swtimer_stop(brand_pressed_timernum);
            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }
    }
    else if (ir_state == IR_RECEIVE_STATE)
    {
        if (key_pressed_num == 2 && keynum == OK__Keynum && keynum_secnd == Down_Keynum )
        {
            key_pressed_time++;
            if(key_pressed_time == 30)
            {
                continu_ir_receive = true;
                memset(ir_receive_keynum,0,sizeof(ir_receive_keynum));
                ir_state_keynum = 0;
                led_on(LED_1, 0, 0);
     
            }
        }
        else if(key_pressed_num == 0 && continu_ir_receive == true)
        {
            tempStartTimer = 0;
ir_fail:
            memset(&irparams,0,sizeof(irparams));
            led_on(LED_1, 0, 0);
            continu_ir_receive = false;
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
            if(JUDE_receive == IR_LEARN_OVERTIME )
            {
                led_on(LED_1, 0, 400);
                
                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                swtimer_start(led_state_timernum, 400, TIMER_START_ONCE);  
            }
            else if(JUDE_receive == IR_LEARN_SUCE)
            {
                key_pressed_time = 0;
                ir_state = IR_RECEIVE_JUDGE_STATE;
                led_on(LED_1, 200, 0);
            }
            else if(JUDE_receive == IR_LEARN_FAIL){
                goto ir_fail;
            }
        }
        else if(key_pressed_num == 0 && continu_ir_receive == false) {
            DEBUG_LOG_STRING("stop  \r\n");
            swtimer_stop(brand_pressed_timernum);
            ir_state = NORMAL;
            key_pressed_time = 0;

            if (bt_check_le_connected()) {
                app_sleep_lock_set(APP_LOCK, false);
            }
        }

    }
    else if(ir_state == IR_RECEIVE_JUDGE_STATE)
    {
        if (!bt_check_le_connected()) {
            app_sleep_lock_set(ADV_LOCK, true);
        }

        key_pressed_time++;
        DEBUG_LOG_STRING("key_pressed_time = %d\r\n",key_pressed_time);
        if(key_pressed_time >= 300){
            swtimer_stop(brand_pressed_timernum);
            // DEBUG_LOG_STRING("time out ok\r\n");
            key_pressed_time = 0;
            continu_ir_receive = false;
            memset(&irparams,0,sizeof(irparams));
            led_on(LED_1, 0, 400);
            swtimer_start(led_state_timernum, 400, TIMER_START_ONCE);  
        }

    }
}              

static void tv_dvb_brand(uint8_t brand_num)
{
    if ((brand_num >= 0) && (brand_num < 10))
    {
        if (brand_cnt < 4)
        {
            brand_code *= 10;
            brand_code += brand_num;
            brand_cnt++;
            // DEBUG_LOG_STRING(" brand_code = %d \r\n", brand_code);
            if (brand_cnt == 4)
            {
                if (ir_state == AV_STATE)
                {
                    ir_judge = brand_AV_init(brand_code);
                    if (ir_judge != NULL)
                    {
                        av_brand_code = brand_code;
                        for(uint16_t i  = 0 ;i < SEARCH_NUMBER ; i++){
                            if(search_ir_number[i] == av_brand_code){
                                search_ir = i;
                                flash_write(HD_SEARCH_STATE, (uint8_t*)&search_ir, sizeof(search_ir), STATE_INF);
                                DEBUG_LOG_STRING(" search_ir = %d\r\n",search_ir);
                                break;
                            }
                        }
                        vol_state = 0;
                        ir_learn_remote_clr(0XFF);
                        flash_write(HD_VOL_STATE, (uint8_t*)&vol_state, sizeof(vol_state), STATE_INF);
                        flash_write(HD_AV_IR, (uint8_t*)&av_brand_code, sizeof(av_brand_code), STATE_INF);
                        led_on(LED_1, 100, 700);
                        swtimer_start(led_state_timernum, 500, TIMER_START_ONCE);
                        DEBUG_LOG_STRING(" AV_STATE\r\n");
                    }
                    else {
                        led_on(LED_1, 0, 300);
                        swtimer_start(led_state_timernum, 300, TIMER_START_ONCE);
                    }
                }
                else if (ir_state == DVB_STATE)
                {
                    ir_judge = brand_DVB_init(brand_code);
                    if (ir_judge != NULL)
                    {
                        dvb_brand_code = brand_code;
                        flash_write(HD_DVB_IR, (uint8_t*)&dvb_brand_code, sizeof(dvb_brand_code), STATE_INF);
                        led_on(LED_2, 100, 700);
                        swtimer_start(led_state_timernum, 500, TIMER_START_ONCE);
                        DEBUG_LOG_STRING(" DVB_STATE\r\n");
                    }
                    else {
                        led_on(LED_2, 0, 300);
                        swtimer_start(led_state_timernum, 300, TIMER_START_ONCE);
                    }
                }
                ir_done = 0;
                brand_code = 0;
                brand_cnt = 0;
                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
        }
    }
    else if (brand_num == 0XF1)//back按键
    {
        led_off(LED_NUM);
        brand_code = 0;
        brand_cnt = 0;
        key_pressed_time = 0;
        if (ir_state == AV_STATE) {
            led_on(LED_1, 0, 400);
        }
        else {
            led_on(LED_2, 0, 400);
        }
        DEBUG_LOG_STRING(" 463 \r\n");
        swtimer_start(led_state_timernum, 400, TIMER_START_ONCE);
        swtimer_stop(brand_pressed_timernum);
        if (bt_check_le_connected()) {
            app_sleep_lock_set(APP_LOCK, false);
        }
    }
}

static float mouse_data_check(float data)
{
    if (data > mouse_angle_range) {
        data = mouse_angle_range;
    }
    else if (data < -mouse_angle_range) {
        data = -mouse_angle_range;
    }

    return data;
}

static void mouse_handle(void *data)
{
    sensor_data_t *sensor_data = (sensor_data_t *)data;
    euler_angles_t euler_angles;
    memset(&euler_angles, 0, sizeof(euler_angles_t));
    EulerAngle_Calculate(&euler_angles, -sensor_data->ay, sensor_data->ax, sensor_data->az, -sensor_data->gy, sensor_data->gx, sensor_data->gz);
    // DEBUG_LOG_STRING("pitch = [%d][%d], yaw = [%d][%d] \r\n", (int)(euler_angles.pitch), (int)(euler_angles.pitch*1000), (int)(euler_angles.yaw), (int)(euler_angles.yaw*1000));
    // DEBUG_LOG_STRING("MOUSE DATA: AX[%d] AY[%d] AZ[%d] GX[%d] GY[%d] GZ[%d] \r\n", (int)sensor_data->ax, (int)sensor_data->ay, (int)sensor_data->az, (int)sensor_data->gx, (int)sensor_data->gy, (int)sensor_data->gz);
    float left_right_temp, up_down_temp;
    uint8_t left_right, up_down;

    left_right_temp = euler_angles.yaw - euler_angles_offset.yaw ;
    // if (left_right_temp > 0.065f) {
    if (left_right_temp > 0.08f) {
        left_right = (uint8_t)mouse_data_check(left_right_temp * mouse_ratio * 33.555f);
        app_sleep_lock_set(MOUSE_LOCK, true);
    }
    // else if (left_right_temp < -0.065f) {
    else if (left_right_temp < -0.08f) {        
        left_right = (uint8_t)(256.0f + mouse_data_check(left_right_temp * mouse_ratio * 33.555f));
        app_sleep_lock_set(MOUSE_LOCK, true);
    }
    else {
        left_right = 0x00;
    }

    up_down_temp = euler_angles.pitch - euler_angles_offset.pitch;
    // if (up_down_temp > 0.065f) {
    if (up_down_temp > 0.1f) {
        up_down = (uint8_t)mouse_data_check(up_down_temp * mouse_ratio * 21.555f);
        app_sleep_lock_set(MOUSE_LOCK, true);
    }
    // else if (up_down_temp < -0.065f) {
    else if (up_down_temp < -0.1f) {
        up_down = (uint8_t)(256.0f + mouse_data_check(up_down_temp * mouse_ratio * 21.555f));
        app_sleep_lock_set(MOUSE_LOCK, true);
    }
    else {
        up_down = 0x00;
    }

    memcpy(&euler_angles_offset, &euler_angles, sizeof(euler_angles_t));
    
    uint8_t send_buf[MOUSE_SEND_LEN] = {0, 0, 0, 0};
    if (mouse_flag < 20) {
        mouse_flag++;
    }
    else {
        send_buf[1] = left_right;
        send_buf[2] = up_down;
    }
    ATT_sendNotify(MOUSE_SNED_HANDLE, send_buf, MOUSE_SEND_LEN);
}

static void keyvalue_handle(key_report_t *key_report)
{
    key_pressed_num = key_report->key_press_cnt;

    if (key_pressed_num == 0)
    {
#ifdef AUDIO_TEST_MODE
        mic_close();
#else
        if (bt_check_le_connected() && ir_state == NORMAL)
        {
            if (keynum != MOUSE_KEY && !mouse_on_off)
            {
                uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
                memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);

                if (KeyBuf[keynum].key_send_len == 8) {
                    hid_send_buf[2] = hid_send_buf[0];
                    hid_send_buf[0] = 0x00;
                }
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                
				if (keynum != 21) {
                    led_off(LED_2);
                }
            }
        }

        if (ir_state == NORMAL) {
            keynum = 0;
            led_off(LED_1);
        }

        set_key_press_state(false);
#endif
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] + key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] + key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];
        // DEBUG_LOG_STRING(" ir_state av = %d\r\n",ir_state);
        // DEBUG_LOG_STRING("KEYNUM [%d] [%d] [%d] [%d] [%d] [%d]\r\n", key_report->keynum_report_buf[0],key_report->keynum_report_buf[1],key_report->keynum_report_buf[2],key_report->keynum_report_buf[3],key_report->keynum_report_buf[4],key_report->keynum_report_buf[5]);
#ifdef AUDIO_TEST_MODE
        if (keynum == 6) {
            voice_status.mode = STANDARD_DATA;
            mic_open();
        }
        else if (keynum == 24) {
        }
        else if (keynum == 9) {
            voice_status.mode = ENCODE_DATA; 
            mic_open();
        }
        voice_status.mode = PCM_DATA;
        mic_open();
#else
        if (bt_check_le_connected() && ir_state == NORMAL)
        {
            if (keynum == MOUSE_KEY) {
                mouse_switch();
            }
            else if (!mouse_on_off)
            {
                if (!((power_state == 1 && keynum == PWR_KEYNUM) || (vol_state == 0 && (keynum == VOL_H || keynum == VOL_L || keynum == METU))
                        || keynum == TV || keynum == AV)) {
                    uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
                    memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                    memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);

                    if (KeyBuf[keynum].key_send_len == 8) {
                        hid_send_buf[2] = hid_send_buf[0];
                        hid_send_buf[0] = 0x00;
                    }
                    
                    if (keynum == 21 && !voice_status.mic_open_flag)
                    {
                        update_conn_param(false);
                        ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                        app_sleep_lock_set(AUDIO_LOCK, true);

                        voice_status.search = true;
                        voice_status_change();
                        swtimer_start(vioce_timernum, 5000, TIMER_START_ONCE);
						led_on(LED_2, 0, 0);
                    }
					else {
                        ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
						led_on(LED_2, 150, 0);
					}
                }
            }
            else if (keynum == MOUSE_OK) {
                uint8_t send_buf[MOUSE_SEND_LEN] = {0x01, 0x00, 0x00, 0x00};
                ATT_sendNotify(MOUSE_SNED_HANDLE, (void*)send_buf, MOUSE_SEND_LEN);
            }
        }
        
        if(ir_state == JUDGE_PRESS)
        {
            AV_DVB_VALUE = brand_number[keynum];
            if ((AV_DVB_VALUE >= 0) && (AV_DVB_VALUE < 10)) {
                key_pressed_time = 0;
                ir_state = AV_STATE;
            }
            else if (keynum == CH_keynum ||keynum == CH__keynum) {
                key_pressed_time = 0;
                ir_done = 0;
                ir_state = SEARCH_STATE;
                swtimer_stop(brand_pressed_timernum);
                swtimer_start(brand_pressed_timernum,100,TIMER_START_REPEAT);
            }else if(keynum == Back_Keynum){

                led_on(LED_1, 0, 400);
                swtimer_start(led_state_timernum, 300, TIMER_START_ONCE);
                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
        }

        if (ir_state == AV_STATE) {
            led_on(LED_1, 0, 150);
            tv_dvb_brand(brand_number[keynum]);
            AV_DVB_VALUE = brand_number[keynum];
        }
        else if (ir_state == DVB_STATE) {
            led_on(LED_2, 0, 150);
            tv_dvb_brand(brand_number[keynum]);
            AV_DVB_VALUE = brand_number[keynum];
        }
        else if(ir_state == SEARCH_STATE)
        {
            if(keynum == CH_keynum)
            {
                led_on(LED_1, 0, 150);
                key_pressed_time = 0;
                ++search_ir;
                if(search_ir > 475){
                    search_ir = 1;
                }
                ir_brand_send = brand_AV_init(search_ir_number[search_ir]);
                ir_brand_send(TV);
                set_key_press_state(true);
                swtimer_start(ir_done_timernum,200,TIMER_START_ONCE);
            }
            else if(keynum == CH__keynum)
            {
                led_on(LED_1, 0, 150);
                key_pressed_time = 0;
                --search_ir;
                if(search_ir == 0){
                    search_ir = SEARCH_NUMBER;
                }
                ir_brand_send = brand_AV_init(search_ir_number[search_ir]);
                ir_brand_send(TV);
                set_key_press_state(true);
                swtimer_start(ir_done_timernum,200,TIMER_START_ONCE);
            }
            else if(keynum == OK__Keynum)
            {
                av_brand_code = search_ir_number[search_ir];
                vol_state = 0;
                flash_write(HD_VOL_STATE, (uint8_t*)&vol_state, sizeof(vol_state), STATE_INF);
                flash_write(HD_AV_IR, (uint8_t*)&av_brand_code, sizeof(av_brand_code), STATE_INF);
                flash_write(HD_SEARCH_STATE, (uint8_t*)&search_ir, sizeof(search_ir), STATE_INF);
                ir_learn_remote_clr(0xFF);
                led_on(LED_1, 100, 700);
                swtimer_start(led_state_timernum, 500, TIMER_START_ONCE);
                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
            else if(keynum == Back_Keynum){
                flash_read(HD_SEARCH_STATE,(uint8_t*)&search_ir, sizeof(search_ir));

                led_on(LED_1, 0, 400);
                swtimer_start(led_state_timernum, 300, TIMER_START_ONCE);
                key_pressed_time = 0;
                swtimer_stop(brand_pressed_timernum);
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
        }
        else if(ir_state == IR_RECEIVE_STATE)
        {
            if(keynum == Back_Keynum){
                swtimer_stop(brand_pressed_timernum);
                
                key_pressed_time = 0;

                DEBUG_LOG_STRING("Back_Keynum ok\r\n");
                for(uint8_t i = 0; i < ir_state_keynum; i++){
                    ir_learn_remote_clr(ir_receive_keynum[i]);
                }

                led_on(LED_1, 600, 600);
                swtimer_start(led_state_timernum, 560, TIMER_START_ONCE); 
            }
            else if(keynum == OK__Keynum){
                swtimer_stop(brand_pressed_timernum);
                DEBUG_LOG_STRING("OK__Keynum ok\r\n");
                key_pressed_time = 0;
                
                for(uint8_t i = 0; i < ir_state_keynum; i++){
                    ir_learn_remote_write(ir_receive_keynum[i]);
                }
                led_on(LED_1, 200, 1600);
                swtimer_start(led_state_timernum, 750, TIMER_START_ONCE); 
            }
            else{
                led_on(LED_1, 0, 0);   
                DEBUG_LOG_STRING("else  ok\r\n");
                ir_state = IR_RECEIVE_STATE;
                continu_ir_receive = true;
                swtimer_start(brand_pressed_timernum, 50, TIMER_START_REPEAT);
            }
        }
        else if(ir_state == IR_RECEIVE_JUDGE_STATE)
        {
            if(keynum == TV)
            {
                led_on(LED_1, 0, 0);
                DEBUG_LOG_STRING("TV ok\r\n");
                flash_write(HD_TV_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);

                ir_receive_keynum[ir_state_keynum++] = keynum;

                continu_ir_receive = true;
                ir_state = IR_RECEIVE_STATE;
                swtimer_start(brand_pressed_timernum, 50, TIMER_START_REPEAT);  
            }
            else if(keynum == AV)
            {
                led_on(LED_1, 0, 0);
                DEBUG_LOG_STRING("AV ok\r\n");
                flash_write(HD_AV_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);

                ir_receive_keynum[ir_state_keynum++] = keynum;

                continu_ir_receive = true;
                ir_state = IR_RECEIVE_STATE;
                swtimer_start(brand_pressed_timernum, 50, TIMER_START_REPEAT);  
            }
            else if(keynum == VOL_H)
            {
                led_on(LED_1, 0, 0);
                DEBUG_LOG_STRING("VOL_H ok\r\n");
                flash_write(HD_VOL_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);

                ir_receive_keynum[ir_state_keynum++] = keynum;

                continu_ir_receive = true;
                ir_state = IR_RECEIVE_STATE;
                swtimer_start(brand_pressed_timernum, 50, TIMER_START_REPEAT);  
            }
            else if(keynum == VOL_L)
            {
                led_on(LED_1, 0, 0);
                DEBUG_LOG_STRING("VOL_L ok\r\n");
                flash_write(HD_VOL__DATA_OLD,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);

                ir_receive_keynum[ir_state_keynum++] = keynum;

                continu_ir_receive = true;
                ir_state = IR_RECEIVE_STATE;
                swtimer_start(brand_pressed_timernum, 50, TIMER_START_REPEAT);  
            }
            else if(keynum == METU)
            {
                led_on(LED_1, 0, 0);
                DEBUG_LOG_STRING("VOL_L ok\r\n");                
                flash_write(HD_MUTE_DATA_OLD,(uint8_t *)&irparams,sizeof(irparams),CHUNK_INF);

                ir_receive_keynum[ir_state_keynum++] = keynum;

                continu_ir_receive = true;
                ir_state = IR_RECEIVE_STATE;
                swtimer_start(brand_pressed_timernum, 50, TIMER_START_REPEAT);  
            }
            else if(keynum == Back_Keynum)
            {
                led_on(LED_1, 600, 600);
                swtimer_stop(brand_pressed_timernum);
                for(uint8_t i = 0; i < ir_state_keynum; i++){
                    ir_learn_remote_clr(ir_receive_keynum[i]);
                }
                key_pressed_time = 0;
                swtimer_start(led_state_timernum, 800, TIMER_START_ONCE); 
            }
            else if(keynum == OK__Keynum)
            {
                led_on(LED_1, 200, 1600);
                swtimer_stop(brand_pressed_timernum);
                for(uint8_t i = 0; i < ir_state_keynum; i++){
                    ir_learn_remote_write(ir_receive_keynum[i]);
                }
                key_pressed_time = 0;
                swtimer_start(led_state_timernum, 750, TIMER_START_ONCE); 
            }
        }
        else if (ir_state == NORMAL)
        {
            if ((keynum == TV || keynum == AV))
            {
                led_on(LED_1, 30, 0);
                if(keynum == TV && !flash_read(HD_TV_DATA,(uint8_t *)&irparams,sizeof(irparams))){
                    SysTick_DelayMs(120);
                    ir_remote_learn_send(irparams);
                    set_key_press_state(true);
                }
                else if(keynum == AV && !flash_read(HD_AV_DATA,(uint8_t *)&irparams,sizeof(irparams))){
                    SysTick_DelayMs(120);
                    ir_remote_learn_send(irparams);
                    set_key_press_state(true);
                }
                else{
                    if (ir_done != 1) {
                        ir_done = 1;
                        ir_brand_send = brand_AV_init(av_brand_code);
                    }
                    SysTick_DelayMs(100);
                    ir_brand_send(keynum);
                    set_key_press_state(true);
                    DEBUG_LOG_STRING(" tv av = %d\r\n",ir_state);
                }
            }
            else if (keynum == VOL_H || keynum == VOL_L || keynum == METU)
            {
                if (vol_state == 0)
                {
                    // DEBUG_LOG_STRING("HD_VOL_DATA %d ok\r\n",!flash_read(HD_VOL__DATA,(uint8_t *)&irparams,sizeof(irparams)));
                    led_on(LED_1, 30, 0);
                    for (uint8_t i = 0; i < irparams.len; i++) {
                    }
                    if(keynum == VOL_H && !flash_read(HD_VOL_DATA,(uint8_t *)&irparams,sizeof(irparams))){
                        SysTick_DelayMs(120);
                        ir_remote_learn_send(irparams);
                        set_key_press_state(true);
                    }
                    else if(keynum == VOL_L && !flash_read(HD_VOL__DATA,(uint8_t *)&irparams,sizeof(irparams))){
                        SysTick_DelayMs(120);
                        ir_remote_learn_send(irparams);
                        set_key_press_state(true);
                    }
                    else if(keynum == METU && !flash_read(HD_MUTE_DATA,(uint8_t *)&irparams,sizeof(irparams))){
                        SysTick_DelayMs(120);
                        ir_remote_learn_send(irparams);
                        set_key_press_state(true);
                    }
                    else{
                        DEBUG_LOG_STRING("VOL_H ok\r\n");
                        if (ir_done != 1) {
                            ir_done = 1;
                            ir_brand_send = brand_AV_init(av_brand_code);
                        }
                        SysTick_DelayMs(100);
                        ir_brand_send(keynum);
                        set_key_press_state(true);
                    }
                }
                else
                {
                    if (!bt_check_le_connected())
                    {
                        led_on(LED_1, 30, 0);
                        if (ir_done != 2) {
                            ir_done = 2;
                            ir_brand_send = brand_DVB_init(dvb_brand_code);
                        }
                        SysTick_DelayMs(100);
                        ir_brand_send(keynum);
                        set_key_press_state(true);
                    }
                }
            }
            else if (keynum == PWR_KEYNUM)
            {
                if (power_state == 0)
                {
                    if (!bt_check_le_connected())
                    {
                        led_on(LED_1, 30, 0);
                        if (ir_done != 2) {
                            ir_done = 2;
                            ir_brand_send = brand_DVB_init(dvb_brand_code);
                        }
                        SysTick_DelayMs(100);
                        ir_brand_send(keynum);
                        set_key_press_state(true);
                    }
                }
                else
                {
                    led_on(LED_1, 150, 0);

					if(bt_check_le_connected()){
                        app_sleep_lock_set(APP_LOCK, true);
                    }
                    ir_done = 0;
                    ir_brand_send = brand_DVB_init(dvb_brand_code);
                    ir_brand_send(PWR_KEYNUM);
                    set_key_press_state(true);
                    swtimer_start(power_timernum, 400, TIMER_START_ONCE);
                    DEBUG_LOG_STRING("power ok\r\n");
                }
            }
            else
            {
                if (!bt_check_le_connected() && keynum != 20 && keynum != 21)
                {
                    led_on(LED_1, 30, 0);
                    if (ir_done != 2) {
                        ir_done = 2;
                        ir_brand_send = brand_DVB_init(dvb_brand_code);
                    }
                    SysTick_DelayMs(100);
                    ir_brand_send(keynum);
                    set_key_press_state(true);
                }
            }
        }

#endif
    }
    else if (key_pressed_num == 2)
    {
        set_key_press_state(false);
        led_off(LED_NUM);
		
        if (key_report->keynum_report_buf[OK__Row] == OK__Keynum && key_report->keynum_report_buf[0] == 7) {
            keynum = key_report->keynum_report_buf[5];
            keynum_secnd = key_report->keynum_report_buf[0];
            DEBUG_LOG_STRING(" TV start \r\n");
            ir_state = AV_PRESS;
            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_REPEAT);
        }
        else if (key_report->keynum_report_buf[OK__Row] == OK__Keynum && key_report->keynum_report_buf[1] == 8) {
            keynum = key_report->keynum_report_buf[5];
            keynum_secnd = key_report->keynum_report_buf[1];
            DEBUG_LOG_STRING(" DVB_PRESS\r\n");
            ir_state = DVB_PRESS;
            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }     
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_REPEAT);                   
        }
        else if (key_report->keynum_report_buf[Back_Row] == Back_Keynum && key_report->keynum_report_buf[3] == 4) {
            keynum = key_report->keynum_report_buf[4];
            keynum_secnd = key_report->keynum_report_buf[3];
            DEBUG_LOG_STRING(" PAIR_STATE\r\n");
            ir_state = PAIR_STATE;
            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }          
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_REPEAT);              
        }
        else if (key_report->keynum_report_buf[OK__Row] == OK__Keynum && key_report->keynum_report_buf[0] == 1) {
            keynum = key_report->keynum_report_buf[5];
            keynum_secnd = key_report->keynum_report_buf[0];
            DEBUG_LOG_STRING(" POWER_STATE\r\n");
            ir_state = POWER_STATE;
            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }    
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_REPEAT);                    
        }
        else if (key_report->keynum_report_buf[OK__Row] == OK__Keynum && key_report->keynum_report_buf[0] == 13) {
            keynum = key_report->keynum_report_buf[5];
            keynum_secnd = key_report->keynum_report_buf[0];
            DEBUG_LOG_STRING(" VOL_STATE\r\n");
            ir_state = VOL_STATE;
            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_REPEAT);                        
        }
        else if (key_report->keynum_report_buf[OK__Row] == OK__Keynum && key_report->keynum_report_buf[Back_Row] == Back_Keynum) {
            keynum = key_report->keynum_report_buf[OK__Row];
            keynum_secnd = key_report->keynum_report_buf[Back_Row];
            DEBUG_LOG_STRING(" FACTORY_DEFAULT\r\n");
            ir_state = FACTORY_DEFAULT;

            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_REPEAT);                     
        }
        else if (key_report->keynum_report_buf[OK__Row] == OK__Keynum && key_report->keynum_report_buf[Down_Row] == Down_Keynum) {
            keynum = key_report->keynum_report_buf[OK__Row];
            keynum_secnd = key_report->keynum_report_buf[Down_Row];

            ir_state = IR_RECEIVE_STATE;
            continu_ir_receive = false;
            
            if(bt_check_le_connected()){
                app_sleep_lock_set(APP_LOCK, true);
            }
            swtimer_start(brand_pressed_timernum, 100, TIMER_START_REPEAT);                     
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
    mouse_on_off = false;

    keyscan_start();
    software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
    vbat_reinit();
    led_reinit();
    voice_report_reinit();
    mouse_reinit();
    EulerAngle_Calculate_ReStart();
}

void mouse_switch(void)
{
    if (mouse_on_off) {
        mouse_on_off = false;

        mouse_turn_off();
        led_off(LED_2);
        bt_disable_le_tx_md();
        System_ChangeXtal24M();
        app_sleep_lock_set(MOUSE_LOCK, false);
    }
    else {
        mouse_flag = 0;
        mouse_on_off = true;

        memset(&euler_angles_offset, 0, sizeof(euler_angles_t));
        mouse_turn_on();
        led_on(LED_2, 0, 0);
        bt_enable_le_tx_md();
        System_ChangeDPLL(CLOCK_DPLL_48M_multiple);
        app_sleep_lock_set(MOUSE_LOCK, true);
    }
}

void action_after_mic_close(void)
{
    led_off(LED_2);
}

void update_conn_param(bool is_sleep)
{
    if (is_sleep) {
        bt_le_conn_updata_param_req(0x08, 0x08, 99, 400);
    }
    else {
        bt_le_conn_updata_param_req(0x06, 0x06, 0, 200);
        app_sleep_lock_set(LATENCY_LOCK, false);
    }
}

void start_adv(enum advType type, uint16_t adv_interval)
{
    struct bt_le_adv_param bt_adv_param;
    bt_adv_param.Type = type;
    bt_adv_param.adv_max_interval = adv_interval;
    app_sleep_lock_set(ADV_LOCK, true);

    if (bt_adv_param.Type == ADV_TYPE_NOMAL) {
        bt_set_le_state(BLE_ADV);
        bt_start_le_adv(&bt_adv_param, adv_data_buf, sizeof(adv_data_buf));
        app_sleep_timer_set(SHUTDOWN_TIME);
    }
    else if (bt_adv_param.Type == ADV_TYPE_DIRECT) {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        app_sleep_timer_set(DIRECT_ADV_TIME);
    }
}

void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset)
{
    uint8_t senddata[251] = {ATTOP_READ_BLOB_RESPONSE};
    uint16_t sendlen = ((table->dataLen - offset) >= ((HREADW(mem_mtu_size)) - 1)) ? ((HREADW(mem_mtu_size)) - 1) : (table->dataLen - offset);

    memcpy(senddata + 1, table->dataPtr + offset, sendlen);
    ATT_Send(senddata, sendlen + 1);
}

void Read_Parse(const ATT_TABLE_TYPE *table)
{
    uint8_t senddata[256] = {ATTOP_READ_RESPONSE};
    uint16_t sendlen = (table->dataLen >= (HREADW(mem_mtu_size)) - 1) ? (HREADW(mem_mtu_size)) - 1 : table->dataLen;

    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);
    memcpy((void*)&senddata[1], table->dataPtr, sendlen);
    ATT_Send(senddata, sendlen + 1);
}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);

    if (table->handle == 92)
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
        else if (!memcmp(MIC_CLOSE, data, 1) && voice_status.mic_open_flag) {
            voice_status.mic_close = true;
            voice_status_change();
        }
    }
    else if (table->handle == 89)
    {
        if (*data == 0xaa && *(data + 1) == 0x55)
        {
            if (*(data + 2) == 0x00)
            {
                uint16_t tmp  = 0;
                tmp |= (*(data + 3) << 8);
                tmp |= *(data + 4);
                ir_judge = brand_AV_init(tmp);
                if (ir_judge != NULL)
                {
                    av_brand_code = tmp;
                    vol_state = 0;
                    flash_write(HD_VOL_STATE, (uint8_t*)&vol_state, sizeof(vol_state), STATE_INF);
                    flash_write(HD_AV_IR, (uint8_t*)&av_brand_code, sizeof(av_brand_code), STATE_INF);
                    led_on(LED_1, 100, 500);
                    DEBUG_LOG_STRING(" AV_STATE\r\n");
                }
            }
            else if (*(data + 2) == 0x01)
            {
                if (*(data + 3) == 0x00) {
                    power_state = true;
                }
                else {
                    power_state = false;
                }
                flash_write(HD_POWER_STATE, (uint8_t*)&power_state, sizeof(power_state), STATE_INF);
            }
            else if (*(data + 2) == 0x02)
            {
                if (*(data + 3) == 0x00) {
                    vol_state = true;
                }
                else {
                    vol_state = false;
                }
                flash_write(HD_VOL_STATE, (uint8_t*)&vol_state, sizeof(vol_state), STATE_INF);
            }
        }
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
    if (HREADW(mem_le_slave_latency) == 99) {
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
    DEBUG_LOG_STRING("ENCRYPT DONE \r\n");
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
    led_off(LED_NUM);
    led_on(LED_2, 200, 800);
    ir_state = PAIR_STATE;
    swtimer_start(led_state_timernum,300,TIMER_START_ONCE);
}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
    if(bt_check_save_connect_info()){
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    }
}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);
    bt_set_tx_power(TX_POWER_0DB);
    Bt_HciFifo_Init();
    app_sleep_lock_set(LATENCY_LOCK, false);
    
    if (reason == 0x13) {
        prepare_before_sleep();
        enter_deep_sleep();
        return;
    }

    app_queue_reset();
    remote_control_reinit();

    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x10);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x30);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("connected! \r\n");
    bt_set_lpm_overhead_wake_time(0x0b);    // 0x08 ~ 0x10
    bt_set_le_state(BLE_CONNECTED);
    bt_set_le_mtu_size(251);
    bt_disable_le_tx_md();
    Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    ATT_SendExchangeMtuReq();
    update_conn_param(false);
}

void Dev_PowerOn(void)
{
    DEBUG_LOG_STRING("DEVICE POWER ON \r\n");

    IPC_TxControlCmd(BT_CMD_FIRST_POWER_ON);

    Flash_LoadFlashRecord();
    Bt_ReadAddrFromFlash();

    if (!Lpm_GetWakeFlag()) {
        bt_renew_scan_rsp((void*)scan_rsp_data_buf, sizeof(scan_rsp_data_buf));
    }

    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x10);
    }
    else {
        app_sleep_lock_set(ADV_LOCK, true);
        app_sleep_timer_set(SHUTDOWN_TIME);
    }
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
        tx_power_switch = true;

        // bt_rf_capacitance_config(0x17);
        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(DEFAULT_TYPE, DEFAULT_CUSTOMER);
        mouse_init(mouse_handle);
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);



        if (!flash_record_exist(HD_AV_IR)) {
            uint16_t len = sizeof(av_brand_code);
            flash_read(HD_AV_IR, (uint8_t*)&av_brand_code, len);
        }
        if (!flash_record_exist(HD_DVB_IR)) {
            uint16_t len = sizeof(dvb_brand_code);
            flash_read(HD_DVB_IR, (uint8_t*)&dvb_brand_code, len);
        }
        if (!flash_record_exist(HD_POWER_STATE)) {
            uint16_t len = sizeof(power_state);
            flash_read(HD_POWER_STATE, (uint8_t*)&power_state, len);
        }
        if (!flash_record_exist(HD_VOL_STATE)) {
            uint16_t len = sizeof(vol_state);
            flash_read(HD_VOL_STATE, (uint8_t*)&vol_state, len);
        }
        if (!flash_record_exist(HD_SEARCH_STATE)) {
            uint16_t len = sizeof(search_ir);
            flash_read(HD_SEARCH_STATE,(uint8_t*)&search_ir, len);
        }

        vioce_timernum = swtimer_add(mic_close);
        led_state_timernum = swtimer_add(led_state_handle);
        brand_pressed_timernum = swtimer_add(brand_pressed_handle);
        power_timernum = swtimer_add(power_state_handle);
        power_led_timernum = swtimer_add(power_led_state_handle);
        pair_done_timernum = swtimer_add(pair_done_handle);
        ir_done_timernum = swtimer_add(ir_done_handle);
        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
    else
    {
        app_queue_reset();

        if (key_wakeup_get())
        {
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP \r\n");
        }
        else {
            enter_low_sleep();
            DEBUG_LOG_STRING("KEEP CONNECT \r\n");
        }
    }
}
