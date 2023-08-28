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
static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"OCN REMOTE"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t PPCP_value[] = {0x08,0x00,0x08,0x00,0x63,0x00,0x2C,0X01};
static const uint8_t CENTRAL_ADDRESS_RESOLUTION[] = {UUID2ARRAY(UUID_CENTRAL_ADDRESS_RESOLUTION)};
static const uint8_t ADDRESS_RESOLUTION_value[] = {"Address resolution not supported"};
//generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[1] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {"Sont Corporation"};
static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t system_id_value[] = {0x20,0xE7,0XB6,0XFF,0XFE,0XAF,0XD0,0X6D};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {"SMPL_NA"};

static const uint8_t CHRCTR_Serial_number_String[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t Serial_number_string_value[1];
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {"01.01.01.019"};
static const uint8_t CHRCTR_Hardwar_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t Hardwar_Revision_String_value[] = {"01"};

static const uint8_t CHRCTR_Software_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t Software_Revision_String_value[] = {"BL 197"};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t PNP_ID_value[1];
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t IEEE_value[1];
//battery
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[2];
static uint8_t BATTERY_LEVEL_CCC[2];
//Link_Loss
static const uint8_t Link_Loss_ATT[] = {UUID2ARRAY(UUID_Link_Loss_ATT)};
static const uint8_t CHRCTR_Alert_Level[] = {UUID2ARRAY(UUID_CHRCTR_Alert_Level)};
static const uint8_t Link_Loss_value[1];
//Immediate_Alert
static const uint8_t Immediate_Alert[] = {UUID2ARRAY(UUID_Immediate_Alert)};
static const uint8_t Immediate_Alert_value[1];
//UUID_Tx_Power
static const uint8_t Tx_Power[] = {UUID2ARRAY(UUID_Tx_Power)};
static const uint8_t CHRCTR_Tx_power_Level[] = {UUID2ARRAY(UUID_CHRCTR_Tx_power_Level)};
static const uint8_t Tx_power_Level_value[1];

//human inf device
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = 
{
 0x05 ,0x01 ,0x09 ,0x06 ,0xA1 ,0x01 ,0x85 ,0x03 ,0x05 ,0x07 
,0x19 ,0xE0 ,0x29 ,0xE7 ,0x15 ,0x00 ,0x25 ,0x01 ,0x75 ,0x01 
,0x95 ,0x08 ,0x81 ,0x02 ,0x95 ,0x01 ,0x75 ,0x08 ,0x81 ,0x03
,0x95 ,0x06 ,0x75 ,0x08 ,0x15 ,0x00 ,0x26 ,0xFF ,0x00 ,0x05 
,0x07 ,0x19 ,0x00 ,0x29 ,0xFF ,0x81 ,0x00 ,0xC0 ,0x05 ,0x0C 
,0x09 ,0x01 ,0xA1 ,0x01 ,0x85 ,0x04 ,0x95 ,0x01 ,0x75 ,0x10
,0x15 ,0x00 ,0x26 ,0xFF ,0x05 ,0x19 ,0x00 ,0x2A ,0xFF ,0x05
,0x81 ,0x00 ,0xC0 ,0x06 ,0xC1 ,0xFF ,0x09 ,0x07 ,0xA1 ,0x01 
,0xA1 ,0x02 ,0x85 ,0x40 ,0x95 ,0x01 ,0x75 ,0x08 ,0x15 ,0x00
,0x26 ,0xFF ,0x00 ,0x09 ,0x07 ,0x91 ,0x22 ,0xC0 ,0xC0};
static uint8_t report_map_value_CCC[2] = {0x19,0x2A};

static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[] = {0x11, 0x01, 0x00, 0x01};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x03, 0x01};
static const uint8_t report2_rr[] = {0x04, 0x01};
static const uint8_t report3_rr[] = {0x40, 0x02};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report_value[2];
//devece name
static const uint8_t CHRCTR_fb349b_180f[] = {0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x0a,0x18,0x00,0x00};
static const uint8_t CHRCTR_fb349b_2a19[] = {0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x50,0x2a,0x00,0x00};
static uint8_t ATV_CHRCTR_fb349b_2a19_value[] = {"OCN VC REMOTE 01"};
//battery 
static const uint8_t CHRCTR_fb349b_180a[] = {0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x0f,0x18,0x00,0x00};
static const uint8_t CHRCTR_fb349b_2a50[] = {0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x19,0x2a,0x00,0x00};
static uint8_t ATV_CHRCTR_fb349b_2a50_value[2];
static uint8_t ATV_CHRCTR_fb349b_2a50_CCC[2];

//ota
static const uint8_t CHRCTR_ota_1912[] = {0x12,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
static const uint8_t CHRCTR_ota_2b12[] = {0x12,0x2b,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
static uint8_t ATV_CHRCTR_ota_2b12_value[2];

// Google Voice
static const uint8_t CHRCTR_ATV_Voice_Service_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Write_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Read_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB};
static const uint8_t CHRCTR_Control_uuid_128[] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB};
static uint8_t Voice_Read_CCC[2];
static uint8_t Voice_Ctrl_CCC[2];

static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];
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
	{8,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{9,2,CENTRAL_ADDRESS_RESOLUTION,sizeof(ADDRESS_RESOLUTION_value) - 1,ADDRESS_RESOLUTION_value},
	/* Generic Attribute   8 -11*/
	{16,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

	{17,2,GATT_CHARACTERISTIC,1,INDICATE_READ},
	{18,2,CHRCTR_SERVICE_CHANGED,0,NULL},
	{19,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},

    /* Device Information 12 - 15 */
	{48,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{49,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{50,2,CHRCTR_Manufacturer_Name_String, sizeof(Manufacturer_Name_String_value) - 1, Manufacturer_Name_String_value},

	{51,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{52,2,CHRCTR_SYSTEM_ID,sizeof(system_id_value),system_id_value},

	{53,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{54,2,CHRCTR_MODEL_NUMBER,sizeof(model_number_value) - 1,model_number_value},

	{55,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{56,2,CHRCTR_Serial_number_String, 1, Serial_number_string_value},

	{57,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{58,2,CHRCTR_Firmware_Revision_String, sizeof(Firmware_Revision_String_value) - 1, Firmware_Revision_String_value},

	{59,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{60,2,CHRCTR_Hardwar_Revision_String, sizeof(Hardwar_Revision_String_value) - 1, Hardwar_Revision_String_value},

	{61,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{62,2,CHRCTR_Software_Revision_String, sizeof(Software_Revision_String_value) - 1, Software_Revision_String_value},

	{63,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{64,2,CHRCTR_IEEE, 0, NULL},

	{65,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{66,2,CHRCTR_PNP_ID, 0, NULL},
	/* Battery Service */
    {80,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{81,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{82,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{83,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},

	/* Link_Loss_ATT */
    {96,2,PRIMARY_SERVICE,2,Link_Loss_ATT},

	{97,2,GATT_CHARACTERISTIC,1,WRITE_READ_PREMITTED},
	{98,2,CHRCTR_Alert_Level,0,NULL},
	/* Immediate_Alert */
    {99,2,PRIMARY_SERVICE,2,Immediate_Alert},

	{100,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{101,2,CHRCTR_Alert_Level,1,Immediate_Alert_value},
	/* UUID_Tx_Power */
    {102,2,PRIMARY_SERVICE,2,Tx_Power},

	{103,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{104,2,CHRCTR_Tx_power_Level,0,NULL},

	/* Human Interface Device */
	{112,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{113,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{114,2,CHRCTR_HID_INFO,1,hid_info_value},

	{115,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{116,2,CHRCTR_REPORT_MAP,sizeof(report_map_value),report_map_value},
	{117,2,CHARACTERTIC_CONFIGURATION,2,report_map_value_CCC},

	{118,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{119,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},


	{120,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, // ccc 1
	{121,2,CHRCTR_REPORT,1,report_value},
	{122,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{123,2,REPORT_REFERENCE,2,report1_rr},
	
	{124,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 2
	{125,2,CHRCTR_REPORT,1,report_value},
	{126,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
	{127,2,REPORT_REFERENCE,2,report2_rr},

	{128,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 3
	{129,2,CHRCTR_REPORT,1,report_value},
	{130,2,REPORT_REFERENCE,2,report3_rr},

	//device name 
	{271,2,PRIMARY_SERVICE,16,CHRCTR_fb349b_180f},

    {272,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
    {273,16,CHRCTR_fb349b_2a19,sizeof(ATV_CHRCTR_fb349b_2a19_value) - 1,ATV_CHRCTR_fb349b_2a19_value},

	//battery
	{274,2,PRIMARY_SERVICE,16,CHRCTR_fb349b_180a},

    {275,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
    {276,16,CHRCTR_fb349b_2a50,1,ATV_CHRCTR_fb349b_2a50_value},
	{277,2,CHARACTERTIC_CONFIGURATION,2,ATV_CHRCTR_fb349b_2a50_CCC},

	//ota ocn
	{278,2,PRIMARY_SERVICE,16,CHRCTR_ota_1912},

    {279,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
    {280,16,CHRCTR_ota_2b12,1,ATV_CHRCTR_ota_2b12_value},

	//google voice
	{512,2,PRIMARY_SERVICE,16,CHRCTR_ATV_Voice_Service_uuid_128},

    {513,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
    {514,16,CHRCTR_Write_uuid_128,0,NULL},

    {515,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {516,16,CHRCTR_Read_uuid_128,0,NULL},
    {517,2,CHARACTERTIC_CONFIGURATION,2,Voice_Read_CCC},

    {518,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {519,16,CHRCTR_Control_uuid_128,0,NULL},
    {520,2,CHARACTERTIC_CONFIGURATION,2,Voice_Ctrl_CCC},

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
