#ifndef _YC11XX_TIMER_H_
#define _YC11XX_TIMER_H_
#include "yc11xx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    TIM0 = 0,
    TIM1,
    TIM2,
    TIM3,
    TIM4,
    TIM5,
    TIM6,
    TIM7
} TIM_NumTypeDef;
#define TIM_NumTypeDef(TIMx)	(TIMx==TIM0||\
                                 TIMx==TIM1||\
                                 TIMx==TIM2||\
                                 TIMx==TIM3||\
                                 TIMx==TIM4||\
                                 TIMx==TIM5||\
                                 TIMx==TIM6||\
                                 TIMx==TIM7)

#define FREQUENCY_DIVISION_0		0x0
#define FREQUENCY_DIVISION_1		0x1
#define FREQUENCY_DIVISION_2		0x2
#define FREQUENCY_DIVISION_3		0x3
#define FREQUENCY_DIVISION_4		0x4
#define FREQUENCY_DIVISION_5		0x5
#define FREQUENCY_DIVISION_6		0x6
#define FREQUENCY_DIVISION_7		0x7
#define IS_FREQUENCY_DIVISION(FREQUENCY)		((FREQUENCY == FREQUENCY_DIVISION_0)||\
                                                (FREQUENCY == FREQUENCY_DIVISION_1) ||\
                                                (FREQUENCY == FREQUENCY_DIVISION_2)	||\
                                                (FREQUENCY == FREQUENCY_DIVISION_3) ||\
                                                (FREQUENCY == FREQUENCY_DIVISION_4)	||\
                                                (FREQUENCY == FREQUENCY_DIVISION_5) ||\
                                                (FREQUENCY == FREQUENCY_DIVISION_6)	||\
                                                (FREQUENCY == FREQUENCY_DIVISION_7))

/**
  * @brief load pwm configuration_value
  */
typedef enum
{
    Load_default  = 0,
    Load_enable = 0x80
} LOAD_TypeDef;
#define IS_PWM_LOAD(LOAD)		(LOAD == Load_default || LOAD == Load_enable)

/**
  *@brief Sync mode control
  */
typedef enum
{
    Sync_default = 0,
    Sync_enable  = 0x08
} PWM_SyncTypedef;
#define IS_PWM_SYNC(SYNC)		(SYNC == Sync_default || SYNC == Sync_enable)



/**
  *@brief High first.
  */
#define PWM_HIGHF_ENABLE  0x10
#define PWM_HIGHF_DISABLE 0x0



/**
  * @brief  PWM Init Structure
  */
typedef struct
{
    uint16_t LowLevelPeriod;
    uint16_t HighLevelPeriod;
    LOAD_TypeDef LoadMode;
    PWM_SyncTypedef Sync;
    uint8_t LevelProOutput;
} PWM_InitTypeDef;

/**
 * @brief mode selection
 */
typedef enum
{
    TIM_Mode_PWM	= 0x20,
    TIM_Mode_TIMER	= 0xe0
} TIM_ModeTypeDef;
#define IS_TIM_MODE(MODE)		(MODE == TIM_Mode_TIMER || MODE == TIM_Mode_PWM)

/**
  * @brief  timer Init Structure
  */
typedef struct
{
    TIM_NumTypeDef TIMx;
    uint16_t period;
    uint8_t frequency;
    TIM_ModeTypeDef mode;
    PWM_InitTypeDef pwm;
    
} TIM_InitTypeDef;

#define IS_PERIOD_VALUE(x) (x<=0xffff)
#define PWM_LOAD_MODE_BIT  1 << 7

void TIM_Init(TIM_InitTypeDef *tim_init_struct);
void TIM_Cmd(TIM_NumTypeDef timx, FunctionalState state);

#endif /* _YC11XX_TIMER_H_ */
