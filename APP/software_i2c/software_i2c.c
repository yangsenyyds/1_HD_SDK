#include "software_i2c.h"
#include "yc11xx_gpio.h"
#include "yc11xx_systick.h"

#include "remote_control.h"

#define IIC_SCL_OUT_0()     GPIO_Init(IIC_SCL_PIN, GPIO_Mode_Out_Low)
#define IIC_SCL_OUT_1()     GPIO_Init(IIC_SCL_PIN, GPIO_Mode_Out_High)
#define IIC_SCL_IN()        GPIO_Init(IIC_SCL_PIN, GPIO_Mode_Deinit)

#define IIC_SDA_OUT_0()     GPIO_Init(IIC_SDA_PIN, GPIO_Mode_Out_Low)
#define IIC_SDA_OUT_1()     GPIO_Init(IIC_SDA_PIN, GPIO_Mode_Out_High)
#define IIC_SDA_IN()        GPIO_Init(IIC_SDA_PIN, GPIO_Mode_Deinit)
#define IIC_SDA_IN_1()      GPIO_Init(IIC_SDA_PIN, GPIO_Mode_In_Up)
#define IIC_SDA_READ()      GPIO_ReadDataBit(IIC_SDA_PIN)

#define vTaskDelay(a) SysTick_DelayUs((a) * 1)
   
void IIC_NAck(void)
{
    IIC_SCL_OUT_0();
    vTaskDelay(1);
    IIC_SDA_OUT_1();            // CPU驱动SDA = 1
    vTaskDelay(2);
    IIC_SCL_OUT_1();            // CPU产生1个时钟
    vTaskDelay(2);
    IIC_SCL_OUT_0();
}

void IIC_Ack(void)
{
    IIC_SCL_OUT_0();            // CPU产生1个时钟
    vTaskDelay(1);
    IIC_SDA_OUT_0();            // CPU驱动SDA = 0
    vTaskDelay(2);
    IIC_SCL_OUT_1();
    vTaskDelay(2);
    IIC_SCL_OUT_0();
}

/**
 @brief CPU产生一个时钟，并读取器件的ACK应答信号
 @param 无
 @return 返回0表示正确应答，1表示无器件响应
*/
uint8_t IIC_WaitAck(void)
{
    uint8_t cyc = 0; 
    
    IIC_SDA_IN();               // CPU释放SDA总线
    vTaskDelay(1); 
    IIC_SCL_OUT_1();            // CPU驱动SCL = 1, 此时器件会返回ACK应答
    vTaskDelay(1);
    while(IIC_SDA_READ())
    {
        cyc++;
        if (cyc > 200) {
            IIC_Stop();
            return 1;
        }
    }
    
    IIC_SCL_OUT_0();
    vTaskDelay(1);

    return 0;  
}

/**
 @brief CPU从I2C总线设备读取8bit数据
 @param 无
 @return 读到的数据
*/ 
uint8_t IIC_ReadByte(uint8_t ack)
{
    uint8_t value = 0;
    
    IIC_SDA_IN();          // SDA线输入模式
    
    for(uint8_t i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC_SCL_OUT_1();
        vTaskDelay(1);
        if(IIC_SDA_READ()) {
            value++;
        }            
        IIC_SCL_OUT_0(); 
        vTaskDelay(2); 
    }					

    if (!ack) {
        IIC_NAck();
    }	
	else {
        IIC_Ack();
    }
	
    return value;
}

/**
 @brief CPU向I2C总线设备发送8bit数据
 @param ucByte -[in] 等待发送的字节
 @return 无
*/  
void IIC_SendByte(uint8_t ucByte)
{
    IIC_SCL_OUT_0();            // 拉低时钟开始数据传输
    
    for(uint8_t i = 0; i < 8; i++)
    {              
        if(ucByte & 0x80) {            
            IIC_SDA_OUT_1();
        }
        else {
            IIC_SDA_OUT_0();
        }
		ucByte <<= 1; 	
        vTaskDelay(2);        
        IIC_SCL_OUT_1();
        vTaskDelay(2);
        IIC_SCL_OUT_0();
        vTaskDelay(2);        
    }	 
}

void IIC_Stop(void)
{
    IIC_SCL_OUT_0();
    IIC_SDA_OUT_0();
    vTaskDelay(4);
    IIC_SCL_OUT_1();
    vTaskDelay(4);
    IIC_SDA_OUT_1();
}

void IIC_Start(void)
{
    IIC_SDA_OUT_1();
    IIC_SCL_OUT_1();
    vTaskDelay(4);
    IIC_SDA_OUT_0();
    vTaskDelay(4);
    IIC_SCL_OUT_0();
}

void IIC_DeInit(void)
{
    IIC_SCL_OUT_0();
    IIC_SDA_OUT_0();
}

void IIC_Init(void)
{
    IIC_SCL_OUT_1();
    IIC_SDA_OUT_1();

    IIC_Stop();             // 给一个停止信号, 复位I2C总线上的所有设备到待机模式
}

/**
 @brief 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
 @param address -[in] 设备的I2C总线地址+读写控制bit（0 = w， 1 = r)
 @return 0 - 表示正确， 1 - 表示未探测到
*/  
uint8_t IIC_CheckDevice(uint8_t address)
{
    uint8_t ucAck;

    IIC_Init();
    IIC_Start();
    IIC_SendByte(address);
    ucAck = IIC_WaitAck();
    IIC_Stop();

    return ucAck;
}
