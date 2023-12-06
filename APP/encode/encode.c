#include "encode.h"
#include "remote_control.h"
#include "app_queue.h"
#include "yc_debug.h"

/************************************************************************************************

函数名称：Encoder

函数功能：编码器旋转的扫描及处理

入口参数：无

出口参数：char型    0-无旋转   'R'-正转(向右转)   'L'-反转(向左转)

************************************************************************************************/
bool encode_state;
bool encode_onoff;
static uint8_t encode_chk_ok = ENCODE_DIR_UNCHECK; /// 0-向上   1-向下 0xff-未检测
static uint8_t Enc_Type = 0; // Enc_Type=0 一定位一脉冲,=1两定位一脉冲
// 函数返回值  0：无动作； 1：正转；  -1：反转； 2:只按下按键; 3:按下按键同时正转; -3:按下按键同时反转动;
static int8_t Encoder_Scan(void)
{
    uint8_t Enc_A = GPIO_ReadDataBit(ENCODE_SA_PIN);
    uint8_t Enc_B = GPIO_ReadDataBit(ENCODE_SB_PIN);
    int8_t ScanResult = 0;  // 返回编码器扫描结果，用于分析编码器的动作
                            //  0：无动作；      1：正转；           -1：反转；
                            //  2:只按下按键;    3:按下按键同时正转   -3:按下按键同时反转动
    static uint8_t dir = 0; // 编码器状态
    
    // DEBUG_LOG_STRING("Enc_A = %d\r\n",Enc_A);
    // DEBUG_LOG_STRING("Enc_B = %d\r\n",Enc_B);

    if (Enc_Type == 0) // 一定位一脉冲
    {
        if (Enc_A == 1 && Enc_B == 1)
        {
            dir = 0; // 一定位一脉冲不动作时都只有一种状态,即只要卡在定位的状态A和B都为高电平
        }
        else if (Enc_A == 0 && Enc_B == 1 && dir == 0)
            dir = 'R'; // A和B不相等时证明已经被扭动,通过A和B对应的电平判断出方向,此为第一次扫描
        else if (Enc_A == 1 && Enc_B == 0 && dir == 0)
            dir = 'L';
        else if (Enc_A == 0 && Enc_B == 0 && dir != 0xff) // 当扭到A和B都在低电平状态时执行处理相应的方向,此为第二次扫描
        {
            if (dir == 'L')
            {
                ScanResult = -1;
                dir = 0xff;
                return 'L';
            }
            else if (dir == 'R')
            {
                ScanResult = 1;
                dir = 0xff;
                return 'R';
            }
        }
    }
    else if (Enc_Type == 1) // 两定位一脉冲
    {                       // 两定位一脉冲的,定位卡,间隔的定位卡位上的A和B电平是有两种电平状态的,一格A和B都为高电平,一格A和B都为低电平
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
            if (dir == 'L')
            {
                ScanResult = -1;
                dir = 0xFF;
                return 'L';
            }
            else if (dir == 'R')
            {
                ScanResult = 1;
                dir = 0xFF;
                return 'R';
            }
        }
    }
    return ScanResult;
}

static void encode_timer_cb(void)
{
    uint8_t cstr = Encoder_Scan();
    if (cstr == 0 && encode_state == true){
        encode_state = false;
        // DEBUG_LOG_STRING("0\r\n");
    }
    else if (cstr == 'R' && encode_state == false)
    {
        encode_chk_ok = ENCODE_DIR_DOWN; /// 向下滚
        encode_state = true;
        // DEBUG_LOG_STRING("R\r\n");
    }
    else if (cstr == 'L' && encode_state == false)
    {
        encode_chk_ok = ENCODE_DIR_UP; /// 向上滚
        encode_state = true;
        // DEBUG_LOG_STRING("L\r\n");
    }

    //  if (cstr == 'R' )
    // {
    //     encode_chk_ok = ENCODE_DIR_DOWN; /// 向下滚
    //     // encode_state = true;
    //     DEBUG_LOG_STRING("R\r\n");
    // }
    // else if (cstr == 'L')
    // {
    //     encode_chk_ok = ENCODE_DIR_UP; /// 向上滚
    //     // encode_state = true;
    //     DEBUG_LOG_STRING("L\r\n");
    // }
    if(!encode_onoff){
        app_queue_insert(encode_timer_cb);
    }
}

void encode_timer_init(void)
{
    GPIO_Init(ENCODE_SA_PIN, GPIO_Mode_Deinit);
    GPIO_Init(ENCODE_SB_PIN, GPIO_Mode_Deinit);
#ifdef ENCODE_PWR_PIN
    GPIO_Init(ENCODE_PWR_PIN, GPIO_Mode_Out_High);
#endif
    app_queue_insert(encode_timer_cb);
}
void encode_timer_deinit(void) {
#ifdef ENCODE_PWR_PIN
    GPIO_Init(ENCODE_PWR_PIN, GPIO_Mode_Out_Low);
#endif
    GPIO_Init(ENCODE_SA_PIN, GPIO_Mode_Out_Low);
    GPIO_Init(ENCODE_SB_PIN, GPIO_Mode_Out_Low);
}
uint8_t get_encode_chk_ok_state(void)
{
    uint8_t res = ENCODE_DIR_UNCHECK;
    if (encode_chk_ok != ENCODE_DIR_UNCHECK)
    {
        res = encode_chk_ok;
        encode_chk_ok = ENCODE_DIR_UNCHECK;
    }
    return res;
}

void encode_on(void) {
    encode_onoff = false;
    app_queue_insert(encode_timer_cb);
}
void encode_off(void) {
    encode_onoff = true;
}