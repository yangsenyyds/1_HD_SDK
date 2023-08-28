/******************** (C) COPYRIGHT 2021 MiraMEMS *****************************
* File Name     : mir3da.c
* Author        : dhzhou@miramems.com
* Version       : V1.0
* Date          : 05/18/2021
* Description   : Demo for configuring da718/da6618
*******************************************************************************/
#include "mir3da_267.h"
#include <stdlib.h>
#include "yc11xx_systick.h"
#include "yc_debug.h"

/*******************************************************************************
Typedef definitions
********************************************************************************/

u8_m i2c_addr = 0x26;
// u8_m i2c_addr;
static uint8_t sw_i2c_writereg(uint8_t slave_addr, uint8_t reg_addr, uint8_t reg_val)
{
    IIC_Start();
    IIC_SendByte(slave_addr << 1 | 0x00);
    if (IIC_WaitAck())
    {
        return 1;
    }
    IIC_SendByte(reg_addr);
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
    IIC_SendByte(slave_addr << 1 | 0x00);
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
    IIC_SendByte(slave_addr <<1|0x01);//| DEVICE_SLAVE_READ);
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
//todo i2c/spi read api
s8_m mir3da_register_read(u8_m addr, u8_m *data_m, u8_m len)
{
    uint8_t ret = 1;
    uint16_t retry = 0;

    while ((ret) && (retry++ < 5))
    {
        ret = sw_i2c_readreg(i2c_addr, addr, data_m, len);
    }
	return 0;
}

//todo i2c/spi write api
s8_m mir3da_register_write(u8_m addr, u8_m data_m)
{
	uint8_t ret = 1;
    uint16_t retry = 0;

    while ((ret) && (retry++ < 5))
    {
        ret = sw_i2c_writereg(i2c_addr, addr, data_m);
    }
	return 0;
}

s8_m mir3da_register_mask_write(u8_m addr, u8_m mask, u8_m data)
{
	s8_m res=0;
    unsigned char tmp_data=0;

    res = mir3da_register_read(addr, &tmp_data, 1);
    tmp_data &= ~mask;
    tmp_data |= data & mask;
    res = mir3da_register_write(addr, tmp_data);
	
	return 0;
}

//Initialization
s8_m mir3da_init(void)
{
	s8_m res = 0;
	u8_m data_m = 0;
	int i;
	for(i=0; i<3; i++){
		mir3da_register_read(REG_CHIP_ID, &data_m, 1);

        if(data_m != 0x13)
		{
			DEBUG_LOG_STRING("data_m %x-----\r\n", data_m); 
			return -1;
			// while(1){

			// }
		}
	}
	DEBUG_LOG_STRING("------mir3da chip id = %x-----\r\n", data_m); 

	//do softreset
	res |= mir3da_register_write(REG_SPI_I2C, 0x24);
	SysTick_DelayMs(20);
	
	res |= mir3da_register_write(REG_RESOLUTION_RANGE, 0x60);	//+/-2G,16bit
	res |= mir3da_register_write(REG_MODE_ODR, 0x07);			//lowpower mode, ODR 100hz

	//private register, key to close internal pull up.
	mir3da_register_write(0x7F, 0x83);
	mir3da_register_write(0x7F, 0x69);
	mir3da_register_write(0x7F, 0xBD);
#if 0	//only for debug
	mir3da_register_read(0xC3, &data_m, 1);
	DEBUG_LOG_STRING("0xC3=%02x", data_m);
	mir3da_register_read(0xC4, &data_m, 1);
	DEBUG_LOG_STRING("0xC4=%02x", data_m);
	mir3da_register_read(0xC5, &data_m, 1);
	DEBUG_LOG_STRING("0xC5=%02x", data_m);
	mir3da_register_read(0xC6, &data_m, 1);
	DEBUG_LOG_STRING("0xC6=%02x", data_m);
#endif
	//mir3da_register_mask_write(0x8F, 0x02, 0x00);	//don't pull up i2c sda/scl pin if needed
	//mir3da_register_mask_write(0x8C, 0x80, 0x00);	//don't pull up cs pin if needed

	if(i2c_addr == 0x26){
		mir3da_register_mask_write(0x8C, 0x40, 0x00);	//must close pin1/sdo pull up
	}
	
#ifdef _SPI
    mir3da_register_mask_write(0x8C, 0xC0, 0x00);	//close CS/MISO pull up
    mir3da_register_mask_write(0x8F, 0x12, 0x00);	//close i2c filter and MOSI/SCL pull up
#endif


	return res;	    	
}

//enable/disable the chip
s8_m mir3da_set_enable(u8_m enable)
{
	s8_m res = 0;
	if(enable)
		res = mir3da_register_mask_write(REG_MODE_AXIS, 0x80, 0x00);
	else	
		res = mir3da_register_mask_write(REG_MODE_AXIS, 0x80, 0x80);

	return res;	
}

//mir3da_init -> mir3da_set_enable -> mir3da_read_data
//Read three axis data, SENSITIVITY=16384lsb=1G
s8_m mir3da_read_data(s16_m *x, s16_m *y, s16_m *z)
{
    u8_m tmp_data[6] = {0};

    if (mir3da_register_read(REG_ACC_X_LSB, tmp_data, 6) != 0) {
        return -1;
    }

    *x = (s16_m)(tmp_data[1] << 8 | tmp_data[0]);
    *y = (s16_m)(tmp_data[3] << 8 | tmp_data[2]);
    *z = (s16_m)(tmp_data[5] << 8 | tmp_data[4]);	

    return 0;
}

void mir3da_read_fifo_data(s16_m *x, s16_m *y, s16_m *z)
{
    u8_m tmp_data[32*6] = {0};
	u8_m data_m = 0;
	u32_m fifo_data_len;
	int i;
	
	mir3da_register_read(REG_FIFO_STATUS, &data_m, 1);	//read the length of the data in the FIFO
	fifo_data_len = data_m & 0x3F;

    mir3da_register_read(REG_ACC_X_LSB, tmp_data, fifo_data_len*6);

	for(i=0; i<fifo_data_len; i++){
        *(x+i*6) = (s16_m)(tmp_data[1+i*6] << 8 | tmp_data[0+i*6]);
        *(y+i*6) = (s16_m)(tmp_data[3+i*6] << 8 | tmp_data[2+i*6]);
        *(z+i*6) = (s16_m)(tmp_data[5+i*6] << 8 | tmp_data[4+i*6]);
	}
}

void mir3da_spi_msb_read_fifo_data(s16_m *x, s16_m *y, s16_m *z)
{
    u8_m tmp_data[32*6] = {0};
	u8_m data_m = 0;
	u32_m fifo_data_len;
	int i;
	
	mir3da_register_read(REG_FIFO_STATUS, &data_m, 1);	//read the length of the data in the FIFO
	fifo_data_len = data_m & 0x3F;

    mir3da_register_read(REG_ACC_Z_MSB, tmp_data, fifo_data_len*6);

	for(i=0; i<fifo_data_len; i++){
        *(x+i*6) = (s16_m)(tmp_data[4+i*6] << 8 | tmp_data[5+i*6]);
        *(y+i*6) = (s16_m)(tmp_data[2+i*6] << 8 | tmp_data[3+i*6]);
        *(z+i*6) = (s16_m)(tmp_data[0+i*6] << 8 | tmp_data[0+i*6]);
	}
}

/* Configure the FIFO to stream mode
 * Watermark level set to 32
 */
void config_fifo_watermark_int(){
	
	mir3da_register_write(REG_FIFO_CTRL, 0x80|0x1F);	//stream mode, 32 watermark
	
	mir3da_register_write(REG_INT_SET0, 0x08);
	mir3da_register_write(REG_INT_MAP2, 0x02);
}

//open active interrupt
s8_m mir3da_open_active_interrupt(){
	s8_m   res = 0;

	res  = mir3da_register_write(REG_INT_CONFIG,   0x00);	//push-pull, active high
	res |= mir3da_register_write(REG_INT_SET1,     0x87);
	res |= mir3da_register_write(REG_ACTIVE_DUR,   0x00);
	res |= mir3da_register_write(REG_ACTIVE_X_THS, 0x24);
	res |= mir3da_register_write(REG_ACTIVE_Y_THS, 0x24);
	res |= mir3da_register_write(REG_ACTIVE_Z_THS, 0x24);
	res |= mir3da_register_write(REG_INT_MAP1,     0x04);

	return res;
}

//close active interrupt
s8_m mir3da_close_active_interrupt(void){
	s8_m   res = 0;

	res  = mir3da_register_write(REG_INT_SET1, 0x00);
	res |= mir3da_register_write(REG_INT_MAP1, 0x00);

	return res;
}

signed char mir3da_open_step_counter(void)
{
	signed char res = -1;
	
	res  = mir3da_register_write(REG_INT_SET1, 0x80);
	res |= mir3da_register_write(0x2F,   0x78);
	res |= mir3da_register_write(0x30,   0x5D);
	res |= mir3da_register_write(0x31,   0x5D);
	res |= mir3da_register_write(0x32,   0x00);
	res |= mir3da_register_write(0x33,   0xAF); //A3灵敏
	res |= mir3da_register_write(0x35,   0x77);
	res |= mir3da_register_write(0x37,   0x41);
	
	return res;
}

signed char mir3da_close_step_counter(void)
{
	signed char res = -1;
	
	res =  mir3da_register_mask_write(REG_STEP_FILTER, 0x80, 0x22);	
	
	return res;
}


signed char mir3da_get_step(unsigned short *step)
{
	signed char res = -1;
	unsigned char    tmp_data[2] = {0};

	res = mir3da_register_read(REG_STEPS_LSB,  tmp_data,2); 

	if (res == 0)
		*step = tmp_data[0] << 8 | tmp_data[1];
}


signed char mir3da_clear_step(void)
{
	signed char res = -1;
	
	res = mir3da_register_write(REG_RESET_STEP,0x80);
	
	return res;	
}

//open step interrupt
void mir3da_set_step_interrupt_enable(int enable)
{
	signed char res = -1;
    if(enable){
		res = mir3da_register_write(REG_INT_CONFIG, 0x00);	//push-pull, active high
		res = mir3da_register_write(REG_INT_MAP1,   0x02);
		res = mir3da_register_write(REG_INT_SET0,   0x01);
	}
	else{
		res = mir3da_register_write(REG_INT_SET0, 0x00);
		res = mir3da_register_write(REG_INT_MAP1, 0x00);
	}
}

//open singel tap interrupt
void mir3da_set_s_tap_interrupt_enable(int enable)
{
	signed char res = -1;
    if(enable){
		res = mir3da_register_write(REG_INT_CONFIG, 0x00);	//push-pull, active high
		res = mir3da_register_write(REG_TAP_QUIET,  0x08);
		res = mir3da_register_write(REG_TAP_DUR,    0x24);
		res = mir3da_register_write(REG_TAP_THS,    0x0A);
		res = mir3da_register_write(REG_INT_MAP1,   0x20);
		res = mir3da_register_write(REG_INT_SET1,   0xA0);
	}
	else{
		res = mir3da_register_write(REG_INT_SET1, 0x00);
		res = mir3da_register_write(REG_INT_MAP1, 0x00);
	}
}

//open double tap interrupt
void mir3da_set_d_tap_interrupt_enable(int enable)
{
	signed char res = -1;
    if(enable){
		res = mir3da_register_write(REG_INT_CONFIG, 0x00);	//push-pull, active high
		res = mir3da_register_write(REG_TAP_QUIET,  0x08);
		res = mir3da_register_write(REG_TAP_DUR,    0x24);
		res = mir3da_register_write(REG_TAP_THS,    0x0A);
		res = mir3da_register_write(REG_INT_MAP1,   0x10);
		res = mir3da_register_write(REG_INT_SET1,   0x90);
	}
	else{
		res = mir3da_register_write(REG_INT_SET1, 0x00);
		res = mir3da_register_write(REG_INT_MAP1, 0x00);	
	}
}

#define SELF_TEST_MIN 200*16384/1000	//1G=1000mg=16384lsb, 375±175mg
#define SELF_TEST_MAX 550*16384/1000
/*
 * The chip needs to keep horizontal.
 * Do self test.
 * -1: failed
 * 0: success
 */
s8_m mir3da_self_test(void){
	s8_m res = 0;
	s32_m i;
    s16_m x = 0,y = 0,z = 0;
	u8_m tmp_data, backup_fs, backup_level, backup_mode;
	s16_m before_x=0, before_y=0, before_z=0;
	s16_m after_x=0, after_y=0, after_z=0;
	
	mir3da_register_read(REG_RESOLUTION_RANGE, &backup_fs, 1);
	mir3da_register_read(REG_MODE_ODR, &backup_level, 1);
	mir3da_register_read(REG_MODE_AXIS, &backup_mode, 1);

    //must power on
	mir3da_register_write(REG_RESOLUTION_RANGE, 0x00);
    mir3da_register_write(REG_MODE_ODR, 0x88);	//per_mode; level0
    mir3da_register_write(REG_MODE_AXIS, 0x00);	//power on

    for(i=0; i<20; i++)
    {
        mir3da_read_data(&x, &y, &z);
        before_x += (s32_m)x;
        before_y += (s32_m)y;
        before_z += (s32_m)z;
		SysTick_DelayMs(5);
    }
	before_x = before_x/20;
	before_y = before_y/20;
	before_z = before_z/20;
    mir3da_register_mask_write(REG_MODE_AXIS, 0x10, 0x10);	//set self test bit
	SysTick_DelayUs(500);

    for(i=0; i<20; i++)
    {
        mir3da_read_data(&x, &y, &z);
        after_x += (s32_m)x;
        after_y += (s32_m)y;
        after_z += (s32_m)z;
		SysTick_DelayMs(5);
    }
	after_x = after_x/20;
	after_y = after_y/20;
	after_z = after_z/20;
    mir3da_register_mask_write(REG_MODE_AXIS, 0x10, 0x00);	//close self test bit
	
	mir3da_register_write(REG_RESOLUTION_RANGE, backup_fs);
	mir3da_register_write(REG_MODE_ODR, backup_level);
	mir3da_register_write(REG_MODE_AXIS, backup_mode);

    if(abs(after_x-before_x)<=SELF_TEST_MIN)
		return -1;
	if(abs(after_x-before_x)>=SELF_TEST_MAX)
		return -1;
    if(abs(after_y-before_y)<=SELF_TEST_MIN)
		return -1;
	if(abs(after_y-before_y)>=SELF_TEST_MAX)
		return -1;
    if(abs(after_z-before_z)<=SELF_TEST_MIN)
		return -1;
	if(abs(after_z-before_z)>=SELF_TEST_MAX)
		return -1;

	return 0;
}

#define SENSITIVITY 16384
#define OFFSET_THRESHOLD 400*SENSITIVITY/1000	//1G=1000mg=SENSITIVITY lsb, 400mg
/*
  The chip needs to keep horizontal.
*/
void do_difff_calibrate()
{
	int i;
    short x=0, y=0, z=0;
    s16_m off_x=0, off_y=0, off_z=0;
    s32_m totalx = 0,totaly = 0,totalz =0;
	
	mir3da_init();
	mir3da_set_enable(1);

 	//must power on
    for(i=0; i<20; i++)
    {
        mir3da_read_data(&x, &y, &z);
        totalx += (s32_m)x;
        totaly += (s32_m)y;
        totalz += (s32_m)z;
		SysTick_DelayMs(5);
    }
    
    off_x = (s16_m)(totalx/20 - 0);
    off_y = (s16_m)(totaly/20 - 0);
    if(totalz>0)
        off_z = (s16_m)(totalz/20 - SENSITIVITY);
    else
        off_z = (s16_m)(totalz/20 + SENSITIVITY);

    mir3da_set_enable(0);

	//save_to_file_system(off_x, off_y, off_z);	//pseudo-code, Call the actual platform function
}

/*
 * This procedure is used to determine whether the OFFSET exceeds the threshold.
 * The chip needs to keep horizontal.
 * 
 * -1: failed
 * 0: success
 */
s8_m mir3da_offset_test(void){
	int i;
    short x = 0,y = 0,z = 0;
    s16_m offset_x=0, offset_y=0, offset_z=0;
    s32_m totalx = 0,totaly = 0,totalz =0;

 	//must power on
    for(i=0; i<20; i++)
    {
        mir3da_read_data(&x, &y, &z);
        totalx += (s32_m)x;
        totaly += (s32_m)y;
        totalz += (s32_m)z;
		SysTick_DelayMs(5);
    }
    
    offset_x = (s16_m)(totalx/20 - 0);
    offset_y = (s16_m)(totaly/20 - 0);
    if(totalz>0)
        offset_z = (s16_m)(totalz/20 - SENSITIVITY);
    else
        offset_z = (s16_m)(totalz/20 + SENSITIVITY);

	if(abs(offset_x)>OFFSET_THRESHOLD)
		return -1;
	if(abs(offset_y)>OFFSET_THRESHOLD)
		return -1;
	if(abs(offset_z)>OFFSET_THRESHOLD)
		return -1;

	return 0;
}