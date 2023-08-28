#include"yc11xx_sadc.h"
#include"reg_addr.h"
void SADC_CheckChannel(SADC_ChxTypeDef chanllex)
{
	uint8_t adc_ch;
	adc_ch = ((chanllex << 3) | (SADC_Range4 << 1) | BIT0);
	HWRITE(CORE_GPADC_CTRL,adc_ch);
}

void SADC_Enable(void)
{
	uint8_t adc_en;
	adc_en = HREAD(CORE_SBC_SCTRL)| BIT7;
	HWRITE(CORE_SBC_SCTRL,adc_en);//ENABLE ADC SUM
	HWRITE(CORE_SUM_CTRL,ADC_SumCtrl);
}

void SADC_Disable(void)
{
	uint8_t adc_en;
	adc_en = HREAD(CORE_SBC_SCTRL);
	adc_en &= 0x7f;
	HWRITE(CORE_SBC_SCTRL,adc_en);
}

uint16_t SADC_ReadSAdcValue(void)
{
	uint16_t adc_value;
	adc_value = HREADW(CORE_ADC_SUM_RESULT);
	return adc_value;
}

void SADC_Confight(SADC_ModeTypeDef mode)
{
	HWRITE(CORE_RF_IDO_EN0,ADC_LdoEnable);
	HWRITE(CORE_RX_EN0,ADC_RfpllSet);//RX IQ ADC enable, high active
	switch(mode)
	{
		case SADC_GPIO:
			SADC_CheckChannel(SADC_CHANNEL_7);
			break;
		case SADC_HVIN:
			SADC_CheckChannel(SADC_CHANNEL_VBAT);
			break;
		case SADC_VINLPM:
			SADC_CheckChannel(SADC_CHANNEL_VDCDC);
			break;
		default:
			break;
	}
}

void SADC_ConfightGpio(SADC_ChxTypeDef channel)
{
	HWRITE(CORE_RF_IDO_EN0,ADC_LdoEnable);
	HWRITE(CORE_RX_EN0,ADC_RfpllSet);
	SADC_CheckChannel(channel);
}
