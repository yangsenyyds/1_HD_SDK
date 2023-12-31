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
#include "ir_learn_from_remote.h"
#include "yc11xx_bt_interface.h"
#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
#define mouse_angle_range (127.0f)
#define mouse_move_range (35.0f)
#define mouse_ratio (mouse_angle_range / mouse_move_range)

enum
{
    NORMAL = 0,
    IR_SEND = 1,
    IR_RECEIVE_JUDGE_STATE = 2,
    IR_RECEIVE_STATE = 3,
   
    LED_STATE_NORMAL = 0,
    LED_STATE_NO_OFF = 1,
    LED_STATE_SLEEP = 2,
    LED_LEARN_LONG = 3,
    LED_LEARN_CONTINU = 4,


    KEY_PWR = 1,
    KEY_MUTE = 16,
    KEY_MUTE_ROW = 3,

    KEY_HOME = 21,
    KEY_BACK = 26,
    KEY_BACK_ROW = 1,
    KEY_SOURCE = 30,
    KEY_OK = 39,

    KEY_DEVICE = 2,

    KEY_1_NUM = 3,
    KEY_2_NUM = 4,
    KEY_3_NUM = 5,
    KEY_4_NUM = 6,
    KEY_5_NUM = 7,
    KEY_6_NUM = 8,
    KEY_7_NUM = 9,
    KEY_8_NUM = 10,
    KEY_9_NUM = 11,
    KEY_0_NUM = 13,
    KEY_LIST = 12,
    KEY_DIAN_NUM = 14,

    KEY_VOL = 15,
    KEY_VOL_ = 18,
    KEY_CH = 17,
    KEY_CH_ = 20,
    KEY_UP = 22,
    KEY_LEFT = 24,
    KEY_RIGHT = 25,
    KEY_DOWN = 27,
    KEY_SET = 23,
    KEY_PLAY = 37,
    KEY_PAUSE = 38,
    KEY_RED = 32,
    KEY_GREEN = 33,
    KEY_YELLOW = 34,
    KEY_BLUE = 35,
};

typedef struct
{
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

typedef struct
{
    uint8_t MouseNTFBuf[MOUSE_SEND_LEN];
} MouseNTFQue_TypeDef;
#if (LG_Project == 157)//lg20 5501
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x99, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x4C, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 SETTING
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 NETFLIX
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 PRIME VIDEO
    // {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 GOOGLE
    // {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 AMAZON
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0x81, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 MOVIES
    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 PAUSE
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83


};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0xDE}, // 2 STB
    {0x11}, // 3 '1'
    {0x12}, // 4 '2'
    {0x13}, // 5 '3'

    {0x14}, // 6 '4'
    {0x15}, // 7 '5'
    {0x16}, // 8 '6'
    {0x17}, // 9 '7'
    {0x18}, // 10 '8'

    {0x19}, // 11 '9'
    {0x4C}, // 12 LIST
    {0x10}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x7C}, // 21 HOME
    {0x40}, // 22 UP
    {0x43}, // 23 SETTING
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0x28}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0x56}, // 29 NETFLIX
    {0x0B}, // 30 INPUT

    {0x5C}, // 31 PRIME VIDEO
    {0x72}, // 32 RED
    {0x71}, // 33 GREEN
    {0x63}, // 34 YELLOW
    {0x61}, // 35 BLUE
    {0x81}, // 36 MOVIES
    {0xB0}, // 37 PLAY
    {0xBA}, // 38 PAUSE
    {0x44}, // 39 OK -->PAIR 83
};
#elif (LG_Project == 158)//lg20 5502
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x99, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x53, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 SETTING
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0x5D, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 jvj
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x98, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 STB 
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0x81, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 MOVIES
    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 PAUSE
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83


};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0xDE}, // 2 STB
    {0x11}, // 3 '1'
    {0x12}, // 4 '2'
    {0x13}, // 5 '3'

    {0x14}, // 6 '4'
    {0x15}, // 7 '5'
    {0x16}, // 8 '6'
    {0x17}, // 9 '7'
    {0x18}, // 10 '8'

    {0x19}, // 11 '9'
    {0x53}, // 12 LIST
    {0x10}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x7C}, // 21 HOME
    {0x40}, // 22 UP
    {0x43}, // 23 SETTING
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0x28}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0x5D}, // 29 JVJ
    {0x0B}, // 30 INPUT

    {0xDE}, // 31 STB
    {0x72}, // 32 RED
    {0x71}, // 33 GREEN
    {0x63}, // 34 YELLOW
    {0x61}, // 35 BLUE
    {0x81}, // 36 MOVIES
    {0xB0}, // 37 PLAY
    {0xBA}, // 38 PAUSE
    {0x44}, // 39 OK -->PAIR 83
};


#elif (LG_Project == 159)//lg20 5503
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x99, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x4C, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 SETTING
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0xB5, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 sleep
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x98, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 STB 
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0x81, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 MOVIES
    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 PAUSE
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83


};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0xDE}, // 2 STB
    {0x11}, // 3 '1'
    {0x12}, // 4 '2'
    {0x13}, // 5 '3'

    {0x14}, // 6 '4'
    {0x15}, // 7 '5'
    {0x16}, // 8 '6'
    {0x17}, // 9 '7'
    {0x18}, // 10 '8'

    {0x19}, // 11 '9'
    {0x4C}, // 12 LIST
    {0x10}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x7C}, // 21 HOME
    {0x40}, // 22 UP
    {0x43}, // 23 SETTING
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0x28}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0xB5}, // 29 SLEEP
    {0x0B}, // 30 INPUT

    {0xDE}, // 31 STB
    {0x72}, // 32 RED
    {0x71}, // 33 GREEN
    {0x63}, // 34 YELLOW
    {0x61}, // 35 BLUE
    {0x81}, // 36 MOVIES
    {0xB0}, // 37 PLAY
    {0xBA}, // 38 PAUSE
    {0x44}, // 39 OK -->PAIR 83
};
#elif (LG_Project == 160)//18ba
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x99, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x53, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 -LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 SETTING
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 netflix
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 amazon 
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 36PAUSE
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, // 37ZANTING
    {0xAF, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 LME ZOOM
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83


};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0xDE}, // 2 STB
    {0x11}, // 3 '1'
    {0x12}, // 4 '2'
    {0x13}, // 5 '3'

    {0x14}, // 6 '4'
    {0x15}, // 7 '5'
    {0x16}, // 8 '6'
    {0x17}, // 9 '7'
    {0x18}, // 10 '8'

    {0x19}, // 11 '9'
    {0x53}, // 12 LIST
    {0x10}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x7C}, // 21 HOME
    {0x40}, // 22 UP
    {0x43}, // 23 SETTING
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0x28}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0x56}, // 29 NETLIX
    {0x0B}, // 30 INPUT

    {0x5C}, // 31 AMAZON
    {0x72}, // 32 RED
    {0x71}, // 33 GREEN
    {0x63}, // 34 YELLOW
    {0x61}, // 35 BLUE
    {0xB0}, // 36 PAUSE
    {0xBA}, // 37 ZANTING
    {0xDE}, // 38 LE ZOON
    {0x44}, // 39 OK -->PAIR 83
};
#elif (LG_Project == 161)//650
static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x99, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x53, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 -LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 SETTING
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 netflix
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 amazon 
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0x20, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 text
    {0x21, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 t opt
    {0xAF, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 LME ZOOM
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83


};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0xDE}, // 2 STB
    {0x11}, // 3 '1'
    {0x12}, // 4 '2'
    {0x13}, // 5 '3'

    {0x14}, // 6 '4'
    {0x15}, // 7 '5'
    {0x16}, // 8 '6'
    {0x17}, // 9 '7'
    {0x18}, // 10 '8'

    {0x19}, // 11 '9'
    {0x53}, // 12 LIST
    {0x10}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x7C}, // 21 HOME
    {0x40}, // 22 UP
    {0x43}, // 23 SETTING
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0x28}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0x56}, // 29 NETLIX
    {0x0B}, // 30 INPUT

    {0x5C}, // 31 AMAZON
    {0x72}, // 32 RED
    {0x71}, // 33 GREEN
    {0x63}, // 34 YELLOW
    {0x61}, // 35 BLUE
    {0x20}, // 36 text
    {0x21}, // 37 t.opt
    {0xDE}, // 38 LE ZOON
    {0x44}, // 39 OK -->PAIR 83
};
#elif (LG_Project == 163)//650

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x99, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x53, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 节目列表
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x98, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 机顶盒菜单
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0xB5, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 最近列表
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 SET
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 PAUSE
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0xAF, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 放大镜
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83


};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0xDE}, // 2 STB
    {0x11}, // 3 '1'
    {0x12}, // 4 '2'
    {0x13}, // 5 '3'

    {0x14}, // 6 '4'
    {0x15}, // 7 '5'
    {0x16}, // 8 '6'
    {0x17}, // 9 '7'
    {0x18}, // 10 '8'

    {0x19}, // 11 '9'
    {0x53}, // 12 节目列表
    {0x10}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x7C}, // 21 HOME
    {0x40}, // 22 UP
    {0xDE}, // 23 机顶盒菜单
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0x28}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0xB5}, // 29 最近列表
    {0x0B}, // 30 INPUT

    {0x43}, // 31 SET
    {0x72}, // 32 RED
    {0x71}, // 33 GREEN
    {0x63}, // 34 YELLOW
    {0x61}, // 35 BLUE
    {0xB0}, // 36 PLAY
    {0xBA}, // 37 PAUSE
    {0xDE}, // 38 放大镜
    {0x44}, // 39 OK -->PAIR 83
};
#elif (LG_Project == 952 )//LG-21 1821

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 STB
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '3'

    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '4'
    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '8'

    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 '9'
    {0x1A, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 节目列表
    {0x1B, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '0'
    {0x1C, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 VOL+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 CH+
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL-
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 CH-

    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 UP
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 机顶盒菜单
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 RIGHT

    {0x9f, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 BACK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 DOWN
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 GUIDE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 最近列表
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 INPUT

    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 SET
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 RED
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 GREEN
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 YELLOW
    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BLUE
    {0x95, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 PAUSE
    {0x94, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0xEF, 0x00, 1, DAXIS_KEY_HANDLE}, // 38 放大镜
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 OK -->PAIR 83
};

static const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x0B}, // 2 STB
    {0xDE}, // 3 '1'
    {0xDE}, // 4 '2'
    {0xDE}, // 5 '3'

    {0xDE}, // 6 '4'
    {0xDE}, // 7 '5'
    {0xDE}, // 8 '6'
    {0xDE}, // 9 '7'
    {0xDE}, // 10 '8'

    {0xDE}, // 11 '9'
    {0xDE}, // 12 节目列表
    {0xDE}, // 13 '0'
    {0xDE}, // 14 '...'
    {0x02}, // 15 VOL+

    {0x09}, // 16 MUTE
    {0x00}, // 17 CH+
    {0x03}, // 18 VOL-
    {0xDE}, // 19 VOICE
    {0x01}, // 20 CH-

    {0x28}, // 21 HOME
    {0x40}, // 22 UP
    {0x43}, // 23 机顶盒菜单
    {0x07}, // 24 LEFT
    {0x06}, // 25 RIGHT

    {0xDE}, // 26 BACK
    {0x41}, // 27 DOWN
    {0xAB}, // 28 GUIDE
    {0x56}, // 29 最近列表
    {0x7C}, // 30 INPUT

    {0x5c}, // 31 SET
    {0x61}, // 32 RED
    {0x72}, // 33 GREEN
    {0x71}, // 34 YELLOW
    {0x63}, // 35 BLUE
    {0xDE}, // 36 PLAY
    {0xDE}, // 37 PAUSE
    {0xDE}, // 38 放大镜
    {0x44}, // 39 OK -->PAIR 83
};

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
//lg 22
#if LG22
static const uint8_t adv_data_buf[] = {0x02, 0x01, 0x05, 0x17, 0xFF, 0xC4, 0x00, 0x53, 0x43, 0x44, 0x20, 0x31, 0x39, 0x2E, 0x31, 0x2C, 0x42, 0x41, 0x20, 0x33, 0x35, 0x2C, 0x77, 0x65, 0x62, 0x4F, 0x53};
static const uint8_t scan_rsp_data_buf[] = {0x09, 0x09, 0x4C, 0x47, 0x45, 0x20, 0x4D, 0x52, 0x32, 0x32};
//lg 21
#elif LG21
static uint8_t scan_rsp_data_buf[] = {0x09, 0x09, 0x4c, 0x47, 0x45, 0x20, 0x4d, 0x52, 0x32, 0x31};
static const uint8_t adv_data_buf[] = {
0x02 ,0x01 ,0x05 ,0x17 ,0xFF ,0xC4 ,0x00 ,0x53 ,0x43 ,0x44 ,0x20 ,0x31 ,0x39 
,0x2E ,0x31 ,0x2C ,0x42 ,0x41 ,0x20 ,0x33 ,0x35 ,0x2C ,0x77 ,0x65 ,0x62 ,0x4F 
,0x53};
//lg 20
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
#endif
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static bool mouse_on_off;

static uint16_t key_pressed_time;
static uint8_t key_pressed_num;
static uint8_t keynum_secnd;
static uint8_t keynum;

static uint8_t app_rpt_timeout_timernum = 0xFF;
static uint8_t brand_pressed_timernum = 0xFF;
static uint8_t ir_receive_timernum = 0xFF;
static uint8_t adv_timernum = 0xFF;
static uint8_t encode_timernum = 0xFF;
static bool tx_power_switch = false;
static uint8_t paired_frst = 0;
static uint8_t led_state;
static uint8_t continu_ir_receive;
static uint8_t send_state;
static uint8_t usr_axis_key_buf[19];
static uint8_t auto_cnt;
static uint8_t app_rpt_timeout_type = 0;
static bool ir_receive_success_state;

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
                else if (app_ka_cur_state == E_APP_CUR_KA_STATE_KEY_AXIS) /// key+axis->key
                {
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, 0x3f);
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY; /// key
                }
                else /// key
                {
                    app_ka_cur_state = E_APP_CUR_KA_STATE_KEY; /// key
                    start_axis_rpt_ptk(DAXIS_H1, DKEYVALUE_FLAG, kv);
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
    if (!all_state_switch(acc_buf, gyro_buf))
    {
        if(Bt_HciGetUsedBufferNum() <= 12) {
            ATT_sendNotify(MOUSE_KEY_ENCODE_ATTLIST_HANDLE, usr_axis_key_buf, sizeof(usr_axis_key_buf));
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
            led_state = LED_STATE_SLEEP;            
            led_on(LED_1,200, 600);
            uint16_t len = sizeof(send_state);
            flash_read(HD_SEND_STATE, (uint8_t*)&send_state, len);
        }
        else if(JUDE_receive == IR_LEARN_SUCE)
        {
            key_pressed_time = 0;
            continu_ir_receive = true;
            send_state = IR_RECEIVE_JUDGE_STATE;
            led_state = LED_LEARN_LONG;
            led_on(LED_1, 200, 600);
            DEBUG_LOG_STRING("200  \r\n");

        }
        else if(JUDE_receive == IR_LEARN_FAIL){
            led_state = LED_LEARN_LONG;
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

static void adv_handle(void)
{
    if (bt_check_le_connected())
        return;
    else
    {
        stop_adv();
        start_adv(ADV_TYPE_NOMAL, 0x10);
    }
}

static void key_pressed_handle(void)
{
    if (key_pressed_num == 2 && keynum == KEY_BACK && keynum_secnd == KEY_HOME)
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
            led_state = LED_STATE_NO_OFF;
            led_on(LED_1, 200, 1000);
        }
        else
        {
            swtimer_restart(brand_pressed_timernum);
        }
    }
    else if(key_pressed_num == 2 && keynum == KEY_BACK && keynum_secnd == KEY_MUTE)
    {
        if (++key_pressed_time > 4) /// 3S
        {
            keynum_secnd = 0;
            key_pressed_time = 0;
            if(send_state == NORMAL)
            {
                send_state = IR_SEND;
                if(bt_check_le_connected()){
                    bt_send_le_disconnect(0x13);
                }
                else {
                    stop_adv();
                }
            }
            else if(send_state == IR_SEND)
            {
                send_state = NORMAL;
            }
            flash_write(HD_SEND_STATE, (uint8_t*)&send_state, sizeof(send_state), STATE_INF);
            led_state = LED_STATE_SLEEP;
            led_on(LED_1, 200, 1000);
        }
        else
        {
            swtimer_restart(brand_pressed_timernum);
        }
    }
    else if((key_pressed_num == 1 || key_pressed_num == 0))
    {
        if(keynum == KEY_SOURCE)
        {
            if(key_pressed_time >= 50)
            {
                if(key_pressed_num == 0){
                    key_pressed_time = 0;
#ifdef FUNCTION_WATCH_DOG
                    IWDG_Disable(WDT);   //bt watch dog     
                    IWDG_Disable(WDT2);  //riscv watch dog
#endif 
                    swtimer_start(ir_receive_timernum, 100, TIMER_START_REPEAT);
                    return ;
                }
                else{
                    swtimer_restart(brand_pressed_timernum);
                    return;
                }
            }

            if(key_pressed_num == 0){
                key_pressed_time = 0;
                if(!bt_check_le_connected()) {
                    ir_single_send(IRKeyBuf[keynum][0],1);
                }else{
                    if(!key_chk_ok) {
                    key_chk_ok = 1;
                    moving_frst = 0;
                    app_sleep_lock_set(MOUSE_LOCK, true);//待确定
                    }
                }
            }else{
                swtimer_restart(brand_pressed_timernum);
                key_pressed_time++;
                if(key_pressed_time == 50){

                    send_state = IR_RECEIVE_STATE;
                    led_state = LED_LEARN_LONG;
                    led_on(LED_1,200,1000);
                }
            }
        }
        else {
            if(send_state == IR_SEND)
            {
                if(keynum == KEY_DEVICE && !flash_read(HD_DEVICE_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }

                else if(keynum == KEY_1_NUM && !flash_read(HD_1_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_2_NUM && !flash_read(HD_2_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_3_NUM && !flash_read(HD_3_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                
                }
                else if(keynum == KEY_4_NUM && !flash_read(HD_4_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_5_NUM && !flash_read(HD_5_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_6_NUM && !flash_read(HD_6_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_7_NUM && !flash_read(HD_7_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_8_NUM && !flash_read(HD_8_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_9_NUM && !flash_read(HD_9_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_0_NUM && !flash_read(HD_0_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_LIST && !flash_read(HD_list_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_DIAN_NUM && !flash_read(HD_DIAN_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                                                                                                                                                        
                else if(keynum == KEY_VOL && !flash_read(HD_VOL_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }              
                }
                else if(keynum == KEY_VOL_ && !flash_read(HD_VOL__DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_CH && !flash_read(HD_CH_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_CH_ && !flash_read(HD_CH__DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_UP && !flash_read(HD_UP_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_LEFT && !flash_read(HD_LEFT_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_RIGHT && !flash_read(HD_RIGHT_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_OK && !flash_read(HD_OK_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_DOWN && !flash_read(HD_DOWN_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_SET && !flash_read(HD_SET_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_PLAY && !flash_read(HD_PLAY_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }
                }
                else if(keynum == KEY_PAUSE && !flash_read(HD_PAUSE_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }        
                else if(keynum == KEY_RED && !flash_read(HD_RED_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_GREEN && !flash_read(HD_GREEN_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_YELLOW && !flash_read(HD_YELLOW_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_BLUE && !flash_read(HD_BLUE_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }                             
                else if(keynum == KEY_BACK && !flash_read(HD_BACK_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                else if(keynum == KEY_MUTE && !flash_read(HD_MUTE_DATA,(uint8_t *)&irparams,sizeof(irparams))) {
                    ir_remote_learn_send(irparams);
                    // if(!led_state){
                    //     DEBUG_LOG_STRING("KEY_MUTE\r\n");
                    //     led_on(LED_1, 100, 0);
                    // }                    
                }
                if(key_pressed_num == 1) {
                    set_key_press_state(true);
                }
                
                else {
                    key_pressed_time = 0;
                }
            }
        }
    }

}

static void keyvalue_handle(key_report_t *key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    DEBUG_LOG_STRING("send_state %d\r\n", send_state);
    if (key_pressed_num == 0)
    {
        if(send_state == NORMAL)
        {
            if(!led_state) {
                DEBUG_LOG_STRING("led off\r\n");
                led_off(LED_1);
            }
            set_key_press_state(false);
            // DEBUG_LOG_STRING("release keynum[%d],val:%02x\r\n", keynum, KeyBuf[keynum].keyvalue[0]);
            if (!get_voice_key_rel_state())
            {
                app_pkt_type = E_APP_PKT_TYPE_AXIS_DATA;
                set_frame_seq(get_audio_frame_seq() - 1);
                set_frame_seq_val();
                start_axis_rpt_ptk(DAXIS_H1, 0x00, 0x00);
            }
        }else if(send_state == IR_SEND) {
            if(!led_state) {
                DEBUG_LOG_STRING("led off\r\n");
                led_off(LED_1);
            }
            set_key_press_state(false);
        }
    }
    else if (key_pressed_num == 1)
    {
        keynum = key_report->keynum_report_buf[0] + key_report->keynum_report_buf[1] + key_report->keynum_report_buf[2] + key_report->keynum_report_buf[3] + key_report->keynum_report_buf[4] + key_report->keynum_report_buf[5];

        DEBUG_LOG_STRING("keynum[%d]\r\n", keynum);
        /*pwr ir*/
        if(send_state == NORMAL)
        {
            if (keynum == KEY_PWR) /// pwr
            {
                set_key_press_state(true);
                ir_comm_send(APP_KEYVALUE_IR_OK);
            }
            else if(keynum == KEY_SOURCE){
                swtimer_start(brand_pressed_timernum, 100, TIMER_START_ONCE);
            }
            else if (!bt_check_le_connected()) /*unpaire 83 44paired 44*/
            {
                if (keynum == KEY_OK) /// ok
                {
                    if (Bt_CheckIsPaired()) /// 44
                    {
                        set_key_press_state(true);
                    }
                    else
                    {
                        led_state = LED_STATE_NO_OFF;
                        led_on(LED_1, 0, 15000);
                        start_adv(ADV_TYPE_NOMAL, 0x10);
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
            if(!led_state){
                led_on(LED_1, 0, 0);
            }
        }
        else if(send_state == IR_SEND){
            if(keynum == KEY_SOURCE){
                swtimer_start(brand_pressed_timernum, 100, TIMER_START_ONCE);
                if(!led_state){
                    led_on(LED_1, 0, 0);
                }
            }
            else
            {
                swtimer_start(brand_pressed_timernum, 100, TIMER_START_ONCE);
                if(!led_state){
                    led_on(LED_1, 100, 0);
                }
            }

        }
        else if(send_state == IR_RECEIVE_STATE)
        {
            if(keynum == KEY_SOURCE)
            {
                swtimer_stop(ir_receive_timernum);
                continu_ir_receive = false;
                key_pressed_time = 0;
                DEBUG_LOG_STRING("Set_ir_keynum ok\r\n");
                led_state = LED_STATE_SLEEP;
                led_on(LED_1,200,1000);
                if(ir_receive_success_state){
                    DEBUG_LOG_STRING("1546\r\n");
                    send_state = IR_SEND;
                    flash_write(HD_SEND_STATE, (uint8_t*)&send_state, sizeof(send_state), STATE_INF);
                }else{
                    uint16_t len = sizeof(send_state);
                    flash_read(HD_SEND_STATE, (uint8_t*)&send_state, len);
                }
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
            if(keynum == KEY_DEVICE  
            || keynum == KEY_1_NUM  || keynum == KEY_2_NUM  || keynum == KEY_3_NUM  || keynum == KEY_4_NUM
            || keynum == KEY_5_NUM  || keynum == KEY_6_NUM  || keynum == KEY_7_NUM  || keynum == KEY_8_NUM
            || keynum == KEY_9_NUM  || keynum == KEY_0_NUM  || keynum == KEY_LIST  || keynum == KEY_DIAN_NUM
            || keynum == KEY_VOL || keynum == KEY_VOL_ || keynum == KEY_CH 
            || keynum == KEY_CH_ || keynum == KEY_UP || keynum == KEY_LEFT || keynum == KEY_RIGHT 
            || keynum == KEY_OK || keynum == KEY_DOWN || keynum == KEY_SET || keynum == KEY_PLAY
            || keynum == KEY_PAUSE || keynum == KEY_RED || keynum == KEY_GREEN || keynum == KEY_YELLOW 
            || keynum == KEY_BLUE || keynum == KEY_BACK || keynum == KEY_MUTE)
            {
                switch (keynum)
                {
                case KEY_DEVICE:
                    flash_write(HD_DEVICE_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_1_NUM:
                    flash_write(HD_1_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_2_NUM:
                    flash_write(HD_2_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_3_NUM:
                    flash_write(HD_3_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_4_NUM:
                    flash_write(HD_4_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;                

                case KEY_5_NUM:
                    flash_write(HD_5_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_6_NUM:
                    flash_write(HD_6_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_7_NUM:
                    flash_write(HD_7_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_8_NUM:
                    flash_write(HD_8_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break; 

                case KEY_9_NUM:
                    flash_write(HD_9_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_0_NUM:
                    flash_write(HD_0_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_LIST:
                    flash_write(HD_list_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_DIAN_NUM:
                    flash_write(HD_DIAN_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break; 

                case KEY_VOL:
                    flash_write(HD_VOL_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_VOL_:
                    flash_write(HD_VOL__DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_CH:
                    flash_write(HD_CH_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_CH_:
                    flash_write(HD_CH__DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_UP:
                    flash_write(HD_UP_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_LEFT:
                    flash_write(HD_LEFT_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_RIGHT:
                    flash_write(HD_RIGHT_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_OK:
                    flash_write(HD_OK_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_DOWN:
                    flash_write(HD_DOWN_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_SET:
                    flash_write(HD_SET_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_PLAY:
                    flash_write(HD_PLAY_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_PAUSE:
                    flash_write(HD_PAUSE_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_RED:
                    flash_write(HD_RED_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_GREEN:
                    flash_write(HD_GREEN_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_YELLOW:
                    flash_write(HD_YELLOW_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_BLUE:
                    flash_write(HD_BLUE_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_BACK:
                    flash_write(HD_BACK_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;
                case KEY_MUTE:
                    flash_write(HD_MUTE_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
                break;      
                }
                led_state = LED_LEARN_CONTINU;
                led_on(LED_1, 200, 600);
                DEBUG_LOG_STRING("KEY_DEVICE ok\r\n");
                

                key_pressed_time = 0;
                continu_ir_receive = 2;
                send_state = IR_RECEIVE_STATE;
                ir_receive_success_state = true;
            }
            else if(keynum == KEY_SOURCE)
            {
                swtimer_stop(ir_receive_timernum);
#ifdef FUNCTION_WATCH_DOG                
                IWDG_Enable(WDT);
                IWDG_Enable(WDT2);
#endif                
                continu_ir_receive = false;
                key_pressed_time = 0;
                led_state = LED_STATE_SLEEP;
                led_on(LED_1,200,1000);
                if(ir_receive_success_state){
                    send_state = IR_SEND;
                    flash_write(HD_SEND_STATE, (uint8_t*)&send_state, sizeof(send_state), STATE_INF);
                }else{
                    uint16_t len = sizeof(send_state);
                    flash_read(HD_SEND_STATE, (uint8_t*)&send_state, len);
                }
                if (bt_check_le_connected()) {
                    app_sleep_lock_set(APP_LOCK, false);
                }
            }
        }

    }
    else if (key_pressed_num == 2)
    {
        set_key_press_state(false);
        led_off(LED_NUM);
        DEBUG_LOG_STRING("KEYNUM [%d] [%d] [%d] [%d] [%d] [%d] [%d]\r\n", key_report->keynum_report_buf[0], key_report->keynum_report_buf[1],
                         key_report->keynum_report_buf[2], key_report->keynum_report_buf[3], key_report->keynum_report_buf[4],
                         key_report->keynum_report_buf[5], key_report->keynum_report_buf[6]);
        if (key_report->keynum_report_buf[KEY_BACK_ROW] == KEY_BACK && key_report->keynum_report_buf[2] == KEY_HOME)
        {
            keynum = key_report->keynum_report_buf[KEY_BACK_ROW];
            keynum_secnd = key_report->keynum_report_buf[2];
            key_pressed_time = 0;
            swtimer_start(brand_pressed_timernum, 1000, TIMER_START_ONCE);
        }else if(key_report->keynum_report_buf[KEY_BACK_ROW] == KEY_BACK && key_report->keynum_report_buf[KEY_MUTE_ROW] == KEY_MUTE)
        {
            keynum = key_report->keynum_report_buf[KEY_BACK_ROW];
            keynum_secnd = key_report->keynum_report_buf[KEY_MUTE_ROW];
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
    if(led_state == LED_STATE_NO_OFF)
    {
        led_state = false;
    }
    else if(led_state == LED_STATE_SLEEP){
        DEBUG_LOG_STRING("state sleep \r\n");
        SysTick_DelayUs(100);
        led_state = false;
        app_queue_reset();
        remote_control_reinit();
    
        enter_deep_sleep();
    }
    else if(led_state == LED_LEARN_LONG){
        led_state = LED_STATE_NORMAL;
        led_on(LED_1,0,0);
    }
    else if(led_state == LED_LEARN_CONTINU) {
        led_state = LED_STATE_NORMAL;
        continu_ir_receive =false;
        led_on(LED_1, 0, 0);
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
            DEBUG_LOG_STRING("kv 11\r\n");
            tv_type_state |= E_APP_TV_TPYE_11;
            ATT_sendNotify(KV_CMD_ATTLIST_HANDLE, (void *)&handle_44_kv11_rsp_buf[0], sizeof(handle_44_kv11_rsp_buf));
        }
        else if ((len == 1) && !memcmp(handle_44_kv17_buf, data, 1))
        {
            DEBUG_LOG_STRING("kv 17\r\n");
            tv_type_state |= E_APP_TV_TPYE_17;
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
    // DEBUG_LOG_STRING("CONN_PAPA_UPDATE_RSP NO %d\r\n",reason);
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
    led_state = LED_STATE_NO_OFF;
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
    if(send_state == IR_SEND){
        app_sleep_timer_set(SHUTDOWN_TIME);
        app_sleep_lock_set(ADV_LOCK, true);
    }
    else{
        if (Bt_CheckIsPaired())
        {
            DEBUG_LOG_STRING("123 \r\n");
        start_adv(ADV_TYPE_DIRECT, 0x08);
        }
        else
        {
            start_adv(ADV_TYPE_NOMAL, 0x10);
        }
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
    if(send_state == IR_SEND){
        app_sleep_timer_set(SHUTDOWN_TIME);
        app_sleep_lock_set(ADV_LOCK, true);
    }
    else{
        if (Bt_CheckIsPaired())
        {
            DEBUG_LOG_STRING("123 \r\n");
        start_adv(ADV_TYPE_DIRECT, 0x08);
        }
        else
        {
            start_adv(ADV_TYPE_NOMAL, 0x10);
        }
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
        if (!flash_record_exist(HD_SEND_STATE)) {
            uint16_t len = sizeof(send_state);
            flash_read(HD_SEND_STATE, (uint8_t*)&send_state, len);
        }else{
            send_state = NORMAL;
            flash_write(HD_SEND_STATE, (uint8_t*)&send_state, sizeof(send_state), STATE_INF);
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
        encode_timer_init();
        app_rpt_timeout_timernum = swtimer_add(app_rpt_timeout_handle);
        brand_pressed_timernum = swtimer_add(key_pressed_handle);
        adv_timernum = swtimer_add(adv_handle);
        encode_timernum = swtimer_add(encode_handle);
        ir_receive_timernum = swtimer_add(ir_receive_handle);
        if(send_state == NORMAL) {
            swtimer_start(encode_timernum, 10, TIMER_START_REPEAT);
        }

        DEBUG_LOG_STRING("5./c \r\n");
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
