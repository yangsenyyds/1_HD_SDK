/**
 * @file yc11xx_timer.c
 * @brief TIMER driver
 * @date 2021-07-07
 *
 * Date           Author      Version        Notes
 * 2021-03-11     zhouduo     V1.0.0      the first version
 * 2021-06-15     zhouduo     V1.1.0      the formal version
 */
#include "yc11xx_timer.h"
#include "reg_addr.h"
#include "reg_struct.h"
/**
 * @brief   Init tim with pwm mode
 * @param   TIM_Init_Struct : TIM initializes the structure
 * @retval  none
 */
void TIM_Init(TIM_InitTypeDef *tim_init_struct)
{
	_ASSERT(TIM_NumTypeDef(tim_init_struct->TIMx));
    uint8_t pwm_ctrl = 0;
    uint8_t pwm_ch = 0;
    if (tim_init_struct->mode == TIM_Mode_TIMER)
    {
        pwm_ctrl |= tim_init_struct->mode;
        pwm_ctrl |= tim_init_struct->frequency;
        HWRITEW(reg_map(CORE_PWM_PCNT(tim_init_struct->TIMx)), tim_init_struct->period);
        HWRITE(reg_map(CORE_PWM_CTRL(tim_init_struct->TIMx)), pwm_ctrl);
    }
    else
    {
        HWCOR((CORE_PWM_CTRL(tim_init_struct->TIMx)),PWM_LOAD_MODE_BIT);
        pwm_ctrl |= tim_init_struct->frequency;
        pwm_ctrl |= tim_init_struct->mode;
        pwm_ctrl |= tim_init_struct->pwm.LoadMode;
        pwm_ctrl |= tim_init_struct->pwm.Sync;
        pwm_ctrl |= tim_init_struct->pwm.LevelProOutput;
        HWRITEW(reg_map(CORE_PWM_PCNT(tim_init_struct->TIMx)), tim_init_struct->pwm.HighLevelPeriod);
        HWRITEW(reg_map(CORE_PWM_NCNT(tim_init_struct->TIMx)), tim_init_struct->pwm.LowLevelPeriod);
        HWRITE(reg_map(CORE_PWM_CTRL(tim_init_struct->TIMx)), pwm_ctrl);
    }
    if((tim_init_struct->TIMx % 2) != 0)
    {
        pwm_ch = tim_init_struct->TIMx - 1;
        pwm_ctrl = HREAD(CORE_PWM_CTRL(pwm_ch)) | tim_init_struct->frequency;
        HWRITE(reg_map(CORE_PWM_CTRL(pwm_ch)), pwm_ctrl);
    }
}

/**
 * @brief   enable or disable timer
 * @param   timx : the timer number,TIM0-TIM7
 * @param   state :Fixed FunctionalState enumeration type
 * @retval  none
 */
void TIM_Cmd(TIM_NumTypeDef timx, FunctionalState state)
{
	_ASSERT(TIM_NumTypeDef(timx));
	uint8_t pwm_clken;
    pwm_clken = HREAD(CORE_PWM_CLKEN);
    pwm_clken &=~ (1 << timx);
    pwm_clken |= (state << timx);
    HWRITE(CORE_PWM_CLKEN,pwm_clken);
}






