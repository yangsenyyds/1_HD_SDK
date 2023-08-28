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
static const uint8_t system_id_value[] = {0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {0x57, 0x4F, 0x57, 0X20, 0X74, 0X76, 0X2B, 0X20, 0X52, 0X43, 0X55, 0X00} ;
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {0x57, 0x4F, 0X57, 0X20, 0X74, 0X76, 0X2B, 0X20, 0X52, 0X43, 0X55, 0X00};
//generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[4] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0x01,0x0D,0x00,0x38,0x38,0x10,0x01};
static const uint8_t CHRCTR_Serial_number_String[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t Serial_number_string_value[] = {0x32,0x30,0x32,0x30,0x30,0x38,0x32,0x34,0x30,0x30,0x30,0x31,0x00};
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {0x53,0x44,0x4B,0X31,0X2E,0X32,0X2E,0X37,0X32,0X2E,0X32,0X30,32,0X30,0X31,0X31,0X32,0X36,0X00};
static const uint8_t CHRCTR_Hardwar_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t Hardwar_Revision_String_value[] = {0X57,0X4E,0X46,0X31,0X37,0X31,0X00};

static const uint8_t CHRCTR_Software_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t Software_Revision_String_value[] = {0X53,0X57,0X56,0X30,0X31,0X31,0X30,0X00};

static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {0X4E,0X41,0X4E,0X4F,0X53,0X49,0X43,0X00};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t IEEE_value[] = {0XFE,0X00,0X65,0X78,0X70,0X65,0X72,0X69,0X6D,0X65,0X6E,0X74,0X61,0X6C};
// Google Voice
static const uint8_t CHRCTR_ATV_Voice_Service_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Write_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Read_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Control_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB};
static uint8_t Voice_Read_CCC[2];
static uint8_t Voice_Ctrl_CCC[2];

//72657672 6573 6174
static const uint8_t Unknow_7265[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X6F,0X74,0X61,0X73,0X65,0X72,0X76,0X65,0X72};
static const uint8_t Unknow_5F747570[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X6F,0X74,0X61,0X69,0X6E,0X70,0X75,0X74,0X5F};
static const uint8_t Unknow_5F747570_value[2];
static const uint8_t Unknow_747570[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X6F,0X74,0X61,0X6F,0X75,0X74,0X70,0X75,0X74};
static const uint8_t Unknow_747570_value[2];
//76726573 6f69
static const uint8_t Unknow_76726573[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X61,0X75,0X64,0X69,0X6F,0X73,0X65,0X72,0X76};
static const uint8_t Unknow_5F5F[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X61,0X75,0X64,0X69,0X6F,0X69,0X6E,0X5F,0X5F};
static const uint8_t Unknow_5F5F_value[1];
static const uint8_t Unknow_5F74[] = {0X6E, 0X61, 0X6E, 0X6F, 0X73, 0X69, 0X63,0X61,0X75,0X64,0X69,0X6F,0X6F,0X75,0X74,0X5F};
static const uint8_t Unknow_5F74_value[2];
//65636976
static const uint8_t Unknow_65636976[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X69,0X72,0X73,0X65,0X72,0X76,0X69,0X63,0X65};
static const uint8_t Unknow_6E69[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X73,0X75,0X70,0X65,0X72,0X69,0X72,0X69,0X6E};
static const uint8_t Unknow_6E69_value[1];
static const uint8_t Unknow_746F7269[] = {0X6E,0X61,0X6E,0X6F,0X73,0X69,0X63,0X73,0X75,0X70,0X65,0X72,0X69,0X72,0X6F,0X74};
static const uint8_t Unknow_5F74_value[2];
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
 0X06 ,0X00 ,0XFF ,0X09 ,0X00 ,0XA1 ,0X01 ,0XA1 
,0X02 ,0X85 ,0X5F ,0X09 ,0X00 ,0X15 ,0X80 ,0X25 
,0X7F ,0X75 ,0X08 ,0X95 ,0X14 ,0X81 ,0X22 ,0XC0 

,0XA1 ,0X02 ,0X85 ,0X5D ,0X09 ,0X00 ,0X15 ,0X80 
,0X25 ,0X7F ,0X75 ,0X08 ,0X95 ,0XFF ,0X81 ,0X22 
,0XC0 ,

0XA1 ,0X02 ,0X85,0X5E ,0X09 ,0X00 ,0X15 ,0X80 ,0X25 ,0X7F ,0X75 
,0X08 ,0X95 ,0XFF ,0X81 ,0X22 ,0XC0 

,0XA1 ,0X02 ,0X85 ,0X2B ,0X09 ,0X00 ,0X15 ,0X80
,0X25 ,0X7F ,0X75 ,0X08 ,0X95 ,0XFF ,0X81 ,0X22 
,0XC0 ,0XC0 ,0X05 ,0X0C ,0X09 ,0X01 ,0XA1 ,0X01 
,0X85 ,0X02 ,0X15 ,0X00 ,0X25 ,0X01
,0X75 ,0X01 ,0X95 ,0X18 ,0X09 ,0X30 ,0X0A ,0X24 
,0X02 ,0X0A ,0X23 ,0X02 ,0X0A ,0X21 ,0X02 ,0X09 
,0X41 ,0X09 ,0X42 ,0X09 ,0X43 ,0X09 
,0X44 ,0X09 ,0X45 ,0X09 ,0XE9 ,0X09 ,0XEA ,0X09 
,0XE2 ,0X0A ,0XA2 ,0X01 ,0X09 ,0XB4 ,0X09 ,0XB3 
,0X09 ,0XB2 ,0X09 ,0X9C ,0X09 ,0X9D
,0X0A ,0X2A ,0X02 ,0X09 ,0XCD ,0X09 ,0X08 ,0X0A 
,0XBD ,0X01 ,0X09 ,0X83 ,0X09 ,0XCE ,0X81 ,0X02 
,0XC0 ,0X05 ,0X01 ,0X09 ,0X07 ,0XA1 ,0X01 ,0X85 
,0X01 ,0X05 ,0X07 ,0X19 ,0XE0 ,0X29 ,0XE7 ,0X15 
,0X00 ,0X25 ,0X01 ,0X75 ,0X01 ,0X95 ,0X08 ,0X81 
,0X02 ,0X95 ,0X01 ,0X75 ,0X08 ,0X81 ,0X03 ,0X95 
,0X05 ,0X75 ,0X01 ,0X05 ,0X08 ,0X19 ,0X01 ,0X29 
,0X05 ,0X91 ,0X02 ,0X95 ,0X01 ,0X75 ,0X03 ,0X91 
,0X03 ,0X95 ,0X06 ,0X75 ,0X08 ,0X15 ,0X00 ,0X25 
,0XFF ,0X05 ,0X07 ,0X19 ,0X00 ,0X29 ,0XFF ,0X81 
,0X00 ,0XC0

//  0X06 ,0X00 ,0XFF ,0X09 ,0X00 ,0XA1 ,0X01 ,0XA1 
// ,0X02 ,0X85 ,0X5F ,0X09 ,0X00 ,0X15 ,0X80 ,0X25 
// ,0X7F ,0X75 ,0X08 ,0X95 ,0X14 ,0X81 ,0X22 ,0XC0 

// ,0XA1 ,0X02 ,0X85 ,0X5D , 0x95 ,0x02 ,0x75 ,0x10 ,0x15 ,0x00 ,0x26 ,0x9C ,0x02 ,0x19 ,0x00 ,0x2A ,0x9C ,0x02 
// ,0x81 ,0x00, 0x06 ,0x00 , 0xFF ,0x09 ,0x00

// ,0XA1 ,0X02 ,0X85,0X5E ,0X09 ,0X00 ,0X15 ,0X80 ,0X25 ,0X7F ,0X75 
// ,0X08 ,0X95 ,0XFF ,0X81 ,0X22 ,0XC0 

// ,0XA1 ,0X02 ,0X85 ,0X2B ,0X09 ,0X00 ,0X15 ,0X80
// ,0X25 ,0X7F ,0X75 ,0X08 ,0X95 ,0XFF ,0X81 ,0X22 
// ,0XC0 ,0XC0 ,0X05 ,0X0C ,0X09 ,0X01 ,0XA1 ,0X01 
// ,0X85 ,0X02 ,0X15 ,0X00 ,0X25 ,0X01
// ,0X75 ,0X01 ,0X95 ,0X18 ,0X09 ,0X30 ,0X0A ,0X24 
// ,0X02 ,0X0A ,0X23 ,0X02 ,0X0A ,0X21 ,0X02 ,0X09 
// ,0X41 ,0X09 ,0X42 ,0X09 ,0X43 ,0X09 
// ,0X44 ,0X09 ,0X45 ,0X09 ,0XE9 ,0X09 ,0XEA ,0X09 
// ,0XE2 ,0X0A ,0XA2 ,0X01 ,0X09 ,0XB4 ,0X09 ,0XB3 
// ,0X09 ,0XB2 ,0X09 ,0X9C ,0X09 ,0X9D
// ,0X0A ,0X2A ,0X02 ,0X09 ,0XCD ,0X09 ,0X08 ,0X0A 
// ,0XBD ,0X01 ,0X09 ,0X83 ,0X09 ,0XCE ,0X81 ,0X02 
// ,0XC0 ,0X05 ,0X01 ,0X09 ,0X07 ,0XA1 ,0X01 ,0X85 
// ,0X01 ,0X05 ,0X07 ,0X19 ,0XE0 ,0X29 ,0XE7 ,0X15 
// ,0X00 ,0X25 ,0X01 ,0X75 ,0X01 ,0X95 ,0X08 ,0X81 
// ,0X02 ,0X95 ,0X01 ,0X75 ,0X08 ,0X81 ,0X03 ,0X95 
// ,0X05 ,0X75 ,0X01 ,0X05 ,0X08 ,0X19 ,0X01 ,0X29 
// ,0X05 ,0X91 ,0X02 ,0X95 ,0X01 ,0X75 ,0X03 ,0X91 
// ,0X03 ,0X95 ,0X06 ,0X75 ,0X08 ,0X15 ,0X00 ,0X25 
// ,0XFF ,0X05 ,0X07 ,0X19 ,0X00 ,0X29 ,0XFF ,0X81 
// ,0X00 ,0XC0
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
static const uint8_t report2_rr[2];
static const uint8_t report3_rr[] = {0x02, 0x01};
static const uint8_t report4_rr[] = {0x2B, 0x01};
static const uint8_t report5_rr[] = {0x5D, 0x01};
static const uint8_t report6_rr[2];
static const uint8_t report7_rr[] = {0x5F, 0x01};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report5_ccc[2];
static uint8_t report6_ccc[2];
static uint8_t report7_ccc[2];
static uint8_t report_value[2];



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
	{16,2,CHRCTR_MODEL_NUMBER,sizeof(model_number_value),model_number_value},
	{17,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{18,2,CHRCTR_Serial_number_String, sizeof(Serial_number_string_value), Serial_number_string_value},

	{19,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{20,2,CHRCTR_Firmware_Revision_String, sizeof(Firmware_Revision_String_value), Firmware_Revision_String_value},

	{21,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{22,2,CHRCTR_Hardwar_Revision_String, sizeof(Hardwar_Revision_String_value), Hardwar_Revision_String_value},

	{23,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{24,2,CHRCTR_Software_Revision_String, sizeof(Software_Revision_String_value), Software_Revision_String_value},
	{25,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{26,2,CHRCTR_Manufacturer_Name_String, sizeof(Manufacturer_Name_String_value), Manufacturer_Name_String_value},
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

	/* unknow 72657672 6573 6174 */
	{38,2,PRIMARY_SERVICE,16,Unknow_7265},

	{39,2,GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
	{40,16,Unknow_5F747570, 2, Unknow_5F747570_value},
	{41,2,CHARACTERTIC_CONFIGURATION,2,report_value},
	{42,2,GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{43,16,Unknow_747570, 2, Unknow_747570_value},

	/* unknow 76726573 6f69 */
	{44,2,PRIMARY_SERVICE,16,Unknow_76726573},

	{45,2,GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
	{46,16,Unknow_5F5F, 2, Unknow_5F5F_value},
	{47,2,CHARACTERTIC_CONFIGURATION,2,report_value},
	{48,2,GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{49,16,Unknow_5F74, 2, Unknow_5F74_value},

	/* Unknow_65636976 */
	{50,2,PRIMARY_SERVICE,16,Unknow_65636976},

	{51,2,GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
	{52,16,Unknow_6E69, 2, Unknow_6E69_value},
	{53,2,CHARACTERTIC_CONFIGURATION,2,report_value},
	{54,2,GATT_CHARACTERISTIC, 1, READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{55,16,Unknow_746F7269, 2, Unknow_5F74_value},
	/* Battery Service */
    {56,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{57,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{58,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{59,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},
	/* Human Interface Device */
	{60,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{61,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{62,2,CHRCTR_REPORT_MAP,sizeof(report_map_value),report_map_value},

	{63,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{64,2,CHRCTR_HID_INFO,4,hid_info_value},

	{65,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{66,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},

	{67,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{68,2,CHRCTR_PROTOCOL_MODE,1,protocol_mode_value},

	{69,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{70,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,1,keyboard_input_report_value},
	{71,2,CHARACTERTIC_CONFIGURATION,2,KEYBOARD_INPUT_REPORT_CCC},
	
	{72,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{73,2,CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT,1,keyboard_output_report_value},

	{74,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, // ccc 1
	{75,2,CHRCTR_REPORT,1,report_value},
	{76,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{77,2,REPORT_REFERENCE,2,report1_rr},
	
	{78,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 2
	{79,2,CHRCTR_REPORT,1,report_value},
	{80,2,REPORT_REFERENCE,2,report2_rr},

	{81,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 3
	{82,2,CHRCTR_REPORT,1,report_value},
	{83,2,CHARACTERTIC_CONFIGURATION,2,report3_ccc},
	{84,2,REPORT_REFERENCE,2,report3_rr},

	{85,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 4
	{86,2,CHRCTR_REPORT,1,report_value},
	{87,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
	{88,2,REPORT_REFERENCE,2,report4_rr},

	{89,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 5
	{90,2,CHRCTR_REPORT,1,report_value},
	{91,2,CHARACTERTIC_CONFIGURATION,2,report5_ccc},
	{92,2,REPORT_REFERENCE,2,report5_rr},

	{93,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 6
	{94,2,CHRCTR_REPORT,1,report_value},
	{95,2,REPORT_REFERENCE,2,report6_rr},

	{96,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 7
	{97,2,CHRCTR_REPORT,1,report_value},
	{98,2,CHARACTERTIC_CONFIGURATION,2,report7_ccc},
	{99,2,REPORT_REFERENCE,2,report7_rr},	

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
