

#include "mouse_wheel.h"
#include "yc11xx_gpio.h"
#include "app_queue.h"

#include "remote_control.h"

#define ENCODE_TYPE     (0)     // 0: 1定位一脉冲, 1: 2定位一脉冲


static WHEEL_DIR_E wheel_dir = WHEEL_DIR_IDLE;
static bool encode_state;
static bool encode_onoff;

// 函数返回值  0：无动作； 1：正转；  -1：反转； 2:只按下按键;  3:按下按键同时正转; -3:按下按键同时反转动;
static int8_t Encoder_Scan(void)
{
    static uint8_t dir = 0; // 编码器状态
    uint8_t Enc_A = GPIO_ReadDataBit(MOUSE_WHEEL_SA_PIN);
    uint8_t Enc_B = GPIO_ReadDataBit(MOUSE_WHEEL_SB_PIN);
    int8_t ScanResult = 0;
    
#if (ENCODE_TYPE == 0)
    if (Enc_A == 1 && Enc_B == 1)
        dir = 0; // 一定位一脉冲不动作时都只有一种状态,即只要卡在定位的状态A和B都为高电平
    else if (Enc_A == 0 && Enc_B == 1 && dir == 0)
        dir = 'R'; // A和B不相等时证明已经被扭动,通过A和B对应的电平判断出方向,此为第一次扫描
    else if (Enc_A == 1 && Enc_B == 0 && dir == 0)
        dir = 'L';
    else if (Enc_A == 0 && Enc_B == 0 && dir != 0xff) // 当扭到A和B都在低电平状态时执行处理相应的方向,此为第二次扫描
    {
        if (dir == 'L') {
            ScanResult = -1;
            dir = 0xff;
            return 'L';
        }
        else if (dir == 'R') {
            ScanResult = 1;
            dir = 0xff;
            return 'R';
        }
    }
#elif (ENCODE_TYPE == 1)
    // 两定位一脉冲的,定位卡,间隔的定位卡位上的A和B电平是有两种电平状态的,一格A和B都为高电平,一格A和B都为低电平
    if (Enc_A == 1 && Enc_B == 1)
            dir = 1; // 两定位一脉冲编码的当前定位状态1,A和B同时都为高电平
        else if (Enc_A == 0 && Enc_B == 0)
            dir = 0; // 两定位一脉冲编码的的当前定位状态2,A和B同时都为高电平
        else if ((Enc_A == 0 && Enc_B == 1 && dir == 1) || (Enc_A == 1 && Enc_B == 0 && dir == 0))
            dir = 'R'; // 对应两种定位电平判断方向
        else if ((Enc_A == 1 && Enc_B == 0 && dir == 1) || (Enc_A == 0 && Enc_B == 1 && dir == 0))
            dir = 'L';        // 对应两种定位电平判断方向
        else if (dir != 0xFF) // 由于一格只能半个脉冲变化,不能像一定位一脉冲哪样,两状态扫描才进入处理确定方向
        {
            if (dir == 'L') {
                ScanResult = -1;
                dir = 0xFF;
                return 'L';
            }
            else if (dir == 'R') {
                ScanResult = 1;
                dir = 0xFF;
                return 'R';
            }
        }
#endif
    
    return ScanResult;
}

static void mouse_wheel_task(void)
{
    uint8_t cstr = Encoder_Scan();

    if (cstr == 0 && encode_state == true){
        encode_state = false;
    }
    else if (cstr == 'R' && encode_state == false) {
        wheel_dir = WHEEL_DIR_DOWN;
        encode_state = true;
    }
    else if (cstr == 'L' && encode_state == false)
    {
        wheel_dir = WHEEL_DIR_UP;
        encode_state = true;
    }

    if(encode_onoff){
        app_queue_insert(mouse_wheel_task);
    }
}

WHEEL_DIR_E get_wheel_dir(void)
{
    WHEEL_DIR_E ret = WHEEL_DIR_IDLE;

    if (wheel_dir != WHEEL_DIR_IDLE)
    {
        ret = wheel_dir;
        wheel_dir = WHEEL_DIR_IDLE;
    }

    return ret;
}

void mouse_wheel_switch(bool onoff)
{
    encode_onoff = onoff;
    if (onoff) {
        app_queue_insert(mouse_wheel_task);
    }
}

void mouse_wheel_deinit(void)
{
    GPIO_Init(MOUSE_WHEEL_PWR_PIN, GPIO_Mode_Out_Low);
    GPIO_Init(MOUSE_WHEEL_SA_PIN, GPIO_Mode_Out_Low);
    GPIO_Init(MOUSE_WHEEL_SB_PIN, GPIO_Mode_Out_Low);
}

void mouse_wheel_init(void)
{
    GPIO_Init(MOUSE_WHEEL_SA_PIN, GPIO_Mode_Deinit);
    GPIO_Init(MOUSE_WHEEL_SB_PIN, GPIO_Mode_Deinit);
    GPIO_Init(MOUSE_WHEEL_PWR_PIN, GPIO_Mode_Out_High);

    app_queue_insert(mouse_wheel_task);
}
