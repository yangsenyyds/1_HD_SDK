#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t INDICATE_READ[] = {INDICATE_PREMITTED | READ_PREMITTED};
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
static const uint8_t device_name[1];
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[2] ;
static const uint8_t CHRCTR_UUID_CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t UUID_CHRCTR_PPCP_value[] = {0x08,0x00,0x0B,0X00,0X00,0X00,0XE8,0X03} ;

//generic access
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[1] = {0};
static uint8_t Service_Changed_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0X01,0X3D,0x03,0x00,0x00,0x01,0x01};
static const uint8_t CHRCTR_FIRMWARE[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t FIRMWARE_value[] = "V1.11";
static const uint8_t CHRCTR_SOFTWARE[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t SOFTWARE_value[] = "R52GABO_T102FFv01.01";
static const uint8_t CHRCTR_HARDWARE[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t HARDWARE_value[] = "V1.0";
//human inf device
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t MODEL_NUMBER_value[] = "Report Protocol Mode";
static const uint8_t CHRCTR_BOOT_KEYBOARD_INPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_INPUT_REPORT)};
static const uint8_t BOOT_KEYBOARD_INPUT_REPORT_value[1] = {0x00};
static uint8_t BOOT_KEYBOARD_INPUT_REPORT_CCC[2];
static const uint8_t CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT)};
static const uint8_t BOOT_KEYBOARD_OUTPUT_REPORT_value[1] = {0x00};
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x02, 0x01};
static const uint8_t report2_rr[] = {0x01, 0x01};
static const uint8_t report3_rr[] = {0x01, 0x02};

static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report_value[2];
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t REPORT_MAP_value[] = {
0x05 ,0x01 ,0x09 ,0x06 ,0xA1 ,0x01 ,0x85 ,0x01 ,0x05 ,0x07, 
0x19 ,0xE0 ,0x29 ,0xE7 ,0x15 ,0x00 ,0x25 ,0x01 ,0x75 ,0x01, 
0x95 ,0x08 ,0x81 ,0x02 ,0x95 ,0x01 ,0x75 ,0x08 ,0x81 ,0x01, 
0x75 ,0x08 ,0x95 ,0x06 ,0x15 ,0x00 ,0x25 ,0xFF ,0x05 ,0x07, 
0x19 ,0x00 ,0x29 ,0x13 ,0x09 ,0x00 ,0x19 ,0x15 ,0x29 ,0xFF, 
0x81 ,0x00 ,0xC0 ,0x05 ,0x0C ,0x09 ,0x01 ,0xA1 ,0x01 ,0x85, 
0x02 ,0x95 ,0x01 ,0x75 ,0x10 ,0x15 ,0x00 ,0x26 ,0xFF ,0x03, 
0x19 ,0x00 ,0x2A ,0xFF ,0x03 ,0x81 ,0x00 ,0xC0};
static uint8_t REPORT_MAP_ccc[2];
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static uint8_t HID_INFO_value[] = {0x11,0x01,0x00,0x01};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t HID_CTRL_POINT_value[2];
//battery
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[2];
static uint8_t BATTERY_LEVEL_CCC[2];
/* 0d1912 */
static const uint8_t Unknown_Service_19120d[] = {0x12,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
static const uint8_t Unknown_Service_192b0c[] = {0x12,0x2b,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
static uint8_t chrctr_192b0c_value[2];
static uint8_t chrctr_192b0c_ccc[2];
static const uint8_t CHRCTR_CLIENT_User_Descriptions[] = {UUID2ARRAY(CLIENT_User_Descriptions)};

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
	{6,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{7,2,CHRCTR_UUID_CHRCTR_PPCP,sizeof(UUID_CHRCTR_PPCP_value),UUID_CHRCTR_PPCP_value},
	/* Generic Attribute */
	{8,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

	{9,2,GATT_CHARACTERISTIC,1,INDICATE_PREMITTED_ONLY},
	{10,2,CHRCTR_SERVICE_CHANGED,0,NULL},
	{11,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},

    /* Device Information */
	{12,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{13,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{14,2,CHRCTR_PNP_ID,sizeof(pnp_id_value),pnp_id_value},
	{15,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{16,2,CHRCTR_FIRMWARE,sizeof(FIRMWARE_value) - 1,FIRMWARE_value},
	{17,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{18,2,CHRCTR_SOFTWARE,sizeof(SOFTWARE_value) - 1,SOFTWARE_value},
	{19,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{20,2,CHRCTR_HARDWARE, sizeof(HARDWARE_value) - 1, HARDWARE_value},
	/* Human Interface Device */
	{22,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{23,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},//protocol mode
	{24,2,CHRCTR_MODEL_NUMBER,sizeof(MODEL_NUMBER_value) -1,MODEL_NUMBER_value},

	{25,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},//input
	{26,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,sizeof(BOOT_KEYBOARD_INPUT_REPORT_value),BOOT_KEYBOARD_INPUT_REPORT_value},
	{27,2,CHARACTERTIC_CONFIGURATION,2,BOOT_KEYBOARD_INPUT_REPORT_CCC},

	{28,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},//output
	{29,2,CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT,sizeof(BOOT_KEYBOARD_OUTPUT_REPORT_value),BOOT_KEYBOARD_OUTPUT_REPORT_value},
	
	{30,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 1
	{31,2,CHRCTR_REPORT,2,report_value},
	{32,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{33,2,REPORT_REFERENCE,2,report1_rr},

	{34,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED}, //ccc 2
	{35,2,CHRCTR_REPORT,2,report_value},
	{36,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
	{37,2,REPORT_REFERENCE,2,report2_rr},

	{38,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 3
	{39,2,CHRCTR_REPORT,2,report_value},
	{40,2,REPORT_REFERENCE,2,report3_rr},
	
	{41,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY}, //report map
	{42,2,CHRCTR_REPORT_MAP,sizeof(REPORT_MAP_value),REPORT_MAP_value},
	{43,2,EXTERNAL_REPORT,2,REPORT_MAP_ccc},
	{44,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY}, //hid info
	{45,2,CHRCTR_HID_INFO,sizeof(HID_INFO_value),HID_INFO_value},
	{46,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY}, //hid control point
	{47,2,CHRCTR_HID_CTRL_POINT,1,HID_CTRL_POINT_value},
	/* Battery Service */
    {48,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{49,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{50,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{51,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},
	/* 0d1912 */
    {52,2,PRIMARY_SERVICE,sizeof(Unknown_Service_19120d),Unknown_Service_19120d},

	{53,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{54,16,Unknown_Service_192b0c,2,chrctr_192b0c_value},
	{55,2,CHRCTR_CLIENT_User_Descriptions,2,chrctr_192b0c_ccc},
	/* Google Voice */
    {56,2,PRIMARY_SERVICE,16,CHRCTR_ATV_Voice_Service_uuid_128},

    {57,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
    {58,16,CHRCTR_Write_uuid_128,0,NULL},
    {59,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {60,16,CHRCTR_Read_uuid_128,0,NULL},
    {61,2,CHARACTERTIC_CONFIGURATION,2,Voice_Read_CCC},
    {62,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {63,16,CHRCTR_Control_uuid_128,0,NULL},
    {64,2,CHARACTERTIC_CONFIGURATION,2,Voice_Ctrl_CCC},

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
