#include "vbat.h"
#include "software_timer.h"
#include "btreg.h"
#include "yc11xx_bt_interface.h"
#include "yc_debug.h"

#include "remote_control.h"

#define HVIN_2_2V_BAT_REF (2200)
#define HVIN_3_3V_BAT_REF (3300)

#ifndef ADC_SAMPLE_ARRAY_NUM
#define ADC_SAMPLE_ARRAY_NUM (5)
#endif
#ifndef ADC_SAMPLE_TIME
#define ADC_SAMPLE_TIME (1000)
#endif

static uint8_t adc_sample_cnt;
static uint16_t adc_value[ADC_SAMPLE_ARRAY_NUM]; 
static uint8_t vbat_adc_timernum = 0XFF;
static power_report_cb_t power_report_cb;

static uint16_t batvalue_calculate(void)
{
    uint32_t read_batReg = HREADW(mem_adc_current_value);

    if(read_batReg == 0) {
    	return 0;
    }

    uint32_t SADC_HVINK = ((HREADL(mem_3v_sadc_hvin_data)) & 0xffffe000) >> 3;
    uint32_t SADC_HVINB = ((HREADL(mem_3v_sadc_hvin_data)) & 0x1fff) << 10;
    uint64_t read_batValue;

    read_batValue = (SADC_HVINK >> 10);
    read_batValue *= read_batReg;
    read_batValue = (read_batValue >> 10);
    read_batValue -= SADC_HVINB;
    read_batValue *= 1000;
    read_batValue = (uint16_t)(read_batValue >> 20);

    return read_batValue;
}

static void vbat_task(void)
{
    if (adc_sample_cnt >= ADC_SAMPLE_ARRAY_NUM-1)
    {
        uint8_t batlevel;
        uint16_t batvalue_average = MedianFilter(adc_value, ADC_SAMPLE_ARRAY_NUM);
        
        if (batvalue_average <= HVIN_2_2V_BAT_REF) {
            batlevel = 0;
        }
        else if (batvalue_average >= HVIN_3_3V_BAT_REF) {
            batlevel = 100;
        }
        else {
            batlevel = (uint8_t)(1.0f*(batvalue_average - HVIN_2_2V_BAT_REF)/(HVIN_3_3V_BAT_REF - HVIN_2_2V_BAT_REF)*100);
        }

        power_report_cb(batlevel);
        adc_sample_cnt = 0;
    }
    else
    {
        uint16_t batvalue = batvalue_calculate();

        if (!batvalue) {
            return;
        }
        else {
            adc_value[adc_sample_cnt] = batvalue;
        }

        adc_sample_cnt++;
    }
}

void vbat_reinit(void)
{
    HWRITEW(mem_hvin_bat_enable, 1);
    swtimer_restart(vbat_adc_timernum);
}

void vbat_deinit(void)
{
    HWRITEW(mem_hvin_bat_enable, 0);
    swtimer_stop(vbat_adc_timernum);
}

uint8_t vbat_init(power_report_cb_t cb)
{
    HWRITEW(mem_hvin_bat_enable, 1);

    power_report_cb = cb;
    vbat_adc_timernum = swtimer_add(vbat_task);
    if (vbat_adc_timernum < TIMER_NUM) {
        swtimer_start(vbat_adc_timernum, ADC_SAMPLE_TIME, TIMER_START_REPEAT);
    }
    else {
        return vbat_adc_timernum;
    }

    return 0;
}
