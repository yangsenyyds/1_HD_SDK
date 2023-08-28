#ifndef __YC11XX_TOUCH_H
#define __YC11XX_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "yc11xx.h"
#include "reg_addr.h"



/* 					  		  * ts_en					bit-30
							  * tch2_en					bit-29
							  * ts_clk_pw_sel			bit-28
							  * ts_clk_pw_width			bit-26
							  * ts_hys_op_ibc			bit-23
							  * ts_hys_vth_set_tch2		bit-20
							  * ts_hys_vtl_set_tch2		bit-17
							  * ts_hys_vth_set_tch1		bit-14
							  * ts_hys_vtl_set_tch1		bit-11
							  * ts_esi_set				bit-8
							  * ts_c_ext				bit-5
							  * ts_ido_en				bit-4
							  * ts_ild_op_sel			bit-2
							  * ts_ildo_sel				bit-0
 */
/**
 * @brief
 * @param    TS_ANA_DEFAULT default value
 * @return
 */
#define TS_ANA_DEFAULT	  	 ((0 << 30) +	\
							  (0 << 29) +	\
							  (0 << 28) +	\
							  (0 << 26) +	\
							  (1 << 23) + 	\
							  (3 << 20) + 	\
							  (3 << 17) + 	\
							  (3 << 14) + 	\
							  (3 << 11) +   \
							  (5 << 8)  +  	\
							  (0 << 5)  +	\
							  (0 << 4)  +	\
							  (1 << 2)  +	\
							  (1 << 0))

/*							 * ts_clk_div				bit-16
							 * 	ts_ch2_ts_en			bit-15
							 * 	ts_ch2_sfi_cfg			bit-12
							 * 	ts_ch2_avg_mode			bit-11
							 * 	ts_ch2_ffi_cfg			bit-8
							 * 	ts_ch1_ts_en			bit-7
							 * 	ts_ch1_sfi_cfg			bit-4
							 * 	ts_ch1_avg_mode			bit-3
							 * 	ts_ch1_ffi_cfg			bit-0
 */
/**
 * @brief
 * @param    TS_FIR_DEFAULT default value
 * @return
 */
#define TS_FIR_DEFAULT		 ((0 << 16) +	\
							  (0 << 15) + 	\
							  (0 << 12) + 	\
							  (0 << 11) + 	\
							  (0 << 8)  +  	\
							  (0 << 7)  +  	\
							  (0 << 4)  +  	\
							  (0 << 3)  +  	\
							  (0 << 0))


/*
							 * ts_ch1_mhd_r				bit-23
							 * ts_ch1_nhd_r				bit-18
							 * ts_ch1_ncl_r				bit-14
							 * ts_ch1_mhd_f				bit-9
							 * ts_ch1_nhd_f				bit-4
							 * ts_ch1_ncl_f				bit-0
 */
/**
 * @brief
 * @param    TS_BL1_DEFAULT default value
 * @return
 */
#define TS_BL1_DEFAULT		 ((1 << 23) + 	\
							  (2 << 18) + 	\
							  (0 << 14) + 	\
							  (1 << 9)  +  	\
							  (2 << 4)  +  	\
							  (0 << 0))


/*
							 * ts_ch2_mhd_r				bit-23
							 * ts_ch2_nhd_r				bit-18
							 * ts_ch2_ncl_r				bit-14
							 * ts_ch2_mhd_f				bit-9
							 * ts_ch2_nhd_f				bit-4
							 * ts_ch2_ncl_f				bit-0
 */
/**
 * @brief
 * @param    TS_BL2_DEFAULT default value
 * @return
 */
#define TS_BL2_DEFAULT		 ((1 << 23) + 	\
 							  (2 << 18) + 	\
 							  (0 << 14) + 	\
 							  (1 << 9)  +  	\
 							  (2 << 4)  +  	\
 							  (0 << 0))

/*
							 *	ts_ch2_baseline_wr		bit-30
							 *	ts_ch2_bl_track_en		bit-29
							 *	ts_ch2_nhd_t			bit-24
							 *	ts_ch2_tfrt_en			bit-23
							 *	ts_ch2_tfrt_cfg			bit-20
							 *	ts_ch2_fdl				bit-16
							 *	ts_ch1_baseline_wr		bit-14
							 *	ts_ch1_bl_track_en		bit-13
							 *	ts_ch1_nhd_t			bit-8
							 *	ts_ch1_tfrt_en			bit-7
							 *	ts_ch1_tfrt_cfg			bit-4
							 *	ts_ch1_fdl				bit-0
 */
/**
 * @brief
 * @param    TS_BL3_DEFAULT default value
 * @return
 */
#define TS_BL3_DEFAULT		 ((0 << 30)   + 	\
							  (0 << 29)   + 	\
							  (10 << 24)   + 	\
							  (0 << 23)   + 	\
							  (0 << 20)   + 	\
							  (0 << 16)   + 	\
							  (0 << 14)   + 	\
							  (0 << 13)   + 	\
							  (10 << 8)    +		\
							  (0 << 7)    + 	\
							  (0 << 4)    +		\
							  (0 << 0))

/*
 * 						* ts_ch2_baseline_cfg		bit-12
 * 						* ts_ch1_baseline_cfg		bit-0
 */
/**
 * @brief
 * @param    TS_BL4_DEFAULT default value
 * @return
 */
#define TS_BL4_DEFAULT		 ((0 << 12)  +		\
  	  	  	  	  	  	  	  (0 << 0))


/*
							 * ts_ch1_tth_cfg			bit-16
							 * ts_ch1_rth_cfg			bit-8
							 * ts_ch1_dt_cfg			bit-4
							 * ts_ch1_dr_cfg			bit-0
 */
/**
* @brief
* @param    TS_CMP1_DEFAULT default value
* @return
*/
#define TS_CMP1_DEFAULT		((0 << 16)	   + 		\
							         (0 <<  8)    + 		\
							         (0  <<  4)    + 		\
							         (0 << 0))

/*
							* ts_ch2_tth_cfg			bit-16
							* ts_ch2_rth_cfg			bit-8
							* ts_ch2_dt_cfg				bit-4
							* ts_ch2_dr_cfg				bit-0
*/
/**
 * @brief
 * @param    TS_CMP2_DEFAULT default value
 * @return
 */
#define TS_CMP2_DEFAULT		    ((0 << 16)	   + 		\
								     (0 <<  8)    + 		\
								     (0  <<  4)    + 		\
								     (0 << 0))

/*
							 * ts_ch1_tl				bit-20
							 * ts_ch1_usl				bit-10
							 * ts_ch1_lsl				bit-0
 */
/**
* @brief
* @param    TS_CAL1_DEFAULT default value
* @return
*/
#define TS_CAL1_DEFAULT		((660 << 20)	 +		\
							 (700 << 10)     +		\
							 (620 << 0))

/*
						  * ts_ch2_tl				bit-20
						  * ts_ch2_usl				bit-10
						  * ts_ch2_lsl				bit-0
*/
/**
 * @brief
 * @param    TS_CAL2_DEFAULT default value
 * @return
 */
#define TS_CAL2_DEFAULT	((660 << 20)	 +		\
 							 (700 << 10)     +		\
 							 (620 << 0))

/*
							 * ts_ch2_cal_en			bit-30
							 * ts_ch2_cal_bypass		bit-29
							 * ts_ch2_cdc_init			bit-24
							 * ts_ch2_ibc_init			bit-20
							 * ts_ch2_cal_cnt_th		bit-16
							 * ts_ch1_cal_en			bit-14
							 * ts_ch1_cal_bypass		bit-13
							 * ts_ch1_cdc_init			bit-8
							 * ts_ch1_ibc_init			bit-4
							 * ts_ch1_cal_cnt_th		bit-0
 */
/**
* @brief
* @param    TS_CAL3_DEFAULT default value
* @return
*/
#define TS_CAL3_DEFAULT		((1 << 30)	   	 + 		\
							 (0 << 29)       + 		\
							 (0 << 24)      + 		\
							 (0 << 20)       + 		\
							 (15 << 16)       + 		\
							 (1 << 14)       + 		\
							 (0 << 13)       + 		\
							 (0 << 8)     	 + 		\
							 (0 << 4)      	 + 		\
							 (15 << 0))

/*
							 * 	ts_ch2_cal_err_th		bit-8
							 * 	ts_ch1_cal_err_th		bit-0
*/
/**
 * @brief
 * @param    TS_CAL4_DEFAULT default value
 * @return
 */
#define TS_CAL4_DEFAULT		((30 << 8)	   + 		\
  	  	  	  	  	  	  	 (30))


#define TS_CLkBit_CLR  3
#define TS_FirCLk_Bit  BIT24
/**
 * @brief
 * @param    channel selection
 * @return
 */
typedef enum
{
    Ts_Ch1 = 0,
    Ts_Ch2,
    Ts_ChMax,
} TOUCH_Chx_Num;

typedef enum
{
    two_channel = 1,
    single_channel = 0,
} TOUCH_MODE;
/**
 * @brief
 * @param    touch status
 * @return
 */
typedef enum
{
    Ts_Release = 0,
    Ts_Touch_1,
    Ts_Touch_2,
} TOUCH_Status;

typedef enum
{
    osc_32k = 0,
    xo_32k,
} TOUCH_Clk_Select;

typedef enum
{
    ele_32k = 0,
    ele_bg,
} TOUCH_Ele_Select;

typedef enum
{
    Ts_Ana = 0,
    Ts_Fir,
    Ts_Cal1,
    Ts_Cal2,
    Ts_Cal3,
    Ts_Cal4,
    Ts_Cmp1,
    Ts_Cmp2,
    //Ts_analog,
    Ts_Bl1,
    Ts_Bl2,
    Ts_Bl3,
    Ts_Bl4,
} Touch_Reg_TypeDef;

typedef enum
{
    ch1_ts_status = 0,
    ch1_ts_cal_done,
    ch1_acff,
    ch2_ts_tsatus,
    ch2_ts_cal_done,
    ch2_acff,
} Touch_LPM_TS_STATUS;

typedef enum
{
	pad0_gpio34,
	pad1_gpio13,
	pad2_gpio10,
	pad_shutdown1,
} Touch_CH1_PAD_SELCT;

typedef enum
{
	pad0_gpio33,
	pad1_gpio12,
	pad2_gpio9,
	pad_shutdown2,
} Touch_CH2_PAD_SELCT;

typedef struct
{
    uint16_t baseLine1;
    uint16_t baseLine2;
} Basetrack_InitTypeDef;

typedef struct
{
    uint8_t ildo_sel33;
    uint8_t ild_op_sel33;
    uint8_t lpm_ts_ldo_en;
    uint8_t lpm_ts_ext;
    uint8_t ts_esi_set;
    uint8_t vtl_set_tch1;
    uint8_t vth_set_tch1;
    uint8_t vtl_set_tch2;
    uint8_t vth_set_tch2;
    uint8_t hys_op_ibc;
    uint8_t pw_width;
    uint8_t clk_pw_sel;
    uint8_t ts_en;/* Sets the sampling interval for the second filter*/
    uint8_t tch2_en;/* 1: Dual-channel mode.0: single-channel mode.Only channel 1 works.*/
} TOUCH_Ana_InitTypeDef;

typedef struct
{
    uint8_t ts_ch1_ffi_cfg;/*Smooth sampling points in channel 1 level 1,0 ~ 5 to 1/2/4/8/16/32*/
    uint8_t ts_ch1_avg_mode;/*The average method of smoothing module 1 is 0: direct average 1: average after removing the maximum and minimum values*/
    uint8_t ts_ch1_sfi_cfg;/*Smooth sampling points in channel 1 leve2 1,0 ~ 5 to 1/2/4/8/16/32*/
    uint8_t ts_ch1_en;/*Channel 2 digital working enable signal*/
    uint8_t ts_ch2_ffi_cfg;/*Smooth sampling points in channel 2 level 1,0 ~ 5 to 1/2/4/8/16/32*/
    uint8_t ts_ch2_avg_mode;/*The average method of smoothing module 1 is 0: direct average 1: average after removing the maximum and minimum values*/
    uint8_t ts_ch2_sfi_cfg;/*Smooth sampling points in channel 1 level 2,0 ~ 5 to 1/2/4/8/16/32*/
    uint8_t ts_ch2_en;/*Channel 2 digital working enable signal*/
    uint8_t ts_clk_div;/*The division ratio of the internal working clock, Defalt 32, the internal clock works at 1kHz.Valid values are only 3~127;*/
    uint8_t hys_delay_en;
    uint8_t ts_ibsel;
    uint8_t test_ts_ldo_en;
    uint8_t pad_sel_tch1;
    uint8_t pad_sel_tch2;
} TOUCH_Fir_InitTypeDef;

typedef struct
{

} TOUCH_Bl1_InitTypeDef;

typedef struct
{

} TOUCH_Bl2_InitTypeDef;

typedef struct
{
    uint8_t ts_ch1_baseline_wr;/* Channel_1Baseline is forcibly updated to register configuration value (BL_CTRL4) with rising edge in effect. */
    uint8_t ts_ch1_track_en;/* Channel_1 Baseline automatically tracks the enabling signal*/
    uint8_t ts_ch1_tfrt_en;/* Channel_1 Touch froce Release Time enable,0:disable,1:enable*/
    uint8_t ts_ch1_tfrt_cfg;/* Channel_1 Upper limit of continuous touch duration Default: 4s */
    uint8_t ts_ch1_fdl;/*Channel_1 Baseline tracks smoothing filter average sampling points */
    uint8_t ts_ch2_baseline_wr;/*Channel_2 Baseline is forcibly updated to register configuration value (BL_CTRL4) with rising edge in effect. */
    uint8_t ts_ch2_track_en;/*  Channel_2 Baseline automatically tracks the enabling signal*/
    uint8_t ts_ch2_tfrt_en;/*Channel_2 Touch froce Release Time enable,0:disable,1:enable */
    uint8_t ts_ch2_tfrt_cfg;/*Channel_2 Upper limit of continuous touch duration Default: 4s */
    uint8_t ts_ch2_fdl;/* Channel_2 Baseline tracks smoothing filter average sampling points */
} TOUCH_Bl3_InitTypeDef;

typedef struct
{
    uint32_t ts_ch1_baseline_cfg;/*Channel_1 Baseline configuration value, high 10bits, low 2 bits automatically fill 0 */
    uint32_t ts_ch2_baseline_cfg;/* Channel_2 Baseline configuration value, high 10bits, low 2 bits automatically fill 0*/
} TOUCH_Bl4_InitTypeDef;

typedef struct
{
    uint8_t ts_ch1_dr_cfg;/* Channel_1 The number of consecutive non-touch, more than the non-touch state.The Default 3 times*/
    uint8_t ts_ch1_dt_cfg;/*  Channel_1 If the number of continuous touches exceeds, the state of touch will be entered: Default 4 times*/
    uint8_t ts_ch1_rth_cfg;/* Channel_1 Release occurred threshold value*/
    uint8_t ts_ch1_tth_cfg;/* Channel_1 Touch occurrence threshold*/
} TOUCH_Cmp1_InitTypeDef;

typedef struct
{
    uint8_t ts_ch2_dr_cfg;/*Channel_2 The number of consecutive non-touch, more than the non-touch state.The Default 3 times*/
    uint8_t ts_ch2_dt_cfg;/* Channel_2 If the number of continuous touches exceeds, the state of touch will be entered: Default 4 times*/
    uint8_t ts_ch2_rth_cfg;/*  Channel_2 Release occurred threshold value*/
    uint8_t ts_ch2_tth_cfg;/*Channel_2 Touch occurrence threshold*/
} TOUCH_Cmp2_InitTypeDef;

typedef struct
{
    uint8_t ts_ch1_lsl;/* channel_1 baseline  Lower Side Limit,default 620 */
    uint8_t ts_ch1_usl;/* channel_1 baseline  Upper Side Limit,default 700	 */
    uint8_t ts_ch1_tl;/* channel_1 baseline target level,default 660*/
} TOUCH_Cal1_InitTypeDef;

typedef struct
{
    uint8_t ts_ch2_lsl;/* channel_2 baseline  Lower Side Limit,default 620 */
    uint8_t ts_ch2_usl;/* channel_2 baseline  Upper Side Limit,default 700	*/
    uint8_t ts_ch2_tl;/*  channel_2 baseline target level,default 660*/
} TOUCH_Cal2_InitTypeDef;

typedef struct
{
    uint8_t ts_ch1_ibc_init;/* channel_1 IBC initial value: Set the channel's charge/discharge bias current control: default=1*/
    uint8_t ts_ch1_cdc_init;/* channel_1 CDC initial value: Sets the channel charge and discharge current */
    uint8_t ts_ch2_ibc_init;/*  channel_2 IBC initial value: Set the channel's charge/discharge bias current control: default=1*/
    uint8_t ts_ch2_cbc_init;/* channel_2 CDC initial value: Sets the channel charge and discharge current*/
} TOUCH_Cal3_InitTypeDef;

typedef struct
{
    uint8_t ts_ch1_cal_err_th;/* channel_1 Error threshold between baseline and TL*/
    uint8_t ts_ch2_cal_err_th;/* channel_2 Error threshold between baseline and TL*/
} TOUCH_Cal4_InitTypeDef;

typedef struct
{
    TOUCH_Ana_InitTypeDef	TS_ANA_CTRL;
    TOUCH_Fir_InitTypeDef	TS_FIR_CTRL;
    TOUCH_Bl1_InitTypeDef	TS_BL1_CTRL;
    TOUCH_Bl2_InitTypeDef	TS_BL2_CTRL;
    TOUCH_Bl3_InitTypeDef	TS_BL3_CTRL;
    TOUCH_Bl4_InitTypeDef	TS_BL4_CTRL;
    TOUCH_Cmp1_InitTypeDef	TS_CMP1_CTRL;
    TOUCH_Cmp2_InitTypeDef	TS_CMP2_CTRL;
    TOUCH_Cal1_InitTypeDef	TS_CAL1_CTRL;
    TOUCH_Cal2_InitTypeDef	TS_CAL2_CTRL;
    TOUCH_Cal3_InitTypeDef	TS_CAL3_CTRL;
    TOUCH_Cal4_InitTypeDef	TS_CAL4_CTRL;
} TOUCH_InitTypeDef;


void TOUCH_Init(TOUCH_InitTypeDef *touch_init_struct);
void TOUCH_Set( FunctionalState newstate);
void TOUCH_PadProtect(Touch_CH1_PAD_SELCT ch1, Touch_CH2_PAD_SELCT ch2);
void TOUCH_PadChange(uint8_t pad1, uint8_t pad2);
void TOUCH_BaselineChange(uint16_t  baseLine1, uint16_t baseLine2);
void TOUCH_WriteReg(Touch_Reg_TypeDef touch_reg, uint32_t touch_ctrl);
uint32_t TOUCH_ReadReg(Touch_Reg_TypeDef touch_reg);
TOUCH_Status TOUCH_ReadStatus(TOUCH_Chx_Num	touchx);
uint32_t TOUCH_ReadBaseline(TOUCH_Chx_Num touchx);
uint32_t TOUCH_ReadFirData(TOUCH_Chx_Num touchx);
void  TOUCH_ChangeClk(TOUCH_Clk_Select clk_sel);
void TOUCH_Ibsel(TOUCH_Ele_Select ele_select);


#endif /* _YC11XX_TOUCH_H_ */
