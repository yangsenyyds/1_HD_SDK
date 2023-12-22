#include "ir_learn_from_tv_google.h"
#include "ir_learn_from_remote.h"
#include "ir_load.h"
#include "ir_send.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "flash.h"
#include "app_config.h"
#include "yc_debug.h"
#include "remote_control.h"

#define IR_3BIT(reg)		(((int)(*(reg)<< 16) | ((int)*(reg + 1) << 8) | ((int)*(reg + 2))) / 10)
#define IR_2BIT(reg)		((((int)*(reg) << 8) | ((int)*(reg + 1))) / 10)

#define IR_3BIT_FILL(reg)		(((int)(*(reg)<< 16) | ((int)*(reg + 1) << 8)) / 10)
#define IR_2BIT_FILL(reg)		(((int)*(reg) << 8) / 10)
static bool ir_learn_done;

#define FREQ_25KHZ_L (250)
#define FREQ_30KHZ_H (300)
#define FREQ_32KHZ_L (320)
#define FREQ_34KHZ_H (340)
#define FREQ_37KHZ_L (370)
#define FREQ_39KHZ_H (390)
#define FREQ_41KHZ_L (410)
#define FREQ_55KHZ_H (550)
#define FREQ_57KHZ_H (570)
enum{
    IR_TYPE_NEC = 2,
    IR_TYPE_REPORT = 3,
    IR_TYPE_TOGGLE = 4,

    IR_TYPE = 1,
    IR_FREQ = IR_TYPE + 2,
    IR_TIME_NUM = IR_FREQ + 2,
    IR_TIME_START = IR_TIME_NUM + 4,
    IR_TIME_BIT = IR_TIME_START + 5,
};

void ir_tv_learn_send(uint16_t keynum)
{
    memset(&irparams, 0x00, sizeof(irparams));
    switch (keynum)
    {
        case POWER_KEYNUM: 
        {
            if(!flash_read(HD_POWER_DATA, (uint8_t *)&irparams,sizeof(irparams))){
                ir_remote_learn_send(irparams);
            }
        }
            break;
        case VOL_KEYNUM:
        {
            if(!flash_read(HD_VOL_DATA, (uint8_t *)&irparams,sizeof(irparams))){
                ir_remote_learn_send(irparams);
            }
        } 
            break;
        case VOL__KEYNUM:
        {
            if(!flash_read(HD_VOL__DATA, (uint8_t *)&irparams,sizeof(irparams))){
                ir_remote_learn_send(irparams);
            }
        } 
            break;
        case MUTE_KEYNUM:
        {
            if(!flash_read(HD_MUTE_DATA, (uint8_t *)&irparams, sizeof(irparams))){
                ir_remote_learn_send(irparams); 
            }
        }
            break;
        case INPUT_KEYNUM:
        {
            if(!flash_read(HD_INPUT__DATA, (uint8_t *)&irparams, sizeof(irparams))){
                ir_remote_learn_send(irparams); 
            }
        }
            break;
            
    }
   
}

void ir_learn_data_fill(uint8_t *tv_ir_learn_data)
{
    uint16_t ir_freq = 0, buf_num = 0,data_len_offest = 0;
    memset(&irparams,0,sizeof(irparams));

    ir_freq = (((uint16_t )tv_ir_learn_data[IR_FREQ] << 8) | tv_ir_learn_data[IR_FREQ+ 1]);
    if(ir_freq > FREQ_25KHZ_L && ir_freq < FREQ_30KHZ_H)
    {
        irparams.freq = 27;
    }
    else if(ir_freq > FREQ_30KHZ_H && ir_freq < FREQ_32KHZ_L){
        irparams.freq = 31;
    }
    else if(ir_freq > FREQ_32KHZ_L && ir_freq < FREQ_34KHZ_H){
        irparams.freq = 33;
    }
    else if(ir_freq > FREQ_34KHZ_H && ir_freq < FREQ_37KHZ_L){
        irparams.freq = 36;
    }
    else if(ir_freq > FREQ_37KHZ_L && ir_freq < FREQ_39KHZ_H){
        irparams.freq = 38;
    }
    else if(ir_freq > FREQ_39KHZ_H && ir_freq < FREQ_41KHZ_L){
        irparams.freq = 40;
    }
    else if(ir_freq > FREQ_41KHZ_L && ir_freq < FREQ_55KHZ_H){
        irparams.freq = 56;
    }
    else if(ir_freq > FREQ_55KHZ_H && ir_freq < FREQ_57KHZ_H){
        irparams.freq = 0;
    }
    else{
        irparams.freq = 38;
    }

    irparams.len = (((uint16_t )tv_ir_learn_data[IR_TIME_NUM] << 8) | tv_ir_learn_data[IR_TIME_NUM+ 1]) * 2;
    irparams.rawbuf[buf_num++] =IR_3BIT(&tv_ir_learn_data[IR_TIME_START]);
    irparams.rawbuf[buf_num++] =IR_2BIT(&tv_ir_learn_data[IR_TIME_START + 3]);
    for(uint8_t i = 0; i < (irparams.len - 4); i++){
        irparams.rawbuf[buf_num++] = IR_2BIT(&tv_ir_learn_data[IR_TIME_BIT + i*2]);
    }
    irparams.rawbuf[buf_num++] = IR_2BIT_FILL(&tv_ir_learn_data[IR_TIME_BIT + ((irparams.len - 4) *2)]);
    irparams.rawbuf[buf_num++] = IR_3BIT_FILL(&tv_ir_learn_data[IR_TIME_BIT + ((irparams.len - 4) *2) + 1]);

    if(tv_ir_learn_data[IR_TYPE] == IR_TYPE_REPORT) {
        irparams.repeat = 1;
        data_len_offest = IR_TIME_START + irparams.len * 2;
            irparams.rawbuf[buf_num++] = IR_3BIT(&tv_ir_learn_data[data_len_offest]);
            data_len_offest += 3;
            irparams.rawbuf[buf_num++] = IR_2BIT(&tv_ir_learn_data[data_len_offest]);
            data_len_offest += 2;
            irparams.rawbuf[buf_num++] = IR_2BIT_FILL(&tv_ir_learn_data[data_len_offest]);
            data_len_offest += 1;
            irparams.rawbuf[buf_num++] = IR_3BIT_FILL(&tv_ir_learn_data[data_len_offest]);
            irparams.repeat_stop = buf_num;
    }
    else if(tv_ir_learn_data[IR_TYPE] == IR_TYPE_TOGGLE) {
        irparams.repeat = 2;
    }
DEBUG_LOG_STRING("flash_record_exist(ir_learn_tag) = %d \r\n",flash_record_exist(ir_learn_tag));
    if (flash_record_exist(ir_learn_tag)) {
        uint8_t learn_tag = 1;
        flash_write(ir_learn_tag, (uint8_t *)&learn_tag, sizeof(learn_tag),STATE_INF);
        ir_learn_done = true;
    }

    switch (tv_ir_learn_data[0])
    {
    case 0x18:
        flash_write(HD_VOL_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF); 
        DEBUG_LOG_STRING("HD_VOL_DATA\r\n"); 
        break;
    case 0x19:
        flash_write(HD_VOL__DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);  
        DEBUG_LOG_STRING("HD_VOL__DATA\r\n"); 
        break;
    case 0xA4:
        flash_write(HD_MUTE_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);
        DEBUG_LOG_STRING("HD_MUTE_DATA\r\n");
        break;
    case 0x1A:
        flash_write(HD_POWER_DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);  
        DEBUG_LOG_STRING("HD_POWER_DATA\r\n");        
        break;                
    case 0xB2:
        flash_write(HD_INPUT__DATA, (uint8_t *)&irparams, sizeof(irparams),CHUNK_INF);  
        DEBUG_LOG_STRING("HD_INPUT__DATA\r\n");        
        break;        
    }
    // DEBUG_LOG_STRING("irparams.freq = %d irparams.len = %d irparams.repeat = %d irparams.repeat_stop = %d\r\n",irparams.freq,irparams.len,irparams.repeat,irparams.repeat_stop);
    // for(uint16_t i = 0; i < buf_num;i++) {
    //     DEBUG_LOG_STRING("time%d = %d \r\n",i,irparams.rawbuf[i]);
    // }
}

void ir_learn_data_clr(void){
    flash_Erase(ir_learn_tag,8);
    flash_Erase(HD_VOL__DATA,sizeof(irparams));
    flash_Erase(HD_VOL_DATA,sizeof(irparams));
    flash_Erase(HD_MUTE_DATA,sizeof(irparams));
    flash_Erase(HD_INPUT__DATA,sizeof(irparams));
    flash_Erase(HD_POWER_DATA,sizeof(irparams));
}

void ir_learn_init(void)
{
    if (flash_record_exist(ir_learn_tag)) {
        ir_learn_done = false;
    }
    else {
        ir_learn_done = true;
    }
}

bool get_ir_learn_state(void)
{
    return ir_learn_done;
}