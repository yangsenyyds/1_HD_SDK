#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_gpio.h"
#include "yc_dev_bt.h"
#include "att_list.h"

/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE   (GPIO_9)

/* SLEEP */
#define ENCRYPT_DONE_DELAY  (5000)      // ms
#define PAIR_DONE_DELAY     (10000)     // ms
#define DIRECT_ADV_TIME     (3)         // s
#define SHUTDOWN_TIME       (60)        // s

#define UNIT_TIME_1S        (1000)      // ms

/* SOFTWARE TIMER */
#define TIMER_NUM       (13)   // KEY 1, LED 1, VBAT 1, VOICE 2, SLEEP 1, REMOTE_CONTROL 5, +1
#define TIMER_UNIT_MS   (10)

/* POWER */
#define BAT_ATTLIST_INDEX   (34)
#define BAT_REPORT_HANDLE   (82)

#define OCN_BAT_ATTLIST_INDEX   (69)
#define OCN_BAT_REPORT_HANDLE   (276)
/* KEY */
#define KEY_ROW1_PIN (GPIO_12)
#define KEY_ROW2_PIN (GPIO_13)
#define KEY_ROW3_PIN (GPIO_14)
#define KEY_ROW4_PIN (GPIO_15)
#define KEY_ROW5_PIN (GPIO_19)
#define KEY_ROW6_PIN (GPIO_23)
// #define KEY_ROW7_PIN (GPIO_34)
#define KEY_ROW_NUM (6)

#define KEY_COL1_PIN (GPIO_24)
#define KEY_COL2_PIN (GPIO_27)
#define KEY_COL3_PIN (GPIO_28)
#define KEY_COL4_PIN (GPIO_29)
#define KEY_COL5_PIN (GPIO_30)
#define KEY_COL6_PIN (GPIO_31)
#define KEY_COL_NUM (6)

/* LED */
#define LED_1_PIN       (GPIO_7)
#define LED_2_PIN       (GPIO_6)

#define LED_ON          (GPIO_Mode_Out_Low)
#define LED_OFF         (GPIO_Mode_Out_High)

/* audio */
#define AUDIO_CMD_HANDLE        (514)

#define ADPCM                   (1)
#define AUDIO_SNED_HANDLE       (AUDIO_CMD_HANDLE+2)
#define AUDIO_CTRL_HANDLE       (AUDIO_SNED_HANDLE+3)
#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN  (20)
#define AdcAnaDiffConfig_VOLTAGE_GAIN       (15)

#define MICRECORD_CACHE_NUM     (2)
#define ENCODE_CACHE_NUM        (20)
#define ENCODE_TIMES_ONCE       (1)
#define ENCODE_INPUT_SIZE       (512)
#define ENCODE_OUTPUT_SIZE      (134)
#define AUDIO_SEND_NTF_SIZE     (20)

#define TIMEOUT_TIME            (15000)


/* IR */
#define IR_IO           (GPIO_4)
#define IR_RCV_PIN    (GPIO_5)


/* FLASH */
typedef enum {
    SecretKey_Addr = 0x3BF00,
    addr_base = 0x3C000,
    ir_format_tag = addr_base,

    HD_POWER_DATA = 0x3FE00,
    HD_AVTV_DATA = 0x3FC00,
    HD_VOL_DATA = 0x3FA00,
    HD_VOL__DATA = 0x3F800,
} FlashRecordAddr_TypeDef; //存储数据需要 8字节


extern void action_after_led_blk(void);
extern void update_conn_param(bool is_sleep);
extern void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset);
extern void Read_Parse(const ATT_TABLE_TYPE *table);
extern void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len);
extern void LE_LTK_LOST(void);
extern void CONN_PARAM_ACCEPTED(void);
extern void ENCRYPT_FAIL(uint8_t reason);
extern void ENCRYPT_DONE(void);
extern void PAIR_FAIL(uint8_t reason);
extern void PAIR_DONE(void);
extern void LE_DISCONNECTED(uint8_t reason);
extern void LE_CONNECTED(void);
extern void Dev_PowerOn(void);
extern void tx_power_switch_set(bool switch_enable);
extern void app_task_run(void);
extern void app_init(void);

#endif
