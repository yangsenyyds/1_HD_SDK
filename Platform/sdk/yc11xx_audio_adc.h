#ifndef YC11XX_AUDIO_ADC_H
#define YC11XX_AUDIO_ADC_H

#include "yc11xx.h"

/*Audio Adc*/
//#define AUDIO_ADC_VOL_MIN 0x4cc
#define AUDIO_ADC_VOL_MAX 0x1000
#define AUDIO_ADC_VOL_GEAR 11      //0 to 10, 11 gears in total

/*ADC sampling accuracy*/
typedef enum
{
    Down_sample32,		//\128   /*32bits*/
    Down_sample16,		//\64	/*16bits*/
} ADC_CicDsSelDEV;
/*MIC clock setting*/
typedef enum
{
    MicClkFromPll,			/*from pll*/
    MicClkFromPllDiv2,		/*From PLL half frequency*/
    MicClkFromPllDiv4,		/*From PLL quarter frequency*/
    MicClkFromGpio,			/*from gpio*/
} ADC_MicClkSelDEV;

/*audio clk source select*/
typedef enum
{
    ADCClk6p144M,			/*6.144M*/
    ADCClk12p288M,			/*12.288M*/
    ADCClk11p2896M,			/*11.2896M*/
    ADCClk4p096M,			/*4.096M*/
    ADCClkFromGpio,			/*from gpio*/
} ADC_AUDIOClkSelDEV;

typedef enum
{
    ADC_ClkDivide1,			/*from pll*/
    ADC_ClkDivide2,			/*From PLL half frequency*/
    ADC_ClkDivide3,			/*From PLL quarter frequency*/
} ADC_ClkSelDev;

/*adc samples*/
typedef enum
{
    ADC_Sample8K = 8,		//8k
    ADC_Sample11p025K = 11,	//11.025k
    ADC_Sample12K = 12,		//12k
    ADC_Sample16K = 16,		//16k
    ADC_Sample22p05K = 22,	//22.05k
    ADC_Sample24K = 24,		//24k
    ADC_Sample32K = 32,		//32k
    ADC_Sample44p1K = 44,	//44.1k
    ADC_Sample48K = 48,		//48k
} AUDIO_ADCSample;

/*mic typedev */
typedef enum
{
    MicIsANA,			//simulate the MIC
    MicIsPDMRisingClk,	//Digital MIC
    MicIsPDMFallingClk  //Digital MIC
} AUDIO_MicTypeDev;
typedef enum
{
    adc_HPFDisable = 0,
    adc_alpha1 = 1997,
    adc_alpha2 = 1792,
    adc_alpha3 = 1587,
    adc_alpha4 = 1382,
    //Add parameters yourself if you need to
} AUDIO_ADCHighPassFilter;
typedef enum
{
    adc_RssiTime1024,
    adc_RssiTime2048,
    adc_RssiTime4096,
    adc_RssiTime8192,
    adc_RssiDisable
} AUDIO_ADCRssiConfig;
typedef enum
{
    ADC_SingleEndOutputWithCapacitor,
    ADC_SingleEndOutputNoCapacitor,
    ADC_DifferentialOutput,
    ADC_VcomOutput
} ADC_OutputMode;
typedef struct
{
    uint8_t 					*Adc_StartAddr;		/*adc DMA start address*/
    uint16_t 					Adc_BufferSize;		/*the buffer length of the ADC */
    AUDIO_ADCSample 			Adc_Sample;			/*ADC operating sampling rate*/
    AUDIO_MicTypeDev 			Mic_Type;			/*audio mic typedev*/
    FunctionalState    			Adc_VolFadeEnable;	/*audio vol fade enable*/
    uint8_t 					Adc_VolStep;		/*ADC vol step of Channel Prompt*/
    uint8_t 					Adc_Vol;			/*voice data*/
    FunctionalState 			Adc_MuteChannel;	/*mute ADC channel*/
    AUDIO_ADCRssiConfig 		Adc_RssiConfig;		/*auto rssi*/
    AUDIO_ADCHighPassFilter     Adc_HPFConfig;      /*HPF*/
    ADC_OutputMode              Adc_OutputMode;     /*SE DIFF VOM*/
} AUDIO_ADCInitTypeDef;

void AUDIO_AdcAnaSEWithCapacitor();
void AUDIO_AdcAnaSENoCapacitor();
void AUDIO_AdcAnaStop(void);
void AUDIO_AdcAnaDiffConfig();
void AUDIO_AdcAnaVcomConfig();
bool AUDIO_AdcVolConfig(uint8_t vol);
bool AUDIO_AdcCheckUseMram(void);
void AUDIO_AdcInit(AUDIO_ADCInitTypeDef adcparam);
void AUDIO_AdcFilterInit(void);
void AUDIO_AdcDmaSet(bool state);
uint16_t AUDIO_AdcWptr(void);
void AUDIO_AdcHPFClr();
void AUDIO_AdcHPFDisable();
void AUDIO_AdcHPFEnable();
void AUDIO_AdcHPFInit(AUDIO_ADCHighPassFilter alpha1);
void AUDIO_AdcStop(void);

#endif
