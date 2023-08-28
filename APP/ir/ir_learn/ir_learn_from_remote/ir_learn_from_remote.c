#include "ir_learn_from_remote.h"
#include "keyscan.h"
#include "yc11xx_gpio.h"
#include "yc11xx_systick.h"
#include "yc_debug.h"
#include "app_config.h"

#include "remote_control.h"

enum {
    IR_STATE_MARK,
    IR_STATE_SPACE,
    IR_REPEAT_MARK,
    IR_REPEAT_SPACE,
    IR_STOP,
};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t done;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t repeat_len;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t raw_len;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t tIRInput;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint32_t Tick;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t State;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint32_t tempEndTimer;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint32_t repear_code[RAW_BUFFER_LENGTH];

MEMORY_NOT_PROTECT_UNDER_LPM_ATT uint32_t tempStartTimer;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT ir_receive_param_t irparams;

static uint32_t SYS_TimerRead(void)
{
    return *(INITCPU_SYST_CVR);
}

static uint32_t SYS_TimerCountGet(uint32_t counter_start, uint32_t counter_end)
{
    uint32_t counter_1us = System_WorkClockGet() / 1000000; //1 us == 24

    if (counter_start == counter_end){ //120000 - 80000
        return 0;
    }
    if (counter_start > counter_end){
        return (counter_start - counter_end) / counter_1us;
    }
    else{
        return (*(INITCPU_SYST_RVR)-counter_end + counter_start) / counter_1us;
    }
}

static uint8_t ir_get_freq(uint8_t freq)
{
	uint8_t freq_temp = 1000/freq;
	
	if(freq_temp > 30 && freq_temp <= 33)
		return (uint8_t)33;
	else if(freq_temp > 33 && freq_temp <= 36)
		return (uint8_t)36;
	else if(freq_temp > 36 && freq_temp < 40)
		return (uint8_t)38;
	else if(freq_temp >= 40 && freq_temp < 45)
		return (uint8_t)40;
	else if(freq_temp > 70 && freq_temp <100)
		return (uint8_t)56;
	else
		return (uint8_t)38;
}

uint8_t ir_receive(void)
{
    done = 0;
    repeat_len = 0;    
    raw_len = 0;
    tIRInput = 0;
    Tick = 0;
    State = 0;
    tempStartTimer = 0;
    memset(&irparams,0,sizeof(irparams));
    memset(repear_code,0,sizeof(repear_code));
	

    while(GPIO_ReadDataBit(IR_RCV_PIN))
    {
        tempStartTimer++;
        if(tempStartTimer > 3000000 ){
            return IR_LEARN_OVERTIME;
        }else if(key_wakeup_get()){
            return IR_LEARN_BACK_KEYNUM;
        }
    }

    tempStartTimer = SYS_TimerRead();
    while(!GPIO_ReadDataBit(IR_RCV_PIN));
	while(GPIO_ReadDataBit(IR_RCV_PIN));
	tempEndTimer = SYS_TimerRead();
    
    while (1)
	{
		SysTick_DelayUs(3);
        uint8_t Input_Level = GPIO_ReadDataBit(IR_RCV_PIN);

        if (State == IR_STATE_MARK)
		{
            if (Input_Level == INPUT_MARK || done == 1) {
                Tick++;
                if (done == 0) {
                    done = 1;
                }
            }
            if (Input_Level != INPUT_MARK && done == 1)
            {
                tIRInput++;
                if (tIRInput > 15) { //23 改为 10
                    irparams.rawbuf[raw_len] = Tick - tIRInput;
                    raw_len++;
                    Tick = tIRInput;
                    done = 0;
                    tIRInput = 0;
                    State = IR_STATE_SPACE;
                }
            }
            else {
                tIRInput = 0;
            }
        }
        else if (State == IR_STATE_SPACE)
		{
            if (Input_Level != INPUT_MARK) {
                Tick++;
                if (Tick > 20000) {
                    State = IR_STOP;
                }
            }
            else
            {
                irparams.rawbuf[raw_len++] = Tick;
                if (Tick < 1200) {
                    State = IR_STATE_MARK;
                }
                else {
                    memset(repear_code, 0x00, sizeof(repear_code));
                    repeat_len = 0;
                    State = IR_REPEAT_MARK;
                }
                Tick = 0;
            }
        }
        else if (State == IR_REPEAT_MARK)
		{
            if (Input_Level == INPUT_MARK || done == 1) {
                Tick++;
                if (done == 0) {
                    done = 1;
                }
            }
            if (Input_Level != INPUT_MARK && done == 1)
            {
                tIRInput++;
                if (tIRInput > 15) {//23 改为10
                    repear_code[repeat_len++] = Tick - tIRInput;
                    Tick = tIRInput;
                    done = 0;
                    tIRInput = 0;
                    State = IR_REPEAT_SPACE;
                }
            }
            else {
                tIRInput = 0;
            }
        }
        else if (State == IR_REPEAT_SPACE)
		{
            if (Input_Level != INPUT_MARK) {
                Tick++;
                if (Tick > 20000) {
                    State = IR_STOP;
                }
            }
            else
            {
                repear_code[repeat_len++] = Tick;
                if (Tick < 1200) {
                    State = IR_REPEAT_MARK;
                }
                else {
                    State = IR_STOP;
                }
                Tick = 0;
            }
        }
        else if (State == IR_STOP)
		{
            uint8_t repeat_time = 0;

            irparams.len = raw_len;
            irparams.rawbuf[0] += 10;
            if(raw_len > RAW_BUFFER_LENGTH){
                return IR_LEARN_FAIL;
            }
            for (uint8_t i = 0; i < raw_len; i++) {
                irparams.rawbuf[i] *= 6.8;
                // DEBUG_LOG_STRING("rawbuf[%d] = %d  ok\r\n",i, irparams.rawbuf[i]);
            }
            for (uint8_t i = 0; i < repeat_len; i++) {
                repear_code[i] *= 6.8;
                // DEBUG_LOG_STRING("repear_code[%d] = %d  ok\r\n",i, repear_code[i]);
            }

            if (repeat_len != raw_len && repeat_len >= 4)
            {
                if ((irparams.rawbuf[0] >= (repear_code[0] + 70)) || (irparams.rawbuf[0] <= (repear_code[0]) - 70)) {
                    repeat_time++;
                }
                if ((irparams.rawbuf[1] >= (repear_code[1] + 70)) || (irparams.rawbuf[1] <= (repear_code[1]) - 70)) {
                    repeat_time++;
                }
                if ((irparams.rawbuf[2] >= (repear_code[2] + 70)) || (irparams.rawbuf[2] <= (repear_code[2]) - 70)) {
                    repeat_time++;
                }
                if (((repear_code[3] > 10000) && (repear_code[3] < 100000)) || ((irparams.rawbuf[3] >= (repear_code[3] + 70)) || (irparams.rawbuf[3] <= (repear_code[3]) - 70))) {
                    repeat_time++;
                    repeat_time++;
                }
                if (((repear_code[4] > 10000) && (repear_code[4] < 100000)) || ((irparams.rawbuf[4] >= (repear_code[4] + 70)) || (irparams.rawbuf[4] <= (repear_code[4]) - 70))) {
                    repeat_time++;
                    repeat_time++;
                }
                if (((repear_code[5] > 10000) && (repear_code[5] < 100000)) || ((irparams.rawbuf[5] >= (repear_code[5] + 70)) || (irparams.rawbuf[5] <= (repear_code[5]) - 70))) {
                    repeat_time++;
                    repeat_time++;
                }
                if (repeat_time >= 2) {
                    irparams.repeat = 1;
                    DEBUG_LOG_STRING("repeat 4");
                    irparams.repeat_stop = repeat_len;
                    memcpy(&irparams.rawbuf[irparams.len], &repear_code[0], repeat_len * sizeof(uint32_t));
                }
            }
            else if(repeat_len == raw_len)
            {
                for(uint8_t i = 0; i < raw_len - 1; i++){
                    if ((irparams.rawbuf[i] >= (repear_code[i] + 70)) || (irparams.rawbuf[i] <= (repear_code[i]) - 70)) {
                        repeat_time++;
                    }
                }
                if(repeat_time >= 8){
                    DEBUG_LOG_STRING("repeat 8");
                    irparams.repeat = 1;
                    irparams.repeat_stop = repeat_len;
                    memcpy(&irparams.rawbuf[irparams.len], &repear_code[0], repeat_len * sizeof(uint32_t));
                }
            }

            if (irparams.rawbuf[irparams.len - 1] >= 8000 && irparams.len >= 10) {
                DEBUG_LOG_STRING("SYS_TimerCountGet(tempStartTimer,tempEndTimer)  = %d\r\n",SYS_TimerCountGet(tempStartTimer,tempEndTimer));
                
                irparams.freq = ir_get_freq((uint8_t)SYS_TimerCountGet(tempStartTimer,tempEndTimer));
				return IR_LEARN_SUCE;
            }
            else {
                return IR_LEARN_FAIL;
            }
        }
    }  
}
