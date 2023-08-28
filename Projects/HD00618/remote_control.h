#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_gpio.h"
#include "yc11xx_audio_adc.h"
#include "yc_dev_bt.h"
#include "att_list.h"
#include "factory_test.h"

/* WATCH_DOG */
#define FUNCTION_WATCH_DOG

/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE   (GPIO_9)

/* SecretKey_Check_enable */
// #define SecretKey_Check_enable (1)

/* sleep 1 hour*/
#define SLEEP_ONE_HOUR      (1)

/* SLEEP */
#define ENCRYPT_DONE_DELAY  (3000)      // ms
#define PAIR_DONE_DELAY     (10000)     // ms
#define DIRECT_ADV_TIME     (10)         // s
#define SHUTDOWN_TIME       (30)        // s

#define UNIT_TIME_1S        (1000)      // ms

/* SOFTWARE TIMER */
#define TIMER_NUM       (12)   // KEY 1, LED 1, VBAT 1, VOICE 2, SLEEP 1, REMOTE_CONTROL 5, +1
#define TIMER_UNIT_MS   (10)

/* POWER */
#define BAT_REPORT_HANDLE   (66)
#define BAT_ATTLIST_INDEX   (27)
/* KEY */
#define KEY_ROW1_PIN    (GPIO_12)
#define KEY_ROW2_PIN    (GPIO_13)
#define KEY_ROW3_PIN    (GPIO_14)
#define KEY_ROW4_PIN    (GPIO_15)
#define KEY_ROW5_PIN    (GPIO_19)
#define KEY_ROW6_PIN    (GPIO_23)
#define KEY_ROW_NUM     (6)

#define KEY_COL1_PIN    (GPIO_24)
#define KEY_COL2_PIN    (GPIO_27)
#define KEY_COL3_PIN    (GPIO_28)
#define KEY_COL4_PIN    (GPIO_29)
#define KEY_COL5_PIN    (GPIO_30)
#define KEY_COL6_PIN    (GPIO_31)
#define KEY_COL_NUM     (6)

/* LED */
#define LED_1_PIN   (GPIO_8)
#define LED_2_PIN   (GPIO_6)
#define LED_3_PIN   (GPIO_7)

#define LED_ON      (GPIO_Mode_Out_Low)
#define LED_OFF     (GPIO_Mode_Out_High)

/* VOICE */
// #define AUDIO_TEST_MODE  
#ifdef AUDIO_TEST_MODE
#include "yc11xx_uart.h"
#define AUDIO_UART              (UARTA)
#define AUDIO_UART_RX_GPIO      (GPIO_11)
#define AUDIO_UART_TX_GPIO      (DEBUG_GPIO_DEFINE)
#endif
#define AdcOutputMode   (ADC_SingleEndOutputWithCapacitor)
#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN  (20)
#define AdcAnaDiffConfig_VOLTAGE_GAIN       (15)

#define OPUS                    (1)

#define MICRECORD_CACHE_NUM     (4)
#define ENCODE_CACHE_NUM        (20)
#define ENCODE_TIMES_ONCE       (1)
#define ENCODE_INPUT_SIZE       (640)
#define ENCODE_OUTPUT_SIZE      (80)
#define AUDIO_SEND_NTF_SIZE     (ENCODE_OUTPUT_SIZE)

#define TIMEOUT_TIME            (15000)

#define AUDIO_CMD_HANDLE        (100)
#define AUDIO_SNED_HANDLE       (get_VoiceHandle())

/* IR */
#define IR_IO           (GPIO_4)
#define IR_RCV_PIN      (GPIO_5)

#define RAW_BUFFER_LENGTH (508)

#define PWR_KEYNUM      (1)
#define VOL_UP_KEYNUM   (17)
#define VOL_DOWN_KEYNUM (20)
#define MUTE_KEYNUM     (18)

#define IR_LEARN_DATA_BUFSIZE   (1024)

/* FLASH */
typedef enum {
    SecretKey_Addr = 0x3BF00,
    addr_base = 0x3C000,
    ir_learn_tag = addr_base,
    ir_power = ir_learn_tag + 0x8,
    ir_vol = ir_power + 0x8,
    ir_vol_ = ir_vol + 0x8,
    ir_mute = ir_vol_ + 0x8,
    first_done = ir_mute + 0x8,
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
