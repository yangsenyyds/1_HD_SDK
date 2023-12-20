/**
  ******************************************************************************
  * @file    qma6100.c
  * @author  Yangzhiqiang@qst
  * @version V1.0
  * @date    2020-5-27
  * @brief    qma6100
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 

#include "qma6100.h"
#include <math.h>
#include "yc11xx_systick.h"
#include "yc_debug.h"

// #ifdef SOFTWARE_IIC
#include "software_i2c.h"
// #else
// #include "yc11xx_iic.h"
// #endif
typedef struct
{
    qs16 sign[3];
    qu16 map[3];
}qst_convert;


typedef struct
{
	qu8					slave;
	qu8					chip_id;
	qs32				lsb_1g;
	qu8					layout;
	qst_convert			cvt;
	qma6100_fifo_mode	fifo_mode;
	qs32				fifo_len;	
	qs32				raw[3];
}qma6100_data;

static const qst_convert qst_map[] = 
{
    { { 1, 1, 1}, {0, 1, 2} },
    { {-1, 1, 1}, {1, 0, 2} },
    { {-1,-1, 1}, {0, 1, 2} },
    { { 1,-1, 1}, {1, 0, 2} },

    { {-1, 1, -1}, {0, 1, 2} },
    { { 1, 1, -1}, {1, 0, 2} },
    { { 1,-1, -1}, {0, 1, 2} },
    { {-1,-1, -1}, {1, 0, 2} }
};

#if defined(QMA6100_USE_CALI)
void qma6100_data_cali_1000mg(int32_t data[3]);
#endif

#define DEVICE_SLAVE_WRITE   (0x01)
#define DEVICE_SLAVE_READ  (0x00)
#define vTaskDelay(a) SysTick_DelayUs((a) * 1)
static qma6100_data g_qma6100;

static uint8_t sw_i2c_writereg(uint8_t slave_addr, uint8_t reg_addr, uint8_t reg_val)
{
    IIC_Start();
    IIC_SendByte(slave_addr << 1 | 0x00);
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_SendByte(reg_addr );
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_SendByte(reg_val);
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
    IIC_SendByte(slave_addr<< 1 | 0x00);
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
    IIC_SendByte(slave_addr <<1|0x01);
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

static uint8_t qst_sw_writereg(uint8_t SLAVE_ADDR ,uint8_t reg_addr, uint8_t reg_val)
{
    uint8_t ret = 1;
    uint16_t retry = 0;

    while ((ret) && (retry++ < 5))
    {
        ret = sw_i2c_writereg(SLAVE_ADDR, reg_addr, reg_val);
    }
    return ret;
}

static uint8_t qst_sw_readreg(uint8_t SLAVE_ADDR ,uint8_t reg_addr, uint8_t *read_buf, uint16_t len)
{
    uint8_t ret = 1;
    uint16_t retry = 0;

    while ((ret) && (retry++ < 5))
    {
        ret = sw_i2c_readreg(SLAVE_ADDR, reg_addr, read_buf, len);
    }
    return ret;
}

void qma6100_delay_ms(qu32 delay)
{
	vTaskDelay(delay);
}

qs32 qma6100_writereg(qu8 reg_add,qu8 reg_dat)
{
	qs32 ret = QMA6100_FAIL;
	qs32 ret_i2c = 0;
	qu32 retry = 0;

	while((ret==QMA6100_FAIL) && (retry++ < 5))
	{
#if defined(QST_USE_SPI)
		ret_i2c = (qs32)qst_acc_spi_write(reg_add, reg_dat);
#else
		#if defined(QST_USE_SW_I2C)
		// ret_i2c = (qs32)qst_sw_writereg(g_qma6100.slave<<1, reg_add, reg_dat);
		ret_i2c = (qs32)qst_sw_writereg(g_qma6100.slave, reg_add, reg_dat);
		#else
		ret_i2c = (qs32)I2C_ByteWrite(g_qma6100.slave<<1, reg_add, reg_dat);
		#endif
#endif
		if(ret_i2c == 1)
			return QMA6100_SUCCESS;
		else
			ret = QMA6100_FAIL;
	}

	return ret;
}

qs32 qma6100_readreg(qu8 reg_add,qu8 *buf,qu16 num)
{
	qs32 ret = QMA6100_FAIL;
	qs32 ret_i2c = 0;
	qu32 retry = 0;

	while((ret==QMA6100_FAIL) && (retry++ < 5))
	{
#if defined(QST_USE_SPI)
		ret_i2c = (qs32)qst_acc_spi_read(reg_add, buf, num);
#else
		#if defined(QST_USE_SW_I2C)
		ret_i2c =  (qs32)qst_sw_readreg(g_qma6100.slave, reg_add, buf, num);
		#else
		ret_i2c =  (qs32)I2C_BufferRead(g_qma6100.slave, reg_add, buf, (qu16)num);
		#endif
#endif
		if(ret_i2c == 1)
			return QMA6100_SUCCESS;
		else
			ret = QMA6100_FAIL;
	}

	return ret;
}

qu8 qma6100_chip_id()
{
	qu8 chip_id = 0x00;
	qs32 ret = QMA6100_FAIL;

	ret = qma6100_readreg(QMA6100_CHIP_ID, &chip_id, 1);
	DEBUG_LOG_STRING("qma6100_chip_id =0x%x ret=%d\n", chip_id, ret);

	return chip_id;
}

qs32 qma6100_set_range(qu8 range)
{
	qs32 ret = 0;

	if(range == QMA6100_RANGE_4G)
		g_qma6100.lsb_1g = 2048;
	else if(range == QMA6100_RANGE_8G)
		g_qma6100.lsb_1g = 1024;
	else if(range == QMA6100_RANGE_16G)
		g_qma6100.lsb_1g = 512;
	else if(range == QMA6100_RANGE_32G)
		g_qma6100.lsb_1g = 256;
	else
		g_qma6100.lsb_1g = 4096;

	
	ret = qma6100_writereg(QMA6100_REG_RANGE, range);

	return ret;
}

//BW<4:0> ODR ODR (MCLK = 500kHz)
//				BW			ODR
//xx000 MCLK/512		977 Hz		488.5
//xx001 MCLK/256		1953 Hz		976.5
//xx010 MCLK/128		3906 Hz		1953
//xx011 MCLK/64		7813 Hz		3906.5
//xx100 MCLK/32		15625 Hz		7812.5
//xx101 MCLK/1024		488 Hz		244
//xx110 MCLK/2048		244 Hz		122
//xx111 MCLK/4096		122 Hz		61
//Others MCLK/512		977 Hz		488.5S
qs32 qma6100_set_mode_odr(qs32 mode, qs32 mclk, qs32 div, qs32 lpf)
{
	qs32 ret = 0;
	qu8 mclk_reg = (qu8)mclk;
	qu8 odr_reg = (qu8)div;
	qu8 lpf_reg = (qu8)lpf;//|0x80;
#if 0
	qs32 mclk_f, div_f;

	if(mclk == QMA6100_MCLK_500K)
		mclk_f = 500*1000;
	else if(mclk == QMA6100_MCLK_333K)
		mclk_f = 333*1000;
	else if(mclk == QMA6100_MCLK_200K)
		mclk_f = 200*1000;
	else if(mclk == QMA6100_MCLK_100K)
		mclk_f = 100*1000;
	else if(mclk == QMA6100_MCLK_50K)
		mclk_f = 50*1000;
	else if(mclk == QMA6100_MCLK_20K)
		mclk_f = 20*1000;
	else if(mclk == QMA6100_MCLK_10K)
		mclk_f = 10*1000;
	else if(mclk == QMA6100_MCLK_5K)
		mclk_f = 5*1000;
	else 
		mclk_f = 500*1000;

	if(div == QMA6100_DIV_32)
		div_f = 32;
	else if(div == QMA6100_DIV_64)
		div_f = 64;
	else if(div == QMA6100_DIV_128)
		div_f = 128;
	else if(div == QMA6100_DIV_256)
		div_f = 256;
	else if(div == QMA6100_DIV_512)
		div_f = 512;
	else if(div == QMA6100_DIV_1024)
		div_f = 1024;
	else if(div == QMA6100_DIV_2048)
		div_f = 2048;
	else if(div == QMA6100_DIV_4096)
		div_f = 4096;
	else
		div_f = 2048;

	DEBUG_LOG_STRING("qma6100_set_odr MCLK:%dKHz DIV:%d BW:%d ODR:%d lpf:%d\n", 
				mclk_f/1000,div_f,mclk_f/div_f,mclk_f/div_f/2, lpf);
#endif
	if(mode >= QMA6100_MODE_ACTIVE)
	{
		ret = qma6100_writereg(QMA6100_REG_POWER_CTL, 0x80|mclk_reg);
		ret = qma6100_writereg(QMA6100_REG_BW_ODR, lpf_reg|odr_reg);
	}
	else
	{
		ret = qma6100_writereg(QMA6100_REG_POWER_CTL, 0x00);
	}

	return ret;
}


void qma6100_dump_reg(void)
{
	qu8 reg_data = 0;
	qs32 i=0;

	DEBUG_LOG_STRING("qma6100_dump_reg\n");
	for(i=0; i< 0xff; i++)
	{
		qma6100_readreg(i,&reg_data,1);
		DEBUG_LOG_STRING("0x%x = 0x%x\n", i, reg_data);
	}
}

#if defined(QMA6100_DATA_READY)
void qma6100_drdy_config(qs32 int_map, qs32 enable)
{
	qu8 reg_17 = 0;
	qu8 reg_1a = 0;
	qu8 reg_1c = 0;

	DEBUG_LOG_STRING("qma6100_drdy_config %d\n", enable);
	qma6100_readreg(0x17, &reg_17, 1);
	qma6100_readreg(0x1a, &reg_1a, 1);
	qma6100_readreg(0x1c, &reg_1c, 1);

	if(enable)
	{
		reg_17 |= 0x10;
		reg_1a |= 0x10;
		reg_1c |= 0x10;
		qma6100_writereg(0x17, reg_17);
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x1a, reg_1a);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1c, reg_1c);
	}
	else
	{
		reg_17 &= (~0x10);
		reg_1a &= (~0x10);
		reg_1c &= (~0x10);
		qma6100_writereg(0x17, reg_17);
		qma6100_writereg(0x1a, reg_1a);
		qma6100_writereg(0x1c, reg_1c);
	}

}
#endif

#if defined(QMA6100_FIFO_FUNC)
 qu8 qma6100_fifo_reg[64*6];

void qma6100_fifo_config(qs32 int_map, qs32 enable)
{
	qu8	reg_17, reg_1a, reg_1c=0;

	DEBUG_LOG_STRING("qma6100_fifo_config enable:%d\n", enable);
	qma6100_readreg(0x17, &reg_17, 1);
	qma6100_readreg(0x1a, &reg_1a, 1);
	qma6100_readreg(0x1c, &reg_1c, 1);

	if(enable)
	{
		g_qma6100.fifo_mode = QMA6100_FIFO_MODE_FIFO;
		if(g_qma6100.fifo_mode == QMA6100_FIFO_MODE_FIFO)
		{
			qma6100_writereg(0x31, 0x40);
			qma6100_writereg(0x3E, 0x47);	//bit[6:7] 0x00:BYPASS 0x40:FIFO 0x80:STREAM
			qma6100_writereg(0x17, reg_17|0x20);
			if(int_map == QMA6100_MAP_INT1)
				qma6100_writereg(0x1a, reg_1a|0x20);
			else if(int_map == QMA6100_MAP_INT2)
				qma6100_writereg(0x1c, reg_1c|0x20);
		}
		else if(g_qma6100.fifo_mode == QMA6100_FIFO_MODE_STREAM)
		{	
			qma6100_writereg(0x31, 0x20);	// 0x3f
			qma6100_writereg(0x3E, 0x87);	//bit[6:7] 0x00:BYPASS 0x40:FIFO 0x80:STREAM
			qma6100_writereg(0x17, reg_17|0x40);
			if(int_map == QMA6100_MAP_INT1)
				qma6100_writereg(0x1a, reg_1a|0x40);
			else if(int_map == QMA6100_MAP_INT2)
				qma6100_writereg(0x1c, reg_1c|0x40);
		}
		else if(g_qma6100.fifo_mode == QMA6100_FIFO_MODE_BYPASS)
		{
			qma6100_writereg(0x3E, 0x07);	//bit[6:7] 0x00:BYPASS 0x40:FIFO 0x80:STREAM
			qma6100_writereg(0x17, reg_17|0x20);
			if(int_map == QMA6100_MAP_INT1)
				qma6100_writereg(0x1a, reg_1a|0x20);
			else if(int_map == QMA6100_MAP_INT2)
				qma6100_writereg(0x1c, reg_1c|0x20);
		}
	}
	else
	{
		g_qma6100.fifo_mode = QMA6100_FIFO_MODE_NONE;
		reg_17 &= (~0x60);
		reg_1a &= (~0x60);
		reg_1c &= (~0x60);
		qma6100_writereg(0x17, reg_17);
		qma6100_writereg(0x1a, reg_1a);
		qma6100_writereg(0x1c, reg_1c);
	}
}

qs32 qma6100_read_fifo(qu8 *fifo_buf)
{
	qs32 ret = 0;
	qu8 databuf[2];

#if defined(QMA6100_INT_LATCH)
	ret = qma6100_readreg(QMA6100_INT_STAT2, databuf, 1);
#endif
	ret = qma6100_readreg(QMA6100_FIFO_STATE, databuf, 1);
	if(ret != QMA6100_SUCCESS)
	{
		QMA6100_ERR("qma6100_read_fifo state error\n");
		return ret;
	}
	g_qma6100.fifo_len = databuf[0]&0x7f;
	if(g_qma6100.fifo_len > 64)
	{
		QMA6100_ERR("qma6100_read_fifo depth(%d) error\n",g_qma6100.fifo_len);
		return QMA6100_FAIL;
	}
#if 1
	qma6100_readreg(0x3f, fifo_buf, g_qma6100.fifo_len*6);
#else
	for(int icount=0; icount<g_qma6100.fifo_len; icount++)
	{
		qma6100_readreg(0x3f, &fifo_buf[icount*6], 6);
	}
#endif
	if(g_qma6100.fifo_mode == QMA6100_FIFO_MODE_FIFO)
	{
		ret = qma6100_writereg(0x3e, 0x47);
	}
	else if(g_qma6100.fifo_mode == QMA6100_FIFO_MODE_STREAM)
	{
		ret = qma6100_writereg(0x3e, 0x87);
	}
	else if(g_qma6100.fifo_mode == QMA6100_FIFO_MODE_BYPASS)
	{
		ret = qma6100_writereg(0x3e, 0x07);
	}
// log fifo
	return ret;
}

void qma6100_exe_fifo(qu8 *fifo_buf)
{
	qs32 icount;	
	qs16 raw_data[3];
	int32_t accData[3];
	//float acc_data[3];

	//QMA6100_ERR("fifo_depth=%d\n", g_qma6100.fifo_len);
// log fifo
	for(icount=0; icount<g_qma6100.fifo_len; icount++)
	{
		raw_data[0]  = (qs16)(((qs16)(fifo_buf[1+icount*6]<<8)) |(fifo_buf[0+icount*6]));
		raw_data[1]  = (qs16)(((qs16)(fifo_buf[3+icount*6]<<8)) |(fifo_buf[2+icount*6]));
		raw_data[2]  = (qs16)(((qs16)(fifo_buf[5+icount*6]<<8)) |(fifo_buf[4+icount*6]));
		raw_data[0]  = raw_data[0]>>2;
		raw_data[1]  = raw_data[1]>>2;
		raw_data[2]  = raw_data[2]>>2;
		DEBUG_LOG_STRING("%d:%d	%d	%d	\r\n",icount,raw_data[0],raw_data[1],raw_data[2]);
		if((icount%4==0))
		{
			//DEBUG_LOG_STRING("\r\n");
		}

		accData[0] = (raw_data[0]*1000)/(g_qma6100.lsb_1g);			
		accData[1] = (raw_data[1]*1000)/(g_qma6100.lsb_1g);
		accData[2] = (raw_data[2]*1000)/(g_qma6100.lsb_1g);

	#if defined(QMA6100_USE_CALI)
		qma6100_data_cali_1000mg(accData);
	#endif
	DEBUG_LOG_STRING("cail_xyz = %d   %d	%d	\r\n",accData[0],accData[1],accData[2]);
	}	

}


void read_fifo_handle(void)
{
		qma6100_read_fifo(qma6100_fifo_reg);
		qma6100_exe_fifo(qma6100_fifo_reg);
}


#endif

#if defined(QMA6100_STEPCOUNTER)
qu32 qma6100_read_stepcounter(void)
{
	qu8 data[3];
	qu8 ret;
	qu32 step_num;
	qs32 step_dif;
	static qu32 step_last = 0;

	ret = qma6100_readreg(QMA6100_STEP_CNT_L, data, 2);	
	if(ret != QMA6100_SUCCESS)
	{
		step_num = step_last;
		return QMA6100_SUCCESS;
	}
	ret = qma6100_readreg(QMA6100_STEP_CNT_M, &data[2], 1);	
	if(ret != QMA6100_SUCCESS)
	{
		step_num = step_last;
		return QMA6100_SUCCESS;
	}
		
	step_num = (qu32)(((qu32)data[2]<<16)|((qu32)data[1]<<8)|data[0]);

#if 1//defined(QMA6100_CHECK_ABNORMAL_DATA)
	step_dif = (qs32)(step_num-step_last);
	if(QMA6100_ABS(step_dif) > 100)
	{
		qu32 step_num_temp[3];

		ret = qma6100_readreg(QMA6100_STEP_CNT_L, data, 2);	
		ret = qma6100_readreg(QMA6100_STEP_CNT_M, &data[2], 1);
		step_num_temp[0] = (qu32)(((qu32)data[2]<<16)|((qu32)data[1]<<8)|data[0]);
		qma6100_delay_ms(2);
		
		ret = qma6100_readreg(QMA6100_STEP_CNT_L, data, 2);	
		ret = qma6100_readreg(QMA6100_STEP_CNT_M, &data[2], 1);
		step_num_temp[1] = (qu32)(((qu32)data[2]<<16)|((qu32)data[1]<<8)|data[0]);
		qma6100_delay_ms(2);
		
		ret = qma6100_readreg(QMA6100_STEP_CNT_L, data, 2);	
		ret = qma6100_readreg(QMA6100_STEP_CNT_M, &data[2], 1);
		step_num_temp[2] = (qu32)(((qu32)data[2]<<16)|((qu32)data[1]<<8)|data[0]);
		qma6100_delay_ms(2);
		if((step_num_temp[0]==step_num_temp[1])&&(step_num_temp[1]==step_num_temp[2]))
		{
			DEBUG_LOG_STRING("qma6100 check data, confirm!\n");
			step_num = step_num_temp[0];
		}
		else
		{	
			DEBUG_LOG_STRING("qma6100 check data, abnormal!\n");
			return step_last;
		}
	}
#endif
	step_last = step_num;

	return step_num;
}



void qma6100_stepcounter_config(qs32 enable)
{ 
//  qs32 odr = 27;   // 100Hz
  qu8 reg_12 = 0x00;
  qu8 reg_14 = 0x00;
  qu8 reg_15 = 0x00;
  qu8 reg_1e = 0x00;

  qma6100_writereg(0x13, 0x80);
  qma6100_delay_ms(1);
  qma6100_writereg(0x13, 0x7f);     //  0x7f(P2P/16), 0.977*16*LSB
  // Windows time
  if(enable)
  {
#if defined(QMA6100_ODR_25)
    reg_12 = 0x84;
    reg_14 = 0x05;      //((200*odr)/(1000));      // about:200 ms
    reg_15 = 0x07;      //(((2200/8)*odr)/1000);   // 2000 ms
#elif defined(QMA6100_ODR_50)
    reg_12 = 0x89;
    reg_14 = 0x0B;      //((200*odr)/(1000)); 
    reg_15 = 0x0e;      //(((2200/8)*odr)/1000);   // 2000 ms
#elif defined(QMA6100_ODR_100)
    reg_12 = 0x8f;
    reg_14 = 0x1f;      //((280*odr)/(1000));      // about:280 ms
    reg_15 = 0x1c;   //0x19   //(((2200/8)*odr)/1000);   // 2000 ms
#endif
    DEBUG_LOG_STRING("step time config 0x14=0x%x 0x15=0x%x\n", reg_14, reg_15);
    qma6100_writereg(0x12, reg_12);
    qma6100_writereg(0x14, reg_14);
    qma6100_writereg(0x15, reg_15);
    // lpf
    qma6100_readreg(0x1e, &reg_1e, 1);
    reg_1e &= 0x3f;
    qma6100_writereg(0x1e, (qu8)(reg_1e|QMA6100_STEP_LPF_0));   // default 0x08
    // start count, p2p, fix peak
    qma6100_writereg(0x1f, (qu8)QMA6100_STEP_START_24|0x00);    // 0x10

#if defined(QMA6100_STEP_USE_2_AXIS)
	qma6100_writereg(0x32, 0x02);    // 0x02 -> only use xz  //STEP_BP_AXIS = 11  xy axis;  10 =  xz;   01 =  yz;
	//qma6100_writereg(0x32, 0x01);    // 0x01 -> only use yz
#endif

  }
}


 void qma6100_clear_step(void)
{
	qma6100_writereg(0x13, 0x80);		// clear step	
	qma6100_delay_ms(10);
	qma6100_writereg(0x13, 0x80);		// clear step
	qma6100_delay_ms(10);
	qma6100_writereg(0x13, 0x80);		// clear step
	qma6100_delay_ms(10);
	qma6100_writereg(0x13, 0x80);		// clear step		
	qma6100_writereg(0x13, 0x7f);		// clear step		
}



#if defined(QMA6100_STEP_INT)
void qma6100_step_int_config(qs32 int_map, qs32 enable)
{
	qu8	reg_16=0;
	qu8	reg_19=0;
	qu8	reg_1b=0;

	qma6100_readreg(0x16, &reg_16, 1);
	qma6100_readreg(0x19, &reg_19, 1);
	qma6100_readreg(0x1b, &reg_1b, 1);
	if(enable)
	{
		reg_16 |= 0x08;
		reg_19 |= 0x08;
		reg_1b |= 0x08;
		qma6100_writereg(0x16, reg_16);
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x19, reg_19);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1b, reg_1b);
	}
	else
	{
		reg_16 &= (~0x08);
		reg_19 &= (~0x08);
		reg_1b &= (~0x08);

		qma6100_writereg(0x16, reg_16);
		qma6100_writereg(0x19, reg_19);
		qma6100_writereg(0x1b, reg_1b);
	}
}
#endif

#if defined(QMA6100_SIGNIFICANT_STEP_INT)
void qma6100_sigstep_int_config(qs32 int_map, qs32 enable)
{
	qu8	reg_16=0;
	qu8	reg_19=0;
	qu8	reg_1b=0;

	qma6100_readreg(0x16, &reg_16, 1);
	qma6100_readreg(0x19, &reg_19, 1);
	qma6100_readreg(0x1b, &reg_1b, 1);
	
	qma6100_writereg(0x1d, 0x1a);
	if(enable)
	{
		reg_16 |= 0x40;
		reg_19 |= 0x40;
		reg_1b |= 0x40;
		qma6100_writereg(0x16, reg_16);
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x19, reg_19);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1b, reg_1b);
	}
	else
	{
		reg_16 &= (~0x40);
		reg_19 &= (~0x40);
		reg_1b &= (~0x40);

		qma6100_writereg(0x16, reg_16);
		qma6100_writereg(0x19, reg_19);
		qma6100_writereg(0x1b, reg_1b);
	}
}
#endif


#endif

#if defined(QMA6100_ANY_MOTION)
void qma6100_anymotion_config(qs32 int_map, qs32 enable)
{
	qu8 reg_0x18 = 0;
	qu8 reg_0x1a = 0;
	qu8 reg_0x1c = 0;
	qu8 reg_0x2c = 0;
#if defined(QMA6100_SIGNIFICANT_MOTION)
	qu8 reg_0x19 = 0;
	qu8 reg_0x1b = 0;
#endif

	DEBUG_LOG_STRING("qma6100_anymotion_config %d\n", enable);
	qma6100_readreg(0x18, &reg_0x18, 1);
	qma6100_readreg(0x1a, &reg_0x1a, 1);
	qma6100_readreg(0x1c, &reg_0x1c, 1);
	qma6100_readreg(0x2c, &reg_0x2c, 1);
	reg_0x2c |= 0x00;

	qma6100_writereg(0x2c, reg_0x2c);
	qma6100_writereg(0x2e, 0x09);		// 0.488*16*32 = 250mg
	// add by yang, tep counter, raise wake, and tap detector,any motion by pass LPF
	//qma6100_writereg(0x30, 0x80|0x3f);	// default 0x3f
	// add by yang, tep counter, raise wake, and tap detector,any motion by pass LPF
	if(enable)
	{
		reg_0x18 |= 0x07;
		reg_0x1a |= 0x01;
		reg_0x1c |= 0x01;

		qma6100_writereg(0x18, reg_0x18);	// enable any motion
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x1a, reg_0x1a);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1c, reg_0x1c);
	}
	else
	{
		reg_0x18 &= (~0x07);
		reg_0x1a &= (~0x01);
		reg_0x1c &= (~0x01);
		
		qma6100_writereg(0x18, reg_0x18);
		qma6100_writereg(0x1a, reg_0x1a);
		qma6100_writereg(0x1c, reg_0x1c);
	}
	
#if defined(QMA6100_SIGNIFICANT_MOTION)
	qma6100_readreg(0x19, &reg_0x19, 1);
	qma6100_readreg(0x1b, &reg_0x1b, 1);
	
	qma6100_writereg(0x2f, 0x01);		// bit0: selecat significant motion
	if(enable)
	{
		reg_0x19 |= 0x01;
		reg_0x1b |= 0x01;
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x19, reg_0x19);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1b, reg_0x1b);
	}
	else
	{
		reg_0x19 &= (~0x01);
		reg_0x1b &= (~0x01);
		qma6100_writereg(0x19, reg_0x19);
		qma6100_writereg(0x1b, reg_0x1b);
	}
#endif	
}
#endif

#if defined(QMA6100_NO_MOTION)
void qma6100_nomotion_config(qs32 int_map, qs32 enable)
{
	qu8 reg_0x18 = 0;
	qu8 reg_0x1a = 0;
	qu8 reg_0x1c = 0;
	qu8 reg_0x2c = 0;

	DEBUG_LOG_STRING("qma6100_nomotion_config %d\n", enable);

	qma6100_readreg(0x18, &reg_0x18, 1);
	qma6100_readreg(0x1a, &reg_0x1a, 1);
	qma6100_readreg(0x1c, &reg_0x1c, 1);
	qma6100_readreg(0x2c, &reg_0x2c, 1);
	reg_0x2c |= 0x24;		// 10s
	//reg_0x2c |= 0xc0; 		// 100s

	qma6100_writereg(0x2c, reg_0x2c);
	qma6100_writereg(0x2d, 0x14);
	if(enable)
	{
		reg_0x18 |= 0xe0;
		reg_0x1a |= 0x80;
		reg_0x1c |= 0x80;		
		qma6100_writereg(0x18, reg_0x18);
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x1a, reg_0x1a);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1c, reg_0x1c);
	}
	else
	{
		reg_0x18 &= (~0xe0);
		reg_0x1a &= (~0x80);
		reg_0x1c &= (~0x80);
		
		qma6100_writereg(0x18, reg_0x18);
		qma6100_writereg(0x1a, reg_0x1a);
		qma6100_writereg(0x1c, reg_0x1c);
	}

}
#endif

#if defined(QMA6100_TAP_FUNC)
void qma6100_tap_config(qs32 tap_type, qs32 int_map, qs32 enable)
{
	qu8 reg_16,reg_19,reg_1a,reg_1b,reg_1c;
	qu8 tap_reg = (qu8)tap_type;

	qma6100_readreg(0x16, &reg_16, 1);
	qma6100_readreg(0x19, &reg_19, 1);
	qma6100_readreg(0x1a, &reg_1a, 1);
	qma6100_readreg(0x1b, &reg_1b, 1);
	qma6100_readreg(0x1c, &reg_1c, 1);

	//qma6100_writereg(0x1e, 0x03);		// TAP_QUIET_TH 31.25*8 = 250mg 
	//qma6100_writereg(0x2a, 0x43);		// tap config1
	//qma6100_writereg(0x2b, (0xc0+6));		// tap config2
	// add by yang, tep counter, raise wake, and tap detector,any motion by pass LPF
	//qma6100_writereg(0x30, 0x80|0x40|0x3f);	// default 0x3f
	// add by yang, tep counter, raise wake, and tap detector,any motion by pass LPF

	qma6100_writereg(0x1e, TAP_QUIET_TH);		// TAP_QUIET_TH 31.25*8 = 250mg 
	/*single tap param*/
	qma6100_writereg(0x2a, TAP_QUIET_TIME_20MS|TAP_SHOCK_TIME_50MS|TAP_TRIPLE_NOT_WAIT_QUAD_DELAY_Y|TAP_EARIN_N|TAP_DUR_500MS);
	//qma6100_writereg(0x2a, TAP_QUIET_TIME_30MS|TAP_SHOCK_TIME_75MS|TAP_TRIPLE_NOT_WAIT_QUAD_DELAY_Y|TAP_EARIN_N|TAP_DUR_200MS);// tap config1
	qma6100_writereg(0x2b, TAP_AXIS_Z|TAP_SHOCK_TH);		// tap config2

	if(enable)
	{
		reg_16 |= tap_reg;
		qma6100_writereg(0x16, reg_16);

		if(int_map == QMA6100_MAP_INT1)
		{
			if(tap_type & QMA6100_TAP_QUARTER)
			{
				reg_1a |= 0x02;
				qma6100_writereg(0x1a, reg_1a);
			}

			reg_19 |= tap_reg;
			qma6100_writereg(0x19, reg_19);

		}
		else if(int_map == QMA6100_MAP_INT2)
		{
			if(tap_type & QMA6100_TAP_QUARTER)
			{
				reg_1c |= 0x02;
				qma6100_writereg(0x1c, reg_1c);
			}

			reg_1b |= tap_reg;
			qma6100_writereg(0x1b, reg_1b);
		}
	}
	else
	{	
		reg_16 &= (~tap_reg);
		qma6100_writereg(0x16, reg_16);

		if(int_map == QMA6100_MAP_INT1)
		{
			if(tap_type &QMA6100_TAP_QUARTER)
			{
				reg_1a &= (~0x02);
				qma6100_writereg(0x1a, reg_1a);
			}

			reg_19 &= (~tap_reg);
			qma6100_writereg(0x19, reg_19);

		}
		else if(int_map == QMA6100_MAP_INT2)
		{
			if(tap_type & QMA6100_TAP_QUARTER)
			{
				reg_1c &= (~0x02);
				qma6100_writereg(0x1c, reg_1c);
			}

			reg_1b &= (~tap_reg);
			qma6100_writereg(0x1b, reg_1b);

		}
	}
}
#endif

#if defined(QMA6100_HAND_RAISE_DOWN)
void qma6100_set_hand_up_down(int layout)
{
#if 1//defined(QMA7981_SWAP_XY)
	unsigned char reg_0x42 = 0;
#endif
	unsigned char reg_0x1e = 0;
	unsigned char reg_0x34 = 0;
	unsigned char yz_th_sel = 4;
	char y_th = -3; //-2;				// -16 ~ 15
	unsigned char x_th = 6;		// 0--7.5
	char z_th = 6;				// -8--7

#if 1//defined(QMA7981_SWAP_XY)	// swap xy
	if(layout%2)
	{
		qma6100_readreg(0x42, &reg_0x42, 1);
		reg_0x42 |= 0x80;		// 0x42 bit 7 swap x and y
		qma6100_writereg(0x42, reg_0x42);
	}
#endif

	if((layout >=0) && (layout<=3))
	{
		z_th = 3;
		if((layout == 2)||(layout == 3))
			y_th = 3; 
		else if((layout == 0)||(layout == 1))	
			y_th = -3;
	}
	else if((layout >=4) && (layout<=7))
	{
		z_th = -3;
		
		if((layout == 6)||(layout == 7))
			y_th = 3; 
		else if((layout == 4)||(layout == 5))	
			y_th = -3;
	}

	// 0x34 YZ_TH_SEL[7:5]	Y_TH[4:0], default 0x9d  (YZ_TH_SEL   4   9.0 m/s2 | Y_TH  -3  -3 m/s2)
	//qmaX981_writereg(0x34, 0x9d);	//|yz|>8 m/s2, y>-3 m/m2
	if((y_th&0x80))
	{
		reg_0x34 |= yz_th_sel<<5;
		reg_0x34 |= (y_th&0x0f)|0x10;
		qma6100_writereg(0x34, reg_0x34);
	}
	else
	{	
		reg_0x34 |= yz_th_sel<<5;
		reg_0x34 |= y_th;
		qma6100_writereg(0x34, reg_0x34);	//|yz|>8m/s2, y<3 m/m2
	}
	//Z_TH<7:4>: -8~7, LSB 1 (unit : m/s2)	X_TH<3:0>: 0~7.5, LSB 0.5 (unit : m/s2) 
	//qmaX981_writereg(0x1e, 0x68);	//6 m/s2, 4 m/m2

	qma6100_writereg(0x22, (0x19|(0x03<<6)));			// 12m/s2 , 0.5m/s2
	qma6100_writereg(0x23, (0x7c|(0x03>>2)));
	//qmaX981_writereg(0x2a, (0x19|(0x02<<6)));			// 12m/s2 , 0.5m/s2
	//qmaX981_writereg(0x2b, (0x7c|(0x02)));
	qma6100_writereg(0x25, 0x0e);		  //50	
	//qmaX981_readreg(0x1e, &reg_0x1e, 1);
	if((z_th&0x80))
	{
		reg_0x1e |= (x_th&0x0f);
		reg_0x1e |= ((z_th<<4)|0x80);
		qma6100_writereg(0x35, reg_0x1e);
	}
	else
	{
		reg_0x1e |= (x_th&0x0f);
		reg_0x1e |= (z_th<<4);
		qma6100_writereg(0x35, reg_0x1e);
	}
}
void qma6100_hand_raise_down(qs32 int_map, qs32 enable)
{
	qu8 reg_16,reg_19,reg_1b;

	qma6100_set_hand_up_down(0);
	
	qma6100_readreg(0x16, &reg_16, 1);
	qma6100_readreg(0x19, &reg_19, 1);
	qma6100_readreg(0x1b, &reg_1b, 1);

	// 0x24: RAISE_WAKE_TIMEOUT_TH<7:0>: Raise_wake_timeout_th[11:0] * ODR period = timeout count
	// 0x25: RAISE_WAKE_PERIOD<7:0>: Raise_wake_period[10:0] * ODR period = wake count
	// 0x26:
	// RAISE_MODE: 0:raise wake function, 1:ear-in function
	// RAISE_WAKE_PERIOD<10:8>: Raise_wake_period[10:0] * ODR period = wake count
	// RAISE_WAKE_TIMEOUT_TH<11:8>: Raise_wake_timeout_th[11:0] * ODR period = timeout count

	if(enable)
	{
		reg_16 |= (0x02|0x04);
		reg_19 |= (0x02|0x04);
		reg_1b |= (0x02|0x04);
		qma6100_writereg(0x16, reg_16);
		if(int_map == QMA6100_MAP_INT1)
			qma6100_writereg(0x19, reg_19);
		else if(int_map == QMA6100_MAP_INT2)
			qma6100_writereg(0x1b, reg_1b);
	}
	else
	{
		reg_16 &= ~((0x02|0x04));
		reg_19 &= ~((0x02|0x04));
		reg_1b &= ~((0x02|0x04));
		qma6100_writereg(0x16, reg_16);
		qma6100_writereg(0x19, reg_19);
		qma6100_writereg(0x1b, reg_1b);
	}
}
#endif

void qma6100_irq_hdlr(void)
{
	qu8 ret = QMA6100_FAIL;
	qu8 databuf[4];
	qs32 retry = 0;

	while((ret==QMA6100_FAIL)&&(retry++<10))
	{
		ret = qma6100_readreg(QMA6100_INT_STAT0, databuf, 4);
		if(ret == QMA6100_SUCCESS)
		{
			break;
		}
	}
	if(ret == QMA6100_FAIL)
	{
		DEBUG_LOG_STRING("qma6100_irq_hdlr read status fail!\n");
		return;
	}
	else
	{
		DEBUG_LOG_STRING("irq [0x%x 0x%x 0x%x 0x%x]\n", databuf[0],databuf[1],databuf[2],databuf[3]);
	}

#if defined(QMA6100_DATA_READY)
	if(databuf[2]&0x10)
	{
		qma6100_read_raw_xyz(g_qma6100.raw);
		DEBUG_LOG_STRING("drdy	%d	%d	%d\n",g_qma6100.raw[0],g_qma6100.raw[1],g_qma6100.raw[2]);
	}
#endif
#if defined(QMA6100_FIFO_FUNC)
	if(databuf[2]&0x20)
	{
		DEBUG_LOG_STRING("FIFO FULL\n");
		qma6100_read_fifo(qma6100_fifo_reg);
		qma6100_exe_fifo(qma6100_fifo_reg);
	}
	if(databuf[2]&0x40)
	{
		//DEBUG_LOG_STRING("FIFO WMK\n");
		qma6100_read_fifo(qma6100_fifo_reg);
		qma6100_exe_fifo(qma6100_fifo_reg);
	}
#endif
#if defined(QMA6100_ANY_MOTION)
	if(databuf[0]&0x07)
	{
		DEBUG_LOG_STRING("any motion!\n");
	}
#if defined(QMA6100_SIGNIFICANT_MOTION)
	if(databuf[1]&0x01)
	{
		DEBUG_LOG_STRING("significant motion!\n");
	}
#endif
#endif
#if defined(QMA6100_NO_MOTION)
	if(databuf[0]&0x80)
	{
		DEBUG_LOG_STRING("no motion!\n");
	}
#endif
#if defined(QMA6100_STEP_INT)
	if(databuf[1]&0x08)
	{
		DEBUG_LOG_STRING("step int!\n");
	}
#endif
#if defined(QMA6100_SIGNIFICANT_STEP_INT)
	if(databuf[1]&0x40)
	{
		DEBUG_LOG_STRING("significant step int!\n");
	}
#endif
#if defined(QMA6100_TAP_FUNC)
	if(databuf[1]&0x80)
	{
		DEBUG_LOG_STRING("SINGLE tap int!\n");
	}
	if(databuf[1]&0x20)
	{
		DEBUG_LOG_STRING("DOUBLE tap int!\n");
	}
	if(databuf[1]&0x10)
	{
		DEBUG_LOG_STRING("TRIPLE tap int!\n");
	}	
	if(databuf[2]&0x01)
	{
		DEBUG_LOG_STRING("QUARTER tap int!\n");
	}
#endif
#if defined(QMA6100_HAND_RAISE_DOWN)
	if(databuf[1]&0x02)
	{
		DEBUG_LOG_STRING("hand raise!\n");
	}
	if(databuf[1]&0x04)
	{
		DEBUG_LOG_STRING("hand down!\n");
	}
#endif
}

#ifdef __UNIT_M_S2__
#if defined(QMA6100_USE_CALI)
#define QMI8658_CALI_DATA_NUM 20
float acc_last[3],acc_bias[3];
float acc_sum[3]={0.0f};
uint8_t acc_cali_num= 0,acc_cali_flag = 0;
void qma6100_data_cali(float data[3])
{
	float data_diff[3];


	data_diff[0] = QFABS((data[0]-acc_last[0]));
	data_diff[1] = QFABS((data[1]-acc_last[1]));
	data_diff[2] = QFABS((data[2]-acc_last[2]));
	acc_last[0] = data[0];
	acc_last[1] = data[1];
	acc_last[2] = data[2];

	if((data_diff[0]+data_diff[1]+data_diff[2]) < 0.5f)
	{
		if((acc_cali_flag == 0)&&(data[2]<15.0f)&&(data[2]>5.0f)&&(data[0]<2)&&(data[1]<2))
		{
			if(acc_cali_num == 0)
			{
				acc_sum[0] = 0.0f;
				acc_sum[1] = 0.0f;
				acc_sum[2] = 0.0f;
			}
			if(acc_cali_num < QMI8658_CALI_DATA_NUM)
			{
				acc_cali_num++;
				acc_sum[0] += data[0];
				acc_sum[1] += data[1];
				acc_sum[2] += data[2];
				if(acc_cali_num == QMI8658_CALI_DATA_NUM)
				{
					acc_sum[0] = acc_sum[0]/QMI8658_CALI_DATA_NUM;
					acc_sum[1] = acc_sum[1]/QMI8658_CALI_DATA_NUM;
					acc_sum[2] = acc_sum[2]/QMI8658_CALI_DATA_NUM;

					acc_bias[0] = 0.0f - acc_sum[0];
					acc_bias[1] = 0.0f - acc_sum[1];
					acc_bias[2] = 9.807f - acc_sum[2];
					acc_cali_flag = 1;
				}
			}
		}

	}
	else
	{
		acc_cali_num = 0;
		acc_sum[0] = 0.0f;
		acc_sum[1] = 0.0f;
		acc_sum[2] = 0.0f;
		acc_cali_flag = 0;
	}


	{
		data[0] = data[0] + acc_bias[0];
		data[1] = data[1] + acc_bias[1];
		data[2] = data[2] + acc_bias[2];

	}


}
#endif
#endif

#if defined(QMA6100_USE_CALI)
#define GAVITY_1G 				1000
#define QMI8658_CALI_DATA_NUM 20
int32_t acc_last[3],acc_bias[3];
int32_t acc_sum[3]={0};
uint8_t acc_cali_num= 0,acc_cali_flag = 0;


#ifdef __WRITE_OFFSET_REG__
uint8_t  write_offset_reg_flag = 0;
void qma6100_write_offset_reg(int8_t offset_data[3])
{
	qma6100_writereg(0x27, offset_data[0]);
	qma6100_writereg(0x28, offset_data[1]);
	qma6100_writereg(0x29, offset_data[2]);
}

void qma6100_clear_offset_reg(void)
{
	qma6100_writereg(0x27, 0);
	qma6100_writereg(0x28, 0);
	qma6100_writereg(0x29, 0);
}

#endif

void qma6100_data_cali_1000mg(int32_t data[3])
{
	int32_t data_diff[3];
	uint8_t databuf[3];
	int8_t temp_offset[3];


	data_diff[0] = QMA6100_ABS((data[0]-acc_last[0]));
	data_diff[1] = QMA6100_ABS((data[1]-acc_last[1]));
	data_diff[2] = QMA6100_ABS((data[2]-acc_last[2]));
	acc_last[0] = data[0];
	acc_last[1] = data[1];
	acc_last[2] = data[2];
	
	if((data_diff[0]+data_diff[1]+data_diff[2]) < 50)
	{
		if((acc_cali_flag == 0)&&(QMA6100_ABS(data[2])<1700)&&(QMA6100_ABS(data[2])>300)
			&&(QMA6100_ABS(data[0])<200)&&(QMA6100_ABS(data[1])<200))
		{
			
			if(acc_cali_num == 0)
			{
				acc_sum[0] = 0;
				acc_sum[1] = 0;
				acc_sum[2] = 0;
			}
			if(acc_cali_num < QMI8658_CALI_DATA_NUM)
			{
				acc_cali_num++;
				acc_sum[0] += data[0];
				acc_sum[1] += data[1];
				acc_sum[2] += data[2];
				if(acc_cali_num == QMI8658_CALI_DATA_NUM)
				{
					acc_sum[0] = acc_sum[0]/QMI8658_CALI_DATA_NUM;
					acc_sum[1] = acc_sum[1]/QMI8658_CALI_DATA_NUM;
					acc_sum[2] = acc_sum[2]/QMI8658_CALI_DATA_NUM;

					acc_bias[0] = acc_sum[0];
					acc_bias[1] = acc_sum[1];
					acc_bias[2] = acc_sum[2] - GAVITY_1G;
					acc_cali_flag = 1;
					DEBUG_LOG_STRING("acc_bias = %d ,%d,%d\n",acc_bias[0],acc_bias[1],acc_bias[2]);
				#ifdef __WRITE_OFFSET_REG__
					if (!write_offset_reg_flag)
					{
						temp_offset[0] = (int8_t)(acc_bias[0]/((int32_t)((4096*39)/(g_qma6100.lsb_1g*10))));
						temp_offset[1] = (int8_t)(acc_bias[1]/((int32_t)((4096*39)/(g_qma6100.lsb_1g*10))));
						temp_offset[2] = (int8_t)(acc_bias[2]/((int32_t)((4096*39)/(g_qma6100.lsb_1g*10))));
						DEBUG_LOG_STRING("temp_offset = %d, %d, %d\n",temp_offset[0],temp_offset[1],temp_offset[2]);
						qma6100_write_offset_reg(temp_offset);
						write_offset_reg_flag = 1;
					}
				#endif
				}
			}
		}
		else
		{
				acc_cali_num = 0;
				acc_sum[0] = 0;
				acc_sum[1] = 0;
				acc_sum[2] = 0;
				//acc_cali_flag = 0;
		}

	}
	else
	{
		acc_cali_num = 0;
		acc_sum[0] = 0;
		acc_sum[1] = 0;
		acc_sum[2] = 0;
		//acc_cali_flag = 0;
	}

	#ifndef __WRITE_OFFSET_REG__
		data[0] = data[0] - acc_bias[0];
		data[1] = data[1] - acc_bias[1];
		data[2] = data[2] - acc_bias[2];
	#endif
}
#endif



qs32 qma6100_read_raw_xyz(qs32 *data)
{
	qu8 databuf[6] = {0}; 	
	qs16 raw_data[3];	
	qs32 ret;

	ret = qma6100_readreg(QMA6100_XOUTL, databuf, 6);
	if(ret == QMA6100_FAIL){
		DEBUG_LOG_STRING("read xyz error!!!");
		return QMA6100_FAIL;	
	}

	raw_data[0] = (qs16)(((qs16)(databuf[1]<<8))|(databuf[0]));
	raw_data[1] = (qs16)(((qs16)(databuf[3]<<8))|(databuf[2]));
	raw_data[2] = (qs16)(((qs16)(databuf[5]<<8))|(databuf[4]));
	data[0] = raw_data[0]>>2;
	data[1] = raw_data[1]>>2;
	data[2] = raw_data[2]>>2;

	return QMA6100_SUCCESS;
}


#ifdef __UNIT_M_S2__
qs32 qma6100_read_acc_xyz_m_s2(float accData[3])
{
	qs32 ret;
	qs32 rawData[3];
	qs32 rawData_temp[3];

	ret = qma6100_read_raw_xyz(rawData);
	if(ret == QMA6100_SUCCESS)
	{
		rawData_temp[g_qma6100.cvt.map[0]] = g_qma6100.cvt.sign[0]*rawData[0];
		rawData_temp[g_qma6100.cvt.map[1]] = g_qma6100.cvt.sign[1]*rawData[1];
		rawData_temp[g_qma6100.cvt.map[2]] = g_qma6100.cvt.sign[2]*rawData[2];

		accData[0] = (float)(rawData_temp[0]*9.807f)/(g_qma6100.lsb_1g);			
		accData[1] = (float)(rawData_temp[1]*9.807f)/(g_qma6100.lsb_1g);
		accData[2] = (float)(rawData_temp[2]*9.807f)/(g_qma6100.lsb_1g);
	}

	#if defined(QMA6100_USE_CALI)
	qma6100_data_cali(accData);
	#endif
	return ret;

}
#endif

int32_t qma6100_read_acc_xyz_1000mg(int32_t accData[3])
{
	int32_t ret;
	int32_t rawData[3];
	int32_t rawData_temp[3];

	ret = qma6100_read_raw_xyz(rawData);
	if(ret == QMA6100_SUCCESS)
	{
		rawData_temp[g_qma6100.cvt.map[0]] = g_qma6100.cvt.sign[0]*rawData[0];
		rawData_temp[g_qma6100.cvt.map[1]] = g_qma6100.cvt.sign[1]*rawData[1];
		rawData_temp[g_qma6100.cvt.map[2]] = g_qma6100.cvt.sign[2]*rawData[2];

		accData[0] = (rawData_temp[0]*1000)/(g_qma6100.lsb_1g);			
		accData[1] = (rawData_temp[1]*1000)/(g_qma6100.lsb_1g);
		accData[2] = (rawData_temp[2]*1000)/(g_qma6100.lsb_1g);
	}

	#if defined(QMA6100_USE_CALI)
		qma6100_data_cali_1000mg(accData);
	#endif
	return ret;

}




qs32 qma6100_soft_reset(void)
{
	qu8 reg_0x11 = 0;
	qu8 reg_0x33 = 0;
	qu32 retry = 0;
	qu8 cnt=0;

	DEBUG_LOG_STRING("qma6100_soft_reset\r\n");
	qma6100_writereg(0x36, 0xb6);
	qma6100_delay_ms(1);
	qma6100_writereg(0x36, 0x00);
	//qma6100_delay_ms(100);


	qma6100_readreg(0x33,&reg_0x33,1);
	DEBUG_LOG_STRING("tim 0x33 = 0x%x\r\n",reg_0x33);
	while((reg_0x33&0x05)!=0x05)
	{
		qma6100_readreg(0x33,&reg_0x33,1);
		qma6100_delay_ms(5);
		cnt++;
		if((reg_0x33&0x05)==0x05)
		DEBUG_LOG_STRING("timeout cnt=%d, 0x33 = 0x%x\r\n",cnt,reg_0x33);
		if(cnt>=100)
		{	
			DEBUG_LOG_STRING("Read 0x33 status timeout\r\n");
			break;
		}
	} 

}


qs32 qma6100_initialize(void)
{
#if defined(QMA6100_INT_LATCH)
	qu8 reg_data[4];
#endif
	qu8 DieId_H, DieId_L, WaferID;
	qu8 reg=0,cnt = 0;

	qma6100_soft_reset();


	reg = 0x00;
	while( 1 )
	{
		qma6100_readreg(0x45,&reg,1);

		if( (reg&0xF0)!=0xC0 )
		{	
			qma6100_soft_reset();
			cnt++;
			if(cnt>=100)
			{
				DEBUG_LOG_STRING("QMA6100P SWRst fail\r\n");
			  	break;
			}
		}
		else
		{
			DEBUG_LOG_STRING("QMA6100P SWRst end\r\n");
			break;
		}
		
	}


	qma6100_readreg(0x47, &DieId_L, 1);
	qma6100_readreg(0x48, &DieId_H, 1);
	qma6100_readreg(0x5a, &WaferID, 1);
	DEBUG_LOG_STRING("DieId_L:%d	DieId_H:%d	WaferID:%d \n", DieId_L, DieId_H, WaferID&0x1f);

	qma6100_writereg(0x4a, 0x20);
	qma6100_writereg(0x50, 0x51);   //0x49  0x51
	qma6100_writereg(0x56, 0x01);

	// /qma6100_set_range(QMA6100_RANGE_2G);	
	qma6100_set_range(QMA6100_RANGE_4G);	
	
	
#if defined(QMA6100_ODR_25)
	qma6100_writereg(0x10, 0x06);
#elif defined(QMA6100_ODR_50)
	qma6100_writereg(0x10, 0x05);   ////0x65  open NLP  	
#elif defined(QMA6100_ODR_100)
	qma6100_writereg(0x10, 0x00);
#endif

#if defined(LOW_POWER_16UA)
#if defined(QMA6100_ODR_50)
	qma6100_writereg(0x10, 0x00);   ////0x65  open NLP  
#endif
	qma6100_writereg(0x11, 0x85);   //0x85
#else
	qma6100_writereg(0x11, 0x84);   //0x84
#endif


#if defined(QMA6100_QST_USE_SPI)
	qma6100_writereg(0x4A, 0x20);  //force to SPI  interface
#else
	qma6100_writereg(0x4A, 0x28);  //force to  IIC interface
#endif

	//qma6100_writereg(0x46, 0x0f);    // for div mclk/2
	
#if defined(QMA6100_QST_USE_SPI)
	qma6100_writereg(0x11, 0x04);
	qma6100_delay_ms(5);
	qma6100_writereg(0x20, 0xC5);  //   SPI interface , pull up  close
	qma6100_writereg(0x11, 0x84);
#else
	qma6100_writereg(0x20, 0x05);
	// qma6100_writereg(0x20, 0x07);
#endif
	
	qma6100_delay_ms(5);
	qma6100_writereg(0x5F, 0x80);
	qma6100_delay_ms(5);
	qma6100_writereg(0x5F, 0x00);
	qma6100_delay_ms(5);

#if defined(QMA6100_DATA_READY)
	qma6100_drdy_config(QMA6100_MAP_INT1, QMA6100_ENABLE);
#endif

#if defined(QMA6100_FIFO_FUNC)
	qma6100_fifo_config(QMA6100_MAP_INT1, QMA6100_ENABLE);
#endif

#if defined(QMA6100_STEPCOUNTER)
	qma6100_stepcounter_config(QMA6100_ENABLE);
	#if defined(QMA6100_STEP_INT)
	qma6100_step_int_config(QMA6100_MAP_INT1, QMA6100_ENABLE);
	// qma6100_step_int_config(QMA6100_MAP_INT2, QMA6100_ENABLE);
	#endif	
	#if defined(QMA6100_SIGNIFICANT_STEP_INT)
	qma6100_sigstep_int_config(QMA6100_MAP_INT1, QMA6100_ENABLE);
	#endif
#endif

#if defined(QMA6100_ANY_MOTION)
	qma6100_anymotion_config(QMA6100_MAP_INT1, QMA6100_ENABLE);
	// qma6100_anymotion_config(QMA6100_MAP_INT2, QMA6100_ENABLE);
#endif
#if defined(QMA6100_NO_MOTION)
	qma6100_nomotion_config(QMA6100_MAP_INT1, QMA6100_ENABLE);
#endif

#if defined(QMA6100_TAP_FUNC)
	qma6100_tap_config(QMA6100_TAP_SINGLE|QMA6100_TAP_DOUBLE|QMA6100_TAP_TRIPLE, QMA6100_MAP_INT1, QMA6100_ENABLE);
#endif

#if defined(QMA6100_HAND_RAISE_DOWN)
	qma6100_hand_raise_down(QMA6100_MAP_INT1, QMA6100_ENABLE);
#endif


#if defined(QMA6100_INT_LATCH)
	qma6100_writereg(0x21, 0x03);	// default 0x1c, step latch mode
	qma6100_readreg(0x09, reg_data, 4);
	DEBUG_LOG_STRING("read status=[0x%x 0x%x 0x%x 0x%x] \n", reg_data[0],reg_data[1],reg_data[2],reg_data[3]);
#endif

#ifdef __WRITE_OFFSET_REG__
	 qma6100_clear_offset_reg();
#endif
	///qma6100_dump_reg();
	return QMA6100_SUCCESS;
}
void qma_int_mode(void) {
	GPIO_Init(MOUSE_INT_PIN, GPIO_Mode_In_Down);
	GPIO_WakeUp(MOUSE_INT_PIN, GPIO_WakeUpHigh);
	DEBUG_LOG_STRING("qma_int_mode \n");
}

void qma6100_power_down(void)
{
	qma6100_writereg(0x11, 0x87);
}

uint8_t reg_value;

qs32 qma6100_init(void)
{
	qs32 ret = 0;
	qu8 slave_addr[2] = {QMA6100_I2C_SLAVE_ADDR, QMA6100_I2C_SLAVE_ADDR2};
	qu8 index=0;


	for(index=0; index<2; index++)
	{
		g_qma6100.chip_id = 0;
		g_qma6100.slave = slave_addr[index];
		g_qma6100.chip_id = qma6100_chip_id();

		if((g_qma6100.chip_id == 0xfa)||((g_qma6100.chip_id & 0xF0) == 0x90))
		{
			DEBUG_LOG_STRING("qma6100 find \n");
			break;
		}
	}
#if defined(QMA6100_FIX_IIC)
	qma6100_writereg(0x20, 0x45);
	g_qma6100.slave = QMA6100_I2C_SLAVE_ADDR;
#endif


	if((g_qma6100.chip_id == 0xfa)||((g_qma6100.chip_id & 0xF0) == 0x90))
		ret = qma6100_initialize();
		// ret = 0;
	else
		ret = QMA6100_FAIL;
	
	//g_qma6100.layout = 3;
	g_qma6100.layout = 0;
	g_qma6100.cvt.map[0] = qst_map[g_qma6100.layout].map[0];
	g_qma6100.cvt.map[1] = qst_map[g_qma6100.layout].map[1];
	g_qma6100.cvt.map[2] = qst_map[g_qma6100.layout].map[2];
	g_qma6100.cvt.sign[0] = qst_map[g_qma6100.layout].sign[0];
	g_qma6100.cvt.sign[1] = qst_map[g_qma6100.layout].sign[1];
	g_qma6100.cvt.sign[2] = qst_map[g_qma6100.layout].sign[2];

	return ret;
}

