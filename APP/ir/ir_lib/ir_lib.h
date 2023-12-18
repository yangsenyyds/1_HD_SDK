#ifndef IR_LIB_H
#define IR_LIB_H
#include "yc11xx.h"

enum LOAD_DATA_BIT {
    COUST_TYPE,
    IR_NUM,
    LEAD_L_TIME,
    LEAD_H_TIME,
    BIT0_L_TIME,
    BIT0_H_TIME,
    BIT1_L_TIME,
    BIT1_H_TIME,
    STOP_L_TIME,
    STOP_H_TIME_1,
    STOP_H_TIME_2,
    REPEAT_DISABLE,
    REPEAT_BUF,
    FREQ_SIZE,
    USER_REPEAT,
};

enum LOAD_TYPE {
    CODE1,
    FB739A,
    COSTOM2_CODE2,
    COSTOM2_CODE2_Panasonic,
    COSTOM1_CODE1_COSTOM1_CODE1,
    COSTOM2_CODE1_0,
    CODE1_COSTOM1_CODE1,
    CODE1_COSTOM1,
    CODE2_COSTOM2,
    COSTOM1_0_CODE1,
    NOPWM_IRT1250C5D7,
    STM708LB1,

    PHILIPS_STOP_CODE,
    PHILIPS_RC_MM,
    HIGH_PHILIPS_TOGGLE_CODE,
    HIGH_START_PHIL_COSTOM2_CODE2,
    HIGH_REF_CUSTOM_CODE,
    HIGH_TOGGLE_COSTOM2_CODE2,
    HIGH_COSTOM_CODE,
    HIGH_COSTOM2_CODE2,
    REPEAT_BIT_CODE,
    Zenith_D5,
};

typedef enum {
    rpt_lc7461_9000_4500,
    rpt_UDP6121_8550,
    rpt_UPD612G2_9000,
    rpt_GEMINI_8500,
    rpt_SINGH2150_8170,
    rpt_TC9012LIAN_9700,
    rpt_DVB_40BIT_100,
} rpt_code_t;

typedef enum {
    CUSTOM_00_00_A,
    CUSTOM_00_1F_A,
    CUSTOM_00_02_A,
    CUSTOM_00_2B_A,
    CUSTOM_00_30_A,
    CUSTOM_00_57_A,
    CUSTOM_00_7F_A,
    CUSTOM_00_80_A,
    CUSTOM_00_99_A,
    CUSTOM_00_AF_A,
    CUSTOM_00_BA_A,
    CUSTOM_00_BD_A,
    CUSTOM_00_BE_A,
    CUSTOM_00_BF_A,
    CUSTOM_00_DF_A,
    CUSTOM_00_D9_A,
    CUSTOM_00_EF_A,
    CUSTOM_00_F6_A,
    CUSTOM_00_F7_A,
    CUSTOM_00_FB_A,
    CUSTOM_00_FD_A,
    CUSTOM_00_FF_A,
    CUSTOM_01_00_A,
    CUSTOM_01_01_A,
    CUSTOM_01_FE_A,
    CUSTOM_02_00_A,
    CUSTOM_02_1D_A,
    CUSTOM_02_BD_A,
    CUSTOM_02_F5_A,
    CUSTOM_02_FD_A,
    CUSTOM_02_7D_A,
    CUSTOM_02_7F_A,
    CUSTOM_02_FF_A,
    CUSTOM_03_00_A,
    CUSTOM_03_AC_A,
    CUSTOM_03_FC_A,
    CUSTOM_04_00_A,
    CUSTOM_04_10_A,
    CUSTOM_04_B9_A,
    CUSTOM_04_FB_A,
    CUSTOM_05_00_A,
    CUSTOM_05_05_A,
    CUSTOM_06_06_A,
    CUSTOM_06_FB_A,
    CUSTOM_06_FE_A,
    CUSTOM_07_07_A,
    CUSTOM_08_00_A,
    CUSTOM_08_08_A,
    CUSTOM_08_0B_A,
    CUSTOM_08_13_A,
    CUSTOM_08_F7_A,
    CUSTOM_09_F6_A,
    CUSTOM_0A_35_A,
    CUSTOM_0A_00_A,
    CUSTOM_0A_0A_A,
    CUSTOM_0B_0B_A,
    CUSTOM_0D_0D_A,
    CUSTOM_0E_09_A,
    CUSTOM_0E_0E_A,
    CUSTOM_0F_00_A,
    CUSTOM_10_EF_A,
    CUSTOM_10_ED_A,
    CUSTOM_10_2D_A,
    CUSTOM_12_12_A,
    CUSTOM_12_ED_A,
    CUSTOM_13_14_A,
    CUSTOM_14_00_A,
    CUSTOM_16_29_A,
    CUSTOM_16_E9_A,
    CUSTOM_18_18_A,
    CUSTOM_18_E7_A,
    CUSTOM_18_E9_A,
    CUSTOM_1A_00_A,
    CUSTOM_1B_00_A,
    CUSTOM_1C_E3_A,
    CUSTOM_1E_1E_A,
    CUSTOM_1E_E1_A,
    CUSTOM_1F_00_A,
    CUSTOM_20_40_A,
    CUSTOM_20_DF_A,
    CUSTOM_20_FD_A,
    CUSTOM_21_DE_A,
    CUSTOM_22_11_A,
    CUSTOM_23_45_A,    
    CUSTOM_24_DB_A,
    CUSTOM_29_D6_A,
    CUSTOM_2B_D4_A,
    CUSTOM_33_00_A,
    CUSTOM_33_CC_A,
    CUSTOM_35_50_A,
    CUSTOM_38_C7_A,
    CUSTOM_3F_C0_A,
    CUSTOM_3F_3F_A,
    CUSTOM_40_02_A,
    CUSTOM_40_20_A,
    CUSTOM_40_40_A,
    CUSTOM_40_BF_A,
    CUSTOM_42_BD_A,
    CUSTOM_42_FD_A,
    CUSTOM_43_00_A,
    CUSTOM_45_16_A,
    CUSTOM_45_BC_A,
    CUSTOM_46_00_A,
    CUSTOM_47_00_A,
    CUSTOM_4B_B4_A,
    CUSTOM_4F_50_A,
    CUSTOM_50_AD_A,
    CUSTOM_50_AF_A,
    CUSTOM_54_00_A,
    CUSTOM_58_A7_A,
    CUSTOM_5A_49_A,
    CUSTOM_60_9F_A,
    CUSTOM_60_9D_A,
    CUSTOM_61_63_A,
    CUSTOM_6A_00_A,
    CUSTOM_69_F1_A,
    CUSTOM_71_8E_A,
    CUSTOM_73_00_A,
    CUSTOM_76_7B_A,
    CUSTOM_77_00_A,
    CUSTOM_77_88_A,
    CUSTOM_7D_00_A,
    CUSTOM_7F_00_A,
    CUSTOM_80_23_A,
    CUSTOM_80_80_A,
    CUSTOM_80_57_A,
    CUSTOM_80_63_A,
    CUSTOM_80_64_A,
    CUSTOM_80_6F_A,
    CUSTOM_80_7F_A,
    CUSTOM_80_8F_A,
    CUSTOM_82_F3_A,
    CUSTOM_83_55_A,
    CUSTOM_83_F1_A,
    CUSTOM_83_F7_A,
    CUSTOM_84_E0_A,
    CUSTOM_85_46_A,
    CUSTOM_86_02_A,
    CUSTOM_86_05_A,
    CUSTOM_86_6B_A,
    CUSTOM_87_22_A,
    CUSTOM_87_4E_A,
    CUSTOM_88_45_A,
    CUSTOM_95_6A_A,
    CUSTOM_96_69_A,
    CUSTOM_97_00_A,
    CUSTOM_A0_5F_A,
    CUSTOM_A0_87_A,
    CUSTOM_A4_00_A,
    CUSTOM_AA_03_A,
    CUSTOM_AA_55_A,
    CUSTOM_C4_00_A,
    CUSTOM_E5_70_A,
    CUSTOM_E6_00_A,
    CUSTOM_E7_0A_A,
    CUSTOM_EA_C7_A,
    CUSTOM_EA_C8_A,
    CUSTOM_EE_00_A,
    CUSTOM_DC_00_A,
    CUSTOM_DE_D6_A,
    CUSTOM_F4_00_A,
    CUSTOM_FE_01_A,

    CUSTOM_0000_B000_A,
    CUSTOM_0000_FFFF_A,
    CUSTOM_055A_0000_A,
    CUSTOM_0057_0000_A,
    CUSTOM_007B_1F84_A,
    CUSTOM_081F_0000_A,
    CUSTOM_0119_1EE6_A,
    CUSTOM_011A_FEE5_A,
    CUSTOM_011C_1EE3_A,
    CUSTOM_013C_1EC3_A,
    CUSTOM_0114_FEEB_A,
    CUSTOM_0200_0000_A,
    CUSTOM_1463_0084_A,
    CUSTOM_2002_0080_A,
    CUSTOM_2002_0280_A,
    CUSTOM_2002_0480_A,
    CUSTOM_2002_0090_A,
    CUSTOM_2002_4880_A,
    CUSTOM_29C0_2600_A,
    CUSTOM_5AAA_162F_A,
    CUSTOM_8000_FFFF_A,
    CUSTOM_87E1_F000_A,
    CUSTOM_BFFF_FFFF_A,
    CUSTOM_434F_004E_A,
    CUSTOM_FFFE_00A4_A,
    CUSTOM_FFFE_00A5_A,
    CUSTOM_FFFE_006A_A,
    CUSTOM_FFFD_0000_A,
    CUSTOM_FFFD_0001_A,
    CUSTOM_FFFD_FFFF_A,
    CUSTOM_FFFF_FFFF_A,

    HIGH_CUSTOM_0000_1800_A,
    HIGH_CUSTOM_3000_0000_A,
    HIGH_CUSTOM_4000_0000_A,
    HIGH_CUSTOM_8000_0000_A,
    HIGH_CUSTOM_8000_2800_A,
    HIGH_CUSTOM_8100_7E00_A,
    HIGH_CUSTOM_A000_0000_A,
    HIGH_CUSTOM_B000_6000_A,
    HIGH_CUSTOM_B000_A000_A,
    HIGH_CUSTOM_B000_F000_A,
    HIGH_CUSTOM_C000_0000_A,
    HIGH_CUSTOM_C000_0800_A,
    HIGH_CUSTOM_C000_6000_0C_A,
    HIGH_CUSTOM_FFFD_6000_A,
    HIGH_CUSTOM_FFFD_E000_A,
    HIGH_CUSTOM_FFFD_D000_3010_1_1A_A,
    HIGH_CUSTOM_FFFD_8800_A,
    DEFAULT_CUSTOMER = 0xFFFF,
} customer_code_t;

typedef enum {
    CUSTOM_6BIT_02,
    CUSTOM_C1D6_03,
    DVB_PAN_7051_SAMSUN_05,
    FB739A_06,
    GEMINI_C10_32_08,
    GEMINI_C16_38_08,
    GEMINI_C10_38_09,
    GEMINI_C10_32_10,
    IRT1250C5D6_12,
    IRT1250C5D7_13,
    KK_Y261_14,
    LBNSAMSUNG_15,
    LC7461M_C13S_16,
    LC7461M_C13S_17,
    LC7464M_PANASONIC_18,
    LG3004_33K_19_Toggle_zero,
    LG3004_33K_19_Toggle_one,
    LG3004_33K_19_Toggle_two,
    LG3004_33K_19_TOGGLE_TWO_Freq_0,
    M3004_38K_20,
    M3004_38K_20_toggle_zero,
    M3004_38K_400_21,
    M50119P_23,
    M50462_24,
    M50560_25,
    MITC8D8_28,
    MN6014C6D6_29,
    MN6014C5D638_30,
    MN6014C5D656_31,
    N3201_32,
    PANPROJECTOR_37,
    PHILIPS_20_38,
    PHILIPS_RC_MM_39,
    PILOT370_40,
    PIONEER_41,
    RCA_56_42,
    RCA_38_43,
    RCA_56_44,
    SAA3010_38_45,
    SAA3010_36_46,
    SAMSUNG_49,
    LX0773CE_50,
    SINGH2150_51,
    TC901_54,
    TC901_D8B8_55,
    RCT311_56,
    UPD6121G_64,
    UPD6121G_40K_65,
    UPD6121F_LIAN_66,
    UPD6121G_001_67,
    UPD6121G2_68,
    UPD6124_D7C5_69,
    UPD6124_D7C8_70,
    UPD6124_D7C13_71,
    VICTOR_C8D8_72,
    XY_2106_75,
    ZENITH_S10_77,
    ZENITH_S10_77_Zero,
    ZENITH_D5_78,
    ZENITH_D7_79,
    ZENITHSIM_C2D8_82,
    Unknow_C8D5_83,
    UPD6121G_001_APPLE_90,
    Pioneer_91,
    STM708LB1_92,
    JX_9070_93,
    GD_2000_94,
    XY_JAECS_95,
    DVB_40BIT_100,
    DEFAULT_TYPE = 0xFFFF,
} ir_type_t;

extern uint16_t *get_ir_rpt(rpt_code_t rpt);
extern uint16_t *get_ir_customer(customer_code_t custom);
extern uint16_t *get_ir_brand(ir_type_t type);

#endif
