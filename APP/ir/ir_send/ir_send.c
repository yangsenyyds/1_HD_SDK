#include "remote_control.h"
#include "ir_send.h"
#include "yc11xx_gpio.h"
#include "yc11xx_systick.h"
#include "yc11xx_pwm.h"


#define PWM_CLK_FREQ 24000  //12MHz
#define IR_CLK_FREQ 38      //38kHz
#define IR_DUTY_CYCLE 0.4   //DUTY 1:3
#define IR_PCNT_RMT PWM_CLK_FREQ / IR_CLK_FREQ* IR_DUTY_CYCLE
#define IR_NCNT_RMT PWM_CLK_FREQ / IR_CLK_FREQ*(1 - IR_DUTY_CYCLE)

static bool key_pressed;

static uint16_t ir_pcnt = IR_PCNT_RMT;
static uint16_t ir_ncnt = IR_NCNT_RMT;
static uint8_t clk_div = 1;
uint8_t get_key_press_state(void){
    return key_pressed;
}

void set_key_press_state(bool state)
{
    key_pressed = state;
}

void set_pwm_freq_div(void)
{
    clk_div = System_WorkClockGet() / 24000000; /// clk dir
}

/// default PWM 24M
void ir_set_freq(uint16_t freq)
{
    ir_pcnt = 24000000 / freq * IR_DUTY_CYCLE;
    ir_ncnt = 24000000 / freq * (1 - IR_DUTY_CYCLE);
}

void ir_gpio_send(IR_CODE_VAL codeVal, uint32_t time)
{
    if (codeVal) {
        GPIO_Init(IR_IO, GPIO_Mode_Out_High);
    }
    else {
        GPIO_Init(IR_IO, GPIO_Mode_Out_Low);
    }

    SysTick_DelayUs(time);
}

void ir_pwm_send(IR_CODE_VAL codeVal, uint32_t time)
{
    if (codeVal) {
        PWM_Config(IR_IO, ir_pcnt, ir_ncnt, clk_div - 1);
    }
    else {
        PWM_DisConfig(IR_IO);
        GPIO_Init(IR_IO, GPIO_Mode_Out_Low);
    }

    SysTick_DelayUs(time);
}



