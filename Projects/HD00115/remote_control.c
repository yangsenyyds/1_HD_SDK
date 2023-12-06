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
#include <stdlib.h>
#include "encode.h"
#include "yc_ota.h"
#include "send_key.h"
#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
#define mouse_angle_range (127.0f)
#define mouse_move_range (35.0f)
#define mouse_ratio (mouse_angle_range / mouse_move_range)
typedef struct
{
    uint8_t MouseNTFBuf[MOUSE_SEND_LEN];
} MouseNTFQue_TypeDef;
enum
{
    APP_KEY_IDX_PWR = 1,
    APP_KEY_IDX_BACK = 27,
    APP_KEY_IDX_HOME = 20,
    APP_KEY_IDX_OK = 25,
    APP_KEY_IDX_SET = 28,
    APP_KEY_IDX_VOICE = 19,
};
#if LG22
static const uint8_t adv_data_buf[] = {0x02, 0x01, 0x05, 0x17, 0xFF, 0xC4, 0x00, 0x53, 0x43, 0x44, 0x20, 0x31, 0x39, 0x2E, 0x31, 0x2C, 0x42, 0x41, 0x20, 0x33, 0x35, 0x2C, 0x77, 0x65, 0x62, 0x4F, 0x53};
static const uint8_t scan_rsp_data_buf[] = {0x09, 0x09, 0x4C, 0x47, 0x45, 0x20, 0x4D, 0x52, 0x32, 0x32};
#elif LG21
static uint8_t scan_rsp_data_buf[] = {0x09, 0x09, 0x4c, 0x47, 0x45, 0x20, 0x4d, 0x52, 0x32, 0x31};
static const uint8_t adv_data_buf[] = {
0x02 ,0x01 ,0x05 ,0x17 ,0xFF ,0xC4 ,0x00 ,0x53 ,0x43 ,0x44 ,0x20 ,0x31 ,0x39 
,0x2E ,0x31 ,0x2C ,0x42 ,0x41 ,0x20 ,0x33 ,0x35 ,0x2C ,0x77 ,0x65 ,0x62 ,0x4F 
,0x53};
#elif LG20
static uint8_t adv_data_buf[] = {
0x02 ,0x01 ,0x05 ,0x17 
,0xFF ,0xC4 ,0x00 ,0x53 ,0x43 ,0x44 ,0x20 ,0x31 ,0x39 ,0x2E ,0x31 ,0x2C 
,0x42 ,0x41 ,0x20 ,0x33 ,0x35 ,0x2C ,0x77 ,0x65 ,0x62 ,0x4F ,0x53};
static uint8_t scan_rsp_data_buf[] = {
0x09 ,0x09 ,0x4C ,0x47 
,0x45 ,0x20 ,0x4D ,0x52 
,0x32 ,0x30
};
#elif LG23
static const uint8_t adv_data_buf[] = {
0x02 ,0x01 ,0x05 ,0x17 ,0xFF ,0xC4 ,0x00 ,0x53 
,0x43 ,0x44 ,0x20 ,0x32 ,0x31 ,0x2E ,0x32 ,0x2C 
,0x42 ,0x41 ,0x20 ,0x33 ,0x35 ,0x2C ,0x77 ,0x65 
,0x62 ,0x4F ,0x53};
static const uint8_t scan_rsp_data_buf[] = {
0x09 ,0x09 
,0x4C ,0x47 
,0x45 ,0x20 
,0x4D ,0x52 
,0x32 ,0x33};
#endif
/// 总计：301条（1 + 1 + 7 + 7 + 8 + 10 + 13 + 12 + 22 + 42 + 39 + 30 + 43 + 35 + 14 + 9 + 5 + 3）
const static uint8_t head_freq_buf[] =
    {
        /// a6 1次
        10,

        /// a8 1次
        10,

        /// aa 7次
        10, 10, 10, 10, 10, 10, 10,

        /// ac 7次
        10, 10, 10, 10, 10, 10, 10,

        /// ae 8次
        10, 10, 10, 10, 10, 10, 10, 10,

        /// b0 10次
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,

        /// b2 13次
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10,

        /// b4 12次(cnt:10 共10次，cnt:20共2次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        20, 20,

        /// b6 22次(cnt:10 共16次，cnt:20共5次，cnt:30共1次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10,
        20, 20, 20, 20, 20,
        30,

        /// b8 42次(cnt:10 共32次，cnt:20共7次，cnt:30共1次，cnt:40共1次，cnt:50共1次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10,
        20, 20, 20, 20, 20, 20, 20,
        30,
        40,
        50,

        /// ba 39次(cnt:10 共29次，cnt:20共7次，cnt:30共1次，cnt:40共2次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10,
        20, 20, 20, 20, 20, 20, 20,
        30,
        40, 40,

        /// bc 30次(cnt:10 共21次，cnt:20共7次，cnt:30共1次，cnt:40共1次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10,
        20, 20, 20, 20, 20, 20, 20,
        30,
        40,

        /// be 43次(cnt:10 共29次，cnt:20共10次，cnt:30共4次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10,
        20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
        30, 30, 30, 30,

        /// c0 35次(cnt:10 共24次，cnt:20共7次，cnt:30共2次，cnt:40共2次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10,
        20, 20, 20, 20, 20, 20, 20,
        30, 30,
        40, 40,

        /// c2 14次(cnt:10 共10次，cnt:20共3次，cnt:50共1次)
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        20, 20, 20,
        50,

        /// c4 9次(cnt:10 共8次，cnt:20共1次)
        10, 10, 10, 10, 10, 10, 10, 10,
        20,

        /// c6 5次(cnt:10 共4次，cnt:20共1次)
        10, 10, 10, 10,
        20,

        /// c8 3次(cnt:10 共3次)
        10, 10, 10

};

const static uint8_t head_value_buf[] =
    {
        0xA6, 0xA8, 0xAA, 0xAC, 0xAE,
        0xB0, 0xB2, 0xB4, 0xB6, 0xB8,
        0xBA, 0xBC, 0xBE, 0xC0, 0xC2,
        0xC4, 0xC6, 0xC8

};

//////////////////////////////////////////////////
/// 频率表
/// 总计：301条（1+1+7+7+8+10+13+12+22+42+39+30+43+35+14+9+5+3）
static uint8_t head_buf[] =
    {
        /// 1次
        H_A6,

        /// 1次
        H_A8,

        /// 7次
        H_AA, H_AA, H_AA, H_AA, H_AA, H_AA, H_AA,

        /// 7次
        H_AC, H_AC, H_AC, H_AC, H_AC, H_AC, H_AC,

        /// 8次
        H_AE, H_AE, H_AE, H_AE, H_AE, H_AE, H_AE, H_AE,

        /// 10次
        H_B0, H_B0, H_B0, H_B0, H_B0, H_B0, H_B0, H_B0, H_B0, H_B0,

        /// 13次
        H_B2, H_B2, H_B2, H_B2, H_B2, H_B2, H_B2, H_B2, H_B2, H_B2,
        H_B2, H_B2, H_B2,

        /// 12次
        H_B4, H_B4, H_B4, H_B4, H_B4, H_B4, H_B4, H_B4, H_B4, H_B4,
        H_B4, H_B4,

        /// 22次
        H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6,
        H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6, H_B6,
        H_B6, H_B6,

        /// 42次
        H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8,
        H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8,
        H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8,
        H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8, H_B8,
        H_B8, H_B8,

        /// 39次
        H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA,
        H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA,
        H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA,
        H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA, H_BA,

        /// 30次
        H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC,
        H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC,
        H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC, H_BC,

        /// 43次
        H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE,
        H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE,
        H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE,
        H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE, H_BE,
        H_BE, H_BE, H_BE,

        /// 35次
        H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0,
        H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0,
        H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0, H_C0,
        H_C0, H_C0, H_C0, H_C0, H_C0,

        /// 14次
        H_C2, H_C2, H_C2, H_C2, H_C2, H_C2, H_C2, H_C2, H_C2, H_C2,
        H_C2, H_C2, H_C2, H_C2,

        /// 9次
        H_C4, H_C4, H_C4, H_C4, H_C4, H_C4, H_C4, H_C4, H_C4,

        /// 5次
        H_C6, H_C6, H_C6, H_C6, H_C6,

        /// 3次
        H_C8, H_C8, H_C8

};

static const uint16_t lpm_time;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static bool mouse_on_off;

static uint16_t key_pressed_time;
static uint8_t key_pressed_num;
static uint8_t keynum_secnd;
static uint8_t keynum;

static uint8_t app_rpt_timeout_timernum = 0xFF;
static uint8_t brand_pressed_timernum = 0xFF;
static uint8_t encode_timernum = 0xFF;
static bool tx_power_switch = false;
static uint8_t paired_frst = 0;
static bool led_state;

static uint8_t usr_axis_key_buf[19];
static uint8_t auto_cnt;
static uint8_t app_rpt_timeout_type = 0;

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

static uint8_t get_app_rpt_timeout_type(void)
{
    return app_rpt_timeout_type;
}

void set_app_rpt_timeout_type(uint8_t type)
{
    app_rpt_timeout_type |= type;
}

void clr_app_rpt_timeout_type(uint8_t type)
{
    app_rpt_timeout_type &= ~type;
}

/*
0-ok
1-err
*/
static uint8_t get_voice_key_state(void)
{
    uint8_t res = 1;
    if ((key_pressed_num == 1) &&
        ((KeyBuf[keynum].keyvalue[0] == LG_AUDIO_KEYVALUE) ||
         (KeyBuf[keynum].keyvalue[0] == GOOGLE_AUDIO_KEYVALUE) ||
         (KeyBuf[keynum].keyvalue[0] == AMAZON_AUDIO_KEYVALUE)))
    {
        res = 0;
    }
    return res;
}

static uint8_t get_voice_key_rel_state(void)
{
    uint8_t res = 1;
    if (((KeyBuf[keynum].keyvalue[0] == LG_AUDIO_KEYVALUE) ||
         (KeyBuf[keynum].keyvalue[0] == GOOGLE_AUDIO_KEYVALUE) ||
         (KeyBuf[keynum].keyvalue[0] == AMAZON_AUDIO_KEYVALUE)))
    {
        res = 0;
    }
    return res;
}

/*
0-650A
1-18BA,19BA,20GA,21GA,22GA
*/
static uint8_t tv_type_state = 0;
static uint8_t get_cur_type(void)
{
    uint8_t res = 1;
    do
    {
        if (tv_type_state == KVCMD_LG650A)
        {
            res = 0;
            break;
        }
        else if (tv_type_state == KVCMD_LG18BA_19BA)
        {
        }
        else if (tv_type_state == KVCMD_LG20GA)
        {
        }
        else if (tv_type_state == KVCMD_LG21GA)
        {
        }
        else if (tv_type_state == KVCMD_LG22GA)
        {
        }
        else if (tv_type_state == KVCMD_LG23GA)
        {
        }
    } while (0);
    return res;
}

static uint8_t audio_frame_seq = 0;

static void set_audio_frame_seq(uint8_t seq)
{
    audio_frame_seq = (uint8_t)(seq);
}

static uint8_t get_audio_frame_seq(void)
{
    return audio_frame_seq++;
}

const static uint8_t msbc_flag_buf[] = {0xc8, 0x08};
const static uint8_t msbc_flag_buf1[] = {0xF8, 0x38};
static uint8_t get_msbc_flag(uint8_t flag)
{
    uint8_t res = 0xff;
    if (flag == MSBC_FLAG_0)
    {
        res = msbc_flag_buf[audio_frame_seq % 2];
    }
    else if (flag == MSBC_FLAG_1)
    {
        res = msbc_flag_buf1[audio_frame_seq % 2];
    }

    return res;
}

/*
0-audio keyvalue
1-err
kval :return keyvalue  0xff:err
*/
static uint8_t get_audio_keyvalue(uint8_t *kval)
{
    uint8_t res = 1;
    *kval = 0xff; /// err
    if (!get_voice_key_state())
    {
        res = 0;
        *kval = KeyBuf[keynum].keyvalue[0];
    }
    return res;
}

static uint8_t get_keyvalue(uint8_t *kval)
{
    uint8_t res = 1;
    *kval = 0xff; /// err
    if (key_pressed_num == 1)
    {
        res = 0;
        *kval = KeyBuf[keynum].keyvalue[0];
    }
    return res;
}

static uint8_t app_pkt_type = E_APP_PKT_TYPE_INIT;
uint8_t get_cur_pkt_type(void)
{
    return app_pkt_type;
}

void set_cur_pkt_type(uint8_t type)
{
    app_pkt_type = type;
}

#define UPT_VOICE_PKT_OFFSET (2)
/*
pkt 123byte
pkt_type
0-start audio
1-axis data
2-stop audio
*/
void update_voice_packet(uint8_t *pkt)
{
    uint8_t len = 0;
    if (pkt)
    {
        if (app_pkt_type == E_APP_PKT_TYPE_START_ADUIO) /// kv 03
        {
            pkt[0 + UPT_VOICE_PKT_OFFSET] = get_audio_frame_seq();
            uint8_t kv;
            if (!get_audio_keyvalue(&kv))
            {
                if (get_cur_type())
                {
                    pkt[1 + UPT_VOICE_PKT_OFFSET] = DKEYVALUE_FLAG;
                    pkt[2 + UPT_VOICE_PKT_OFFSET] = kv;
                }
                else
                {
                    pkt[1 + UPT_VOICE_PKT_OFFSET] = 0x00;
                    pkt[2 + UPT_VOICE_PKT_OFFSET] = 0x00;
                }
            }
            else
            {
                pkt[1 + UPT_VOICE_PKT_OFFSET] = 0x00;
                pkt[2 + UPT_VOICE_PKT_OFFSET] = 0x00;
            }

            pkt[3 + UPT_VOICE_PKT_OFFSET] = 0x01;
            pkt[4 + UPT_VOICE_PKT_OFFSET] = get_msbc_flag(MSBC_FLAG_0);
            ////////////////////////////////////
            /// audio 5-61 57byte
            ////////////////////////////////////
            pkt[62 + UPT_VOICE_PKT_OFFSET] = 0x00; /// 5 + 54 + 3
            pkt[63 + UPT_VOICE_PKT_OFFSET] = 0x01; /// 54 + 3 + 5 + 1 + 0
            pkt[64 + UPT_VOICE_PKT_OFFSET] = get_msbc_flag(MSBC_FLAG_1);
            ////////////////////////////////////
            /// audio 65-121 57byte
            ////////////////////////////////////
            pkt[122 + UPT_VOICE_PKT_OFFSET] = 0x00; /// 63 + 2 + 54 + 3

            len = 123;
        }
        else if (app_pkt_type == E_APP_PKT_TYPE_AXIS_DATA) /// key release
        {
            memcpy(&pkt[2], &usr_axis_key_buf[0], 19);
            app_pkt_type = E_APP_PKT_TYPE_AUDIO_RELEASE;
            len = 19;
            // DEBUG_LOG_STRING("key rel vck\r\n");
        }
        else if (app_pkt_type == E_APP_PKT_TYPE_AUDIO_RELEASE) /// kv 04
        {
            pkt[0 + UPT_VOICE_PKT_OFFSET] = get_audio_frame_seq();

            pkt[1 + UPT_VOICE_PKT_OFFSET] = 0x00;
            pkt[2 + UPT_VOICE_PKT_OFFSET] = 0x00;

            pkt[3 + UPT_VOICE_PKT_OFFSET] = 0x01;
            pkt[4 + UPT_VOICE_PKT_OFFSET] = get_msbc_flag(MSBC_FLAG_0);
            ////////////////////////////////////
            /// audio 5-61 57byte
            ////////////////////////////////////
            pkt[62 + UPT_VOICE_PKT_OFFSET] = 0x00; /// 5 + 54 + 3
            pkt[63 + UPT_VOICE_PKT_OFFSET] = 0x01; /// 54 + 3 + 5 + 1 + 0
            pkt[64 + UPT_VOICE_PKT_OFFSET] = get_msbc_flag(MSBC_FLAG_1);
            ////////////////////////////////////
            /// audio 65-121 57byte
            ////////////////////////////////////
            pkt[122 + UPT_VOICE_PKT_OFFSET] = 0x00; /// 63 + 2 + 54 + 3
            len = 123;
        }

        if (len)
        {
            if (len == 19)
            {
                pkt[0] = DAXIS_KEY_HANDLE; /// handle
            }
            else if (len == 123)
            {
                pkt[0] = DVOICE_HANDLE; /// handle
            }
            pkt[1] = len; /// len
        }
    }
}

/*
calibration axis
*/
static int16_t acc_pre[3];
static int16_t gyro_pre[3];
static uint8_t cnt = 0;
static int16_t gyro_raw_buf[GYRO_BUF_SIZE][3];
static int16_t gyro_offset[3] = {0, 0, 0};
static void auto_calibration_axis(int16_t *acc_in, int16_t *gyro_in, int16_t *gyro_out)
{
    if (acc_in && gyro_in && gyro_out)
    {
        int16_t delta_acc[3];
        int16_t delta_gyro[3];
        int32_t sum_gyro[3] = {0, 0, 0};
        uint8_t i = 0;

        delta_acc[0] = acc_in[0] - acc_pre[0];
        delta_acc[1] = acc_in[1] - acc_pre[1];
        delta_acc[2] = acc_in[2] - acc_pre[2];

        acc_pre[0] = acc_in[0];
        acc_pre[1] = acc_in[1];
        acc_pre[2] = acc_in[2];

        delta_gyro[0] = gyro_in[0] - gyro_pre[0];
        delta_gyro[1] = gyro_in[1] - gyro_pre[1];
        delta_gyro[2] = gyro_in[2] - gyro_pre[2];

        gyro_pre[0] = gyro_in[0];
        gyro_pre[1] = gyro_in[1];
        gyro_pre[2] = gyro_in[2];

        // DEBUG_LOG_STRING("dx:%d,%d,%d,%d,%d,%d\r\n", delta_acc[0], delta_acc[1], delta_acc[2], delta_gyro[0], delta_gyro[1],
        //                  delta_gyro[2]);

        /// 静止时acc的数据范围为30以下可以顺利时入校准
        /// 把鼠标竖直起来范围为40以下可以顺利进入校准
        if ((((app_abs(delta_acc[0]) < ((ACC_RESOLUTION_THR_VALUE))) 
        && (app_abs(delta_acc[1]) < ((ACC_RESOLUTION_THR_VALUE))) 
        && (app_abs(delta_acc[2]) < ((ACC_RESOLUTION_THR_VALUE))) 
        && (app_abs(delta_gyro[0]) < GYRO_RESOLUTION_THR_VALUE) 
        && (app_abs(delta_gyro[1]) < GYRO_RESOLUTION_THR_VALUE) 
        && (app_abs(delta_gyro[2]) < GYRO_RESOLUTION_THR_VALUE)) ||
            ////////////////////////////////////////////////////////////////////////////////
            //////// 校准设置两个边限，小的为静止边限，大的为大到一定幅度也会自动校准
            ((app_abs(delta_acc[0]) > ((ACC_RESOLUTION_THR_VALUE_MAX))) && 
            (app_abs(delta_acc[1]) > ((ACC_RESOLUTION_THR_VALUE_MAX))) && 
            (app_abs(delta_acc[2]) > ((ACC_RESOLUTION_THR_VALUE_MAX))) &&
             (app_abs(delta_gyro[0]) > GYRO_RESOLUTION_THR_VALUE_MAX) && 
             (app_abs(delta_gyro[1]) > GYRO_RESOLUTION_THR_VALUE_MAX) && 
             (app_abs(delta_gyro[2]) > GYRO_RESOLUTION_THR_VALUE_MAX))) && auto_cnt++ > 100)
        {
            auto_cnt = 0; 
            gyro_raw_buf[cnt][0] = gyro_in[0];
            gyro_raw_buf[cnt][1] = gyro_in[1];
            gyro_raw_buf[cnt][2] = gyro_in[2];

            if (cnt >= (GYRO_BUF_SIZE - 1))
            {
                for (i = 0; i < GYRO_BUF_SIZE; i++)
                {
                    sum_gyro[0] += gyro_raw_buf[i][0];
                    sum_gyro[1] += gyro_raw_buf[i][1];
                    sum_gyro[2] += gyro_raw_buf[i][2];
                }

                gyro_offset[0] = (int16_t)(sum_gyro[0] / GYRO_BUF_SIZE);
                gyro_offset[1] = (int16_t)(sum_gyro[1] / GYRO_BUF_SIZE);
                gyro_offset[2] = (int16_t)(sum_gyro[2] / GYRO_BUF_SIZE);
                DEBUG_LOG_STRING("axis calibration:%d	%d	%d\n", gyro_offset[0], gyro_offset[1], gyro_offset[2]);
                cnt = 0;
            }
            else
            {
                cnt++;
            }
        }
        else
        {
            auto_cnt = 0; 
            cnt = 0;
         }

        gyro_out[0] = gyro_in[0] - gyro_offset[0];
        gyro_out[1] = gyro_in[1] - gyro_offset[1];
        gyro_out[2] = gyro_in[2] - gyro_offset[2];
    }
}

static void update_axis_2_usr_data(int16_t *acc_in, int16_t *gyro_in)
{
    int16_t gyro_out[3];
    float gyro[3], acc[3];
    auto_calibration_axis(acc_in, gyro_in, gyro_out);
    acc[0] = acc_in[0] * (1.0f / get_acc_lsb_div()) * AXRATIO * G_CONST_VALUE;
    acc[1] = acc_in[1] * (1.0f / get_acc_lsb_div()) * AYRATIO * G_CONST_VALUE;
    acc[2] = acc_in[2] * (1.0f / get_acc_lsb_div()) * AZRATIO * G_CONST_VALUE;

    gyro[0] = gyro_out[0] * (1.0f / get_gyro_lsb_div()) * GYXRATIO * GYRO_CONST_VALUE;
    gyro[1] = gyro_out[1] * (1.0f / get_gyro_lsb_div()) * GYYRATIO * GYRO_CONST_VALUE;
    gyro[2] = gyro_out[2] * (1.0f / get_gyro_lsb_div()) * GYZRATIO * GYRO_CONST_VALUE;

    /// ax
    usr_axis_key_buf[ACC_GYRO_FRM_AX_H] = (((int16_t)gyro[0]) >> 8) & 0xff;
    usr_axis_key_buf[ACC_GYRO_FRM_AX_L] = (((int16_t)gyro[0]) >> 0) & 0xff;
    /// ay
    usr_axis_key_buf[ACC_GYRO_FRM_AY_H] = (((int16_t)gyro[1]) >> 8) & 0xff;
    usr_axis_key_buf[ACC_GYRO_FRM_AY_L] = (((int16_t)gyro[1]) >> 0) & 0xff;
    /// az
    usr_axis_key_buf[ACC_GYRO_FRM_AZ_H] = (((int16_t)gyro[2]) >> 8) & 0xff;
    usr_axis_key_buf[ACC_GYRO_FRM_AZ_L] = (((int16_t)gyro[2]) >> 0) & 0xff;
    /// gx
    usr_axis_key_buf[ACC_GYRO_FRM_GX_H] = (((int16_t)acc[0]) >> 8) & 0xff;
    usr_axis_key_buf[ACC_GYRO_FRM_GX_L] = (((int16_t)acc[0]) >> 0) & 0xff;
    /// gy
    usr_axis_key_buf[ACC_GYRO_FRM_GY_H] = (((int16_t)acc[1]) >> 8) & 0xff;
    usr_axis_key_buf[ACC_GYRO_FRM_GY_L] = (((int16_t)acc[1]) >> 0) & 0xff;
    /// gz
    usr_axis_key_buf[ACC_GYRO_FRM_GZ_H] = (((int16_t)acc[2]) >> 8) & 0xff;
    usr_axis_key_buf[ACC_GYRO_FRM_GZ_L] = (((int16_t)acc[2]) >> 0) & 0xff;
    // DEBUG_LOG_STRING("ux:%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\r\n", usr_axis_key_buf[4], usr_axis_key_buf[5], usr_axis_key_buf[6], usr_axis_key_buf[7],
    // usr_axis_key_buf[8], usr_axis_key_buf[9], usr_axis_key_buf[10], usr_axis_key_buf[11],
    // usr_axis_key_buf[12], usr_axis_key_buf[13], usr_axis_key_buf[14], usr_axis_key_buf[15]);
}

/*
kv --keyval
kv --special val
*/
static void update_usr_keyvalue(uint8_t key_state, uint8_t kv)
{
    if (key_state) /// press
    {
        usr_axis_key_buf[ACC_GYRO_FRM_K1] = DKEYVALUE_FLAG;
        usr_axis_key_buf[ACC_GYRO_FRM_KV] = kv;
    }
    else /// release
    {
        usr_axis_key_buf[ACC_GYRO_FRM_K1] = 0x00;
        usr_axis_key_buf[ACC_GYRO_FRM_KV] = 0x00;
    }
}

static uint8_t get_frame_len(uint8_t *tag)
{
    uint8_t len = 0;
    int num = rand() % HHEAD_TOTAL_LEN; // 产生一个1-10之间的数
    len = head_freq_buf[num % HHEAD_TOTAL_LEN];
    *tag = head_value_buf[head_buf[num % HHEAD_TOTAL_LEN] % H_HEAD_MAX];

    return len;
}

static uint8_t hstep = 0;
static uint8_t hcnt = 0;
static uint8_t htag = 0;

static void update_frm_head(void)
{
    if (hstep == 0)
    {
        hcnt = get_frame_len(&htag) - 1;
        hstep++;
    }
    else if (hstep == 1)
    {
        if (--hcnt == 0)
        {
            hstep = 0;
        }
    }

    usr_axis_key_buf[ACC_GYRO_FRM_HEAD] = htag;
}

static uint8_t frame_seq = 0;

static uint8_t get_frame_seq(void)
{
    return frame_seq;
}

void set_frame_seq_val(void)
{
    usr_axis_key_buf[ACC_GYRO_FRM_SEQ] = frame_seq++;
}

void set_frame_seq(uint8_t seq)
{
    frame_seq = seq;
}

static void update_frame_h1_h2(uint8_t h1, uint8_t h2)
{
    usr_axis_key_buf[ACC_GYRO_FRM_H1] = h1;
    usr_axis_key_buf[ACC_GYRO_FRM_H2] = h2;
}

static uint8_t kv_cmd_state_value = 0;
static uint8_t get_kvcmd_state(void)
{
    return kv_cmd_state_value;
}

void set_kvcmd_state(uint8_t state)
{
    kv_cmd_state_value |= state;
}

void clr_kvcmd_state(uint8_t state)
{
    kv_cmd_state_value &= ~state;
}

static void update_encode_value_pkt(uint8_t val)
{
    usr_axis_key_buf[ACC_GYRO_FRM_END] = val;
}

static void start_axis_rpt_ptk(uint8_t h1, uint8_t kv, uint8_t crpt)
{
    usr_axis_key_buf[ACC_GYRO_FRM_H1] = h1;
    usr_axis_key_buf[ACC_GYRO_FRM_K1] = kv;
    usr_axis_key_buf[ACC_GYRO_FRM_KV] = crpt;
}

static uint8_t app_ka_cur_state = E_APP_CUR_KA_STATE_INIT;
static uint8_t axis_init_en = 0;
static uint8_t moving_press_2_init_state = 0; /// 1-enable,0-disable
static uint8_t axis_key_stop_cmd_step = 0;    /// 1-start,0-stop
static uint8_t moving_frst = 0;
static uint8_t axis_key_enable = 1;
static uint8_t kv04_cmd_rpt_pkt_en = 0;
static uint8_t encode_chk_ok = 0;                         /// 1-ok,0-fail
static uint8_t encode_chk_value = ENCODE_DIR_BLE_UNCHECK; /// value

/// @brief
/// @param
/// @return encode value
static uint8_t get_encode_state(void)
{
    uint8_t res = ENCODE_DIR_BLE_UNCHECK;
    if (encode_chk_ok)
    {
        encode_chk_ok = 0;
        if (encode_chk_value == ENCODE_DIR_UP)
        {
            res = 0x01;
        }
        else if (encode_chk_value == ENCODE_DIR_DOWN)
        {
            res = 0xff;
        }
        encode_chk_value = ENCODE_DIR_BLE_UNCHECK;
    }

    return res;
}

static uint8_t key_chk_ok = 0;
/*
voice,key,axis
*/
static uint8_t all_state_switch(int16_t *acc_in, int16_t *gyro_in)
{
    uint8_t res = 1;
    uint8_t kv;
    if (!get_mouse_int_state() || !get_keyvalue(&kv))
    {
        // DEBUG_LOG_STRING("moving...\r\n");
        app_sleep_lock_set(MOUSE_LOCK, true);
        if (!moving_frst)
        {
            moving_frst = 1;
            moving_press_2_init_state = 0;
            axis_key_enable = 1;
            DEBUG_LOG_STRING("krst\r\n");
        }
    }

    if (bt_check_le_connected() && axis_key_enable)
    {
        uint8_t head_upt = 0;
        do
        {
            if (kv04_cmd_rpt_pkt_en) /// kv04 ptk
            {
                kv04_cmd_rpt_pkt_en = 0;
                head_upt = 1;
                set_frame_seq(get_audio_frame_seq() - 1);
                set_frame_seq_val();
                start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x0d);
                break;
            }

            // DEBUG_LOG_STRING("ahs:%d,%d,%d\r\n", get_axis_moving_state(), get_keyvalue(&kv), moving_press_2_init_state);
            if ((get_axis_moving_state() && get_keyvalue(&kv)) && moving_press_2_init_state) /// static unpress
            {
                DEBUG_LOG_STRING("ke1:%d\r\n", app_ka_cur_state);
                /// stop update data
                if (app_ka_cur_state == E_APP_CUR_KA_STATE_INIT) /// init
                {
                }
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY) /// key release
                {
                    start_axis_rpt_ptk(DAXIS_H1, 0x00, 00);
                    head_upt = 1;
                }
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY_AXIS) /// key+axis
                {
                }
                else /// axis
                {
                }
                app_ka_cur_state = E_APP_CUR_KA_STATE_INIT;
                axis_init_en = 0;
                moving_press_2_init_state = 1;
                moving_frst = 0;
                axis_key_enable = 0;
                key_chk_ok = 0;
                break;
            }
            else if (!get_axis_moving_state() && !get_keyvalue(&kv)) /// moving press
            {
                // DEBUG_LOG_STRING("mp:%d\r\n", app_ka_cur_state);
                if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY) /// key->key+axis
                {
                    set_frame_seq(0); /// seq=0 waiting key -> switch key+axis
                    set_frame_seq_val();
                    update_axis_2_usr_data(acc_in, gyro_in);            /// update axis
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x3e); /// start axis
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY_AXIS;     /// key+axis
                }
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_AXIS) /// axis->key+axis
                {
                    update_axis_2_usr_data(acc_in, gyro_in); /// update axis
                    start_axis_rpt_ptk(0xfd, DKEYVALUE_FLAG, kv);
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY_AXIS; /// key+axis
                }
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY_AXIS) /// key+axis->key+axis
                {
                    set_frame_seq_val();
                    update_axis_2_usr_data(acc_in, gyro_in); /// update axis
                    start_axis_rpt_ptk(0xfd, DKEYVALUE_FLAG, kv);
                    if (get_kvcmd_state() & E_KV_CMD_VALUE_KV03) /// mic open
                    {
                        clr_kvcmd_state(E_KV_CMD_VALUE_KV03);
                        start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x3f); /// stop axis
                        axis_key_stop_cmd_step = 1;
                    }
                    else if(axis_key_stop_cmd_step)
                    {
                        axis_key_stop_cmd_step = 0;
                        start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x0a); /// start voice
                        DEBUG_LOG_STRING("start voice\r\n");
                        set_cur_pkt_type(E_APP_PKT_TYPE_START_ADUIO);
                        set_audio_frame_seq(get_frame_seq());
                        mic_open();
                        axis_key_enable = 0;
                        app_ka_cur_state = E_APP_CUR_KA_STATE_INIT;

                        update_frm_head();
                    }
                }
                else /// init
                {
                    if (get_kvcmd_state() & E_KV_CMD_VALUE_KV01)
                    {
                        clr_kvcmd_state(E_KV_CMD_VALUE_KV01);
                    }
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY;
                    head_upt = 0;
                    break;
                }
                head_upt = 1;
                break;
            }
            else if (!get_axis_moving_state() || (!moving_press_2_init_state && get_keyvalue(&kv))) /// moving unpress
            {
                // DEBUG_LOG_STRING("m1:%d\r\n", app_ka_cur_state);
                uint8_t encode_value = get_encode_state();
                if (encode_value != ENCODE_DIR_BLE_UNCHECK)
                {
                    update_encode_value_pkt(encode_value);
                }
                else
                {
                    update_encode_value_pkt(0x00);
                }

                if (app_ka_cur_state == E_APP_CUR_KA_STATE_INIT) /// init->axis
                {
                    set_frame_seq(0); /// seq=0 waiting key -> switch key+axis
                    set_frame_seq_val();
                    update_axis_2_usr_data(acc_in, gyro_in);            /// update axis
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x3e); /// start axis
                    app_ka_cur_state = E_APP_CUR_KA_STATE_AXIS;
                }
                // else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY) /// key->axis
                // {
                // }
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY_AXIS) /// key+axis->axis
                {
                    update_axis_2_usr_data(acc_in, gyro_in); /// update axis
                    start_axis_rpt_ptk(0xfd, 0x00, 0x00);
                    app_ka_cur_state = E_APP_CUR_KA_STATE_AXIS;
                }
                else /// axis
                {
                    app_ka_cur_state = E_APP_CUR_KA_STATE_AXIS;
                    /// update seq
                    set_frame_seq_val();
                    update_axis_2_usr_data(acc_in, gyro_in);     /// update axis
                    start_axis_rpt_ptk(0xfd, 0x00, 0x00);        /// start axis
                    if (get_kvcmd_state() & E_KV_CMD_VALUE_KV02) /// stop axis cmd
                    {
                        clr_kvcmd_state(E_KV_CMD_VALUE_KV02);
                        start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x3f); /// stop axis
                        moving_press_2_init_state = 1;                      /// stop moving update
                        DEBUG_LOG_STRING("kv021\r\n");
                        moving_frst = 0;
                        axis_key_enable = 0;
                        key_chk_ok = 0;
                    }
                }
                head_upt = 1;
                break;
            }
            else if (!get_keyvalue(&kv)) /// static press
            {
                // DEBUG_LOG_STRING("k1:%d\r\n", app_ka_cur_state);
                // DEBUG_LOG_STRING("key1c\r\n");
                if (app_ka_cur_state == E_APP_CUR_KA_STATE_INIT) /// init->key
                {
                    set_frame_seq(0); /// alway seq=0 waiting axis moving -> switch key+axis
                    set_frame_seq_val();
                    if (!axis_init_en)
                    {
                        axis_init_en = 1;
                        update_axis_2_usr_data(acc_in, gyro_in); /// only one
                    }
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, kv);
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY; /// key
                    DEBUG_LOG_STRING("key1\r\n");
                }
                // else if (app_ka_cur_state == E_APP_CUR_KA_STATE_AXIS) /// axis->key
                // {
                // }
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY_AXIS) /// key+axis->key
                {
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x3f);
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY; /// key
                }
                else /// key
                {
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY; /// key
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, kv);
                    // DEBUG_LOG_STRING("key2\r\n");
                    /*
                    voice key waiting kv03
                    */
                    if (get_kvcmd_state() & E_KV_CMD_VALUE_KV03) /// mic open
                    {
                        clr_kvcmd_state(E_KV_CMD_VALUE_KV03);
                        set_cur_pkt_type(E_APP_PKT_TYPE_START_ADUIO);
                        set_audio_frame_seq(get_frame_seq());
                        mic_open();
                        axis_key_enable = 0;
                        start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x0a); /// start voice
                        app_ka_cur_state = E_APP_CUR_KA_STATE_INIT;
                    }
                }
                head_upt = 1;
                break;
            }

        } while (0);

        if (head_upt)
        {
            update_frm_head();
            res = 0;
        }
    }

    return res;
}

static void mouse_handle(void *data)
{

    sensor_data_t *sensor_data = (sensor_data_t *)data;
    uint8_t kv;

    int16_t acc_buf[3], gyro_buf[3];
    acc_buf[0] = sensor_data->ax;
    acc_buf[1] = sensor_data->ay;
    acc_buf[2] = sensor_data->az;

    gyro_buf[0] = sensor_data->gx;
    gyro_buf[1] = sensor_data->gy;
    gyro_buf[2] = sensor_data->gz;
    // DEBUG_LOG_STRING("acc_buf = %d %d %d,gyro_buf = %d %d %dr\n",acc_buf[0], acc_buf[1], acc_buf[2], gyro_buf[0],gyro_buf[1] ,gyro_buf[2]);
    if (!all_state_switch(acc_buf, gyro_buf))
    {
        if(Bt_HciGetUsedBufferNum() <= 12) {
            ATT_sendNotify(MOUSE_KEY_ENCODE_ATTLIST_HANDLE, usr_axis_key_buf, sizeof(usr_axis_key_buf));
        }else{
            DEBUG_LOG_STRING("start axis... %d\r\n",Bt_HciGetUsedBufferNum());
        }
    }
}

static void app_rpt_timeout_handle(void)
{
    if ((get_app_rpt_timeout_type() & E_APP_RPT_TM_EVT_MOUSE_START) && !mouse_on_off)
    {
        clr_app_rpt_timeout_type(E_APP_RPT_TM_EVT_MOUSE_START);
        mouse_init(mouse_handle);
        DEBUG_LOG_STRING("start axis...\r\n");
    }

    if (get_app_rpt_timeout_type() & E_APP_RPT_TM_EVT_BACK_HOME_UNPAIRED)
    {
        clr_app_rpt_timeout_type(E_APP_RPT_TM_EVT_BACK_HOME_UNPAIRED);
        if (bt_check_le_connected())
        {
            bt_send_le_disconnect(0x13);
            DEBUG_LOG_STRING("HOME BACK DISCON\r\n");
        }
        stop_adv();
        Bt_ClearRemoteDevInfo();
        Bt_ClearDeviceNvdataInfo();
    }
}

static void encode_handle(void)
{
    uint8_t encode_value = get_encode_chk_ok_state();
    if (encode_value != ENCODE_DIR_UNCHECK)
    {
        // DEBUG_LOG_STRING("encode_value[%d]\r\n", encode_value);
        if (!bt_check_le_connected())
        {
            ir_comm_send(encode_value);
        }
        else
        {
            encode_chk_ok = 1;
            encode_chk_value = encode_value;
        }
    }
}

static void key_pressed_handle(void)
{
    if (key_pressed_num == 2 && keynum == APP_KEY_IDX_BACK && keynum_secnd == APP_KEY_IDX_HOME)
    {
        if (++key_pressed_time > 2) /// 3S
        {
            keynum_secnd = 0;
            keynum = 0;
            key_pressed_time = 0;
            if (bt_check_le_connected())
            {
                uint8_t cmd1[] = {0x00};
                uint8_t cmd2[] = {0x40, 0x00};
                axis_key_enable = 0;
                ATT_sendNotify(BACK_HOME_UNPAIRED_ATTLIST_HANDLE, cmd1, 1);
                ATT_sendNotify(BACK_HOME_UNPAIRED_ATTLIST_HANDLE, cmd2, 2);
                set_app_rpt_timeout_type(E_APP_RPT_TM_EVT_BACK_HOME_UNPAIRED);

                swtimer_start(app_rpt_timeout_timernum, 20, TIMER_START_ONCE);
            }
            else
            {

                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10);
            }
            led_state = true;
            led_on(LED_1, 200, 1000);
        }
        else
        {
            swtimer_restart(brand_pressed_timernum);
        }
    }
    else if(key_pressed_num == 2 && keynum == APP_KEY_IDX_SET && keynum_secnd == APP_KEY_IDX_HOME)
    {
        if (++key_pressed_time > 2) /// 3S
        {
            keynum_secnd = 0;
            keynum = 0;
            key_pressed_time = 0;
            if (bt_check_le_connected())
            {
                uint8_t cmd1[] = {0x00};
                uint8_t cmd2[] = {0x40, 0x00};
                axis_key_enable = 0;
                ATT_sendNotify(BACK_HOME_UNPAIRED_ATTLIST_HANDLE, cmd1, 1);
                ATT_sendNotify(BACK_HOME_UNPAIRED_ATTLIST_HANDLE, cmd2, 2);
                set_app_rpt_timeout_type(E_APP_RPT_TM_EVT_BACK_HOME_UNPAIRED);

                swtimer_start(app_rpt_timeout_timernum, 20, TIMER_START_ONCE);
                uint8_t set_home = 1;        
                flash_write(set_home_state, (uint8_t *)&set_home, sizeof(set_home), STATE_INF);
                // SysTick_DelayMs(100);
            }
            else
            {
                uint8_t set_home = 1;
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                // start_adv(ADV_TYPE_NOMAL, 0x10);
                DEBUG_LOG_STRING("set home\r\n");
                flash_write(set_home_state, (uint8_t *)&set_home, sizeof(set_home), STATE_INF);
                SysTick_DelayMs(100);
                SYS_RESET();
            }
            led_state = true;
            led_on(LED_1, 200, 1000);
        }
        else
        {
            swtimer_restart(brand_pressed_timernum);
        }
    }
}

static void keyvalue_handle(key_report_t *key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    if (key_pressed_num == 0)
    {
        if(!led_state) {
            led_off(LED_1);
        }
        set_key_press_state(false);
        if (!get_voice_key_rel_state())
        {
            app_pkt_type = E_APP_PKT_TYPE_AXIS_DATA;
            set_frame_seq(get_audio_frame_seq() - 1);
            set_frame_seq_val();
            start_axis_rpt_ptk(DAXIS_H1, 0x00, 0x00);
        }
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] + 
            key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] + 
            key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];

        DEBUG_LOG_STRING("keynum[%d]\r\n", keynum);
        /*pwr ir*/
        if (keynum == APP_KEY_IDX_PWR) /// pwr
        {
            set_key_press_state(true);
            ir_comm_send(APP_KEYVALUE_IR_OK);
        }
        else if (!bt_check_le_connected()) /*unpaire 83 44paired 44*/
        {
            if (keynum == APP_KEY_IDX_OK) /// ok
            {
                if (Bt_CheckIsPaired()) /// 44
                {
                    set_key_press_state(true);
                }
                else
                {
                    led_state = true;
                    led_on(LED_1, 0, 15000);
                    // start_adv(ADV_TYPE_NOMAL, 0x10);
                    ir_single_send(APP_KEYVALUE_IR_PAIRED, 2);
                    DEBUG_LOG_STRING("paire ir\r\n");
                }
            }
            else
            {
                set_key_press_state(true);
            }

            ir_comm_send(IRKeyBuf[keynum][0]);
        }
        else
        {
            DEBUG_LOG_STRING("online key chk ok:%d\r\n", keynum);
            if (!key_chk_ok)
            {
                key_chk_ok = 1;
                moving_frst = 0;
                app_sleep_lock_set(MOUSE_LOCK, true);
            }
        }

        if(!led_state) {
            led_on(LED_1, 0, 0);
        }
    }
    else if (key_pressed_num == 2)
    {
        set_key_press_state(false);
        led_off(LED_NUM);
        DEBUG_LOG_STRING("KEYNUM [%d] [%d] [%d] [%d] [%d] [%d] [%d]\r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1],
                         key_report->keynum_report_buf[2], key_report->keynum_report_buf[3], key_report->keynum_report_buf[4],
                         key_report->keynum_report_buf[5], key_report->keynum_report_buf[6]);
        if (key_report->keynum_report_buf[1] == APP_KEY_IDX_HOME&& key_report->keynum_report_buf[2] == APP_KEY_IDX_BACK)
        {
            keynum = key_report->keynum_report_buf[2];
            keynum_secnd = key_report->keynum_report_buf[1];
            key_pressed_time = 0;
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_ONCE);
        }
        else if(key_report->keynum_report_buf[1] == APP_KEY_IDX_HOME&& key_report->keynum_report_buf[3] == APP_KEY_IDX_SET){
            
            keynum = key_report->keynum_report_buf[3];
            keynum_secnd = key_report->keynum_report_buf[1];
            key_pressed_time = 0;
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_ONCE);
        }


    }
}

static void power_handle(uint8_t batlevel)
{
    if (batlevel <= 20 && tx_power_switch)
    {
        tx_power_switch = false;
        bt_set_tx_power(TX_POWER_5DB);
    }

    if (batlevel <= 5)
    {
        if (bt_check_le_connected())
        {
            ATT_sendNotify(BAT_REPORT_HANDLE, &batlevel, 1);
        }
    }
    else
    {
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
    if (mouse_on_off)
    {
        mouse_on_off = false;

        mouse_turn_off();
        bt_disable_le_tx_md();
        System_ChangeXtal24M();

        app_sleep_lock_set(MOUSE_LOCK, false);
    }
    else
    {
        mouse_on_off = true;

        mouse_turn_on();
        bt_enable_le_tx_md();
        System_ChangeDPLL(CLOCK_DPLL_48M_multiple);
        app_sleep_lock_set(MOUSE_LOCK, true);
    }
}

void action_after_led_blk(void)
{
    if(led_state) {
        led_state = false;
    }
}

void action_after_mic_close(void){
    led_state = false;
    led_off(LED_1);
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
    start_adv(ADV_TYPE_NOMAL, 0x30);
}

void update_conn_param(bool is_sleep)
{
    DEBUG_LOG_STRING("is_sleep %d \r\n", is_sleep);
    if (is_sleep)
    {
        bt_le_conn_updata_param_req(0x08, 0x08, 4, 300);
    }
    else
    {
        bt_le_conn_updata_param_req(0x08, 0x08, 4, 300);
    }
}

void start_adv(enum advType type, uint16_t adv_interval)
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
        app_sleep_timer_set(SHUTDOWN_TIME);

    }
    else if (bt_adv_param.Type == ADV_TYPE_DIRECT)
    {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        app_sleep_timer_set(DIRECT_ADV_TIME);
    }
}

void stop_adv(void)
{
    if (bt_check_le_connected()) return;
    else
    {
        if (bt_get_le_state() == BLE_DIRECT_ADV)
        {
            IPC_TxControlCmd(BT_CMD_STOP_DIRECT_ADV);
        }
        else
        {
            IPC_TxControlCmd(BT_CMD_STOP_ADV);
        }
    }
    bt_set_le_state(BLE_IDLE);
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
    memcpy((void *)&senddata[1], table->dataPtr, sendlen);
    ATT_Send(senddata, sendlen + 1);
    
    if ((table->handle == 22) && !mouse_on_off)
    {
        if(!Bt_CheckIsPaired()){
            paired_frst = 1;
        }
        set_app_rpt_timeout_type(E_APP_RPT_TM_EVT_MOUSE_START);
        swtimer_start(app_rpt_timeout_timernum, DAXIS_START_TIMER_MAX, TIMER_START_ONCE);
    }
}

const static uint8_t handle_44_6d_buf[] = {0x6d};
const static uint8_t handle_44_6d_rsp_buf[] = {0x6d, 0x00};
const static uint8_t handle_44_6c_buf[] = {0x6c, 0x01};
const static uint8_t handle_44_6c_rsp_buf[] = {0x6c, 0x00};
const static uint8_t handle_44_kv01_buf[] = {0x01};
const static uint8_t handle_44_kv02_buf[] = {0x02};
const static uint8_t handle_44_kv03_buf[] = {0x03};
const static uint8_t handle_44_kv04_buf[] = {0x04};
const static uint8_t handle_44_kv11_buf[] = {0x11};
const static uint8_t handle_44_kv11_rsp_buf[] = {0x12, 0x15, 0x61, 0x02, 0x02, 0x28, 0x34, 0x23, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const static uint8_t handle_44_kv13_buf[] = {0x13};
const static uint8_t handle_44_kv13_rsp_buf[] = {0x14, 0x15, 0x61, 0x02, 0x40, 0x22, 0x80, 0x01, 0x40, 0x03, 0x60, 0x11};
const static uint8_t handle_44_kv17_buf[] = {0x17};
const static uint8_t handle_44_kv17_rsp_buf[] = {0x19, 0x34, 0x23, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const static uint8_t handle_44_kv19_buf[] = {0x19};
const static uint8_t handle_44_kv19_rsp_buf[] = {0x19, 0x34, 0x23, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const static uint8_t handle_44_kv90_buf[] = {0x90};
static const uint8_t unknow_1byte_17_rsp[] = {
    0x17, 0x01, 0x96, 0x00, 0x04, 0x01, 0x05, 0x00, 0xFB, 0xFA, 0xE1, 0xE7, 0x0A, 0xD7, 0x23, 0x3C, 0x0A, 0xD7, 0xA3, 0x3C, 0xCD, 0xCC, 0x9C, 0x28, 0xCD, 0xCC, 0x9C, 0xC2, 0x00, 0x00, 0x00, 0x00, 0xCD,
    0xCC, 0x9C, 0x42, 0xCD, 0xCC, 0x9C, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCC, 0x9C, 0x42, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x2D, 0x10, 0x41, 0xE0, 0x2D, 0x10,
    0x41, 0xE0, 0x2D, 0x10, 0x41, 0x00, 0x00, 0x80, 0xBF, 0x6F, 0xA1, 0x8B, 0x20, 0x6F, 0xA1, 0x8B, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x6F, 0xA1, 0x8B, 0x3A, 0x6F, 0xA1, 0x8B, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6F, 0xA1, 0x8B, 0x3A, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00

};

static const uint8_t unknow_1byte_17_2_rsp[] = {
    0x17, 0x00, 0x6E, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6F, 0xA1, 0x8B, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6F, 0xA1, 0x8B, 0x3A, 0x00, 0x00, 0x80, 0x3F, 0x6F, 0xA1, 0x8B, 0x3A, 0x0A, 0x1C, 0x01, 0x40, 0xC2, 0xB8, 0xB2, 0x3E, 0x5D, 0xC4, 0x27, 0x40, 0x5D, 0xC4, 0x27, 0x40, 0x5D, 0xC4, 0x27, 0x40, 0x00,
    0x48, 0x46, 0xC1, 0x64, 0x43, 0xC4, 0x3D, 0xA9, 0x58, 0xD1, 0x39, 0xE4, 0xBA, 0xAC, 0xAB, 0x8B, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x6F, 0xA1, 0x8B, 0x3A, 0x6F, 0xA1, 0x8B, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6F, 0xA1, 0x8B, 0x3A, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00

};

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);
    // DEBUG_LOG_STRING("data %d \r\n", data[0]);
    if (table->handle == 44)
    {        
        if ((len == 1) && !memcmp(handle_44_kv03_buf, data, 1))
        {
            DEBUG_LOG_STRING("kv 03\r\n");
            // set_kvcmd_state(E_KV_CMD_VALUE_KV03);
            set_cur_pkt_type(E_APP_PKT_TYPE_START_ADUIO);
            set_audio_frame_seq(get_frame_seq());
            axis_key_enable = 0;
            start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x0a); /// start voice
            app_ka_cur_state = E_APP_CUR_KA_STATE_INIT;
            update_frm_head();
            ATT_sendNotify(MOUSE_KEY_ENCODE_ATTLIST_HANDLE, usr_axis_key_buf, sizeof(usr_axis_key_buf));
            mouse_off();
            mic_open();
        }
        else if ((len == 1) && !memcmp(handle_44_kv04_buf, data, 1))
        {
            DEBUG_LOG_STRING("kv 04\r\n");
            mic_close();
           // mouse_switch();
            mouse_on();
            axis_key_enable = 1;
            kv04_cmd_rpt_pkt_en = 1;
        }
        else if ((len == 1) && !memcmp(handle_44_6d_buf, data, 1))
        {
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_6d_rsp_buf[0], sizeof(handle_44_6d_rsp_buf));
        }
        else if ((len == 2) && !memcmp(handle_44_6c_buf, data, 2))
        {
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_6c_rsp_buf[0], sizeof(handle_44_6c_rsp_buf));
        }
        else if ((len == 1) && !memcmp(handle_44_kv01_buf, data, 1))
        {
            // DEBUG_LOG_STRING("kv 01\r\n");
            set_kvcmd_state(E_KV_CMD_VALUE_KV01);
        }
        else if ((len == 1) && !memcmp(handle_44_kv02_buf, data, 1))
        {
            // DEBUG_LOG_STRING("kv 02\r\n");
            set_kvcmd_state(E_KV_CMD_VALUE_KV02);
        }
        else if ((len == 1) && !memcmp(handle_44_kv13_buf, data, 1))
        {
            // DEBUG_LOG_STRING("kv 13\r\n");
            tv_type_state |= E_APP_TV_TPYE_13;
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_kv13_rsp_buf[0], sizeof(handle_44_kv13_rsp_buf));
        }
        else if ((len == 1) && !memcmp(handle_44_kv17_buf, data, 1))
        {
            // DEBUG_LOG_STRING("kv 17\r\n");
            tv_type_state |= E_APP_TV_TPYE_17;
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&unknow_1byte_17_rsp[0], sizeof(unknow_1byte_17_rsp));
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&unknow_1byte_17_2_rsp[0], sizeof(unknow_1byte_17_2_rsp));
        }
        else if ((len == 1) && !memcmp(handle_44_kv19_buf, data, 1))
        {
            // DEBUG_LOG_STRING("kv 19\r\n");
            tv_type_state |= E_APP_TV_TPYE_19;
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_kv19_rsp_buf[0], sizeof(handle_44_kv19_rsp_buf));
        }
        else if ((len == 1) && !memcmp(handle_44_kv90_buf, data, 1))
        {
            // DEBUG_LOG_STRING("kv 19\r\n");
            tv_type_state |= E_APP_TV_TPYE_90;
            // ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_kv19_rsp_buf[0], sizeof(handle_44_kv19_rsp_buf));
        }        
    }
    else if (table->handle == 30)
    {
        if ((len == 1) && !memcmp(handle_44_kv03_buf, data, 1))
        {
            DEBUG_LOG_STRING("kv 03\r\n");
            // set_kvcmd_state(E_KV_CMD_VALUE_KV03);
            set_cur_pkt_type(E_APP_PKT_TYPE_START_ADUIO);
            set_audio_frame_seq(get_frame_seq());
            axis_key_enable = 0;
            start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x0a); /// start voice
            app_ka_cur_state = E_APP_CUR_KA_STATE_INIT;
            update_frm_head();
            ATT_sendNotify(MOUSE_KEY_ENCODE_ATTLIST_HANDLE, usr_axis_key_buf, sizeof(usr_axis_key_buf));
            //mouse_switch();
            mouse_off();
            if(!led_state){
                led_state = true;
                led_on(LED_1,0,0);
            }
            mic_open();
        }
        else if ((len == 1) && !memcmp(handle_44_kv04_buf, data, 1))
        {
            DEBUG_LOG_STRING("kv 04\r\n");
            mic_close();
           // mouse_switch();
            mouse_on();
            axis_key_enable = 1;
            kv04_cmd_rpt_pkt_en = 1;
        }
        else if ((len == 1) && !memcmp(handle_44_kv11_buf, data, 1))
        {
            
            tv_type_state |= E_APP_TV_TPYE_11;
            DEBUG_LOG_STRING("kv 11 %d\r\n",tv_type_state);
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_kv11_rsp_buf[0], sizeof(handle_44_kv11_rsp_buf));
        }
        else if ((len == 1) && !memcmp(handle_44_kv17_buf, data, 1))
        {
            tv_type_state |= E_APP_TV_TPYE_17;
            DEBUG_LOG_STRING("kv 17 %d\r\n",tv_type_state);
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&unknow_1byte_17_rsp[0], sizeof(unknow_1byte_17_rsp));
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&unknow_1byte_17_2_rsp[0], sizeof(unknow_1byte_17_2_rsp));
        }
    }
    else if (table->handle == OTA_WRITE_HANDLE)
    {
        if (app_sleep_check())
        {
            software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        }
        app_sleep_lock_set(APP_LOCK, true);
        app_sleep_lock_set(ADV_LOCK, false);
        OS_ENTER_CRITICAL();
        if (OTA_EVT_HANDLE(len, data) != OTA_NO_ERROR)
        {
            OTA_RESET();
            app_sleep_lock_set(APP_LOCK, false);
        }
        OS_EXIT_CRITICAL();
    }
    else if (table->dataLen >= len)
    {
        memcpy((void *)table->dataPtr, (void *)data, len);
    }
    else
    {
        DEBUG_LOG_STRING("ATT WRITE DATA ERROR, HANDLE: %d, WRITE LEN: %d \r\n", table->handle, len);
    }
}

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if (HREADW(mem_le_slave_latency) == 99)
    {
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
        mouse_lpm_int_mode();
    }
}

void CONN_PAPA_UPDATE_RSP(uint8_t reason){
    DEBUG_LOG_STRING("CONN_PAPA_UPDATE_RSP NO %d\r\n",reason);
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
    if (Bt_CheckIsPaired() && !mouse_on_off && !paired_frst)
    {
        DEBUG_LOG_STRING("ENCRYPT start axis... \r\n");
        mouse_init(mouse_handle);
    }
    update_conn_param(false);

}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
    led_state = true;
    led_on(LED_1, 200, 1000);

}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);
    paired_frst = 0;

    Bt_HciFifo_Init();
    app_sleep_lock_set(LATENCY_LOCK, false);
    bt_set_le_state(BLE_IDLE);

    System_ChangeXtal24M();
    app_queue_reset();
    remote_control_reinit();
    if (!flash_record_exist(set_home_state)) {
        SYS_RESET();
        DEBUG_LOG_STRING("set_home_state 4\r\n");
    }
    enter_deep_sleep();
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("connected! \r\n");

    bt_set_lpm_overhead_wake_time(0x0b); // 0x08 ~ 0x10
    bt_set_le_state(BLE_CONNECTED);

    bt_disable_le_tx_md();
    if(bt_check_save_connect_info()){
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    }

}

void Dev_WakeUp(void)
{
    DEBUG_LOG_STRING("Dev WakeUp \r\n");

    IPC_TxControlCmd(BT_CMD_FIRST_POWER_ON);

    Flash_LoadFlashRecord();
    Bt_ReadAddrFromFlash();

    if (!Lpm_GetWakeFlag())
    {
        bt_renew_scan_rsp((void *)scan_rsp_data_buf, sizeof(scan_rsp_data_buf));
    }
    OS_ENTER_CRITICAL(); /// add 20230522
    if(!Bt_CheckIsPaired()) {
        bt_send_security_request();
    }
    OS_EXIT_CRITICAL();

    if (Bt_CheckIsPaired())
    {
        start_adv(ADV_TYPE_DIRECT, 0x08);
    }
    else
    {
        start_adv(ADV_TYPE_NOMAL, 0x10);
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
    OS_ENTER_CRITICAL(); /// add 20230522
    if(!Bt_CheckIsPaired()) {
        bt_send_security_request();
    }
    OS_EXIT_CRITICAL();


    if (Bt_CheckIsPaired())
    {
        start_adv(ADV_TYPE_DIRECT, 0x08);
    }
    else
    {
        start_adv(ADV_TYPE_NOMAL, 0x10);
    }
    led_on(LED_1, 200, 1000);
}

void app_task_run(void)
{
    if (tx_power_switch)
    {
        bt_dynamic_switch_power();
    }
    app_queue_task();
}

void app_init(void)
{
    if (!Lpm_GetWakeFlag())
    {
        bt_set_le_mtu_size(251);
        bt_set_auth_for_security_req(AUTH_BOND); /// BONING

        bt_update_dle();
        bt_update_mtu();
        bt_update_slave_feature();

        bt_set_ce_length_num(0x08);

        software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(UPD6121G2_68, CUSTOM_04_FB_A);

        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        encode_timer_init();
        app_rpt_timeout_timernum = swtimer_add(app_rpt_timeout_handle);
        brand_pressed_timernum = swtimer_add(key_pressed_handle);
        encode_timernum = swtimer_add(encode_handle);
        swtimer_start(encode_timernum, 10, TIMER_START_REPEAT);
        if (!flash_record_exist(set_home_state)) {
            flash_Erase(set_home_state, sizeof(addr_inf_t));
            ir_single_send(APP_KEYVALUE_IR_PAIRED, 2);
            DEBUG_LOG_STRING("set_home_state2\r\n");
        }
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
            remote_control_reinit();
            mouse_switch();

            DEBUG_LOG_STRING("WAKE UP \r\n");
        }
        else
        {
            enter_low_sleep();
        }
    }
}
