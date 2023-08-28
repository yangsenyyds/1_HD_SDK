#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include <string.h>
#include "yc11xx.h"
#include "yc11xx_gpio.h"
#include "yc_dev_bt.h"
#include "att_list.h"


/* WATCH_DOG */
// #define FUNCTION_WATCH_DOG
// #define FUNCTION_WATCH_DOG_INTR

/* LOG */
#define FUNCTION_CONTROL_DEBUG_ENABLE
#define DEBUG_GPIO_DEFINE   (GPIO_9)

/* SLEEP */
#define ENCRYPT_DONE_DELAY  (5000)      // ms
#define PAIR_DONE_DELAY     (10000)     // ms
#define DIRECT_ADV_TIME     (3)         // s
#define SHUTDOWN_TIME       (5)         // s

#define UNIT_TIME_1S        (1000)      // ms

/* SOFTWARE TIMER */
#define TIMER_NUM           (17)   // KEY 1, LED 1, VBAT 1, VOICE 2, SLEEP 1, MOUSE 2, REMOTE_CONTROL 7, +1
#define TIMER_UNIT_MS       (5)

/* POWER */
#define BAT_ATTLIST_INDEX   (78)
#define BAT_REPORT_HANDLE   (80)


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
#define LED_1_PIN       (GPIO_7)
#define LED_2_PIN       (GPIO_6)

#define LED_ON          (GPIO_Mode_Out_Low)
#define LED_OFF         (GPIO_Mode_Out_High)

/* VOICE */
// #define AUDIO_TEST_MODE  
#ifdef AUDIO_TEST_MODE
#include "yc11xx_uart.h"
#define AUDIO_UART              (UARTA)
#define AUDIO_UART_RX_GPIO      (GPIO_33)
#define AUDIO_UART_TX_GPIO      (DEBUG_GPIO_DEFINE)
#endif

#define AdcAnaSEWithCapacitor_VOLTAGE_GAIN  (20)
#define AdcAnaDiffConfig_VOLTAGE_GAIN       (15)

#define ADPCM                   (1)

#define MICRECORD_CACHE_NUM     (10)
#define ENCODE_CACHE_NUM        (10)
#define ENCODE_TIMES_ONCE       (1)
#define ENCODE_INPUT_SIZE       (512)
#define ENCODE_OUTPUT_SIZE      (134)
#define AUDIO_SEND_NTF_SIZE     (20)

#define AUDIO_CTRL_HANDLE       (97)
#define AUDIO_SNED_HANDLE       (94)

/* IR */
#define IR_IO           (GPIO_4)
#define IR_RCV_PIN      (GPIO_5)

#define PWR_KEYNUM      (1)

/* IIC */
#define SOFTWARE_IIC
// #define HARDWARE_IIC
#ifdef SOFTWARE_IIC
#include "software_i2c.h"
#else
#include "yc11xx_iic.h"
#define IIC_TYPE        (IIC_0)
#endif

#define IIC_SCL_PIN     (GPIO_33)
#define IIC_SDA_PIN     (GPIO_32)

/* MOUSE */
#define MOUSE
// #define MOUSE_INT_MODE
#define XY_TYPE         (1)
// #define ORI_TYPE        (2)

#ifdef MOUSE
#define MOUSE_PWR_PIN   (GPIO_36)
#define MOUSE_CS_PIN    (GPIO_34)
#define MOUSE_INT_PIN   (GPIO_8)

#define QMI8658_SLAVE_ADDR_L    (0x6a)
#define QMI8658_SLAVE_ADDR_H    (0x6b)
#define QMI8658_SLAVE_ADDR      (QMI8658_SLAVE_ADDR_L << 1)

#define MOUSE_STOP_TIMEOUT      (20)    // unit: s

#define MOUSE_SNED_HANDLE   (52)
#define MOUSE_SEND_LEN      (4)

#define MOUSE_KEY   (20)
#define MOUSE_OK    (24)
#endif

/* FLASH */
typedef enum {
    addr_base = 0x3C000,
    HD_AV_IR = addr_base,
    HD_DVB_IR = HD_AV_IR + 0x8,
    HD_POWER_STATE = HD_DVB_IR + 0x8,
    HD_VOL_STATE = HD_POWER_STATE + 0x8,
    HD_SEARCH_STATE = HD_VOL_STATE + 0x8,
    HD_TV_DATA = 0x3FE00,
    HD_AV_DATA = 0x3FC00,
    HD_VOL_DATA = 0x3FA00,
    HD_VOL__DATA = 0x3F800,
    HD_MUTE_DATA = 0x3F600,

    HD_TV_DATA_OLD = 0x3F400,
    HD_AV_DATA_OLD = 0x3F200,
    HD_VOL_DATA_OLD= 0x3F000,
    HD_VOL__DATA_OLD = 0x3EE00,
    HD_MUTE_DATA_OLD = 0x3EC00,
} FlashRecordAddr_TypeDef; //存储数据需要 8字节


extern void mouse_switch(void);
extern void action_after_mic_close(void);
extern void update_conn_param(bool is_sleep);
extern void start_adv(enum advType type, uint16_t adv_interval);
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
