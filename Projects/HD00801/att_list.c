#include "att_list.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t INDICAT_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {WRITE_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED};
static const uint8_t READ_WRITE_NOTIFY_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t WRITE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {WRITE_PREMITTED | READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_WITHOUT_RESPONSE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_INDICATE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | INDICATE_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED | NOTIFY_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static const uint8_t service_changed[] = {0x00};
static uint8_t service_changed_ccc[2];

static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"Xiaomi RC"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0xc1, 0x03};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t ppcp_value[] = {
0x0a, 0x00, 0x0a, 0x00, 0x63, 0x00, 0x98, 0x3a
// 0x0A ,0x00 ,0x0A ,0x00 ,0x63 ,0x00 ,0xdc,0X05
//,0X98,0X3A //,0x97 ,0x03
};
static const uint8_t CHRCTR_CENTRAL_ADDR_RESOLUTION[] = {UUID2ARRAY(UUID_CHRCTR_CCAR)};
static const uint8_t CCAR_value[] = {0x00};

static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
// static const uint8_t pnp_id_value[] = {0x01, 0x17, 0x27, 0xb9, 0x32, 0x05, 0x00};
static const uint8_t pnp_id_value[] = {0x01, 0x17, 0x27, 0xb9, 0x32, 0x03, 0x00};
static const uint8_t CHRCTR_MANUFACTURE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
// static const uint8_t manufacture_name_value[] = {0x52, 0x65, 0x61, 0x6C, 0x74, 0x65, 0x6B, 0x20, 0x42, 0x54, 0x00};
static const uint8_t manufacture_name_value[] = {"Realtek BT"};
static const uint8_t CHRCTR_SOFTWARE[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
// static const uint8_t software_value[] = {0x36, 0x2E, 0x30, 0x2E, 0x31, 0x2E, 0x35, 0x00};
static const uint8_t software_value[] = {"6.0.1.3"};
static const uint8_t CHRCTR_HARDWARE[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
// static const uint8_t hardware_value[] = {0x36, 0x2E, 0x30};
static const uint8_t hardware_value[] = {"6.0"};
static const uint8_t CHRCTR_FIRMWARE[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
// static const uint8_t firmware_value[] = {0x31, 0x2E, 0x30, 0x2E, 0x32, 0x36, 0x35, 0x2E, 0x32, 0x37}; // 版本号199
static const uint8_t firmware_value[] = {"1.0.265.27"}; // 版本号199
static const uint8_t CHRCTR_SERIAL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
// static const uint8_t serial_number_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x53, 0x65, 0x72, 0x69, 0x61, 0x6C, 0x4E, 0x75, 0x6D, 0x00};
static const uint8_t serial_number_value[] = {"RTKBeeSerialNum"};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {"Model XiaoMi"};
static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t system_id_value[] = {0x00, 0x01, 0x02, 0x00, 0x00, 0x03, 0x04, 0x05};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t ieee_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x49, 0x45, 0x45, 0x45, 0x44, 0x61, 0x74, 0x61, 0x6C, 0x69, 0x73, 0x74, 0x00};

static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static const uint8_t battery_rr[] = {0x19, 0x2A};
static uint8_t battery_leval_value[] = {100};
static uint8_t battery_ccc[2];

static const uint8_t SERVICE_SCAN_PARAM[] = {UUID2ARRAY(UUID_SERVICE_SCAN_PARAM)};
static const uint8_t CHRCTR_SCAN_INTERVAL_WINDOW[] = {UUID2ARRAY(UUID_SCAN_INTERVAL_WINDOW)};
static uint8_t scan_param_value[2];

static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t hid_include_value[] = {0x40, 0x00, 0x44, 0x00, 0x0F, 0x18};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[] = {0x00, 0x01, 0x00, 0x00};
static const uint8_t CHRCTR_RPT_PROTOCOL_MODE[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static uint8_t hid_rpt_protocol_mode_value[1] = {0x01};
static const uint8_t CHRCTR_BOOTKEYBOARD_INPUT[] = {UUID2ARRAY(UUID_CHRCTR_BOOTKEYBOARD_INPUT_RPT)};
static const uint8_t CHRCTR_BOOTKEYBOARD_OUTPUT[] = {UUID2ARRAY(UUID_CHRCTR_BOOTKEYBOARD_OUTPUT_RPT)};

static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = {
	0x05, 0x0C, 0x09, 0x01, 0xA1,
	0x01, 0x85, 0x01, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x18, 0x09,
	0x41, 0x09, 0x42, 0x09, 0x43, 0x09, 0x44, 0x09, 0x45, 0x09, 0x30, 0x09,
	0xCF, 0x09, 0xE9, 0x09, 0xEA, 0x0A, 0xA2, 0x01, 0x0A, 0xB8, 0x01, 0x09,
	0x6A, 0x09, 0x6B, 0x09, 0x6C, 0x0A, 0x8A, 0x01, 0x0A, 0x92, 0x01, 0x0A,
	0x94, 0x01, 0x09, 0xCF, 0x0A, 0x23, 0x02, 0x0A, 0x24, 0x02, 0x0A, 0x25,
	0x02, 0x0A, 0x26, 0x02, 0x0A, 0x27, 0x02, 0x0A, 0x2A, 0x02, 0x81, 0x02,
	0x06, 0x00, 0xFF, 0x09, 0x00, 0xA1, 0x01, 0x85, 0x06, 0x75, 0x08, 0x95,
	0x78, 0x15, 0x00, 0x25, 0xFF, 0x19, 0x00, 0x29, 0xFF, 0x81, 0x00, 0x85,
	0x07, 0x75, 0x08, 0x95, 0x78, 0x15, 0x00, 0x25, 0xFF, 0x19, 0x00, 0x29,
	0xFF, 0x81, 0x00, 0x85, 0x08, 0x75, 0x08, 0x95, 0x78, 0x15, 0x00, 0x25,
	0xFF, 0x19, 0x00, 0x29, 0xFF, 0x81, 0x00, 0xC0, 0x05, 0x01, 0x09, 0x06,
	0xA1, 0x01, 0x85, 0x02, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00,
	0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08,
	0x81, 0x01, 0x95, 0x05, 0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05,
	0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x01, 0x95, 0x06, 0x75, 0x08,
	0x15, 0x28, 0x25, 0xFE, 0x05, 0x07, 0x19, 0x28, 0x29, 0xFE, 0x81, 0x00,
	0xC0, 0xC0

};

// Google Voice
static const uint8_t CHRCTR_ATV_Voice_Service_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Write_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Read_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Control_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB};
static uint8_t voice_Read_ccc[2];
static uint8_t voice_Control_ccc[2];

static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};

static const uint8_t report1_rr[] = {0x02, 0x01};
static const uint8_t report2_rr[] = {0x02, 0x02};
static const uint8_t report3_rr[] = {0x03, 0x01};
static const uint8_t report4_rr[] = {0x04, 0x02};
static const uint8_t report5_rr[] = {0x05, 0x01};
static const uint8_t report6_rr[] = {0x06, 0x01};
static const uint8_t report7_rr[] = {0x07, 0x01};
static const uint8_t report8_rr[] = {0x08, 0x01};
static const uint8_t report9_rr[] = {0x09, 0x02};
static const uint8_t report10_rr[] = {0x0a, 0x03};
static const uint8_t report11_rr[] = {0xe0, 0x03};
static const uint8_t report12_rr[] = {0xe1, 0x03};
static const uint8_t report13_rr[] = {0xe2, 0x03};
static const uint8_t report14_rr[] = {0xe3, 0x03};
static const uint8_t report15_rr[] = {0xe4, 0x03};
static const uint8_t report16_rr[] = {0xe5, 0x03};
static const uint8_t report17_rr[] = {0xe6, 0x03};
static const uint8_t report18_rr[] = {0xe7, 0x03};
static const uint8_t report19_rr[] = {0xe8, 0x03};
static const uint8_t report20_rr[] = {0xe9, 0x03};
static const uint8_t report21_rr[] = {0xea, 0x03};
static const uint8_t report22_rr[] = {0xeb, 0x03};
static const uint8_t report23_rr[] = {0xec, 0x03};
static const uint8_t report24_rr[] = {0xed, 0x03};
static const uint8_t report25_rr[] = {0xee, 0x03};
static const uint8_t report26_rr[] = {0x01, 0x01};

static uint8_t report_value[2];//report 113

static const uint8_t report_handle89_value[] = {0x27, 0x17, 0x32, 0xb9, 0xD3, 0x04, 0x37, 0x19, 0xa0, 0x20, 0x00, 0x00};
static const uint8_t report_handle92_value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x20};
static const uint8_t report_handle98_value[] = {0x02};
static const uint8_t report_handle101_value[] = {0x02};
static uint8_t bootkeyboard_input_ccc[2];
static uint8_t bootkeyboard_output_ccc[2];
static uint8_t report1_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report5_ccc[2];
static uint8_t report5_ccc[2];
static uint8_t report6_ccc[2];
static uint8_t report7_ccc[2];
static uint8_t report8_ccc[2];
static uint8_t report10_ccc[2];
static uint8_t report26_ccc[2];

static const uint8_t Unknown_Service_2141[] = {0x12, 0xa2, 0x4d, 0x2e, 0xfe, 0x14, 0x48, 0x8e, 0x93, 0xd2, 0x17, 0x3c, 0xff, 0xd0, 0x00, 0x00};
static const uint8_t CHRCTR_2141_01[] = {0x01, 0xA0};
static const uint8_t CHRCTR_2141_02[] = {0x02, 0xA0};
static const uint8_t CHRCTR_2141_04[] = {0x04, 0xA0};
static uint8_t chrctr_d1ff_01_ccc[2];
static uint8_t chrctr_d1ff_04_ccc[2];
uint8_t chrctr_d1ff_01_value[2] = {0x01};
uint8_t chrctr_d1ff_02_value[2];
uint8_t chrctr_d1ff_04_value[2];

static const uint8_t Unknown_Service_D0FF[] = {0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8E, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD0, 0x00, 0x00};
static const uint8_t CHRCTR_FFD1[] = {0xD1, 0xFF};
static const uint8_t CHRCTR_FFD2[] = {0xD2, 0xFF};
static const uint8_t CHRCTR_FFD3[] = {0xD3, 0xFF};
static const uint8_t CHRCTR_FFD4[] = {0xD4, 0xFF};
static const uint8_t CHRCTR_FFD5[] = {0xD5, 0xFF};
static const uint8_t CHRCTR_FFD8[] = {0xD8, 0xFF};
static const uint8_t CHRCTR_FFF1[] = {0xF1, 0xFF};
static const uint8_t CHRCTR_FFF2[] = {0xF2, 0xFF};
static const uint8_t CHRCTR_FFE0[] = {0xE0, 0xFF};
uint8_t chrctr_ffd1_value[2];
// uint8_t chrctr_ffd2_value[] = {0xa4, 0xd1, 0x20, 0x1b, 0xa1, 0x5e};
uint8_t chrctr_ffd2_value[] = {0xa4, 0xd1, 0x20, 0x1b, 0xa9, 0x00};
uint8_t chrctr_ffd3_value[] = {0x01, 0x90, 0x10, 0xd8};
uint8_t chrctr_ffd4_value[] = {0x06, 0x10, 0x00, 0x28};
uint8_t chrctr_ffd5_value[2];
uint8_t chrctr_ffd8_value[2];
uint8_t chrctr_fff1_value[] = {0x05, 0x01, 0x00, 0x07, 0x00, 0x08, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00};
uint8_t chrctr_fff2_value[2];
uint8_t chrctr_ffe0_value[] = {0x00, 0x00, 0x00, 0x01, 0x11, 0x20, 0x00, 0x00, 0x01, 0x90, 0x10, 0xd8, 0x06, 0x10, 0x00, 0x18};

static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];

const ATT_TABLE_TYPE Att_List[] =
	{
		/* Generic Attribute */
		{1, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ATT},
		{2, 2, GATT_CHARACTERISTIC, 1, INDICAT_PREMITTED_ONLY},
		{3, 2, CHRCTR_SERVICE_CHANGED, 1, service_changed},
		{4, 2, CHARACTERTIC_CONFIGURATION, 2, service_changed_ccc},

		/* Generic Access */
		{5, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ACC},

		{6, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{7, 2, CHRCTR_DEVICE_NAME, 9, device_name},
		{8, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{9, 2, CHRCTR_APPEARANCE, 2, appearance_value},
		{10, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{11, 2, CHRCTR_PPCP, 8, ppcp_value},
		{12, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{13, 2, CHRCTR_CENTRAL_ADDR_RESOLUTION, 1, CCAR_value},

		/* Battery Service */
		{14, 2, PRIMARY_SERVICE, 2, SERVICE_BATTERY},

		{15, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{16, 2, CHRCTR_BATTERY_LEVEL, 1, battery_leval_value},
		{17, 2, CHARACTERTIC_CONFIGURATION, 2, battery_ccc},

		/* Device Information */
		{18, 2, PRIMARY_SERVICE, 2, SERVICE_DEVICE_INFO},

		{19, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{20, 2, CHRCTR_MANUFACTURE_NAME, sizeof(manufacture_name_value) - 1, manufacture_name_value},

		{21, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{22, 2, CHRCTR_MODEL_NUMBER, sizeof(model_number_value) - 1, model_number_value},

		{23, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{24, 2, CHRCTR_SERIAL_NUMBER, sizeof(serial_number_value) - 1, serial_number_value},

		{25, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{26, 2, CHRCTR_HARDWARE, sizeof(hardware_value) - 1, hardware_value},

		{27, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{28, 2, CHRCTR_FIRMWARE, sizeof(firmware_value) - 1, firmware_value},

		{29, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{30, 2, CHRCTR_SOFTWARE, sizeof(software_value) - 1, software_value},

		{31, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{32, 2, CHRCTR_SYSTEM_ID, 8, system_id_value},

		{33, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{34, 2, CHRCTR_IEEE, 19, ieee_value},

		{35, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{36, 2, CHRCTR_PNP_ID, 7, pnp_id_value},

		/* Human Interface Device */
		{37, 2, PRIMARY_SERVICE, 2, SERVICE_HIDS},

		{38, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{39, 2, CHRCTR_HID_INFO, 4, hid_info_value},

		{40, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{41, 2, CHRCTR_HID_CTRL_POINT, 1, hid_ctrl_point_value},

		{42, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{43, 2, CHRCTR_RPT_PROTOCOL_MODE, 1, hid_rpt_protocol_mode_value},

		{44, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{45, 2, CHRCTR_REPORT_MAP, 199, report_map_value},

		{46, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{47, 2, CHRCTR_BOOTKEYBOARD_INPUT, 2, report_value},
		{48, 2, CHARACTERTIC_CONFIGURATION, 2, bootkeyboard_input_ccc},

		{49, 2, GATT_CHARACTERISTIC, 1, WRITE_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{50, 2, CHRCTR_BOOTKEYBOARD_OUTPUT, 2, report_value},

		{51, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{52, 2, CHRCTR_REPORT, 2, report_value},
		{53, 2, CHARACTERTIC_CONFIGURATION, 2, report1_ccc},
		{54, 2, REPORT_REFERENCE, 2, report1_rr},

		{55, 2, GATT_CHARACTERISTIC, 1, WRITE_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{56, 2, CHRCTR_REPORT, 2, report_value},
		{57, 2, REPORT_REFERENCE, 2, report2_rr},

		{58, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{59, 2, CHRCTR_REPORT, 2, report_value},
		{60, 2, CHARACTERTIC_CONFIGURATION, 2, report3_ccc},
		{61, 2, REPORT_REFERENCE, 2, report3_rr},

		{62, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{63, 2, CHRCTR_REPORT, 2, report_value},
		{64, 2, REPORT_REFERENCE, 2, report4_rr},

		{65, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{66, 2, CHRCTR_REPORT, 2, report_value},
		{67, 2, CHARACTERTIC_CONFIGURATION, 2, report5_ccc},
		{68, 2, REPORT_REFERENCE, 2, report5_rr},

		{69, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{70, 2, CHRCTR_REPORT, 2, report_value},
		{71, 2, CHARACTERTIC_CONFIGURATION, 2, report6_ccc},
		{72, 2, REPORT_REFERENCE, 2, report6_rr},

		{73, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{74, 2, CHRCTR_REPORT, 2, report_value},
		{75, 2, CHARACTERTIC_CONFIGURATION, 2, report7_ccc},
		{76, 2, REPORT_REFERENCE, 2, report7_rr},

		{77, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{78, 2, CHRCTR_REPORT, 2, report_value},
		{79, 2, CHARACTERTIC_CONFIGURATION, 2, report8_ccc},
		{80, 2, REPORT_REFERENCE, 2, report8_rr},

		{81, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{82, 2, CHRCTR_REPORT, 2, report_value},
		{83, 2, REPORT_REFERENCE, 2, report9_rr},

		{84, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{85, 2, CHRCTR_REPORT, 2, report_value},
		{86, 2, CHARACTERTIC_CONFIGURATION, 2, report10_ccc},
		{87, 2, REPORT_REFERENCE, 2, report10_rr},

		{88, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{89, 2, CHRCTR_REPORT, 12, report_handle89_value},
		{90, 2, REPORT_REFERENCE, 2, report11_rr},

		{91, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{92, 2, CHRCTR_REPORT, 6, report_handle92_value},
		{93, 2, REPORT_REFERENCE, 2, report12_rr},

		{94, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{95, 2, CHRCTR_REPORT, 2, report_value},
		{96, 2, REPORT_REFERENCE, 2, report13_rr},

		{97, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{98, 2, CHRCTR_REPORT, 1, report_handle98_value},
		{99, 2, REPORT_REFERENCE, 2, report14_rr},

		{100, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{101, 2, CHRCTR_REPORT, 1, report_handle101_value},
		{102, 2, REPORT_REFERENCE, 2, report15_rr},

		{103, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{104, 2, CHRCTR_REPORT, 2, report_value},
		{105, 2, REPORT_REFERENCE, 2, report16_rr},

		{106, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{107, 2, CHRCTR_REPORT, 2, report_value},
		{108, 2, REPORT_REFERENCE, 2, report17_rr},

		{109, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{110, 2, CHRCTR_REPORT, 2, report_value},
		{111, 2, REPORT_REFERENCE, 2, report18_rr},

		{112, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{113, 2, CHRCTR_REPORT, 2, report_value},
		{114, 2, REPORT_REFERENCE, 2, report19_rr},

		{115, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{116, 2, CHRCTR_REPORT, 2, report_value},
		{117, 2, REPORT_REFERENCE, 2, report20_rr},

		{118, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{119, 2, CHRCTR_REPORT, 2, report_value},
		{120, 2, REPORT_REFERENCE, 2, report21_rr},

		{121, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{122, 2, CHRCTR_REPORT, 2, report_value},
		{123, 2, REPORT_REFERENCE, 2, report22_rr},

		{124, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{125, 2, CHRCTR_REPORT, 2, report_value},
		{126, 2, REPORT_REFERENCE, 2, report23_rr},

		{127, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{128, 2, CHRCTR_REPORT, 2, report_value},
		{129, 2, REPORT_REFERENCE, 2, report24_rr},

		{130, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
		{131, 2, CHRCTR_REPORT, 2, report_value},
		{132, 2, REPORT_REFERENCE, 2, report25_rr},

		{133, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{134, 2, CHRCTR_REPORT, 2, report_value},
		{135, 2, CHARACTERTIC_CONFIGURATION, 2, report26_ccc},
		{136, 2, REPORT_REFERENCE, 2, report26_rr},
		////////////////////////////////////////////////////////

		/* Unknown Service D1FF */
		{137, 2, PRIMARY_SERVICE, 16, Unknown_Service_2141},

		{138, 2, GATT_CHARACTERISTIC, 1, NOTIFY_PREMITTED_ONLY},
		{139, 2, CHRCTR_2141_01, 2, chrctr_d1ff_01_value},
		{140, 2, CHARACTERTIC_CONFIGURATION, 2, chrctr_d1ff_01_ccc},

		{141, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{142, 2, CHRCTR_2141_02, 2, chrctr_d1ff_02_value},

		{143, 2, GATT_CHARACTERISTIC, 1, NOTIFY_PREMITTED_ONLY},
		{144, 2, CHRCTR_2141_04, 2, chrctr_d1ff_04_value},
		{145, 2, CHARACTERTIC_CONFIGURATION, 2, chrctr_d1ff_04_ccc},

		/* Unknown Service D0FF */
		{146, 2, PRIMARY_SERVICE, 16, Unknown_Service_D0FF},

		{147, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{148, 2, CHRCTR_FFD1, 2, chrctr_ffd1_value},

		{149, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{150, 2, CHRCTR_FFD2, 2, chrctr_ffd2_value},

		{151, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{152, 2, CHRCTR_FFD3, 2, chrctr_ffd3_value},

		{153, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{154, 2, CHRCTR_FFD4, 2, chrctr_ffd4_value},

		{155, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{156, 2, CHRCTR_FFD5, 2, chrctr_ffd5_value},

		{157, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{158, 2, CHRCTR_FFD8, 2, chrctr_ffd8_value},

		{159, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{160, 2, CHRCTR_FFF1, 2, chrctr_fff1_value},

		{161, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		{162, 2, CHRCTR_FFF2, 2, chrctr_fff2_value},

		{163, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{164, 2, CHRCTR_FFE0, 2, chrctr_ffe0_value},

		/* Google Voice */
		// {165, 2, PRIMARY_SERVICE, 16, CHRCTR_ATV_Voice_Service_uuid_128},

		// {166, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		// {167, 16, CHRCTR_Write_uuid_128, 0, NULL},
		// {168, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		// {169, 16, CHRCTR_Read_uuid_128, 0, NULL},
		// {170, 2, CHARACTERTIC_CONFIGURATION, 2, voice_Read_ccc},
		// {171, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		// {172, 16, CHRCTR_Control_uuid_128, 0, NULL},
		// {173, 2, CHARACTERTIC_CONFIGURATION, 2, voice_Control_ccc},

		/* OTA Service */
		{65500, 2, PRIMARY_SERVICE, 2, OTA_SERVICE},

		{65501, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
		{65502, 2, CHRCTR_OTA_NOTIFY, 0, NULL},
		{65503, 2, CHARACTERTIC_CONFIGURATION, 2, chrctr_ota_ccc},

		{65504, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{65505, 2, CHRCTR_OTA_WRITE, 0, NULL},
};

static uint16_t Get_AttListSize(void)
{
	return (sizeof(Att_List) / sizeof(ATT_TABLE_TYPE));
}

int Get_AttListIndex(uint16_t handle)
{
	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle == handle)
		{
			return list_index;
		}
	}

	return -1;
}

const ATT_TABLE_TYPE *Find_AttTable_ByHandle(uint16_t handle)
{
	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle == handle)
		{
			return &Att_List[list_index];
		}
	}

	return NULL;
}

const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid)
{
	if (starthandle > endhandle || starthandle > Att_List[Get_AttListSize() - 1].handle)
	{
		return NULL;
	}

	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle >= starthandle && Att_List[list_index].handle <= endhandle && Att_List[list_index].uuid_len == uuid->Len && !memcmp(Att_List[list_index].uuid, uuid->Att, uuid->Len))
		{
			return &Att_List[list_index];
		}
	}

	return NULL;
}

const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type_Data(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid, uint8_t *dataPtr, uint8_t dataLen)
{
	if (starthandle > endhandle || starthandle > Att_List[Get_AttListSize() - 1].handle)
	{
		return NULL;
	}

	for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++)
	{
		if (Att_List[list_index].handle >= starthandle && Att_List[list_index].handle <= endhandle && Att_List[list_index].uuid_len == uuid->Len && !memcmp(Att_List[list_index].uuid, uuid->Att, uuid->Len) && Att_List[list_index].dataLen == dataLen && !memcmp(Att_List[list_index].dataPtr, dataPtr, dataLen))
		{
			return &Att_List[list_index];
		}
	}

	return NULL;
}
