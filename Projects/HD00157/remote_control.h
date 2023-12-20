#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_gpio.h"
#include "yc11xx_audio_adc.h"
#include "yc_dev_bt.h"
#include "att_list.h"
#define LG22 (2)
#define LG_Project  (952)
/* WATCH_DOG */
#define FUNCTION_WATCH_DOG
// #define ADV_TIME 1
/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE (GPIO_9)

/* SecretKey_Check_enable */
#define SecretKey_Check_enable (1)

/* sleep 1 hour*/
#define SLEEP_ONE_HOUR      (1)

/* SLEEP */
#define ENCRYPT_DONE_DELAY (5000) // ms
#define PAIR_DONE_DELAY (10000)   // ms
#define DIRECT_ADV_TIME (10)       // s
#define SHUTDOWN_TIME (60)        // s

#define UNIT_TIME_1S (1000) // ms

/* SOFTWARE TIMER */
#define TIMER_NUM (15) // KEY 1, LED 1, VBAT 1, VOICE 2, SLEEP 1, MOUSE 2, REMOTE_CONTROL 5, +1
#define TIMER_UNIT_MS (5)

/* POWER */
#define BAT_ATTLIST_INDEX (49)
#define BAT_REPORT_HANDLE (48)
#define BACK_HOME_UNPAIRED_ATTLIST_HANDLE (25)
#define MOUSE_KEY_ENCODE_ATTLIST_HANDLE (34)
#define KV_CMD_ATTLIST_HANDLE (25)

/* KEY */
#define KEY_ROW1_PIN (GPIO_12)
#define KEY_ROW2_PIN (GPIO_13)
#define KEY_ROW3_PIN (GPIO_14)
#define KEY_ROW4_PIN (GPIO_15)
#define KEY_ROW5_PIN (GPIO_19)
#define KEY_ROW6_PIN (GPIO_23)
#define KEY_ROW7_PIN (GPIO_34)
#define KEY_ROW_NUM (7)

#define KEY_COL1_PIN (GPIO_24)
#define KEY_COL2_PIN (GPIO_27)
#define KEY_COL3_PIN (GPIO_28)
#define KEY_COL4_PIN (GPIO_29)
#define KEY_COL5_PIN (GPIO_30)
#define KEY_COL6_PIN (GPIO_31)
#define KEY_COL_NUM (6)

/* LED */
#define LED_1_PIN (GPIO_7)

#define LED_ON (GPIO_Mode_Out_Low)
#define LED_OFF (GPIO_Mode_Out_High)

/*encode*/
#define ENCODE_SA_PIN (GPIO_11)
#define ENCODE_SB_PIN (GPIO_36)
#define ENCODE_PWR_PIN (GPIO_6)

#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN (26)
#define AdcAnaDiffConfig_VOLTAGE_GAIN (20)
#define MSBC (1)
#define SingleEndOut (1)
#define LG (2)

#define MICRECORD_CACHE_NUM (10)
#define ENCODE_CACHE_NUM (10)
#define ENCODE_TIMES_ONCE (1)
#define ENCODE_INPUT_SIZE (240)
#define ENCODE_OUTPUT_SIZE (60)
#define AUDIO_SEND_NTF_SIZE (118)  /// 57+3+57+1
#define AUDIO_FRAME_HEADER_LEN (7) /// handle(1byte)+len(1byte)+frame head(5byte)
#define AUDIO_NTF_BUFSIZE (AUDIO_FRAME_HEADER_LEN + AUDIO_SEND_NTF_SIZE)
#define ENCODE_OUPUT_SECOND_OFFSET (5 - 2)

#define TIMEOUT_TIME (15000)

/* IR */
#define IR_IO (GPIO_4)
#define IR_RCV_PIN (GPIO_5)

#define PWR_KEYNUM (1)

/* IIC */
#define SOFTWARE_IIC (1)
// #define HARDWARE_IIC    (1)
#if SOFTWARE_IIC
#include "software_i2c.h"
#elif HARDWARE_IIC
#include "yc11xx_iic.h"
#define IIC_TYPE (IIC_0)
#endif

#define IIC_SCL_PIN (GPIO_33)
#define IIC_SDA_PIN (GPIO_32)

/* MOUSE */
#define MOUSE
// #define MOUSE_COM_MODE (1)
#define MOUSE_INT_MODE (2)
// #define MOUSE_PWR_CS_HARDWARD_MODE
#define QMI8658_USE_ANYMOTION
#define ORI_TYPE (2)

#ifdef MOUSE
#ifdef MOUSE_PWR_CS_HARDWARD_MODE
#define MOUSE_PWR_PIN ()
#define MOUSE_CS_PIN ()
#endif
#define MOUSE_INT_PIN (GPIO_8)

#define QMI8658_SLAVE_ADDR_L (0x6a)
#define QMI8658_SLAVE_ADDR_H (0x6b)
#define QMI8658_SLAVE_ADDR (QMI8658_SLAVE_ADDR_L << 1)

#define MOUSE_STOP_TIMEOUT (23) // unit: s

#define MOUSE_SNED_HANDLE (52)
#define MOUSE_SEND_LEN (4)

#define MOUSE_KEY (20)
#define MOUSE_OK (24)
#endif

#define LG_AUDIO_KEYVALUE (0x8b)
#define GOOGLE_AUDIO_KEYVALUE (0x05)
#define AMAZON_AUDIO_KEYVALUE (0x0c)
#define APP_KEYVALUE_IR_PAIRED (0x83)
#define APP_KEYVALUE_IR_OK (0x08)

enum
{
    MSBC_FLAG_0 = 0,
    MSBC_FLAG_1,
};

/// 此参数与系统配置的加速度范围有关系 ,默认设置是acc_lsb_div = (1<<12);
/// 如果阀值太小不容易进入校准，如果阀值太大，容易把原来好的数据，校准为错误数据
#define ACC_RESOLUTION_THR_VALUE (80) ///(1 << 12)/2048 /// acc_lsb_div = (1<<12);  STATIC_TH*(ACC_RESOLUTION_THR_VALUE)
#define GYRO_RESOLUTION_THR_VALUE (220)

#define ACC_RESOLUTION_THR_VALUE_MAX (1500) ///(1 << 12)/2048 /// acc_lsb_div = (1<<12);  STATIC_TH*(ACC_RESOLUTION_THR_VALUE)
#define GYRO_RESOLUTION_THR_VALUE_MAX (1000)

#define GYRO_BUF_SIZE (20) /// 20->15
#define app_abs(a) ((a) > 0 ? (a) : -(a))
#define G_CONST_VALUE (9.80665f)
#define AXRATIO (3.5f)
#define AYRATIO (3.5f)
#define AZRATIO (4.5f)

#define GYRO_CONST_VALUE (45.0f)
#define GYXRATIO (0.3f)
#define GYYRATIO (0.3f)
#define GYZRATIO (0.45f)

enum acc_gyro_fram_t
{
    ACC_GYRO_FRM_HEAD, /// 0
    ACC_GYRO_FRM_SEQ,
    ACC_GYRO_FRM_H1,
    ACC_GYRO_FRM_H2,

    ACC_GYRO_FRM_AX_H, /// 4
    ACC_GYRO_FRM_AX_L,
    ACC_GYRO_FRM_AY_H,
    ACC_GYRO_FRM_AY_L,
    ACC_GYRO_FRM_AZ_H,
    ACC_GYRO_FRM_AZ_L,
    ACC_GYRO_FRM_GX_H,
    ACC_GYRO_FRM_GX_L,
    ACC_GYRO_FRM_GY_H,
    ACC_GYRO_FRM_GY_L,
    ACC_GYRO_FRM_GZ_H,
    ACC_GYRO_FRM_GZ_L,

    ACC_GYRO_FRM_K1,
    ACC_GYRO_FRM_KV,
    ACC_GYRO_FRM_END, /// 18 WHEEL
    ACC_GYRO_FRM_MAX,
};

#define DKEYVALUE_FLAG (0x80)
#define DAXIS_H1 (0xfe)

#define H_HEAD_A6_T10_LEN (1)
#define H_HEAD_A6_LEN (H_HEAD_A6_T10_LEN)

#define H_HEAD_A8_T10_LEN (1)
#define H_HEAD_A8_LEN (H_HEAD_A8_T10_LEN)

#define H_HEAD_AA_T10_LEN (7)
#define H_HEAD_AA_LEN (H_HEAD_AA_T10_LEN)

#define H_HEAD_AC_T10_LEN (7)
#define H_HEAD_AC_LEN (H_HEAD_AC_T10_LEN)

#define H_HEAD_AE_T10_LEN (8)
#define H_HEAD_AE_LEN (H_HEAD_AE_T10_LEN)

#define H_HEAD_B0_T10_LEN (10)
#define H_HEAD_B0_LEN (H_HEAD_B0_T10_LEN)

#define H_HEAD_B2_T10_LEN (13)
#define H_HEAD_B2_LEN (H_HEAD_B2_T10_LEN)

#define H_HEAD_B4_T10_LEN (10)
#define H_HEAD_B4_T20_LEN (2)
#define H_HEAD_B4_LEN (H_HEAD_B4_T10_LEN + H_HEAD_B4_T20_LEN)

#define H_HEAD_B6_T10_LEN (16)
#define H_HEAD_B6_T20_LEN (5)
#define H_HEAD_B6_T30_LEN (1)
#define H_HEAD_B6_LEN (H_HEAD_B6_T10_LEN + H_HEAD_B6_T20_LEN + \
                       H_HEAD_B6_T30_LEN)

#define H_HEAD_B8_T10_LEN (32)
#define H_HEAD_B8_T20_LEN (7)
#define H_HEAD_B8_T30_LEN (1)
#define H_HEAD_B8_T40_LEN (1)
#define H_HEAD_B8_T50_LEN (1)
#define H_HEAD_B8_LEN (H_HEAD_B8_T10_LEN + H_HEAD_B8_T20_LEN + \
                       H_HEAD_B8_T30_LEN + H_HEAD_B8_T40_LEN + H_HEAD_B8_T50_LEN)

#define H_HEAD_BA_T10_LEN (29)
#define H_HEAD_BA_T20_LEN (7)
#define H_HEAD_BA_T30_LEN (1)
#define H_HEAD_BA_T40_LEN (2)
#define H_HEAD_BA_LEN (H_HEAD_BA_T10_LEN + H_HEAD_BA_T20_LEN + \
                       H_HEAD_BA_T30_LEN + H_HEAD_BA_T40_LEN)

#define H_HEAD_BC_T10_LEN (21)
#define H_HEAD_BC_T20_LEN (7)
#define H_HEAD_BC_T30_LEN (1)
#define H_HEAD_BC_T40_LEN (1)
#define H_HEAD_BC_LEN (H_HEAD_BC_T10_LEN + H_HEAD_BC_T20_LEN + \
                       H_HEAD_BC_T30_LEN + H_HEAD_BC_T40_LEN)

#define H_HEAD_BE_T10_LEN (29)
#define H_HEAD_BE_T20_LEN (10)
#define H_HEAD_BE_T30_LEN (4)
#define H_HEAD_BE_LEN (H_HEAD_BE_T10_LEN + H_HEAD_BE_T20_LEN + \
                       H_HEAD_BE_T30_LEN)
#define H_HEAD_BE_OFFSET (H_HEAD_BC_OFFSET + H_HEAD_BE_LEN)
#define H_HEAD_BE_ADDR (H_HEAD_BC_OFFSET)

#define H_HEAD_C0_T10_LEN (24)
#define H_HEAD_C0_T20_LEN (7)
#define H_HEAD_C0_T30_LEN (2)
#define H_HEAD_C0_T40_LEN (2)
#define H_HEAD_C0_LEN (H_HEAD_C0_T10_LEN + H_HEAD_C0_T20_LEN + \
                       H_HEAD_C0_T30_LEN + H_HEAD_C0_T40_LEN)

#define H_HEAD_C2_T10_LEN (10)
#define H_HEAD_C2_T20_LEN (3)
#define H_HEAD_C2_T50_LEN (1)
#define H_HEAD_C2_LEN (H_HEAD_C2_T10_LEN + H_HEAD_C2_T20_LEN + \
                       H_HEAD_C2_T50_LEN)

#define H_HEAD_C4_T10_LEN (8)
#define H_HEAD_C4_T20_LEN (1)
#define H_HEAD_C4_LEN (H_HEAD_C4_T10_LEN + H_HEAD_C4_T20_LEN)

#define H_HEAD_C6_T10_LEN (4)
#define H_HEAD_C6_T20_LEN (1)
#define H_HEAD_C6_LEN (H_HEAD_C6_T10_LEN + H_HEAD_C6_T20_LEN)

#define H_HEAD_C8_T10_LEN (3)
#define H_HEAD_C8_LEN (H_HEAD_C8_T10_LEN)

////总计：301条（1+1+7+7+8+10+13+12+22+42+39+30+43+35+14+9+5+3）
#define HHEAD_TOTAL_LEN (H_HEAD_A6_LEN + H_HEAD_A8_LEN + H_HEAD_AA_LEN + H_HEAD_AC_LEN + H_HEAD_AE_LEN + \
                         H_HEAD_B0_LEN + H_HEAD_B2_LEN + H_HEAD_B4_LEN + H_HEAD_B6_LEN + H_HEAD_B8_LEN + \
                         H_HEAD_BA_LEN + H_HEAD_BC_LEN + H_HEAD_BE_LEN + H_HEAD_C0_LEN + H_HEAD_C2_LEN + \
                         H_HEAD_C4_LEN + H_HEAD_C6_LEN + H_HEAD_C8_LEN)

enum
{
    H_A6 = 0,
    H_A8,
    H_AA,
    H_AC,
    H_AE,
    H_B0,
    H_B2,
    H_B4,
    H_B6,
    H_B8,
    H_BA,
    H_BC,
    H_BE,
    H_C0,
    H_C2,
    H_C4,
    H_C6,
    H_C8,
    H_HEAD_MAX,
};

enum kv_cmd_value_t
{
    E_KV_CMD_VALUE_INIT = 0,
    E_KV_CMD_VALUE_KV01 = 0x01,
    E_KV_CMD_VALUE_KV02 = 0x02,
    E_KV_CMD_VALUE_KV03 = 0x04,
    E_KV_CMD_VALUE_KV04 = 0x08,
    E_KV_CMD_VALUE_KV05 = 0x10,
};

enum app_pkt_type_t
{
    E_APP_PKT_TYPE_INIT = 0,
    E_APP_PKT_TYPE_START_ADUIO,
    E_APP_PKT_TYPE_AXIS_DATA,
    E_APP_PKT_TYPE_AUDIO_RELEASE,
};

enum app_key_axis_state_t
{
    E_APP_CUR_KA_STATE_INIT = 0,
    E_APP_CUR_KA_STATE_KEY,
    E_APP_CUR_KA_STATE_AXIS,
    E_APP_CUR_KA_STATE_KEY_AXIS,
};

enum app_rpt_timeout_event_t
{
    E_APP_RPT_TM_EVT_MOUSE_START = 0x01,
    E_APP_RPT_TM_EVT_BACK_HOME_UNPAIRED = 0x02,
};

enum app_tv_kvcmd_t
{
    E_APP_TV_TPYE_11 = 0x01,
    E_APP_TV_TPYE_13 = 0x02,
    E_APP_TV_TPYE_17 = 0x04,
    E_APP_TV_TPYE_19 = 0x08,
};

#define KVCMD_LG650A (E_APP_TV_TPYE_11 | E_APP_TV_TPYE_17)
#define KVCMD_LG18BA_19BA (E_APP_TV_TPYE_11)
#define KVCMD_LG20GA (E_APP_TV_TPYE_13 | E_APP_TV_TPYE_19)
#define KVCMD_LG21GA (E_APP_TV_TPYE_13 | E_APP_TV_TPYE_17 | E_APP_TV_TPYE_19)
#define KVCMD_LG22GA (E_APP_TV_TPYE_13 | E_APP_TV_TPYE_17 | E_APP_TV_TPYE_19)

#define DAXIS_KEY_HANDLE (34)
#define DVOICE_HANDLE (39)

////start axis timer
#define DAXIS_START_TIMER_MAX (500)
#define DAXIS_START_TIMER_MAX_1 (50)

/* FLASH */
typedef enum
{
    SecretKey_Addr = 0x3BF00,
    addr_base = 0x3C000,
    HD_SEND_STATE = addr_base,
    HD_DEVICE_DATA = 0x3FE00,
    HD_VOL_DATA = 0x3FC00,
    HD_VOL__DATA = 0x3FA00,
    HD_CH_DATA = 0x3F800,
    HD_CH__DATA = 0x3F600,
    HD_UP_DATA = 0x3F400,
    HD_LEFT_DATA = 0x3F200,
    HD_RIGHT_DATA = 0x3F000,
    HD_OK_DATA = 0x3EE00,
    HD_DOWN_DATA = 0x3EC00,
    HD_SET_DATA = 0x3EA00,
    HD_PLAY_DATA = 0x3E800,
    HD_PAUSE_DATA = 0x3E600,
    HD_RED_DATA = 0x3E400,
    HD_GREEN_DATA = 0x3E200,
    HD_YELLOW_DATA = 0x3E000,
    HD_BLUE_DATA = 0x3DE00,
    HD_BACK_DATA = 0x3DC00,
    HD_MUTE_DATA = 0x3DA00,

    HD_1_DATA = 0x3D800,
    HD_2_DATA = 0x3D600,
    HD_3_DATA = 0x3D400,
    HD_4_DATA = 0x3D200,
    HD_5_DATA = 0x3D000,
    HD_6_DATA = 0x3CE00,
    HD_7_DATA = 0x3CC00,
    HD_8_DATA = 0x3CA00,
    HD_9_DATA = 0x3C800,
    HD_0_DATA = 0x3C600,
    HD_list_DATA = 0x3C400,
    HD_DIAN_DATA = 0x3C200,

} FlashRecordAddr_TypeDef; // 存储数据需要 8字节
extern void LE_LTK_LOST(void);
extern void action_after_mic_close(void);
extern void action_after_led_blk(void);
extern void mouse_switch(void);
extern void update_conn_param(bool is_sleep);
extern void start_adv(enum advType type, uint16_t adv_interval);
extern void stop_adv(void);
extern void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset);
extern void Read_Parse(const ATT_TABLE_TYPE *table);
extern void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len);
extern void CONN_PARAM_ACCEPTED(void);
extern void ENCRYPT_FAIL(uint8_t reason);
extern void ENCRYPT_DONE(void);
extern void PAIR_FAIL(uint8_t reason);
extern void PAIR_DONE(void);
extern void LE_DISCONNECTED(uint8_t reason);
extern void LE_CONNECTED(void);
extern void Dev_WakeUp(void);
extern void Dev_PowerOn(void);
extern void app_task_run(void);
extern void app_init(void);

#endif
