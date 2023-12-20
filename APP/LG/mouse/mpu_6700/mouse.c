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

unsigned char TX_DATA[4];  	 //鏄剧ず鎹紦瀛樺尯
unsigned char BUF[10];       //鎺ユ敹鏁版嵁缂撳瓨鍖�
char  test=0; 				 //IIC鐢ㄥ埌
short T_X,T_Y,T_Z,T_T;		 //X,Y,Z杞达紝娓╁害

bool Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)		     //void
{
    IIC_Start();
    IIC_SendByte(SlaveAddress);   //鍙戦€佽澶囧湴鍧€+鍐欎俊鍙�//IIC_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//璁剧疆楂樿捣濮嬪湴鍧€+鍣ㄤ欢鍦板潃 
    if(IIC_WaitAck())
    {
        IIC_Stop(); 
        return 1;
    }
    IIC_SendByte(REG_Address);   //璁剧疆浣庤捣濮嬪湴鍧€      
    IIC_WaitAck();	
    IIC_SendByte(REG_data);
    IIC_WaitAck();   
    IIC_Stop(); 
    delay5ms();
    return true;
}

//鍗曞瓧鑺傝鍙�*****************************************
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address)
{   
    uint8_t REG_data;     	
    IIC_Start();
    IIC_SendByte(SlaveAddress); //IIC_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//璁剧疆楂樿捣濮嬪湴鍧€+鍣ㄤ欢鍦板潃 
    if(IIC_WaitAck())
    {
        IIC_Stop();
        test=1; 
        return 1;
    }
    IIC_SendByte(REG_Address);   //璁剧疆浣庤捣濮嬪湴鍧€      
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(SlaveAddress | 0x01);
    IIC_WaitAck();

	REG_data= IIC_ReadByte(0);
    IIC_Stop();
    //return true;
	return REG_data;

}	

static int Init_MPU9250(void)
{
	
	// GPIO_Configuration();		 //gyro閰嶇疆GPIO
	// I2C_GPIO_Config();		 //gyro閰嶇疆IIC浣跨敤绔彛
	
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //卤2000掳
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
    Single_Write(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//瑙ｉ櫎浼戠湢鐘舵€�
	Single_Write(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
	Single_Write(GYRO_ADDRESS,CONFIG, 0x06);
	Single_Write(GYRO_ADDRESS,GYRO_CONFIG, 0x18);
	Single_Write(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);
    uint8_t chip_id = Single_Read(MPU_6500_SENSOR_ADDR,WHO_AM_I_REG_ADDR);
    if(chip_id != 0x90){
        while(1){

        }
    }
    
//----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	
    return 1;
}
INIT_BOARD_EXPORT(Init_MPU9250);
	
//******璇诲彇MPU9250鏁版嵁****************************************
void READ_MPU9250_ACCEL(void)
{ 

   BUF[0]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_L); 
   BUF[1]=Single_Read(ACCEL_ADDRESS,ACCEL_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=164; 						   //璇诲彇璁＄畻X杞存暟鎹�

   BUF[2]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_L);
   BUF[3]=Single_Read(ACCEL_ADDRESS,ACCEL_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=164; 						   //璇诲彇璁＄畻Y杞存暟鎹�
   BUF[4]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=164; 					       //璇诲彇璁＄畻Z杞存暟鎹�
 
}

void READ_MPU9250_GYRO(void)
{ 

   BUF[0]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=Single_Read(GYRO_ADDRESS,GYRO_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=16.4; 						   //璇诲彇璁＄畻X杞存暟鎹�

   BUF[2]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=Single_Read(GYRO_ADDRESS,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=16.4; 						   //璇诲彇璁＄畻Y杞存暟鎹�
   BUF[4]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=Single_Read(GYRO_ADDRESS,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=16.4; 					       //璇诲彇璁＄畻Z杞存暟鎹�
 
 
  // BUF[6]=Single_Read(GYRO_ADDRESS,TEMP_OUT_L); 
  // BUF[7]=Single_Read(GYRO_ADDRESS,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// 璇诲彇璁＄畻鍑烘俯搴�
}
/*
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
*/