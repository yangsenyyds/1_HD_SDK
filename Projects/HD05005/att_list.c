#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};
const uint8_t SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SVC_CHANGED)};

static uint8_t service_changed_val[1];
static uint8_t service_changed_ccc[2];

static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED};
static const uint8_t WRITE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_WITHOUT_RESPONSE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_INDICATE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | INDICATE_PREMITTED};
static const uint8_t READ_WRITE_NOTIFY_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {READ_PREMITTED | WRITE_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};

static const uint8_t PRO_INDICATE_PREMITTED[] = {INDICATE_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};
static const uint8_t CHARACTERISTIC_USER_DESC[] = {UUID2ARRAY(CHARACTERISTIC_USER_DISC)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};

static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"LGE MR22"};
// static const uint8_t device_name[] = {"LGE MR 15RA"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0xC0, 0x03};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t ppcp_value[] = {0x08, 0x00, 0x08, 0x00, 0x04, 0x00, 0x2C, 0x01};
static const uint8_t CHRCTR_CARN[] = {UUID2ARRAY(UUID_CHRCTR_CARN)};
static const uint8_t carn_value[] = {0x00};

static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0x01, 0x0F, 0x00, 0x12, 0x34, 0x01, 0x00};
static const uint8_t CHRCTR_MANUFACTURE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t manufacture_name_value[] = {0x52, 0x65, 0x61, 0x6C, 0x74, 0x65, 0x6B, 0x20, 0x42, 0x54};
static const uint8_t CHRCTR_SOFTWARE[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t software_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x53, 0x6F, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x52, 0x65, 0x76};
static const uint8_t CHRCTR_HARDWARE[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t hardware_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x48, 0x61, 0x72, 0x64, 0x77, 0x61, 0x72, 0x65, 0x52, 0x65, 0x76};
static const uint8_t CHRCTR_FIRMWARE[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t firmware_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x46, 0x69, 0x72, 0x6D, 0x77, 0x61, 0x72, 0x65, 0x52, 0x65, 0x76};
static const uint8_t CHRCTR_SERIAL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t serial_number_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x53, 0x65, 0x72, 0x69, 0x61, 0x6C, 0x4E, 0x75, 0x6D};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {0x4D, 0x6F, 0x64, 0x65, 0x6C, 0x20, 0x4E, 0x62, 0x72, 0x20, 0x30, 0x2E, 0x39};
static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t system_id_value[] = {0x00, 0x01, 0x02, 0x00, 0x00, 0x03, 0x04, 0x05};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t system_ieee_value[] = {0x52, 0x54, 0x4B, 0x42, 0x65, 0x65, 0x49, 0x45, 0x45, 0x45, 0x44, 0x61, 0x74, 0x61, 0x6C, 0x69, 0x73, 0x74};

static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static const uint8_t CHRCTR_BATTERY_value[] = {0x19, 0x19};
static const uint8_t battery_rr[] = {0x19, 0x2A};
static uint8_t battery_leval_value[] = {100};
static uint8_t battery_ccc[2];

static const uint8_t SERVICE_SCAN_PARAM[] = {UUID2ARRAY(UUID_SERVICE_SCAN_PARAM)};
static const uint8_t CHRCTR_SCAN_INTERVAL_WINDOW[] = {UUID2ARRAY(UUID_SCAN_INTERVAL_WINDOW)};
static uint8_t scan_param_value[2];
static const uint8_t CHRCTR_SCAN_REFRESH[] = {UUID2ARRAY(UUID_SCAN_REFRESH)};
static const uint8_t scan_refresh_val[2];
static const uint8_t scan_refresh_ccc[2];

static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t hid_include_value[] = {0x40, 0x00, 0x44, 0x00, 0x0F, 0x18};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_HID_PROTOCOL_MODE[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static const uint8_t hid_protocol_mode[] = {0X69};
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
// static const uint8_t hid_info_value[] = {0x00, 0x01, 0x00, 0x00};
static const uint8_t hid_info_value[] = {0x00, 0x00, 0x00, 0x01};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = {
	0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0xFD, 0x95, 0x1E,
	0x75, 0x08, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x81, 0x00, 0xC0,
	0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0xF9, 0x95, 0x1E,
	0x75, 0x08, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x81, 0x00, 0x95,
	0xC8, 0x75, 0x08, 0x15, 0x00, 0x26, 0xFF, 0x00, 0xB1, 0x00,
	0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0xFE, 0x95,
	0x7C, 0x75, 0x08, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x81, 0x00,
	0xC0

};
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0xF9, 0x01};
static const uint8_t report2_rr[] = {0xF9, 0x03};
static const uint8_t report3_rr[] = {0xFD, 0x01};
static const uint8_t report4_rr[] = {0xFE, 0x01};
static const uint8_t report5_rr[] = {0xF9, 0x02};

static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report8_ccc[2];
static uint8_t report_value[2];

static uint8_t report1_usr_desc[2];
static uint8_t report2_usr_desc[2];
static uint8_t report3_usr_desc[2];
static uint8_t report4_usr_desc[2];
static uint8_t report5_usr_desc[2];

static const uint8_t Unknown_Service_D1FF[] = {0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8E, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD1, 0x00, 0x00};
static const uint8_t CHRCTR_A001[] = {0x01, 0xA0};
static uint8_t chrctr_a001_ccc[2];
static const uint8_t chrctr_a0_01_value[] = {0x00};

static const uint8_t Unknown_Service_D0FF[] = {0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8E, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD0, 0x00, 0x00};
static const uint8_t CHRCTR_FF_D1[] = {0xd1, 0xff};
static const uint8_t CHRCTR_FF_D2[] = {0xd2, 0xff};
static const uint8_t CHRCTR_FF_D3[] = {0xd3, 0xff};
static const uint8_t CHRCTR_FF_D4[] = {0xd4, 0xff};
static const uint8_t CHRCTR_FF_D5[] = {0xd5, 0xff};
static const uint8_t CHRCTR_FF_D8[] = {0xd8, 0xff};
static const uint8_t CHRCTR_FF_F1[] = {0xf1, 0xff};
static const uint8_t CHRCTR_FF_F2[] = {0xf2, 0xff};
static const uint8_t CHRCTR_FF_E0[] = {0xe0, 0xff};

uint8_t chrctr_ff_d1_value[2];
uint8_t chrctr_ff_d2_value[2];
uint8_t chrctr_ff_d3_value[2];
uint8_t chrctr_ff_d4_value[2];
uint8_t chrctr_ff_d5_value[2];
uint8_t chrctr_ff_d8_value[2];
uint8_t chrctr_ff_f1_value[2];
uint8_t chrctr_ff_f2_value[2];
uint8_t chrctr_ff_e0_value[2];

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

static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];
const ATT_TABLE_TYPE Att_List[] =
	{
		/* Generic Attribute */
		{1, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ATT},
		/// service changed
		{2, 2, GATT_CHARACTERISTIC, 1, PRO_INDICATE_PREMITTED},
		{3, 2, SERVICE_CHANGED, 1, service_changed_val},
		{4, 2, CHARACTERTIC_CONFIGURATION, 2, service_changed_ccc},

		/* Generic Access */
		{5, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ACC},
		/// device name
		{6, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{7, 2, CHRCTR_DEVICE_NAME, sizeof(device_name) - 1, device_name},
		/// appearance
		{8, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{9, 2, CHRCTR_APPEARANCE, 2, appearance_value},
		/// ppcp peripheral preferred connection
		{10, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{11, 2, CHRCTR_PPCP, 8, ppcp_value},
		/// carn central address resolution
		{12, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{13, 2, CHRCTR_CARN, 1, carn_value},

		/* Human Interface Device */
		{14, 2, PRIMARY_SERVICE, 2, SERVICE_HIDS},

		{15, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{16, 2, CHRCTR_HID_INFO, 4, hid_info_value},

		{17, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{18, 2, CHRCTR_HID_CTRL_POINT, 1, hid_ctrl_point_value},

		{19, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{20, 2, CHRCTR_HID_PROTOCOL_MODE, 1, hid_protocol_mode},

		{21, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{22, 2, CHRCTR_REPORT_MAP, 71, report_map_value},

		// {23, 2, EXTERNAL_REPORT, 2, CHRCTR_BATTERY_LEVEL},
		{23, 2, EXTERNAL_REPORT, 2, CHRCTR_BATTERY_value},

		{24, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{25, 2, CHRCTR_REPORT, 2, report_value},
		{26, 2, CHARACTERTIC_CONFIGURATION, 2, report1_ccc},
		{27, 2, REPORT_REFERENCE, 2, report1_rr},
		{28, 2, CHARACTERISTIC_USER_DESC, 2, report1_usr_desc},

		{29, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{30, 2, CHRCTR_REPORT, 2, report_value},
		{31, 2, REPORT_REFERENCE, 2, report2_rr},
		{32, 2, CHARACTERISTIC_USER_DESC, 2, report2_usr_desc},

		{33, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_NOTIFY_PREMITTED},
		{34, 2, CHRCTR_REPORT, 2, report_value},
		{35, 2, CHARACTERTIC_CONFIGURATION, 2, report3_ccc},
		{36, 2, REPORT_REFERENCE, 2, report3_rr},
		{37, 2, CHARACTERISTIC_USER_DESC, 2, report3_usr_desc},

		{38, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{39, 2, CHRCTR_REPORT, 2, report_value},
		{40, 2, CHARACTERTIC_CONFIGURATION, 2, report4_ccc},
		{41, 2, REPORT_REFERENCE, 2, report4_rr},
		{42, 2, CHARACTERISTIC_USER_DESC, 2, report4_usr_desc},

		{43, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{44, 2, CHRCTR_REPORT, 2, report_value},
		{45, 2, REPORT_REFERENCE, 2, report5_rr},
		{46, 2, CHARACTERISTIC_USER_DESC, 2, report5_usr_desc},

		/* Battery Service */
		{47, 2, PRIMARY_SERVICE, 2, SERVICE_BATTERY},

		{48, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{49, 2, CHRCTR_BATTERY_LEVEL, 1, battery_leval_value},
		{50, 2, CHARACTERTIC_CONFIGURATION, 2, battery_ccc},

		/* Device Information */
		{51, 2, PRIMARY_SERVICE, 2, SERVICE_DEVICE_INFO},

		{52, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{53, 2, CHRCTR_MANUFACTURE_NAME, 10, manufacture_name_value},

		{54, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{55, 2, CHRCTR_MODEL_NUMBER, 10, model_number_value},

		{56, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{57, 2, CHRCTR_SERIAL_NUMBER, 15, serial_number_value},

		{58, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{59, 2, CHRCTR_HARDWARE, 17, hardware_value},

		{60, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{61, 2, CHRCTR_FIRMWARE, 17, firmware_value},

		{62, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{63, 2, CHRCTR_SOFTWARE, 17, software_value},

		{64, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{65, 2, CHRCTR_SYSTEM_ID, 8, system_id_value},

		{66, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{67, 2, CHRCTR_IEEE, 18, system_ieee_value},

		{68, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{69, 2, CHRCTR_PNP_ID, 7, pnp_id_value},

		/* Scan Parameters */
		{70, 2, PRIMARY_SERVICE, 2, SERVICE_SCAN_PARAM},

		{71, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{72, 2, CHRCTR_SCAN_INTERVAL_WINDOW, 2, scan_param_value},

		{73, 2, GATT_CHARACTERISTIC, 1, NOTIFY_PREMITTED_ONLY},
		{74, 2, CHRCTR_SCAN_REFRESH, 2, scan_refresh_val},
		{75, 2, CHARACTERTIC_CONFIGURATION, 2, scan_refresh_ccc},

		/* Unknown Service D1FF */
		{76, 2, PRIMARY_SERVICE, 16, Unknown_Service_D1FF},

		{77, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
		{78, 2, CHRCTR_A001, 1, chrctr_a0_01_value},
		{79, 2, CHARACTERTIC_CONFIGURATION, 2, chrctr_a001_ccc},

		/* Unknown Service D0FF */
		{80, 2, PRIMARY_SERVICE, 16, Unknown_Service_D0FF},

		{81, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{82, 2, CHRCTR_FF_D1, 2, chrctr_ff_d1_value},

		{83, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{84, 2, CHRCTR_FF_D2, 2, chrctr_ff_d2_value},

		{85, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{86, 2, CHRCTR_FF_D3, 2, chrctr_ff_d3_value},

		{87, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{88, 2, CHRCTR_FF_D4, 2, chrctr_ff_d4_value},

		{89, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{90, 2, CHRCTR_FF_D5, 2, chrctr_ff_d5_value},

		{91, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{92, 2, CHRCTR_FF_D8, 2, chrctr_ff_d8_value},

		{93, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{94, 2, CHRCTR_FF_F1, 2, chrctr_ff_f1_value},

		{95, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		{96, 2, CHRCTR_FF_F2, 2, chrctr_ff_f2_value},

		{97, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{98, 2, CHRCTR_FF_E0, 2, chrctr_ff_e0_value},

		// 	/* OTA Service */
		{65500,2,PRIMARY_SERVICE,2,OTA_SERVICE},

		{65501,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
		{65502,2,CHRCTR_OTA_NOTIFY,0,NULL},
		{65503,2,CHARACTERTIC_CONFIGURATION,2,chrctr_ota_ccc},

		{65504,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{65505,2,CHRCTR_OTA_WRITE,0,NULL},
};

static uint16_t Get_AttListSize(void)
{
	return (sizeof(Att_List) / sizeof(ATT_TABLE_TYPE));
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
