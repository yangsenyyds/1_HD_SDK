#include "ir_learn_from_tv.h"
#include "ir_send.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "flash.h"
#include "app_config.h"

#include "remote_control.h"

#define FREQ_30KHZ_L (29000)
#define FREQ_30KHZ_H (31000)
#define FREQ_33KHZ_L (32000)
#define FREQ_33KHZ_H (34000)
#define FREQ_36KHZ_L (35000)
#define FREQ_36KHZ_H (37000)
#define FREQ_38KHZ_L (37000)
#define FREQ_38KHZ_H (39000)
#define FREQ_40KHZ_L (39000)
#define FREQ_40KHZ_H (41000)
#define FREQ_56KHZ_L (55000)
#define FREQ_56KHZ_H (57000)

#define STR_f_L "f["
#define STR_f_L_LEN (sizeof(STR_f_L) - 1)
#define STR_R_c_L "]c["
#define STR_R_c_L_LEN (sizeof(STR_R_c_L) - 1)
#define STR_R_l_L "]l["
#define STR_R_l_L_LEN (sizeof(STR_R_l_L) - 1)
#define STR_R_r_L "]r["
#define STR_R_r_L_LEN (sizeof(STR_R_r_L) - 1)
#define STR_R_d_L "]d["
#define STR_R_d_L_LEN (sizeof(STR_R_d_L) - 1)
#define STR_R_t_L "]t["
#define STR_R_t_L_LEN (sizeof(STR_R_t_L) - 1)
#define STR_R "]"
#define STR_R_LEN (sizeof(STR_R) - 1)

#define STR_LEN_STOP_MAX (2048)
#define IR_LEARN_DATA_BIT_MAX (10)
#define IR_LERAN_ORG_DATA_ARRAY_LEN (512)

#pragma pack(1)
typedef struct {
    uint16_t repeat;
    uint16_t freq;
    uint32_t num;
    uint16_t period[IR_LERAN_ORG_DATA_ARRAY_LEN];
} ir_learn_param_t;
#pragma pack()

static const uint8_t ir_learn_end_buf[] = { 0x05, 0x00, 0x01, 0x00, 0x00 };

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t ir_learn_data_buf[IR_LEARN_DATA_BUFSIZE];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static ir_learn_param_t ir_learn_param;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint16_t str_stop;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static char *str_p;
static uint32_t ir_learn_record = ir_power;
static bool ir_learn_done;


static uint16_t get_freq_type(uint16_t freq)
{
    uint16_t ret = 0xFF;

    if ((freq > FREQ_30KHZ_L) && (freq < FREQ_30KHZ_H)) {
        ret = FREQ_30KHZ_H - 1000;
    }
    else if ((freq > FREQ_33KHZ_L) && (freq < FREQ_33KHZ_H)) {
        ret = FREQ_33KHZ_H - 1000;
    }
    else if ((freq > FREQ_36KHZ_L) && (freq < FREQ_36KHZ_H)) {
        ret = FREQ_36KHZ_H - 1000;
    }
    else if ((freq > FREQ_38KHZ_L) && (freq < FREQ_38KHZ_H)) {
        ret = FREQ_38KHZ_H - 1000;
    }
    else if ((freq > FREQ_40KHZ_L) && (freq < FREQ_40KHZ_H)) {
        ret = FREQ_40KHZ_H - 1000;
    }
    else if ((freq > FREQ_56KHZ_L) && (freq < FREQ_56KHZ_H)) {
        ret = FREQ_56KHZ_H - 1000;
    }

    return ret;
}

static void ir_data_load(void)
{
    app_sleep_lock_set(IR_LOCK, true);
	
    uint16_t edge_num = ir_learn_param.num >> 2;
    for (uint16_t i = 0; i < edge_num; i++) {
        ir_pwm_send(IR_START, ir_learn_param.period[i * 2] * 10);
        ir_pwm_send(IR_STOP, ir_learn_param.period[i * 2 + 1] * 10);
    }

    ir_pwm_send(IR_STOP, 1);
    
    if (get_key_press_state()) {
        app_queue_insert(ir_data_load);
    }
	else {
        app_sleep_lock_set(IR_LOCK, false);
    }
}

void ir_tv_learn_send(uint16_t keynum)
{
    if (!ir_learn_done) return;

    memset(&ir_learn_param, 0x00, sizeof(ir_learn_param_t));
    switch (keynum)
    {
        case PWR_KEYNUM: 
        {
            flash_read(ir_power, (uint8_t *)&ir_learn_param,sizeof(ir_learn_param));
            uint16_t freq_type = get_freq_type(ir_learn_param.freq);

            if (freq_type != 0xFF) {
                ir_set_freq(freq_type);
            }
            app_queue_insert(ir_data_load);
        }
            break;
        case VOL_UP_KEYNUM:
        {
            flash_read(ir_vol, (uint8_t *)&ir_learn_param,sizeof(ir_learn_param));
            uint16_t freq_type = get_freq_type(ir_learn_param.freq);
            if (freq_type != 0xFF) {
                ir_set_freq(freq_type);
            }
            app_queue_insert(ir_data_load);
        } 
            break;
        case VOL_DOWN_KEYNUM:
        {
            flash_read(ir_vol_, (uint8_t *)&ir_learn_param,sizeof(ir_learn_param));
            uint16_t freq_type = get_freq_type(ir_learn_param.freq);
            if (freq_type != 0xFF) {
                ir_set_freq(freq_type);
            }
            app_queue_insert(ir_data_load);
        } 
            break;
        case MUTE_KEYNUM:
        {
            flash_read(ir_mute, (uint8_t *)&ir_learn_param, sizeof(ir_learn_param));
            uint16_t freq_type = get_freq_type(ir_learn_param.freq);
            if (freq_type != 0xFF) {
                ir_set_freq(freq_type);
            }
            app_queue_insert(ir_data_load);
        }
            break;

        default:
            break;
    }
}

void ir_learn_data_fill(void)
{
    uint8_t cyc_cnt = 0;
    char str_array[IR_LEARN_DATA_BIT_MAX] = { 0 };
    uint16_t bit_num;

    while (str_stop++ < STR_LEN_STOP_MAX)
    {
        str_p = strstr((void *)&ir_learn_data_buf[str_stop], (void *)STR_f_L);
        if (str_p)
        {
            str_p += STR_f_L_LEN;
            cyc_cnt = 0;
            memset((void *)str_array, 0, IR_LEARN_DATA_BIT_MAX);
            while ((*str_p != ']') && (isxdigit(*str_p)) && (cyc_cnt < IR_LEARN_DATA_BIT_MAX)) {
                str_array[cyc_cnt++] = *str_p++;
            }
            if (cyc_cnt >= IR_LEARN_DATA_BIT_MAX) {
                return;
            }
            ir_learn_param.freq = atol((void *)str_array);
            break;
        }
    }
    if (str_stop >= STR_LEN_STOP_MAX) {
        return;
    }

    str_p = strstr((void *)str_p, (void *)STR_R_c_L);
    if (str_p) {
        str_p += STR_R_c_L_LEN;
    }
    else {
        return;
    }

    str_p = strstr((void *)str_p, (void *)STR_R_l_L);
    if (str_p)
    {
        str_p += STR_R_l_L_LEN;
        cyc_cnt = 0;
        memset((void *)str_array, 0, IR_LEARN_DATA_BIT_MAX);
        while ((*str_p != ']') && (isxdigit(*str_p)) && (cyc_cnt < IR_LEARN_DATA_BIT_MAX)) {
            str_array[cyc_cnt++] = *str_p++;
        }
        if (cyc_cnt >= IR_LEARN_DATA_BIT_MAX) {

            return;
        }
        bit_num = atol((void *)str_array);

        str_p += 2;
        cyc_cnt = 0;
        memset((void *)str_array, 0, IR_LEARN_DATA_BIT_MAX);
        while ((*str_p != ']') && (isxdigit(*str_p)) && (cyc_cnt < IR_LEARN_DATA_BIT_MAX)) {
            str_array[cyc_cnt++] = *str_p++;
        }
        if (cyc_cnt >= IR_LEARN_DATA_BIT_MAX) {

            return;
        }
        bit_num += atol((void *)str_array);
    }
    else {
        return;
    }

    str_p = strstr((void *)str_p, (void *)STR_R_r_L);
    if (str_p)
    {
        str_p += STR_R_r_L_LEN;
        cyc_cnt = 0;
        memset((void *)str_array, 0, IR_LEARN_DATA_BIT_MAX);
        while ((*str_p != ']') && (isxdigit(*str_p)) && (cyc_cnt < IR_LEARN_DATA_BIT_MAX)) {
            str_array[cyc_cnt++] = *str_p++;
        }
        if (cyc_cnt >= IR_LEARN_DATA_BIT_MAX) {
            return;
        }
        ir_learn_param.repeat = atol((void *)str_array);
    }
    else {
        return;
    }

    str_p = strstr((void *)str_p, (void *)STR_R_d_L);

    if (str_p) {
        str_p += STR_R_d_L_LEN;
    }
    else {
        return;
    }

    str_p = strstr((void *)str_p, (void *)STR_R_t_L);

    if (str_p) {
        str_p += STR_R_t_L_LEN;
    }
    else {
        return;
    }

    str_p = strstr((void *)str_p, (void *)STR_R);
    if (str_p)
    {
        str_p += STR_R_LEN + 1;
        ir_learn_param.num = 0;
        while ((ir_learn_param.num < (bit_num << 2) + 1) && (ir_learn_param.num < STR_LEN_STOP_MAX)
            && memcmp((void *)str_p, (void *)ir_learn_end_buf, sizeof(ir_learn_end_buf))) {
            memcpy((void *)&ir_learn_param.period[ir_learn_param.num >> 1], str_p, 4);
            ir_learn_param.num += 4;
            str_p += 4;
        }
        if (ir_learn_param.num > STR_LEN_STOP_MAX) {
            return;
        }
        flash_write(ir_learn_record, (uint8_t *)&ir_learn_param, sizeof(ir_learn_param),DATA_INF);
    }
    else {
        return;
    }

    if (ir_learn_record == ir_mute)
    {
        str_stop = 0;
        ir_learn_done = true;
        ir_learn_record = ir_power;
        memset((void *)ir_learn_data_buf, 0, sizeof(ir_learn_data_buf));
        return;
    }
    else {
        ir_learn_record += 0x8;
        ir_learn_data_fill();
    }
}

uint8_t *get_ir_learn_data_buf_addr(uint16_t offset)
{
    return &ir_learn_data_buf[offset];
}

void ir_learn_data_clr(void)
{
    flash_Erase(ir_power, sizeof(addr_inf_t));
    flash_Erase(ir_vol, sizeof(addr_inf_t));
    flash_Erase(ir_vol_, sizeof(addr_inf_t));
    flash_Erase(ir_mute, sizeof(addr_inf_t));
    ir_learn_done = false;
}

void ir_learn_reinit(void)
{
    str_stop = 0;
    str_p = NULL;
    memset(&ir_learn_param, 0, sizeof(ir_learn_param));
    memset(ir_learn_data_buf, 0, sizeof(ir_learn_data_buf));
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
