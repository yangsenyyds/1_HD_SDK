#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_gpio.h"
#include "yc11xx_audio_adc.h"
#include "yc_dev_bt.h"
#include "att_list.h"
#include "factory_test_off_line.h"
#define Project_key  (259)

#if 0
#define AdcOutputMode   (ADC_SingleEndOutputWithCapacitor)
#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN  (20)
#define AdcAnaDiffConfig_VOLTAGE_GAIN       (15)
#else
#define AdcOutputMode (ADC_DifferentialOutput)
#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN (26)
#define AdcAnaDiffConfig_VOLTAGE_GAIN (20)
#endif

/* SecretKey_Check_enable */
#define SecretKey_Check_enable (1)

/* WATCH_DOG */
#define FUNCTION_WATCH_DOG

/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE   (GPIO_9)

/* sleep 1 hour*/
// #define SLEEP_ONE_HOUR      (1)

/* SLEEP */
#define ENCRYPT_DONE_DELAY  (3000)      // ms
#define PAIR_DONE_DELAY     (10000)     // ms
#define DIRECT_ADV_TIME     (10)         // s
#define SHUTDOWN_TIME       (30)        // s

#define UNIT_TIME_1S        (1000)      // ms

/* SOFTWARE TIMER */
#define TIMER_NUM       (11)   // KEY 1, LED 1, VBAT 1, VOICE 2, SLEEP 1, REMOTE_CONTROL 4, +1
#define TIMER_UNIT_MS   (10)

/* POWER */
#define BAT_REPORT_HANDLE   (98)
#define BAT_ATTLIST_INDEX   (39)

/* KEY */
#define KEY_ROW1_PIN    (GPIO_12)
#define KEY_ROW2_PIN    (GPIO_13)
#define KEY_ROW3_PIN    (GPIO_14)
#define KEY_ROW4_PIN    (GPIO_15)

#define KEY_ROW_NUM     (4)

#define KEY_COL1_PIN    (GPIO_24)
#define KEY_COL2_PIN    (GPIO_27)
#define KEY_COL3_PIN    (GPIO_28)
#define KEY_COL4_PIN    (GPIO_29)
#define KEY_COL5_PIN    (GPIO_30)
#define KEY_COL6_PIN    (GPIO_31)
#define KEY_COL7_PIN    (GPIO_33)
#define KEY_COL_NUM     (7)

#define KEY_SEND_HANDLE (139)
#define KEY_SEND_LEN    (6)

/* LED */
#define LED_1_PIN   (GPIO_7)
#define LED_2_PIN   (GPIO_6)


#define LED_ON      (GPIO_Mode_Out_Low)
#define LED_OFF     (GPIO_Mode_Out_High)

#define MICRECORD_CACHE_NUM     (10)
#define ENCODE_CACHE_NUM        (10)
#define ENCODE_TIMES_ONCE       (1)
#define ENCODE_INPUT_SIZE       (240)
#define ENCODE_OUTPUT_SIZE      (60)
#define AUDIO_SEND_NTF_SIZE     (172)
#define AUDIO_FRAME_HEADER_LEN  (1)
#define AUDIO_NTF_BUFSIZE       (AUDIO_FRAME_HEADER_LEN + AUDIO_SEND_NTF_SIZE)

#define MSBC                    (1)
#define SAMSUNG                 (1)
#define AUDIO_CMD_HANDLE        (141)
#define AUDIO_SNED_HANDLE       (get_VoiceHandle())
#define TIMEOUT_TIME            (15000)
/* IR */
#define IR_IO           (GPIO_4)

/* FLASH */
typedef enum {
    SecretKey_Addr = 0x3BF00,
    addr_base = 0x3C000,
} FlashRecordAddr_TypeDef; //存储数据需要 8字节

#define AUTH_NONE 0                                                             /**< No auth requirement. */
#define AUTH_BOND (1 << 0)                                                      /**< Bond flag. */
#define AUTH_MITM (1 << 2)                                                      /**< MITM flag. */
#define AUTH_SEC_CON (1 << 3)                                                   /**< Security connection flag. */
#define AUTH_KEY_PRESS_NOTIFY (1 << 4)                                          /**< Key press notify flag. */
#define AUTH_ALL (AUTH_BOND | AUTH_MITM | AUTH_SEC_CON | AUTH_KEY_PRESS_NOTIFY) /**< All authentication flags are on. */


/// slave latency
#define DF_SLAVE_LATENCY_VALUE_128 (128)
#define DF_SLAVE_LATENCY_VALUE_32 (32)
extern const uint8_t product_key_s[];
extern bool encrypt_state;
extern void LE_LTK_LOST(void);
extern void update_conn_param(bool is_sleep);
extern void stop_adv(void);
extern void start_adv(enum advType type, uint16_t adv_interval, bool timeout);
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
extern void Dev_PowerOn(void);
extern void tx_power_switch_set(bool switch_enable);
extern void app_task_run(void);
extern void app_init(void);

#endif
