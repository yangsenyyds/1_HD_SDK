/**
 * @file yc11xx_gpio.c
 * @author duanchu.he
 * @brief GPIO driver
 * @version 0.1
 * @date 2021-07-06
 *
 *
 */
#include "yc11xx_gpio.h"
#include "reg_addr.h"
#include "btreg.h"


/**
 * @brief  Initializes the GPIOx peripheral according to the specified
 *          parameters in the GPIO_InitStruct.
 *
 * @param gpiox  where x can be (A..E) to select the GPIO peripheral.
 * @param gpio_InitStruct
 */
void GPIO_Init(GPIO_X gpio_pin, GPIOMode_TypeDef gpio_mode)
{
	_ASSERT(IS_GPIO_PIN(gpio_pin));
    HWRITE(CORE_GPIO_CONFIG_BASE(gpio_pin), gpio_mode);
}

/**
 * @brief Reads the specified GPIO input data bit.
 *
 * @param gpiox where x can be (A..E) to select the GPIO peripheral.
 * @param gpio_pin specifies the port bit to read.
 * This parameter can be GPIO_Pin_x where x can be (0..7).
 * @return gpio input data bit value.
 */
uint8_t GPIO_ReadDataBit(GPIO_X gpio_pin)
{
    _ASSERT(IS_GPIO_PIN(gpio_pin));
    uint32_t vlaue = HREADL(CORE_GPIO_IN_BASE(gpio_pin));

    if(vlaue & reg_deal_gpiox(gpio_pin))
	    return 1;
	else
	    return 0;
}

/**
 * @brief Read gpio interrupt status.
 *
 * @param gpiox where x can be (A..E) to select the GPIO peripheral.
 * @param gpio_pin specifies the port bit to read.
 *  				 	This parameter can be GPIO_Pin_x where x can be (0..7).
 * @return true
 * @return false
 */
uint8_t GPIO_GetInterruptStatus(GPIO_X gpio_pin)
{
    _ASSERT(IS_GPIO_PIN(gpio_pin));

    uint32_t value = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_ISR_L, gpio_pin));

    if(value & reg_deal_gpiox(gpio_pin))
		return 1;
	else
		return 0;
}

/**
 * @brief Sets the gpio interrupt trigger mode.
 *
 * @param gpiox where x can be (A..E) to select the GPIO peripheral.
 * @param gpio_pin specifies the port bit to read.
 *  				 	This parameter can be GPIO_Pin_x where x can be (0..7).
 * @param interrupt_type gpio interrupt type structure
 */
void GPIO_SetInterrupt(GPIO_X gpio_pin, GPIO_IntrTypeDef int_type)
{
    _ASSERT(IS_GPIO_PIN(gpio_pin));
	_ASSERT(IS_INTERRUPT(int_type));

	uint32_t tmp = 0;

	switch(int_type)
	{
	case Rising_Interrupt :

		tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_RTS_L, gpio_pin));
		HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_RTS_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
		break;
	case Fall_Interrupt :

		tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_FTS_L, gpio_pin));
		HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_FTS_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
		break;
	case Low_Interrupt :

		tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_LTS_L, gpio_pin));
		HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_LTS_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
		break;
	case High_Interrupt :

		tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_LTS_L, gpio_pin));
		HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_LTS_L, gpio_pin), tmp & ~reg_deal_gpiox(gpio_pin));
		break;
	case Rising_Fall_interrupt :

		tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_FTS_L, gpio_pin));
		HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_FTS_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));

		tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_RTS_L, gpio_pin));
		HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_RTS_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
		break;
	}

	tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_IEN_L, gpio_pin));
	HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_IEN_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
}

/**
 * @brief Clear  gpio interrupt status
 *
 * @param gpiox where x can be (A..E) to select the GPIO peripheral.
 * @param gpio_pin specifies the port bit to read.
 *  				 	This parameter can be GPIO_Pin_x where x can be (0..7).
 */
void GPIO_ClearInterrupt(GPIO_X gpio_pin)
{
    _ASSERT(IS_GPIO_PIN(gpio_pin));

    uint32_t tmp = 0;

	tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_ICR_L, gpio_pin));
	HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_ICR_L, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));

	tmp = HREADL(CORE_GPIO_INTER_BASE(CORE_GPIO_ICR_L, gpio_pin));
	HWRITEL(CORE_GPIO_INTER_BASE(CORE_GPIO_ICR_L, gpio_pin), tmp & ~reg_deal_gpiox(gpio_pin));
}

/**
 * @brief Gpio long press reset
 *
 * @param GPIO_Pinx_Reset Initializes the GPIOx  peripheral according to the specified
 *          parameters in the GPIO_LongPress_Reset.
 * @param bit_status DISABE or ENABLE
 */
void GPIO_SetLongPressReset(GPIO_LongPress_Reset gpio_reset, FunctionalState bit_status)
{
	uint32_t val = 0;

    if(bit_status != DISABLE)
    {
    	val = CoreReg_LpmGetIceWdtLongRst() | (1 << 3);

    	CoreReg_LpmWrite2IceWdtLongRst(val);
        HWOR(CORE_IO_RST_EN, 1 << gpio_reset);
    }
    else
    {
    	val = CoreReg_LpmGetIceWdtLongRst() & (~(1 << 3));

    	CoreReg_LpmWrite2IceWdtLongRst(val);
    	HWCOR(CORE_IO_RST_EN, 1 << gpio_reset);
    }
}

/**
 * @brief Gpio long press reset time config
 *
 * @param longpress_timeout gpio longpress reset time set
 */
void GPIO_SetLongPressResetTimeOut(GPIO_LongPress_TimeOut longpress_timeout)
{
    HWRITE(CORE_LPM_RD_MUX + 1, longpress_timeout);
}

/**
 * @brief Gpio long press reset time config
 *
 * @param longpress_timeout gpio longpress reset time set
 */
void GPIO_WakeUp(GPIO_X gpio_pin, GPIOWakeUp_TypeDef wakeup_type)
{
    uint32_t tmp = 0;

    if(wakeup_type != GPIO_WakeUpLow)
    {
    	tmp = HREADL(CORE_GPIO_WAKEUP_LOW_BASE(mem_gpio_wakeup_low, gpio_pin));
    	HWRITEL(CORE_GPIO_WAKEUP_LOW_BASE(mem_gpio_wakeup_low, gpio_pin), tmp & ~reg_deal_gpiox(gpio_pin));

    	tmp = HREADL(CORE_GPIO_WAKEUP_HIGH_BASE(mem_gpio_wakeup_high, gpio_pin));
    	HWRITEL(CORE_GPIO_WAKEUP_HIGH_BASE(mem_gpio_wakeup_high, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
    }
    else
    {
    	tmp = HREADL(CORE_GPIO_WAKEUP_HIGH_BASE(mem_gpio_wakeup_high, gpio_pin));
    	HWRITEL(CORE_GPIO_WAKEUP_HIGH_BASE(mem_gpio_wakeup_high, gpio_pin), tmp & ~reg_deal_gpiox(gpio_pin));

    	tmp = HREADL(CORE_GPIO_WAKEUP_LOW_BASE(mem_gpio_wakeup_low, gpio_pin));
    	HWRITEL(CORE_GPIO_WAKEUP_LOW_BASE(mem_gpio_wakeup_low, gpio_pin), tmp | reg_deal_gpiox(gpio_pin));
    }
}


