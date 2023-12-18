#include "ir_lib.h"

static const uint16_t rpt_TC9012LIAN_9700_BUF[] = {
    600, 1660,
    600, 60000, 34000
};
static const uint16_t rpt_lc7461_9000_4500_BUF[] = {
    9000, 4500,
    560, 60000, 34000
};
static const uint16_t rpt_udp6121_9000_BUF[] = {
    9000, 2250,
    560, 60000, 34000
};
static const uint16_t rpt_udp6121_8550_BUF[] = {
    8550, 2140,
    532, 60000, 34000
};
static const uint16_t rpt_GEMINI_8500_BUF[] = {
    0, 0,
    0, 60000, 30000
};
static const uint16_t rpt_SINGH2150_8170_BUF[] = {
    0, 0,
    0, 60000, 21700
};
static const uint16_t rpt_DVB_40BIT_BUF[] = {
    3600, 3600,
    560, 60000, 36000
};

static const uint16_t CUSTOM_00_00_BUF[] = {0X00, 0x00};
static const uint16_t CUSTOM_00_1F_BUF[] = {0X00, 0x1F};
static const uint16_t CUSTOM_00_02_BUF[] = {0X00, 0x02};
static const uint16_t CUSTOM_00_2B_BUF[] = {0X00, 0x2B};
static const uint16_t CUSTOM_00_30_BUF[] = {0X00, 0x30};
static const uint16_t CUSTOM_00_57_BUF[] = {0X00, 0x57};
static const uint16_t CUSTOM_00_7F_BUF[] = {0X00, 0x7F};
static const uint16_t CUSTOM_00_80_BUF[] = {0X00, 0x80};
static const uint16_t CUSTOM_00_99_BUF[] = {0X00, 0x99};
static const uint16_t CUSTOM_00_AF_BUF[] = {0X00, 0xAF};
static const uint16_t CUSTOM_00_BA_BUF[] = {0X00, 0xBA};
static const uint16_t CUSTOM_00_BD_BUF[] = {0X00, 0xBD};
static const uint16_t CUSTOM_00_BE_BUF[] = {0X00, 0xBE};
static const uint16_t CUSTOM_00_BF_BUF[] = {0X00, 0xBF};
static const uint16_t CUSTOM_00_DF_BUF[] = {0X00, 0xDF};
static const uint16_t CUSTOM_00_D9_BUF[] = {0X00, 0xD9};
static const uint16_t CUSTOM_00_EF_BUF[] = {0X00, 0xEF};
static const uint16_t CUSTOM_00_F6_BUF[] = {0X00, 0xF6};
static const uint16_t CUSTOM_00_F7_BUF[] = {0X00, 0xF7};
static const uint16_t CUSTOM_00_FB_BUF[] = {0X00, 0xFB};
static const uint16_t CUSTOM_00_FD_BUF[] = {0X00, 0xFD};
static const uint16_t CUSTOM_00_FF_BUF[] = {0X00, 0xFF};
static const uint16_t CUSTOM_01_00_BUF[] = {0X01, 0x00};
static const uint16_t CUSTOM_01_01_BUF[] = {0X01, 0x01};
static const uint16_t CUSTOM_01_FE_BUF[] = {0X01, 0xFE};
static const uint16_t CUSTOM_02_00_BUF[] = {0X02, 0x00};
static const uint16_t CUSTOM_02_1D_BUF[] = {0X02, 0x1D};
static const uint16_t CUSTOM_02_BD_BUF[] = {0X02, 0xBD};
static const uint16_t CUSTOM_02_FD_BUF[] = {0X02, 0xFD};
static const uint16_t CUSTOM_02_7D_BUF[] = {0X02, 0x7D};
static const uint16_t CUSTOM_02_7F_BUF[] = {0X02, 0x7F};
static const uint16_t CUSTOM_02_FF_BUF[] = {0X02, 0xFF};
static const uint16_t CUSTOM_02_F5_BUF[] = {0X02, 0xF5};
static const uint16_t CUSTOM_03_00_BUF[] = {0X03, 0x00};
static const uint16_t CUSTOM_03_AC_BUF[] = {0X03, 0xAC};
static const uint16_t CUSTOM_03_FC_BUF[] = {0X03, 0xFC};
static const uint16_t CUSTOM_04_00_BUF[] = {0X04, 0x00};
static const uint16_t CUSTOM_04_10_BUF[] = {0X04, 0x10};
static const uint16_t CUSTOM_04_B9_BUF[] = {0X04, 0xB9};
static const uint16_t CUSTOM_04_FB_BUF[] = {0X04, 0xFB};
static const uint16_t CUSTOM_05_00_BUF[] = {0X05, 0x00};
static const uint16_t CUSTOM_05_05_BUF[] = {0X05, 0x05};
static const uint16_t CUSTOM_06_06_BUF[] = {0X06, 0x06};
static const uint16_t CUSTOM_06_FB_BUF[] = {0X06, 0xFB};
static const uint16_t CUSTOM_06_FE_BUF[] = {0X06, 0xFE};
static const uint16_t CUSTOM_07_07_BUF[] = {0X07, 0x07};
static const uint16_t CUSTOM_08_00_BUF[] = {0X08, 0x00};
static const uint16_t CUSTOM_08_08_BUF[] = {0X08, 0x08};
static const uint16_t CUSTOM_08_0B_BUF[] = {0X08, 0x0B};
static const uint16_t CUSTOM_08_13_BUF[] = {0X08, 0x13};
static const uint16_t CUSTOM_08_F7_BUF[] = {0X08, 0xF7};
static const uint16_t CUSTOM_09_F6_BUF[] = {0X09, 0xF6};
static const uint16_t CUSTOM_0A_35_BUF[] = {0X0A, 0x35};
static const uint16_t CUSTOM_0A_00_BUF[] = {0X0A, 0x00};
static const uint16_t CUSTOM_0A_0A_BUF[] = {0X0A, 0x0A};
static const uint16_t CUSTOM_0B_0B_BUF[] = {0X0B, 0x0B};
static const uint16_t CUSTOM_0D_0D_BUF[] = {0X0D, 0x0D};
static const uint16_t CUSTOM_0E_09_BUF[] = {0X0E, 0x09};
static const uint16_t CUSTOM_0E_0E_BUF[] = {0X0E, 0x0E};
static const uint16_t CUSTOM_0F_00_BUF[] = {0X0F, 0x00};
static const uint16_t CUSTOM_10_ED_BUF[] = {0X10, 0xED};
static const uint16_t CUSTOM_10_EF_BUF[] = {0X10, 0xEF};
static const uint16_t CUSTOM_10_2D_BUF[] = {0X10, 0x2D};
static const uint16_t CUSTOM_12_12_BUF[] = {0X12, 0x12};
static const uint16_t CUSTOM_12_ED_BUF[] = {0X12, 0xED};
static const uint16_t CUSTOM_13_14_BUF[] = {0X13, 0x14};
static const uint16_t CUSTOM_14_00_BUF[] = {0X14, 0X00};
static const uint16_t CUSTOM_16_29_BUF[] = {0X16, 0X29};
static const uint16_t CUSTOM_16_E9_BUF[] = {0X16, ~0X16};
static const uint16_t CUSTOM_18_18_BUF[] = {0X18, 0X18};
static const uint16_t CUSTOM_18_E7_BUF[] = {0X18, 0xE7};
static const uint16_t CUSTOM_18_E9_BUF[] = {0X18, 0xE9};
static const uint16_t CUSTOM_1A_00_BUF[] = {0X1A, 0x00};
static const uint16_t CUSTOM_1B_00_BUF[] = {0X1B, 0x00};
static const uint16_t CUSTOM_1C_E3_BUF[] = {0X1C, 0xE3};
static const uint16_t CUSTOM_1E_1E_BUF[] = {0X1E, 0x1E};
static const uint16_t CUSTOM_1E_E1_BUF[] = {0X1E, 0xE1};
static const uint16_t CUSTOM_1F_00_BUF[] = {0X1F, 0x00};
static const uint16_t CUSTOM_20_40_BUF[] = {0X20, 0x40};
static const uint16_t CUSTOM_20_DF_BUF[] = {0X20, 0xDF};
static const uint16_t CUSTOM_20_FD_BUF[] = {0X20, 0xFD};
static const uint16_t CUSTOM_21_DE_BUF[] = {0X21, 0xDE};
static const uint16_t CUSTOM_22_11_BUF[] = {0X22, 0x11};
static const uint16_t CUSTOM_24_DB_BUF[] = {0X24, 0xDB};
static const uint16_t CUSTOM_29_D6_BUF[] = {0X29, 0xD6};
static const uint16_t CUSTOM_2B_D4_BUF[] = {0X2B, 0xD4};
static const uint16_t CUSTOM_33_00_BUF[] = {0X33, 0x00};
static const uint16_t CUSTOM_33_CC_BUF[] = {0X33, 0xCC};
static const uint16_t CUSTOM_35_50_BUF[] = {0X35, 0x50};
static const uint16_t CUSTOM_38_C7_BUF[] = {0X38, 0xC7};
static const uint16_t CUSTOM_3F_C0_BUF[] = {0X3F, 0xC0};
static const uint16_t CUSTOM_3F_3F_BUF[] = {0X3F, 0x3F};
static const uint16_t CUSTOM_40_02_BUF[] = {0X40, 0x02};
static const uint16_t CUSTOM_40_20_BUF[] = {0X40, 0x20};
static const uint16_t CUSTOM_40_40_BUF[] = {0X40, 0x40};
static const uint16_t CUSTOM_40_BF_BUF[] = {0X40, 0xBF};
static const uint16_t CUSTOM_42_BD_BUF[] = {0X42, 0xBD};
static const uint16_t CUSTOM_42_FD_BUF[] = {0X42, 0xFD};
static const uint16_t CUSTOM_43_00_BUF[] = {0X43, 0x00};
static const uint16_t CUSTOM_45_16_BUF[] = {0X45, 0x16};
static const uint16_t CUSTOM_23_45_BUF[] = {0X23, 0x45};
static const uint16_t CUSTOM_45_BC_BUF[] = {0X45, 0xBC};
static const uint16_t CUSTOM_46_00_BUF[] = {0X46, 0x00};
static const uint16_t CUSTOM_47_00_BUF[] = {0X47, 0x00};
static const uint16_t CUSTOM_4B_B4_BUF[] = {0X4B, 0xB4};
static const uint16_t CUSTOM_4F_50_BUF[] = {0X4F, 0x50};
static const uint16_t CUSTOM_50_AD_BUF[] = {0X50, 0xAD};
static const uint16_t CUSTOM_50_AF_BUF[] = {0X50, 0xAF};
static const uint16_t CUSTOM_54_00_BUF[] = {0X54, 0x00};
static const uint16_t CUSTOM_58_A7_BUF[] = {0X58, 0xA7};
static const uint16_t CUSTOM_5A_49_BUF[] = {0X5A, 0x49};
static const uint16_t CUSTOM_60_9F_BUF[] = {0X60, 0x9F};
static const uint16_t CUSTOM_60_9D_BUF[] = {0X60, 0x9D};
static const uint16_t CUSTOM_61_63_BUF[] = {0X61, 0x63};
static const uint16_t CUSTOM_6A_00_BUF[] = {0X6A, 0x00};
static const uint16_t CUSTOM_69_F1_BUF[] = {0X69, 0xF1};
static const uint16_t CUSTOM_71_8E_BUF[] = {0X71, 0x8E};
static const uint16_t CUSTOM_73_00_BUF[] = {0X73, 0x00};
static const uint16_t CUSTOM_76_7B_BUF[] = {0X76, 0x7B};
static const uint16_t CUSTOM_77_00_BUF[] = {0X77, 0x00};
static const uint16_t CUSTOM_77_88_BUF[] = {0X77, 0x88};
static const uint16_t CUSTOM_7D_00_BUF[] = {0X7D, 0x00};
static const uint16_t CUSTOM_7F_00_BUF[] = {0X7F, 0x00};
static const uint16_t CUSTOM_80_23_BUF[] = {0X80, 0x23};
static const uint16_t CUSTOM_80_80_BUF[] = {0X80, 0x80};
static const uint16_t CUSTOM_80_57_BUF[] = {0X80, 0x57};
static const uint16_t CUSTOM_80_63_BUF[] = {0X80, 0x63};
static const uint16_t CUSTOM_80_64_BUF[] = {0X80, 0x64};
static const uint16_t CUSTOM_80_6F_BUF[] = {0X80, 0x6F};
static const uint16_t CUSTOM_80_7F_BUF[] = {0X80, 0x7F};
static const uint16_t CUSTOM_80_8F_BUF[] = {0X80, 0x8F};
static const uint16_t CUSTOM_82_F3_BUF[] = {0X82, 0xF3};
static const uint16_t CUSTOM_83_55_BUF[] = {0X83, 0x55};
static const uint16_t CUSTOM_83_F1_BUF[] = {0X83, 0xF1};
static const uint16_t CUSTOM_83_F7_BUF[] = {0X83, 0xF7};
static const uint16_t CUSTOM_84_E0_BUF[] = {0X84, 0xE0};
static const uint16_t CUSTOM_85_46_BUF[] = {0X85, 0x46};
static const uint16_t CUSTOM_86_02_BUF[] = {0X86, 0x02};
static const uint16_t CUSTOM_86_05_BUF[] = {0X86, 0x05};
static const uint16_t CUSTOM_86_6B_BUF[] = {0X86, 0x6B};
static const uint16_t CUSTOM_87_22_BUF[] = {0X87, 0x22};
static const uint16_t CUSTOM_87_4E_BUF[] = {0X87, 0x4E};
static const uint16_t CUSTOM_88_45_BUF[] = {0X88, 0x45};
static const uint16_t CUSTOM_95_6A_BUF[] = {0X95, 0x6A};
static const uint16_t CUSTOM_96_69_BUF[] = {0X96, 0x69};
static const uint16_t CUSTOM_97_00_BUF[] = {0X97, 0x00};
static const uint16_t CUSTOM_A0_5F_BUF[] = {0XA0, 0x5F};
static const uint16_t CUSTOM_A0_87_BUF[] = {0XA0, 0x87};
static const uint16_t CUSTOM_A4_00_BUF[] = {0XA4, 0x00};
static const uint16_t CUSTOM_AA_03_BUF[] = {0XAA, 0x03};
static const uint16_t CUSTOM_AA_55_BUF[] = {0XAA, 0x55};
static const uint16_t CUSTOM_C4_00_BUF[] = {0XC4, 0x00};
static const uint16_t CUSTOM_E5_70_BUF[] = {0XE5, 0x70};
static const uint16_t CUSTOM_E6_00_BUF[] = {0XE6, 0x00};
static const uint16_t CUSTOM_E7_0A_BUF[] = {0XE7, 0x0A};
static const uint16_t CUSTOM_EA_C7_BUF[] = {0XEA, 0xC7};
static const uint16_t CUSTOM_EA_C8_BUF[] = {0XEA, 0xC8};
static const uint16_t CUSTOM_EE_00_BUF[] = {0XEE, 0x00};
static const uint16_t CUSTOM_DC_00_BUF[] = {0XDC, 0x00};
static const uint16_t CUSTOM_DE_D6_BUF[] = {0XDE, 0xD6};
static const uint16_t CUSTOM_F4_00_BUF[] = {0XF4, 0x00};
static const uint16_t CUSTOM_FE_01_BUF[] = {0XFE, 0x01};

static const uint16_t CUSTOM_0000_B000_BUF[] = {0X0000, 0xB000};
static const uint16_t CUSTOM_0000_FFFF_BUF[] = {0X0000, 0xFFFF};
static const uint16_t CUSTOM_055A_0000_BUF[] = {0X055A, 0x0000};
static const uint16_t CUSTOM_0057_0000_BUF[] = {0x0057, 0x0000};
static const uint16_t CUSTOM_007B_1F84_BUF[] = {0x007B, 0x1F84};
static const uint16_t CUSTOM_081F_0000_BUF[] = {0x081F, 0x0000};
static const uint16_t CUSTOM_0119_1EE6_BUF[] = {0x0119, 0x1EE6};
static const uint16_t CUSTOM_011A_FEE5_BUF[] = {0x011A, 0xFEE5};
static const uint16_t CUSTOM_011C_1EE3_BUF[] = {0x011C, 0x1EE3};
static const uint16_t CUSTOM_013C_1EC3_BUF[] = {0x013C, 0x1EC3};
static const uint16_t CUSTOM_0114_FEEB_BUF[] = {0x0114, 0xFEEB};
static const uint16_t CUSTOM_0200_0000_BUF[] = {0x0200, 0x0000};
static const uint16_t CUSTOM_1463_0084_BUF[] = {0x6314, 0x8400};
static const uint16_t CUSTOM_2002_4880_BUF[] = {0x2002, 0x4880};
static const uint16_t CUSTOM_29C0_2600_BUF[] = {0x29C0, 0x2600};
static const uint16_t CUSTOM_5AAA_162F_BUF[] = {0x5AAA, 0x162F};
static const uint16_t CUSTOM_8000_FFFF_BUF[] = {0x8000, 0xFFFF};
static const uint16_t CUSTOM_87E1_F000_BUF[] = {0x87E1, 0x000F};
static const uint16_t CUSTOM_2002_0090_BUF[] = {0X2002, 0x0090};
static const uint16_t CUSTOM_434F_004E_BUF[] = {0x434F, 0X004E};
static const uint16_t CUSTOM_BFFF_FFFF_BUF[] = {0xBFFF, 0XFFFF};
static const uint16_t CUSTOM_FFFD_0000_BUF[] = {0xFFFD, 0X0000};
static const uint16_t CUSTOM_FFFE_00A4_BUF[] = {0xFFFE, 0X004A};
static const uint16_t CUSTOM_FFFE_00A5_BUF[] = {0xFFFE, 0X00A5};
static const uint16_t CUSTOM_FFFE_006A_BUF[] = {0xFFFE, 0X006A};
static const uint16_t CUSTOM_FFFD_0001_BUF[] = {0xFFFD, 0X0001};
static const uint16_t CUSTOM_FFFD_FFFF_BUF[] = {0xFFFD, 0XFFFF};
static const uint16_t CUSTOM_FFFF_FFFF_BUF[] = {0xFFFF, 0XFFFF};

static const uint16_t HIGH_CUSTOM_2002_0080_BUF[] = {0x2002, 0X0080};
static const uint16_t CUSTOM_2002_0280_BUF[]      = {0x2002, 0X0280};
static const uint16_t CUSTOM_2002_0480_BUF[]      = {0x2002, 0X0480};
static const uint16_t HIGH_CUSTOM_0000_1800_BUF[] = {0x0000, 0X1800};
static const uint16_t HIGH_CUSTOM_3000_0000_BUF[] = {0x3000, 0X0080};
static const uint16_t HIGH_CUSTOM_4000_0000_BUF[] = {0x4000, 0X0000};
static const uint16_t HIGH_CUSTOM_8000_0000_BUF[] = {0X8000, 0x0000};
static const uint16_t HIGH_CUSTOM_8000_2800_BUF[] = {0X8000, 0x2800};
static const uint16_t HIGH_CUSTOM_8100_7E00_BUF[] = {0X8100, 0x7E00};
static const uint16_t HIGH_CUSTOM_A000_0000_BUF[] = {0XA000, 0X0000};
static const uint16_t HIGH_CUSTOM_B000_6000_BUF[] = {0XB000, 0X6000};
static const uint16_t HIGH_CUSTOM_B000_A000_BUF[] = {0XB000, 0XA000};
static const uint16_t HIGH_CUSTOM_B000_F000_BUF[] = {0XB000, 0XF000};
static const uint16_t HIGH_CUSTOM_C000_0000_BUF[] = {0XC000, 0X0000};
static const uint16_t HIGH_CUSTOM_C000_0800_BUF[] = {0XC000, 0X0800};
static const uint16_t HIGH_CUSTOM_C000_6000_BUF[] = {0XC000, 0X6000};
static const uint16_t HIGH_CUSTOM_FFFD_6000_BUF[] = {0xFFFD, 0X6000};
static const uint16_t HIGH_CUSTOM_FFFD_E000_BUF[] = {0xFFFD, 0XE000};
static const uint16_t HIGH_CUSTOM_FFFD_D000_3010_1_1A_A_BUF[] = {0xFFFD, 0XD000};
static const uint16_t HIGH_CUSTOM_FFFD_8800_BUF[] = {0xFFFD, 0X8800};

static const uint16_t CUSTOM_6BIT_02_BUF[] = {
    CODE1, 0x6000, // REPEAT_DISABLE src   keynum
    0, 0, //LEAD TIME
    495, 1470, // BIT0_L_TIME  BIT0_H_TIME
    495, 3470, //  BIT1_L_TIME  BIT1_H_TIME
    590, 29000, 0, // STOP TIME
    0, 0, 38000, 1, // REPEAT_DISABLE src FREQ
};
static const uint16_t CUSTOM_C1D6_03_BUF[] = {
    CODE1_COSTOM1, 0x6100,
    0, 0,
    160, 5000,
    160, 7000,
    590, 29000, 0,
    0, 0, 38000, 0,
};
static const uint16_t DVB_PAN_7051_SAMSUN_05_BUF[] = {
    COSTOM2_CODE1_0, 0xFFF0,
    3400, 1670,
    426, 416,
    426, 1288,
    426, 44500, 0,
    0, 0, 38000, 0,
};
static const uint16_t FB739A_06_BUF[] = {
    FB739A, 0X8500,
    0, 0,
    345, 0,
    230, 1,
    0, 45000, 60000,
    0, 0, 38000, 0,
};
static const uint16_t GEMINI_C16_38_08_BUF[] = {
    PHILIPS_STOP_CODE, 0xf1e1,
    500, 2500,
    500, 0,
    500, 1,
    0, 14500, 0,
    1, rpt_GEMINI_8500, 31400, 1,
};
static const uint16_t GEMINI_C10_32_08_BUF[] = {
    PHILIPS_STOP_CODE, 0xA190,
    500, 2500,
    500, 0,
    500, 1,
    0, 22600, 0,
    1, rpt_GEMINI_8500, 31400, 1,
};
static const uint16_t GEMINI_C10_38_09_BUF[] = {
    PHILIPS_STOP_CODE, 0xA190,
    510, 2150,
    510, 0,
    510, 1,
    0, 22600, 0,
    1, rpt_GEMINI_8500, 38000, 0,
};
static const uint16_t GEMINI_C10_32_10_BUF[] = {
    PHILIPS_STOP_CODE, 0xA170,
    510, 2550,
    510, 0,
    510, 1,
    0, 22600, 0,
    1, rpt_GEMINI_8500, 31400, 0,
};
static const uint16_t IRT1250C5D6_12_BUF[] = {
    COSTOM2_CODE2, 0x5060,
    10, 290,
    10, 80,
    10, 180,
    10, 63000, 0,
    0, 0, 0, 0,
};
static const uint16_t IRT1250C5D7_13_BUF[] = {
    NOPWM_IRT1250C5D7, 0x5070,
    5, 380,
    5, 80,
    5, 280,
    5, 44000, 0,
    0, 0, 0, 0,
};
static const uint16_t KK_Y261_14_BUF[] = {
    HIGH_COSTOM_CODE, 0x8080,
    2990, 2970,
    490, 1470,
    490, 2470,
    490, 20000, 0,
    0, 0, 38000, 0,
};
static const uint16_t LBNSAMSUNG_15_BUF[] = {
    COSTOM2_CODE1_0, 0x8480,
    4500, 4470,
    550, 540,
    550, 1650,
    520, 22440, 0,
    0, 0, 34000, 2,
};
static const uint16_t LC7461M_C13S_16_BUF[] = {
    COSTOM2_CODE2, 0xDD88,
    9000, 4470,
    550, 540,
    550, 1650,
    560, 0, 23380,
    1, rpt_lc7461_9000_4500, 38000, 0,
};
static const uint16_t LC7461M_C13S_17_BUF[] = {
    COSTOM2_CODE2, 0xDD88,
    9000, 4500,
    550, 540,
    550, 1650,
    560, 0, 23380,
    0, 0, 38000, 0,
};
static const uint16_t LC7464M_PANASONIC_18_BUF[] = {
    COSTOM2_CODE2_Panasonic, 0xFF88,
    3360, 1700,
    425, 400,
    415, 1300,
    425, 60000, 14000,
    0, 0, 38000, 0,
};
static const uint16_t LG3004_33K_19_TOGGLE_ONE_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x1136,
    0, 0,
    165, 5560,
    165, 8420,
    165, 32270, 0,
    0, 0, 33000, 0,
};
static const uint16_t LG3004_33K_19_TOGGLE_TWO_BUF[] = {
    HIGH_REF_CUSTOM_CODE, 0x2036,
    0, 0,
    165, 5560,
    165, 8420,
    165, 32270, 0,
    0, 0, 33000, 0,
};
static const uint16_t LG3004_33K_19_TOGGLE_TWO_Freq_0_BUF[] = {
    HIGH_REF_CUSTOM_CODE, 0x2036,
    0, 0,
    30, 5560,
    30, 8420,
    30, 32270, 0,
    0, 0, 0, 0,
};
static const uint16_t LG3004_33K_19_TOGGLE_ZERO_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x2036,
    0, 0,
    165, 5560,
    165, 8420,
    165, 32270, 0,
    0, 0, 33000, 0,
};
static const uint16_t M3004_38K_20_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x1136,
    0, 0,
    130, 4870,
    130, 7420,
    130, 45560, 0,
    0, 0, 38000, 0,
};
static const uint16_t M3004_38K_20_toggle_zero_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x2036,
    0, 0,
    130, 4870,
    130, 7420,
    130, 45560, 0,
    0, 0, 38000, 0,
};
static const uint16_t M3004_38K_21_400_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x1136,
    0, 0,
    400, 4870,
    400, 7420,
    400, 45560, 0,
    0, 0, 38000, 0,
};
static const uint16_t M50119P_23_BUF[] = {
    COSTOM2_CODE1_0, 0x3070,
    0, 0,
    500, 470,
    500, 1470,
    500, 13500, 0,
    0, 0, 38000, 3,
};
static const uint16_t M50462_24_BUF[] = {
    COSTOM2_CODE1_0, 0x8080,
    0, 0,
    340, 670,
    340, 1670,
    340, 37000, 0,
    0, 0, 38000, 3,
};
static const uint16_t M50560_25_BUF[] = {
    COSTOM2_CODE1_0, 0x8080,
    8400, 4170,
    500, 480,
    500, 1500,
    530, 0, 24490,
    0, 0, 38000, 1,
};
static const uint16_t MITC8D8_28_BUF[] = {
    COSTOM2_CODE1_0, 0x8080,
    0, 0,
    315, 920,
    315, 2170,
    315, 0, 27120,
    0, 0, 34000, 2,
};
static const uint16_t MN6014C6D6_29_BUF[] = {
    COSTOM1_CODE1_COSTOM1_CODE1, 0x6666,
    3360, 3330,
    840, 810,
    840, 2500,
    841, 0, 31775,
    0, 0, 38000, 1,
};
static const uint16_t MN6014C5D638_30_BUF[] = {
    COSTOM1_CODE1_COSTOM1_CODE1, 0x5656,
    3400, 3370,
    870, 860,
    870, 2570,
    870, 0, 33800,
    0, 0, 38000, 0,
};
static const uint16_t MN6014C5D656_31_BUF[] = {
    COSTOM1_CODE1_COSTOM1_CODE1, 0x5656,
    3366, 3366,
    841, 841,
    841, 2523,
    841, 0, 33175,
    0, 0, 56000, 0,
};
static const uint16_t N3201_32_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    8600, 4370,
    555, 1130,
    555, 2920,
    555, 0, 60000,
    0, 0, 38000, 1,
};
static const uint16_t PANPROJECTOR_37_BUF[] = {
    COSTOM2_CODE2_Panasonic, 0xFF88,
    3360, 1690,
    425, 395,
    421, 1290,
    425, 60000, 14000,
    0, 0, 38000, 2,
};
static const uint16_t PHILIPS_20_38_BUF[] = {
    HIGH_PHILIPS_TOGGLE_CODE, 0x4188,
    2670, 869,
    444, 0,
    444, 1,
    0, 15050, 60000,
    0, 0, 36000, 0,
};
static const uint16_t PHILIPS_RC_MM_39_BUF[] = {
    PHILIPS_RC_MM, 0x8440,
    516, 260,
    166, 444,
    611, 778,
    166, 32600, 60000,
    0, 0, 36000, 0,
};
static const uint16_t PILOT370_40_BUF[] = {
    PHILIPS_STOP_CODE, 0x7160,
    510, 2500,
    510, 0,
    510, 1,
    0, 24500, 0,
    1, rpt_GEMINI_8500, 31400, 0,
};
static const uint16_t PIONEER_41_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    8550, 4270,
    530, 500,
    530, 1570,
    530, 0, 37000,
    0, 0, 40000, 1,
};
static const uint16_t RCA_56_42_BUF[] = {
    HIGH_COSTOM2_CODE2, 0x4848,
    4000, 3970,
    500, 970,
    500, 1970,
    500, 9470, 0,
    0, 0, 56800, 2,
};
static const uint16_t RCA_38_43_BUF[] = {
    HIGH_COSTOM2_CODE2, 0x4848,
    4000, 3970,
    500, 970,
    500, 1970,
    500, 9470, 0,
    0, 0, 38000, 0,
};
static const uint16_t RCA_56_44_BUF[] = {
    HIGH_COSTOM2_CODE2, 0x4848,
    20000, 4000,
    500, 1000,
    500, 2000,
    500, 8000, 0,
    0, 0, 56800, 0,
};
static const uint16_t SAA3010_38_45_BUF[] = {
    HIGH_START_PHIL_COSTOM2_CODE2, 0x1156,
    0, 0,
    830, 1,
    830, 0,
    0, 60000, 24490,
    0, 0, 38000, 0,
};
static const uint16_t SAA3010_36_46_BUF[] = {
    HIGH_START_PHIL_COSTOM2_CODE2, 0x1156,
    0, 0,
    870, 1,
    888, 0,
    0, 0, 23745,
    0, 0, 36000, 1,
};
static const uint16_t SAMSUNG_38_49_BUF[] = {
    COSTOM2_CODE2, 0x888C,
    4500, 4500,
    560, 540,
    560, 1660,
    560, 0, 42400,
    0, 0, 38000, 0,
};
static const uint16_t LX0773CE_50_BUF[] = {
    COSTOM1_CODE1_COSTOM1_CODE1, 0x5858,
    0, 0,
    255, 756,
    255, 1806,
    255, 46190, 0,
    0, 0, 38000, 0,
};
static const uint16_t SINGH2150_51_BUF[] = {
    PHILIPS_STOP_CODE, 0xF188,
    490, 2500,
    490, 0,
    490, 1,
    0, 20000, 0,
    1, rpt_SINGH2150_8170, 38000, 0,
};
static const uint16_t TC901_54_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    4500, 4470,
    550, 540,
    550, 1660,
    540, 0, 45500,
    1, rpt_TC9012LIAN_9700, 38000, 0,
};
static const uint16_t TC901_55_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    4500, 4470,
    550, 540,
    550, 1660,
    540, 0, 45500,
    0, 0, 38000, 0,
};
static const uint16_t RCT311_56_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x4107,
    0, 0,
    500, 1970,
    500, 4470,
    500, 0, 34500,
    0, 0, 33000, 1,
};
static const uint16_t UPD6121G2_64_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    9000, 4470,
    555, 540,
    555, 1660,
    560, 0, 40000,
    1, rpt_UPD612G2_9000, 38000, 0,
};
static const uint16_t UPD6121G2_40K_65_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    9000, 4500,
    527, 502,
    527, 1566,
    560, 0, 40000,
    1, rpt_UPD612G2_9000, 40000, 0,
};
static const uint16_t UPD6121G_LIAN_66_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    9000, 4470,
    555, 540,
    555, 1660,
    560, 0, 40000,
    0, 0, 38000, 0,
};
static const uint16_t UPD6121G_001_67_BUF[] = {
    CODE2_COSTOM2, 0x8888,
    9000, 4470,
    555, 540,
    555, 1660,
    560, 0, 40000,
    1, rpt_UPD612G2_9000, 38000, 0,
};
static const uint16_t UPD6121G2_D8_68_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    9000, 4470,
    555, 540,
    555, 1660,
    560, 0, 40000,
    1, rpt_UPD612G2_9000, 38000, 0,
};
static const uint16_t UPD6124_D7C5_69_BUF[] = {
    CODE1_COSTOM1, 0x7500,
    2400, 570,
    600, 570,
    1200, 570,
    0, 0, 27000,
    0, 0, 40000, 2,
};
static const uint16_t UPD6124_D7C8_70_BUF[] = {
    CODE1_COSTOM1, 0x7800,
    2400, 600,
    600, 600,
    1200, 600,
    0, 0, 27000,
    0, 0, 40000, 2,
};
static const uint16_t UPD6124_D7C13_71_BUF[] = {
    CODE1_COSTOM1, 0x7D00,
    2400, 570,
    600, 570,
    1200, 570,
    0, 0, 15000,
    0, 0, 40000, 2,
};
static const uint16_t VICTOR_C8D8_72_BUF[] = {
    COSTOM2_CODE1_0, 0x8080,
    8420, 4170,
    520, 490,
    520, 1530,
    526, 0, 21570,
    0, 0, 38000, 1,
};
static const uint16_t XY_2106_75_BUF[] = {
    COSTOM2_CODE2, 0x8088,
    8500, 4050,
    536, 536,
    536, 1500,
    536, 0, 25500,
    0, 0, 40000, 0,
};
static const uint16_t ZENITH_S10_77_BUF[] = {
    REPEAT_BIT_CODE, 0x8020,
    550, 550,
    370, 550,
    550, 550,
    560, 0, 550,
    0, 0, 38000, 0,
};
static const uint16_t ZENITH_S10_77_Zero_BUF[] = {
    REPEAT_BIT_CODE, 0x8020,
    550, 550,
    370, 550,
    550, 550,
    560, 0, 550,
    0, 0, 38000, 0,
};
static const uint16_t ZENITH_D5_78_BUF[] = {
    Zenith_D5, 0x0050,
    500, 500,
    500, 500,
    4100, 5100,
    0, 60000, 61000,
    0, 0, 40000, 0,
};
static const uint16_t ZENITH_D7_79_BUF[] = {
    Zenith_D5, 0x0070,
    500, 500,
    500, 500,
    4100, 5100,
    0, 60000, 61000,
    0, 0, 40000, 0,
};
static const uint16_t ZENITHSIM_C2D8_82_BUF[] = {
    HIGH_TOGGLE_COSTOM2_CODE2, 0x0118,
    20, 3720,
    20, 4950,
    20, 7450,
    20, 50100, 0,
    0, 0, 0, 0,
};
static const uint16_t Unknow_C8D5_83_BUF[] = {
    HIGH_COSTOM2_CODE2, 0x8585,
    253, 720,
    253, 720,
    253, 220,
    253, 20000, 0,
    0, 0, 38000, 1,
};
static const uint16_t UPD6121G_001_APPLE_90_BUF[] = {
    COSTOM2_CODE1_0, 0xF880,
    9000, 4470,
    555, 530,
    555, 1660,
    560, 0, 40000,
    1, rpt_UPD612G2_9000, 38000, 0,
};
static const uint16_t Pioneer_91_BUF[] = {
    COSTOM2_CODE2, 0x8888,
    8500, 4170,
    535, 500,
    535, 1550,
    535, 0, 25000,
    0, 0, 38000, 1,
};
static const uint16_t STM708LB1_92_BUF[] = {
    NOPWM_IRT1250C5D7, 0x5061,
    200, 6000,
    200, 1500,
    200, 3000,
    300, 0, 29000,
    0, 0, 38000, 0,
};
static const uint16_t JX_9070_93_BUF[] = {
    COSTOM2_CODE2, 0x80F0,
    9000, 4450,
    555, 530,
    555, 1660,
    560, 0, 28400,
    0, 0, 38000, 0,
};
static const uint16_t GD_2000_94_BUF[] = {
    CODE1_COSTOM1_CODE1, 0x8440,
    9000, 4470,
    500, 2500,
    500, 4500,
    560, 0, 30000,
    1, rpt_UPD612G2_9000, 38000, 1,
};
static const uint16_t XY_JAECS_95_BUF[] = {
    COSTOM1_0_CODE1, 0x8080,
    2700, 2560,
    1200, 1200,
    1200, 2900,
    1200, 0, 57760,
    0, 0, 38000, 0,
};
static const uint16_t DVB_40BIT_100_BUF[] = {
    COSTOM2_CODE1_0, 0xf8f0,
    3600, 1800,
    560, 560,
    560, 1120,
    560, 0, 53800,
    1, rpt_DVB_40BIT_100, 38000, 0,
};

uint16_t *get_ir_rpt(rpt_code_t rpt)
{
    uint16_t *ir_rpt = (uint16_t *)rpt_udp6121_9000_BUF;

    switch (rpt) {
    case rpt_lc7461_9000_4500:
        ir_rpt = (uint16_t *)rpt_lc7461_9000_4500_BUF;
        break;
    case rpt_UPD612G2_9000:
        ir_rpt = (uint16_t *)rpt_udp6121_9000_BUF;
        break;
    case rpt_UDP6121_8550:
        ir_rpt = (uint16_t *)rpt_udp6121_8550_BUF;
        break;
    case rpt_GEMINI_8500:
        ir_rpt = (uint16_t *)rpt_GEMINI_8500_BUF;
        break;
    case rpt_SINGH2150_8170:
        ir_rpt = (uint16_t *)rpt_SINGH2150_8170_BUF;
        break;
    case rpt_TC9012LIAN_9700:
        ir_rpt = (uint16_t *)rpt_TC9012LIAN_9700_BUF;
        break;
    case rpt_DVB_40BIT_100:
        ir_rpt = (uint16_t *)rpt_DVB_40BIT_BUF;
        break;
	
    default:
        break;
    }

    return ir_rpt;
}

uint16_t * get_ir_customer(customer_code_t customer)
{
    uint16_t * ir_customer = (uint16_t *)CUSTOM_DC_00_BUF;

    switch (customer) {
    case CUSTOM_00_00_A:
        ir_customer = (uint16_t *)CUSTOM_00_00_BUF;
        break;
    case CUSTOM_00_1F_A:
        ir_customer = (uint16_t *)CUSTOM_00_1F_BUF;
        break;
    case CUSTOM_00_02_A:
        ir_customer = (uint16_t *)CUSTOM_00_02_BUF;
        break;
    case CUSTOM_00_2B_A:
        ir_customer = (uint16_t *)CUSTOM_00_2B_BUF;
        break;
    case CUSTOM_00_30_A:
        ir_customer = (uint16_t *)CUSTOM_00_30_BUF;
        break;
    case CUSTOM_00_57_A:
        ir_customer = (uint16_t *)CUSTOM_00_57_BUF;
        break;
    case CUSTOM_00_7F_A:
        ir_customer = (uint16_t *)CUSTOM_00_7F_BUF;
        break;
    case CUSTOM_00_80_A:
        ir_customer = (uint16_t *)CUSTOM_00_80_BUF;
        break;
    case CUSTOM_00_99_A:
        ir_customer = (uint16_t *)CUSTOM_00_99_BUF;
        break;
    case CUSTOM_00_AF_A:
        ir_customer = (uint16_t *)CUSTOM_00_AF_BUF;
        break;
    case CUSTOM_00_BA_A:
        ir_customer = (uint16_t *)CUSTOM_00_BA_BUF;
        break;        
    case CUSTOM_00_BD_A:
        ir_customer = (uint16_t *)CUSTOM_00_BD_BUF;
        break;
    case CUSTOM_00_BE_A:
        ir_customer = (uint16_t *)CUSTOM_00_BE_BUF;
        break;
    case CUSTOM_00_BF_A:
        ir_customer = (uint16_t *)CUSTOM_00_BF_BUF;
        break;
    case CUSTOM_00_DF_A:
        ir_customer = (uint16_t *)CUSTOM_00_DF_BUF;
        break;
    case CUSTOM_00_D9_A:
        ir_customer = (uint16_t *)CUSTOM_00_D9_BUF;
        break;
    case CUSTOM_00_EF_A:
        ir_customer = (uint16_t *)CUSTOM_00_EF_BUF;
        break;
    case CUSTOM_00_F6_A:
        ir_customer = (uint16_t *)CUSTOM_00_F6_BUF;
        break;         
    case CUSTOM_00_F7_A:
        ir_customer = (uint16_t *)CUSTOM_00_F7_BUF;
        break;
    case CUSTOM_00_FB_A:
        ir_customer = (uint16_t *)CUSTOM_00_FB_BUF;
        break;
    case CUSTOM_00_FD_A:
        ir_customer = (uint16_t *)CUSTOM_00_FD_BUF;
        break;
    case CUSTOM_00_FF_A:
        ir_customer = (uint16_t *)CUSTOM_00_FF_BUF;
        break;
    case CUSTOM_01_00_A:
        ir_customer = (uint16_t *)CUSTOM_01_00_BUF;
        break;
    case CUSTOM_01_01_A:
        ir_customer = (uint16_t *)CUSTOM_01_01_BUF;
        break;
    case CUSTOM_01_FE_A:
        ir_customer = (uint16_t *)CUSTOM_01_FE_BUF;
        break;
    case CUSTOM_02_00_A:
        ir_customer = (uint16_t *)CUSTOM_02_00_BUF;
        break;
    case CUSTOM_02_1D_A:
        ir_customer = (uint16_t *)CUSTOM_02_1D_BUF;
        break;        
    case CUSTOM_02_BD_A:
        ir_customer = (uint16_t *)CUSTOM_02_BD_BUF;
        break;
    case CUSTOM_02_F5_A:
        ir_customer = (uint16_t *)CUSTOM_02_F5_BUF;
        break;
    case CUSTOM_02_FD_A:
        ir_customer = (uint16_t *)CUSTOM_02_FD_BUF;
        break;
    case CUSTOM_02_7D_A:
        ir_customer = (uint16_t *)CUSTOM_02_7D_BUF;
        break;        
    case CUSTOM_02_7F_A:
        ir_customer = (uint16_t *)CUSTOM_02_7F_BUF;
        break;
    case CUSTOM_02_FF_A:
        ir_customer = (uint16_t *)CUSTOM_02_FF_BUF;
        break;              
    case CUSTOM_03_00_A:
        ir_customer = (uint16_t *)CUSTOM_03_00_BUF;
        break;
    case CUSTOM_03_AC_A:
        ir_customer = (uint16_t *)CUSTOM_03_AC_BUF;
        break;
    case CUSTOM_03_FC_A:
        ir_customer = (uint16_t *)CUSTOM_03_FC_BUF;
        break;
    case CUSTOM_04_00_A:
        ir_customer = (uint16_t *)CUSTOM_04_00_BUF;
        break;
    case CUSTOM_04_10_A:
        ir_customer = (uint16_t *)CUSTOM_04_10_BUF;
        break;
    case CUSTOM_04_B9_A:
        ir_customer = (uint16_t *)CUSTOM_04_B9_BUF;
        break;
    case CUSTOM_04_FB_A:
        ir_customer = (uint16_t *)CUSTOM_04_FB_BUF;
        break;
    case CUSTOM_05_00_A:
        ir_customer = (uint16_t *)CUSTOM_05_00_BUF;
        break;
    case CUSTOM_05_05_A:
        ir_customer = (uint16_t *)CUSTOM_05_05_BUF;
        break;
    case CUSTOM_06_06_A:
        ir_customer = (uint16_t *)CUSTOM_06_06_BUF;
        break;
    case CUSTOM_06_FB_A:
        ir_customer = (uint16_t *)CUSTOM_06_FB_BUF;
        break;
    case CUSTOM_06_FE_A:
        ir_customer = (uint16_t *)CUSTOM_06_FE_BUF;
        break;        
    case CUSTOM_07_07_A:
        ir_customer = (uint16_t *)CUSTOM_07_07_BUF;
        break;
    case CUSTOM_08_00_A:
        ir_customer = (uint16_t *)CUSTOM_08_00_BUF;
        break;
    case CUSTOM_08_08_A:
        ir_customer = (uint16_t *)CUSTOM_08_08_BUF;
        break;
    case CUSTOM_08_0B_A:
        ir_customer = (uint16_t *)CUSTOM_08_0B_BUF;
        break;
    case CUSTOM_08_13_A:
        ir_customer = (uint16_t *)CUSTOM_08_13_BUF;
        break;
    case CUSTOM_08_F7_A:
        ir_customer = (uint16_t *)CUSTOM_08_F7_BUF;
        break;
    case CUSTOM_09_F6_A:
        ir_customer = (uint16_t *)CUSTOM_09_F6_BUF;
        break;        
    case CUSTOM_0A_35_A:
        ir_customer = (uint16_t *)CUSTOM_0A_35_BUF;
        break;
    case CUSTOM_0A_00_A:
        ir_customer = (uint16_t *)CUSTOM_0A_00_BUF;
        break;
    case CUSTOM_0A_0A_A:
        ir_customer = (uint16_t *)CUSTOM_0A_0A_BUF;
        break;
    case CUSTOM_0B_0B_A:
        ir_customer = (uint16_t *)CUSTOM_0B_0B_BUF;
        break;
    case CUSTOM_0D_0D_A:
        ir_customer = (uint16_t *)CUSTOM_0D_0D_BUF;
        break;
    case CUSTOM_0E_09_A:
        ir_customer = (uint16_t *)CUSTOM_0E_09_BUF;
        break;
    case CUSTOM_0E_0E_A:
        ir_customer = (uint16_t *)CUSTOM_0E_0E_BUF;
        break;
    case CUSTOM_0F_00_A:
        ir_customer = (uint16_t *)CUSTOM_0F_00_BUF;
        break;
    case CUSTOM_10_ED_A:
        ir_customer = (uint16_t *)CUSTOM_10_ED_BUF;
        break;
    case CUSTOM_10_EF_A:
        ir_customer = (uint16_t *)CUSTOM_10_EF_BUF;
        break;
    case CUSTOM_10_2D_A:
        ir_customer = (uint16_t *)CUSTOM_10_2D_BUF;
        break;
    case CUSTOM_12_12_A:
        ir_customer = (uint16_t *)CUSTOM_12_12_BUF;
        break;
    case CUSTOM_12_ED_A:
        ir_customer = (uint16_t *)CUSTOM_12_ED_BUF;
        break;
    case CUSTOM_13_14_A:
        ir_customer = (uint16_t *)CUSTOM_13_14_BUF;
        break;
    case CUSTOM_14_00_A:
        ir_customer = (uint16_t *)CUSTOM_14_00_BUF;
        break;
    case CUSTOM_16_29_A:
        ir_customer = (uint16_t *)CUSTOM_16_29_BUF;
        break;
    case CUSTOM_16_E9_A:
        ir_customer = (uint16_t *)CUSTOM_16_E9_BUF;
        break;
    case CUSTOM_18_18_A:
        ir_customer = (uint16_t *)CUSTOM_18_18_BUF;
        break;
    case CUSTOM_18_E7_A:
        ir_customer = (uint16_t *)CUSTOM_18_E7_BUF;
        break;
    case CUSTOM_1B_00_A:
        ir_customer = (uint16_t *)CUSTOM_1B_00_BUF;
        break;
    case CUSTOM_1C_E3_A:
        ir_customer = (uint16_t *)CUSTOM_1C_E3_BUF;
        break;
    case CUSTOM_1E_1E_A:
        ir_customer = (uint16_t *)CUSTOM_1E_1E_BUF;
        break;
    case CUSTOM_1E_E1_A:
        ir_customer = (uint16_t *)CUSTOM_1E_E1_BUF;
        break;
    case CUSTOM_1F_00_A:
        ir_customer = (uint16_t *)CUSTOM_1F_00_BUF;
        break;
    case CUSTOM_18_E9_A:
        ir_customer = (uint16_t *)CUSTOM_18_E9_BUF;
        break;
    case CUSTOM_1A_00_A:
        ir_customer = (uint16_t *)CUSTOM_1A_00_BUF;
        break;        
    case CUSTOM_20_40_A:
        ir_customer = (uint16_t *)CUSTOM_20_40_BUF;
        break;
    case CUSTOM_20_DF_A:
        ir_customer = (uint16_t *)CUSTOM_20_DF_BUF;
        break;
    case CUSTOM_20_FD_A:
        ir_customer = (uint16_t *)CUSTOM_20_FD_BUF;
        break;
    case CUSTOM_21_DE_A:
        ir_customer = (uint16_t *)CUSTOM_21_DE_BUF;
        break;
    case CUSTOM_22_11_A:
        ir_customer = (uint16_t *)CUSTOM_22_11_BUF;
        break;
    case CUSTOM_23_45_A:
        ir_customer = (uint16_t *)CUSTOM_23_45_BUF;
        break;          
    case CUSTOM_24_DB_A:
        ir_customer = (uint16_t *)CUSTOM_24_DB_BUF;
        break;
    case CUSTOM_29_D6_A:
        ir_customer = (uint16_t *)CUSTOM_29_D6_BUF;
        break;
    case CUSTOM_2B_D4_A:
        ir_customer = (uint16_t *)CUSTOM_2B_D4_BUF;
        break;
    case CUSTOM_33_00_A:
        ir_customer = (uint16_t *)CUSTOM_33_00_BUF;
        break;
    case CUSTOM_33_CC_A:
        ir_customer = (uint16_t *)CUSTOM_33_CC_BUF;
        break;
    case CUSTOM_35_50_A:
        ir_customer = (uint16_t *)CUSTOM_35_50_BUF;
        break;
    case CUSTOM_38_C7_A:
        ir_customer = (uint16_t *)CUSTOM_38_C7_BUF;
        break;
    case CUSTOM_3F_C0_A:
        ir_customer = (uint16_t *)CUSTOM_3F_C0_BUF;
        break;
    case CUSTOM_3F_3F_A:
        ir_customer = (uint16_t *)CUSTOM_3F_3F_BUF;
        break;        
    case CUSTOM_40_02_A:
        ir_customer = (uint16_t *)CUSTOM_40_02_BUF;
        break;
    case CUSTOM_40_20_A:
        ir_customer = (uint16_t *)CUSTOM_40_20_BUF;
        break;
    case CUSTOM_40_40_A:
        ir_customer = (uint16_t *)CUSTOM_40_40_BUF;
        break;
    case CUSTOM_40_BF_A:
        ir_customer = (uint16_t *)CUSTOM_40_BF_BUF;
        break;
    case CUSTOM_42_BD_A:
        ir_customer = (uint16_t *)CUSTOM_42_BD_BUF;
        break;
    case CUSTOM_42_FD_A:
        ir_customer = (uint16_t *)CUSTOM_42_FD_BUF;
        break;
    case CUSTOM_43_00_A:
        ir_customer = (uint16_t *)CUSTOM_43_00_BUF;
        break;
    case CUSTOM_45_16_A:
        ir_customer = (uint16_t *)CUSTOM_45_16_BUF;
        break;
      
    case CUSTOM_45_BC_A:
        ir_customer = (uint16_t *)CUSTOM_45_BC_BUF;
        break;
    case CUSTOM_46_00_A:
        ir_customer = (uint16_t *)CUSTOM_46_00_BUF;
        break;        
    case CUSTOM_47_00_A:
        ir_customer = (uint16_t *)CUSTOM_47_00_BUF;
        break;
    case CUSTOM_4B_B4_A:
        ir_customer = (uint16_t *)CUSTOM_4B_B4_BUF;
        break;
    case CUSTOM_4F_50_A:
        ir_customer = (uint16_t *)CUSTOM_4F_50_BUF;
        break;
    case CUSTOM_50_AD_A:
        ir_customer = (uint16_t *)CUSTOM_50_AD_BUF;
        break;
    case CUSTOM_50_AF_A:
        ir_customer = (uint16_t *)CUSTOM_50_AF_BUF;
        break;
    case CUSTOM_54_00_A:
        ir_customer = (uint16_t *)CUSTOM_54_00_BUF;
        break;
    case CUSTOM_58_A7_A:
        ir_customer = (uint16_t *)CUSTOM_58_A7_BUF;
        break;        
    case CUSTOM_5A_49_A:
        ir_customer = (uint16_t *)CUSTOM_5A_49_BUF;
        break;
    case CUSTOM_60_9F_A:
        ir_customer = (uint16_t *)CUSTOM_60_9F_BUF;
        break;
    case CUSTOM_60_9D_A:
        ir_customer = (uint16_t *)CUSTOM_60_9D_BUF;
        break;        
    case CUSTOM_61_63_A:
        ir_customer = (uint16_t *)CUSTOM_61_63_BUF;
        break;
    case CUSTOM_6A_00_A:
        ir_customer = (uint16_t *)CUSTOM_6A_00_BUF;
        break;        
    case CUSTOM_69_F1_A:
        ir_customer = (uint16_t *)CUSTOM_69_F1_BUF;
        break;
    case CUSTOM_71_8E_A:
        ir_customer = (uint16_t *)CUSTOM_71_8E_BUF;
        break;
    case CUSTOM_73_00_A:
        ir_customer = (uint16_t *)CUSTOM_73_00_BUF;
        break;
    case CUSTOM_76_7B_A:
        ir_customer = (uint16_t *)CUSTOM_76_7B_BUF;
        break;        
    case CUSTOM_77_00_A:
        ir_customer = (uint16_t *)CUSTOM_77_00_BUF;
        break;
    case CUSTOM_77_88_A:
        ir_customer = (uint16_t *)CUSTOM_77_88_BUF;
        break;
    case CUSTOM_7D_00_A:
        ir_customer = (uint16_t *)CUSTOM_7D_00_BUF;
        break;
    case CUSTOM_7F_00_A:
        ir_customer = (uint16_t *)CUSTOM_7F_00_BUF;
        break;
    case CUSTOM_80_23_A:
        ir_customer = (uint16_t *)CUSTOM_80_23_BUF;
        break;
    case CUSTOM_80_80_A:
        ir_customer = (uint16_t *)CUSTOM_80_80_BUF;
        break;
    case CUSTOM_80_57_A:
        ir_customer = (uint16_t *)CUSTOM_80_57_BUF;
        break;
    case CUSTOM_80_63_A:
        ir_customer = (uint16_t *)CUSTOM_80_63_BUF;
        break;
    case CUSTOM_80_64_A:
        ir_customer = (uint16_t *)CUSTOM_80_64_BUF;
        break;
    case CUSTOM_80_6F_A:
        ir_customer = (uint16_t *)CUSTOM_80_6F_BUF;
        break;
    case CUSTOM_80_7F_A:
        ir_customer = (uint16_t *)CUSTOM_80_7F_BUF;
        break;
    case CUSTOM_80_8F_A:
        ir_customer = (uint16_t *)CUSTOM_80_8F_BUF;
        break;
    case CUSTOM_82_F3_A:
        ir_customer = (uint16_t *)CUSTOM_82_F3_BUF;
        break;
    case CUSTOM_83_55_A:
        ir_customer = (uint16_t *)CUSTOM_83_55_BUF;
        break;
    case CUSTOM_83_F1_A:
        ir_customer = (uint16_t *)CUSTOM_83_F1_BUF;
        break;
    case CUSTOM_83_F7_A:
        ir_customer = (uint16_t *)CUSTOM_83_F7_BUF;
        break;
    case CUSTOM_84_E0_A:
        ir_customer = (uint16_t *)CUSTOM_84_E0_BUF;
        break;
    case CUSTOM_85_46_A:
        ir_customer = (uint16_t *)CUSTOM_85_46_BUF;
        break;
    case CUSTOM_86_02_A:
        ir_customer = (uint16_t *)CUSTOM_86_02_BUF;
        break;
    case CUSTOM_86_05_A:
        ir_customer = (uint16_t *)CUSTOM_86_05_BUF;
        break;
    case CUSTOM_86_6B_A:
        ir_customer = (uint16_t *)CUSTOM_86_6B_BUF;
        break;
    case CUSTOM_87_22_A:
        ir_customer = (uint16_t *)CUSTOM_87_22_BUF;
        break;
    case CUSTOM_87_4E_A:
        ir_customer = (uint16_t *)CUSTOM_87_4E_BUF;
        break;
    case CUSTOM_88_45_A:
        ir_customer = (uint16_t *)CUSTOM_88_45_BUF;
        break;
    case CUSTOM_95_6A_A:
        ir_customer = (uint16_t *)CUSTOM_95_6A_BUF;
        break;
    case CUSTOM_96_69_A:
        ir_customer = (uint16_t *)CUSTOM_96_69_BUF;
        break;
    case CUSTOM_97_00_A:
        ir_customer = (uint16_t *)CUSTOM_97_00_BUF;
        break;
    case CUSTOM_A0_5F_A:
        ir_customer = (uint16_t *)CUSTOM_A0_5F_BUF;
        break;
    case CUSTOM_A0_87_A:
        ir_customer = (uint16_t *)CUSTOM_A0_87_BUF;
        break;        
    case CUSTOM_A4_00_A:
        ir_customer = (uint16_t *)CUSTOM_A4_00_BUF;
        break;
    case CUSTOM_AA_03_A:
        ir_customer = (uint16_t *)CUSTOM_AA_03_BUF;
        break;
    case CUSTOM_AA_55_A:
        ir_customer = (uint16_t *)CUSTOM_AA_55_BUF;
        break;
    case CUSTOM_C4_00_A:
        ir_customer = (uint16_t *)CUSTOM_C4_00_BUF;
        break;        
    case CUSTOM_E5_70_A:
        ir_customer = (uint16_t *)CUSTOM_E5_70_BUF;
        break;
    case CUSTOM_E6_00_A:
        ir_customer = (uint16_t *)CUSTOM_E6_00_BUF;
        break;
    case CUSTOM_E7_0A_A:
        ir_customer = (uint16_t *)CUSTOM_E7_0A_BUF;
        break;
    case CUSTOM_EA_C7_A:
        ir_customer = (uint16_t *)CUSTOM_EA_C7_BUF;
        break;
    case CUSTOM_EA_C8_A:
        ir_customer = (uint16_t *)CUSTOM_EA_C8_BUF;
        break;
    case CUSTOM_EE_00_A:
        ir_customer = (uint16_t *)CUSTOM_EE_00_BUF;
        break;
    case CUSTOM_DC_00_A:
        ir_customer = (uint16_t *)CUSTOM_DC_00_BUF;
        break;
    case CUSTOM_DE_D6_A:
        ir_customer = (uint16_t *)CUSTOM_DE_D6_BUF;
        break;
    case CUSTOM_F4_00_A:
        ir_customer = (uint16_t *)CUSTOM_F4_00_BUF;
        break;
    case CUSTOM_FE_01_A:
        ir_customer = (uint16_t *)CUSTOM_FE_01_BUF;
        break;        

    case CUSTOM_0000_B000_A:
        ir_customer = (uint16_t *)CUSTOM_0000_B000_BUF;
        break;
    case CUSTOM_0000_FFFF_A:
        ir_customer = (uint16_t *)CUSTOM_0000_FFFF_BUF;
        break;
    case CUSTOM_055A_0000_A:
        ir_customer = (uint16_t *)CUSTOM_055A_0000_BUF;
        break;
    case CUSTOM_0057_0000_A:
        ir_customer = (uint16_t *)CUSTOM_0057_0000_BUF;
        break;
    case CUSTOM_007B_1F84_A:
        ir_customer = (uint16_t *)CUSTOM_007B_1F84_BUF;
        break;
    case CUSTOM_081F_0000_A:
        ir_customer = (uint16_t *)CUSTOM_081F_0000_BUF;
        break;
    case CUSTOM_0119_1EE6_A:
        ir_customer = (uint16_t *)CUSTOM_0119_1EE6_BUF;
        break;
    case CUSTOM_011A_FEE5_A:
        ir_customer = (uint16_t *)CUSTOM_011A_FEE5_BUF;
        break;
    case CUSTOM_011C_1EE3_A:
        ir_customer = (uint16_t *)CUSTOM_011C_1EE3_BUF;
        break;
    case CUSTOM_013C_1EC3_A:
        ir_customer = (uint16_t *)CUSTOM_013C_1EC3_BUF;
        break;
    case CUSTOM_0114_FEEB_A:
        ir_customer = (uint16_t *)CUSTOM_0114_FEEB_BUF;
        break;
    case CUSTOM_0200_0000_A:
        ir_customer = (uint16_t *)CUSTOM_0200_0000_BUF;
        break;
    case CUSTOM_1463_0084_A:
        ir_customer = (uint16_t *)CUSTOM_1463_0084_BUF;
        break;
    case CUSTOM_2002_0080_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_2002_0080_BUF;
        break;
    case CUSTOM_2002_0280_A:
        ir_customer = (uint16_t *)CUSTOM_2002_0280_BUF;
        break;
    case CUSTOM_2002_0480_A:
        ir_customer = (uint16_t *)CUSTOM_2002_0480_BUF;
        break;
    case CUSTOM_2002_4880_A:
        ir_customer = (uint16_t *)CUSTOM_2002_4880_BUF;
        break;
    case CUSTOM_29C0_2600_A:
        ir_customer = (uint16_t *)CUSTOM_29C0_2600_BUF;
        break;
    case CUSTOM_5AAA_162F_A:
        ir_customer = (uint16_t *)CUSTOM_5AAA_162F_BUF;
        break;
    case CUSTOM_8000_FFFF_A:
        ir_customer = (uint16_t *)CUSTOM_8000_FFFF_BUF;
        break;
    case CUSTOM_87E1_F000_A:
        ir_customer = (uint16_t *)CUSTOM_87E1_F000_BUF;
        break;
    case CUSTOM_2002_0090_A:
        ir_customer = (uint16_t *)CUSTOM_2002_0090_BUF;
        break;
    case CUSTOM_BFFF_FFFF_A:
        ir_customer = (uint16_t *)CUSTOM_BFFF_FFFF_BUF;
        break;
    case CUSTOM_434F_004E_A:
        ir_customer = (uint16_t *)CUSTOM_434F_004E_BUF;
        break;
    case CUSTOM_FFFE_00A4_A:
        ir_customer = (uint16_t *)CUSTOM_FFFE_00A4_BUF;
        break;
    case CUSTOM_FFFE_00A5_A:
        ir_customer = (uint16_t *)CUSTOM_FFFE_00A5_BUF;
        break;
    case CUSTOM_FFFE_006A_A:
        ir_customer = (uint16_t *)CUSTOM_FFFE_006A_BUF;
        break;
    case CUSTOM_FFFD_0000_A:
        ir_customer = (uint16_t *)CUSTOM_FFFD_0000_BUF;
        break;
    case CUSTOM_FFFD_0001_A:
        ir_customer = (uint16_t *)CUSTOM_FFFD_0001_BUF;
        break;

    case CUSTOM_FFFD_FFFF_A:
        ir_customer = (uint16_t *)CUSTOM_FFFD_FFFF_BUF;
        break;
    case CUSTOM_FFFF_FFFF_A:
        ir_customer = (uint16_t *)CUSTOM_FFFF_FFFF_BUF;
        break;

    case HIGH_CUSTOM_0000_1800_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_0000_1800_BUF;
        break;
    case HIGH_CUSTOM_3000_0000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_3000_0000_BUF;
        break;
    case HIGH_CUSTOM_4000_0000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_4000_0000_BUF;
        break;
    case HIGH_CUSTOM_8000_0000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_8000_0000_BUF;
        break;
    case HIGH_CUSTOM_8000_2800_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_8000_2800_BUF;
        break;
    case HIGH_CUSTOM_8100_7E00_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_8100_7E00_BUF;
        break;
    case HIGH_CUSTOM_A000_0000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_A000_0000_BUF;
        break;
    case HIGH_CUSTOM_B000_6000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_B000_6000_BUF;
        break;
    case HIGH_CUSTOM_B000_A000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_B000_A000_BUF;
        break;        
    case HIGH_CUSTOM_B000_F000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_B000_F000_BUF;
        break;
    case HIGH_CUSTOM_C000_0000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_C000_0000_BUF;
        break;
    case HIGH_CUSTOM_C000_0800_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_C000_0800_BUF;
        break;
    case HIGH_CUSTOM_C000_6000_0C_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_C000_6000_BUF;
        break;
    case HIGH_CUSTOM_FFFD_6000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_FFFD_6000_BUF;
        break;
    case HIGH_CUSTOM_FFFD_E000_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_FFFD_E000_BUF;
        break;
    case HIGH_CUSTOM_FFFD_D000_3010_1_1A_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_FFFD_D000_3010_1_1A_A_BUF;
        break;        
    case HIGH_CUSTOM_FFFD_8800_A:
        ir_customer = (uint16_t *)HIGH_CUSTOM_FFFD_8800_BUF;
        break;
    default:
        break;
    }

    return ir_customer;
}

uint16_t *get_ir_brand(ir_type_t type)
{
    uint16_t *ir_brand = (uint16_t *)UPD6121G2_D8_68_BUF;

    switch (type)
    {
    case CUSTOM_6BIT_02:
        ir_brand = (uint16_t *)CUSTOM_6BIT_02_BUF;
        break;
    case CUSTOM_C1D6_03:
        ir_brand = (uint16_t *)CUSTOM_C1D6_03_BUF;
        break;
    case DVB_PAN_7051_SAMSUN_05:
        ir_brand = (uint16_t *)DVB_PAN_7051_SAMSUN_05_BUF;
        break;
    case FB739A_06:
        ir_brand = (uint16_t *)FB739A_06_BUF;
        break;
    case GEMINI_C16_38_08:
        ir_brand = (uint16_t *)GEMINI_C16_38_08_BUF;
        break;
    case GEMINI_C10_32_08:
        ir_brand = (uint16_t *)GEMINI_C10_32_08_BUF;
        break;
    case GEMINI_C10_38_09:
        ir_brand = (uint16_t *)GEMINI_C10_38_09_BUF;
        break;
    case GEMINI_C10_32_10:
        ir_brand = (uint16_t *)GEMINI_C10_32_10_BUF;
        break;
    case IRT1250C5D6_12:
        ir_brand = (uint16_t *)IRT1250C5D6_12_BUF;
        break;
    case IRT1250C5D7_13:
        ir_brand = (uint16_t *)IRT1250C5D7_13_BUF;
        break;
    case KK_Y261_14:
        ir_brand = (uint16_t *)KK_Y261_14_BUF;
        break;
    case LBNSAMSUNG_15:
        ir_brand = (uint16_t *)LBNSAMSUNG_15_BUF;
        break;
    case LC7461M_C13S_16:
        ir_brand = (uint16_t *)LC7461M_C13S_16_BUF;
        break;
    case LC7461M_C13S_17:
        ir_brand = (uint16_t *)LC7461M_C13S_17_BUF;
        break;
    case LC7464M_PANASONIC_18:
        ir_brand = (uint16_t *)LC7464M_PANASONIC_18_BUF;
        break;
    case LG3004_33K_19_Toggle_one:
        ir_brand = (uint16_t *)LG3004_33K_19_TOGGLE_ONE_BUF;
        break;
    case LG3004_33K_19_Toggle_two:
        ir_brand = (uint16_t *)LG3004_33K_19_TOGGLE_TWO_BUF;
        break;
    case LG3004_33K_19_Toggle_zero:
        ir_brand = (uint16_t *)LG3004_33K_19_TOGGLE_ZERO_BUF;
        break;
    case LG3004_33K_19_TOGGLE_TWO_Freq_0:
        ir_brand = (uint16_t *)LG3004_33K_19_TOGGLE_TWO_Freq_0_BUF;
        break;
    case M3004_38K_20:
        ir_brand = (uint16_t *)M3004_38K_20_BUF;
        break;
    case M3004_38K_20_toggle_zero:
        ir_brand = (uint16_t *)M3004_38K_20_toggle_zero_BUF;
        break;
    case M3004_38K_400_21:
        ir_brand = (uint16_t *)M3004_38K_21_400_BUF;
        break;
    case M50119P_23:
        ir_brand = (uint16_t *)M50119P_23_BUF;
        break;
    case M50462_24:
        ir_brand = (uint16_t *)M50462_24_BUF;
        break;
    case M50560_25:
        ir_brand = (uint16_t *)M50560_25_BUF;
        break;
    case MITC8D8_28:
        ir_brand = (uint16_t *)MITC8D8_28_BUF;
        break;
    case MN6014C6D6_29:
        ir_brand = (uint16_t *)MN6014C6D6_29_BUF;
        break;
    case MN6014C5D638_30:
        ir_brand = (uint16_t *)MN6014C5D638_30_BUF;
        break;
    case MN6014C5D656_31:
        ir_brand = (uint16_t *)MN6014C5D656_31_BUF;
        break;
    case N3201_32:
        ir_brand = (uint16_t *)N3201_32_BUF;
        break;
    case PANPROJECTOR_37:
        ir_brand = (uint16_t *)PANPROJECTOR_37_BUF;
        break;
    case PHILIPS_20_38:
        ir_brand = (uint16_t *)PHILIPS_20_38_BUF;
        break;
    case PHILIPS_RC_MM_39:
        ir_brand = (uint16_t *)PHILIPS_RC_MM_39_BUF;
        break;
    case PILOT370_40:
        ir_brand = (uint16_t *)PILOT370_40_BUF;
        break;
    case PIONEER_41:
        ir_brand = (uint16_t *)PIONEER_41_BUF;
        break;
    case RCA_56_42:
        ir_brand = (uint16_t *)RCA_56_42_BUF;
        break;
    case RCA_38_43:
        ir_brand = (uint16_t *)RCA_38_43_BUF;
        break;
    case RCA_56_44:
        ir_brand = (uint16_t *)RCA_56_44_BUF;
        break;
    case SAA3010_38_45:
        ir_brand = (uint16_t *)SAA3010_38_45_BUF;
        break;
    case SAA3010_36_46:
        ir_brand = (uint16_t *)SAA3010_36_46_BUF;
        break;
    case SAMSUNG_49:
        ir_brand = (uint16_t *)SAMSUNG_38_49_BUF;
        break;
    case LX0773CE_50:
        ir_brand = (uint16_t *)LX0773CE_50_BUF;
        break;
    case SINGH2150_51:
        ir_brand = (uint16_t *)SINGH2150_51_BUF;
        break;
    case TC901_54:
        ir_brand = (uint16_t *)TC901_54_BUF;
        break;
    case TC901_D8B8_55:
        ir_brand = (uint16_t *)TC901_55_BUF;
        break;
    case RCT311_56:
        ir_brand = (uint16_t *)RCT311_56_BUF;
        break;
    case UPD6121G_64:
        ir_brand = (uint16_t *)UPD6121G2_64_BUF;
        break;
    case UPD6121G_40K_65:
        ir_brand = (uint16_t *)UPD6121G2_40K_65_BUF;
        break;
    case UPD6121F_LIAN_66:
        ir_brand = (uint16_t *)UPD6121G_LIAN_66_BUF;
        break;
    case UPD6121G_001_67:
        ir_brand = (uint16_t *)UPD6121G_001_67_BUF;
        break;
    case UPD6121G2_68:
        ir_brand = (uint16_t *)UPD6121G2_D8_68_BUF;
        break;
    case UPD6124_D7C5_69:
        ir_brand = (uint16_t *)UPD6124_D7C5_69_BUF;
        break;
    case UPD6124_D7C8_70:
        ir_brand = (uint16_t *)UPD6124_D7C8_70_BUF;
        break;
    case UPD6124_D7C13_71:
        ir_brand = (uint16_t *)UPD6124_D7C13_71_BUF;
        break;
    case VICTOR_C8D8_72:
        ir_brand = (uint16_t *)VICTOR_C8D8_72_BUF;
        break;
    case XY_2106_75:
        ir_brand = (uint16_t *)XY_2106_75_BUF;
        break;
    case ZENITH_S10_77:
        ir_brand = (uint16_t *)ZENITH_S10_77_BUF;
        break;
    case ZENITH_S10_77_Zero:
        ir_brand = (uint16_t *)ZENITH_S10_77_Zero_BUF;
        break;
    case ZENITH_D5_78:
        ir_brand = (uint16_t *)ZENITH_D5_78_BUF;
        break;
    case ZENITH_D7_79:
        ir_brand = (uint16_t *)ZENITH_D7_79_BUF;
        break;
    case ZENITHSIM_C2D8_82:
        ir_brand = (uint16_t *)ZENITHSIM_C2D8_82_BUF;
        break;
    case Unknow_C8D5_83:
        ir_brand = (uint16_t *)Unknow_C8D5_83_BUF;
        break;
    case UPD6121G_001_APPLE_90:
        ir_brand = (uint16_t *)UPD6121G_001_APPLE_90_BUF;
        break;
    case Pioneer_91:
        ir_brand = (uint16_t *)Pioneer_91_BUF;
        break;
    case STM708LB1_92:
        ir_brand = (uint16_t *)STM708LB1_92_BUF;
        break;
    case JX_9070_93:
        ir_brand = (uint16_t *)JX_9070_93_BUF;
        break;
    case GD_2000_94:
        ir_brand = (uint16_t *)GD_2000_94_BUF;
        break;
    case XY_JAECS_95:
        ir_brand = (uint16_t *)XY_JAECS_95_BUF;
        break;        
    case DVB_40BIT_100:
        ir_brand = (uint16_t *)DVB_40BIT_100_BUF;
        break;
	
    default:
        break;
    }

    return ir_brand;
}
