#include "app_sleep.h"
#include "software_timer.h"
#include "app_queue.h"
#include "keyscan.h"
#include "vbat.h"
#include "led.h"
#include "audio.h"
#ifdef SOFTWARE_IIC
#include "software_i2c.h"
#else
#include "yc11xx_iic.h"
#endif
#ifdef MOUSE
#include "mouse.h"
#endif
#ifdef LG
#include "encode.h"
#endif
#ifdef QMA_6100
// #include "qma6100.h"
#endif
#ifdef MIR3DA_267
#include "mir3da_267.h"
#endif
#ifdef SONY_200
#include "audio.h"
#endif
#include "yc11xx_lpm.h"
#include "yc_dev_bt.h"
#include "yc11xx_bt_interface.h"
#include "yc_debug.h"

#include "app_config.h"

#ifndef CONN_PARAM_UPDATE_TIME
#define CONN_PARAM_UPDATE_TIME  (300)
#endif


typedef struct {
    bool adv:1;
    bool app:1;
    bool ble:1;
    bool key:1;
    bool led:1;
    bool ir:1;
    bool auido:1;
    bool latency:1;
    bool sleep_ready:1;
    bool sleep_flag:1;
    bool keep_conn:1;  
    uint8_t adv_time_cnt;
    uint8_t shutdown_time;
#ifdef MOUSE
    bool mouse:1;
    uint8_t mouse_time_cnt;
#endif
}remote_control_status_t;

static remote_control_status_t remote_control_status;
static uint8_t sleep_timernum = 0xFF;

WEAK void Action_After_Prepare_Sleep(void){}
WEAK void Action_After_Enter_Deep_Sleep(void){}

static bool app_lock_check(void)
{
    return remote_control_status.sleep_ready && !remote_control_status.adv
            && !remote_control_status.key && !remote_control_status.led
            && !remote_control_status.ir && !remote_control_status.auido
#ifdef MOUSE
            && !remote_control_status.mouse
#endif
    ;
}

static void app_sleep_task(void)
{
    if (remote_control_status.adv)
    {
        remote_control_status.adv_time_cnt++;
#ifdef ADV_TIME
        DEBUG_LOG_STRING("ADV TIME CNT: %d \r\n", remote_control_status.adv_time_cnt);
#endif
        if (remote_control_status.adv_time_cnt >= remote_control_status.shutdown_time) {
            enter_deep_sleep();
        }
        else {
            swtimer_restart(sleep_timernum);
        }
    }
    else if (!remote_control_status.sleep_ready)
    {
        remote_control_status.sleep_ready = true;
        app_queue_insert(app_sleep_task);
    }
#ifdef MOUSE
    else if (remote_control_status.mouse)
    {
        remote_control_status.mouse_time_cnt++;
        // DEBUG_LOG_STRING("MOUSE TIME CNT: %d \r\n", remote_control_status.mouse_time_cnt);
        if (remote_control_status.mouse_time_cnt >= MOUSE_STOP_TIMEOUT) {
#if LG
		bt_send_le_disconnect(0x13);
#else
        mouse_switch();
#endif

        }
        else {
            swtimer_restart(sleep_timernum);
        }
    }
#endif
    else if (!remote_control_status.app && !remote_control_status.key && !remote_control_status.led
        && !remote_control_status.ir && !remote_control_status.auido)
    {
        if (remote_control_status.latency) {
            remote_control_status.sleep_flag = true;
            Lpm_unLockLpm(LPM_ALL_LOCK);
        }
        else 
        {
#if LG
        bt_send_le_disconnect(0x13);
#else
        update_conn_param(true);
#endif
        }
    }
}

void prepare_before_sleep(void)
{
    DEBUG_LOG_STRING("remote_control_status.keep_conn = %d \r\n",remote_control_status.keep_conn);
    if (!remote_control_status.keep_conn)
    {
        DEBUG_LOG_STRING("PREPARE BEFORE SLEEP \r\n");
        remote_control_status.keep_conn = true;
        app_queue_reset();
        vbat_deinit();
        led_deinit();
        keyscan_stop();
        software_timer_stop();

#if defined(SOFTWARE_IIC) || defined(HARDWARE_IIC)
        GPIO_Init(IIC_SCL_PIN, GPIO_Mode_Out_Low);
        GPIO_Init(IIC_SDA_PIN, GPIO_Mode_Out_Low);

#endif
#ifdef  LG
        mouse_lpm_int_mode();
#endif
#ifdef IR_RCV_PIN
        GPIO_Init(IR_RCV_PIN, GPIO_Mode_Out_High);
#endif
        key_wakeup_set();
        Action_After_Prepare_Sleep();        
    }
}

void enter_deep_sleep(void)
{
    DEBUG_LOG_STRING("ENTER DEEP SLEEP \r\n");
    remote_control_status.keep_conn = false;
    remote_control_status.sleep_flag = true;
#ifdef MOUSE_INT_MODE
    mouse_int_mode();
#if LG
    encode_timer_deinit();
#endif
#endif

    Action_After_Enter_Deep_Sleep();

    if (bt_check_le_connected())
    {
#ifdef SONY_200
        bt_send_le_disconnect(0x13);
#else
        bt_send_le_disconnect(0x16);
#endif
    }
    else
    {
#ifdef SONY_200
        mic_close();
#endif
        Bt_SndCmdPwroff();
    }
}

void enter_low_sleep(void)
{
    Lpm_unLockLpm(LPM_ALL_LOCK);
}

bool app_sleep_check(void)
{
    return remote_control_status.sleep_flag;
}

void app_sleep_lock_set(DEV_LOCK_TypeDef dev_lock, bool state)
{
    remote_control_status.keep_conn = false;
    remote_control_status.sleep_flag = false;

    switch (dev_lock)
    {
    case ADV_LOCK:
        remote_control_status.adv_time_cnt = 0;
        remote_control_status.adv = state;
        if (!state) {
            swtimer_stop(sleep_timernum);
        }
        break;
    
    case APP_LOCK:
    {
        remote_control_status.app = state;
        if (state) {
            Lpm_LockLpm(APP_LPM_LOCK);
        }
        else if (app_lock_check()) {
            app_queue_insert(app_sleep_task);
        }
    }
        break;
    case KEY_LOCK:
    {
        remote_control_status.key = state;
        if (state) {
            remote_control_status.adv_time_cnt = 0;
            Lpm_LockLpm(KEY_LPM_FLAG);
        }
        else if (app_lock_check()) {
            app_queue_insert(app_sleep_task);
        }
    }
        break;
    case LED_LOCK:
    {
        remote_control_status.led = state;
        if (state) {
            remote_control_status.adv_time_cnt = 0;
            Lpm_LockLpm(LED_LPM_FLAG);
        }
        else if (app_lock_check()) {
            app_queue_insert(app_sleep_task);
        }
    }
        break;
    case IR_LOCK:
    {
        remote_control_status.ir = state;
        if (state) {
            Lpm_LockLpm(IR_LPM_FLAG);
        }
        else if (app_lock_check()) {
            app_queue_insert(app_sleep_task);
        }
    }
        break;
    case AUDIO_LOCK:
    {
        remote_control_status.auido = state;
        if (state) {
            Lpm_LockLpm(AUDIO_LPM_FLAG);
        }
        else if (app_lock_check()) {
            app_queue_insert(app_sleep_task);
        }
    }
        break;
#ifdef MOUSE
    case MOUSE_LOCK:
    {
        remote_control_status.mouse = state;
        remote_control_status.mouse_time_cnt = 0;
        if (state) {
            Lpm_LockLpm(MOUSE_LPM_FLAG);
        }
        else if (app_lock_check()) {
            app_queue_insert(app_sleep_task);
        }
    }
        break;
#endif

    case LATENCY_LOCK:
        remote_control_status.latency = state;
        if (state) {
            // app_queue_insert(app_sleep_task);
            app_sleep_task();
        }
        break;

    default:
        break;
    }
}

void app_sleep_timer_set(uint16_t time)
{
    if (time == PAIR_DONE_DELAY || time == ENCRYPT_DONE_DELAY) {
        remote_control_status.adv = false;
    }
    else if (time == DIRECT_ADV_TIME) {
        remote_control_status.adv_time_cnt = 0;
        remote_control_status.shutdown_time = DIRECT_ADV_TIME;
        time = UNIT_TIME_1S;
    }
    else if (time == SHUTDOWN_TIME) {
        remote_control_status.adv_time_cnt = 0;
        remote_control_status.shutdown_time = SHUTDOWN_TIME;
        time = UNIT_TIME_1S;
    }
    
    remote_control_status.sleep_ready = false;
    swtimer_start(sleep_timernum, time, TIMER_START_ONCE);
}

uint8_t app_sleep_init(void)
{
    remote_control_status.shutdown_time = SHUTDOWN_TIME;
    
    sleep_timernum = swtimer_add(app_sleep_task);
    if (sleep_timernum > TIMER_NUM) {
        return sleep_timernum;
    }

    return 0;
}
