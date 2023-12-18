#include "att_list.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED};
static const uint8_t WRITE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_INDICATE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|INDICATE_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};

static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {0x41,0x52,0x00};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0xC0,0x03};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t ppcp_value[] = {0x10,0x00,0x10,0x00,0x31,0x00,0xF4,0x01};

static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
/// PNP ID Value   01 0F 00 12 34 01 00
///功能键PRIME VIDEO,NETFLIX,DISNEP+,HULU
// #if defined(PRIMEVIDEO_NETFLIX_DISNEP_HULU)
// #define APP_DIS_PNP_ID ("\x02\x71\x01\x1e\x04\x37\x00") /// add 20220803此处有一个重大的BUG，每个型号此处的PNP必须一一对应，不然4个功能键无法使用
// #elif defined(PRIMEVIDEO_NETFLIX_APPS_AMAZONMUSIC)
// ///功能键PRIME VIDEO NETFLIX,APPS,AMAZON MUSIC ADD 20220819
// #define APP_DIS_PNP_ID ("\x02\x71\x01\x1E\x04\x3A\x00") /// add 20220803此处有一个重大的BUG，每个型号此处的PNP必须一一对应，不然4个功能键无法使用
// #elif defined(PRIMEVIDEO_NETFLIX_DAZN_AMAZONMUSIC)
// ///功能键PRIME VIDEO NETFLIX,DAZN,AMAZON MUSIC ADD 20220819
// #define APP_DIS_PNP_ID ("\x02\x71\x01\x1E\x04\x39\x00") /// add 20220803此处有一个重大的BUG，每个型号此处的PNP必须一一对应，不然4个功能键无法使用
// #elif defined(PRIME_VIDEO_NETFLIX_DISNEP_AMAZONMUSIC)
// ///功能键PRIME VIDEO NETFLIX,DISNEP+,AMAZON MUSIC ADD 20220819
// #define APP_DIS_PNP_ID ("\x02\x71\x01\x1E\x04\x38\x00") /// add 20220803此处有一个重大的BUG，每个型号此处的PNP必须一一对应，不然4个功能键无法使用
// #elif defined(PRIME_VIDEO_NETFLIX_APPS_AMAZONSILK)
// ///功能键PRIME VIDEO NETFLIX,DISNEP+,AMAZON SILK ADD 20220819
// #define APP_DIS_PNP_ID ("\x02\x71\x01\x1E\x04\x3b\x00") /// add 20220803此处有一个重大的BUG，每个型号此处的PNP必须一一对应，不然4个功能键无法使用
// #endif
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
#if (Project_key == 603)
static const uint8_t pnp_id_value[] = {0x02,0x71,0x01,0x14,0x04,0x3C,0x00};
#elif (Project_key == 621)
static const uint8_t pnp_id_value[] = {0x02,0x71,0x01,0x14,0x04,0x16,0x00};//PRIMEVIDEO_NETFLIX_hbo_HULU
#elif (Project_key == 625)
static const uint8_t pnp_id_value[] = {0x02,0x71,0x01,0x14,0x04,0x12,0x00};//PRIMEVIDEO_NETFLIX_hbo_vue
// static const uint8_t pnp_id_value[] = {0x02,0x71,0x01,0x18,0x04,0x43,0x00};//PRIMEVIDEO_NETFLIX_hbo_vue
#endif
static const uint8_t CHRCTR_MANUFACTURE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t manufacture_name_value[] = {"Amazon Remote"};
static const uint8_t CHRCTR_SOFTWARE[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t software_value[] = {"123"};
static const uint8_t CHRCTR_HARDWARE[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t hardware_value[] = {"104"};
static const uint8_t CHRCTR_FIRMWARE[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t firmware_value[] = {"52509901"};//版本号199
static const uint8_t CHRCTR_SERIAL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t serial_number_value[] = {"G2P1VA01129755U1"};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {"M123456789"};
static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t system_id_value[] = {"1234567890"};

static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static const uint8_t battery_rr[] = {0x19, 0x2A};
static uint8_t battery_leval_value[] = {100};
static uint8_t battery_ccc[2];

static const uint8_t SERVICE_SCAN_PARAM[] = {UUID2ARRAY(UUID_SERVICE_SCAN_PARAM)};
static const uint8_t CHRCTR_SCAN_INTERVAL_WINDOW[] = {UUID2ARRAY(UUID_SCAN_INTERVAL_WINDOW)};
static uint8_t scan_param_value[2];

static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t hid_include_value[] = {0x40,0x00,0x44,0x00,0x0F,0x18};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[]= {0x00, 0x01, 0x00, 0x00};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = {
    0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07, 0x85, 0x01, 0x95, 0x03, 0x75,
	0x08, 0x15, 0x00, 0x25, 0xFF, 0x19, 0x00, 0x29, 0xFF, 0x81, 0x00, 0xC0, 0x05,
	0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02, 0x95, 0x02, 0x75, 0x10, 0x15, 0x00,
	0x26, 0x9C, 0x02, 0x19, 0x00, 0x2A, 0x9C, 0x02, 0x81, 0x00, 0xC0, 0x06, 0xFF,
	0x00, 0x09, 0x00, 0xA1, 0x01, 0x85, 0xF0, 0x95, 0x50, 0x75, 0x08, 0x15, 0x00,
	0x25, 0xFF, 0x81, 0x00, 0x85, 0xF1, 0x95, 0x03, 0x75, 0x08, 0x09, 0x00, 0x81,
	0x02, 0x85, 0xF2, 0x95, 0x01, 0x75, 0x08, 0x09, 0x00, 0x91, 0x02, 0x85, 0xF3,
	0x95, 0x0A, 0x75, 0x08, 0x09, 0x00, 0x91, 0x02, 0x85, 0xEF, 0x95, 0x03, 0x75,
	0x08, 0x15, 0x00, 0x25, 0xFF, 0x19, 0x00, 0x29, 0xFF, 0x09, 0x01, 0x81, 0x00,
	0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x03, 0x05, 0x01, 0x09, 0x06,
	0xA1, 0x02, 0x05, 0x06, 0x09, 0x20, 0x15, 0x00, 0x26, 0x64, 0x00, 0x75, 0x08,
	0x95, 0x01, 0x81, 0x02, 0xC0, 0xC0
};
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x01, 0x01};
static const uint8_t report2_rr[] = {0x02, 0x01};
static const uint8_t report3_rr[] = {0xF0, 0x01};
static const uint8_t report4_rr[] = {0xF1, 0x01};
static const uint8_t report5_rr[] = {0xF2, 0x02};
static const uint8_t report6_rr[] = {0xF3, 0x02};
static const uint8_t report7_rr[] = {0xCC, 0x03};
static const uint8_t report8_rr[] = {0xEF, 0x01};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report8_ccc[2];
static uint8_t report_value[2];

static const uint8_t Unknown_Service_FE15[] = {0xD3, 0x93, 0xA8, 0x0C, 0xF3, 0x86, 0x77, 0x8B, 0xE6, 0x11, 0x8D, 0x5E, 0x00, 0x15, 0x15, 0xFE};
static const uint8_t CHRCTR_FE15_01[] = {0xD3, 0x93, 0xA8, 0x0C, 0xF3, 0x86, 0x77, 0x8B, 0xE6, 0x11, 0x8D, 0x5E, 0x01, 0x15, 0x15, 0xFE};
static const uint8_t CHRCTR_FE15_02[] = {0xD3, 0x93, 0xA8, 0x0C, 0xF3, 0x86, 0x77, 0x8B, 0xE6, 0x11, 0x8D, 0x5E, 0x02, 0x15, 0x15, 0xFE};
static const uint8_t CHRCTR_FE15_03[] = {0xD3, 0x93, 0xA8, 0x0C, 0xF3, 0x86, 0x77, 0x8B, 0xE6, 0x11, 0x8D, 0x5E, 0x03, 0x15, 0x15, 0xFE};
static const uint8_t CHRCTR_FE15_04[] = {0xD3, 0x93, 0xA8, 0x0C, 0xF3, 0x86, 0x77, 0x8B, 0xE6, 0x11, 0x8D, 0x5E, 0x04, 0x15, 0x15, 0xFE};
static uint8_t chrctr_fe15_01_ccc[2];
static uint8_t chrctr_fe15_03_ccc[2];
static const uint8_t chrctr_fe15_01_value[] = {0x02};
uint8_t chrctr_fe15_02_value[2];
static const uint8_t chrctr_fe15_03_value[] = {0x02};
uint8_t chrctr_fe15_04_value[2];

static const uint8_t Unknown_Service_5DE2[] = {0xd3, 0x93, 0xa8, 0x0c, 0xf3, 0x86, 0x77, 0x8b, 0xe6, 0x11, 0x8d, 0x5e, 0x00, 0x00, 0xe2, 0x5d};
static const uint8_t CHRCTR_5DE2_01[] = {0xd3, 0x93, 0xa8, 0x0c, 0xf3, 0x86, 0x77, 0x8b, 0xe6, 0x11, 0x8d, 0x5e, 0x17, 0x4a, 0xe2, 0x5d};
static const uint8_t CHRCTR_5DE2_02[] = {0xd3, 0x93, 0xa8, 0x0c, 0xf3, 0x86, 0x77, 0x8b, 0xe6, 0x11, 0x8d, 0x5e, 0x18, 0x4a, 0xd2, 0x5d};
static const uint8_t CHRCTR_5DE2_03[] = {0xd3, 0x93, 0xa8, 0x0c, 0xf3, 0x86, 0x77, 0x8b, 0xe6, 0x11, 0x8d, 0x5e, 0x19, 0x4a, 0xe2, 0x5d};
static uint8_t chrctr_5de2_02_ccc[2];
static uint8_t chrctr_5de2_03_ccc[2];
uint8_t chrctr_5de2_01_value[2];
uint8_t chrctr_5de2_02_value[2];
uint8_t chrctr_5de2_03_value[2];

static const uint8_t Unknown_Service_CFBF[] = {0x2d, 0x0a, 0xde, 0xec, 0xe3, 0x20, 0x43, 0xa0, 0x12, 0x49, 0x2c, 0x76, 0x00, 0xa0, 0xbf, 0xcf};
static const uint8_t CHRCTR_CFBF_01[] = {0x2d, 0x0a, 0xde, 0xec, 0xe3, 0x20, 0x43, 0xa0, 0x12, 0x49, 0x2c, 0x76, 0x01, 0xa0, 0xbf, 0xcf};
static const uint8_t CHRCTR_CFBF_02[] = {0x2d, 0x0a, 0xde, 0xec, 0xe3, 0x20, 0x43, 0xa0, 0x12, 0x49, 0x2c, 0x76, 0x02, 0xa0, 0xbf, 0xcf};
static const uint8_t CHRCTR_CFBF_03[] = {0x2d, 0x0a, 0xde, 0xec, 0xe3, 0x20, 0x43, 0xa0, 0x12, 0x49, 0x2c, 0x76, 0x03, 0xa0, 0xbf, 0xcf};
static const uint8_t CHRCTR_CFBF_04[] = {0x2d, 0x0a, 0xde, 0xec, 0xe3, 0x20, 0x43, 0xa0, 0x12, 0x49, 0x2c, 0x76, 0x04, 0xa0, 0xbf, 0xcf};
static uint8_t chrctr_cfbf_01_ccc[2];
static uint8_t chrctr_cfbf_03_ccc[2];
static uint8_t chrctr_cfbf_04_ccc[2];
uint8_t chrctr_cfbf_01_value[2] = {0x01};
uint8_t chrctr_cfbf_02_value[2];
uint8_t chrctr_cfbf_03_value[2];
uint8_t chrctr_cfbf_04_value[2];

static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];


const ATT_TABLE_TYPE Att_List[] =
{
	/* Generic Attribute */
    {1,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

    /* Generic Access */
    {20,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ACC},

	{21,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{22,2,CHRCTR_DEVICE_NAME,3,device_name},
	{23,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{24,2,CHRCTR_APPEARANCE,2,appearance_value},
	{25,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{26,2,CHRCTR_PPCP,8,ppcp_value},
	
    /* Device Information */
	{40,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{41,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{42,2,CHRCTR_PNP_ID,7,pnp_id_value},
	{43,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{44,2,CHRCTR_MANUFACTURE_NAME,sizeof(manufacture_name_value) - 1,manufacture_name_value},
    {45,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{46,2,CHRCTR_SOFTWARE,sizeof(software_value) - 1,software_value},
	{47,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{48,2,CHRCTR_HARDWARE,sizeof(hardware_value) - 1,hardware_value},
    {49,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{50,2,CHRCTR_FIRMWARE,sizeof(firmware_value) - 1,firmware_value},
    {51,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{52,2,CHRCTR_SERIAL_NUMBER,sizeof(serial_number_value) - 1,serial_number_value},
	{53,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{54,2,CHRCTR_MODEL_NUMBER,sizeof(model_number_value) - 1,model_number_value},
    {55,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{56,2,CHRCTR_SYSTEM_ID, sizeof(system_id_value) - 1, system_id_value},
	
    /* Battery Service */
    {64,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{65,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{66,2,CHRCTR_BATTERY_LEVEL,1,battery_leval_value},
	{67,2,CHARACTERTIC_CONFIGURATION,2,battery_ccc},
    {68,2,REPORT_REFERENCE,2,battery_rr},

    /* Scan Parameters */
    {74,2,PRIMARY_SERVICE,2,SERVICE_SCAN_PARAM},

	{75,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{76,2,CHRCTR_SCAN_INTERVAL_WINDOW,2,scan_param_value},

	/* Human Interface Device */
	{79,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{80,2,GATT_INCLUDE,2,SERVICE_BATTERY},
	{81,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{82,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},
	{83,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{84,2,CHRCTR_HID_INFO,4,hid_info_value},
	{85,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{86,2,CHRCTR_REPORT_MAP,149,report_map_value},
	{87,2,EXTERNAL_REPORT,2,CHRCTR_BATTERY_LEVEL},
	{93,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{94,2,CHRCTR_REPORT,2,report_value},
	{95,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
    {96,2,REPORT_REFERENCE,2,report1_rr},
    {97,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {98,2,CHRCTR_REPORT,2,report_value},
	{99,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
    {100,2,REPORT_REFERENCE,2,report2_rr},
    {101,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {102,2,CHRCTR_REPORT,2,report_value},
	{103,2,CHARACTERTIC_CONFIGURATION,2,report3_ccc},
    {104,2,REPORT_REFERENCE,2,report3_rr},
    {105,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {106,2,CHRCTR_REPORT,2,report_value},
	{107,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
    {108,2,REPORT_REFERENCE,2,report4_rr},
    {109,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
    {110,2,CHRCTR_REPORT,2,report_value},
    {111,2,REPORT_REFERENCE,2,report5_rr},
    {112,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
    {113,2,CHRCTR_REPORT,2,report_value},
    {114,2,REPORT_REFERENCE,2,report6_rr},
    {115,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
    {116,2,CHRCTR_REPORT,2,report_value},
    {117,2,REPORT_REFERENCE,2,report7_rr},
    {118,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {119,2,CHRCTR_REPORT,2,report_value},
	{120,2,CHARACTERTIC_CONFIGURATION,2,report8_ccc},
    {121,2,REPORT_REFERENCE,2,report8_rr},

    /* Unknown Service FE15 */
	{64674,2,PRIMARY_SERVICE,16,Unknown_Service_FE15},

	{64675,2,GATT_CHARACTERISTIC,1,READ_WRITE_INDICATE_PREMITTED},
	{64676,16,CHRCTR_FE15_01,1,chrctr_fe15_01_value},
	{64677,2,CHARACTERTIC_CONFIGURATION,2,chrctr_fe15_01_ccc},
	{64678,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
	{64679,16,CHRCTR_FE15_02,2,chrctr_fe15_02_value},
    {64680,2,GATT_CHARACTERISTIC,1,READ_WRITE_INDICATE_PREMITTED},
	{64681,16,CHRCTR_FE15_03,1,chrctr_fe15_03_value},
    {64682,2,CHARACTERTIC_CONFIGURATION,2,chrctr_fe15_03_ccc},
    {64683,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{64684,16,CHRCTR_FE15_04,2,chrctr_fe15_04_value},

    /* Unknown Service 5DE2 */
	{64930,2,PRIMARY_SERVICE,16,Unknown_Service_5DE2},

	{64931,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
	{64932,16,CHRCTR_5DE2_01,2,chrctr_5de2_01_value},
	{64933,2,GATT_CHARACTERISTIC,1,NOTIFY_PREMITTED_ONLY},
	{64934,16,CHRCTR_5DE2_02,2,chrctr_5de2_02_value},
    {64935,2,CHARACTERTIC_CONFIGURATION,2,chrctr_5de2_02_ccc},
    {64936,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{64937,16,CHRCTR_5DE2_03,750,chrctr_5de2_03_value},
    {64938,2,CHARACTERTIC_CONFIGURATION,2,chrctr_5de2_03_ccc},

    /* Unknown Service CFBF */
    {65026,2,PRIMARY_SERVICE,16,Unknown_Service_CFBF},

    {65027,2,GATT_CHARACTERISTIC,1,WRITE_NOTIFY_PREMITTED},
	{65028,16,CHRCTR_CFBF_01,2,chrctr_cfbf_01_value},
	{65029,2,CHARACTERTIC_CONFIGURATION,2,chrctr_cfbf_01_ccc},
    {65030,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
	{65031,16,CHRCTR_CFBF_03,2,chrctr_cfbf_03_value},
    {65032,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
	{65033,16,CHRCTR_CFBF_02,2,chrctr_cfbf_02_value},
	{65034,2,CHARACTERTIC_CONFIGURATION,2,chrctr_cfbf_03_ccc},
    {65035,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
	{65036,16,CHRCTR_CFBF_04,2,chrctr_cfbf_04_value},
	{65037,2,CHARACTERTIC_CONFIGURATION,2,chrctr_cfbf_04_ccc},

	/* OTA Service */
	{65500,2,PRIMARY_SERVICE,2,OTA_SERVICE},

	{65501,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
	{65502,2,CHRCTR_OTA_NOTIFY,0,NULL},
	{65503,2,CHARACTERTIC_CONFIGURATION,2,chrctr_ota_ccc},

	{65504,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{65505,2,CHRCTR_OTA_WRITE,0,NULL},
};

static uint16_t Get_AttListSize(void)
{
    return (sizeof(Att_List)/sizeof(ATT_TABLE_TYPE));
}

int Get_AttListIndex(uint16_t handle)
{
    for(uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if(Att_List[list_index].handle == handle) {
			return list_index;
		}
	}

	return -1;
}

const ATT_TABLE_TYPE *Find_AttTable_ByHandle(uint16_t handle)
{
	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle == handle) {
			return &Att_List[list_index];
		}
	}

	return NULL;
}

const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid)
{
	if (starthandle > endhandle || starthandle > Att_List[Get_AttListSize()-1].handle)
	{
		return NULL;
	}

	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle >= starthandle && Att_List[list_index].handle <= endhandle
			&& Att_List[list_index].uuid_len == uuid->Len && !memcmp(Att_List[list_index].uuid, uuid->Att, uuid->Len))
		{
			return &Att_List[list_index];
		}
	}

	return NULL;
}

const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type_Data(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid, uint8_t *dataPtr, uint8_t dataLen)
{
	if (starthandle > endhandle || starthandle > Att_List[Get_AttListSize()-1].handle)
	{
		return NULL;
	}

	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle >= starthandle && Att_List[list_index].handle <= endhandle
			&& Att_List[list_index].uuid_len == uuid->Len && !memcmp(Att_List[list_index].uuid, uuid->Att, uuid->Len)
			&& Att_List[list_index].dataLen == dataLen && !memcmp(Att_List[list_index].dataPtr, dataPtr, dataLen))
		{
			return &Att_List[list_index];
		}
	}

	return NULL;
}
