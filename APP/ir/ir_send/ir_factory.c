#include "remote_control.h"
#include "ir_send.h"
#include "yc11xx_gpio.h"
#include "yc11xx_systick.h"
#include "yc11xx_pwm.h"


#define PWM_CLK_FREQ 24000  //12MHz
#define IR_CLK_FREQ 38      //38kHz
#define IR_DUTY_CYCLE 0.4   //DUTY 1:3

static uint32_t ir_pulse_data[256];
static uint8_t length;

static uint16_t ir_pcnt = PWM_CLK_FREQ / IR_CLK_FREQ* IR_DUTY_CYCLE;
static uint16_t ir_ncnt = PWM_CLK_FREQ / IR_CLK_FREQ*(1 - IR_DUTY_CYCLE);

static uint16_t bit0_l_tm = 550;
static uint16_t bit0_h_tm = 550;
static uint16_t bit1_l_tm = 550;
static uint16_t bit1_h_tm = 1660;


void ir_pwm_send(IR_CODE_VAL codeVal, uint32_t time)
{
    if (codeVal) {
        PWM_Config(IR_IO, ir_pcnt, ir_ncnt, 0);
    }
    else {
        PWM_DisConfig(IR_IO);
        GPIO_Init(IR_IO, GPIO_Mode_Out_Low);
    }

    SysTick_DelayUs(time);
}

void ir_send(void)
{
    for (uint16_t i = 0; i < length; i)
    {
        if (ir_pulse_data[i] == 0) {
            i++;
            if (ir_pulse_data[i] != 0) {
                ir_pwm_send(0, ir_pulse_data[i]);
            }
            i++;
        }
        else if (ir_pulse_data[i] != 0) {
            i++;
            if (ir_pulse_data[i] != 0) {
                ir_pwm_send(1, ir_pulse_data[i]);
            }
            i++;
        }
    }
    if (get_key_press_state()) {
        app_queue_insert(ir_send);
    }
}

static void upt_pwm_time_pulse(uint32_t *data)
{
    uint32_t high = *data++;
    uint32_t low = *data++;
    ir_pulse_data[length] = 1; ///打开载波
    ir_pulse_data[length + 1] = high; ///打开载波
    length += 2;

    ir_pulse_data[length] = 0; ///关闭载波
    ir_pulse_data[length + 1] = low; ///关闭载波
    length += 2;
}

static void bit_time(uint16_t data, uint8_t len)
{
    uint32_t buf[2];
    if (len == 0xF) {
        len += 1;
    }
    for (uint8_t i = 0; i < len; i++)
    {
        if (data & 0x01) {			/// '1'
            buf[0] = bit1_l_tm;
            buf[1] = bit1_h_tm;
            upt_pwm_time_pulse(buf);
        }
        else {						/// '0'
            buf[0] = bit0_l_tm;
            buf[1] = bit0_h_tm;
            upt_pwm_time_pulse(buf);
        }
        data >>= 1; /// 先发低位
    }
}

static uint8_t ir_data_ready(uint16_t irnum)
{
    memset((void *)&ir_send_param, 0, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    uint32_t lead_buf[2] = {4500,4470};
    upt_pwm_time_pulse(lead_buf);
   
    bit_time(0x0E, 8);
    bit_time(0x0E, 8);
    bit_time(irnum, 8);
    bit_time(~irnum, 8);
    
    uint32_t stop_buf[2] = {540,45500};
    upt_pwm_time_pulse(stop_buf);

    return 0;
}

static void 