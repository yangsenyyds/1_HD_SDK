#include "yc11xx_pwm.h"
#include "yc11xx_gpio.h"
/**
 * @brief  PWM param config
 * @param    GPIOx   GPIO grouping
 * @param    pin    :GPIO number
 * @param    pcnt  :Duration of PWM high level
 * @param    pcnt  :Duration of PWM low level
 * @return  None
 */
void PWM_Config(GPIO_X gpio_pin, uint16_t pcnt, uint16_t ncnt, uint8_t div)
{
    GPIO_Init(gpio_pin, GPIO_Mode_Pwm_Out0);
    TIM_Cmd(gpio_pin & 7, ENABLE);

    TIM_InitTypeDef pwm_init_struct;
    pwm_init_struct.TIMx = (TIM_NumTypeDef)gpio_pin & 7;
    pwm_init_struct.mode = TIM_Mode_PWM;
    pwm_init_struct.pwm.LoadMode = Load_enable; //load the pcnt/ncnt from self, default when sync mode on
    pwm_init_struct.frequency = div;            //to do a long high level
    pwm_init_struct.pwm.Sync = Sync_default;
    pwm_init_struct.pwm.HighLevelPeriod = pcnt;
    pwm_init_struct.pwm.LowLevelPeriod = ncnt;
    pwm_init_struct.pwm.LevelProOutput = 0;  //not use  reserve
    TIM_Init(&pwm_init_struct);
}
/**
 * @brief  Failure of PWM function corresponding to GPIO
 * @param    GPIOx   GPIO grouping
 * @param    pin    :GPIO number
 * @return  None
 */
void PWM_DisConfig(GPIO_X gpio_pin)
{
    GPIO_Init(gpio_pin, GPIO_Mode_Deinit);
    TIM_Cmd(gpio_pin &7, DISABLE);
}
/**
 * @brief  Set the PWM synchronization function
 * @param    GPIOx   GPIO grouping
 * @param    pin    :GPIO number
 * @param    pcnt  :Duration of PWM high level
 * @param    pcnt  :Duration of PWM low level
 * @return  None
 */
void PWM_Config_Sync(GPIO_X gpio_pin, uint16_t pcnt, uint16_t ncnt,uint8_t div)
{
    GPIO_Init(gpio_pin, GPIO_Mode_Pwm_Out0);
    TIM_Cmd(gpio_pin & 7, ENABLE);

    TIM_InitTypeDef pwm_init_struct;
    pwm_init_struct.TIMx = (TIM_NumTypeDef)gpio_pin & 7;
    pwm_init_struct.mode = TIM_Mode_PWM;
    pwm_init_struct.pwm.LoadMode = Load_default;//default when sync mode on
    pwm_init_struct.frequency = div;
    pwm_init_struct.pwm.Sync = Sync_enable;
    pwm_init_struct.pwm.HighLevelPeriod = pcnt;
    pwm_init_struct.pwm.LowLevelPeriod = ncnt;
    pwm_init_struct.pwm.LevelProOutput = PWM_HIGHF_DISABLE;
    TIM_Init(&pwm_init_struct);
}
