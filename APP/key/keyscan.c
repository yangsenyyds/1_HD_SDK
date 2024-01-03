#include "keyscan.h"
#include "app_queue.h"
#include "software_timer.h"
#include "app_sleep.h"
#include "yc11xx_gpio.h"
#include "app_config.h"
#include "yc_debug.h"
#ifdef LG
#include "encode.h"
#endif

#ifndef KEY_REPORT_INTERVAL_TIME
#define KEY_REPORT_INTERVAL_TIME (10)
#endif

typedef enum {
#if (KEY_ROW_NUM >= 1)
    ROW1,
#endif
#if (KEY_ROW_NUM >= 2)
    ROW2,
#endif
#if (KEY_ROW_NUM >= 3)
    ROW3,
#endif
#if (KEY_ROW_NUM >= 4)
    ROW4,
#endif
#if (KEY_ROW_NUM >= 5)
    ROW5,
#endif
#if (KEY_ROW_NUM >= 6)
    ROW6,
#endif
#if (KEY_ROW_NUM >= 7)
    ROW7,
#endif
#if (KEY_ROW_NUM >= 8)
    ROW8,
#endif
    ROW_NUM
}rownum_t;

typedef enum {
#if (KEY_COL_NUM >= 1)
    COL1,
#endif
#if (KEY_COL_NUM >= 2)
    COL2,
#endif
#if (KEY_COL_NUM >= 3)
    COL3,
#endif
#if (KEY_COL_NUM >= 4)
    COL4,
#endif
#if (KEY_COL_NUM >= 5)
    COL5,
#endif
#if (KEY_COL_NUM >= 6)
    COL6,
#endif
#if (KEY_COL_NUM >= 7)
    COL7,
#endif
#if (KEY_COL_NUM >= 8)
    COL8,
#endif
    COL_NUM
}colnum_t;

typedef struct KEY_CFG {
    key_mode_t key_mode;
    key_report_cb_t key_report_cb;
}key_cfg_t;


 static bool key_changed = false;
static key_report_t key_report;
static uint8_t key_report_timernum = 0XFF;
 uint8_t key_lock_state = 0;
static uint8_t keynum_buf[ROW_NUM][COL_NUM];
static key_cfg_t key_cfg;

WEAK void app_sleep_lock_set(DEV_LOCK_TypeDef dev_lock, bool state){}

static void row_scan(uint8_t rownum)
{
    switch (rownum)
    {
#if (KEY_ROW_NUM >= 1)
    case ROW1:
        GPIO_Init(KEY_ROW1_PIN, GPIO_Mode_Out_High);
#if (KEY_ROW_NUM == 2)
        GPIO_Init(KEY_ROW2_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM == 3)
        GPIO_Init(KEY_ROW3_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM == 4)
        GPIO_Init(KEY_ROW4_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM == 5)
        GPIO_Init(KEY_ROW5_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM == 6)
        GPIO_Init(KEY_ROW6_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM == 7)
        GPIO_Init(KEY_ROW7_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM == 8)
        GPIO_Init(KEY_ROW8_PIN, GPIO_Mode_Out_Low);
#endif
        return;
#endif
#if (KEY_ROW_NUM >= 2)
    case ROW2:
        GPIO_Init(KEY_ROW2_PIN, GPIO_Mode_Out_High);
        break;
#endif
#if (KEY_ROW_NUM >= 3)
    case ROW3:
        GPIO_Init(KEY_ROW3_PIN, GPIO_Mode_Out_High);
        break;
#endif
#if (KEY_ROW_NUM >= 4)
    case ROW4:
        GPIO_Init(KEY_ROW4_PIN, GPIO_Mode_Out_High);
        break;
#endif
#if (KEY_ROW_NUM >= 5)
    case ROW5:
        GPIO_Init(KEY_ROW5_PIN, GPIO_Mode_Out_High);
        break;
#endif
#if (KEY_ROW_NUM >= 6)
    case ROW6:
        GPIO_Init(KEY_ROW6_PIN, GPIO_Mode_Out_High);
        break;
#endif
#if (KEY_ROW_NUM >= 7)
    case ROW7:
        GPIO_Init(KEY_ROW7_PIN, GPIO_Mode_Out_High);
        break;
#endif
#if (KEY_ROW_NUM >= 8)
    case ROW8:
        GPIO_Init(KEY_ROW8_PIN, GPIO_Mode_Out_High);
        break;
#endif
    
    default:
        break;
    }

    switch (rownum - 1)
    {
#if (KEY_ROW_NUM >= 1)
    case ROW1:
        GPIO_Init(KEY_ROW1_PIN, GPIO_Mode_Out_Low);
        break;
#endif
#if (KEY_ROW_NUM >= 2)
    case ROW2:
        GPIO_Init(KEY_ROW2_PIN, GPIO_Mode_Out_Low);
        break;
#endif
#if (KEY_ROW_NUM >= 3)
    case ROW3:
        GPIO_Init(KEY_ROW3_PIN, GPIO_Mode_Out_Low);
        break;
#endif
#if (KEY_ROW_NUM >= 4)
    case ROW4:
        GPIO_Init(KEY_ROW4_PIN, GPIO_Mode_Out_Low);
        break;
#endif
#if (KEY_ROW_NUM >= 5)
    case ROW5:
        GPIO_Init(KEY_ROW5_PIN, GPIO_Mode_Out_Low);
        break;
#endif
#if (KEY_ROW_NUM >= 6)
    case ROW6:
        GPIO_Init(KEY_ROW6_PIN, GPIO_Mode_Out_Low);
        break;
#endif
#if (KEY_ROW_NUM >= 7)
    case ROW7:
        GPIO_Init(KEY_ROW7_PIN, GPIO_Mode_Out_Low);
        break;
#endif

    default:
        break;
    }
}

static void keyscan_once(void)
{
    key_report.key_press_cnt = 0;
    memset(key_report.keynum_scan_buf, 0, KEY_COL_NUM);

    for (rownum_t rownum = ROW1; rownum < ROW_NUM; rownum++)
    {
        row_scan(rownum);

#if (KEY_COL_NUM >= 1)
        if (GPIO_ReadDataBit(KEY_COL1_PIN)) {
            key_report.keynum_scan_buf[COL1] = keynum_buf[rownum][COL1];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 2)
        if (GPIO_ReadDataBit(KEY_COL2_PIN)) {
            key_report.keynum_scan_buf[COL2] = keynum_buf[rownum][COL2];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 3)
        if (GPIO_ReadDataBit(KEY_COL3_PIN)) {
            key_report.keynum_scan_buf[COL3] = keynum_buf[rownum][COL3];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 4)
        if (GPIO_ReadDataBit(KEY_COL4_PIN)) {
            key_report.keynum_scan_buf[COL4] = keynum_buf[rownum][COL4];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 5)
        if (GPIO_ReadDataBit(KEY_COL5_PIN)) {
            key_report.keynum_scan_buf[COL5] = keynum_buf[rownum][COL5];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 6)
        if (GPIO_ReadDataBit(KEY_COL6_PIN)) {
            key_report.keynum_scan_buf[COL6] = keynum_buf[rownum][COL6];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 7)
        if (GPIO_ReadDataBit(KEY_COL7_PIN)) {
            key_report.keynum_scan_buf[COL7] = keynum_buf[rownum][COL7];
            key_report.key_press_cnt += 1;
        }
#endif
#if (KEY_COL_NUM >= 8)
        if (GPIO_ReadDataBit(KEY_COL8_PIN)) {
            key_report.keynum_scan_buf[COL8] = keynum_buf[rownum][COL8];
            key_report.key_press_cnt += 1;
        }
#endif
    }
    if (key_report.key_press_cnt > 0) {

        if(!key_lock_state){
        // DEBUG_LOG_STRING("key lock \r\n");
            app_sleep_lock_set(KEY_LOCK, true);
        }
    }
}

static void key_task(void)
{
    if (!memcmp(key_report.keynum_report_buf, key_report.keynum_scan_buf, KEY_COL_NUM))
    {

        if (key_cfg.key_mode == KEY_MODE_SINGLE)
        {
            // DEBUG_LOG_STRING("273 \r\n");
            if (key_changed)
            {
                key_changed = false;
                key_cfg.key_report_cb(&key_report);
                if (key_report.key_press_cnt == 0) {
                    app_sleep_lock_set(KEY_LOCK, false);
                }
            }
            else {
                keyscan_once();
            }
        }
        else if (key_cfg.key_mode == KEY_MODE_CONTINUE)
        {
            if (key_changed || key_report.key_press_cnt > 0) {
                key_changed = false;
                key_cfg.key_report_cb(&key_report);
                if (key_report.key_press_cnt == 0) {
                    app_sleep_lock_set(KEY_LOCK, false);
                }
            }
            else {
                keyscan_once();
            }
        }
    }
    else {
        key_changed = true;
        memcpy(key_report.keynum_report_buf, key_report.keynum_scan_buf, KEY_COL_NUM);
        keyscan_once();
    }
}

static void keyscan_row_cfg(void)
{
#if (KEY_ROW_NUM >= 1)
    GPIO_Init(KEY_ROW1_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 2)
    GPIO_Init(KEY_ROW2_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 3)
    GPIO_Init(KEY_ROW3_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 4)
    GPIO_Init(KEY_ROW4_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 5)
    GPIO_Init(KEY_ROW5_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 6)
    GPIO_Init(KEY_ROW6_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 7)
    GPIO_Init(KEY_ROW7_PIN, GPIO_Mode_Out_Low);
#endif
#if (KEY_ROW_NUM >= 8)
    GPIO_Init(KEY_ROW8_PIN, GPIO_Mode_Out_Low);
#endif
}

void keyscan_row_cfg_set(void)
{
#if (KEY_ROW_NUM >= 1)
    GPIO_Init(KEY_ROW1_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 2)
    GPIO_Init(KEY_ROW2_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 3)
    GPIO_Init(KEY_ROW3_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 4)
    GPIO_Init(KEY_ROW4_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 5)
    GPIO_Init(KEY_ROW5_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 6)
    GPIO_Init(KEY_ROW6_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 7)
    GPIO_Init(KEY_ROW7_PIN, GPIO_Mode_Out_High);
#endif
#if (KEY_ROW_NUM >= 8)
    GPIO_Init(KEY_ROW8_PIN, GPIO_Mode_Out_High);
#endif
}

static void keyscan_col_cfg(void)
{
#if (KEY_COL_NUM >= 1)
    GPIO_Init(KEY_COL1_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 2)
    GPIO_Init(KEY_COL2_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 3)
    GPIO_Init(KEY_COL3_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 4)
    GPIO_Init(KEY_COL4_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 5)
    GPIO_Init(KEY_COL5_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 6)
    GPIO_Init(KEY_COL6_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 7)
    GPIO_Init(KEY_COL7_PIN, GPIO_Mode_In_Down);
#endif
#if (KEY_COL_NUM >= 8)
    GPIO_Init(KEY_COL8_PIN, GPIO_Mode_In_Down);
#endif
}

bool key_wakeup_get(void)
{
    bool ret = false;
    
#if (KEY_COL_NUM >= 1)
    ret |= !GPIO_ReadDataBit(KEY_COL1_PIN);
#endif
#if (KEY_COL_NUM >= 2)
    ret |= !GPIO_ReadDataBit(KEY_COL2_PIN);
#endif
#if (KEY_COL_NUM >= 3)
    ret |= !GPIO_ReadDataBit(KEY_COL3_PIN);
#endif
#if (KEY_COL_NUM >= 4)
    ret |= !GPIO_ReadDataBit(KEY_COL4_PIN);
#endif
#if (KEY_COL_NUM >= 5)
    ret |= !GPIO_ReadDataBit(KEY_COL5_PIN);
#endif
#if (KEY_COL_NUM >= 6)
    ret |= !GPIO_ReadDataBit(KEY_COL6_PIN);
#endif
#if (KEY_COL_NUM >= 7)
    ret |= !GPIO_ReadDataBit(KEY_COL7_PIN);
#endif
#if (KEY_COL_NUM >= 8)
    ret |= !GPIO_ReadDataBit(KEY_COL8_PIN);
#endif

#ifdef LG
    ret |= GPIO_ReadDataBit(MOUSE_INT_PIN);
#endif
    return ret;
}

void key_wakeup_set(void)
{
#if (KEY_COL_NUM >= 1)
    GPIO_Init(KEY_COL1_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL1_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 2)
    GPIO_Init(KEY_COL2_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL2_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 3)
    GPIO_Init(KEY_COL3_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL3_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 4)
    GPIO_Init(KEY_COL4_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL4_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 5)
    GPIO_Init(KEY_COL5_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL5_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 6)
    GPIO_Init(KEY_COL6_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL6_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 7)
    GPIO_Init(KEY_COL7_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL7_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 8)
    GPIO_Init(KEY_COL8_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL8_PIN, GPIO_WakeUpLow);
#endif
}

bool key_wakeup_get_high(uint8_t *report_buf)
{
    bool ret = false;
    
#if (KEY_COL_NUM >= 1)
    if(key_report.keynum_report_buf[0] != 0 || report_buf[0] != 0)
    {
        ret |= GPIO_ReadDataBit(KEY_COL1_PIN);
         //DEBUG_LOG_STRING("470 ret = %d\r\n",ret);
    }
    // else
    // {
    //     ret |= !GPIO_ReadDataBit(KEY_COL1_PIN);
    // }
#endif
#if (KEY_COL_NUM >= 2)
    if(key_report.keynum_report_buf[1] != 0 || report_buf[1] != 0)
    {
        ret |= GPIO_ReadDataBit(KEY_COL2_PIN);
        //DEBUG_LOG_STRING("481 ret = %d\r\n",ret);
    }
    // else
    // {
    //     ret |= !GPIO_ReadDataBit(KEY_COL2_PIN);
    // }
#endif
#if (KEY_COL_NUM >= 3)
    if(key_report.keynum_report_buf[2] != 0 || report_buf[2] != 0)
    {
        ret |= GPIO_ReadDataBit(KEY_COL3_PIN);
        //DEBUG_LOG_STRING("492 ret = %d\r\n",ret);
    }
    // else
    // {
    //     ret |= !GPIO_ReadDataBit(KEY_COL3_PIN);
    // }
#endif
#if (KEY_COL_NUM >= 4)
    if(key_report.keynum_report_buf[3] != 0 || report_buf[3] != 0)
    {
        ret |= GPIO_ReadDataBit(KEY_COL4_PIN);
        //DEBUG_LOG_STRING("503 ret = %d\r\n",ret);   
    }
    // else
    // {
    //     ret |= !GPIO_ReadDataBit(KEY_COL4_PIN);
    // }
#endif
#if (KEY_COL_NUM >= 5)
    if(key_report.keynum_report_buf[4] != 0 || report_buf[4] != 0)
    {
        ret |= GPIO_ReadDataBit(KEY_COL5_PIN);
        //DEBUG_LOG_STRING("514 ret = %d\r\n",ret);
    }
    // else
    // {
    //     ret |= !GPIO_ReadDataBit(KEY_COL5_PIN);
    // }
#endif
#if (KEY_COL_NUM >= 6)
    if(key_report.keynum_report_buf[5] != 0 || report_buf[5] != 0)
    {
        ret |= GPIO_ReadDataBit(KEY_COL6_PIN);
        //DEBUG_LOG_STRING("525 ret = %d\r\n",ret);
    }
    // else
    // {
    //     ret |= !GPIO_ReadDataBit(KEY_COL6_PIN);
    // }
#endif
#if (KEY_COL_NUM >= 7)

#endif
#if (KEY_COL_NUM >= 8)
    ret |= GPIO_ReadDataBit(KEY_COL8_PIN);
#endif

#ifdef LG
    ret |= GPIO_ReadDataBit(MOUSE_INT_PIN);
#endif
    return ret;
}

void key_wakeup_set_high(uint8_t *report_buf)
{
#if (KEY_COL_NUM >= 1)
    if(key_report.keynum_report_buf[0] != 0 || report_buf[0] != 0)
    {
        GPIO_Init(KEY_COL1_PIN, GPIO_Mode_In_Up);
        GPIO_WakeUp(KEY_COL1_PIN, GPIO_WakeUpHigh);
        //DEBUG_LOG_STRING("553 \r\n");
    }
    // else
    // {
    //     GPIO_Init(KEY_COL1_PIN, GPIO_Mode_In_Up);
    //     GPIO_WakeUp(KEY_COL1_PIN, GPIO_WakeUpLow);
    // }

#endif
#if (KEY_COL_NUM >= 2)
    if(key_report.keynum_report_buf[1] != 0 || report_buf[1] != 0)
    {
        GPIO_Init(KEY_COL2_PIN, GPIO_Mode_In_Up);
        GPIO_WakeUp(KEY_COL2_PIN, GPIO_WakeUpHigh);
        //DEBUG_LOG_STRING("567 \r\n");
    }
    // else
    // {
    //     GPIO_Init(KEY_COL2_PIN, GPIO_Mode_In_Up);
    //     GPIO_WakeUp(KEY_COL2_PIN, GPIO_WakeUpLow);
    // }
#endif
#if (KEY_COL_NUM >= 3)
    if(key_report.keynum_report_buf[2] != 0 || report_buf[2] != 0)
    {
        GPIO_Init(KEY_COL3_PIN, GPIO_Mode_In_Up);
        GPIO_WakeUp(KEY_COL3_PIN, GPIO_WakeUpHigh);
        //DEBUG_LOG_STRING("580 \r\n");
    }
    // else
    // {
    //     GPIO_Init(KEY_COL3_PIN, GPIO_Mode_In_Up);
    //     GPIO_WakeUp(KEY_COL3_PIN, GPIO_WakeUpLow);
    // }
#endif
#if (KEY_COL_NUM >= 4)
    if(key_report.keynum_report_buf[3] != 0 || report_buf[3] != 0)
    {
        GPIO_Init(KEY_COL4_PIN, GPIO_Mode_In_Up);
        GPIO_WakeUp(KEY_COL4_PIN, GPIO_WakeUpHigh);
        //DEBUG_LOG_STRING("593 \r\n");        
    }
    // else
    // {
    //     GPIO_Init(KEY_COL4_PIN, GPIO_Mode_In_Up);
    //     GPIO_WakeUp(KEY_COL4_PIN, GPIO_WakeUpLow);
    // }
#endif
#if (KEY_COL_NUM >= 5)
    if(key_report.keynum_report_buf[4] != 0 || report_buf[4] != 0)
    {
        GPIO_Init(KEY_COL5_PIN, GPIO_Mode_In_Up);
        GPIO_WakeUp(KEY_COL5_PIN, GPIO_WakeUpHigh);
        //DEBUG_LOG_STRING("606 \r\n");                
    }
    // else
    // {
    //     GPIO_Init(KEY_COL5_PIN, GPIO_Mode_In_Up);
    //     GPIO_WakeUp(KEY_COL5_PIN, GPIO_WakeUpLow);
    // }
#endif
#if (KEY_COL_NUM >= 6)
    if(key_report.keynum_report_buf[5] != 0 || report_buf[5] != 0){
        GPIO_Init(KEY_COL6_PIN, GPIO_Mode_In_Up);
        GPIO_WakeUp(KEY_COL6_PIN, GPIO_WakeUpHigh);
        //DEBUG_LOG_STRING("618 \r\n");         
    }
    // else
    // {
    //     GPIO_Init(KEY_COL6_PIN, GPIO_Mode_In_Up);
    //     GPIO_WakeUp(KEY_COL6_PIN, GPIO_WakeUpLow);
    // }
#endif
#if (KEY_COL_NUM >= 7)
    GPIO_Init(KEY_COL7_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL7_PIN, GPIO_WakeUpLow);
#endif
#if (KEY_COL_NUM >= 8)
    GPIO_Init(KEY_COL8_PIN, GPIO_Mode_In_Up);
    GPIO_WakeUp(KEY_COL8_PIN, GPIO_WakeUpLow);
#endif    
}

void keyscan_stop(void)
{
    swtimer_stop(key_report_timernum);
    keyscan_row_cfg();
}

void keyscan_start(void)
{
    key_changed = false;

    keyscan_col_cfg();
    keyscan_row_cfg();
    keyscan_once();
    // memset(key_report.keynum_report_buf, 0, KEY_COL_NUM);
    swtimer_restart(key_report_timernum);
}

uint8_t keyscan_init(key_mode_t mode, key_report_cb_t cb)
{
    uint8_t keynum = 1;

    key_cfg.key_mode = mode;
    key_cfg.key_report_cb = cb;

    for (rownum_t rownum = ROW1; rownum < ROW_NUM; rownum++)
    {
        for (colnum_t colnum = COL1; colnum < COL_NUM; colnum++)
        {
            keynum_buf[rownum][colnum] = keynum++;
        }
    }

    keyscan_col_cfg();
    keyscan_row_cfg();
    keyscan_once();

    key_report_timernum = swtimer_add(key_task);
    if (key_report_timernum > TIMER_NUM) {
        return key_report_timernum;
    }
    swtimer_start(key_report_timernum, KEY_REPORT_INTERVAL_TIME, TIMER_START_REPEAT);

    return 0;
}

uint8_t get_key_timernum(void)
{
    return key_report_timernum;
}

