#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_gpio.h"
#include "yc_dev_bt.h"
#include "att_list.h"


/* WATCH_DOG */
#define FUNCTION_WATCH_DOG

/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE   (GPIO_9)

/* SLEEP */
#define UNIT_TIME_1S        (1000)      // ms

/* SOFTWARE TIMER */
#define TIMER_NUM       (20)
#define TIMER_UNIT_MS   (10)

#define MI_RC  (1)
#ifdef MI_RC
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
#define KEY_COL6_PIN    (GPIO_4)
#define KEY_COL_NUM     (6)
/* LED */

#define LED_1_PIN (GPIO_36)
#define LED_2_PIN   (GPIO_6)
#define LED_3_PIN   (GPIO_8)
#define LED_ON (GPIO_Mode_Out_Low)
#define LED_OFF (GPIO_Mode_Out_High)

/* IR */
#define IR_IO           (GPIO_5)
#else
/* KEY */
#define KEY_ROW1_PIN    (GPIO_12)
#define KEY_ROW2_PIN    (GPIO_13)
#define KEY_ROW3_PIN    (GPIO_14)
#define KEY_ROW4_PIN    (GPIO_15)
#define KEY_ROW5_PIN    (GPIO_19)
#define KEY_ROW6_PIN    (GPIO_23)
#define KEY_ROW7_PIN    (GPIO_32)
#define KEY_ROW_NUM     (7)

#define KEY_COL1_PIN    (GPIO_24)
#define KEY_COL2_PIN    (GPIO_27)
#define KEY_COL3_PIN    (GPIO_28)
#define KEY_COL4_PIN    (GPIO_29)
#define KEY_COL5_PIN    (GPIO_30)
#define KEY_COL6_PIN    (GPIO_31)
#define KEY_COL7_PIN    (GPIO_33)
#define KEY_COL_NUM     (7)

/* LED */
#define LED_1_PIN   (GPIO_7)
#define LED_2_PIN   (GPIO_6)
#define LED_3_PIN   (GPIO_8)

#define LED_ON      (GPIO_Mode_Out_Low)
#define LED_OFF     (GPIO_Mode_Out_High)
/* IR */
#define IR_IO           (GPIO_4)
#endif
/* VOICE */
// #define AUDIO_TEST_MODE  
#ifdef AUDIO_TEST_MODE
#include "yc11xx_uart.h"
#define AUDIO_UART              (UARTA)
#define AUDIO_UART_RX_GPIO      (GPIO_11)
#define AUDIO_UART_TX_GPIO      (DEBUG_GPIO_DEFINE)
#endif

#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN  (20)
#define AdcAnaDiffConfig_VOLTAGE_GAIN       (15)

#define MICRECORD_CACHE_NUM     (10)
#define ENCODE_CACHE_NUM        (10)
#define ENCODE_TIMES_ONCE       (1)
#define ENCODE_INPUT_SIZE       (240)
#define ENCODE_OUTPUT_SIZE      (60)
#define ENCODE_SEND_SIZE        (57)



/* FACTORY */
#define FACTORY_MODE

#define FRAME_HEADER    (0xAA)


#define CODE_BURN_FLASHADDR1    (0x1000)
#define CODE_BURN_FLASHADDR2    (0x40000)
#define CODE_OFFSET_LEN         (3)
#define SERIAL_NAME_LEN         (16)
typedef enum{
    PC,
    DONGLE,
    RCU
}DIR_SEND_TypeDef;

typedef enum {
    SecretKey_Addr = 0x3BF00,
    addr_base = 0x3C000,
} FlashRecordAddr_TypeDef; //存储数据需要 8字节
#define CMD_NAME_CHECK  (0x01)
#define CMD_KEY         (0x02)
#define CMD_VOICE       (0x03)
#define CMD_LED         (0x04)
#define CMD_QUIT_FAC    (0xFF)

#define FACTORY_FRAME_LEN   (6)

#define DONGLE_SEND_HANDLE  (0XFF)


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
extern void app_task_run(void);
extern void app_init(void);

#endif
