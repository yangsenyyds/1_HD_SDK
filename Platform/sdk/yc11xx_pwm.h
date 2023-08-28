#ifndef _YC11XX_PWM_H_
#define _YC11XX_PWM_H_
#include "yc11xx.h"
#include "yc11xx_timer.h"
#include "yc11xx_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


void PWM_Config(GPIO_X gpio_pin, uint16_t pcnt, uint16_t ncnt,uint8_t div);
void PWM_DisConfig(GPIO_X gpio_pin);
void PWM_Config_Sync(GPIO_X gpio_pin, uint16_t pcnt, uint16_t ncnt,uint8_t div);

#endif  /* _YC11XX_PWM_H_ */
