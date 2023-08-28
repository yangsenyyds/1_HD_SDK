#include"yc11xx.h"

#define mem_vin_05v 	500
#define mem_vin_1v 		1000
#define mem_vin_3v 		3000
#define mem_vin_5v 		5000
#define ADC_SumCtrl     0x30
#define ADC_LdoEnable   0x0c
#define ADC_RfpllSet  	0x06

typedef enum
{
	SADC_CHANNEL_0 = 0,  	//gpio_5
	SADC_CHANNEL_1,			//gpio_6
	SADC_CHANNEL_2,			//gpio_7
	SADC_CHANNEL_3,			//gpio_8
	SADC_CHANNEL_4,			//gpio_23
	SADC_CHANNEL_5,			//gpio_24
	SADC_CHANNEL_6,			//gpio_33
	SADC_CHANNEL_7,			//gpio_34
	SADC_CHANNEL_VBAT = 0x09,
	SADC_CHANNEL_VDCDC = 0x0A,
}SADC_ChxTypeDef;

typedef enum
{
	SADC_GPIO = 0,
	SADC_HVIN,
	SADC_VINLPM,
	SADC_DIFF,
}SADC_ModeTypeDef;

typedef enum
{
	SADC_Range1 = 0,//0 ~ 0.9V range
	SADC_Range2 ,	//0 ~ 1.8V range
	SADC_Range3 ,	//0 ~ 2.7V range
	SADC_Range4,	//0 ~ 3.6V range
}SADC_RangeTypeDef;

void SADC_CheckChannel(SADC_ChxTypeDef chanllex);
void SADC_CheckVCDC(void);
void SADC_CheckVBAT(void);
void SADC_Enable(void);
void SADC_Disable(void);
void SADC_Confight(SADC_ModeTypeDef mode);
void SADC_ConfightGpio(SADC_ChxTypeDef channel);


