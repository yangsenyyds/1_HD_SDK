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

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"SONY TV RC MIC 001"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t PPCP_value[] = {0x10,0x00,0x10,0x00,0x18,0x00,0x2C,0X01};
//generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[1] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {"Sony Corporation"};
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {"8.1.73.073"};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t PNP_ID_value[] = {0x02,0x4c,0x05,0xe8,0x0b,0x01,0x00};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_value[] = {"CH"};

//battery9
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[2];
static uint8_t BATTERY_LEVEL_CCC[2];

//human inf device
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = 
{
0x06 ,0x00 ,0xFF ,0x09 ,0x04 ,0xA1 ,0x01 ,0x85 ,0x02 ,0x15 ,0x00 ,0x25 ,0xFF 
,0x75 ,0x08 ,0x95 ,0x14 ,0x09 ,0x00 ,0x81 ,0x02 ,0x85 ,0x01 ,0x95 ,0x01 ,0x75 
,0x08 ,0x09 ,0x00 ,0x91 ,0x02 ,0xC0 ,0x05 ,0x01 ,0x09 ,0x06 ,0xA1 ,0x01 ,0x85 
,0x03 ,0x15 ,0x00 ,0x25 ,0x01 ,0x75 ,0x01 ,0x95 ,0x08 ,0x05 ,0x07 ,0x19 ,0xE0 
,0x29 ,0xE7 ,0x81 ,0x02 ,0x75 ,0x08 ,0x95 ,0x01 ,0x81 ,0x03 ,0x75 ,0x08 ,0x95 
,0x06 ,0x15 ,0x00 ,0x26 ,0xFF ,0x00 ,0x05 ,0x07 ,0x19 ,0x00 ,0x29 ,0x91 ,0x81 
,0x00 ,0xC0 ,0x05 ,0x0C ,0x09 ,0x01 ,0xA1 ,0x01 ,0x85 ,0x04 ,0x95 ,0x01 ,0x75 
,0x10 ,0x15 ,0x00 ,0x26 ,0xFF ,0x05 ,0x19 ,0x00 ,0x2A ,0xFF ,0x05 ,0x81 ,0x00 
,0xC0 ,0x06 ,0xC1 ,0xFF ,0x09 ,0x07 ,0xA1 ,0x01 ,0x85 ,0x40 ,0x15 ,0x00 ,0x26 
,0xFF ,0x00 ,0x75 ,0x08 ,0x95 ,0x0A ,0x09 ,0x07 ,0x81 ,0x02 ,0x85 ,0x40 ,0x95 
,0x01 ,0x75 ,0x08 ,0x15 ,0x01 ,0x26 ,0xFF ,0x00 ,0x09 ,0x07 ,0x91 ,0x02 ,0xC0
};
static uint8_t report_map_value_CCC[2] = {0x19,0x2A};

static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[] = {0x01, 0x01, 0x00, 0x00};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x03, 0x01};
static const uint8_t report2_rr[] = {0x04, 0x01};
static const uint8_t report3_rr[] = {0x40, 0x01};
static const uint8_t report4_rr[] = {0x02, 0x01};
static const uint8_t report5_rr[] = {0x40, 0x02};
static const uint8_t report6_rr[] = {0x01, 0x02};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report_value[2];
 // 010 2
 static const uint8_t Unknown_Service_0001[] ={0x12 ,0x19 ,0x0D ,0x0C ,0x0B ,0x0A ,0x09 ,0x08 ,0x07 ,0x06 ,0x05 
 ,0x04 ,0x03 ,0x02 ,0x01 ,0x00};
 static const uint8_t CHRCTR_CFBF_01[] = {
	0x12 ,0x2B ,0x0D ,0x0C ,0x0B ,0x0A ,0x09 ,0x08 
	,0x07 ,0x06 ,0x05 ,0x04 ,0x03 ,0x02 ,0x01 ,0x00 
 };
static const uint8_t CHRCTR_CFBF_01_value[2];

static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];

//factory test   d07c0000-9037-4f23-a1fb-220cbd11163a
static const uint8_t factory_primary_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x00,0x00,0x7C,0xD0};
static const uint8_t factory_characteristic_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x01,0x00,0x7C,0xD0};
static const uint8_t product_id_s[] = "610";
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
	{10,2,CHRCTR_SERVICE_CHANGED ,0,NULL},
	{11,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},

    /* Device Information 12 - 20 */
	{12,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{13,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{14,2,CHRCTR_Manufacturer_Name_String, sizeof(Manufacturer_Name_String_value) - 1, Manufacturer_Name_String_value},

	{15,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{16,2,CHRCTR_MODEL_NUMBER,sizeof(model_number_value) - 1,model_number_value},
	
	{17,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{18,2,CHRCTR_Firmware_Revision_String,sizeof(Firmware_Revision_String_value) - 1,Firmware_Revision_String_value},

	{19,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{20,2,CHRCTR_PNP_ID, sizeof(PNP_ID_value), PNP_ID_value},

	/* Battery Service 21 - 24 */
    {21,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{22,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{23,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{24,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},

	/* Human Interface Device  25-53*/
	{25,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{26,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED}, // ccc 1
	{27,2,CHRCTR_REPORT,1,report_value},
	{28,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{29,2,REPORT_REFERENCE,2,report1_rr},

	{30,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED}, // ccc 2
	{31,2,CHRCTR_REPORT,1,report_value},
	{32,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
	{33,2,REPORT_REFERENCE,2,report2_rr},

	{34,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED}, // ccc 3
	{35,2,CHRCTR_REPORT,1,report_value},
	{36,2,CHARACTERTIC_CONFIGURATION,2,report3_ccc},
	{37,2,REPORT_REFERENCE,2,report3_rr},

	{38,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED}, // ccc 4
	{39,2,CHRCTR_REPORT,1,report_value},
	{40,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
	{41,2,REPORT_REFERENCE,2,report4_rr},

	{42,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED}, // ccc 5
	{43,2,CHRCTR_REPORT,1,report_value},
	{44,2,REPORT_REFERENCE,2,report5_rr},


	{45,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED}, // ccc 6
	{46,2,CHRCTR_REPORT,1,report_value},
	{47,2,REPORT_REFERENCE,2,report6_rr},

	{48,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{49,2,CHRCTR_REPORT_MAP,sizeof(report_map_value),report_map_value},

	{50,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{51,2,CHRCTR_HID_INFO,1,hid_info_value},

	{52,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{53,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},

	/* 00 01 02 03  54-7*/
	{54,2,PRIMARY_SERVICE,16,Unknown_Service_0001},

	{55,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED}, // ccc 1
	{56,16,CHRCTR_REPORT,2,CHRCTR_CFBF_01},
	{57,2,CHARACTERTIC_CONFIGURATION,2,CHRCTR_CFBF_01_value},

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
