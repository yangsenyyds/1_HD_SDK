#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t WRITE_READ_PREMITTED[] = {WRITE_PREMITTED |READ_PREMITTED};
static const uint8_t INDICATE_READ[] = {INDICATE_PREMITTED | READ_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_READ_WRITE_PREMITTED[] = {NOTIFY_PREMITTED|READ_PREMITTED|WRITE_PREMITTED};
static const uint8_t NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {NOTIFY_PREMITTED|READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t WRITE_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};
// static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};


/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"BT22"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t PPCP_value[] = {0x08,0x00,0x08,0x00,0x63,0x00,0x90,0X01};
static const uint8_t CENTRAL_ADDRESS_RESOLUTION[] = {UUID2ARRAY(UUID_CENTRAL_ADDRESS_RESOLUTION)};
static const uint8_t ADDRESS_RESOLUTION_value[] = {"Address resolution not supported"};
//generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[1] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};

static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t PNP_ID_value[] = {0X01,0X01,0X2D,0X0A,0XC0,0X00,0X00};
//battery
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[2];
static uint8_t BATTERY_LEVEL_CCC[2];

//human inf device
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_PROTOCOL_MODE[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static const uint8_t CHRCTR_PROTOCOL_MODE_Value[] = {"Report Protocol Mode"};

static const uint8_t CHRCTR_BOOT_KEYBOARD_INPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_INPUT_REPORT)};
static const uint8_t CHRCTR_BOOT_KEYBOARD_INPUT_REPORT_Value[] = {0x00};

static const uint8_t CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT)};
static const uint8_t CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT_Value[] = {0x00};

static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = 
{
0x05 ,0x01 ,0x09 ,0x06 ,0xA1 ,0x01 ,0x85 ,0x01 
,0x05 ,0x07 ,0x19 ,0xE0 ,0x29 ,0xE7 ,0x15 ,0x00 
,0x25 ,0x01 ,0x75 ,0x01 ,0x95 ,0x08 ,0x81 ,0x02 
,0x95 ,0x01 ,0x75 ,0x08 ,0x81 ,0x03 ,0x95 ,0x05 
,0x75 ,0x01 ,0x05 ,0x08 ,0x19 ,0x01 ,0x29 ,0x05 
,0x91 ,0x02 ,0x95 ,0x01 ,0x75 ,0x03 ,0x91 ,0x03 
,0x95 ,0x06 ,0x75 ,0x08 ,0x15 ,0x00 ,0x25 ,0xFF 
,0x05 ,0x07 ,0x19 ,0x00 ,0x29 ,0xFF ,0x81 ,0x00 
,0xC0 ,0x05 ,0x0C ,0x09 ,0x01 ,0xA1 ,0x01 ,0x85 
,0x02 ,0x75 ,0x10 ,0x95 ,0x02 ,0x15 ,0x01 ,0x26 
,0xFF ,0x03 ,0x19 ,0x01 ,0x2A ,0xFF ,0x03 ,0x81 
,0x60 ,0x95 ,0x01 ,0x75 ,0x08 ,0x15 ,0x00 ,0x25 
,0xFF ,0x81 ,0x03 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x02 
,0xA1 ,0x01 ,0x85 ,0x03 ,0x09 ,0x01 ,0xA1 ,0x00 
,0x05 ,0x09 ,0x19 ,0x01 ,0x29 ,0x05 ,0x15 ,0x00 
,0x25 ,0x01 ,0x95 ,0x05 ,0x75 ,0x01 ,0x81 ,0x02 
,0x95 ,0x01 ,0x75 ,0x03 ,0x81 ,0x03 ,0x05 ,0x01 
,0x09 ,0x30 ,0x09 ,0x31 ,0x09 ,0x38 ,0x15 ,0x80 
,0x25 ,0x7F ,0x75 ,0x08 ,0x95 ,0x03 ,0x81 ,0x06 
,0x09 ,0x01 ,0x15 ,0x00 ,0x25 ,0x03 ,0x95 ,0x01 
,0x75 ,0x02 ,0xB1 ,0x02 ,0x95 ,0x01 ,0x75 ,0x06 
,0xB1 ,0x03 ,0xC0 ,0xC0
};
static uint8_t report_map_value_CCC[2] = {0x19,0x2A};

static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[] = {0x11, 0x01, 0x00, 0x01};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x02, 0x01};
static const uint8_t report2_rr[] = {0x01, 0x01};
static const uint8_t report3_rr[] = {0x01, 0x02};
static const uint8_t report4_rr[] = {0x03, 0x01};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report_value[2];
//00010203
static const uint8_t CHRCTR_ATV_0001_0203[] = {0x12, 0x19 ,0x0d, 0x0c ,0x0b ,0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03,0x02,0x01,0x00};
static const uint8_t CHRCTR_ATV_0001_0203_2b12[] = {0x12, 0x2b ,0x0d, 0x0c ,0x0b ,0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03,0x02,0x01,0x00};
static uint8_t ATV_0001_0203_2b12_value[1];
static uint8_t ATV_0001_0203_2b12_CCC[2];
static const uint8_t CHARACTERTIC_USER_DESCRIPTION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_USER_DESCRIPTION)};
//00001920
static const uint8_t CHRCTR_ATV_0000_1920[] = {0xfb ,0x34 ,0x9b ,0x5f ,0x80 ,0x00 ,0x00, 0x80 ,0x00 ,0x10 ,0x00 ,0x00 ,0x20 ,0x19 ,0x00 ,0x00};
static const uint8_t CHRCTR_ATV_0000_2b00[] = {0xfb ,0x34 ,0x9b ,0x5f ,0x80 ,0x00 ,0x00, 0x80 ,0x00 ,0x10 ,0x00 ,0x00 ,0x00 ,0x2b ,0x00 ,0x00};
static uint8_t ATV_0000_2b00_value[20];
static uint8_t ATV_0000_2b00_ccc[2];
static const uint8_t CHRCTR_ATV_0000_2b01[] = {0xfb ,0x34 ,0x9b ,0x5f ,0x80 ,0x00 ,0x00, 0x80 ,0x00 ,0x10 ,0x00 ,0x00 ,0x01 ,0x2b ,0x00 ,0x00};
static uint8_t ATV_0000_2b01_value[1];
// Google Voice
static const uint8_t CHRCTR_ATV_Voice_Service_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Write_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Read_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Control_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB};
static uint8_t Voice_Read_CCC[2];
static uint8_t Voice_Ctrl_CCC[2];

//0000aa0
static const uint8_t CHRCTR_ATV_0000_0aa0[] = {0x12, 0xa2, 0x4d ,0x2e, 0xfe ,0x14, 0x48, 0x8e, 0x93, 0xd2 ,0x17 ,0x3c ,0xa0 ,0x0a, 0x00, 0x00};
static const uint8_t CHRCTR_ATV_0000_b001[] = {0xfb ,0x34 ,0x9b ,0x5f ,0x80 ,0x00 ,0x00, 0x80 ,0x00 ,0x10 ,0x00 ,0x00 ,0x01 ,0xb0 ,0x00 ,0x00};
static uint8_t ATV_0000_b001_value[1];
static const uint8_t CHRCTR_ATV_0000_b002[] = {0xfb ,0x34 ,0x9b ,0x5f ,0x80 ,0x00 ,0x00, 0x80 ,0x00 ,0x10 ,0x00 ,0x00 ,0x02 ,0xb0 ,0x00 ,0x00};
static uint8_t ATV_0000_b002_value[1];
static uint8_t ATV_0000_b002_ccc[2];
//ota
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];

//factory test   d07c0000-9037-4f23-a1fb-220cbd11163a
static const uint8_t factory_primary_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x00,0x00,0x7C,0xD0};
static const uint8_t factory_characteristic_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x01,0x00,0x7C,0xD0};
static const uint8_t product_id_s[] = "951";
const ATT_TABLE_TYPE Att_List[] =
{
    /* Generic Access 1- 7*/ 
	{1,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ACC},

	{2,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{3,2,CHRCTR_DEVICE_NAME,sizeof(device_name) -1,device_name},
	{4,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{5,2,CHRCTR_APPEARANCE,sizeof(appearance_value),appearance_value},
	{6,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{7,2,CHRCTR_PPCP,sizeof(PPCP_value),PPCP_value},
	/* Generic Attribute   8 -11*/
	{8,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

	{9,2,GATT_CHARACTERISTIC,1,INDICATE_READ},
	{10,2,CHRCTR_SERVICE_CHANGED,0,NULL},
	{11,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},

    /* Device Information 12 - 15 */
	{12,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{13,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{14,2,CHRCTR_PNP_ID, sizeof(PNP_ID_value), PNP_ID_value},

	/* Human Interface Device */
	{15,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{16,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{17,2,CHRCTR_PROTOCOL_MODE,sizeof(CHRCTR_PROTOCOL_MODE_Value) - 1,CHRCTR_PROTOCOL_MODE_Value},

	{18,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{19,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,sizeof(CHRCTR_BOOT_KEYBOARD_INPUT_REPORT_Value),CHRCTR_BOOT_KEYBOARD_INPUT_REPORT_Value},
	{20,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},

	{21,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{22,2,CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT,sizeof(CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT_Value),CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT_Value},

	{23,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, // ccc 1
	{24,2,CHRCTR_REPORT,1,report_value},
	{25,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{26,2,REPORT_REFERENCE,2,report1_rr},
	
	{27,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 2
	{28,2,CHRCTR_REPORT,1,report_value},
	{29,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
	{30,2,REPORT_REFERENCE,2,report2_rr},

	{31,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 3
	{32,2,CHRCTR_REPORT,1,report_value},
	{33,2,REPORT_REFERENCE,2,report3_rr},

	{34,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 4
	{35,2,CHRCTR_REPORT,1,report_value},
	{36,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
	{37,2,REPORT_REFERENCE,2,report2_rr},

	{38,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{39,2,CHRCTR_REPORT_MAP,sizeof(report_map_value),report_map_value},
	{40,2,REPORT_REFERENCE,2,report_value},

	{41,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{42,2,CHRCTR_HID_INFO,sizeof(hid_info_value) - 1,hid_info_value},

	{43,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{44,2,CHRCTR_HID_CTRL_POINT,sizeof(hid_ctrl_point_value) - 1,hid_ctrl_point_value},	
	/* Battery Service */
    {45,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{46,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{47,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{48,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},
	//10203
	{49,2,PRIMARY_SERVICE,16,CHRCTR_ATV_0001_0203},

    {50,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {51,16,CHRCTR_ATV_0001_0203_2b12,1,ATV_0001_0203_2b12_value},
    {52,2,CHARACTERTIC_USER_DESCRIPTION,2,ATV_0001_0203_2b12_CCC},

	//1920
	{53,2,PRIMARY_SERVICE,16,CHRCTR_ATV_0000_1920},

    {54,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {55,16,CHRCTR_ATV_0000_2b00,sizeof(ATV_0000_2b00_value),ATV_0000_2b00_value},
    {56,2,CHARACTERTIC_CONFIGURATION,2,ATV_0000_2b00_ccc},	
    {57,2,GATT_CHARACTERISTIC,1,WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {58,16,CHRCTR_ATV_0000_2b01,sizeof(ATV_0000_2b01_value),ATV_0000_2b01_value},	
	//google voice
	{59,2,PRIMARY_SERVICE,16,CHRCTR_ATV_Voice_Service_uuid_128},

    {60,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
    {61,16,CHRCTR_Write_uuid_128,0,NULL},

    {62,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {63,16,CHRCTR_Read_uuid_128,0,NULL},
    {64,2,CHARACTERTIC_CONFIGURATION,2,Voice_Read_CCC},

    {65,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {66,16,CHRCTR_Control_uuid_128,0,NULL},
    {67,2,CHARACTERTIC_CONFIGURATION,2,Voice_Ctrl_CCC},

	//0aa0
	{68,2,PRIMARY_SERVICE,16,CHRCTR_ATV_0000_0aa0},

    {69,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
    {70,16,CHRCTR_ATV_0000_b001,sizeof(ATV_0000_b001_value),ATV_0000_b001_value},	
    {71,2,GATT_CHARACTERISTIC,1,WRITE_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
    {72,16,CHRCTR_ATV_0000_b002,sizeof(ATV_0000_b002_value),ATV_0000_b002_value},
    {73,2,CHARACTERTIC_CONFIGURATION,2,ATV_0000_2b00_ccc},	

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
