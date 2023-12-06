#include "mouse.h"
#include "software_timer.h"
#include "app_sleep.h"
#ifdef SOFTWARE_IIC
#include "software_i2c.h"
#else
#include "yc11xx_iic.h"
#endif
#include "yc_dev_bt.h"
#include "yc11xx_ipc.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_gpio.h"
#include "yc11xx_systick.h"
#include "yc_debug.h"
#include "app_config.h"
#include "led.h"

#ifndef MOUSE_INIT_DELAY
#define MOUSE_INIT_DELAY (20) ///(100)
#endif

#ifndef MOUSE_REPORT_TIME_MAX
#define MOUSE_REPORT_TIME_MAX (10)
#endif
#ifndef SENSOR_REPORT_INTERVAL_TIME
#define SENSOR_REPORT_INTERVAL_TIME TIMER_UNIT_MS
#endif

#ifndef SENSOR_FILTER_CACHE
#define SENSOR_FILTER_CACHE (10)
#endif
#ifndef SENSOR_SMOOTH_CACHE
#define SENSOR_SMOOTH_CACHE (6)
#endif

#define DEVICE_SLAVE_READ (0x01)
#define DEVICE_SLAVE_WRITE (0x00)

#define QMI8658_CTRL7_DISABLE_ALL (0x0)
#define QMI8658_CTRL7_ACC_ENABLE (0x1)
#define QMI8658_CTRL7_GYR_ENABLE (0x2)
#define QMI8658_CTRL7_MAG_ENABLE (0x4)
#define QMI8658_CTRL7_AE_ENABLE (0x8)
#define QMI8658_CTRL7_GYR_SNOOZE_ENABLE (0x10)
#define QMI8658_CTRL7_ENABLE_MASK (0xF)

#define QMI8658_CONFIG_ACC_ENABLE QMI8658_CTRL7_ACC_ENABLE
#define QMI8658_CONFIG_GYR_ENABLE QMI8658_CTRL7_GYR_ENABLE
#define QMI8658_CONFIG_MAG_ENABLE QMI8658_CTRL7_MAG_ENABLE
#define QMI8658_CONFIG_AE_ENABLE QMI8658_CTRL7_AE_ENABLE
#define QMI8658_CONFIG_ACCGYR_ENABLE (QMI8658_CONFIG_ACC_ENABLE | QMI8658_CONFIG_GYR_ENABLE)
#define QMI8658_CONFIG_ACCGYRMAG_ENABLE (QMI8658_CONFIG_ACC_ENABLE | QMI8658_CONFIG_GYR_ENABLE | QMI8658_CONFIG_MAG_ENABLE)
#define QMI8658_CONFIG_AEMAG_ENABLE (QMI8658_CONFIG_AE_ENABLE | QMI8658_CONFIG_MAG_ENABLE)

#define QMI8658_HANDSHAKE_NEW
#define QMI8658_HANDSHAKE_TO_STATUS

#ifndef ONE_G
#define ONE_G (9.807f)
#endif

#define RAD_S (0.01745f) // pi/180

enum Qmi8658Register
{
    Qmi8658Register_WhoAmI = 0,
    Qmi8658Register_Revision,
    Qmi8658Register_Ctrl1,
    Qmi8658Register_Ctrl2,
    Qmi8658Register_Ctrl3,
    Qmi8658Register_Ctrl4,
    Qmi8658Register_Ctrl5,
    Qmi8658Register_Ctrl6,
    Qmi8658Register_Ctrl7,
    Qmi8658Register_Ctrl8,
    Qmi8658Register_Ctrl9,
    Qmi8658Register_Cal1_L = 11,
    Qmi8658Register_Cal1_H,
    Qmi8658Register_Cal2_L,
    Qmi8658Register_Cal2_H,
    Qmi8658Register_Cal3_L,
    Qmi8658Register_Cal3_H,
    Qmi8658Register_Cal4_L,
    Qmi8658Register_Cal4_H,
    Qmi8658Register_FifoWmkTh = 19,
    Qmi8658Register_FifoCtrl,
    Qmi8658Register_FifoCount,
    Qmi8658Register_FifoStatus,
    Qmi8658Register_FifoData,
    Qmi8658Register_StatusInt = 45,
    Qmi8658Register_Status0,
    Qmi8658Register_Status1,
    Qmi8658Register_Timestamp_L = 48,
    Qmi8658Register_Timestamp_M,
    Qmi8658Register_Timestamp_H,
    Qmi8658Register_Tempearture_L = 51,
    Qmi8658Register_Tempearture_H,
    Qmi8658Register_Ax_L = 53,
    Qmi8658Register_Ax_H,
    Qmi8658Register_Ay_L,
    Qmi8658Register_Ay_H,
    Qmi8658Register_Az_L,
    Qmi8658Register_Az_H,
    Qmi8658Register_Gx_L = 59,
    Qmi8658Register_Gx_H,
    Qmi8658Register_Gy_L,
    Qmi8658Register_Gy_H,
    Qmi8658Register_Gz_L,
    Qmi8658Register_Gz_H,
    Qmi8658Register_Mx_L = 65,
    Qmi8658Register_Mx_H,
    Qmi8658Register_My_L,
    Qmi8658Register_My_H,
    Qmi8658Register_Mz_L,
    Qmi8658Register_Mz_H,
    Qmi8658Register_Q1_L = 73,
    Qmi8658Register_Q1_H,
    Qmi8658Register_Q2_L,
    Qmi8658Register_Q2_H,
    Qmi8658Register_Q3_L,
    Qmi8658Register_Q3_H,
    Qmi8658Register_Q4_L,
    Qmi8658Register_Q4_H,
    Qmi8658Register_Dvx_L = 81,
    Qmi8658Register_Dvx_H,
    Qmi8658Register_Dvy_L,
    Qmi8658Register_Dvy_H,
    Qmi8658Register_Dvz_L,
    Qmi8658Register_Dvz_H,
    Qmi8658Register_AeReg1 = 87,
    Qmi8658Register_AeOverflow,
    Qmi8658Register_Reset = 96,
    Qmi8658Register_I2CM_STATUS = 110
};

enum Qmi8658_Ctrl9Command
{
    Qmi8658_Ctrl9_Cmd_ACK = 0X00,
    Qmi8658_Ctrl9_Cmd_GyroBias = 0X01,
    Qmi8658_Ctrl9_Cmd_Rqst_Sdi_Mod = 0X03,
    Qmi8658_Ctrl9_Cmd_Rst_Fifo = 0X04,
    Qmi8658_Ctrl9_Cmd_Req_Fifo = 0X05,
    Qmi8658_Ctrl9_Cmd_I2CM_Write = 0X06,
    Qmi8658_Ctrl9_Cmd_WoM_Setting = 0x08,
    Qmi8658_Ctrl9_Cmd_AccelHostDeltaOffset = 0x09,
    Qmi8658_Ctrl9_Cmd_GyroHostDeltaOffset = 0x0A,
    Qmi8658_Ctrl9_Cmd_EnableExtReset = 0x0B,
    Qmi8658_Ctrl9_Cmd_EnableTap = 0x0C,
    Qmi8658_Ctrl9_Cmd_EnablePedometer = 0x0D,

    Qmi8658_Ctrl9_Cmd_Motion = 0x0E,
    Qmi8658_Ctrl9_Cmd_CopyUsid = 0x10,
    Qmi8658_Ctrl9_Cmd_SetRpu = 0x11,
    Qmi8658_Ctrl9_Cmd_Dbg_WoM_Data_Enable = 0xF8,

};

enum Qmi8658_AccRange
{
    Qmi8658AccRange_2g = 0x00 << 4, /*!< \brief +/- 2g range */
    Qmi8658AccRange_4g = 0x01 << 4, /*!< \brief +/- 4g range */
    Qmi8658AccRange_8g = 0x02 << 4, /*!< \brief +/- 8g range */
    Qmi8658AccRange_16g = 0x03 << 4 /*!< \brief +/- 16g range */
};

enum Qmi8658_AccOdr
{
    Qmi8658AccOdr_8000Hz = 0x00,         /*!< \brief High resolution 8000Hz output rate. */
    Qmi8658AccOdr_4000Hz = 0x01,         /*!< \brief High resolution 4000Hz output rate. */
    Qmi8658AccOdr_2000Hz = 0x02,         /*!< \brief High resolution 2000Hz output rate. */
    Qmi8658AccOdr_1000Hz = 0x03,         /*!< \brief High resolution 1000Hz output rate. */
    Qmi8658AccOdr_500Hz = 0x04,          /*!< \brief High resolution 500Hz output rate. */
    Qmi8658AccOdr_250Hz = 0x05,          /*!< \brief High resolution 250Hz output rate. */
    Qmi8658AccOdr_125Hz = 0x06,          /*!< \brief High resolution 125Hz output rate. */
    Qmi8658AccOdr_62_5Hz = 0x07,         /*!< \brief High resolution 62.5Hz output rate. */
    Qmi8658AccOdr_31_25Hz = 0x08,        /*!< \brief High resolution 31.25Hz output rate. */
    Qmi8658AccOdr_LowPower_128Hz = 0x0c, /*!< \brief Low power 128Hz output rate. */
    Qmi8658AccOdr_LowPower_21Hz = 0x0d,  /*!< \brief Low power 21Hz output rate. */
    Qmi8658AccOdr_LowPower_11Hz = 0x0e,  /*!< \brief Low power 11Hz output rate. */
    Qmi8658AccOdr_LowPower_3Hz = 0x0f    /*!< \brief Low power 3Hz output rate. */
};

enum Qmi8658_GyrRange
{
    Qmi8658GyrRange_16dps = 0 << 4,   /*!< \brief +-16 degrees per second. */
    Qmi8658GyrRange_32dps = 1 << 4,   /*!< \brief +-32 degrees per second. */
    Qmi8658GyrRange_64dps = 2 << 4,   /*!< \brief +-64 degrees per second. */
    Qmi8658GyrRange_128dps = 3 << 4,  /*!< \brief +-128 degrees per second. */
    Qmi8658GyrRange_256dps = 4 << 4,  /*!< \brief +-256 degrees per second. */
    Qmi8658GyrRange_512dps = 5 << 4,  /*!< \brief +-512 degrees per second. */
    Qmi8658GyrRange_1024dps = 6 << 4, /*!< \brief +-1024 degrees per second. */
    Qmi8658GyrRange_2048dps = 7 << 4, /*!< \brief +-2048 degrees per second. */
};

enum Qmi8658_GyrOdr
{
    Qmi8658GyrOdr_8000Hz = 0x00, /*!< \brief High resolution 8000Hz output rate. */
    Qmi8658GyrOdr_4000Hz = 0x01, /*!< \brief High resolution 4000Hz output rate. */
    Qmi8658GyrOdr_2000Hz = 0x02, /*!< \brief High resolution 2000Hz output rate. */
    Qmi8658GyrOdr_1000Hz = 0x03, /*!< \brief High resolution 1000Hz output rate. */
    Qmi8658GyrOdr_500Hz = 0x04,  /*!< \brief High resolution 500Hz output rate. */
    Qmi8658GyrOdr_250Hz = 0x05,  /*!< \brief High resolution 250Hz output rate. */
    Qmi8658GyrOdr_125Hz = 0x06,  /*!< \brief High resolution 125Hz output rate. */
    Qmi8658GyrOdr_62_5Hz = 0x07, /*!< \brief High resolution 62.5Hz output rate. */
    Qmi8658GyrOdr_31_25Hz = 0x08 /*!< \brief High resolution 31.25Hz output rate. */
};

enum Qmi8658_LpfConfig
{
    Qmi8658Lpf_Disable, /*!< \brief Disable low pass filter. */
    Qmi8658Lpf_Enable   /*!< \brief Enable low pass filter. */
};

enum Qmi8658_StConfig
{
    Qmi8658St_Disable, /*!< \brief Disable high pass filter. */
    Qmi8658St_Enable   /*!< \brief Enable high pass filter. */
};

enum Qmi8658_LpfMode
{
    A_LSP_MODE_0 = 0x00 << 1,
    A_LSP_MODE_1 = 0x01 << 1,
    A_LSP_MODE_2 = 0x02 << 1,
    A_LSP_MODE_3 = 0x03 << 1,

    G_LSP_MODE_0 = 0x00 << 5,
    G_LSP_MODE_1 = 0x01 << 5,
    G_LSP_MODE_2 = 0x02 << 5,
    G_LSP_MODE_3 = 0x03 << 5
};
#ifdef QMI8658_USE_ANYMOTION
enum Qmi8658_anymotion_G_TH
{
    Qmi8658_1G = 1 << 5,
    Qmi8658_2G = 2 << 5,
    Qmi8658_3G = 3 << 5,
    Qmi8658_4G = 4 << 5,
    Qmi8658_5G = 5 << 5,
    Qmi8658_6G = 6 << 5,
    Qmi8658_7G = 7 << 5,
    Qmi8658_8G = 8 << 5,
};
#endif
typedef struct
{
    uint8_t inputSelection;
    enum Qmi8658_AccRange accRange;
    enum Qmi8658_AccOdr accOdr;
    enum Qmi8658_GyrRange gyrRange;
    enum Qmi8658_GyrOdr gyrOdr;
#if defined(QMI8658_USE_FIFO)
    uint8_t fifo_ctrl;
    uint8_t fifo_fss;
    uint8_t fifo_status;
#endif
} Qmi8658_cfg_t;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static sensor_data_t sensor_data_report;
#if XY_TYPE
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static sensor_data_t sensor_smooth_cache[SENSOR_SMOOTH_CACHE];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t sensor_smooth_flag;
#endif
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint16_t acc_lsb_div;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint16_t gyro_lsb_div;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static Qmi8658_cfg_t qmi8658_cfg;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static bool mouse_on_delay;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static bool mouse_on_off;
// static uint8_t mouse_report_timernum = 0xFF;
uint8_t mouse_report_timernum = 0xFF;
static uint8_t mouse_init_timernum = 0xFF;
static mouse_report_cb_t mouse_report_cb;
#ifdef QMI8658_USE_ANYMOTION
uint16_t get_acc_lsb_div(void)
{
    return acc_lsb_div;
}

uint16_t get_gyro_lsb_div(void)
{
    return gyro_lsb_div;
}
#endif
#ifdef SOFTWARE_IIC
static uint8_t sw_i2c_writereg(uint8_t slave_addr, uint8_t reg_addr, uint8_t reg_val)
{
    IIC_Start();
    IIC_SendByte(slave_addr);
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_SendByte(reg_addr);
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_SendByte(reg_val | DEVICE_SLAVE_WRITE);
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_Stop();

    return 0;
}

static uint8_t sw_i2c_readreg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *read_buf, uint16_t read_len)
{
    IIC_Start();
    IIC_SendByte(slave_addr);
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_SendByte(reg_addr);
    if (IIC_WaitAck())
    {
        return 1;
    }

    IIC_Start();
    IIC_SendByte(slave_addr | DEVICE_SLAVE_READ);
    if (IIC_WaitAck())
    {
        return 1;
    }

    for (uint16_t i = 0; i < (read_len - 1); i++)
    {
        *read_buf = IIC_ReadByte(1);
        read_buf++;
    }
    *read_buf = IIC_ReadByte(0);
    IIC_Stop();

    return 0;
}

static uint8_t Qmi8658_write_reg(uint8_t reg_addr, uint8_t reg_val)
{
    uint8_t ret = 1;
    uint16_t retry = 0;

    while ((ret) && (retry++ < 5))
    {
        ret = sw_i2c_writereg(QMI8658_SLAVE_ADDR, reg_addr, reg_val);
    }
    return ret;
}

static uint8_t Qmi8658_read_reg(uint8_t reg_addr, uint8_t *read_buf, uint16_t len)
{
    uint8_t ret = 1;
    uint16_t retry = 0;

    while ((ret) && (retry++ < 5))
    {
        ret = sw_i2c_readreg(QMI8658_SLAVE_ADDR, reg_addr, read_buf, len);
    }
    return ret;
}
#else
static void Qmi8658_write_reg(uint8_t reg_addr, uint8_t reg_val)
{
    uint8_t send_buf[] = {QMI8658_SLAVE_ADDR, reg_addr, reg_val | DEVICE_SLAVE_WRITE};

    IIC_SendData(IIC_TYPE, send_buf, 3);
}

static void Qmi8658_read_reg(uint8_t reg_addr, uint8_t *read_buf, uint16_t len)
{
    uint8_t send_buf[] = {QMI8658_SLAVE_ADDR, reg_addr, QMI8658_SLAVE_ADDR | DEVICE_SLAVE_READ};

    IIC_ReceiveData(IIC_TYPE, send_buf, 3, read_buf, len);
}
#endif

static void Qmi8658_send_ctl9cmd(enum Qmi8658_Ctrl9Command cmd)
{
    uint8_t status = 0x00;
    uint8_t cnt = 0;

    Qmi8658_write_reg(Qmi8658Register_Ctrl9, (uint8_t)cmd);
#if defined(QMI8658_HANDSHAKE_NEW)
#if defined(QMI8658_HANDSHAKE_TO_STATUS)
    uint8_t status_reg = Qmi8658Register_StatusInt;
    uint8_t cmd_done = 0x80;
#else
    uint8_t status_reg = Qmi8658Register_Status1;
    uint8_t cmd_done = 0x01;
#endif

    Qmi8658_read_reg(status_reg, &status, 1);
    while (((status & cmd_done) != cmd_done) && (cnt++ < 200))
    {
        SysTick_DelayMs(1);
        Qmi8658_read_reg(status_reg, &status, 1);
    }

    cnt = 0;
    Qmi8658_write_reg(Qmi8658Register_Ctrl9, Qmi8658_Ctrl9_Cmd_ACK);
    Qmi8658_read_reg(status_reg, &status, 1);
    while (((status & cmd_done) == cmd_done) && (cnt++ < 200))
    {
        SysTick_DelayMs(1);
        Qmi8658_read_reg(status_reg, &status, 1);
    }
#else
    while (((status & QMI8658_STATUS1_CMD_DONE) == 0) && (cnt++ < 200))
    {
        Qmi8658_read_reg(Qmi8658Register_Status1, &status, sizeof(status));
    }
#endif
}

static void Qmi8658_config_gyro(enum Qmi8658_GyrRange range, enum Qmi8658_GyrOdr odr, enum Qmi8658_LpfConfig lpfEnable, enum Qmi8658_StConfig stEnable)
{
    uint8_t ctl_dada;

    switch (range)
    {
    case Qmi8658GyrRange_16dps:
        gyro_lsb_div = 2048;
        break;
    case Qmi8658GyrRange_32dps:
        gyro_lsb_div = 1024;
        break;
    case Qmi8658GyrRange_64dps:
        gyro_lsb_div = 512;
        break;
    case Qmi8658GyrRange_128dps:
        gyro_lsb_div = 256;
        break;
    case Qmi8658GyrRange_256dps:
        gyro_lsb_div = 128;
        break;
    case Qmi8658GyrRange_512dps:
        gyro_lsb_div = 64;
        break;
    case Qmi8658GyrRange_1024dps:
        gyro_lsb_div = 32;
        break;
    case Qmi8658GyrRange_2048dps:
        gyro_lsb_div = 16;
        break;
        // case Qmi8658GyrRange_4096dps:
        //     gyro_lsb_div = 8;
        //     break;

    default:
        range = Qmi8658GyrRange_512dps;
        gyro_lsb_div = 64;
        break;
    }

    if (stEnable == Qmi8658St_Enable)
    {
        ctl_dada = (uint8_t)range | (uint8_t)odr | 0x80;
    }
    else
    {
        ctl_dada = (uint8_t)range | (uint8_t)odr;
    }
    Qmi8658_write_reg(Qmi8658Register_Ctrl3, ctl_dada);

    Qmi8658_read_reg(Qmi8658Register_Ctrl5, &ctl_dada, 1);
    ctl_dada &= 0x0f;
    if (lpfEnable == Qmi8658Lpf_Enable)
    {
        ctl_dada |= G_LSP_MODE_3;
        ctl_dada |= 0x10;
    }
    else
    {
        ctl_dada &= ~0x10;
    }
    Qmi8658_write_reg(Qmi8658Register_Ctrl5, ctl_dada);
}

static void Qmi8658_config_acc(enum Qmi8658_AccRange range, enum Qmi8658_AccOdr odr, enum Qmi8658_LpfConfig lpfEnable, enum Qmi8658_StConfig stEnable)
{
    uint8_t ctl_dada;

    switch (range)
    {
    case Qmi8658AccRange_2g:
        acc_lsb_div = (1 << 14);
        break;
    case Qmi8658AccRange_4g:
        acc_lsb_div = (1 << 13);
        break;
    case Qmi8658AccRange_8g:
        acc_lsb_div = (1 << 12);
        break;
    case Qmi8658AccRange_16g:
        acc_lsb_div = (1 << 11);
        break;

    default:
        range = Qmi8658AccRange_8g;
        acc_lsb_div = (1 << 12);
        break;
    }

    if (stEnable == Qmi8658St_Enable)
    {
        ctl_dada = (uint8_t)range | (uint8_t)odr | 0x80;
    }
    else
    {
        ctl_dada = (uint8_t)range | (uint8_t)odr;
    }
    Qmi8658_write_reg(Qmi8658Register_Ctrl2, ctl_dada);

    Qmi8658_read_reg(Qmi8658Register_Ctrl5, &ctl_dada, 1);
    ctl_dada &= 0xf0;
    if (lpfEnable == Qmi8658Lpf_Enable)
    {
        ctl_dada |= A_LSP_MODE_3;
        ctl_dada |= 0x01;
    }
    else
    {
        ctl_dada &= ~0x01;
    }
    Qmi8658_write_reg(Qmi8658Register_Ctrl5, ctl_dada);
}

#ifdef QMI8658_USE_ANYMOTION
/*******/
/*motion_g_th  is bit5~bit7 ,  1 bit is   1G .  such as  :  001 is  1G, 111 is  7G .  */
/*motion_mg_th  is  bit0 ~bit4,   1bit is   1/32G = 32.25mg;    00001 is  32.25mg ,   00002 is  32.25*2mg .  motion_mg_th   set range is  0~ 31     */
/*****/
static uint8_t anymotion_Config(enum Qmi8658_anymotion_G_TH motion_g_th, uint8_t motion_mg_th) // 11hz=0x0e???
{
    Qmi8658_write_reg(Qmi8658Register_Cal1_L, (motion_g_th + motion_mg_th)); // 0x03   // any motion X threshold U 3.5 first three bit(uint 1g)  last five bit (uint 1/32 g)
    Qmi8658_write_reg(Qmi8658Register_Cal1_H, (motion_g_th + motion_mg_th)); // 0x03  //the value of AnyMotionYThr.
    Qmi8658_write_reg(Qmi8658Register_Cal2_L, (motion_g_th + motion_mg_th)); // 0x03  //the value of AnyMotionZThr.
    Qmi8658_write_reg(Qmi8658Register_Cal2_H, 0x01);
    Qmi8658_write_reg(Qmi8658Register_Cal3_L, 0x01);
    Qmi8658_write_reg(Qmi8658Register_Cal3_H, 0x01);
    Qmi8658_write_reg(Qmi8658Register_Cal4_L, 0x07);

    Qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x01);
    Qmi8658_send_ctl9cmd(Qmi8658_Ctrl9_Cmd_Motion);

    Qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x3); // 0x05
    Qmi8658_write_reg(Qmi8658Register_Cal1_H, 0x2a);
    Qmi8658_write_reg(Qmi8658Register_Cal2_L, 0x2c);
    Qmi8658_write_reg(Qmi8658Register_Cal2_H, 0x01);
    Qmi8658_write_reg(Qmi8658Register_Cal3_L, 0x64);
    Qmi8658_write_reg(Qmi8658Register_Cal3_H, 0x00);
    Qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x02);
    Qmi8658_send_ctl9cmd(Qmi8658_Ctrl9_Cmd_Motion);

    return 0;
}

static void Qmi8658_reset(void)
{
    Qmi8658_write_reg(Qmi8658Register_Reset, 0xB0);
}

static void anymotion_lowpwr_config(void)
{
    qmi8658_cfg.inputSelection = QMI8658_CONFIG_ACC_ENABLE; // QMI8658_CONFIG_ACCGYR_ENABLE;
    qmi8658_cfg.accRange = Qmi8658AccRange_4g;
    qmi8658_cfg.accOdr = Qmi8658AccOdr_LowPower_3Hz; // Qmi8658AccOdr_LowPower_3Hz;//Qmi8658AccOdr_LowPower_11Hz;

    Qmi8658_reset();
    SysTick_DelayMs(30); // delay 30ms.

    Qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x78);
    Qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);

    Qmi8658_write_reg(Qmi8658Register_Ctrl8, 0xc2); // 0xC2  AMD  interrupt to  int1

    anymotion_Config(0, 2); //  th = 1.1g;

    Qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x0F);
    Qmi8658_send_ctl9cmd(Qmi8658_Ctrl9_Cmd_SetRpu);

#if defined(QMI8658_USE_FIFO)
    Qmi8658_config_fifo(20, Qmi8658_Fifo_128, Qmi8658_Fifo_Fifo, QMI8658_FORMAT_12_BYTES);
#endif
    Qmi8658_config_acc(qmi8658_cfg.accRange, qmi8658_cfg.accOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
    Qmi8658_write_reg(Qmi8658Register_Ctrl7, qmi8658_cfg.inputSelection & QMI8658_CTRL7_ENABLE_MASK);
}

static void anymotion_high_odr_enable(void)
{
    unsigned char ctrl8_data = 0x00;

    Qmi8658_read_reg(Qmi8658Register_Ctrl8, &ctrl8_data, 1);
    Qmi8658_write_reg(Qmi8658Register_Ctrl8, ctrl8_data | 0xC2); // 0xC2
    anymotion_Config(0, 1);                                      ////  th = 1.1g;  /// Qmi8658_1G  3
}

static void anymotion_disable(void)
{
    unsigned char ctrl8_data = 0x00;

    Qmi8658_read_reg(Qmi8658Register_Ctrl8, &ctrl8_data, 1);
    Qmi8658_write_reg(Qmi8658Register_Ctrl8, ctrl8_data & (~0x02)); // 0xC2

    Qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x23); // 0x03   // any motion X threshold U 3.5 first three bit(uint 1g)  last five bit (uint 1/32 g)
    Qmi8658_write_reg(Qmi8658Register_Cal1_H, 0x23); // 0x03  //the value of AnyMotionYThr.
    Qmi8658_write_reg(Qmi8658Register_Cal2_L, 0x23); // 0x03  //the value of AnyMotionZThr.
    Qmi8658_write_reg(Qmi8658Register_Cal2_H, 0x03);
    Qmi8658_write_reg(Qmi8658Register_Cal3_L, 0x03);
    Qmi8658_write_reg(Qmi8658Register_Cal3_H, 0x03);
    Qmi8658_write_reg(Qmi8658Register_Cal4_L, 0x00);
    Qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x01);

    Qmi8658_send_ctl9cmd(Qmi8658_Ctrl9_Cmd_Motion);
    Qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x03); // 0x05
    Qmi8658_write_reg(Qmi8658Register_Cal1_H, 0x1a);
    Qmi8658_write_reg(Qmi8658Register_Cal2_L, 0x2c);
    Qmi8658_write_reg(Qmi8658Register_Cal2_H, 0x01);
    Qmi8658_write_reg(Qmi8658Register_Cal3_L, 0x64);
    Qmi8658_write_reg(Qmi8658Register_Cal3_H, 0x00);
    Qmi8658_write_reg(Qmi8658Register_Cal4_H, 0x02);
    Qmi8658_send_ctl9cmd(Qmi8658_Ctrl9_Cmd_Motion);
}

#endif

#ifdef MOUSE_INT_MODE
void mouse_int_mode(void)
{
    Qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x78);
    GPIO_Init(MOUSE_INT_PIN, GPIO_Mode_In_Down);
    GPIO_WakeUp(MOUSE_INT_PIN, GPIO_WakeUpHigh);

    anymotion_lowpwr_config();
}

void mouse_lpm_int_mode(void)
{
    DEBUG_LOG_STRING("mouse_lpm_int_mode \r\n");
    Qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x78);
    GPIO_Init(MOUSE_INT_PIN, GPIO_Mode_In_Down);

    anymotion_lowpwr_config();
}
#endif

static void Qmi8658_power_off(void)
{
    Qmi8658_write_reg(Qmi8658Register_Ctrl2, 0x0F);
    Qmi8658_write_reg(Qmi8658Register_Ctrl3, 0x0F);
    Qmi8658_write_reg(Qmi8658Register_Ctrl7, 0x00);
}

static void Qmi8658_power_on(void)
{
    Qmi8658_write_reg(Qmi8658Register_Cal1_L, 0x0F);
    Qmi8658_send_ctl9cmd(Qmi8658_Ctrl9_Cmd_SetRpu);

#ifdef MOUSE_INT_MODE
    Qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x6C);
#else
    Qmi8658_write_reg(Qmi8658Register_Ctrl1, 0x60);
#endif

#ifdef QMI8658_USE_ANYMOTION
    anymotion_high_odr_enable();
#endif
#ifdef LG
    qmi8658_cfg.inputSelection = QMI8658_CONFIG_ACCGYR_ENABLE;
    qmi8658_cfg.accRange = Qmi8658AccRange_8g;
    qmi8658_cfg.accOdr = Qmi8658AccOdr_250Hz;
    qmi8658_cfg.gyrRange = Qmi8658GyrRange_2048dps;
    qmi8658_cfg.gyrOdr = Qmi8658GyrOdr_250Hz;
#else
    qmi8658_cfg.inputSelection = QMI8658_CONFIG_ACCGYR_ENABLE;
    qmi8658_cfg.accRange = Qmi8658AccRange_4g;
    qmi8658_cfg.accOdr = Qmi8658AccOdr_8000Hz;
    qmi8658_cfg.gyrRange = Qmi8658GyrRange_1024dps;
    qmi8658_cfg.gyrOdr = Qmi8658GyrOdr_8000Hz;
#endif
#if defined(QMI8658_USE_FIFO)
    Qmi8658_config_fifo(20, Qmi8658_Fifo_128, Qmi8658_Fifo_Fifo, QMI8658_FORMAT_12_BYTES);
#endif
    Qmi8658_config_acc(qmi8658_cfg.accRange, qmi8658_cfg.accOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
    Qmi8658_config_gyro(qmi8658_cfg.gyrRange, qmi8658_cfg.gyrOdr, Qmi8658Lpf_Disable, Qmi8658St_Disable);
    Qmi8658_write_reg(Qmi8658Register_Ctrl7, qmi8658_cfg.inputSelection & QMI8658_CTRL7_ENABLE_MASK);
}

#if XY_TYPE
static void Qmi8658_read_xyz(sensor_data_t *data)
{
    uint8_t buf_reg[12];
    short raw_acc_xyz[3], raw_gyro_xyz[3];

    Qmi8658_read_reg(Qmi8658Register_Ax_L, buf_reg, 12);
    raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));
    raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
    raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));
    // m/s2
    data->ax = (float)(raw_acc_xyz[0] * ONE_G) / acc_lsb_div;
    data->ay = (float)(raw_acc_xyz[1] * ONE_G) / acc_lsb_div;
    data->az = (float)(raw_acc_xyz[2] * ONE_G) / acc_lsb_div;
    // dps/s
    // data->gx = (float)(raw_gyro_xyz[0]*1.0f)/gyro_lsb_div;
    // data->gy = (float)(raw_gyro_xyz[1]*1.0f)/gyro_lsb_div;
    // data->gz = (float)(raw_gyro_xyz[2]*1.0f)/gyro_lsb_div;
    // rad/s
    data->gx = (float)(raw_gyro_xyz[0] * RAD_S) / gyro_lsb_div;
    data->gy = (float)(raw_gyro_xyz[1] * RAD_S) / gyro_lsb_div;
    data->gz = (float)(raw_gyro_xyz[2] * RAD_S) / gyro_lsb_div;
}

static void sensor_data_smooth(void)
{
    sensor_data_t data_filter;
    memset((void *)&data_filter, 0, sizeof(sensor_data_t));

    for (uint8_t cache = 0; cache < SENSOR_FILTER_CACHE; cache++)
    {
        sensor_data_t data;

        Qmi8658_read_xyz(&data);
        for (uint8_t element = 0; element < 6; element++)
        {
            *(&data_filter.ax + element) += *(&data.ax + element) * 0.1f;
        }

        SysTick_DelayUs(100);
    }

    memset(&sensor_data_report, 0, sizeof(sensor_data_t));
    memcpy((void *)&sensor_smooth_cache[sensor_smooth_flag], (void *)&data_filter, sizeof(sensor_data_t));

    for (uint8_t element = 0; element < 6; element++)
    {
        for (uint8_t cache = 0; cache < SENSOR_SMOOTH_CACHE; cache++)
        {
            if (cache == sensor_smooth_flag)
            {
                *(&sensor_data_report.ax + element) += *(&sensor_smooth_cache[cache].ax + element) * 0.60f;
            }
            else
            {
                *(&sensor_data_report.ax + element) += *(&sensor_smooth_cache[cache].ax + element) * 0.08f;
            }
        }
    }

    sensor_smooth_flag++;
    if (sensor_smooth_flag == SENSOR_SMOOTH_CACHE)
    {
        sensor_smooth_flag = 0;
    }
}
#elif ORI_TYPE
static void Qmi8658_read_xyz(void)
{
    uint8_t buf_reg[12];

    Qmi8658_read_reg(Qmi8658Register_Ax_L, buf_reg, 12);
    sensor_data_report.ax = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    sensor_data_report.ay = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    sensor_data_report.az = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    sensor_data_report.gx = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));
    sensor_data_report.gy = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
    sensor_data_report.gz = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));
    // DEBUG_LOG_STRING("gyro %d %d %d acc %d %d %d \r\n",sensor_data_report.ax,sensor_data_report.ay,sensor_data_report.az,sensor_data_report.gx,sensor_data_report.gy,sensor_data_report.gz);
}
#endif

static void mouse_task(void)
{
    if (bt_check_le_connected() && mouse_on_off)
    {
        swtimer_restart(mouse_report_timernum);
    }
    else
        return;

    if (IPC_TxBufferIsEnough(4) && Bt_HciGetUsedBufferNum() < (HCI_QUEUE_MAX - 2))
    {
#if XY_TYPE
        sensor_data_smooth();
#elif ORI_TYPE
        Qmi8658_read_xyz();
#endif
        mouse_report_cb((void *)&sensor_data_report);
    }
}

static void mouse_init_handle(void)
{
    uint8_t qmi8658_chip_id = 0x00;

    if (mouse_report_timernum == 0xFF)
    {
        mouse_report_timernum = swtimer_add(mouse_task);
    }

#ifdef SOFTWARE_IIC
    while (IIC_CheckDevice(QMI8658_SLAVE_ADDR | DEVICE_SLAVE_READ))
    {
        DEBUG_LOG_STRING("IIC CHECK MOUSE FAIL \r\n");
    }
#endif
    Qmi8658_read_reg(Qmi8658Register_WhoAmI, &qmi8658_chip_id, 1);
    if (qmi8658_chip_id == 0x05)
    {
        DEBUG_LOG_STRING("chi id ok:%d \r\n", mouse_on_off);
        if (!mouse_on_off)
        {
            mouse_switch();
        }
        // else
        // {
        //     Qmi8658_power_off();
        // }
    }
    else
    {
        while (1)
        {
            DEBUG_LOG_STRING("Read Qmi8658 chip id fail: 0x%x \r\n", qmi8658_chip_id);
        }
    }

    DEBUG_LOG_STRING("mouse init done \r\n");
}

void mouse_turn_off(void)
{
    mouse_on_off = false;

    Qmi8658_power_off();
    DEBUG_LOG_STRING("MOUSE TURN OFF \r\n");
}

void mouse_off(void) {
    mouse_on_off = false;
}
void mouse_on(void) {
    mouse_on_off = true;
    app_sleep_timer_set(UNIT_TIME_1S);
    swtimer_start(mouse_report_timernum, MOUSE_REPORT_TIME_MAX, TIMER_START_ONCE);
    bt_enable_le_tx_md(); /// 锟斤拷锟斤拷锟斤拷模式 add 20230522
}
void mouse_turn_on(void)
{
    mouse_on_off = true;
    // DEBUG_LOG_STRING("MOUSE TURN ON:%d %d\r\n", mouse_on_delay, mouse_report_timernum);
    // if (mouse_on_delay || mouse_report_timernum == 0xFF)
    // {
    //     mouse_on_delay = false;
    //     swtimer_start(mouse_init_timernum, MOUSE_INIT_DELAY, TIMER_START_ONCE);
    // }
    // else
    {
#if XY_TYPE
        sensor_smooth_flag = 0;
        memset(&sensor_smooth_cache, 0, sizeof(sensor_smooth_cache));
#endif
         Qmi8658_power_on();
        app_sleep_timer_set(UNIT_TIME_1S);
        swtimer_start(mouse_report_timernum, MOUSE_REPORT_TIME_MAX, TIMER_START_ONCE);
        DEBUG_LOG_STRING("MOUSE TURN ON \r\n");
    }
}

void mouse_reinit(void)
{
#ifdef MOUSE_PWR_CS_HARDWARD_MODE
    GPIO_Init(MOUSE_PWR_PIN, GPIO_Mode_Out_High);
    GPIO_Init(MOUSE_CS_PIN, GPIO_Mode_Out_High);
#endif
    GPIO_Init(MOUSE_INT_PIN, GPIO_Mode_In_Down);

#ifdef HARDWARE_IIC
    GPIO_Init(IIC_SCL_PIN, GPIO_Mode_Iic_Scl);
    GPIO_Init(IIC_SDA_PIN, GPIO_Mode_Iic_Sda);
    IIC_Init(IIC_TYPE, iic_rate_100k, IIC_MRAM);
#endif

    mouse_on_off = false;
    mouse_on_delay = true;
}

void mouse_deinit(void)
{
#ifdef MOUSE_PWR_CS_HARDWARD_MODE
    GPIO_Init(MOUSE_CS_PIN, GPIO_Mode_Out_Low);
    GPIO_Init(MOUSE_PWR_PIN, GPIO_Mode_Out_Low);
#endif
    GPIO_Init(MOUSE_INT_PIN, GPIO_Mode_In_Down);
}

void mouse_init(mouse_report_cb_t cb)
{
    mouse_report_cb = cb;
#ifdef MOUSE_PWR_CS_HARDWARD_MODE
    GPIO_Init(MOUSE_PWR_PIN, GPIO_Mode_Out_High);
    GPIO_Init(MOUSE_CS_PIN, GPIO_Mode_Out_High);
#endif

    GPIO_Init(MOUSE_INT_PIN, GPIO_Mode_In_Down);

#ifdef HARDWARE_IIC
    GPIO_Init(IIC_SCL_PIN, GPIO_Mode_Iic_Scl);
    GPIO_Init(IIC_SDA_PIN, GPIO_Mode_Iic_Sda);
    IIC_Init(IIC_TYPE, iic_rate_100k, IIC_MRAM);
#endif

    mouse_init_timernum = swtimer_add(mouse_init_handle);
    swtimer_start(mouse_init_timernum, MOUSE_INIT_DELAY, TIMER_START_ONCE);
}
void check_mouse_chip_id(void)
{
    uint8_t qmi8658_chip_id = 0x00;
    Qmi8658_read_reg(Qmi8658Register_WhoAmI, &qmi8658_chip_id, 1);
    if (qmi8658_chip_id != 0x05)
    {
        while (1)
        {
            led_on(LED_1, 0, 0);
        }
    }
}
#define MOVING_INT_CNT_MAX (300 - 1)
static uint16_t int_cnt = 0;
static uint8_t moving_state = 1; /// 1-static 0-moving
uint8_t get_mouse_int_state(void)
{
    uint8_t res = 1;
    uint8_t state = GPIO_ReadDataBit(MOUSE_INT_PIN);
    if (state)
    {
        res = 0; /// OK
        moving_state = 0;
        int_cnt = 0;
    }
    else
    {
        if (++int_cnt > MOVING_INT_CNT_MAX)
        {
            int_cnt = 0;
            moving_state = 1;
            DEBUG_LOG_STRING("unmoving...\r\n");
        }
    }

    return res;
}

uint8_t get_axis_moving_state(void)
{
    return moving_state;
}
