#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t INDICATE_READ[] = {INDICATE_PREMITTED | READ_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {WRITE_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_READ_WRITE_PREMITTED[] = {NOTIFY_PREMITTED | READ_PREMITTED | WRITE_PREMITTED};
static const uint8_t NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {NOTIFY_PREMITTED | READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};
static const uint8_t CHARACTERTIC_DESCRIPTION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_DESCRIPTION)};

/* Remote Control Service */
static const uint8_t FIRMWARE_REVISION_ID[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t system_id_value[] = {0X12, 0X34, 0X56, 0XFF, 0XFE, 0X9A, 0XBC, 0XDE};
static const uint8_t firmware_id_value[] = {"RTL_618_V0001"};
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"RemoteG10"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t CHRCTR_APPEARANCE_2a04[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t appearance_value[] = {0x80, 0x01};
static const uint8_t peripheral_value[2];
static const uint8_t SOFTWARE_REVISION_ID[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t software_revision_value[] = {"V1.36"};
// generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[4] = {0};
static uint8_t Service_Changed_CCC[2];
// device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0x01, 0x57, 0x09, 0x07, 0x00, 0x01, 0x00};
;
static const uint8_t HARDWARE_REVISION_ID[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t hardware_revision_value[] = {"RTKBeeHardwareRev"};
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {"MY4_B001_901V_V1.1"};
static const uint8_t CHRCTR_Hardwar_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t Hardwar_Revision_String_value[] = {"MY4_B001_FMR8_HW_V1.0"};

static const uint8_t CHRCTR_Software_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t Software_Revision_String_value[] = {"MY4_B001_FMR8_V1.1"};

static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {"MY3 BT RC"};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t IEEE_value[] = {0XFF, 0XEE, 0XDD, 0XCC, 0XBB, 0XAA};
// Google Voice
static const uint8_t CHRCTR_ATV_Voice_Service_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Write_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Read_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Control_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB};
static uint8_t Voice_Read_CCC[2];
static uint8_t Voice_Ctrl_CCC[2];

static const uint8_t SERVICE_C88D[] = {0x71, 0x01, 0x11, 0x79, 0x9E, 0xCD, 0x72, 0x8E, 0x08, 0x47, 0xDA, 0xEF, 0xCB, 0x51, 0x8D, 0xC8};
static const uint8_t CHRCTR_EA3D[] = {0x2A, 0x09, 0x4D, 0xA9, 0xE1, 0x83, 0xCF, 0xBE, 0xDA, 0x4A, 0xE7, 0xCE, 0xCA, 0xDD, 0x3D, 0xEA};
static uint8_t CHRCTR_EA3D_CCC[2];

// battery
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[1] = {0x64};
static uint8_t BATTERY_LEVEL_CCC[2];
// human inf device
static const uint8_t SERVICE_HUMAN_ATT[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t PROTOCOL_MODE_ID[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static const uint8_t protocol_mode[2];
static const uint8_t BOOT_KEYBOARD_INPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_INPUT_REPORT)};
static const uint8_t boot_keyboard_input_report[2];
static const uint8_t REPORT_UUID[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report[2];
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] =
	{
		// 0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85,
		// 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7,
		// 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95,
		// 0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08,
		// 0x81, 0x01, 0x95, 0x05, 0x75, 0x01, 0x05,
		// 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02,
		// 0x95, 0x01, 0x75, 0x03, 0x91, 0x01, 0x95,
		// 0x06, 0x75, 0x08, 0x15, 0x00, 0x25, 0xF1,
		// 0x05, 0x07, 0x19, 0x00, 0x29, 0xF1, 0x81,
		// 0x00, 0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1,
		// 0x01, 0x85, 0x02, 0x75, 0x10, 0x95, 0x02,
		// 0x15, 0x01, 0x26, 0x8C, 0x02, 0x19, 0x01,
		// 0x2A, 0x8C, 0x02, 0x81, 0x00, 0xC0
		0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85,
		0x03, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7,
		0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95,
		0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08,
		0x81, 0x03, 0x95, 0x06, 0x75, 0x08, 0x15,
		0x00, 0x26, 0xFF, 0x00, 0x05, 0x07, 0x19,
		0x00, 0x29, 0xFF, 0x81, 0x00, 0xC0, 0x05,
		0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x04,
		0x95, 0x01, 0x75, 0x10, 0x15, 0x00, 0x26,
		0xFF, 0x05, 0x19, 0x00, 0x2A, 0xFF, 0x05,
		0x81, 0x00, 0xC0, 0x06, 0xC1, 0xFF, 0x09,
		0x07, 0xA1, 0x01, 0xA1, 0x02, 0x85, 0x40,
		0x95, 0x01, 0x75, 0x08, 0x15, 0x00, 0x26,
		0xFF, 0x00, 0x09, 0x07, 0x91, 0x22, 0xC0, 0xC0};
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[] = {0x11, 0x01, 0x00, 0x01};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_PROTOCOL_MODE[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static uint8_t protocol_mode_value[1];

static const uint8_t CHRCTR_BOOT_KEYBOARD_INPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_INPUT_REPORT)};
static const uint8_t keyboard_input_report_value[] = {0x00};
static uint8_t KEYBOARD_INPUT_REPORT_CCC[2];

static const uint8_t CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT)};
static uint8_t keyboard_output_report_value[1];

static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
// static const uint8_t report1_rr[] = {0x01, 0x01};
// static const uint8_t report2_rr[] = {0x01, 0x02};
// static const uint8_t report3_rr[] = {0x02, 0x01};
static const uint8_t report4_rr[] = {0x5D, 0x01};
static const uint8_t report5_rr[] = {0x5E, 0x02};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report_value[2];

// 3a69Зa691804 - 0000 - 1000 - 8000 - 00805 - f587e61
static const uint8_t Unknow_3a6918[] = {0x61, 0x7e, 0x58, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x04, 0x18, 0x69, 0x3a};
static const uint8_t Unknow_3a69a06[] = {0x61, 0x7e, 0x58, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x06, 0x2a, 0x69, 0x3a};
static const uint8_t Unknow_3a69a06_value[2];

// 4169726 f - 6861 - 4446 - 5553 - 657276696365
static const uint8_t Unknow_4169_65[] = {0x65, 0x63, 0x69, 0x76, 0x72, 0x65, 0x53, 0x55, 0x46, 0x44, 0x61, 0x68, 0x6f, 0x72, 0x69, 0x41};
static const uint8_t Unknow_4169_64[] = {0x64, 0x6e, 0x61, 0x6d, 0x6d, 0x6f, 0x43, 0x55, 0x46, 0x44, 0x61, 0x68, 0x6f, 0x72, 0x69, 0x41};
static const uint8_t Unknow_4169_64_value[2];
static const uint8_t Unknow_4169_70[] = {0x70, 0x73, 0x65, 0x52, 0x64, 0x6d, 0x43, 0x55, 0x46, 0x44, 0x61, 0x68, 0x6f, 0x72, 0x69, 0x41};
static const uint8_t Unknow_4169_70_value[2];

static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];

// 00010203-0405-0607-0809-0a0b0c0d1912
static const uint8_t Unknow0001912[] = {0x12, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static const uint8_t Unknow0002b12[] = {0x12, 0x2b, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static const uint8_t Unknow0002b12value[2];

// AB5E0001-5A21-4F05-BC7D-AF01F617B664
static const uint8_t Unknowab5e0001[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0x01, 0x00, 0x5e, 0xab};
static const uint8_t Unknowab5e0002[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0x02, 0x00, 0x5e, 0xab};
static const uint8_t Unknowab5e0003[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0x03, 0x00, 0x5e, 0xab};
static const uint8_t Unknowab5e0004[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0x04, 0x00, 0x5e, 0xab};

// D343BFC0-5A21-4F05-BC7D-AF01F617B664
static const uint8_t Unknowd3433bfc0[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0xc0, 0xbf, 0x43, 0xd3};
static const uint8_t Unknowd3433bfc1[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0xc1, 0xbf, 0x43, 0xd3};
static const uint8_t Unknowd3433bfc1value[2];
static const uint8_t Unknowd3433bfc2[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0xc2, 0xbf, 0x43, 0xd3};
static const uint8_t Unknowd3433bfc2value[2];
static const uint8_t Unknowd3433bfc3[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0xc3, 0xbf, 0x43, 0xd3};
static const uint8_t Unknowd3433bfc3value[2];
static const uint8_t Unknowd3433bfc4[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0xc4, 0xbf, 0x43, 0xd3};
static const uint8_t Unknowd3433bfc4value[2];
static const uint8_t Unknowd3433bfc5[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc, 0x05, 0x4f, 0x21, 0x5a, 0xc5, 0xbf, 0x43, 0xd3};
static const uint8_t Unknowd3433bfc5value[2];

static const uint8_t report1_rr[] = {0x03, 0x01};
static const uint8_t report2_rr[] = {0x04, 0x01};
static const uint8_t report3_rr[] = {0x40, 0x02};
const ATT_TABLE_TYPE Att_List[] =
	{
		/* Generic Access */
		{1, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ACC},

		{2, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{3, 2, CHRCTR_DEVICE_NAME, sizeof(device_name), device_name},
		{4, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{5, 2, CHRCTR_APPEARANCE, sizeof(appearance_value), appearance_value},
		{6, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{7, 2, CHRCTR_APPEARANCE_2a04, sizeof(peripheral_value), peripheral_value},

		/* Generic Attribute */
		{8, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ATT},

		{9, 2, GATT_CHARACTERISTIC, 1, INDICATE_PREMITTED_ONLY},
		{10, 2, CHRCTR_SERVICE_CHANGED, sizeof(service_changed_value), service_changed_value},
		{11, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},

		/* Device Information */
		{12, 2, PRIMARY_SERVICE, 2, SERVICE_DEVICE_INFO},

		{13, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{14, 2, CHRCTR_PNP_ID, sizeof(pnp_id_value), pnp_id_value},
		{15, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{16, 2, FIRMWARE_REVISION_ID, sizeof(firmware_id_value) - 1, firmware_id_value},
		{17, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{18, 2, SOFTWARE_REVISION_ID, sizeof(software_revision_value) - 1, software_revision_value},
		{19, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{20, 2, HARDWARE_REVISION_ID, sizeof(hardware_revision_value) - 1, hardware_revision_value},

		/* Human interface Device*/
		{21, 2, PRIMARY_SERVICE, 2, SERVICE_HUMAN_ATT},

		{23, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{24, 2, PROTOCOL_MODE_ID, sizeof(protocol_mode), protocol_mode},

		{25, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{26, 2, BOOT_KEYBOARD_INPUT_REPORT, sizeof(boot_keyboard_input_report), boot_keyboard_input_report},
		{27, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},
		{28, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{29, 2, CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT, sizeof(keyboard_output_report_value), keyboard_output_report_value},
		// report1
		{30, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{31, 2, REPORT_UUID, sizeof(report), report},
		{32, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},
		{33, 2, REPORT_REFERENCE, 2, report1_rr},
		// report2
		{34, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{35, 2, REPORT_UUID, sizeof(report), report},
		{36, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},
		{37, 2, REPORT_REFERENCE, 2, report2_rr},
		// report3
		{38, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{39, 2, REPORT_UUID, sizeof(report), report},
		{40, 2, REPORT_REFERENCE, 2, report3_rr},
		// report map
		{41, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{42, 2, CHRCTR_REPORT_MAP, sizeof(report_map_value), report_map_value},
		{43, 2, EXTERNAL_REPORT, 2, Service_Changed_CCC},
		{44, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
		{45, 2, CHRCTR_HID_INFO, 4, hid_info_value},
		{46, 2, GATT_CHARACTERISTIC, 1, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
		{47, 2, CHRCTR_HID_CTRL_POINT, 1, hid_ctrl_point_value},

		/* Battery Service */
		{48, 2, PRIMARY_SERVICE, 2, SERVICE_BATTERY},

		{49, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{50, 2, CHRCTR_BATTERY_LEVEL, 1, Battery_Level_value},
		{51, 2, CHARACTERTIC_CONFIGURATION, 2, BATTERY_LEVEL_CCC},

		/* Unknown Service 00010203*/
		{52, 2, PRIMARY_SERVICE, 16, Unknow0001912},

		// Unkonwn Service  0002b12
		{53, 2, GATT_CHARACTERISTIC, 1, NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{54, 16, Unknow0002b12, 2, Unknow0002b12value},
		{55, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},
		{56, 2, CHARACTERTIC_DESCRIPTION, 2, Service_Changed_CCC},

		/*Unkonwn Service AB5E0001*/ /* Google Voice */
		{57, 2, PRIMARY_SERVICE, 16, Unknowab5e0001},

		// ab5e0002
		{58, 2, GATT_CHARACTERISTIC, 1, NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
		{59, 16, Unknowab5e0002, 0, NULL},
		// ab5e0003
		{60, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{61, 16, Unknowab5e0003, 0, NULL},
		{62, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},
		// ab5e0004
		{63, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
		{64, 16, Unknowab5e0004, 0, NULL},
		{65, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},

		/* Unknown Service d343bfc0*/
		{66, 2, PRIMARY_SERVICE, 16, Unknowd3433bfc0},

		// d343bfc1
		{67, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		{68, 16, Unknowd3433bfc1, 2, Unknowd3433bfc1value},
		// d343bfc2
		{69, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		{70, 16, Unknowd3433bfc2, 2, Unknowd3433bfc2value},
		// d343bfc3
		{71, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		{72, 16, Unknowd3433bfc3, 2, Unknowd3433bfc3value},
		// d343bfc4
		{73, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
		{74, 16, Unknowd3433bfc4, 2, Unknowd3433bfc4value},
		// d343bfc5
		{75, 2, GATT_CHARACTERISTIC, 1, NOTIFY_PREMITTED_ONLY},
		{76, 16, Unknowd3433bfc5, 2, Unknowd3433bfc5value},
		{77, 2, CHARACTERTIC_CONFIGURATION, 2, Service_Changed_CCC},

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