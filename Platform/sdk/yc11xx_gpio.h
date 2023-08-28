/**
* @file yc11xx_gpio.h
* @author duanchu.he
* @brief GPIO driver
* @version 0.1
* @date 2021-07-06
*
*
*/
#ifndef __YC11XX_GPIO_H_
#define __YC11XX_GPIO_H_


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "yc11xx.h"


/**
 * @brief Configuration Mode enumeration
 *
 */
typedef enum
{
  GPIO_Mode_Deinit = 0x00,  // IN FLOATING
  GPIO_Mode_QSPI_Ncs = 0x02,
  GPIO_Mode_Qspi_Sck,
  GPIO_Mode_Qspi_Io0,
  GPIO_Mode_Qspi_Io1,
  GPIO_Mode_Qspi_Io2,
  GPIO_Mode_Qspi_Io3,
  GPIO_Mode_Uart_Txd,
  GPIO_Mode_Uart_Rxd,
  GPIO_Mode_Uart_Rts,
  GPIO_Mode_Uart_Cts,
  GPIO_Mode_Uartb_Txd,
  GPIO_Mode_Uartb_Rxd,
  GPIO_Mode_Uartb_Rts,
  GPIO_Mode_Uartb_Cts,
  GPIO_Mode_Pwm_Out0,
  GPIO_mode_Nec_Gpioi = 0x11,
  GPIO_Mode_Qspi1_Ncs = 0x12,
  GPIO_Mode_Qspi1_Sck,
  GPIO_Mode_Qspi1_Io0,
  GPIO_Mode_Qspi1_Io1,
  GPIO_Mode_Qspi1_Io2,
  GPIO_Mode_Qspi1_Io3,
  GPIO_Mode_I2s_Dout,
  GPIO_Mode_I2s_Lrckout,
  GPIO_Mode_I2s_Bclkout,
  GPIO_Mode_I2s_Mclkout,
  GPIO_Mode_I2s_Din,
  GPIO_Mode_I2s_Lrckin,
  GPIO_Mode_I2s_Bclkin,
  GPIO_Mode_Spid_Miso,
  GPIO_Mode_Spid_Ncs,
  GPIO_Mode_Spid_Sck,
  GPIO_Mode_Spid_Mosi,
  GPIO_Mode_Spid_Mosi_Miso,
  GPIO_Mode_Spid_Ncsi,
  GPIO_Mode_Spid_Scki,
  GPIO_Mode_Sd_Pad_Clk,
  GPIO_Mode_Sd_Pad_Cmd,
  GPIO_Mode_Sd_Pad_Data0,
  GPIO_Mode_Sd_Pad_Data1,
  GPIO_Mode_Sd_Pad_Data2,
  GPIO_Mode_Sd_Pad_Data3,
  GPIO_Mode_Iic_Scl,
  GPIO_Mode_Iic_Sda,
  GPIO_Mode_Jtag_Tck,
  GPIO_Mode_Test_Audio_Gpio2,
  GPIO_Mode_Test_Audio_Gpio,//0x30
  GPIO_Mode_Clk_Gpio,
  GPIO_Mode_Clk_Dpll_38M4,
  GPIO_Mode_Bit_Send,
  GPIO_Mode_Micclk_Pdm,
  GPIO_Mode_Micdat_Pdm,
  GPIO_Mode_Qspi2_Ncs,
  GPIO_Mode_Qspi2_Sck,
  GPIO_Mode_Qspi2_Io0,
  GPIO_Mode_Qspi2_Io1,
  GPIO_Mode_Qspi2_Io2,
  GPIO_Mode_Qspi2_Io3,
  GPIO_Mode_Iic_Scl1,
  GPIO_Mode_Iic_Sda1,
  GPIO_Mode_Out_Low,  //62
  GPIO_Mode_Out_High, //63
  GPIO_Mode_In_Up,    //0x40
  GPIO_Mode_In_Down = 0x80,
  GPIO_Mode_Analog = 0xc0,
} GPIOMode_TypeDef;

/**
 * @brief gpio speed selection
 *
 */
typedef enum
{
    GPIO_WakeUpLow = 0,
    GPIO_WakeUpHigh,
} GPIOWakeUp_TypeDef;

/**
 * @brief gpio long press reset time config
 *
 */
typedef enum
{
	LONG_PRESS_TIMEOUT_64S = 0,
	LONG_PRESS_TIMEOUT_56S,
	LONG_PRESS_TIMEOUT_48S,
	LONG_PRESS_TIMEOUT_40S,
	LONG_PRESS_TIMEOUT_32S,
	LONG_PRESS_TIMEOUT_24S,
	LONG_PRESS_TIMEOUT_16S,
	LONG_PRESS_TIMEOUT_8S,
}GPIO_LongPress_TimeOut;

#define IS_GPIO_LongPress_TimeOut(LongPress_TimeOut) 	(((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_64S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_56S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_48S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_40S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_32S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_24S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_16S) || \
														((LongPress_TimeOut) == LONG_PRESS_TIMEOUT_8S))

/**
 * @brief gpio long press reset select
 *
 */
typedef enum
{
  TOUCH_0_Long_Press_Reset = 3,
  TOUCH_1_Long_Press_Reset,
  GPIO_11_Long_Press_Reset,
  GPIO_33_Long_Press_Reset,
  GPIO_35_Long_Press_Reset,
}GPIO_LongPress_Reset;

#define IS_GPIO_LongPress_Reset(LongPress_Reset) 	(((LongPress_Reset) == GPIO_20_Long_Press_Reset) || \
													((LongPress_Reset) == GPIO_26_Long_Press_Reset) || \
													((LongPress_Reset) == GPIO_31_Long_Press_Reset))

/**
 * @brief gpio interrupt type
 *
 */
typedef enum
{
	High_Interrupt = 0,
	Low_Interrupt,
	Rising_Interrupt,
	Fall_Interrupt,
	Rising_Fall_interrupt,
}GPIO_IntrTypeDef;

#define IS_INTERRUPT(intr_type) 			(((intr_type) == Low_Interrupt) 		|| \
													((intr_type) == High_Interrupt) 		|| \
													((intr_type) == Rising_Interrupt) 	|| \
													((intr_type) == Fall_Interrupt))


typedef enum
{
	GPIO_0 = 0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
	GPIO_16,
	GPIO_17,
	GPIO_18,
	GPIO_19,
	GPIO_20,
	GPIO_21,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
	GPIO_27,
	GPIO_28,
	GPIO_29,
	GPIO_30,
	GPIO_31,
	GPIO_32,
	GPIO_33,
	GPIO_34,
	GPIO_35,
	GPIO_36,
	GPIO_37,
}GPIO_X;

#define IS_GPIO_PIN(PIN) 							(((PIN) >= 0x00) && ((PIN < 0x26)))

void GPIO_Init(GPIO_X gpio_pin, GPIOMode_TypeDef gpio_mode);
uint8_t GPIO_ReadDataBit(GPIO_X gpio_pin);
uint8_t GPIO_GetInterruptStatus(GPIO_X gpio_pin);
void GPIO_SetInterrupt(GPIO_X gpio_pin, GPIO_IntrTypeDef int_type);
void GPIO_ClearInterrupt(GPIO_X gpio_pin);
void GPIO_SetLongPressReset(GPIO_LongPress_Reset gpio_reset, FunctionalState bit_status);
void GPIO_SetLongPressResetTimeOut(GPIO_LongPress_TimeOut longpress_timeout);
void GPIO_WakeUp(GPIO_X gpio_pin, GPIOWakeUp_TypeDef wakeup_type);

#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */


/******************************************************************END OF FILE****/
