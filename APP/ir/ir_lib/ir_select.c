#include "ir_select.h"
#include "ir_lib.h"
#include "ir_load.h"
#include "app_config.h"


static const uint8_t KEY_BUF_00_0B[] = {0x00, 0x0B, 0x03, 0x04, 0x05};
static const uint8_t KEY_BUF_00_03[] = {0x00, 0x03, 0x09, 0x07, 0x06};
static const uint8_t KEY_BUF_00_03_12[] = {0x00, 0x03, 0x12, 0x11, 0x14};
static const uint8_t KEY_BUF_00_04[] = {0x00, 0x04, 0x11, 0x12, 0x17};
static const uint8_t KEY_BUF_00_10_42[] = {0x00, 0x10, 0x19, 0x15, 0x42};
static const uint8_t KEY_BUF_00_10[] = {0x00, 0x10, 0x19, 0x15, 0x43};
static const uint8_t KEY_BUF_00_1D[] = {0x00, 0x1D, 0x05, 0x04, 0x03};
static const uint8_t KEY_BUF_00_1D_16[] = {0x00, 0x1D, 0x16, 0x17, 0x1A};
static const uint8_t KEY_BUF_00_20[] = {0x00, 0x20, 0x02, 0x03, 0x04};
static const uint8_t KEY_BUF_00_0E[] = {0x00, 0x0E, 0x18, 0x19, 0x4A};
static const uint8_t KEY_BUF_00_3D[] = {0x00, 0x3D, 0x1D, 0x1E, 0x1E};
static const uint8_t KEY_BUF_00_45[] = {0x00, 0x45, 0x19, 0x15, 0x42};
static const uint8_t KEY_BUF_00_4C[] = {0x00, 0x4C, 0x52, 0x53, 0x01};
static const uint8_t KEY_BUF_00_50[] = {0x00, 0x50, 0x1F, 0x40, 0x17};
static const uint8_t KEY_BUF_00_00_20[] = {0x00, 0x00, 0x20, 0x21, 0x22};

static const uint8_t KEY_BUF_01_02[] = {0x01, 0x02, 0x06, 0x07, 0x05};
static const uint8_t KEY_BUF_01_06[] = {0x01, 0x06, 0x04, 0x05, 0x45};
static const uint8_t KEY_BUF_01_0C[] = {0x01, 0x0C, 0x03, 0x04, 0x1D};
static const uint8_t KEY_BUF_01_06_0C[] = {0x01, 0x06, 0x0C, 0x10, 0x04};
static const uint8_t KEY_BUF_01_08[] = {0x01, 0x08, 0x0E, 0x0C, 0x40};
static const uint8_t KEY_BUF_01_08_0C[] = {0x01, 0x08, 0x0C, 0x10, 0x04};
static const uint8_t KEY_BUF_01_16[] = {0x01, 0x16, 0x06, 0x07, 0x00};
static const uint8_t KEY_BUF_01_19[] = {0x01, 0x19, 0x2E, 0x2F, 0x06};
static const uint8_t KEY_BUF_01_1e[] = {0x01, 0x1e, 0x2E, 0x2F, 0x06};
static const uint8_t KEY_BUF_01_22[] = {0x01, 0x22, 0x04, 0x05, 0x1E};
static const uint8_t KEY_BUF_01_25[] = {0x01, 0x25, 0x2E, 0x2F, 0x06};
static const uint8_t KEY_BUF_01_32[] = {0x01, 0x32, 0x2E, 0x2F, 0x06};
static const uint8_t KEY_BUF_01_36[] = {0x01, 0x36, 0x0a, 0x09, 0x06};
static const uint8_t KEY_BUF_01_37[] = {0x41, 0x77, 0x39, 0x7B, 0x03};
static const uint8_t KEY_BUF_01_3E[] = {0x01, 0x3E, 0x2E, 0x2F, 0x26};
static const uint8_t KEY_BUF_01_FF[] = {0x01, 0XFC, 0x0E, 0x0C, 0x40};
static const uint8_t KEY_BUF_01_46[] = {0x01, 0x46, 0x0C, 0x10, 0x04};
static const uint8_t KEY_BUF_01_50[] = {0x01, 0x50, 0x0E, 0x0C, 0x40};

static const uint8_t KEY_BUF_02[] = {0x02, 0x1A, 0x08, 0x09, 0x01};
static const uint8_t KEY_BUF_02_01[] = {0x02, 0x01, 0x07, 0x0B, 0x0F};
static const uint8_t KEY_BUF_02_02[] = {0x02, 0x02, 0x08, 0x09, 0x01};
static const uint8_t KEY_BUF_02_04[] = {0x02, 0x04, 0x08, 0x09, 0x01};
static const uint8_t KEY_BUF_02_10[] = {0x02, 0x10, 0x08, 0x09, 0x01};
static const uint8_t KEY_BUF_02_1B[] = {0x02, 0x1B, 0x07, 0x0B, 0x0F};
static const uint8_t KEY_BUF_02_1D[] = {0x02, 0x1D, 0x08, 0x09, 0x01};
static const uint8_t KEY_BUF_02_1D_FF[] = {0x02, 0x1D, 0XFC, 0XFC, 0XFC};
static const uint8_t KEY_BUF_02_3A[] = {0x02, 0x3A, 0x22, 0x2A, 0x32};
static const uint8_t KEY_BUF_02_78[] = {0x02, 0x78, 0x72, 0x73, 0x0F};
static const uint8_t KEY_BUF_02_78_72_73_23[] = {0x02, 0x78, 0x72, 0x73, 0x23};

static const uint8_t KEY_BUF_03[] = {0x03, 0x1A, 0x0C, 0x0D, 0x07};
static const uint8_t KEY_BUF_03_1E[] = {0x03, 0x1E, 0x16, 0x1A, 0x15};
static const uint8_t KEY_BUF_03_27[] = {0x03, 0x27, 0XFC, 0XFC, 0XFC};
static const uint8_t KEY_BUF_03_44[] = {0x03, 0x44, 0x42, 0x40, 0x15};

static const uint8_t KEY_BUF_04[] = {0x04, 0x45, 0x05, 0x0A, 0x15};

static const uint8_t KEY_BUF_05[] = {0x05, 0x07, 0x03, 0x04, 0x22};
static const uint8_t KEY_BUF_05_1F[] = {0x05, 0x1F, 0x17, 0x57, 0x5F};
static const uint8_t KEY_BUF_05_40[] = {0x05, 0x40, 0x00, 0x01, 0x0B};
static const uint8_t KEY_BUF_05_44[] = {0x05, 0x44, 0x00, 0x01, 0x0B};

static const uint8_t KEY_BUF_06_08[] = {0x06, 0x08, 0XFF, 0XFF, 0x1D};

static const uint8_t KEY_BUF_08_0A[] = {0x08, 0x0A, 0x02, 0x03, 0x09};
static const uint8_t KEY_BUF_08_0B[] = {0x08, 0x0B, 0x02, 0x03, 0x09};
static const uint8_t KEY_BUF_08_23[] = {0x08, 0x23, 0x43, 0x44, 0x09};
static const uint8_t KEY_BUF_08_25[] = {0x08, 0x25, 0x02, 0x03, 0x09};
static const uint8_t KEY_BUF_08_4D[] = {0x08, 0x4D, 0x5E, 0x5F, 0x5F};
static const uint8_t KEY_BUF_08_43[] = {0x08, 0x43, 0x02, 0x03, 0x09};
static const uint8_t KEY_BUF_08_98[] = {0x08, 0x98, 0x02, 0x03, 0x09};

static const uint8_t KEY_BUF_0A[] = {0x0A, 0x0D, 0x06, 0x07, 0x00};
static const uint8_t KEY_BUF_0A_09[] = {0x0A, 0x09, 0x12, 0x15, 0x0B};
static const uint8_t KEY_BUF_0A_0C[] = {0x0A, 0x0C, 0x0E, 0x0F, 0x14};
static const uint8_t KEY_BUF_0A_0C_15[] = {0x0A, 0x0C, 0x15, 0x1C, 0x0F};
static const uint8_t KEY_BUF_0A_12[] = {0x0A, 0x12, 0x15, 0x13, 0x0B};
static const uint8_t KEY_BUF_0A_18[] = {0x0A, 0x18, 0x0E, 0x0F, 0x16};
static const uint8_t KEY_BUF_0A_1B[] = {0x0A, 0x1B, 0x1E, 0x5F, 0x0B};
static const uint8_t KEY_BUF_0A_1E[] = {0x0A, 0x1E, 0x10, 0x11, 0x0C};
static const uint8_t KEY_BUF_0A_40[] = {0x0A, 0x40, 0x15, 0x13, 0x0B};
static const uint8_t KEY_BUF_0A_40_15_1C[] = {0x0A, 0x40, 0x15, 0x1C, 0x0F};
static const uint8_t KEY_BUF_0A_41_15_1C[] = {0x0A, 0x41, 0x15, 0x1C, 0x0F};
static const uint8_t KEY_BUF_0A_53[] = {0x0A, 0x53, 0x1E, 0x5F, 0x0B};
static const uint8_t KEY_BUF_0A_53_50[] = {0x0A, 0x53, 0x1E, 0x5F, 0x50};
static const uint8_t KEY_BUF_0A_53_F9[] = {0x0A, 0x53, 0xF9, 0xF8, 0x0B};

static const uint8_t KEY_BUF_0B_0C[] = {0x0B, 0x0C, 0x12, 0x10, 0x0A};
static const uint8_t KEY_BUF_0B_0F[] = {0x0B, 0x0F, 0x13, 0x12, 0x14};
static const uint8_t KEY_BUF_0B_10[] = {0x0B, 0x10, 0x1E, 0x1D, 0x1F};
static const uint8_t KEY_BUF_0B_15[] = {0x0B, 0x15, 0x1B, 0x1A, 0x0C};

static const uint8_t KEY_BUF_0C[] = {0x0C, 0x74, 0x16, 0x17, 0x28};
static const uint8_t KEY_BUF_0C_75[] = {0x0C, 0x75, 0x16, 0x17, 0x28};
static const uint8_t KEY_BUF_0C_0F[] = {0x0C, 0x0F, 0x13, 0x14, 0x0D};
static const uint8_t KEY_BUF_0C_0F_12_13[] = {0x0C, 0x0F, 0x12, 0x13, 0x0E};
static const uint8_t KEY_BUF_0C_0F_14_15[] = {0x0C, 0x0F, 0x14, 0x15, 0x0D};
static const uint8_t KEY_BUF_0C_0D[] = {0x0C, 0x0D, 0x41, 0x00, 0x58};
static const uint8_t KEY_BUF_0C_0D_10[] = {0x0C, 0x0D, 0x10, 0x11, 0x0E};
static const uint8_t KEY_BUF_0C_0D_1A[] = {0x0C, 0x0D, 0x1A, 0x1B, 0x0A};
static const uint8_t KEY_BUF_0C_1B[] = {0x0C, 0x1B, 0x10, 0x11, 0x0E};

static const uint8_t KEY_BUF_0D_0C[] = {0x0D, 0x0C, 0x5C, 0x59, 0x01};
static const uint8_t KEY_BUF_0D_12[] = {0x0D, 0x12, 0x4C, 0x4D, 0x0E};
static const uint8_t KEY_BUF_0D_12_18[] = {0x0D, 0x12, 0x18, 0x19, 0x0E};
static const uint8_t KEY_BUF_0D_12_44[] = {0x0D, 0x12, 0x44, 0x43, 0x0E};

static const uint8_t KEY_BUF_0E[] = {0x0E, 0x0D, 0x03, 0x04, 0x09};
static const uint8_t KEY_BUF_0E_02[] = {0x0E, 0x02, 0x54, 0x42, 0x12};
static const uint8_t KEY_BUF_0E_40[] = {0x0E, 0x40, 0x54, 0x42, 0x4A};

static const uint8_t KEY_BUF_0F[] = {0x0F, 0XFC, 0x10, 0x11, 0x0E};
static const uint8_t KEY_BUF_0F_12[] = {0x0F, 0x12, 0x0C, 0x0D, 0x0E};
static const uint8_t KEY_BUF_0F_12_0D_0C[] = {0x0F, 0x12, 0x0D, 0x0C, 0x0E};
static const uint8_t KEY_BUF_0F_12_53[] = {0x0F, 0x12, 0x53, 0x54, 0x0E};
static const uint8_t KEY_BUF_0F_14[] = {0x0F, 0x14, 0x0A, 0x0B, 0x18};

static const uint8_t KEY_BUF_10[] = {0x10, 0x0F, 0x13, 0x14, 0x11};
static const uint8_t KEY_BUF_10_12[] = {0x10, 0x12, 0x0E, 0x0F, 0x11};
static const uint8_t KEY_BUF_10_15[] = {0x10, 0x15, 0x1B, 0x1A, 0x1F};
static const uint8_t KEY_BUF_10_17[] = {0x10, 0x17, 0x20, 0x21, 0x1F};
static const uint8_t KEY_BUF_10_40[] = {0x10, 0x40, 0x5B, 0x1E, 0x55};

static const uint8_t KEY_BUF_11_03[] = {0x11, 0x03, 0x08, 0x5E, 0x5D};
static const uint8_t KEY_BUF_11_0B[] = {0x11, 0x0B, 0x08, 0x5E, 0x5D};
static const uint8_t KEY_BUF_11_13[] = {0x11, 0x13, 0x53, 0x57, 0x03};
static const uint8_t KEY_BUF_11_13_13[] = {0x11, 0x0B, 0x08, 0x5E, 0X13};
static const uint8_t KEY_BUF_11_52[] = {0x11, 0x52, 0x18, 0x4F, 0X13};

static const uint8_t KEY_BUF_12[] = {0x12, 0x14, 0x1A, 0x1E, 0x10};
static const uint8_t KEY_BUF_12_00[] = {0x12, 0x00, 0x0D, 0x0E, 0x13};
static const uint8_t KEY_BUF_12_02[] = {0x12, 0x02, 0x0F, 0x13, 0x1C};
static const uint8_t KEY_BUF_12_09[] = {0x12, 0x09, 0x00, 0x0F, 0x10};
static const uint8_t KEY_BUF_12_0E[] = {0x12, 0x0E, 0x0D, 0x14, 0x44};
static const uint8_t KEY_BUF_12_0F[] = {0x12, 0x0F, 0x1A, 0x1E, 0x10};
static const uint8_t KEY_BUF_12_0B[] = {0x12, 0x0B, 0x10, 0x14, 0x16};
static const uint8_t KEY_BUF_12_13[] = {0x12, 0x13, 0x0E, 0x0F, 0x18};
static const uint8_t KEY_BUF_12_14_0E[] = {0x12, 0x14, 0x0E, 0x0F, 0x10};
static const uint8_t KEY_BUF_12_14_1A[] = {0x12, 0x14, 0x1A, 0x1E, 0x11};
static const uint8_t KEY_BUF_12_14_1B[] = {0x12, 0x14, 0x1B, 0x1F, 0x10};
static const uint8_t KEY_BUF_12_14[] = {0x12, 0x14, 0x0D, 0x0E, 0x0B};
static const uint8_t KEY_BUF_12_25[] = {0x12, 0x25, 0x0D, 0x0E, 0x1F};

static const uint8_t KEY_BUF_13[] = {0x13, 0x15, 0x0D, 0x0E, 0x0B};
static const uint8_t KEY_BUF_13_19[] = {0x13, 0x19, 0x0D, 0x0E, 0x25};

static const uint8_t KEY_BUF_14[] = {0x14, 0x16, 0x12, 0x13, 0x15};
static const uint8_t KEY_BUF_14_03[] = {0x14, 0x03, 0x48, 0x49, 0x4C};
static const uint8_t KEY_BUF_14_03_FF[] = {0x14, 0x03, 0x48, 0x49, 0XFF};
static const uint8_t KEY_BUF_14_16_1C[] = {0x14, 0x16, 0x1C, 0x1D, 0x15};

static const uint8_t KEY_BUF_15[] = {0x15, 0x25, 0x12, 0x13, 0x14};
static const uint8_t KEY_BUF_15_12[] = {0x15, 0x12, 0x1B, 0x1A, 0x16};
static const uint8_t KEY_BUF_15_16[] = {0x15, 0x16, 0x13, 0x14, 0x12};
static const uint8_t KEY_BUF_15_1C[] = {0x15, 0x1C, 0x13, 0x19, 0x12};
static const uint8_t KEY_BUF_15_40[] = {0x15, 0x40, 0x12, 0x13, 0x14};
static const uint8_t KEY_BUF_15_57_12[] = {0x15, 0x57, 0x12, 0x13, 0x14};
static const uint8_t KEY_BUF_15_5B[] = {0x15, 0x5B, 0x12, 0x13, 0x14};

static const uint8_t KEY_BUF_16[] = {0x16, 0x13, 0x14, 0x15, 0x17};
static const uint8_t KEY_BUF_16_1A[] = {0x16, 0x1A, 0XFC, 0XFC, 0XFC};

static const uint8_t KEY_BUF_17[] = {0x17, 0x13, 0x1E, 0x1F, 0x1C};
static const uint8_t KEY_BUF_17_06[] = {0x17, 0x06, 0x12, 0x15, 0x1B};
static const uint8_t KEY_BUF_17_09[] = {0x17, 0x09, 0x12, 0x15, 0x0B};
static const uint8_t KEY_BUF_17_09_72[] = {0x17, 0x09, 0x72, 0x73, 0x0B};
static const uint8_t KEY_BUF_17_10[] = {0x17, 0x10, 0x08, 0x04, 0x29};
static const uint8_t KEY_BUF_17_11[] = {0x17, 0x11, 0x1B, 0x1A, 0x0E};
static const uint8_t KEY_BUF_17_1A[] = {0x17, 0x1A, 0x1E, 0x1F, 0x1C};

static const uint8_t KEY_BUF_17_13[] = {0x17, 0x13, 0x1B, 0x1A, 0x0E};
static const uint8_t KEY_BUF_17_2E[] = {0x17, 0x2E, 0x12, 0x15, 0x0B};
static const uint8_t KEY_BUF_17_44[] = {0x17, 0x44, 0x12, 0x15, 0x1B};
static const uint8_t KEY_BUF_17_FF[] = {0x17, 0XFC, 0x1B, 0x1A, 0x0E};

static const uint8_t KEY_BUF_1B[] = {0x1B, 0x13, 0x0A, 0x0B, 0x49};

static const uint8_t KEY_BUF_1C[] = {0x1C, 0x13, 0x0A, 0x0B, 0x49};
static const uint8_t KEY_BUF_1C_0D[] = {0x1C, 0x0D, 0x1F, 0x1E, 0x15};
static const uint8_t KEY_BUF_1C_14[] = {0x1C, 0x14, 0x16, 0x17, 0x15};
static const uint8_t KEY_BUF_1C_17[] = {0x1C, 0x17, 0x4B, 0x4F, 0x08};
static const uint8_t KEY_BUF_1C_1A[] = {0x1C, 0x1A, 0x40, 0x5D, 0x5F};
static const uint8_t KEY_BUF_1C_30[] = {0x1C, 0x30, 0xA2, 0x53, 0x73};
static const uint8_t KEY_BUF_1C_52[] = {0x1C, 0x52, 0xA1, 0xA2, 0xA3};
static const uint8_t KEY_BUF_1C_53[] = {0x1C, 0x53, 0xA1, 0xA2, 0xA3};

static const uint8_t KEY_BUF_1D_16[] = {0x1D, 0x16, 0x0D, 0x0C, 0x18};

static const uint8_t KEY_BUF_1E_1C[] = {0x1E, 0x1C, 0x0A, 0x06, 0x5C};
static const uint8_t KEY_BUF_1F_0C[] = {0x1F, 0x0C, 0x18, 0x19, 0x0E};
static const uint8_t KEY_BUF_1F_12[] = {0x1F, 0x12, 0x00, 0x04, 0x01};
static const uint8_t KEY_BUF_1F_1C[] = {0x1F, 0x1C, 0x10, 0x11, 0x43};
static const uint8_t KEY_BUF_1F_1E[] = {0x1F, 0x1E, 0x18, 0x1C, 0x1B};
static const uint8_t KEY_BUF_1F_1D[] = {0x1F, 0x1D, 0x04, 0x00, 0x08};

static const uint8_t KEY_BUF_20_0D[] = {0x20, 0x0D, 0x30, 0x31, 0x33};
static const uint8_t KEY_BUF_20_1B[] = {0x20, 0x1B, 0x00, 0x01, 0x0B};
static const uint8_t KEY_BUF_20_2A[] = {0x20, 0x2A, 0x01, 0x0D, 0x0B};
static const uint8_t KEY_BUF_20_21[] = {0x20, 0x21, 0x06, 0x07, 0x01};
static const uint8_t KEY_BUF_20_40[] = {0x20, 0x40, 0x60, 0x61, 0x64};

static const uint8_t KEY_BUF_21[] = {0x21, 0x12, 0x38, 0x39, 0x34};
static const uint8_t KEY_BUF_21_1C[] = {0x21, 0x2C, 0x2D, 0x2E, 0x34};
static const uint8_t KEY_BUF_22[] = {0x22, 0x21, 0x27, 0x2B, 0x2F};
static const uint8_t KEY_BUF_27[] = {0x27, 0x38, 0x00, 0x01, 0x20};
static const uint8_t KEY_BUF_33[] = {0x33, 0XFC, 0x00, 0x01, 0x0C};
static const uint8_t KEY_BUF_3D_03[] = {0x3D, 0x03, 0x20, 0x21, 0x91};
static const uint8_t KEY_BUF_3D_6D[] = {0x3D, 0x6D, 0XFC, 0XFC, 0XFC};
static const uint8_t KEY_BUF_3D[] = {0x3D, 0x05, 0x20, 0x21, 0x32};

static const uint8_t KEY_BUF_3F[] = {0x3F, 0x05, 0x20, 0x21, 0x32};
static const uint8_t KEY_BUF_40[] = {0x40, 0x0E, 0x17, 0x15, 0x41};
static const uint8_t KEY_BUF_40_4D[] = {0x40, 0x4D, 0x0C, 0x18, 0x12};
static const uint8_t KEY_BUF_48[] = {0x48, 0x40, 0x4B, 0x49, 0x0B};
static const uint8_t KEY_BUF_49[] = {0x49, 0x4E, 0x0E, 0x0F, 0x0D};
static const uint8_t KEY_BUF_4A[] = {0x4A, 0x4F, 0x5A, 0x5B, 0x62};
static const uint8_t KEY_BUF_4A_73[] = {0x4A, 0x73, 0x6F, 0x70, 0x6A};
static const uint8_t KEY_BUF_4A_76[] = {0x4A, 0x76, 0x6F, 0x70, 0x6A};
static const uint8_t KEY_BUF_4E[] = {0x4E, 0x52, 0x82, 0x83, 0x14};
static const uint8_t KEY_BUF_49_00[] = {0x49, 0x00, 0x0E, 0x0F, 0x0D};
static const uint8_t KEY_BUF_52[] = {0x52, 0x40, 0x03, 0x41, 0x53};
static const uint8_t KEY_BUF_56[] = {0x56, 0x0C, 0x0F, 0x0E, 0x1E};
static const uint8_t KEY_BUF_5B[] = {0x5B, 0x1D, 0x5C, 0x58, 0x57};
static const uint8_t KEY_BUF_5B_5C[] = {0x5B, 0x5C, 0x19, 0x1A, 0x5E};
static const uint8_t KEY_BUF_5F[] = {0x5F, 0x4F, 0x4A, 0x4D, 0x49};

static const uint8_t KEY_BUF_80[] = {0x80, 0xCC, 0x91, 0x92, 0x81};
static const uint8_t KEY_BUF_80_9E[] = {0x80, 0x9E, 0x03, 0x04, 0x09};
static const uint8_t KEY_BUF_82[] = {0x82, 0x90, 0x88, 0x89, 0x81};
static const uint8_t KEY_BUF_82_CC[] = {0x82, 0xCC, 0x9F, 0x8B, 0x98};
static const uint8_t KEY_BUF_82_93[] = {0x82, 0x93, 0x9F, 0x8B, 0xD0};
static const uint8_t KEY_BUF_83[] = {0x83, 0x8E, 0x87, 0x8B, 0x97};
static const uint8_t KEY_BUF_83_C3[] = {0x83, 0xC3, 0x87, 0x8B, 0x97};
static const uint8_t KEY_BUF_84[] = {0x84, 0x21, 0x88, 0x09, 0xA0};
static const uint8_t KEY_BUF_84_87[] = {0x84, 0x87, 0x88, 0x89, 0xA0};
static const uint8_t KEY_BUF_87_8C[] = {0x87, 0x8C, 0XFF, 0XFF, 0x8B};

static const uint8_t KEY_BUF_90_9D[] = {0x90, 0x9D, 0xB1, 0xB3, 0x93};
static const uint8_t KEY_BUF_E0[] = {0xE0, 0xCD, 0xF0, 0xF1, 0xF3};

static const uint8_t KEY_BUF_D7_80[] = {0xD7, 0x80, 0x96, 0x97, 0x95};
static const uint8_t KEY_BUF_D7_C3[] = {0xD7, 0xC3, 0x83, 0x86, 0xDF};
static const uint8_t KEY_BUF_D7_DC[] = {0xD7, 0xDC, 0x83, 0x86, 0xDB};
static const uint8_t KEY_BUF_DB_1D[] = {0xDB, 0x1D, 0x09, 0x08, 0x5C};
static const uint8_t KEY_BUF_DB_5C[] = {0xDB, 0x5C, 0x19, 0x1A, 0x5E};
static const uint8_t KEY_BUF_C0[] = {0xC0, 0x9C, 0xCE, 0x9A, 0xC4};
static const uint8_t KEY_BUF_C0_0A[] = {0xC0, 0x0A, 0x02, 0x03, 0x09};
static const uint8_t KEY_BUF_C0_C9[] = {0xC0, 0xC9, 0xCC, 0xD4, 0xC4};
static const uint8_t KEY_BUF_C0_9C[] = {0xC0, 0x9C, 0xCE, 0x9A, 0xC4};
static const uint8_t KEY_BUF_C2[] = {0xC2, 0x93, 0x9B, 0x8F, 0xD0};

static const uint8_t HIGH_KEY_BUF_04_02[] = {0x10, 0x08, 0x20, 0x1C, 0x0C};
static const uint8_t HIGH_KEY_BUF_06[] = {0x30, 0XFC, 0x10, 0x08, 0x40};
static const uint8_t HIGH_KEY_BUF_07[] = {0x1C, 0x18, 0x44, 0x40, 0x04};
static const uint8_t HIGH_KEY_BUF_0B[] = {0x0B, 0x18, 0x0D, 0x0E, 0x21};
static const uint8_t HIGH_KEY_BUF_0B_0A[] = {0x0B, 0x0A, 0x13, 0x12, 0x14};
static const uint8_t HIGH_KEY_BUF_0B_0F[] = {0x0B, 0x0F, 0x13, 0x12, 0x14};
static const uint8_t HIGH_KEY_BUF_0B_1C[] = {0x0B, 0x1C, 0x13, 0x12, 0x14};
static const uint8_t HIGH_KEY_BUF_0C[] = {0x30, 0xE0, 0x40, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_00[] = {0x30, 0x00, 0x54, 0x50, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_00_10[] = {0x30, 0x00, 0x40, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_0A[] = {0x30, 0x28, 0X54, 0X50, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_1A[] = {0x30, 0x68, 0XB8, 0X44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_1B[] = {0x30, 0x6C, 0X58, 0X54, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_2B[] = {0x30, 0xAC, 0X58, 0X54, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_30[] = {0x30, 0xC0, 0x58, 0x54, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_31[] = {0x30, 0xC4, 0x40, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_38[] = {0x30, 0xE0, 0x58, 0x54, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_38_0E[] = {0x30, 0xE0, 0x38, 0x90, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_38_11[] = {0x30, 0xE0, 0x44, 0x40, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_38_10[] = {0x0C, 0x38, 0x10, 0x11, 0x0D};
static const uint8_t HIGH_KEY_BUF_0C_38_10_11_0D[] = {0x0C, 0x38, 0x10, 0x11, 0x0D};
static const uint8_t HIGH_KEY_BUF_0C_38_10_11[] = {0x30, 0xE0, 0x40, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_38_12_11[] = {0x30, 0xE0, 0x48, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_38_12_13[] = {0x30, 0xE0, 0x48, 0x4C, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_3A[] = {0x30, 0xE8, 0x40, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_3C[] = {0x30, 0xF0, 0x40, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_3B[] = {0x30, 0xEC, 0x48, 0x44, 0x34};
static const uint8_t HIGH_KEY_BUF_0C_FF[] = {0x30, 0XFC, 0x40, 0x44, 0x0D};
static const uint8_t HIGH_KEY_BUF_0C_FF_0E[] = {0x30, 0XFC, 0x38, 0xB8, 0x34};
static const uint8_t HIGH_KEY_BUF_0D_00[] = {0x34, 0x00, 0xD8, 0xD4, 0x4C};
static const uint8_t HIGH_KEY_BUF_0D_0E[] = {0x34, 0x38, 0x44, 0x48, 0x54};
static const uint8_t HIGH_KEY_BUF_0E[] = {0x38, 0x44, 0x80, 0x84, 0x2C};
static const uint8_t HIGH_KEY_BUF_0F[] = {0x3C, 0x30, 0x84, 0xD8, 0x04};

static const uint8_t HIGH_KEY_BUF_10[] = {0x2A, 0xA3, 0x2F, 0x2E, 0x3F};
static const uint8_t HIGH_KEY_BUF_1B[] = {0x1B, 0x17, 0x16, 0x19, 0x0B};
static const uint8_t HIGH_KEY_BUF_1D[] = {0x74, 0x60, 0x6C, 0x70, 0x14};
static const uint8_t HIGH_KEY_BUF_1D_18_29_2A[] = {0x3A, 0x30, 0x52, 0x54, 0x0A};
static const uint8_t HIGH_KEY_BUF_1D_18_29[] = {0x74, 0x60, 0xA4, 0xA8, 0x14};
static const uint8_t HIGH_KEY_BUF_1D_0E[] = {0x74, 0x38, 0xA4, 0xA8, 0x14};
static const uint8_t HIGH_KEY_BUF_1D_13[] = {0x74, 0x4C, 0xDC, 0x50, 0xBC};
static const uint8_t HIGH_KEY_BUF_1D_43[] = {0x3A, 0x86, 0x52, 0x54, 0x0A};

static const uint8_t HIGH_KEY_BUF_20[] = {0x80, 0xA0, 0x18, 0x1C, 0x04};
static const uint8_t HIGH_KEY_BUF_20_21[] = {0x80, 0x84, 0x90, 0x94, 0xD8};
static const uint8_t HIGH_KEY_BUF_20_2A[] = {0x80, 0xA8, 0x90, 0x94, 0xAC};
static const uint8_t HIGH_KEY_BUF_20_29[] = {0x80, 0xA4, 0x90, 0x94, 0xD8};
static const uint8_t HIGH_KEY_BUF_2A_A3[] = {0x2A, 0xA3, 0x2E, 0x2F, 0x3F};
static const uint8_t HIGH_KEY_BUF_30_00[] = {0xC0, 0XFC, 0x40, 0x44, 0xA4};
static const uint8_t HIGH_KEY_BUF_30_21[] = {0x30, 0x21, 0x1C, 0x1D, 0x1E};
static const uint8_t HIGH_KEY_BUF_32_29[] = {0xC8, 0xE4, 0x88, 0x8C, 0xC4};
static const uint8_t HIGH_KEY_BUF_32_36[] = {0xC8, 0xD8, 0x84, 0x44, 0xD0};
static const uint8_t HIGH_KEY_BUF_34[] = {0xD0, 0x0C, 0x7C, 0x54, 0xB0};
static const uint8_t HIGH_KEY_BUF_34_03_15[] = {0xD0, 0x0C, 0x54, 0x7C, 0xB0};

static const uint16_t KEY_BUF_10C[] = {0x10C, 0xFC, 0x116, 0x117, 0x128};
static const uint16_t KEY_BUF_037[] = {0x100c, 0xFC, 0x1016, 0x1017, 0x1028};
static const uint16_t KEY_BUF_84_84[] = {0x0084, 0x0384, 0x2084, 0x2184, 0x2184};
static const uint16_t KEY_BUF_8A_84[] = {0x008A, 0x408B, 0x108C, 0x118C, 0x128C};
static const uint8_t DVB_21[] = {
    0X00,

    0X12, //pwr 1
    0X99, // back + menu
    0X00,
    0X07, //MEH 11
    0X58, //右箭头 11

    0X0B, //UP 6
    0x49, //LEFT 1
    0X4A, //RIGHT 1
    0X0F, //down 1
    0X5A, //lleft 11

    0X02, //BACK 11
    0X0E, //lright
    0X20,  //vol+
    0X04, //暂停 11
    0x18, // ch+

    0X22, // 16
    0X21, //vol - 
    0X1C, //ch- 11
    0X60, //方框 11
    0X00, //飞鼠

    0X00,
    0X15, //1 1
    0X17, //3 1
    0X0D, //OK 1
    0X16, //2

    0X19, //4 1
    0X1B, //6 1
    0X1A, // 5 1
    0X1D, //7 1
    0X1F, //9 1

    0X1E, //8 1
    0X41 // 0 1
};

static const uint16_t error_11_av[] = {38000, 30, 13, 600, 13, 140, 13, 140, 13, 140, 13, 300, 13, 140, 13, 140, 13, 450, 13, 140, 13, 140, 13, 140, 13, 140, 13, 300, 13, 600, 13, 5750};
static const uint16_t error_11_tv[] = {38000, 30, 13, 600, 13, 140, 13, 140, 13, 140, 13, 300, 13, 140, 13, 140, 13, 140, 13, 140, 13, 140, 13, 300, 13, 140, 13, 300, 13, 600, 13, 5750};
static const uint16_t error_11_vol[] = {38000, 30,13, 600, 13, 140, 13, 140, 13, 140, 13, 300,13, 140, 13, 140, 13, 450, 13, 300, 13, 450,13, 300, 13, 140, 13, 140, 13, 600, 13, 5700};
static const uint16_t error_11_vol_[] = {38000, 30, 13, 600, 13,140, 13,140, 13, 140, 13, 300,13, 140, 13, 300, 13, 450, 13, 300, 13, 450, 13, 300, 13, 140, 13, 300, 13, 600, 13, 5750};
static const uint16_t error_11_mute[] = {38000, 30, 13, 600, 13,140, 13,140, 13, 140, 13, 300,13, 140, 13, 300, 13, 140, 13, 140, 13, 140, 13, 140, 13, 140, 13, 300, 13, 600, 13, 5750};
static uint8_t coust_brand_0;
static uint8_t coust_brand_1;
static uint8_t *ir_brand;
static uint16_t *dvb_ir_brand;

const uint16_t search_ir_number[] = {
    0,0,1,9,10,11,35,36,37,45,47,53,60,68,70,72,73,74,93,105,108,109,150,154,163,166,170,178,180,191,195,200,208,217,218,225,226,235,245,247,250,264,282,287,292,327,339,343,346,349,355,356,361,367,370,371,374,388,394,411,412,418,435,443,447,451,455,463,471,473,480,481,486,487,498,499,508,512,535,548,554,556,560,565,576,578,587,602,605,606,610,618,619,620,624,625,631,633,634,644,648,650,653,655,661,668,672,679,683,696,698,701,706,712,714,715,719,744,746,753,754,760,766,767,790,796,797,808,810,812,814,818,819,820,821,825,830,832,834,852,853,858,865,876,877,880,882,887,
    890,1005,1037,1039,1040,1045,1072,1081,1089,1112,1137,1149,1156,1163,1183,1193,1208,1217,1223,1232,1235,1243,1248,1249,1259,1260,1265,1269,1270,1281,1289,1290,1295,1298,1305,1308,1310,1312,1314,1319,1324,1326,1335,1338,1339,1343,1348,1351,1363,1371,1372,1376,1388,1393,1394,1413,1423,1430,1447,1454,1455,1457,1458,1468,1474,1480,1481,1484,1495,1505,1506,1508,1511,1521,1523,1531,1535,1539,1542,1546,1555,1556,1560,1562,1563,1567,1574,1576,1582,1583,1584,1585,1588,1595,1598,1606,1610,1612,
    1615,1617,1619.1624,1625,1630,1633,1636,1637,1644,1645,1649,1650,1651,1652,1656,1659,1661,1663,1666,1667,1678,1681,1682,1685,1687,1691,1694,1695,1701,1702,1704,1709,1719,1720,1721,1727,1732,1744,1751,1754,1755,1756,1768,1771,1772,1775,1776,1779,1783,1784,1795,1812,1817,1818,1820,1825,1840,1842,1846,1847,1849,1852,1854,1859,1860,1865,1867,1871,1884,1887,1899,1900,1902,1908,1909,1916,1935,1954,1955,1963,1964,1982,1983,1985,1988,1994,2001,2005,2007,2015,2020,2021,2022,2027,2028,2032,2037,2050,2051,2053,2055,2056,2057,2072,2074,2094,2097,2098,
    2100,2101,2103,2104,2106,2108,2111,2118,2125,2127,2129,2131,2132,2134,2137,2144,2146,2154,2155,2168,2182,2190,2197,2200,2207,2212,2214,2218,2223,2241,2245,2254,2264,2265,2266,2267,2268,2270,2272,2274,2276,2279,2285,2329,2331,2332,2333,2342,2353,2362,2390,2399,2402,2422,2426,2450,2457,2463,2561,2578,2598,2604,2625,2663,2665,2668,2673,2675,2714,2715,2718,2719,2731,2732,2748,2767,2778,2795,2797,2801,2807,2810,2817,2818,2890,3024,4001,4002,4003,4004,4005,4006,4007,4008,4009,4010,4011,4012,4013,4014,4015,4016,4017,4018,4019,4020,4021,4022,4023,4024,4025,4026,4027,4028,4029,4030,4031,4032,4033,
};

static void ir_247_brand(uint8_t brand)
{
     if (brand == TV) {
        ir_time_send(&error_11_av[0]);
    }
    else if (brand == AV) {
        ir_time_send(&error_11_tv[0]);
    }
    else if (brand == VOL_H) {
        ir_time_send(&error_11_vol[0]);
    }
    else if (brand == VOL_L) {
        ir_time_send(&error_11_vol_[0]);
    }
    else if (brand == METU) {
        ir_time_send(&error_11_mute[0]);
    }
}

static void ir_587_brand(uint8_t brand)
{
    if (brand == TV || brand == AV || brand == METU)
    {
        ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
        if (brand == TV) {
            ir_comm_send(HIGH_KEY_BUF_0C_38[0]);
        }
        else if (brand == AV) {
            if (HIGH_KEY_BUF_0C_38[1] != 0XFC) {
                ir_comm_send(HIGH_KEY_BUF_0C_38[1]);
            }
        }
        else if (brand == METU) {
            if (HIGH_KEY_BUF_0C_38[4] != 0XFC) {
                ir_comm_send(HIGH_KEY_BUF_0C_38[4]);
            }
        }
    }
    else
    {
        ir_type_init(SAA3010_36_46, CUSTOM_00_00_A);
        if (brand == VOL_H) {
            if (HIGH_KEY_BUF_0C_38[2] != 0XFC) {
                ir_comm_send(HIGH_KEY_BUF_0C_38[2]);
            }
        }
        else if (brand == VOL_L) {
            if (HIGH_KEY_BUF_0C_38[3] != 0XFC) {
                ir_comm_send(HIGH_KEY_BUF_0C_38[3]);
            }
        }
    }
}

static void ir_683_brand(uint8_t brand)
{
    if (brand == TV) {
        ir_comm_send(dvb_ir_brand[0]);
    }
    else if (brand == AV && dvb_ir_brand[1] != 0xFC) {
        ir_comm_send(dvb_ir_brand[1]);
    }
    else if (brand == VOL_H) {
        ir_comm_send(dvb_ir_brand[2]);
    }
    else if (brand == VOL_L) {
        ir_comm_send(dvb_ir_brand[3]);
    }
    else if (brand == METU) {
        ir_comm_send(dvb_ir_brand[4]);
    }
}

static void ir_834_brand(uint8_t brand)
{
    if (brand != AV)
    {
        ir_type_init(UPD6124_D7C8_70, CUSTOM_54_00_A);
        if (brand == TV) {
            ir_comm_send(KEY_BUF_15_57_12[0]);
        }
        else if (brand == METU) {
            ir_comm_send(KEY_BUF_15_57_12[4]);
        }
        else if (brand == VOL_H) {
            ir_comm_send(KEY_BUF_15_57_12[2]);
        }
        else if (brand == VOL_L) {
            ir_comm_send(KEY_BUF_15_57_12[3]);
        }
    }
    else {
        ir_type_init(UPD6124_D7C13_71, CUSTOM_055A_0000_A);
        if (brand == AV) {
            ir_comm_send(KEY_BUF_15_57_12[1]);
        }
    }
}

static void ir_1704_brand(uint8_t brand)
{
    if (brand != TV)
    {
        ir_type_init(UPD6121G_64, coust_brand_0);
        if (brand == AV) {
            ir_comm_send(ir_brand[1]);
        }
        else if (brand == METU) {
            ir_comm_send(ir_brand[4]);
        }
        else if (brand == VOL_H) {
            ir_comm_send(ir_brand[2]);
        }
        else if (brand == VOL_L) {
            ir_comm_send(ir_brand[3]);
        }
    }
    else {
        ir_type_init(UPD6121G_64, coust_brand_1);
        if (brand == TV) {
            ir_comm_send(ir_brand[0]);
        }
    }
}

static void ir_1818_brand(uint8_t brand)
{
    if (brand != AV)
    {
        ir_type_init(VICTOR_C8D8_72, coust_brand_0);
        if (brand == TV) {
            ir_comm_send(ir_brand[0]);
        }

        else if (brand == METU) {
            ir_comm_send(ir_brand[4]);
        }
        else if (brand == VOL_H) {
            ir_comm_send(ir_brand[2]);
        }
        else if (brand == VOL_L) {
            ir_comm_send(ir_brand[3]);
        }
    }
    else {
        ir_type_init(VICTOR_C8D8_72, coust_brand_1);
        if (brand == AV) {
            ir_comm_send(ir_brand[1]);
        }
    }
}

static void ir_2665_brand(uint8_t brand)
{
    if (brand != AV)
    {
        ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
        if (brand == TV) {
            ir_comm_send(HIGH_KEY_BUF_0C_38_10_11[0]);
        }
        else if (brand == METU) {
            ir_comm_send(HIGH_KEY_BUF_0C_38_10_11[4]);
        }
        else if (brand == VOL_H) {
            ir_comm_send(HIGH_KEY_BUF_0C_38_10_11[2]);
        }
        else if (brand == VOL_L) {
            ir_comm_send(HIGH_KEY_BUF_0C_38_10_11[3]);
        }
    }
    else {
        ir_type_init(PHILIPS_20_38, HIGH_CUSTOM_8000_0000_A);
        if (brand == AV) {
            ir_comm_send(0X38);
        }
    }
}

static void ir_DVB(uint8_t brand)
{
    if (DVB_21[brand] != 0x00) {
        ir_comm_send(DVB_21[brand]);
    }
}

static void ir_AV(uint8_t brand)
{
    if (brand == TV) {
        ir_comm_send(ir_brand[0]);
    }
    else if (brand == AV) {
        if (ir_brand[1] != 0XFC) {
            ir_comm_send(ir_brand[1]);
        }
    }
    else if (brand == VOL_H) {
        if (ir_brand[2] != 0XFC) {
            ir_comm_send(ir_brand[2]);
        }
    }
    else if (brand == VOL_L) {
        if (ir_brand[3] != 0XFC) {
            ir_comm_send(ir_brand[3]);
        }
    }
    else if (brand == METU) {
        if (ir_brand[4] != 0XFC) {
            ir_comm_send(ir_brand[4]);
        }
    }
}

ir_brand_init_t brand_AV_init(uint16_t brand)
{
    if (brand == 105) {
        dvb_ir_brand = (uint16_t *)KEY_BUF_10C;
        ir_type_init(GEMINI_C16_38_08, CUSTOM_FFFD_0000_A);
        return ir_683_brand;
    }
    else if (brand == 247) {
        return ir_247_brand;
    }
    else if (brand == 346) {
        dvb_ir_brand = (uint16_t *)KEY_BUF_037;
        ir_type_init(GEMINI_C16_38_08, CUSTOM_FFFD_0000_A);
        return ir_683_brand;
    }
    else if (brand == 587) {
        return ir_587_brand;
    }
    else if (brand == 834) {
        return ir_834_brand;
    }
    else if (brand == 683) {
        dvb_ir_brand = (uint16_t *)KEY_BUF_84_84;
        ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_1463_0084_A);
        return ir_683_brand;
    }
    else if (brand == 853) {
        dvb_ir_brand = (uint16_t *)KEY_BUF_8A_84;
        ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_1463_0084_A);
        return ir_683_brand;
    }

    else if (brand == 1704) {
        coust_brand_0 = CUSTOM_18_E7_A;
        coust_brand_1 = CUSTOM_18_18_A;
        ir_brand = (uint8_t *)KEY_BUF_C0_0A;
        return ir_1704_brand;
    }
    else if (brand == 2074) {
        coust_brand_0 = CUSTOM_50_AF_A;
        coust_brand_1 = CUSTOM_80_23_A;
        ir_brand = (uint8_t *)KEY_BUF_0A_09;
        return ir_1704_brand;
    }

    else if (brand == 1818 || brand == 2118 || brand == 2801) {
        coust_brand_0 = CUSTOM_03_00_A;
        coust_brand_1 = CUSTOM_0F_00_A;
        ir_brand = (uint8_t *)KEY_BUF_17_1A;
        return ir_1818_brand;
    }

    else if (brand == 2665) {
        return ir_2665_brand;
    }
    else
    {
        switch (brand)
        {
            case 36:
                ir_brand = (uint8_t *)KEY_BUF_0E;
                ir_type_init(CUSTOM_6BIT_02, DEFAULT_CUSTOMER);
                break;
            case 683:
                ir_brand = (uint8_t *)KEY_BUF_82;
                ir_type_init(DVB_PAN_7051_SAMSUN_05, CUSTOM_1463_0084_A);
                break;
            case 74:
                ir_brand = (uint8_t *)KEY_BUF_82;
                ir_type_init(FB739A_06, CUSTOM_08_00_A);
                break;
            case 200:
                ir_brand = (uint8_t *)KEY_BUF_84;
                ir_type_init(FB739A_06, CUSTOM_08_00_A);
                break;
            case 327:
                ir_brand = (uint8_t *)KEY_BUF_84_87;
                ir_type_init(FB739A_06, CUSTOM_08_00_A);
                break;
            case 1223:
            case 2625:
                ir_brand = (uint8_t *)KEY_BUF_02_04;
                ir_type_init(GEMINI_C10_32_08, CUSTOM_FFFF_FFFF_A);
                break;
            case 191:
            case 195:
            case 554:
                ir_brand = (uint8_t *)KEY_BUF_02_1D;
                ir_type_init(GEMINI_C10_32_08, CUSTOM_FFFF_FFFF_A);
                break;
            case 535:
            case 1582:
                ir_brand = (uint8_t *)KEY_BUF_02_10;
                ir_type_init(GEMINI_C10_32_08, CUSTOM_FFFF_FFFF_A);
                break;
            case 292:
                ir_brand = (uint8_t *)KEY_BUF_20_21;
                ir_type_init(GEMINI_C10_32_08, CUSTOM_FFFF_FFFF_A);
                break;
            case 370:
                ir_brand = (uint8_t *)KEY_BUF_27;
                ir_type_init(GEMINI_C10_32_08, CUSTOM_FFFF_FFFF_A);
                break;
            case 367:
                ir_brand = (uint8_t *)KEY_BUF_33;
                ir_type_init(GEMINI_C10_32_08, CUSTOM_FFFF_FFFF_A);
                break;
            case 486:
                ir_brand = (uint8_t *)KEY_BUF_20_1B;
                ir_type_init(GEMINI_C10_38_09, CUSTOM_FFFD_FFFF_A);
                break;
            case 163:
                ir_brand = (uint8_t *)KEY_BUF_01_25;
                ir_type_init(IRT1250C5D6_12, CUSTOM_00_00_A);
                break;
            case 349:
                ir_brand = (uint8_t *)KEY_BUF_01_3E;
                ir_type_init(IRT1250C5D6_12, CUSTOM_00_00_A);
                break;
            case 356:
                ir_brand = (uint8_t *)KEY_BUF_01_19;
                ir_type_init(IRT1250C5D6_12, CUSTOM_00_00_A);
                break;
            case 361:
                ir_brand = (uint8_t *)KEY_BUF_01_32;
                ir_type_init(IRT1250C5D6_12, CUSTOM_00_00_A);
                break;
            case 473:
                ir_brand = (uint8_t *)KEY_BUF_01_36;
                ir_type_init(IRT1250C5D6_12, CUSTOM_00_00_A);
                break;
            case 548:
                ir_brand = (uint8_t *)KEY_BUF_01_1e;
                ir_type_init(IRT1250C5D6_12, CUSTOM_00_00_A);
                break;   
            case 217:
                ir_brand = (uint8_t *)KEY_BUF_01_37;
                ir_type_init(IRT1250C5D7_13, CUSTOM_10_EF_A);
                break;
            case 2055:
            case 2268:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0B_0A;
                ir_type_init(KK_Y261_14, CUSTOM_0200_0000_A);
                break;
            case 4018:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0B_0F;
                ir_type_init(KK_Y261_14, CUSTOM_0200_0000_A);
                break;
            case 2267:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0B_1C;
                ir_type_init(KK_Y261_14, CUSTOM_0200_0000_A);
                break;
            case 766:
                ir_brand = (uint8_t *)KEY_BUF_21;
                ir_type_init(LBNSAMSUNG_15, CUSTOM_01_00_A);
                break;
            case 814:
                ir_brand = (uint8_t *)KEY_BUF_21_1C;
                ir_type_init(LBNSAMSUNG_15, CUSTOM_01_00_A);
                break;
            case 701:
            case 1955:
                ir_brand = (uint8_t *)KEY_BUF_00_0E;
                ir_type_init(LC7461M_C13S_16, CUSTOM_007B_1F84_A);
                break;
            case 754:
                ir_brand = (uint8_t *)KEY_BUF_1C_0D;
                ir_type_init(LC7461M_C13S_16, CUSTOM_0119_1EE6_A);
                break;
            case 4014:
                ir_brand = (uint8_t *)KEY_BUF_12_0B;
                ir_type_init(LC7461M_C13S_16, CUSTOM_011C_1EE3_A);
                break;
            case 858:
                ir_brand = (uint8_t *)KEY_BUF_12_0B;
                ir_type_init(LC7461M_C13S_16, CUSTOM_013C_1EC3_A);
                break;
            case 250:
            case 650:
            case 1480:
            case 1636:
            case 1650:
                ir_brand = (uint8_t *)KEY_BUF_3D;
                ir_type_init(LC7464M_PANASONIC_18, CUSTOM_2002_0080_A);
                break;
            case 2264:
                ir_brand = (uint8_t *)KEY_BUF_3F;
                ir_type_init(LC7464M_PANASONIC_18, CUSTOM_2002_0080_A);
                break;
            case 471:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_0E;
                ir_type_init(LG3004_33K_19_Toggle_two, HIGH_CUSTOM_B000_A000_A);
                break;
            case 498:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_0E;
                ir_type_init(LG3004_33K_19_Toggle_two, HIGH_CUSTOM_B000_A000_A);
                break;
            case 560:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_0E;
                ir_type_init(LG3004_33K_19_Toggle_one, HIGH_CUSTOM_B000_F000_A);
                break;
            case 287:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D;
                ir_type_init(LG3004_33K_19_Toggle_one, CUSTOM_FFFD_FFFF_A);
                break;
            case 109:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D;
                ir_type_init(LG3004_33K_19_TOGGLE_TWO_Freq_0, CUSTOM_FFFD_FFFF_A);
                break;
            case 343:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_0E;
                ir_type_init(LG3004_33K_19_TOGGLE_TWO_Freq_0, CUSTOM_FFFD_FFFF_A);
                break;
            case 388:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_18_29;
                ir_type_init(LG3004_33K_19_Toggle_two, CUSTOM_FFFD_FFFF_A);
                break;
            case 808:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_13;
                ir_type_init(M3004_38K_20, HIGH_CUSTOM_B000_F000_A);
                break;
            case 455:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_07;
                ir_type_init(M3004_38K_20, CUSTOM_8000_FFFF_A);
                break;
            case 825:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_07;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 72:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_04_02;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 418:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_20;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 606:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_20_21;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 1324:
            case 1372:
            case 2146:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_20_29;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 715:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_20_2A;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 411:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_30_00;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 1624:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_32_29;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 668:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_34;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;
            case 4032:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_34_03_15;
                ir_type_init(M3004_38K_20, CUSTOM_FFFD_FFFF_A);
                break;

            case 11:
                ir_brand = (uint8_t *)KEY_BUF_02;
                ir_type_init(M50119P_23, CUSTOM_05_00_A);
                break;
            case 1269:
            case 2285:
                ir_brand = (uint8_t *)KEY_BUF_02_3A;
                ir_type_init(M50462_24, CUSTOM_47_00_A);
                break;
            case 1633:
                ir_brand = (uint8_t *)KEY_BUF_06_08;
                ir_type_init(M50462_24, CUSTOM_73_00_A);
                break;
            case 4029:
                ir_brand = (uint8_t *)KEY_BUF_0F_12;
                ir_type_init(M50560_25, CUSTOM_00_00_A);
                break;
            case 1:
                ir_brand = (uint8_t *)KEY_BUF_0E;
                ir_type_init(M50560_25, CUSTOM_01_00_A);
                break;
            case 9:
                ir_brand = (uint8_t *)KEY_BUF_0E;
                ir_type_init(M50560_25, CUSTOM_02_00_A);
                break;
            case 4004:
                ir_brand = (uint8_t *)KEY_BUF_02_01;
                ir_type_init(M50560_25, CUSTOM_04_00_A);
                break;
            case 4003:
                ir_brand = (uint8_t *)KEY_BUF_22;
                ir_type_init(M50560_25, CUSTOM_04_00_A);
                break;
            case 374:
                ir_brand = (uint8_t *)KEY_BUF_1F_1E;
                ir_type_init(M50560_25, CUSTOM_14_00_A);
                break;
            case 451:
            case 624:
            case 634:
            case 661:
            case 1661:
            case 1902:
            case 1909:
            case 2266:
                ir_brand = (uint8_t *)KEY_BUF_15_16;
                ir_type_init(M50560_25, CUSTOM_14_00_A);
                break;
            case 1694:
                ir_brand = (uint8_t *)KEY_BUF_15_1C;
                ir_type_init(M50560_25, CUSTOM_14_00_A);
                break;
            case 487:
                ir_brand = (uint8_t *)KEY_BUF_1F_1D;
                ir_type_init(M50560_25, CUSTOM_1B_00_A);
                break;
            case 712:
                ir_brand = (uint8_t *)KEY_BUF_05;
                ir_type_init(M50560_25, CUSTOM_DC_00_A);
                break;
            case 1681:
                ir_brand = (uint8_t *)KEY_BUF_0E;
                ir_type_init(M50560_25, CUSTOM_E6_00_A);
                break;
            case 1985:
                ir_brand = (uint8_t *)KEY_BUF_80_9E;
                ir_type_init(M50560_25, CUSTOM_EE_00_A);
                break;
            case 108:
            case 150:
                ir_brand = (uint8_t *)KEY_BUF_02_3A;
                ir_type_init(MITC8D8_28, CUSTOM_47_00_A);
                break;
            case 218:
                ir_brand = (uint8_t *)KEY_BUF_0A;
                ir_type_init(MN6014C6D6_29, CUSTOM_16_E9_A);
                break;
            case 282:
            case 1040:
                ir_brand = (uint8_t *)KEY_BUF_0A;
                ir_type_init(MN6014C6D6_29, CUSTOM_0A_35_A);
                break;
            case 1394:
            case 1595:
                ir_brand = (uint8_t *)KEY_BUF_20_0D;
                ir_type_init(MN6014C6D6_29, CUSTOM_29_D6_A);
                break;
            case 1695:
            case 1755:
                ir_brand = (uint8_t *)KEY_BUF_01_02;
                ir_type_init(MN6014C6D6_29, CUSTOM_3F_C0_A);
                break;
            case 4026:
                ir_brand = (uint8_t *)KEY_BUF_0B_0F;
                ir_type_init(MN6014C5D638_30, CUSTOM_00_FF_A);
                break;
            case 226:
                ir_brand = (uint8_t *)KEY_BUF_20_2A;
                ir_type_init(MN6014C5D638_30, CUSTOM_00_FF_A);
                break;
            case 890:
                ir_brand = (uint8_t *)KEY_BUF_C0;
                ir_type_init(N3201_32, CUSTOM_00_2B_A);
                break;
            case 1310:
                ir_brand = (uint8_t *)KEY_BUF_3D_03;
                ir_type_init(PANPROJECTOR_37, CUSTOM_2002_4880_A);
                break;
            case 1335:
                ir_brand = (uint8_t *)KEY_BUF_3D_6D;
                ir_type_init(PANPROJECTOR_37, CUSTOM_2002_4880_A);
                break;
            case 605:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_10;
                ir_type_init(PHILIPS_20_38, HIGH_CUSTOM_8000_0000_A);
                break;
            case 1744:
            case 1867:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_10;
                ir_type_init(PHILIPS_20_38, HIGH_CUSTOM_8000_0000_A);
                break;
            case 1887:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_10_11_0D;
                ir_type_init(PHILIPS_20_38, HIGH_CUSTOM_8000_0000_A);
                break;
            case 70:
                ir_brand = (uint8_t *)KEY_BUF_02_02;
                ir_type_init(PILOT370_40, CUSTOM_7D_00_A);
                break;
            case 166:
            case 679:
            case 1260:
                ir_brand = (uint8_t *)KEY_BUF_1C;
                ir_type_init(PIONEER_41, CUSTOM_AA_55_A);
                break;
            case 760:
                ir_brand = (uint8_t *)KEY_BUF_1B;
                ir_type_init(PIONEER_41, CUSTOM_AA_55_A);
                break;
            case 47:
            case 1447:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_10;
                ir_type_init(RCA_56_42, CUSTOM_FFFD_0000_A);
                break;
            case 2675:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_2A_A3;
                ir_type_init(RCA_38_43, HIGH_CUSTOM_FFFD_E000_A);
                break;
            case 2154:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0D_0E;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_FFFD_6000_A);
                break;
            case 1871:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0E;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_FFFD_E000_A);
                break;
            case 4030:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_0E;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_8000_0000_A);
                break;
            case 4011:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_3B;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_8000_0000_A);
                break;
            case 4031:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_11;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_8000_0000_A);
                break;
            case 4027:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_32_36;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_8000_0000_A);
                break;
            case 4012:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_FF_0E;
                ir_type_init(SAA3010_38_45, HIGH_CUSTOM_8000_0000_A);
                break;
            case 37:
            case 499:
            case 556:
            case 655:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1884:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_00;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 2422:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_00_10;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 790:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_0A;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1183:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_1A;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 2265:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_1B;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1617:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_2B;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 744:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_30;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 633:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_31;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;

            case 1037:
            case 1039:
            case 1232:
            case 1308:
            case 1314:
            case 1388:
            case 1454:
            case 1455:
            case 1474:
            case 1495:
            case 1506:
            case 1555:
            case 1556:
            case 1583:
            case 1585:
            case 1637:
            case 1982:
            case 2015:
            case 2125:
            case 2272:
            case 2276:
            case 2279:
            case 2463:
            case 2663:
            case 2890:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_10_11;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1983:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_12_11;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 2200:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_12_13;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 512:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_3A;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 2053:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_3C;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1137:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_FF;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1081:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0F;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 714:
            case 1289:
            case 1652:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_20_2A;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0000_A);
                break;
            case 1149:
            case 1163:
            case 1667:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0C_38_10_11;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0800_A);
                break;
            case 648:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0D_00;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_0800_A);
                break;
            case 1338:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0D_0E;
                ir_type_init(SAA3010_36_46, HIGH_CUSTOM_C000_6000_0C_A);
                break;

            case 93:
            case 818:
            case 1193:
            case 1393:
            case 1659:
            case 2810:
                ir_brand = (uint8_t *)KEY_BUF_16;
                ir_type_init(LX0773CE_50, CUSTOM_01_01_A);
                break;
            case 852:
                ir_brand = (uint8_t *)KEY_BUF_16_1A;
                ir_type_init(LX0773CE_50, CUSTOM_0D_0D_A);
                break;
            case 480:
                ir_brand = (uint8_t *)KEY_BUF_0C;
                ir_type_init(SINGH2150_51, CUSTOM_FFFE_00A4_A);
                break;
            case 631:
                ir_brand = (uint8_t *)KEY_BUF_0C_75;
                ir_type_init(SINGH2150_51, CUSTOM_FFFE_006A_A);
                break;
            case 1112:
                ir_brand = (uint8_t *)KEY_BUF_1D_16;
                ir_type_init(TC901_54, CUSTOM_08_08_A);
                break;
            case 2134:
                ir_brand = (uint8_t *)KEY_BUF_0B_10;
                ir_type_init(TC901_54, CUSTOM_08_08_A);
                break;
            case 68:
                ir_brand = (uint8_t *)KEY_BUF_0F;
                ir_type_init(TC901_54, CUSTOM_08_08_A);
                break;
            case 180:
                ir_brand = (uint8_t *)KEY_BUF_0F_12;
                ir_type_init(TC901_54, CUSTOM_08_08_A);
                break;
            case 696:
                ir_brand = (uint8_t *)KEY_BUF_13;
                ir_type_init(TC901_54, CUSTOM_0A_0A_A);
                break;
            case 796:
                ir_brand = (uint8_t *)KEY_BUF_13_19;
                ir_type_init(TC901_54, CUSTOM_0A_0A_A);
                break;
            case 371:
                ir_brand = (uint8_t *)KEY_BUF_0A_0C;
                ir_type_init(TC901_54, CUSTOM_0B_0B_A);
                break;
            case 698:
            case 706:
                ir_brand = (uint8_t *)KEY_BUF_0B_0F;
                ir_type_init(TC901_54, CUSTOM_0E_0E_A);
                break;
            case 4007:
                ir_brand = (uint8_t *)KEY_BUF_0C_0F_14_15;
                ir_type_init(TC901_54, CUSTOM_0E_0E_A);
                break;
            case 264:
                ir_brand = (uint8_t *)KEY_BUF_14;
                ir_type_init(TC901_54, CUSTOM_0E_0E_A);
                break;
            case 412:
                ir_brand = (uint8_t *)KEY_BUF_14_16_1C;
                ir_type_init(TC901_54, CUSTOM_0E_0E_A);
                break;
            case 1682:
                ir_brand = (uint8_t *)KEY_BUF_0C_0F;
                ir_type_init(TC901_54, CUSTOM_1E_1E_A);
                break;
            case 1560:
            case 1615:
                ir_brand = (uint8_t *)KEY_BUF_0B_15;
                ir_type_init(TC901_54, CUSTOM_18_18_A);
                break;
            case 4002:
                ir_brand = (uint8_t *)KEY_BUF_0A_0C_15;
                ir_type_init(TC901_54, CUSTOM_40_40_A);
                break;
            case 2155:
                ir_brand = (uint8_t *)KEY_BUF_0A_40_15_1C;
                ir_type_init(TC901_54, CUSTOM_40_40_A);
                break;
            case 4001:
                ir_brand = (uint8_t *)KEY_BUF_0A_41_15_1C;
                ir_type_init(TC901_54, CUSTOM_40_40_A);
                break;
            case 819:
                ir_brand = (uint8_t *)KEY_BUF_14;
                ir_type_init(TC901_54, CUSTOM_40_40_A);
                break;
            case 602:
                ir_brand = (uint8_t *)KEY_BUF_13;
                ir_type_init(TC901_54, CUSTOM_80_80_A);
                break;
            case 60:
            case 618:
            case 644:
            case 812:
            case 1235:
            case 1249:
            case 1312:
            case 1458:
            case 1619:
            case 1630:
            case 1678:
            case 2051:
            case 2094:
            case 2097:
            case 2103:
            case 2137:
                ir_brand = (uint8_t *)KEY_BUF_02_01;
                ir_type_init(TC901_D8B8_55, CUSTOM_07_07_A);
                break;
            case 1584:
                ir_brand = (uint8_t *)KEY_BUF_02_1B;
                ir_type_init(TC901_D8B8_55, CUSTOM_07_07_A);
                break;
            case 1719:
                ir_brand = (uint8_t *)KEY_BUF_02_78;
                ir_type_init(TC901_D8B8_55, CUSTOM_07_07_A);
                break;
            case 1720:
                ir_brand = (uint8_t *)KEY_BUF_02_78_72_73_23;
                ir_type_init(TC901_D8B8_55, CUSTOM_07_07_A);
                break;
            case 625:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_18_29_2A;
                ir_type_init(RCT311_56, HIGH_CUSTOM_3000_0000_A);
                break;
            case 2715:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1D_43;
                ir_type_init(RCT311_56, HIGH_CUSTOM_3000_0000_A);
                break;
            case 2022:
                ir_brand = (uint8_t *)KEY_BUF_01_16;
                ir_type_init(UPD6121G_64, CUSTOM_00_1F_A);
                break;
            case 2807:
                ir_brand = (uint8_t *)KEY_BUF_4E;
                ir_type_init(UPD6121G_64, CUSTOM_00_30_A);
                break;
            case 1709:
            case 1775:
            case 2223:
                ir_brand = (uint8_t *)KEY_BUF_0A_53;
                ir_type_init(UPD6121G_64, CUSTOM_00_7F_A);
                break;
            case 2104:
            case 2106:
            case 2426:
                ir_brand = (uint8_t *)KEY_BUF_0A_53_50;
                ir_type_init(UPD6121G_64, CUSTOM_00_7F_A);
                break;
            case 1531:
                ir_brand = (uint8_t *)KEY_BUF_0A_1B;
                ir_type_init(UPD6121G_64, CUSTOM_00_7F_A);
                break;
            case 4016:
                ir_brand = (uint8_t *)KEY_BUF_15_12;
                ir_type_init(UPD6121G_64, CUSTOM_00_7F_A);
                break;
            case 2797:
                ir_brand = (uint8_t *)KEY_BUF_1E_1C;
                ir_type_init(UPD6121G_64, CUSTOM_00_7F_A);
                break;
            case 1295:
            case 1535:
            case 1539:
            case 2057:
                ir_brand = (uint8_t *)KEY_BUF_00_50;
                ir_type_init(UPD6121G_64, CUSTOM_00_80_A);
                break;
            case 4023:
                ir_brand = (uint8_t *)KEY_BUF_11_52;
                ir_type_init(UPD6121G_64, CUSTOM_00_AF_A);
                break;
            case 1089:
                ir_brand = (uint8_t *)KEY_BUF_01_06_0C;
                ir_type_init(UPD6121G_64, CUSTOM_00_BD_A);
                break;
            case 2214:
            case 2402:
                ir_brand = (uint8_t *)KEY_BUF_01_08_0C;
                ir_type_init(UPD6121G_64, CUSTOM_00_BD_A);
                break;
            case 1588:
                ir_brand = (uint8_t *)KEY_BUF_01_46;
                ir_type_init(UPD6121G_64, CUSTOM_00_BD_A);
                break;
            case 2021:
                ir_brand = (uint8_t *)KEY_BUF_05_1F;
                ir_type_init(UPD6121G_64, CUSTOM_00_BE_A);
                break;
            case 4005:
                ir_brand = (uint8_t *)KEY_BUF_03_1E;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 2333:
                ir_brand = (uint8_t *)KEY_BUF_03_44;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 1363:
                ir_brand = (uint8_t *)KEY_BUF_10_40;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 2037:
                ir_brand = (uint8_t *)KEY_BUF_0D_12;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 2098:
                ir_brand = (uint8_t *)KEY_BUF_0D_12_18;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 2399:
                ir_brand = (uint8_t *)KEY_BUF_0D_12_44;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 4022:
                ir_brand = (uint8_t *)KEY_BUF_56;
                ir_type_init(UPD6121G_64, CUSTOM_00_BF_A);
                break;
            case 1964:
                ir_brand = (uint8_t *)KEY_BUF_11_03;
                ir_type_init(UPD6121G_64, CUSTOM_00_EF_A);
                break;
            case 1994:
                ir_brand = (uint8_t *)KEY_BUF_11_0B;
                ir_type_init(UPD6121G_64, CUSTOM_00_EF_A);
                break;
            case 2032:
            case 2100:
            case 2241:
            case 2457:
                ir_brand = (uint8_t *)KEY_BUF_1C_17;
                ir_type_init(UPD6121G_64, CUSTOM_00_DF_A);
                break;
            case 435:
                ir_brand = (uint8_t *)KEY_BUF_00_1D;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 2168:
                ir_brand = (uint8_t *)KEY_BUF_00_4C;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 1511:
                ir_brand = (uint8_t *)KEY_BUF_01_0C;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 2604:
                ir_brand = (uint8_t *)KEY_BUF_DB_1D;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 1900:
                ir_brand = (uint8_t *)KEY_BUF_5B;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 2056:
                ir_brand = (uint8_t *)KEY_BUF_5B_5C;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 2050:
                ir_brand = (uint8_t *)KEY_BUF_DB_5C;
                ir_type_init(UPD6121G_64, CUSTOM_00_FF_A);
                break;
            case 4033:
                ir_brand = (uint8_t *)KEY_BUF_00_03_12;
                ir_type_init(UPD6121G_64, CUSTOM_01_FE_A);
                break;
            case 1852:
                ir_brand = (uint8_t *)KEY_BUF_0C_0F_12_13;
                ir_type_init(UPD6121G_64, CUSTOM_01_FE_A);
                break;
            case 2245:
            case 2561:
                ir_brand = (uint8_t *)KEY_BUF_10_12;
                ir_type_init(UPD6121G_64, CUSTOM_01_FE_A);
                break;
            case 2001:
                ir_brand = (uint8_t *)KEY_BUF_11_13_13;
                ir_type_init(UPD6121G_64, CUSTOM_01_FE_A);
                break;
            case 2748:
                ir_brand = (uint8_t *)KEY_BUF_03_27;
                ir_type_init(UPD6121G_64, CUSTOM_00_02_A);
                break;
            case 1701:
            case 2342:
                ir_brand = (uint8_t *)KEY_BUF_40;
                ir_type_init(UPD6121G_64, CUSTOM_02_BD_A);
                break;
            case 887:
                ir_brand = (uint8_t *)KEY_BUF_01_06;
                ir_type_init(UPD6121G_64, CUSTOM_02_F5_A);
                break;
            case 1546:
                ir_brand = (uint8_t *)KEY_BUF_5F;
                ir_type_init(UPD6121G_64, CUSTOM_03_AC_A);
                break;
            case 1606:
                ir_brand = (uint8_t *)KEY_BUF_11_13;
                ir_type_init(UPD6121G_64, CUSTOM_03_FC_A);
                break;
            case 672:
                ir_brand = (uint8_t *)KEY_BUF_0F_12;
                ir_type_init(UPD6121G_64, CUSTOM_04_10_A);
                break;
            case 4028:
                ir_brand = (uint8_t *)KEY_BUF_0F_12_0D_0C;
                ir_type_init(UPD6121G_64, CUSTOM_04_10_A);
                break;
            case 1610:
                ir_brand = (uint8_t *)KEY_BUF_00_1D_16;
                ir_type_init(UPD6121G_64, CUSTOM_04_B9_A);
                break;
            case 178:
            case 1265:
            case 1305:
            case 1319:
            case 1423:
            case 1721:
            case 1840:
            case 1842:
            case 1859:
            case 1860:
            case 2182:
            case 2362:
            case 2731:
                ir_brand = (uint8_t *)KEY_BUF_08_0B;
                ir_type_init(UPD6121G_64, CUSTOM_04_FB_A);
                break;
            case 1727:
            case 1865:
                ir_brand = (uint8_t *)KEY_BUF_08_25;
                ir_type_init(UPD6121G_64, CUSTOM_04_FB_A);
                break;
            case 2212:
                ir_brand = (uint8_t *)KEY_BUF_08_43;
                ir_type_init(UPD6121G_64, CUSTOM_04_FB_A);
                break;
            case 1768:
                ir_brand = (uint8_t *)KEY_BUF_08_98;
                ir_type_init(UPD6121G_64, CUSTOM_04_FB_A);
                break;
            case 1846:
            case 2668:
                ir_brand = (uint8_t *)KEY_BUF_0C_0D;
                ir_type_init(UPD6121G_64, CUSTOM_04_FB_A);
                break;
            case 1820:
                ir_brand = (uint8_t *)KEY_BUF_0D_0C;
                ir_type_init(UPD6121G_64, CUSTOM_04_FB_A);
                break;
            case 877:
                ir_brand = (uint8_t *)KEY_BUF_0E_02;
                ir_type_init(UPD6121G_64, CUSTOM_06_FB_A);
                break;
            case 1783:
                ir_brand = (uint8_t *)KEY_BUF_0E_40;
                ir_type_init(UPD6121G_64, CUSTOM_06_FB_A);
                break;
            case 1644:
                ir_brand = (uint8_t *)KEY_BUF_87_8C;
                ir_type_init(UPD6121G_64, CUSTOM_08_13_A);
                break;
            case 1523:
            case 1645:
            case 1687:
            case 2127:
                ir_brand = (uint8_t *)KEY_BUF_05_40;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 4020:
                ir_brand = (uint8_t *)KEY_BUF_05_44;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 4015:
                ir_brand = (uint8_t *)KEY_BUF_0A_53_F9;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 4025:
                ir_brand = (uint8_t *)KEY_BUF_0B_0F;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 2129:
            case 2270:
                ir_brand = (uint8_t *)KEY_BUF_0C_0D_10;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 3024:
                ir_brand = (uint8_t *)KEY_BUF_0C_0D_1A;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 4017:
                ir_brand = (uint8_t *)KEY_BUF_0C_1B;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 1326:
                ir_brand = (uint8_t *)KEY_BUF_12;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 2020:
                ir_brand = (uint8_t *)KEY_BUF_12_0E;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 2718:
            case 2719:
                ir_brand = (uint8_t *)KEY_BUF_D7_C3;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 4021:
                ir_brand = (uint8_t *)KEY_BUF_D7_DC;
                ir_type_init(UPD6121G_64, CUSTOM_08_F7_A);
                break;
            case 753:
                ir_brand = (uint8_t *)KEY_BUF_12_14;
                ir_type_init(UPD6121G_64, CUSTOM_0A_00_A);
                break;
            case 882:
                ir_brand = (uint8_t *)KEY_BUF_14_03;
                ir_type_init(UPD6121G_64, CUSTOM_0E_09_A);
                break;
            case 1776:
            case 2218:
                ir_brand = (uint8_t *)KEY_BUF_1F_1C;
                ir_type_init(UPD6121G_64, CUSTOM_10_2D_A);
                break;
            case 820:
                ir_brand = (uint8_t *)KEY_BUF_D7_80;
                ir_type_init(UPD6121G_64, CUSTOM_10_EF_A);
                break;
            case 2028:
                ir_brand = (uint8_t *)KEY_BUF_1F_12;
                ir_type_init(UPD6121G_64, CUSTOM_10_ED_A);
                break;
            case 1521:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_1B;
                ir_type_init(UPD6121G_64, CUSTOM_12_12_A);
                break;
            case 170:
            case 1270:
                ir_brand = (uint8_t *)KEY_BUF_08_0A;
                ir_type_init(UPD6121G_64, CUSTOM_18_E7_A);
                break;
            case 245:
                ir_brand = (uint8_t *)KEY_BUF_08_4D;
                ir_type_init(UPD6121G_64, CUSTOM_18_E9_A);
                break;
            case 2144:
                ir_brand = (uint8_t *)KEY_BUF_14_03_FF;
                ir_type_init(UPD6121G_64, CUSTOM_18_E9_A);
                break;
            case 1598:
                ir_brand = (uint8_t *)KEY_BUF_0A_12;
                ir_type_init(UPD6121G_64, CUSTOM_1C_E3_A);
                break;
            case 1298:
                ir_brand = (uint8_t *)KEY_BUF_0A_40;
                ir_type_init(UPD6121G_64, CUSTOM_1C_E3_A);
                break;
            case 1413:
            case 2329:
            case 2331:
            case 2332:
                ir_brand = (uint8_t *)KEY_BUF_12;
                ir_type_init(UPD6121G_64, CUSTOM_1C_E3_A);
                break;
            case 865:
                ir_brand = (uint8_t *)KEY_BUF_00_04;
                ir_type_init(UPD6121G_64, CUSTOM_20_40_A);
                break;
            case 880:
                ir_brand = (uint8_t *)KEY_BUF_00_03;
                ir_type_init(UPD6121G_64, CUSTOM_20_DF_A);
                break;
            case 1702:
                ir_brand = (uint8_t *)KEY_BUF_48;
                ir_type_init(UPD6121G_64, CUSTOM_20_DF_A);
                break;
            case 4006:
                ir_brand = (uint8_t *)KEY_BUF_52;
                ir_type_init(UPD6121G_64, CUSTOM_20_DF_A);
                break;
            case 2353:
                ir_brand = (uint8_t *)KEY_BUF_00_20;
                ir_type_init(UPD6121G_64, CUSTOM_22_11_A);
                break;
            case 4024:
                ir_brand = (uint8_t *)KEY_BUF_08_0B;
                ir_type_init(UPD6121G_64, CUSTOM_24_DB_A);
                break;
            case 1649:
                ir_brand = (uint8_t *)KEY_BUF_00_3D;
                ir_type_init(UPD6121G_64, CUSTOM_33_00_A);
                break;
            case 1348:
            case 1351:
            case 2817:
            case 2818:
                ir_brand = (uint8_t *)KEY_BUF_83;
                ir_type_init(UPD6121G_64, CUSTOM_33_CC_A);
                break;
            case 2027:
                ir_brand = (uint8_t *)KEY_BUF_83_C3;
                ir_type_init(UPD6121G_64, CUSTOM_33_CC_A);
                break;
            case 154:
                ir_brand = (uint8_t *)KEY_BUF_12_13;
                ir_type_init(UPD6121G_64, CUSTOM_38_C7_A);
                break;
            case 45:
                ir_brand = (uint8_t *)KEY_BUF_10;
                ir_type_init(UPD6121G_64, CUSTOM_38_C7_A);
                break;
            case 208:
            case 339:
            case 1208:
                ir_brand = (uint8_t *)KEY_BUF_1C_14;
                ir_type_init(UPD6121G_64, CUSTOM_38_C7_A);
                break;
            case 876:
            case 1784:
                ir_brand = (uint8_t *)KEY_BUF_08_0B;
                ir_type_init(UPD6121G_64, CUSTOM_40_02_A);
                break;
            case 1663:
                ir_brand = (uint8_t *)KEY_BUF_08_0B;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 4010:
                ir_brand = (uint8_t *)KEY_BUF_03_1E;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 1732:
                ir_brand = (uint8_t *)KEY_BUF_04;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 35:
            case 508:
            case 767:
            case 821:
            case 1072:
            case 1243:
            case 1508:
            case 1567:
            case 2598:
                ir_brand = (uint8_t *)KEY_BUF_12;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 1156:
            case 1656:
            case 2732:
                ir_brand = (uint8_t *)KEY_BUF_12_0F;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 2795:
            case 4013:
                ir_brand = (uint8_t *)KEY_BUF_12_14_0E;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 1005:
                ir_brand = (uint8_t *)KEY_BUF_12_14_1B;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 2197:
                ir_brand = (uint8_t *)KEY_BUF_17_11;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 1988:
                ir_brand = (uint8_t *)KEY_BUF_17_13;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 1779:
                ir_brand = (uint8_t *)KEY_BUF_17_FF;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 4008:
                ir_brand = (uint8_t *)KEY_BUF_40_4D;
                ir_type_init(UPD6121G_64, CUSTOM_40_BF_A);
                break;
            case 2131:
                ir_brand = (uint8_t *)KEY_BUF_08_23;
                ir_type_init(UPD6121G_64, CUSTOM_4B_B4_A);
                break;
            case 1754:
                ir_brand = (uint8_t *)KEY_BUF_02_1D_FF;
                ir_type_init(UPD6121G_64, CUSTOM_4F_50_A);
                break;
            case 1259:
                ir_brand = (uint8_t *)KEY_BUF_01_08;
                ir_type_init(UPD6121G_64, CUSTOM_42_BD_A);
                break;
            case 1248:
                ir_brand = (uint8_t *)KEY_BUF_01_50;
                ir_type_init(UPD6121G_64, CUSTOM_42_BD_A);
                break;
            case 1563:
                ir_brand = (uint8_t *)KEY_BUF_01_FF;
                ir_type_init(UPD6121G_64, CUSTOM_42_BD_A);
                break;
            case 1376:
                ir_brand = (uint8_t *)KEY_BUF_0B_0C;
                ir_type_init(UPD6121G_64, CUSTOM_42_FD_A);
                break;
            case 1281:
                ir_brand = (uint8_t *)KEY_BUF_10_15;
                ir_type_init(UPD6121G_64, CUSTOM_45_16_A);
                break;
            case 1468:
                ir_brand = (uint8_t *)KEY_BUF_10_17;
                ir_type_init(UPD6121G_64, CUSTOM_45_16_A);
                break;
            case 797:
                ir_brand = (uint8_t *)KEY_BUF_00_0B;
                ir_type_init(UPD6121G_64, CUSTOM_50_AD_A);
                break;
            case 225:
                ir_brand = (uint8_t *)KEY_BUF_17_06;
                ir_type_init(UPD6121G_64, CUSTOM_50_AF_A);
                break;
            case 481:
            case 576:
            case 719:
            case 1045:
            case 1481:
            case 1691:
            case 1854:
            case 2207:
                ir_brand = (uint8_t *)KEY_BUF_17_09;
                ir_type_init(UPD6121G_64, CUSTOM_50_AF_A);
                break;
            case 1576:
                ir_brand = (uint8_t *)KEY_BUF_17_09_72;
                ir_type_init(UPD6121G_64, CUSTOM_50_AF_A);
                break;
            case 1484:
            case 1772:
                ir_brand = (uint8_t *)KEY_BUF_17_2E;
                ir_type_init(UPD6121G_64, CUSTOM_50_AF_A);
                break;
            case 578:
                ir_brand = (uint8_t *)KEY_BUF_17_44;
                ir_type_init(UPD6121G_64, CUSTOM_50_AF_A);
                break;
            case 2390:
                ir_brand = (uint8_t *)KEY_BUF_00_10_42;
                ir_type_init(UPD6121G_64, CUSTOM_60_9F_A);
                break;
            case 1574:
                ir_brand = (uint8_t *)KEY_BUF_00_10;
                ir_type_init(UPD6121G_64, CUSTOM_60_9F_A);
                break;
            case 1756:
                ir_brand = (uint8_t *)KEY_BUF_03;
                ir_type_init(UPD6121G_64, CUSTOM_60_9F_A);
                break;
            case 1562:
                ir_brand = (uint8_t *)KEY_BUF_00_45;
                ir_type_init(UPD6121G_64, CUSTOM_60_9F_A);
                break;
            case 1339:
            case 2190:
                ir_brand = (uint8_t *)KEY_BUF_01_22;
                ir_type_init(UPD6121G_64, CUSTOM_61_63_A);
                break;
            case 1935:
                ir_brand = (uint8_t *)KEY_BUF_4A_73;
                ir_type_init(UPD6121G_64, CUSTOM_71_8E_A);
                break;
            case 1954:
                ir_brand = (uint8_t *)KEY_BUF_4A_76;
                ir_type_init(UPD6121G_64, CUSTOM_71_8E_A);
                break;
            case 1899:
                ir_brand = (uint8_t *)KEY_BUF_80;
                ir_type_init(UPD6121G_64, CUSTOM_77_88_A);
                break;
            case 355:
                ir_brand = (uint8_t *)KEY_BUF_0A_18;
                ir_type_init(UPD6121G_64, CUSTOM_80_57_A);
                break;
            case 443:
            case 2005:
            case 2007:
                ir_brand = (uint8_t *)KEY_BUF_0F_12;
                ir_type_init(UPD6121G_64, CUSTOM_80_63_A);
                break;
            case 1908:
                ir_brand = (uint8_t *)KEY_BUF_49;
                ir_type_init(UPD6121G_64, CUSTOM_80_64_A);
                break;
            case 1916:
                ir_brand = (uint8_t *)KEY_BUF_49_00;
                ir_type_init(UPD6121G_64, CUSTOM_80_64_A);
                break;
            case 235:
                ir_brand = (uint8_t *)KEY_BUF_0A_18;
                ir_type_init(UPD6121G_64, CUSTOM_80_6F_A);
                break;
            case 1847:
                ir_brand = (uint8_t *)KEY_BUF_82_CC;
                ir_type_init(UPD6121G_64, CUSTOM_80_7F_A);
                break;
            case 1849:
            case 2450:
            case 2767:
                ir_brand = (uint8_t *)KEY_BUF_82_93;
                ir_type_init(UPD6121G_64, CUSTOM_80_7F_A);
                break;
            case 2673:
                ir_brand = (uint8_t *)KEY_BUF_C2;
                ir_type_init(UPD6121G_64, CUSTOM_80_7F_A);
                break;
            case 830:
                ir_brand = (uint8_t *)KEY_BUF_12_25;
                ir_type_init(UPD6121G_64, CUSTOM_82_F3_A);
                break;
            case 1290:
                ir_brand = (uint8_t *)KEY_BUF_90_9D;
                ir_type_init(UPD6121G_64, CUSTOM_83_55_A);
                break;
            case 1542:
                ir_brand = (uint8_t *)KEY_BUF_0A_1E;
                ir_type_init(UPD6121G_64, CUSTOM_83_F7_A);
                break;
            case 1817:
                ir_brand = (uint8_t *)KEY_BUF_20_40;
                ir_type_init(UPD6121G_64, CUSTOM_84_E0_A);
                break;
            case 610:
                ir_brand = (uint8_t *)KEY_BUF_12_00;
                ir_type_init(UPD6121G_64, CUSTOM_85_46_A);
                break;
            case 463:
                ir_brand = (uint8_t *)KEY_BUF_0F_12;
                ir_type_init(UPD6121G_64, CUSTOM_86_05_A);
                break;
            case 1343:
                ir_brand = (uint8_t *)KEY_BUF_12_02;
                ir_type_init(UPD6121G_64, CUSTOM_86_6B_A);
                break;
            case 1963:
                ir_brand = (uint8_t *)KEY_BUF_E0;
                ir_type_init(UPD6121G_64, CUSTOM_87_22_A);
                break;
            case 1430:
                ir_brand = (uint8_t *)KEY_BUF_17_10;
                ir_type_init(UPD6121G_64, CUSTOM_87_4E_A);
                break;
            case 832:
                ir_brand = (uint8_t *)KEY_BUF_12_14_1A;
                ir_type_init(UPD6121G_64, CUSTOM_E7_0A_A);
                break;
            case 1612:
                ir_brand = (uint8_t *)KEY_BUF_12_09;
                ir_type_init(UPD6121G_64, CUSTOM_95_6A_A);
                break;
            case 2132:
                ir_brand = (uint8_t *)KEY_BUF_08_23;
                ir_type_init(UPD6121G_64, CUSTOM_96_69_A);
                break;
            case 1771:
                ir_brand = (uint8_t *)KEY_BUF_1F_0C;
                ir_type_init(UPD6121G_64, CUSTOM_E5_70_A);
                break;
            case 4009:
                ir_brand = (uint8_t *)KEY_BUF_1C_1A;
                ir_type_init(UPD6121G_64, CUSTOM_A0_5F_A);
                break;
            case 2254:
                ir_brand = (uint8_t *)KEY_BUF_C0_9C;
                ir_type_init(UPD6121F_LIAN_66, CUSTOM_00_2B_A);
                break;
            case 1217:
                ir_brand = (uint8_t *)KEY_BUF_C0_C9;
                ir_type_init(UPD6121F_LIAN_66, CUSTOM_00_2B_A);
                break;
            case 2108:
                ir_brand = (uint8_t *)KEY_BUF_0F_12_53;
                ir_type_init(UPD6121F_LIAN_66, CUSTOM_80_63_A);
                break;
            case 2101:
            case 2111:
                ir_brand = (uint8_t *)KEY_BUF_0A_1E;
                ir_type_init(UPD6121F_LIAN_66, CUSTOM_83_F1_A);
                break;

            case 1457:
                ir_brand = (uint8_t *)KEY_BUF_1C;
                ir_type_init(UPD6121F_LIAN_66, CUSTOM_AA_55_A);
                break;
            case 4019:
                ir_brand = (uint8_t *)KEY_BUF_1C_30;
                ir_type_init(UPD6121G_001_67, CUSTOM_35_50_A);
                break;
            case 1795:
            case 1812:
                ir_brand = (uint8_t *)KEY_BUF_1C_52;
                ir_type_init(UPD6121G_001_67, CUSTOM_35_50_A);
                break;
            case 2274:
            case 2714:
                ir_brand = (uint8_t *)KEY_BUF_1C_53;
                ir_type_init(UPD6121G_001_67, CUSTOM_35_50_A);
                break;
            case 0:
            case 10:
            case 810:
            case 1505:
            case 1625:
            case 1651:
            case 1751:
            case 1825:
            case 2778:
                ir_brand = (uint8_t *)KEY_BUF_15;
                ir_type_init(UPD6124_D7C5_69, CUSTOM_01_00_A);
                break;
            case 1666:
            case 2072:
                ir_brand = (uint8_t *)KEY_BUF_15_5B;
                ir_type_init(UPD6124_D7C5_69, CUSTOM_01_00_A);
                break;

            case 1685:
                ir_brand = (uint8_t *)KEY_BUF_15_40;
                ir_type_init(UPD6124_D7C13_71, CUSTOM_0057_0000_A);
                break;
            case 53:
            case 653:
            case 2578:
                ir_brand = (uint8_t *)KEY_BUF_17;
                ir_type_init(VICTOR_C8D8_72, CUSTOM_03_00_A);
                break;
            case 1371:
                ir_brand = (uint8_t *)KEY_BUF_0F_14;
                ir_type_init(XY_2106_75, CUSTOM_08_0B_A);
                break;
            case 447:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_0B;
                ir_type_init(ZENITH_S10_77, DEFAULT_CUSTOMER);
                break;
            case 746:
                ir_brand = (uint8_t *)KEY_BUF_4A;
                ir_type_init(ZENITH_S10_77_Zero, DEFAULT_CUSTOMER);
                break;
            case 73:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_30_21;
                ir_type_init(ZENITHSIM_C2D8_82, CUSTOM_00_00_A);
                break;
            case 394:
                ir_brand = (uint8_t *)HIGH_KEY_BUF_06;
                ir_type_init(Unknow_C8D5_83, HIGH_CUSTOM_8100_7E00_A);
                break;
            
            default:
                ir_brand = (uint8_t *)KEY_BUF_E0;
                ir_type_init(UPD6121G_64, CUSTOM_87_22_A);
                break;
        }
    }

    return ir_AV;
}

ir_brand_init_t brand_DVB_init(uint16_t brand)
{
    switch (brand)
    {
        case 3220:
            ir_type_init(UPD6121G_64, CUSTOM_00_FD_A);
            break;
        case 3221:
            ir_type_init(UPD6121G_64, CUSTOM_20_FD_A);
            break;
        case 3222:
            ir_type_init(UPD6121G_64, CUSTOM_88_45_A);
            break;
        case 3223:
            ir_type_init(UPD6121G_64, CUSTOM_5A_49_A);
            break;
        case 3224:
            ir_type_init(UPD6121G_64, CUSTOM_69_F1_A);
            break;

    default:
            ir_type_init(UPD6121G_64, CUSTOM_69_F1_A);
            break;
    }

    return ir_DVB;
}
