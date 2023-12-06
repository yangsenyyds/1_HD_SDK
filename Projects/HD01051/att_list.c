#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t NOTIFY_WRITE_PREMITTED_ONLY[] = {NOTIFY_PREMITTED |WRITE_PREMITTED};
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
static const uint8_t system_id_value[8];
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"Chromecast Remote"} ;
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {"ABBEY"};
static const uint8_t CHRCTR_CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t CHRCTR_PPCP_value[] = {0x08,0x00,0x0F,0x00,0x2B,0x00,0x20,0X03};
//generic access63
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[4] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0X01, 0xD1,0x18, 0x50,0x94,0x10,0x01};
static const uint8_t CHRCTR_Serial_number_String[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t Serial_number_string_value[] = {"17141HFAK3KHUT"};
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {"24.7"};
static const uint8_t CHRCTR_Hardwar_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t Hardwar_Revision_String_value[] = {"PVT/MP"};

static const uint8_t CHRCTR_Software_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t Software_Revision_String_value[] = {"24.7"};

static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {"Google lnc."};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t IEEE_value[] = {0XFE, 0X00, 0X65, 0X78, 0X70, 0X65 ,0X72, 0X69, 0X6D, 0X65, 0X6E, 0X74, 0X61 ,0X6C};
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
0x06 ,0x81 ,0xFF ,0x09 ,0x00 ,0xA1 ,0x80 ,0x85 ,0x10 ,0x15 ,0x00 ,0x25 
,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x09 ,0x00 ,0x81 ,0x02 ,0x95 ,0x04 ,0x09 
,0x00 ,0x91 ,0x02 ,0xC0 ,0x06 ,0x82 ,0xFF ,0x09 ,0x00 ,0xA1 ,0x81 ,0x85 
,0x0C ,0x15 ,0x00 ,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x14 ,0x09 ,0x00 ,0x81 
,0x02 ,0x09 ,0x00 ,0x91 ,0x02 ,0xC0 ,0x05 ,0x0C ,0x09 ,0x01 ,0xA1 ,0x01 
,0x85 ,0x01 ,0x75 ,0x10 ,0x95 ,0x01 ,0x15 ,0x00 ,0x26 ,0xFF ,0x03 ,0x19 
,0x00 ,0x2A ,0xFF ,0x03 ,0x81 ,0x00 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x06 ,0xA1 
,0x01 ,0x85 ,0x0B ,0x05 ,0x07 ,0x19 ,0xE0 ,0x29 ,0xE7 ,0x15 ,0x00 ,0x25 
,0x01 ,0x75 ,0x01 ,0x95 ,0x08 ,0x81 ,0x02 ,0x95 ,0x01 ,0x75 ,0x08 ,0x81 
,0x01 ,0x95 ,0x03 ,0x75 ,0x01 ,0x05 ,0x08 ,0x19 ,0x01 ,0x29 ,0x03 ,0x91 
,0x02 ,0x95 ,0x05 ,0x75 ,0x01 ,0x91 ,0x01 ,0x95 ,0x06 ,0x75 ,0x08 ,0x15 
,0x00 ,0x26 ,0xFF ,0x00 ,0x05 ,0x07 ,0x19 ,0x00 ,0x2A ,0xFF ,0x00 ,0x81 
,0x00 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x02 ,0xA1 ,0x01 ,0x85 ,0x03 ,0x09 ,0x01 
,0xA1 ,0x00 ,0x05 ,0x09 ,0x19 ,0x01 ,0x29 ,0x08 ,0x15 ,0x00 ,0x25 ,0x01 
,0x95 ,0x08 ,0x75 ,0x01 ,0x81 ,0x02 ,0x05 ,0x01 ,0x09 ,0x30 ,0x09 ,0x31 
,0x09 ,0x38 ,0x15 ,0x81 ,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x03 ,0x81 ,0x06 
,0xC0 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x80 ,0xA1 ,0x01 ,0x85 ,0x0D ,0x75 ,0x01 
,0x95 ,0x03 ,0x15 ,0x00 ,0x25 ,0x01 ,0x09 ,0x81 ,0x09 ,0x82 ,0x09 ,0x83 
,0x81 ,0x02 ,0x75 ,0x01 ,0x95 ,0x05 ,0x81 ,0x03 ,0xC0
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
static const uint8_t report1_rr[] = {0x0B, 0x01};
static const uint8_t report2_rr[] = {0x0B, 0x02};
static const uint8_t report3_rr[] = {0x01, 0x01};
static const uint8_t report4_rr[] = {0x03, 0x01};
static const uint8_t report5_rr[] = {0x0D, 0x01};
static const uint8_t report6_rr[] = {0x0C, 0x01};
static const uint8_t report7_rr[] = {0x0C, 0x02};
static const uint8_t report8_rr[] = {0x10, 0x01};
static const uint8_t report9_rr[] = {0x10, 0x02};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report5_ccc[2];
static uint8_t report6_ccc[2];
static uint8_t report7_ccc[2];
static uint8_t report8_ccc[2];
static uint8_t report9_ccc[2];
static uint8_t report_value[2];

//d343bfc0 5a21 4f05 bc 7d af 01 f6 17 b6 64
static const uint8_t Unknow_d343bfc0[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc ,0x05 ,0x4f ,0x21 ,0x5a, 0xc0 ,0xbf ,0x43 ,0xd3};
static const uint8_t Unknow_d343bfc1[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc ,0x05 ,0x4f ,0x21 ,0x5a, 0xc1 ,0xbf ,0x43 ,0xd3};
static const uint8_t Unknow_d343bfc2[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc ,0x05 ,0x4f ,0x21 ,0x5a, 0xc2 ,0xbf ,0x43 ,0xd3};
static const uint8_t Unknow_d343bfc3[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc ,0x05 ,0x4f ,0x21 ,0x5a, 0xc3 ,0xbf ,0x43 ,0xd3};
static const uint8_t Unknow_d343bfc4[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc ,0x05 ,0x4f ,0x21 ,0x5a, 0xc4 ,0xbf ,0x43 ,0xd3};
static const uint8_t Unknow_d343bfc5[] = {0x64, 0xb6, 0x17, 0xf6, 0x01, 0xaf, 0x7d, 0xbc ,0x05 ,0x4f ,0x21 ,0x5a, 0xc5 ,0xbf ,0x43 ,0xd3};
static const uint8_t Unknow_d343bfc1_value[2]; 
static const uint8_t Unknow_d343bfc2_value[2]; 
static const uint8_t Unknow_d343bfc3_value[2]; 
static const uint8_t Unknow_d343bfc4_value[2]; 
static const uint8_t Unknow_d343bfc5_value[2]; 
//dfu
static const uint8_t CHRCTR_FE59[] = {UUID2ARRAY(UUID_CHRCTR_FE59)};
static const uint8_t Unknow_90001[] = {0x50, 0xea ,0xda ,0x30 ,0x88 ,0x83, 0xb8,0x9f, 0x60, 0x4f ,0x15, 0xf3, 0x01, 0x00 ,0xc9, 0x8e};
static const uint8_t Unknow_90001_value[2]; 
static const uint8_t Unknow_90002[] = {0x50, 0xea ,0xda ,0x30 ,0x88 ,0x83, 0xb8,0x9f, 0x60, 0x4f ,0x15, 0xf3, 0x02, 0x00 ,0xc9, 0x8e};
static const uint8_t Unknow_90002_value[2]; 
static const uint8_t Unknow_00003[] = {0xfb, 0x34, 0x9b ,0x5f ,0x80, 0x00, 0x00,0x80, 0x00, 0x10, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00};
static const uint8_t Unknow_00003_value[2]; 

static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];

//factory test   d07c0000-9037-4f23-a1fb-220cbd11163a

static const uint8_t factory_primary_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x00,0x00,0x7C,0xD0};
static const uint8_t factory_characteristic_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x01,0x00,0x7C,0xD0};
static const uint8_t product_id_s[] = "1051";
const ATT_TABLE_TYPE Att_List[] =
{
    /* Generic Access */
	{1,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ACC},

	{2,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{3,2,CHRCTR_DEVICE_NAME,sizeof(device_name),device_name},
	{4,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{5,2,CHRCTR_APPEARANCE,sizeof(appearance_value),appearance_value},
	{6,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{7,2,CHRCTR_CHRCTR_PPCP,sizeof(CHRCTR_PPCP_value),CHRCTR_PPCP_value},

    /* Device Information */
	{8,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},
	{9,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{10,2,CHRCTR_Manufacturer_Name_String, sizeof(Manufacturer_Name_String_value) - 1, Manufacturer_Name_String_value},
	{11,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{12,2,CHRCTR_MODEL_NUMBER,sizeof(model_number_value) - 1,model_number_value},	
	{13,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{14,2,CHRCTR_Serial_number_String, sizeof(Serial_number_string_value) - 1, Serial_number_string_value},	
	{15,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},

	{16,2,CHRCTR_Hardwar_Revision_String, sizeof(Hardwar_Revision_String_value) - 1, Hardwar_Revision_String_value},
	{17,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{18,2,CHRCTR_Firmware_Revision_String, sizeof(Firmware_Revision_String_value) - 1, Firmware_Revision_String_value},
	{19,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{20,2,CHRCTR_Software_Revision_String, sizeof(Software_Revision_String_value) - 1, Software_Revision_String_value},
	{21,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{22,2,CHRCTR_SYSTEM_ID,sizeof(system_id_value),system_id_value},	

	{23,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{24,2,CHRCTR_IEEE, sizeof(IEEE_value), IEEE_value},

	{25,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{26,2,CHRCTR_PNP_ID,sizeof(pnp_id_value),pnp_id_value},

	/* Battery Service */
    {27,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{28,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{29,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{30,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},

	/* Human Interface Device */
	{31,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{32,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{33,2,CHRCTR_HID_INFO,4,hid_info_value},

	{34,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{35,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},

	{36,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{37,2,CHRCTR_REPORT_MAP,sizeof(report_map_value),report_map_value},

	{38,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{39,2,CHRCTR_PROTOCOL_MODE,sizeof(protocol_mode_value),protocol_mode_value},

	{40,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{41,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,1,keyboard_input_report_value},
	{42,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},

	{43,2,CHARACTERTIC_CONFIGURATION,2,KEYBOARD_INPUT_REPORT_CCC},

	{44,2,CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT,1,keyboard_output_report_value},

	{45,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, // ccc 1
	{46,2,CHRCTR_REPORT,1,report_value},
	{47,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{48,2,REPORT_REFERENCE,2,report1_rr},
	
	{49,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 2
	{50,2,CHRCTR_REPORT,1,report_value},
	{51,2,REPORT_REFERENCE,2,report2_rr},

	{52,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 3
	{53,2,CHRCTR_REPORT,1,report_value},
	{54,2,CHARACTERTIC_CONFIGURATION,2,report3_ccc},
	{55,2,REPORT_REFERENCE,2,report3_rr},

	{56,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 4
	{57,2,CHRCTR_REPORT,1,report_value},
	{58,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
	{59,2,REPORT_REFERENCE,2,report4_rr},
	{60,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 5
	{61,2,CHRCTR_REPORT,1,report_value},
	{62,2,CHARACTERTIC_CONFIGURATION,2,report5_ccc},
	{63,2,REPORT_REFERENCE,2,report5_rr},

	{64,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 6
	{65,2,CHRCTR_REPORT,1,report_value},
	{66,2,CHARACTERTIC_CONFIGURATION,2,report6_ccc},
	{67,2,REPORT_REFERENCE,2,report6_rr},

	{68,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 7
	{69,2,CHRCTR_REPORT,1,report_value},
	{70,2,REPORT_REFERENCE,2,report7_rr},

	{71,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 8
	{72,2,CHRCTR_REPORT,1,report_value},
	{73,2,CHARACTERTIC_CONFIGURATION,2,report8_ccc},
	{74,2,REPORT_REFERENCE,2,report8_rr},		

	{75,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 9
	{76,2,CHRCTR_REPORT,1,report_value},
	{77,2,CHARACTERTIC_CONFIGURATION,2,report9_ccc},		

	/* Google Voice */
    {78,2,PRIMARY_SERVICE,16,CHRCTR_ATV_Voice_Service_uuid_128},

	{79, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
    {80,16,CHRCTR_Write_uuid_128,0,NULL},
    {81,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {82,16,CHRCTR_Read_uuid_128,0,NULL},
    {83,2,CHARACTERTIC_CONFIGURATION,2,Voice_Read_CCC},
    {84,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {85,16,CHRCTR_Control_uuid_128,0,NULL},
    {86,2,CHARACTERTIC_CONFIGURATION,2,Voice_Ctrl_CCC},
	/* unknow d3 */
	{87,2,PRIMARY_SERVICE,16,Unknow_d343bfc0},

	{88, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
	{89, 16, Unknow_d343bfc1, 2, Unknow_d343bfc1_value},

	{90, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
	{91, 16, Unknow_d343bfc2, 2, Unknow_d343bfc2_value},
	{92, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
	{93, 16, Unknow_d343bfc3, 2, Unknow_d343bfc3_value},
	{94, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
	{95, 16, Unknow_d343bfc4, 2, Unknow_d343bfc4_value},
	{96, 2, GATT_CHARACTERISTIC, 1, WRITE_PREMITTED_ONLY},
	{97, 16, Unknow_d343bfc5, 2, Unknow_d343bfc5_value},
	{98, 2,CHARACTERTIC_CONFIGURATION, 2, report_value},
	/* unknow fe59 */
	{99,2,PRIMARY_SERVICE,2,CHRCTR_FE59},

	{100,2,GATT_CHARACTERISTIC, 1, NOTIFY_WRITE_PREMITTED_ONLY},
	{101,16,Unknow_90001, 2, Unknow_90001_value},
	{102,2,CHARACTERTIC_CONFIGURATION, 2, report_value},
	{103,2,GATT_CHARACTERISTIC, 1, READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{104,16,Unknow_90002, 2, Unknow_90002_value},
	{105,2,GATT_CHARACTERISTIC, 1, NOTIFY_READ_WRITE_PREMITTED},
	{106,16,Unknow_00003, 2, Unknow_00003_value},	
	{107,2,CHARACTERTIC_CONFIGURATION, 2, report_value},
	/* OTA Service */
	{65500,2,PRIMARY_SERVICE,2,OTA_SERVICE},

	{65501,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
	{65502,2,CHRCTR_OTA_NOTIFY,0,NULL},
	{65503,2,CHARACTERTIC_CONFIGURATION,2,chrctr_ota_ccc},

	{65504,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{65505,2,CHRCTR_OTA_WRITE,0,NULL},

	/*factory test*/
	{65530,2,PRIMARY_SERVICE,16,factory_primary_uuid},

	{65531, 2, GATT_CHARACTERISTIC, 1, NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{65532, 16, factory_characteristic_uuid, sizeof(product_id_s), product_id_s},

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
