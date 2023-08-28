#ifndef __YC11XX_DAC_H__
#define __YC11XX_DAC_H__

#include "yc11xx.h"

/*Audio Dac*/
#define AUDIO_DAC_VOL_MAX 0x1000
#define AUDIO_DAC_VOL_GEAR 11      //0 to 10, 11 gears in total

typedef enum
{
    DAC_AnaStereo,
    DAC_AnaLeft,
    DAC_AnaRight
} DAC_AnaConfig;

typedef enum
{
    DAC_Mclk,//128
    DAC_Mclk2,//\2   256
    DAC_Mclk4,
    DAC_MclkRsvd,
} DAC_CoreMclkSelDev;

typedef enum
{
    DAC_Clk6p144M,
    DAC_Clk12p288M,
    DAC_Clk11p2896M,
    DAC_Clk4p096M,
    DAC_ClkFromGpio,
} DAC_AudioClkSelDEV;

typedef enum
{
    DAC_ClkDivide1,
    DAC_ClkDivide2,
    DAC_ClkDivide3,
} DAC_ClkSelDev;

////////////////DAC INIT///////////////////
typedef enum
{
    stereo = 0,
    mono,
} DAC_SoundTrackDev;

typedef enum
{
    DAC_LChannelSelLeft,
    DAC_LChannelSelRight,
    DAC_LChannelSelLeftRightMix,
    DAC_LChannelSelLeftRightNegation,
    DAC_LChannelClose = 0xff,
} DAC_LeftTrackConfigDev;

typedef enum
{
    DAC_RChannelSelLeft,
    DAC_RChannelSelRight,
    DAC_RChannelSelLeftRightMix,
    DAC_RChannelSelLeftRightNegation,
    DAC_RChannelClose = 0xff,
} DAC_RightTrackConfigDev;

typedef enum
{
    DAC_Clk8k = 8,
    DAC_Clk11p025 = 11,
    DAC_Clk12 = 12,
    DAC_Clk16k = 16,
    DAC_Clk22p05 = 22,
    DAC_Clk24k = 24,
    DAC_Clk32k = 32,
    DAC_Clk44p1k = 44,
    DAC_Clk48k = 48,
} DAC_ClkDev;

typedef enum
{
    DAC_WorkContinuous,
    DAC_WorkWptr,
} DAC_WorkModeDEV;

typedef enum
{
    DAC_Date16Bit,
    DAC_Date24Bit,
} DAC_DateModeDev;

typedef enum
{
    DAC_SingleEndOutputNoCapacitance,
    DAC_SingleEndOutputHaveCapacitance,
    DAC_DifferentialOutputNoCapacitance,
    DAC_DifferentialOutputHaveCapacitance,
    DAC_VcomOutputNoCapacitance,
    DAC_VcomOutputHaveCapacitance
} DAC_OutputMode;

typedef struct
{
    DAC_SoundTrackDev SoundTrack;                   /*!< Choose stereo or mono. */
    DAC_LeftTrackConfigDev LeftChannelConfig;       /*!< Left channel configuration. */
    DAC_RightTrackConfigDev RightChannelConfig;     /*!< Right channel configuration. */
    DAC_ClkDev DacClk;                              /*!< Sampling rate selection. */
    uint8_t *DacStartAddr;                          /*!< DAC playback start address. */
    uint16_t DacBufferSize;                         /*!< DAC playback buff size. */
    DAC_WorkModeDEV DacWorkMode;                    /*!< Play mode select loop mode or write pointer mode. */
    uint8_t DacZeroNumSel;                          /*!< 0~7:  2^(9+dac_zero_num_sel) */
    DAC_DateModeDev DateMode;                       /*!< 0: input data width is 16bits, 1: input data width is 24bits. */
    DAC_OutputMode OutMode;                         /*!< Configure single-ended or differential circuits. */
} DAC_InitTypeDef;

typedef enum
{
    HINTTONE_HintToneVol0,
    HINTTONE_HintToneVol1,
    HINTTONE_HintToneVol2,
    HINTTONE_HintToneVol3,
} HINTTONE_Vol;

typedef enum
{
    HINTTONE_MainVol0,
    HINTTONE_MainVol1,
    HINTTONE_MainVol2,
    HINTTONE_MainVol3,
} HINTTONE_MainVol;

typedef enum
{
    HINTTONE_WorkWptr,
    HINTTONE_WorkContinuous
} HINTTONE_WorkModeDev;

typedef enum
{
    HINTTONE_LeftEarPlay,
    HINTTONE_RightEarPlay,
    HINTTONE_LREarPlay
} HINTTONE_LREarSelect;

typedef struct
{
    HINTTONE_Vol HintToneVol;                        /*!< Configure the volume of mixed sound. */
    HINTTONE_MainVol MainVol;                        /*!< Configure the main volume of the mixed sound. */
    HINTTONE_WorkModeDev WorkMode;                   /*!< Mode configuration. */
    HINTTONE_LREarSelect LRPlay;                     /*!< The mixed sound comes out from the left or right channel or left and right channels. */
    uint8_t *StarAddr;                               /*!< The starting address of the data to be mixed. */
    uint16_t BuffLen;                                /*!< Buff size. */
} HINTTONE_InitTypeDef;

void AUDIO_DacAnaVcomNoCapacitanceConfig(DAC_AnaConfig config);
void AUDIO_DacAnaDiffNoCapacitanceConfig(DAC_AnaConfig config);
void AUDIO_DacAnaSENoCapacitanceConfig(DAC_AnaConfig config);
void AUDIO_HighPerformanceTest();
bool AUDIO_DacIsAnaOpen();
void AUDIO_DacAnaVcomConfig(DAC_AnaConfig config, DAC_OutputMode vmid_cfg);
void AUDIO_DacAnaDiffConfig(DAC_AnaConfig config, DAC_OutputMode vmid_cfg);
void AUDIO_DacAnaSEConfig(DAC_AnaConfig config, DAC_OutputMode vmid_cfg);
void AUDIO_DacInit(DAC_InitTypeDef dacparam);
void AUDIO_DacCoefConfig(void);
void AUDIO_DacClearRampDown(void);
void AUDIO_DacSelectChannel(DAC_InitTypeDef config);
void AUDIO_DacMonoSteroConfig(DAC_SoundTrackDev mono_flag);
void AUDIO_DacDigitalStop(void);
void AUDIO_DacAnaStop(void);
bool AUDIO_DacCheckEnable(void);
bool AUDIO_DacCheckEmpty(void);
bool AUDIO_DacCheckBufferMram(void);
uint16_t AUDIO_DacGetBufferSize(void);
uint16_t AUDIO_DacGetRemindBufferSize(void);
void AUDIO_DacMuteConfig(bool mute);
void AUDIO_DacSetWptr(uint8_t *wptrBuff);
bool AUDIO_DacVolConfig(uint8_t step, uint8_t Vol);
void HINTTONE_Init(HINTTONE_InitTypeDef config);
void HINTTONE_Stop(void);
void HINTTONE_SetWptr(uint8_t *hinttone_wptrbuff);
bool HINTTONE_CheckEmpty(void);
uint16_t HINTTONE_DacGetBufferSize(void);
uint16_t HINTTONE_DacGetRemindBufferSize(void);
#endif
