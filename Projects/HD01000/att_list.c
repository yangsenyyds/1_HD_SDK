#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
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
static const uint8_t device_name[] = {0x46, 0x6c, 0x6f, 0x77, 0x52, 0x65, 0x6d, 0x6f, 0x74, 0x65, 0x2d, 0x76, 0x32};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t ppcp_value[] = {0x09, 0x00, 0x09, 0x00, 0x00, 0x00, 0x90, 0x01};

static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[1];
static uint8_t Service_Changed_CCC[2];

static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0x02, 0x7A, 0x05, 0x21, 0x00, 0x04, 0x02};

static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_PROTOCOL_MODE[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static uint8_t protocol_mode_value[] = {0x52, 0x65, 0x70, 0x6f, 0x72, 0x74, 0x20, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x63, 0x6f, 0x6c, 0x20, 0x4d, 0x6f, 0x64, 0x65};
static const uint8_t CHRCTR_BOOT_KEYBOARD_INPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_INPUT_REPORT)};
static const uint8_t keyboard_input_report_value[] = {0x00};
static uint8_t KEYBOARD_INPUT_REPORT_CCC[2];
static const uint8_t CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT)};
static uint8_t keyboard_output_report_value[1];
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x02, 0x01};
static const uint8_t report2_rr[] = {0x01, 0x01};
static const uint8_t report3_rr[] = {0x01, 0x02};
static const uint8_t report4_rr[] = {0x05, 0x03};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report_value[2];
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = {0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x01, 0x95, 0x05, 0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x01, 0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x25, 0xF1, 0x05, 0x07, 0x19, 0x00, 0x29, 0xF1, 0x81, 0x00, 0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02, 0x19, 0x00, 0x2A, 0x9C, 0x02, 0x15, 0x00, 0x26, 0x9C, 0x02, 0x95, 0x02, 0x75, 0x10, 0x81, 0x00, 0xC0};
static const uint8_t external_rr[] = {0x00, 0x00};
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t hid_info_value[] = {0x11, 0x01, 0x00, 0x01};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point_value[1];

static const uint8_t SERVICE_CBC0[] = {0x34, 0x79, 0xE5, 0x84, 0x08, 0x62, 0x82, 0x9B, 0x2E, 0x40, 0xAF, 0x76, 0x85, 0xE1, 0xC0, 0xCB};
static const uint8_t CHRCTR_0F3E[] = {0xB9, 0x26, 0xC1, 0xC2, 0x8B, 0x20, 0xCF, 0xAD, 0xFC, 0x42, 0x87, 0xC6, 0xD8, 0xAB, 0x3E, 0x0F};
static uint8_t chrctr_0f3e_value[20];
static uint8_t CHRCTR_0F3E_CCC[2];
static const uint8_t CHRCTR_8357[] = {0x2C, 0x44, 0x37, 0x1E, 0xE0, 0x8B, 0x51, 0xB4, 0x6A, 0x41, 0xFC, 0x10, 0x89, 0x33, 0x57, 0x83};
static uint8_t chrctr_8357_value[20];
static uint8_t CHRCTR_8357_CCC[2];

static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t battery_leval_value[] = {100};
static uint8_t BATTERY_LEVEL_CCC[2];

static const uint8_t SERVICE_65FA[] = {0xDC, 0x01, 0x41, 0xC4, 0xB7, 0x67, 0xBA, 0xB5, 0xFE, 0x4E, 0xCA, 0xE8, 0x13, 0x95, 0xFA, 0x65};
static const uint8_t CHRCTR_20D6[] = {0x52, 0x7F, 0xAE, 0xCF, 0xC5, 0xC6, 0xEC, 0xAF, 0x11, 0x4D, 0x7B, 0x1F, 0xC7, 0x95, 0xD6, 0x20};
static const uint8_t CHRCTR_9c98[] = {0x22, 0xF2, 0xC8, 0x62, 0x1C, 0xBA, 0x26, 0x8F, 0x61, 0x43, 0xE4, 0x3D, 0x55, 0xFA, 0x98, 0x9C};
static uint8_t chrctr_9c98_value[20];
static uint8_t CHRCTR_9C98_CCC[2];

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
	{3,2,CHRCTR_DEVICE_NAME,13,device_name},
	{4,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{5,2,CHRCTR_APPEARANCE,1,appearance_value},
	{6,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{7,2,CHRCTR_PPCP,8,ppcp_value},

	/* Generic Attribute */
    {8,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

	{9,2,GATT_CHARACTERISTIC,1,INDICATE_PREMITTED_ONLY},
	{10,2,CHRCTR_SERVICE_CHANGED,1,service_changed_value},
	{11,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},
	
    /* Device Information */
	{12,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{13,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{14,2,CHRCTR_PNP_ID,7,pnp_id_value},
	
	/* Human Interface Device */
	{15,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{16,2,GATT_INCLUDE,2,SERVICE_BATTERY},
	{17,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{18,2,CHRCTR_PROTOCOL_MODE,20,protocol_mode_value},
	{19,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{20,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,1,keyboard_input_report_value},
	{21,2,CHARACTERTIC_CONFIGURATION,2,KEYBOARD_INPUT_REPORT_CCC},
	{22,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
	{23,2,CHRCTR_BOOT_KEYBOARD_INPUT_REPORT,1,keyboard_output_report_value},
	{24,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{25,2,CHRCTR_REPORT,2,report_value},
	{26,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
    {27,2,REPORT_REFERENCE,2,report1_rr},
    {28,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {29,2,CHRCTR_REPORT,2,report_value},
	{30,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
    {31,2,REPORT_REFERENCE,2,report2_rr},
    {32,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {33,2,CHRCTR_REPORT,2,report_value},
    {34,2,REPORT_REFERENCE,2,report3_rr},
    {35,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {36,2,CHRCTR_REPORT,2,report_value},
	{37,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
    {38,2,REPORT_REFERENCE,2,report4_rr},
	{39,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{40,2,CHRCTR_REPORT_MAP,90,report_map_value},
	{41,2,EXTERNAL_REPORT,2,external_rr},
	{42,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{43,2,CHRCTR_HID_INFO,4,hid_info_value},
	{44,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{45,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point_value},

	/* Unknown Service CBC0 */
	{46,2,PRIMARY_SERVICE,16,SERVICE_CBC0},

	{47,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_PREMITTED},
    {48,16,CHRCTR_0F3E,20,chrctr_0f3e_value},
    {49,2,CHARACTERTIC_CONFIGURATION,2,CHRCTR_0F3E_CCC},
    {50,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {51,16,CHRCTR_8357,20,chrctr_8357_value},
    {52,2,CHARACTERTIC_CONFIGURATION,2,CHRCTR_8357_CCC},

    /* Battery Service */
    {53,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{54,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{55,2,CHRCTR_BATTERY_LEVEL,1,battery_leval_value},
	{56,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},

	/* Unknown Service 65FA */
	{57,2,PRIMARY_SERVICE,16,SERVICE_65FA},

	{58,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
    {59,16,CHRCTR_20D6,0,NULL},
    {60,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {61,16,CHRCTR_9c98,20,chrctr_9c98_value},
    {62,2,CHARACTERTIC_CONFIGURATION,2,CHRCTR_9C98_CCC},

    /* Google Voice */
    {63,2,PRIMARY_SERVICE,16,CHRCTR_ATV_Voice_Service_uuid_128},

    {64,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
    {65,16,CHRCTR_Write_uuid_128,0,NULL},
    {66,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {67,16,CHRCTR_Read_uuid_128,0,NULL},
    {68,2,CHARACTERTIC_CONFIGURATION,2,Voice_Read_CCC},
    {69,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {70,16,CHRCTR_Control_uuid_128,0,NULL},
    {71,2,CHARACTERTIC_CONFIGURATION,2,Voice_Ctrl_CCC},

	/* Unknown Service C88D */
	{72,2,PRIMARY_SERVICE,16,SERVICE_C88D},

	{73,2,GATT_CHARACTERISTIC,1,NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {74,16,CHRCTR_EA3D,0,NULL},
    {75,2,CHARACTERTIC_CONFIGURATION,2,CHRCTR_EA3D_CCC},
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
