#ifndef __QMA6100_H
#define __QMA6100_H


// #include "stm32f10x.h"
// #include "bsp_usart.h"
#include "yc11xx.h"
#define QST_USE_SW_I2C (1)
// #define QMA6100_STEP_INT	(1)
#if defined(QST_ACC_USE_SPI)
#include "bsp_spi.h"
#endif

#if defined(QST_USE_SW_I2C)
#include "software_i2c.h"
#else
#include "bsp_i2c.h"
#endif

#include <stdbool.h>
#include <string.h>

//typedef		void					void; 	
typedef		int8_t					qs8; 	
typedef		uint8_t					qu8; 
typedef		int16_t					qs16; 	
typedef		uint16_t				qu16; 	
typedef		int32_t					qs32; 	
typedef		uint32_t				qu32; 	
typedef		int64_t					qs64;
typedef		uint64_t				qu64;
// typedef		float					qf32;
// typedef		double					qd64;


#define QMA6100_LOG		printf
#define QMA6100_ERR		printf


//#define QMA6100_USE_CALI
//#define __WRITE_OFFSET_REG__
//#define QMA6100_QST_USE_SPI
//#define QMA6100_DATA_READY
//#define QMA6100_FIFO_FUNC

#define QMA6100_ANY_MOTION

//#define QMA6100_NO_MOTION
// #define QMA6100_SIGNIFICANT_MOTION
// #define QMA6100_STEPCOUNTER
// #define QMA6100_STEP_INT
//#define QMA6100_SIGNIFICANT_STEP_INT
//#define QMA6100_TAP_FUNC
//#define QMA6100_HAND_RAISE_DOWN
// #define QMA6100_INT_LATCH

//#define QMA6100_ODR_13
//#define QMA6100_ODR_25
#define QMA6100_ODR_50
//#define QMA6100_ODR_100



#define QMA6100_DEVICE_ID		    0xfa
#define QMA6100_I2C_SLAVE_ADDR		0x12	// AD0 GND 0x12, AD0 VDD 0x13
#define QMA6100_I2C_SLAVE_ADDR2		0x13	// AD0 GND 0x12, AD0 VDD 0x13
#define QMA6100_FAIL				0
#define QMA6100_SUCCESS				1

#define GRAVITY_EARTH_1000          9807	// about (9.80665f)*1000   mm/s2
#define QMA6100_ABS(X) 				((X) < 0 ? (-1 * (X)) : (X))

#define QMA6100_DELAY				0xff
/*Register Map*/
#define QMA6100_CHIP_ID		    	0x00
#define QMA6100_XOUTL				0x01
#define QMA6100_XOUTH				0x02
#define QMA6100_YOUTL				0x03
#define QMA6100_YOUTH				0x04
#define QMA6100_ZOUTL				0x05
#define QMA6100_ZOUTH				0x06
#define QMA6100_STEP_CNT_L			0x07
#define QMA6100_INT_STAT0			0x09
#define QMA6100_INT_STAT1			0x0a
#define QMA6100_INT_STAT2			0x0b
#define QMA6100_INT_STAT3			0x0c
#define QMA6100_STEP_CNT_M			0x0d
#define QMA6100_FIFO_STATE			0x0e
#define QMA6100_REG_RANGE			0x0f
#define QMA6100_REG_BW_ODR			0x10
#define QMA6100_REG_POWER_CTL		0x11
#define QMA6100_STEP_SAMPLE_CNT		0x12
#define QMA6100_STEP_PRECISION		0x13
#define QMA6100_STEP_TIME_LOW		0x14
#define QMA6100_STEP_TIME_UP		0x15
#define QMA6100_INTPIN_CFG			0x20
#define QMA6100_INT_CFG				0x21
#define QMA6100_OS_CUST_X		    0x27
#define QMA6100_OS_CUST_Y			0x28
#define QMA6100_OS_CUST_Z			0x29
/*ODR SET @lower ODR*/
#define QMA6981_ODR_7808HZ			0x04
#define QMA6981_ODR_3904HZ			0x03
#define QMA6981_ODR_1952HZ			0x02
#define QMA6981_ODR_976HZ			0x01
#define QMA6981_ODR_488HZ			0x00
#define QMA6981_ODR_244HZ			0x05
#define QMA6981_ODR_122HZ			0x06
#define QMA6981_ODR_61HZ			0x07

/* Accelerometer Sensor Full Scale */
#define QMA6100_RANGE_2G			0x01
#define QMA6100_RANGE_4G			0x02
#define QMA6100_RANGE_8G			0x04
#define QMA6100_RANGE_16G			0x08
#define QMA6100_RANGE_32G			0x0f


#define TAP_QUIET_TH			2//5  //8*31.25MG = 250MG
#define TAP_QUIET_TIME_30MS			(1<<7)
#define TAP_QUIET_TIME_20MS			(0<<7)
#define TAP_SHOCK_TIME_50MS			(1<<6)
#define TAP_SHOCK_TIME_75MS			(0<<6)
#define TAP_TRIPLE_WAIT_QUAD_DELAY_Y			(1<<5)
#define TAP_TRIPLE_NOT_WAIT_QUAD_DELAY_Y			(0<<5)
#define TAP_EARIN_Y						(1<<4)
#define TAP_EARIN_N						(0<<4)
#define TAP_DUR_100MS	(0)
#define TAP_DUR_150MS	(0x01)
#define TAP_DUR_200MS	(0x02)
#define TAP_DUR_250MS	(0x03)
#define TAP_DUR_300MS	(0x04)
#define TAP_DUR_400MS	(0x05)
#define TAP_DUR_500MS	(0x06)
#define TAP_DUR_700MS	(0x07)


#define TAP_AXIS_X		(0<<7)|(0<<6)
#define TAP_AXIS_Y		(0<<7)|(1<<6)
#define TAP_AXIS_Z		(1<<7)|(0<<6)
#define TAP_AXIS_XYZ		(1<<7)|(1<<6)
#define TAP_SHOCK_TH		0x05//0x08////0x0e  //0x14*31.25mg = 625mg


#define QFABS(x)		(((x)<0.0f)?(-1.0f*(x)):(x))

typedef enum
{
	QMA6100_DISABLE = 0,
	QMA6100_ENABLE = 1
}qma6100_enable;

typedef enum
{
	QMA6100_MODE_SLEEP,
	QMA6100_MODE_ACTIVE,
	QMA6100_MODE_MAX
}qma6100_power_mode;

typedef enum
{
	QMA6100_FIFO_MODE_NONE,
	QMA6100_FIFO_MODE_FIFO,
	QMA6100_FIFO_MODE_STREAM,
	QMA6100_FIFO_MODE_BYPASS,
	QMA6100_FIFO_MODE_MAX
}qma6100_fifo_mode;

typedef enum
{
	QMA6100_TAP_SINGLE = 0x80,
	QMA6100_TAP_DOUBLE = 0x20,
	QMA6100_TAP_TRIPLE = 0x10,
	QMA6100_TAP_QUARTER = 0x01,
	QMA6100_TAP_MAX = 0xff
}qma6100_tap;

typedef enum
{
	QMA6100_MAP_INT1,
	QMA6100_MAP_INT2,
	QMA6100_MAP_INT_NONE
}qma6100_int_map;

typedef enum
{
	QMA6100_MCLK_500K = 0x00,
	QMA6100_MCLK_333K = 0x01,
	QMA6100_MCLK_200K = 0x02,
	QMA6100_MCLK_100K = 0x03,
	QMA6100_MCLK_50K = 0x04,
	QMA6100_MCLK_20K = 0x05,
	QMA6100_MCLK_10K = 0x06,
	QMA6100_MCLK_5K = 0x07,
	QMA6100_MCLK_RESERVED = 0xff
}qma6100_mclk;

typedef enum
{
	QMA6100_DIV_512 = 0x00,
	QMA6100_DIV_256 = 0x01,
	QMA6100_DIV_128 = 0x02,
	QMA6100_DIV_64 = 0x03,
	QMA6100_DIV_32 = 0x04,
	QMA6100_DIV_1024 = 0x05,
	QMA6100_DIV_2048 = 0x06,
	QMA6100_DIV_4096 = 0x07,
	QMA6100_DIV_RESERVED = 0xff
}qma6100_div;

typedef enum
{
	QMA6100_LPF_0 = (0x00<<5),
	QMA6100_LPF_2 = (0x01<<5),
	QMA6100_LPF_4 = (0x02<<5),
	QMA6100_LPF_16 = (0x03<<5),
	QMA6100_LPF_RESERVED = 0xff
}qma6100_lpf;

typedef enum
{
  QMA6100_STEP_LPF_0 = (0x00<<6),
  QMA6100_STEP_LPF_2 = (0x01<<6),
  QMA6100_STEP_LPF_4 = (0x02<<6),
  QMA6100_STEP_LPF_8 = (0x03<<6),
  QMA6100_STEP_LPF_RESERVED = 0xff
}qma6100_step_lpf;

typedef enum
{
  QMA6100_STEP_AXIS_ALL = 0x00,
  QMA6100_STEP_AXIS_YZ = 0x01,
  QMA6100_STEP_AXIS_XZ = 0x02,
  QMA6100_STEP_AXIS_XY = 0x03,
  QMA6100_STEP_AXIS_RESERVED = 0xff
}qma6100_step_axis;

typedef enum
{
  QMA6100_STEP_START_0 = 0x00,
  QMA6100_STEP_START_4 = 0x20,
  QMA6100_STEP_START_8 = 0x40,
  QMA6100_STEP_START_12 = 0x60,
  QMA6100_STEP_START_16 = 0x80,
  QMA6100_STEP_START_24 = 0xa0,
  QMA6100_STEP_START_32 = 0xc0,
  QMA6100_STEP_START_40 = 0xe0,
  QMA6100_STEP_START_RESERVED = 0xff
}qma6100_step_start_cnt;

extern void qma_int_mode(void);
extern qs32 qma6100_writereg(qu8 reg_add,qu8 reg_dat);
extern qs32 qma6100_readreg(qu8 reg_add,qu8 *buf,qu16 num);
extern qu8 qma6100_chip_id(void);
extern qs32 qma6100_init(void);
extern qs32 qma6100_initialize(void);
extern qs32 qma6100_set_range(unsigned char range);
extern qs32 qma6100_set_mode_odr(qs32 mode, qs32 mclk, qs32 div, qs32 lpf);
extern qs32 qma6100_read_raw_xyz(qs32 *data);
extern int32_t qma6100_read_acc_xyz_1000mg(int32_t accData[3]);
#if defined(QMA6100_DATA_READY)
extern void qma6100_drdy_config(qs32 int_map, qs32 enable);
#endif
#if defined(QMA6100_FIFO_FUNC)
extern void qma6100_fifo_config(qs32 int_map, qs32 enable);
extern qs32 qma6100_read_fifo(qu8 *fifo_buf);
extern void qma6100_exe_fifo(qu8 *fifo_buf);
extern qu8* qma6100_get_fifo(void);
#endif

#if defined(QMA6100_STEPCOUNTER)
extern qu32 qma6100_read_stepcounter(void);
extern void qma6100_stepcounter_config(qs32 enable);
#if defined(QMA6100_STEP_INT)
extern void qma6100_step_int_config(qs32 int_map, qs32 enable);
#endif
#if defined(QMA6100_SIGNIFICANT_STEP_INT)
extern void qma6100_sigstep_int_config(qs32 int_map, qs32 enable);
#endif
#endif

#if defined(QMA6100_ANY_MOTION)
extern void qma6100_anymotion_config(qs32 int_map, qs32 enable);
#endif
#if defined(QMA6100_NO_MOTION)
extern void qma6100_nomotion_config(qs32 int_map, qs32 enable);
#endif

extern void qma6100_irq_hdlr(void);


#endif  /*QMX6981*/
