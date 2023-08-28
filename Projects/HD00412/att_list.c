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
static const uint8_t WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_READ_WRITE_PREMITTED[] = {NOTIFY_PREMITTED|READ_PREMITTED|WRITE_PREMITTED};
static const uint8_t NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {NOTIFY_PREMITTED|READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t system_id_value[] = {0X12,0X34,0X56,0XFF,0XFE,0X9A,0XBC,0XDE};
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[1] ;
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {"MY3-BLE-1."};
//generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[4] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0x01,0x46,0x00,0x38,0x38,0x0D,0x00};
static const uint8_t CHRCTR_Serial_number_String[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t Serial_number_string_value[] = {"1.0.0.0-LE"};
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {"MY4_B001_901V_V1.1"};
static const uint8_t CHRCTR_Hardwar_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t Hardwar_Revision_String_value[] = {"MY4_B001_FMR8_HW_V1.0"};

static const uint8_t CHRCTR_Software_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t Software_Revision_String_value[] = {"MY4_B001_FMR8_V1.1"};

static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {"MY3 BT RC"};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t IEEE_value[] = {0XFF,0XEE,0XDD,0XCC,0XBB,0XAA};
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

//battery
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[1] = {0x64};
static uint8_t BATTERY_LEVEL_CCC[2];
//human inf device
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = 
{
//  0x06 ,0x00 ,0xFF ,0x09 ,0x00 ,0xA1 ,0x01 ,0xA1 ,0x02 ,0x85 ,0x5D 
// ,0x09 ,0x00 ,0x15 ,0x80 ,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x81 
// ,0x22 ,0xC0 ,0xA1 ,0x02 ,0x85 ,0x5E ,0x09 ,0x00 ,0x15 ,0x80 ,0x25 
// ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x81 ,0x22 ,0xC0 ,0xA1 ,0x02 ,0x85 
// ,0x02 ,0x19 ,0x00 ,0x2A ,0x9C ,0x02 ,0x15 ,0x00 ,0x26 ,0x9C ,0x02 
// ,0x95 ,0x01 ,0x75 ,0x10 ,0x80 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x07 ,0xA1 
// ,0x01 ,0x85 ,0x01 ,0x05 ,0x07 ,0x19 ,0xE0 ,0x29 ,0xE7 ,0x15 ,0x00 
// ,0x25 ,0x01 ,0x75 ,0x01 ,0x95 ,0x08 ,0x81 ,0x02 ,0x95 ,0x01 ,0x75 
// ,0x08 ,0x81 ,0x03 ,0x95 ,0x05 ,0x75 ,0x01 ,0x05 ,0x08 ,0x19 ,0x01 
// ,0x29 ,0x05 ,0x91 ,0x02 ,0x95 ,0x01 ,0x75 ,0x03 ,0x91 ,0x03 ,0x95 
// ,0x06 ,0x75 ,0x08 ,0x15 ,0x00 ,0x25 ,0xFF ,0x05 ,0x07 ,0x19 ,0x00 
// ,0x29 ,0xFF ,0x81 ,0x00 ,0xC0

// 0x06 ,0x00 ,0xFF ,0x09 ,0x00 ,0xA1 ,0x01 ,0xA1 ,0x02 ,0x85 ,0x5D 
// ,0x09 ,0x00 ,0x15 ,0x80 ,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x81 
// ,0x22 ,0xC0 ,0xA1 ,0x02 ,0x85 ,0x5E ,0x09 ,0x00 ,0x15 ,0x80 ,0x25 
// ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x81 ,0x22 ,0xC0 ,0xA1 ,0x02 ,0x85 
// ,0x0D ,0x02 ,0x19 ,0x00 ,0x2A ,0x9C ,0x02 ,0x15 ,0x00 ,0x26 ,0x9C 
// ,0x02 ,0x95 ,0x01 ,0x75 ,0x10 ,0x80 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x07 
// ,0xA1 ,0x0D ,0x01 ,0x85 ,0x01 ,0x05 ,0x07 ,0x19 ,0xE0 ,0x29 ,0xE7 
// ,0x15 ,0x00 ,0x25 ,0x01 ,0x75 ,0x01 ,0x95 ,0x08 ,0x81 ,0x02 ,0x95 
// ,0x01 ,0x75 ,0x0D ,0x08 ,0x81 ,0x03 ,0x95 ,0x05 ,0x75 ,0x01 ,0x05 
// ,0x08 ,0x19 ,0x01 ,0x29 ,0x05 ,0x91 ,0x02 ,0x95 ,0x01 ,0x75 ,0x03 
// ,0x91 ,0x03 ,0x95 ,0x0D ,0x06 ,0x75 ,0x08 ,0x15 ,0x00 ,0x25 ,0xFF 
// ,0x05 ,0x07 ,0x19 ,0x00 ,0x29 ,0xFF ,0x81 ,0x00 ,0xC0

0x06 ,0x00 ,0xFF ,0x09 ,0x00 ,0xA1 ,0x01 ,0xA1 ,0x02 ,0x85 ,0x5D 
,0x09 ,0x00 ,0x15 ,0x80 ,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x81 
,0x22 ,0xC0 ,0xA1 ,0x02 ,0x85 ,0x5E ,0x09 ,0x00 ,0x15 ,0x80 ,0x25 
,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x81 ,0x22 ,0xC0 ,0xA1 ,0x02 ,0x85 
,0x2B ,0x09 ,0x00 ,0x15 ,0x80 ,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 
,0x81 ,0x22 ,0xC0 ,0xC0 ,0x05 ,0x0C ,0x09 ,0x01 ,0xA1 ,0x01 ,0x85
,0x02 ,0x19 ,0x00 ,0x2A ,0x9C ,0x02 ,0x15 ,0x00 ,0x26 ,0x9C ,0x02 
,0x95 ,0x01 ,0x75 ,0x10 ,0x80 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x07 ,0xA1 
,0x01 ,0x85 ,0x01 ,0x05 ,0x07 ,0x19 ,0xE0 ,0x29 ,0xE7 ,0x15 ,0x00 
,0x25 ,0x01 ,0x75 ,0x01 ,0x95 ,0x08 ,0x81 ,0x02 ,0x95 ,0x01 ,0x75 
,0x08 ,0x81 ,0x03 ,0x95 ,0x05 ,0x75 ,0x01 ,0x05 ,0x08 ,0x19 ,0x01 
,0x29 ,0x05 ,0x91 ,0x02 ,0x95 ,0x01 ,0x75 ,0x03 ,0x91 ,0x03 ,0x95 
,0x06 ,0x75 ,0x08 ,0x15 ,0x00 ,0x25 ,0xFF ,0x05 ,0x07 ,0x19 ,0x00 
,0x29 ,0xFF ,0x81 ,0x00 ,0xC0
};
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
static const uint8_t report1_rr[] = {0x01, 0x01};
static const uint8_t report2_rr[] = {0x01, 0x02};
static const uint8_t report3_rr[] = {0x02, 0x01};
static const uint8_t report4_rr[] = {0x5D, 0x01};
static const uint8_t report5_rr[] = {0x5E, 0x02};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report_value[2];

//3a69Зa691804 - 0000 - 1000 - 8000 - 00805 - f587e61
static const uint8_t Unknow_3a6918[] = {0x61,0x7e,0x58,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x04,0x18,0x69,0x3a};
static const uint8_t Unknow_3a69a06[] = {0x61,0x7e,0x58,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x06,0x2a,0x69,0x3a};
static const uint8_t Unknow_3a69a06_value[2];

//4169726 f - 6861 - 4446 - 5553 - 657276696365
static const uint8_t Unknow_4169_65[] = {0x65,0x63,0x69,0x76,0x72,0x65,0x53,0x55,0x46,0x44,0x61,0x68,0x6f,0x72,0x69,0x41};
static const uint8_t Unknow_4169_64[] = {0x64,0x6e,0x61,0x6d,0x6d,0x6f,0x43,0x55,0x46,0x44,0x61,0x68,0x6f,0x72,0x69,0x41};
static const uint8_t Unknow_4169_64_value[2]; 
static const uint8_t Unknow_4169_70[] = {0x70,0x73,0x65,0x52,0x64,0x6d,0x43,0x55,0x46,0x44,0x61,0x68,0x6f,0x72,0x69,0x41};
static const uint8_t Unknow_4169_70_value[2]; 

static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];
const ATT_TABLE_TYPE Att_List[] =
{
    /* Generic Access */
	{1,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ACC},

	{2,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{3,2,CHRCTR_DEVICE_NAME,sizeof(device_name),device_name},
	{4,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{5,2,CHRCTR_APPEARANCE,sizeof(appearance_value),appearance_value},

	/* Generic Attribute */
	{6,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

	{7,2,GATT_CHARACTERISTIC,1,INDICATE_READ},
	{8,2,CHRCTR_SERVICE_CHANGED,sizeof(service_changed_value),service_changed_value},
	{9,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},

    /* Device Information */
	{10,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{11,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{12,2,CHRCTR_PNP_ID,sizeof(pnp_id_value),pnp_id_value},
	{13,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{14,2,CHRCTR_SYSTEM_ID,sizeof(system_id_value),system_id_value},
	{15,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{16,2,CHRCTR_MODEL_NUMBER,sizeof(model_number_value) - 1,model_number_value},
	{17,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{18,2,CHRCTR_Serial_number_String, sizeof(Serial_number_string_value) - 1, Serial_number_string_value},

	{19,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{20,2,CHRCTR_Firmware_Revision_String, sizeof(Firmware_Revision_String_value) - 1, Firmware_Revision_String_value},

	{21,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{22,2,CHRCTR_Hardwar_Revision_String, sizeof(Hardwar_Revision_String_value) - 1, Hardwar_Revision_String_value},

	{23,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{24,2,CHRCTR_Software_Revision_String, sizeof(Software_Revision_String_value) - 1, Software_Revision_String_value},
	{25,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{26,2,CHRCTR_Manufacturer_Name_String, sizeof(Manufacturer_Name_String_value) - 1, Manufacturer_Name_String_value},
	{27,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{28,2,CHRCTR_IEEE, sizeof(IEEE_value), IEEE_value},
	/* Google Voice */
    {29,2,PRIMARY_SERVICE,16,CHRCTR_ATV_Voice_Service_uuid_128},

	{30, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
    {31,16,CHRCTR_Write_uuid_128,0,NULL},
    {32,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {33,16,CHRCTR_Read_uuid_128,0,NULL},
    {34,2,CHARACTERTIC_CONFIGURATION,2,Voice_Read_CCC},
    {35,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {36,16,CHRCTR_Control_uuid_128,0,NULL},
    {37,2,CHARACTERTIC_CONFIGURATION,2,Voice_Ctrl_CCC},
	/* Battery Service */
    {38,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{39,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{40,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{41,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},
	/* Human Interface Device */
	{42,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{43,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{44,2,CHRCTR_REPORT_MAP,sizeof(report_map_value),report_map_value},

	{45,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{46,2,CHRCTR_HID_INFO,4,hid_info_value},

	{47,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{48,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},

	{49,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{50,2,CHRCTR_PROTOCOL_MODE,1,protocol_mode_value},

	{51,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{52,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,1,keyboard_input_report_value},
	{53,2,CHARACTERTIC_CONFIGURATION,2,KEYBOARD_INPUT_REPORT_CCC},
	
	{54,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{55,2,CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT,1,keyboard_output_report_value},

	{56,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, // ccc 1
	{57,2,CHRCTR_REPORT,1,report_value},
	{58,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{59,2,REPORT_REFERENCE,2,report1_rr},
	
	{60,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 2
	{61,2,CHRCTR_REPORT,1,report_value},
	{62,2,REPORT_REFERENCE,2,report2_rr},

	{63,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 3
	{64,2,CHRCTR_REPORT,1,report_value},
	{65,2,CHARACTERTIC_CONFIGURATION,2,report3_ccc},
	{66,2,REPORT_REFERENCE,2,report3_rr},

	{67,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 4
	{68,2,CHRCTR_REPORT,1,report_value},
	{69,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
	{70,2,REPORT_REFERENCE,2,report4_rr},

	{71,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 5
	{72,2,CHRCTR_REPORT,1,report_value},
	{73,2,REPORT_REFERENCE,2,report5_rr},

	/* unknow 3a */
	{74,2,PRIMARY_SERVICE,16,Unknow_3a6918},

	{75, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_PREMITTED},
	{76, 16, Unknow_3a69a06, 2, Unknow_3a69a06_value},

	/* unknow 416 */
	{77,2,PRIMARY_SERVICE,16,Unknow_4169_65},

	{78,2,GATT_CHARACTERISTIC, 1, WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{79,16,Unknow_4169_64, 2, Unknow_4169_64_value},
	{80,2,GATT_CHARACTERISTIC, 1, NOTIFY_PREMITTED_ONLY},
	{81,16,Unknow_4169_70, 2, Unknow_4169_70_value},
	{82,2,CHARACTERTIC_CONFIGURATION, 2, report_value},

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
