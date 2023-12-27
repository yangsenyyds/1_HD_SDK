#include "ir_load.h"
#include "ir_send.h"
#include "software_timer.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "yc11xx_gpio.h"
#include "yc11xx_systick.h"
#include "app_config.h"
#include "yc_debug.h"


typedef struct IR_ENDCODE_PARAM {
    bool endcode_en;
    uint16_t endcode_start_offset;
    uint16_t endcode_end_offset;
}ir_endcode_param_t;

typedef struct IR_SEND_PARAM {
    uint16_t length;
    int16_t user_repeat;
    uint32_t *pwm_buf_p;
}ir_send_param_t;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint32_t ir_pulse_data[IR_BUF_SIZE];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint32_t gemini_hand[46];

static ir_endcode_param_t ir_endcode_param;
static ir_send_param_t ir_send_param;

static uint8_t (*ir_ready)(uint16_t irnum);
static void (*pwm_time_pulse)(uint32_t *data);

static uint16_t bit0_h_tm;
static uint16_t bit0_l_tm;
static uint16_t bit1_h_tm;
static uint16_t bit1_l_tm;
static uint32_t rpt_lead_buf[4];
static uint32_t stop_buf[2];
static uint32_t lead_buf[2];
static uint8_t en_type;
static uint8_t user_repeat;
static uint16_t* brand_rpt_code;
static uint16_t* cust_code;
static uint16_t* brand_type;

static bool gemini_state;


static uint16_t coust_code = 0;
static void ir_without_freq(void)
{
    app_sleep_lock_set(IR_LOCK,true);
    for (uint16_t i = 0; i < ir_send_param.length; )
    {
        if (ir_send_param.pwm_buf_p[i] == 0) {
            i++;
            if (ir_send_param.pwm_buf_p[i] != 0) {
                ir_gpio_send(IR_STOP, ir_send_param.pwm_buf_p[i]);
            }
            i++;
        }
        else if (ir_send_param.pwm_buf_p[i] != 0) {
            i++;
            if (ir_send_param.pwm_buf_p[i] != 0) {
                ir_gpio_send(IR_START, ir_send_param.pwm_buf_p[i]);
            }
            i++;
        }
    }

    if (ir_endcode_param.endcode_en) {
        ir_endcode_param.endcode_en = 0;
        ir_send_param.length = ir_endcode_param.endcode_end_offset - ir_endcode_param.endcode_start_offset;
        memcpy(&ir_send_param.pwm_buf_p[0], &ir_send_param.pwm_buf_p[ir_endcode_param.endcode_start_offset], 4 * ir_send_param.length);
    }

    if (get_key_press_state() || ir_send_param.user_repeat-- > 0) {
        app_queue_insert(ir_without_freq);
        return;
    }
    app_sleep_lock_set(IR_LOCK,false);
}

void ir_send(void)
{
    app_sleep_lock_set(IR_LOCK,true);
    DEBUG_LOG_STRING("ir_send\r\n");
    for (uint16_t i = 0; i < ir_send_param.length; i)
    {
        if (ir_send_param.pwm_buf_p[i] == 0) {
            i++;
            if (ir_send_param.pwm_buf_p[i] != 0) {
                ir_pwm_send(0, ir_send_param.pwm_buf_p[i]);
            }
            i++;
        }
        else if (ir_send_param.pwm_buf_p[i] != 0) {
            i++;
            if (ir_send_param.pwm_buf_p[i] != 0) {
                ir_pwm_send(1, ir_send_param.pwm_buf_p[i]);
            }
            i++;
        }
    }
    if (ir_endcode_param.endcode_en)
    {
        ir_endcode_param.endcode_en = 0;
        ir_send_param.length = ir_endcode_param.endcode_end_offset - ir_endcode_param.endcode_start_offset;
        memcpy(&ir_send_param.pwm_buf_p[0], &ir_send_param.pwm_buf_p[ir_endcode_param.endcode_start_offset],
            4 * ir_send_param.length);
    }
    if (get_key_press_state() || ir_send_param.user_repeat-- > 0) {
        app_queue_insert(ir_send);
        return;
    }
    else if (gemini_state == 0 && (en_type == GEMINI_C10_32_08 || en_type == GEMINI_C10_38_09 || en_type == GEMINI_C10_32_10)) {
        gemini_state = 1;
        memcpy(&ir_send_param.pwm_buf_p[0], &gemini_hand[0], sizeof(gemini_hand));
        ir_send_param.length = sizeof(gemini_hand) / 4;
        app_queue_insert(ir_send);
        return;
    }
    app_sleep_lock_set(IR_LOCK,false);
}

static void upt_pwm_time_pulse(uint32_t *data)
{
    uint32_t high = *data++;
    uint32_t low = *data++;
    ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
    ir_send_param.pwm_buf_p[ir_send_param.length + 1] = high; ///打开载波
    ir_send_param.length += 2;

    ir_send_param.pwm_buf_p[ir_send_param.length] = 0; ///关闭载波
    ir_send_param.pwm_buf_p[ir_send_param.length + 1] = low; ///关闭载波
    ir_send_param.length += 2;
}

static void philips_upt_pwm_time_pulse(uint32_t *data)
{
    uint32_t low = *data++;
    uint32_t high = *data++;

    if (high)
    {
        ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
        ir_send_param.pwm_buf_p[ir_send_param.length + 1] = low; ///打开载波
        ir_send_param.length += 2;

        ir_send_param.pwm_buf_p[ir_send_param.length] = 0; ///关闭载波
        ir_send_param.pwm_buf_p[ir_send_param.length + 1] = low; ///关闭载波
        ir_send_param.length += 2;
    }
    else
    {
        ir_send_param.pwm_buf_p[ir_send_param.length] = 0; ///关闭载波
        ir_send_param.pwm_buf_p[ir_send_param.length + 1] = low; ///关闭载波

        ir_send_param.length += 2;
        ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
        ir_send_param.pwm_buf_p[ir_send_param.length + 1] = low; ///打开载波

        ir_send_param.length += 2;
    }
}

static void FB739A_upt_pwm_time_pulse(uint32_t *data)
{
    uint32_t tmp = *data++;
    uint32_t len = *data++;
    uint32_t done = 0;

    if (tmp & 0x01)
    {
        ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
        ir_send_param.pwm_buf_p[ir_send_param.length + 1] = bit1_l_tm; ///打开载波

        ir_send_param.length += 2;
        done = 1;
    }
    else
    {
        ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
        ir_send_param.pwm_buf_p[ir_send_param.length + 1] = bit0_l_tm; ///打开载波

        ir_send_param.length += 2;
        done = 0;
    }

    len -= 1;
    tmp >>= 1; /// 先发低位

    for (uint16_t j = 0; j < 2; j++)
    {
        for (uint16_t i = 0; i < len; i++)
        {
            if (tmp & 0x01) /// '1'
            {
                if (done) {
                    ir_send_param.pwm_buf_p[ir_send_param.length] = 0; ///关闭载波
                    ir_send_param.pwm_buf_p[ir_send_param.length + 1] = bit1_l_tm; ///关闭载波
                }
                else {
                    ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
                    ir_send_param.pwm_buf_p[ir_send_param.length + 1] = bit1_l_tm; ///打开载波
                }
                ir_send_param.length += 2;
                done = 1;
            }
            else /// '0'
            {
                if (done) {
                    ir_send_param.pwm_buf_p[ir_send_param.length] = 1; ///打开载波
                    ir_send_param.pwm_buf_p[ir_send_param.length + 1] = bit0_l_tm; ///打开载波
                }
                else {

                    ir_send_param.pwm_buf_p[ir_send_param.length] = 0; ///关闭载波
                    ir_send_param.pwm_buf_p[ir_send_param.length + 1] = bit0_l_tm; ///关闭载波
                }
                ir_send_param.length += 2;
                done = 0;
            }
            tmp >>= 1; /// 先发低位
        }
        tmp = *data++;
        len = *data++;
    }
}

static void rpt_bit_pwm(uint32_t *repeatcode_time)
{
    uint32_t buf[2]; /// high,low

    buf[0] = *repeatcode_time++;
    buf[1] = *repeatcode_time++;
    upt_pwm_time_pulse(buf);

    buf[0] = *repeatcode_time++;
    buf[1] = *repeatcode_time++;
    upt_pwm_time_pulse(buf);
}

static void IRTI1250C5D7_bit_time(uint16_t data, uint8_t len)
{
    uint32_t buf[2];

    if (len == 0xF) {
        len += 1;
    }

    for (uint8_t i = 0; i < len; i++)
    {
        if (data & 0x01) /// '1'
        {
            if(en_type == IRT1250C5D7_13)
			{
                if (i % 2 != 1) {
                    buf[0] = 10; //290
                    buf[1] = 180;
                    pwm_time_pulse(buf);
                }
                else {
                    buf[0] = bit1_l_tm; //290
                    buf[1] = bit1_h_tm;
                    pwm_time_pulse(buf);
                }
            } 
            else if(en_type == STM708LB1_92)
			{
                if (i % 2 == 1) {
                    buf[0] = 200; //290
                    buf[1] = 4860;
                    pwm_time_pulse(buf);
                }
				else {
                    buf[0] = bit1_l_tm; //290
                    buf[1] = bit1_h_tm;
                    pwm_time_pulse(buf);
                }
            }
        }
        else {			/// '0'
            buf[0] = bit0_l_tm;
            buf[1] = bit0_h_tm;
            pwm_time_pulse(buf);
        }
        data >>= 1; /// 先发低位
    }
}

static void repeat_bit_time(uint16_t data, uint32_t len, uint32_t *bit0, uint32_t *bit1)
{
    if(en_type == ZENITH_S10_77 || en_type == ZENITH_S10_77_Zero)
    {
        for (uint8_t i = 0; i < len; i++)
        {
            if (data & 0x8000) {
                rpt_bit_pwm(bit1);
            }
            else {
                rpt_bit_pwm(bit0);
            }
            data <<= 1; /// 先发最高位
        }
    }
    else if(en_type == ZENITH_D5_78 || en_type == ZENITH_D7_79)
    {
        if (len == 0xF) {
            len += 1;
        }
        for (uint8_t i = 0; i < len; i++)
        {
            if (data & 0x01) {			/// '1'
                rpt_bit_pwm(bit1);
                upt_pwm_time_pulse(&bit1[4]);
            }
            else {						/// '0'
                rpt_bit_pwm(bit0);
                upt_pwm_time_pulse(&bit0[4]);
            }
            data >>= 1; /// 先发低位
        }
    }
}

static void philips_bit_time(uint16_t data, uint8_t len)
{
    uint16_t tmp;
    for (uint8_t i = 0; i < len; i++)
    {
        tmp = (data & 0xC000) >> 14;
        if (tmp == 0) {
            uint32_t bit0[2] = { bit0_l_tm, lead_buf[1] };
            pwm_time_pulse(bit0);
        }
        else if (tmp == 1) {
            uint32_t bit1[2] = { bit0_l_tm, bit0_h_tm };
            pwm_time_pulse(bit1);
        }
        else if (tmp == 2) {
            uint32_t bit2[2] = { bit0_l_tm, bit1_l_tm };
            pwm_time_pulse(bit2);
        }
        else if (tmp == 3) {
            uint32_t bit3[2] = { bit0_l_tm, bit1_h_tm };
            pwm_time_pulse(bit3);
        }
        data <<= 2; /// 先发最高位
    }
}

static void high_bit_time(uint16_t data, uint8_t len)
{
    uint32_t buf[2];

    for (uint8_t i = 0; i < len; i++)
    {
        if (data & 0x8000) /// '1'
        {
            buf[0] = bit1_l_tm;
            buf[1] = bit1_h_tm;
            if (en_type == Unknow_C8D5_83) {
                pwm_time_pulse(buf);
            }
            pwm_time_pulse(buf);
        }
        else /// '0'
        {
            buf[0] = bit0_l_tm;
            buf[1] = bit0_h_tm;
            pwm_time_pulse(buf);
        }
        data <<= 1; /// 先发最高位
    }
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
            pwm_time_pulse(buf);
        }
        else {						/// '0'
            buf[0] = bit0_l_tm;
            buf[1] = bit0_h_tm;
            pwm_time_pulse(buf);
        }
        data >>= 1; /// 先发低位
    }
}

static uint8_t Zenith_D5_ready(uint16_t irnum)
{

    memset((void *)&ir_send_param, 0x00, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    if (user_repeat != 0) {
        ir_send_param.user_repeat = user_repeat;
    }

    if(en_type == ZENITH_D5_78) {
        uint32_t buf[4] = { bit0_l_tm, bit0_l_tm, bit0_l_tm, bit1_l_tm };
        rpt_bit_pwm(buf);
    }
    else if(en_type == ZENITH_D7_79) {
        uint32_t buf[2] = {bit0_l_tm, bit1_h_tm};
        upt_pwm_time_pulse(buf);
    }
    uint32_t bit0[6] = {bit0_l_tm, bit1_h_tm, bit0_l_tm, bit0_l_tm, bit0_l_tm, bit1_l_tm};
    uint32_t bit1[6] = {bit0_l_tm, bit0_l_tm, bit0_l_tm, bit1_l_tm, bit0_l_tm, bit1_h_tm};
    if (((brand_type[IR_NUM] >> 4) & 0xf) != 0) {
        repeat_bit_time(irnum, (brand_type[IR_NUM] >> 4) & 0xf, bit0, bit1);
    }

    upt_pwm_time_pulse(stop_buf);

    return 0;
}

static uint8_t IRTI1250C5D7_ready(uint16_t irnum)
{
    uint16_t src[4];
    memset(src, 0, sizeof(src));
    memset((void *)&ir_send_param, 0, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;
	
    if(en_type == IRT1250C5D7_13) {
        src[0] = cust_code[0];
        src[1] = 0;
        src[2] = irnum;
        src[3] = 0;
    }
	else if (en_type == STM708LB1_92) {
        src[0] = cust_code[0];
        src[1] = 0;
        src[2] = irnum>>8;
        src[3] = irnum & 0xff;
    }

    if (lead_buf[0] != 0) {
        upt_pwm_time_pulse(lead_buf);
    }

    if ((brand_type[IR_NUM] >> 12) != 0) {
        bit_time(src[0], (brand_type[1] >> 12));
    }
    if (((brand_type[IR_NUM] >> 4) & 0xf) != 0) {
        IRTI1250C5D7_bit_time(src[2], ((brand_type[IR_NUM] >> 4) & 0xf));
    }
    if ((brand_type[IR_NUM] & 0xf) != 0) {
        bit_time(src[3], (brand_type[IR_NUM] & 0xf));
    }

    upt_pwm_time_pulse(lead_buf);
    upt_pwm_time_pulse(stop_buf);
    return 0;
}

static uint8_t ir_fb739a_ready(uint16_t irnum)
{
    memset((void *)&ir_send_param, 0x00, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    if (user_repeat != 0) {
        ir_send_param.user_repeat = user_repeat;
    }

    uint32_t buf[4] = { irnum, (brand_type[IR_NUM] >> 12), cust_code[0], (((brand_type[IR_NUM] >> 8) & 0x0f)) };
    pwm_time_pulse(buf);
    upt_pwm_time_pulse(stop_buf);

    return 0;
}

static uint8_t repeat_bit_data_ready(uint16_t irnum)
{
    memset((void *)&ir_send_param, 0x00, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    if (user_repeat != 0) {
        ir_send_param.user_repeat = user_repeat;
    }

    irnum <<= 8;
    uint32_t buf[6] = { bit0_l_tm, bit0_h_tm, bit0_l_tm, 2 * bit0_h_tm, bit0_l_tm, 5 * bit0_h_tm };
    rpt_bit_pwm(buf);
    upt_pwm_time_pulse(&buf[4]);

    uint32_t bit0[4] = { bit0_l_tm, 3 * bit0_h_tm, bit0_l_tm, 4 * bit0_h_tm };
    uint32_t bit1[4] = { bit0_l_tm, bit0_h_tm, bit0_l_tm, 2 * bit0_h_tm };

    if ((brand_type[IR_NUM] >> 12) != 0) {
        repeat_bit_time(irnum, (brand_type[1] >> 12), bit0, bit1);
    }
	
    if (en_type == ZENITH_S10_77)
    {
        if (coust_code > 3) {
            coust_code = 0;
        }
        coust_code <<= 14;
        repeat_bit_time(coust_code, 2, bit0, bit1);
        coust_code >>= 14;
        coust_code++;
    }
	else if (en_type == ZENITH_S10_77_Zero) {
        coust_code ^= 0x4000;
        repeat_bit_time(coust_code, 2, bit0, bit1);
    }

    uint32_t stop_buf[2] = { bit0_l_tm, 60000 };
    upt_pwm_time_pulse(stop_buf);

    return 0;
}

static uint8_t philips_rcmm_bit_data_ready(uint16_t irnum)
{
    static uint16_t toggle = 0x2600;

    memset((void *)&ir_send_param, 0x00, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;
    upt_pwm_time_pulse(lead_buf);

    philips_bit_time(cust_code[0], (brand_type[IR_NUM] >> 12));

    toggle ^= 0x8000;
    philips_bit_time(toggle, ((brand_type[IR_NUM] >> 8) & 0x0f));

    irnum <<= 8;
    philips_bit_time(irnum, ((brand_type[IR_NUM] >> 4) & 0xf));

    upt_pwm_time_pulse(stop_buf);

    return 0;
}

static uint8_t ir_gemini_ready(uint16_t irnum)
{
    memset((void *)&ir_send_param, 0x00, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    if (user_repeat != 0) {
        ir_send_param.user_repeat = user_repeat;
    }

    if (lead_buf[0] != 0) {
        upt_pwm_time_pulse(lead_buf);
    }

    if (en_type == SINGH2150_51) {
        bit_time(0x1, 1);
    }

    if ((brand_type[IR_NUM] >> 12) != 0) {
        bit_time(cust_code[0], (brand_type[1] >> 12));
    }
    upt_pwm_time_pulse(stop_buf);
    if(en_type == GEMINI_C10_32_08 || en_type == GEMINI_C10_38_09 || en_type == GEMINI_C10_32_10 || GEMINI_C16_38_08) {
        gemini_state = 0; 
        memcpy(&gemini_hand[0], &ir_send_param.pwm_buf_p[0], 4 * ir_send_param.length);
    }

    memset((void *)&ir_endcode_param, 0x00, sizeof(ir_endcode_param));
    ir_endcode_param.endcode_en = 1;
    ir_endcode_param.endcode_start_offset = ir_send_param.length;

    upt_pwm_time_pulse(lead_buf);

    if (((brand_type[IR_NUM] >> 8) & 0x0f) != 0) {
        bit_time(0x1, ((brand_type[IR_NUM] >> 8) & 0x0f));
    }
    if (((brand_type[IR_NUM] >> 4) & 0x00f) != 0) {
        bit_time(irnum, ((brand_type[IR_NUM] >> 4) & 0x00f));
    }
    if ((brand_type[IR_NUM] & 0xf) != 0) {
        bit_time(cust_code[1], (brand_type[IR_NUM] & 0xf));
    }

    rpt_bit_pwm(rpt_lead_buf);

    ir_endcode_param.endcode_end_offset = ir_send_param.length;

    return 0;
}

static uint8_t high_ir_data_ready(uint16_t irnum)
{
    uint16_t src[4];
    memset(src, 0, sizeof(src));

    memset((void *)&ir_send_param, 0x00, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    if (user_repeat != 0) {
        ir_send_param.user_repeat = user_repeat;
    }

    irnum <<= 8;
    switch (brand_type[COUST_TYPE])
    {
        case HIGH_COSTOM_CODE:
            src[0] = cust_code[0];
            src[1] = cust_code[1];
            src[2] = irnum;
            break;
        case HIGH_START_PHIL_COSTOM2_CODE2:
		{
            uint32_t start_buf[2] = {888, 0};
            upt_pwm_time_pulse(start_buf);
        }
        case HIGH_PHILIPS_TOGGLE_CODE:
        case HIGH_TOGGLE_COSTOM2_CODE2:
		{
            static uint16_t toggle = 0;
            toggle = toggle ^ 0x8000;

            src[0] = cust_code[0];
            src[1] = toggle;
            src[2] = cust_code[1];
            src[3] = irnum;
        } 
            break;
        case HIGH_REF_CUSTOM_CODE:
		{
            static uint16_t coust_code = 0;
            if (coust_code > 3) {
                coust_code = 0;
            }
            coust_code <<= 14;
            src[0] = coust_code;
            src[1] = 0;
            src[2] = cust_code[0];
            src[3] = irnum;
            coust_code >>= 14;
            coust_code++;
        }
            break;
        case HIGH_COSTOM2_CODE2:
            src[0] = cust_code[0];
            src[1] = irnum;
            src[2] = ~cust_code[0];
            src[3] = ~irnum;
            break;

        default:
            return 1;
    }

    if (lead_buf[0] != 0) {
        upt_pwm_time_pulse(lead_buf);
    }
    if(en_type == ZENITHSIM_C2D8_82)
    {
        upt_pwm_time_pulse(lead_buf);
    }
    if ((brand_type[IR_NUM] >> 12) != 0) {
        high_bit_time(src[0], (brand_type[1] >> 12));
    }

    if (brand_type[COUST_TYPE] == HIGH_PHILIPS_TOGGLE_CODE) {
        bit1_l_tm *= 2;
        bit0_l_tm *= 2;
    }

    if (((brand_type[IR_NUM] >> 8) & 0xf) != 0) {
        high_bit_time(src[1], ((brand_type[IR_NUM] >> 8) & 0xf));
    }

    if (brand_type[COUST_TYPE] == HIGH_PHILIPS_TOGGLE_CODE) {
        bit1_l_tm /= 2;
        bit0_l_tm /= 2;
    }

    if (((brand_type[IR_NUM] >> 4) & 0xf) != 0) {
        high_bit_time(src[2], ((brand_type[IR_NUM] >> 4) & 0xf));
    }

    if (brand_type[COUST_TYPE] == HIGH_COSTOM_CODE) {
        uint32_t end_code[2] = { 500, 4000 };
        upt_pwm_time_pulse(end_code);
    }

    if ((brand_type[IR_NUM] & 0xf) != 0) {
        high_bit_time(src[3], (brand_type[IR_NUM] & 0xf));
    }
    
    upt_pwm_time_pulse(stop_buf);

    return 0;
}

static uint8_t ir_data_ready(uint16_t irnum)
{
    uint16_t src[4];
    memset(src, 0, sizeof(src));
    memset((void *)&ir_send_param, 0, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    if (user_repeat != 0) {
        ir_send_param.user_repeat = user_repeat;
    }

    switch (brand_type[COUST_TYPE])
    {
        case CODE1:
            src[0] = irnum;
            break;
        case CODE1_COSTOM1:
            src[0] = irnum;
            src[1] = cust_code[0];
            break;
        case COSTOM2_CODE1_0:
            src[0] = cust_code[0];
            src[1] = cust_code[1];
            src[2] = irnum;
            break;
        case CODE2_COSTOM2:
            src[0] = irnum;
            src[1] = ~irnum;
            src[2] = cust_code[0];
            src[3] = cust_code[1];
            break;
        case COSTOM2_CODE2:
            src[0] = cust_code[0];
            src[1] = cust_code[1];
            src[2] = irnum;
            src[3] = ~irnum;
            break;
        case COSTOM1_CODE1_COSTOM1_CODE1:
            src[0] = cust_code[0];
            src[1] = irnum;
            src[2] = cust_code[1];
            src[3] = ~irnum;
            break;
        case COSTOM2_CODE2_Panasonic:
		{
            uint8_t costom_a = 0x80;
            uint8_t costom_b = (cust_code[1] >> 8);
            src[0] = cust_code[0];
            src[1] = cust_code[1];
            src[2] = irnum;
            src[3] = costom_a ^ costom_b ^ irnum;
        } 
            break;
        case COSTOM1_0_CODE1:
            src[0] = cust_code[0];
            src[1] = 0;
            src[2] = irnum;
            src[3] = 0;
            break;
        case CODE1_COSTOM1_CODE1:
            src[0] = irnum & 0XFF;
            src[1] = cust_code[0];
            src[2] = irnum >> 8;
            src[3] = 0;
            break;
        default:
            return 1;
    }
    
    if (lead_buf[0] != 0) {
        upt_pwm_time_pulse(lead_buf);
    }
    if (en_type == VICTOR_C8D8_72) {
            memset((void *)&ir_endcode_param, 0x00, sizeof(ir_endcode_param));
            ir_endcode_param.endcode_en = 1;
            ir_endcode_param.endcode_start_offset = ir_send_param.length;
    }

    if ((brand_type[IR_NUM] >> 12) != 0) {
        bit_time(src[0], (brand_type[1] >> 12));
    }
    if (((brand_type[IR_NUM] >> 8) & 0xf) != 0) {
        bit_time(src[1], ((brand_type[IR_NUM] >> 8) & 0xf));
    }

    if (en_type == LX0773CE_50) {
        uint8_t Extention = 1;
        bit_time(Extention, 2);
        upt_pwm_time_pulse(stop_buf);
    }
    else if (en_type == M50560_25) {
        uint32_t end_code[2] = { 530, 4250 };
        upt_pwm_time_pulse(end_code);
    }
    else if (en_type == PANPROJECTOR_37) {
        uint16_t buf = 0x00;
        bit_time(buf, 8);
    }
    else if(en_type == DVB_40BIT_100) {
        upt_pwm_time_pulse(lead_buf);
    }

    if (((brand_type[IR_NUM] >> 4) & 0xf) != 0) {
        bit_time(src[2], ((brand_type[IR_NUM] >> 4) & 0xf));
    }
    if ((brand_type[IR_NUM] & 0xf) != 0) {
        bit_time(src[3], (brand_type[IR_NUM] & 0xf));
    }

    if (en_type == LX0773CE_50) {
        uint8_t Extention = 2;
        bit_time(Extention, 2);
    }
    else if (en_type == IRT1250C5D6_12) {
        upt_pwm_time_pulse(lead_buf);
    }

    upt_pwm_time_pulse(stop_buf);

    if (en_type == VICTOR_C8D8_72) {
        ir_endcode_param.endcode_end_offset = ir_send_param.length;
    }

    if (brand_type[REPEAT_DISABLE]) {
        memset((void *)&ir_endcode_param, 0x00, sizeof(ir_endcode_param));
        ir_endcode_param.endcode_en = 1;
        ir_endcode_param.endcode_start_offset = ir_send_param.length;
        if (en_type == TC901_54) {
            upt_pwm_time_pulse(lead_buf);
        }
        rpt_bit_pwm(rpt_lead_buf);
        ir_endcode_param.endcode_end_offset = ir_send_param.length;
		
        ir_send_param.length = ir_endcode_param.endcode_start_offset;
    }

    return 0;
}

uint8_t *get_ir_pulse_fill(void)
{
    return (uint8_t*)ir_pulse_data;
}

void set_ir_pulse_fill()
{
    memset(ir_pulse_data, 0, sizeof(ir_pulse_data));
}

#ifdef IR_RCV_PIN
void ir_remote_learn_send(ir_receive_param_t irparams)
{
    app_sleep_lock_set(IR_LOCK,true);
    memset((void *)&ir_send_param, 0, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;
    ir_set_freq(irparams.freq*1000);
    set_pwm_freq_div();
    for(uint16_t i = 0  ; i < irparams.len ; i++)
	{
        if(i % 2 == 0){
            ir_send_param.pwm_buf_p[ir_send_param.length] = 1;
        }
		else{
            ir_send_param.pwm_buf_p[ir_send_param.length] = 0;
        }
        ir_send_param.length++;
        ir_send_param.pwm_buf_p[ir_send_param.length] = irparams.rawbuf[i];
        ir_send_param.length++;
    }

    if(irparams.repeat == 1)
    {
        memset((void *)&ir_endcode_param, 0x00, sizeof(ir_endcode_param));
        ir_endcode_param.endcode_en = 1;
        ir_endcode_param.endcode_start_offset = ir_send_param.length;

        for(uint16_t i = 0; i < irparams.repeat_stop ; i++)
        {
            if(i % 2 == 0){
                ir_send_param.pwm_buf_p[ir_send_param.length] = 1;
            }
            else{
                ir_send_param.pwm_buf_p[ir_send_param.length] = 0;
            }
            ir_send_param.length++;
            ir_send_param.pwm_buf_p[ir_send_param.length] = irparams.rawbuf[irparams.len + i];
            ir_send_param.length++;
        }
        ir_endcode_param.endcode_end_offset = ir_send_param.length;
        ir_send_param.length = ir_endcode_param.endcode_start_offset;
    }
    app_queue_insert(ir_send);
}
#endif 

void ir_time_send(const uint16_t *time_data)
{
    uint16_t j = 0;
    memset((void *)&ir_send_param, 0, sizeof(ir_send_param));
    memset((void *)ir_pulse_data, 0, sizeof(ir_pulse_data));
    ir_send_param.pwm_buf_p = ir_pulse_data;

    ir_set_freq(time_data[0]);
    set_pwm_freq_div();
    ir_send_param.length = 2 * time_data[1];
    for(uint16_t i = 2; i < ir_send_param.length+2; i++)
	{
        if(i % 2 == 0){
            ir_send_param.pwm_buf_p[j] = 1;
        }
		else{
            ir_send_param.pwm_buf_p[j] = 0;
        }
        j++;
        ir_send_param.pwm_buf_p[j] = time_data[i] * 10;
        j++;
    }
    app_queue_insert(ir_send);
}

void ir_comm_send(uint16_t irnum)
{
    DEBUG_LOG_STRING("ir_comm_send\r\n");
    app_sleep_lock_set(IR_LOCK,true);
    if (brand_type[FREQ_SIZE] != 0) {
        set_pwm_freq_div();
        if (!ir_ready(irnum)) {
            app_queue_insert(ir_send);
        }
    }
    else {
        if (!ir_ready(irnum)) {
            app_queue_insert(ir_without_freq);
        }
    }
}

void ir_single_send(uint16_t irnum,uint8_t repeat)
{
    app_sleep_lock_set(IR_LOCK,true);
    if (brand_type[FREQ_SIZE] != 0) {
        set_pwm_freq_div();
        if (!ir_ready(irnum)) {
            ir_send_param.user_repeat = repeat;
            ir_send();
        }
    }
}

void ir_type_init(ir_type_t type, customer_code_t customer)
{
    en_type = type;
    brand_type = get_ir_brand(type);
    cust_code = get_ir_customer(customer);
    lead_buf[0] = brand_type[LEAD_L_TIME];
    lead_buf[1] = brand_type[LEAD_H_TIME];
    stop_buf[0] = (uint32_t)brand_type[STOP_L_TIME];
    stop_buf[1] = (uint32_t)(brand_type[STOP_H_TIME_1] + brand_type[STOP_H_TIME_2]);

    if (brand_type[REPEAT_DISABLE]) {
        brand_rpt_code = get_ir_rpt((rpt_code_t)brand_type[REPEAT_BUF]);
        rpt_lead_buf[0] = (uint32_t)brand_rpt_code[0];
        rpt_lead_buf[1] = (uint32_t)brand_rpt_code[1];
        rpt_lead_buf[2] = (uint32_t)brand_rpt_code[2];
        rpt_lead_buf[3] = (uint32_t)brand_rpt_code[3] + brand_rpt_code[4];
    }

    bit0_l_tm = brand_type[BIT0_L_TIME];
    bit0_h_tm = brand_type[BIT0_H_TIME];
    bit1_l_tm = brand_type[BIT1_L_TIME];
    bit1_h_tm = brand_type[BIT1_H_TIME];

    ir_set_freq(brand_type[FREQ_SIZE]);
    user_repeat = brand_type[USER_REPEAT];
    switch (brand_type[COUST_TYPE])
    {
        case CODE1:
        case CODE1_COSTOM1:
        case COSTOM2_CODE2:
        case COSTOM2_CODE1_0:
        case CODE2_COSTOM2:
        case COSTOM2_CODE2_Panasonic:
        case COSTOM1_CODE1_COSTOM1_CODE1:
        case CODE1_COSTOM1_CODE1:
        case COSTOM1_0_CODE1:
            ir_ready = ir_data_ready;
            pwm_time_pulse = upt_pwm_time_pulse;
            break;
        case PHILIPS_STOP_CODE:
            ir_ready = ir_gemini_ready;
            pwm_time_pulse = philips_upt_pwm_time_pulse;
            break;
        case FB739A:
            ir_ready = ir_fb739a_ready;
            pwm_time_pulse = FB739A_upt_pwm_time_pulse;
            break;
        case HIGH_COSTOM_CODE:
        case HIGH_COSTOM2_CODE2:
        case HIGH_TOGGLE_COSTOM2_CODE2:
        case HIGH_REF_CUSTOM_CODE:
            ir_ready = high_ir_data_ready;
            pwm_time_pulse = upt_pwm_time_pulse;
            break;
        case HIGH_PHILIPS_TOGGLE_CODE:
        case HIGH_START_PHIL_COSTOM2_CODE2:
            ir_ready = high_ir_data_ready;
            pwm_time_pulse = philips_upt_pwm_time_pulse;
            break;
        case REPEAT_BIT_CODE:
            ir_ready = repeat_bit_data_ready;
            pwm_time_pulse = upt_pwm_time_pulse;
            break;
        case PHILIPS_RC_MM:
            ir_ready = philips_rcmm_bit_data_ready;
            pwm_time_pulse = upt_pwm_time_pulse;
            break;
        case NOPWM_IRT1250C5D7:
            ir_ready = IRTI1250C5D7_ready;
            pwm_time_pulse = upt_pwm_time_pulse;
            break;
        case Zenith_D5:
            ir_ready = Zenith_D5_ready;
            pwm_time_pulse = upt_pwm_time_pulse;
            break;
		
        default:
            break;
    }
}

void ir_init(ir_type_t type, customer_code_t customer)
{
    GPIO_Init(IR_IO, GPIO_Mode_Out_Low);

#ifdef IR_RCV_PIN
    GPIO_Init(IR_RCV_PIN, GPIO_Mode_Out_High);
#endif

    if (type != DEFAULT_TYPE) {
        ir_type_init(type, customer);
    }
}
