#include "att_list.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED};
static const uint8_t WRITE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED|NOTIFY_PREMITTED |READ_PREMITTED};
static const uint8_t READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_INDICATE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|INDICATE_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t service_changed_value[1] = {0};
static uint8_t Service_Changed_CCC[2];
//generic access
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {0x58,0x52,0x54,0x32,0x36,0x30,0x76,0x35,0x37,0x30};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0x80,0x01};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t ppcp_value[] = {0x0C,0X00,0X0C,0X00,0X64,0X00,0X36,0X01};
static const uint8_t Central_Address_Resolution_WINDOW[] = {UUID2ARRAY(UUID_Central_Address_Resolution_WINDOW)};
static const uint8_t Central_Address_Resolution_WINDOW_value[1];
//battery service
static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t Battery_Level_value[2];
static uint8_t BATTERY_LEVEL_CCC[2];
//device information
static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_Manufacturer_Name_String[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t Manufacturer_Name_String_value[] = {0X4B,0X49,0X45};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t CHRCTR_MODEL_NUMBER_value[] = {0X58,0X52,0X54,0X32,0X36,0X30,0X76,0X32};
static const uint8_t CHRCTR_SERIAL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t CHRCTR_SERIAL_NUMBER_value[] = {0X52,0X54,0X4B,0X42,0X65,0X65,0X53,0X65,0X72,0X69,0X61,0X6C,0X4E,0X75,0X6D};
static const uint8_t CHRCTR_HARDWARE[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t CHRCTR_HARDWARE_value[] = {0X52,0X54,0X4B,0X42,0X65,0X65,0X48,0X61,0X72,0X64,0X77,0X61,0X72,0X65,0X52,0X65,0X76};
static const uint8_t CHRCTR_FIRMWARE[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t CHRCTR_FIRMWARE_value[] = {0X35,0X2E,0X37,0X2E,0X30};
static const uint8_t CHRCTR_SOFTWARE[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t CHRCTR_SOFTWARE_value[] = {0XBE,0XAD,0XDE};

static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t CHRCTR_SYSTEM_ID_value[] = {0x00,0x01,0x02,0x00,0x00,0x03,0x04,0x05};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t CHRCTR_IEEE_value[] = {0X52,0X54,0X4B,0X42,0X65,0X65,0X49,0X45,0X45,0X45,0X44,0X61,0X74,0X61,0X6C,0X69,0X73,0X74};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t CHRCTR_PNP_ID_value[] = {0X01,0X5D,0X00,0X01,0X00,0X03,0X00};
//human interface device
static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static const uint8_t CHRCTR_HID_INFO_value[1];
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static const uint8_t CHRCTR_HID_CTRL_POINT_value[1];
static const uint8_t CHRCTR_PROTOCOL_MODE[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static const uint8_t CHRCTR_PROTOCOL_MODE_value[1];
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t CHRCTR_REPORT_MAP_value[] = {
	0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x95, 0x08, 0x75, 0x08, 0x15,
	0x00, 0x25, 0xFF, 0x19, 0x00, 0x29, 0xFF, 0x81, 0x00, 0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02, 0x15,
	0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x18, 0x09, 0xB5, 0x09, 0xB6, 0x09, 0xB7, 0x09, 0xCD, 0x09, 0xE2, 0x09, 0x08,
	0x09, 0x07, 0x09, 0xE9, 0x09, 0xEA, 0x09, 0x34, 0x09, 0x42, 0x09, 0x44, 0x09, 0x41, 0x09, 0x45, 0x09, 0x43, 0x09,
	0x40, 0x0A, 0x94, 0x01, 0x0A, 0x21, 0x02, 0x0A, 0x23, 0x02, 0x0A, 0x24, 0x02, 0x0A, 0x25, 0x02, 0x0A, 0x26, 0x02,
	0x0A, 0x27, 0x02, 0x0A, 0xA2, 0x01, 0x81, 0x02, 0xC0, 0x06, 0x00, 0xFF, 0x09, 0x00, 0xA1, 0x01, 0x85, 0x5A, 0x95,
	0xFF, 0x75, 0x08, 0x15, 0x00, 0x25, 0xFF, 0x19, 0x00, 0x29, 0xFF, 0x81, 0x00, 0xC0, 0xC0};
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0x01, 0x01};
static const uint8_t report2_rr[] = {0x5A, 0x01};
static const uint8_t report3_rr[] = {0x5A, 0x02};
static const uint8_t report4_rr[] = {0x02, 0x02};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report_value[2];
//0000d1ff3c17
static const uint8_t CHRCTR_ATV_000d1ff[] = {0x12,0xa2,0x4d,0x2e,0xfe,0x14,0x48,0x8e,0x93,0xd2,0x17,0x3c,0xff,0xd1,0x00,0x00};
static const uint8_t CHRCTR_ATV_a001[] = {0x01,0xa0};
static uint8_t ATV_a001_value[2];
static uint8_t ATV_a001_ccc[2];
static const uint8_t CHRCTR_ATV_a002[] = {0x02, 0xa0};
static uint8_t ATV_a002_value[2];
//0000d1ff3c17
// 2141e100
static const uint8_t CHRCTR_ATV_000d0ff[] = {0x12,0xa2,0x4d,0x2e,0xfe,0x14,0x48,0x8e,0x93,0xd2,0x17,0x3c,0xff,0xd0,0x00,0x00};
static const uint8_t CHRCTR_ATV_ffd1[] = {0xd1,0xff};
static uint8_t ATV_ffd1_value[2];
static const uint8_t CHRCTR_ATV_ffd2[] = {0xd2,0xff};
static uint8_t ATV_ffd2_value[] = {0XA0,0X3B,0X01,0X0A,0X0E,0X7B};
static const uint8_t CHRCTR_ATV_ffd3[] = {0xd3,0xff};
static uint8_t ATV_ffd3_value[] = {0X01,0XE0,0X17,0X48};
static const uint8_t CHRCTR_ATV_ffd4[] = {0xd4,0xff};
static uint8_t ATV_ffd4_value[]= {0X75,0X00,0X00,0X00};
static const uint8_t CHRCTR_ATV_ffd5[] = {0xd5,0xff};
static uint8_t ATV_ffd5_value[2];
static const uint8_t CHRCTR_ATV_ffd8[] = {0xd8,0xff};
static uint8_t ATV_ffd8_value[2];
static const uint8_t CHRCTR_ATV_fff1[] = {0xf1,0xff};
static uint8_t ATV_fff1_value[] = {0X05,0X01,0X00,0X07,0X00,0X08,0X00,0X00,0XF0,0X0F,0X00,0X00};
static const uint8_t CHRCTR_ATV_fff2[] = {0xf2,0xff};
static uint8_t ATV_fff2_value[2];
static const uint8_t CHRCTR_ATV_ffe0[] = {0xe0,0xff};
static uint8_t ATV_ffe0_value[] = {0X00,0X00,0X00,0X01,0X01,0X00,0X00,0X00,0X01,0XE0,0X17,0X48,0X75,0X00,0X00,0X00};
//6287
static const uint8_t CHRCTR_ATV_62_000d0ff[] = {0x12,0xa2,0x4d,0x2e,0xfe,0x14,0x48,0x8e,0x93,0xd2,0x17,0x3c,0x87,0x62,0x00,0x00};
static const uint8_t CHRCTR_ATV_63_000d0ff[] = {0x12,0xa2,0x4d,0x2e,0xfe,0x14,0x48,0x8e,0x93,0xd2,0x17,0x3c,0x87,0x63,0x00,0x00};
static uint8_t ATV_63_000d0ff_value[2];
static const uint8_t CHRCTR_ATV_64_000d0ff[] = {0x12,0xa2,0x4d,0x2e,0xfe,0x14,0x48,0x8e,0x93,0xd2,0x17,0x3c,0x87,0x64,0x00,0x00};
static uint8_t ATV_64_000d0ff_value[2];
static uint8_t ATV_64_000d0ff_ccc[2];
//ota
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];
//factory test   d07c0000-9037-4f23-a1fb-220cbd11163a
static const uint8_t NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {NOTIFY_PREMITTED|READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t factory_primary_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x00,0x00,0x7C,0xD0};
static const uint8_t factory_characteristic_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x01,0x00,0x7C,0xD0};
static const uint8_t product_id_s[] = "703";
const ATT_TABLE_TYPE Att_List[] =
{
	/* Generic Attribute */
    {1,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},
	{2,2,GATT_CHARACTERISTIC,1,INDICATE_PREMITTED_ONLY},
	{3,2,CHRCTR_SERVICE_CHANGED,1,service_changed_value},
	{4,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_CCC},
    /* Generic Access */
    {5,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ACC},

	{6,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{7,2,CHRCTR_DEVICE_NAME,sizeof(device_name),device_name},
	{8,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{9,2,CHRCTR_APPEARANCE,2,appearance_value},
	{10,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{11,2,CHRCTR_PPCP,sizeof(ppcp_value),ppcp_value},
	
	{12,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{13,2,Central_Address_Resolution_WINDOW,sizeof(Central_Address_Resolution_WINDOW_value),Central_Address_Resolution_WINDOW_value},
	/* Battery Service */
    {14,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{15,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{16,2,CHRCTR_BATTERY_LEVEL,1,Battery_Level_value},
	{17,2,CHARACTERTIC_CONFIGURATION,2,BATTERY_LEVEL_CCC},
	//device information
	{18,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{19,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{20,2,CHRCTR_Manufacturer_Name_String, sizeof(Manufacturer_Name_String_value), Manufacturer_Name_String_value},
	{21,2,GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
	{22,2,CHRCTR_MODEL_NUMBER, sizeof(CHRCTR_MODEL_NUMBER_value), CHRCTR_MODEL_NUMBER_value},
	{23,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{24,2,CHRCTR_SERIAL_NUMBER, sizeof(CHRCTR_SERIAL_NUMBER_value), CHRCTR_SERIAL_NUMBER_value},
	{25,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{26,2,CHRCTR_HARDWARE, sizeof(CHRCTR_HARDWARE_value), CHRCTR_HARDWARE_value},
	{27,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{28,2,CHRCTR_FIRMWARE, sizeof(CHRCTR_FIRMWARE_value), CHRCTR_FIRMWARE_value},

	{29,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{30,2,CHRCTR_SOFTWARE, sizeof(CHRCTR_SOFTWARE_value), CHRCTR_SOFTWARE_value},
	{31,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{32,2,CHRCTR_SYSTEM_ID, sizeof(CHRCTR_SYSTEM_ID_value), CHRCTR_SYSTEM_ID_value},
	{33,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{34,2,CHRCTR_IEEE, sizeof(CHRCTR_IEEE_value), CHRCTR_IEEE_value},
	{35,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{36,2,CHRCTR_PNP_ID, sizeof(CHRCTR_PNP_ID_value), CHRCTR_PNP_ID_value},

	/* Human Interface Device */
	{37,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{38,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{39,2,CHRCTR_HID_INFO,1,CHRCTR_HID_INFO_value},

	{40,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{41,2,CHRCTR_HID_CTRL_POINT,sizeof(CHRCTR_HID_CTRL_POINT_value),CHRCTR_HID_CTRL_POINT_value},
	{42,2,GATT_CHARACTERISTIC,1,READ_WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{43,2,CHRCTR_PROTOCOL_MODE,sizeof(CHRCTR_PROTOCOL_MODE_value),CHRCTR_PROTOCOL_MODE_value},

	{44,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{45,2,CHRCTR_REPORT_MAP,sizeof(CHRCTR_REPORT_MAP_value),CHRCTR_REPORT_MAP_value},

	{46,2,GATT_CHARACTERISTIC,1,READ_WRITE_NOTIFY_PREMITTED}, // ccc 1
	{47,2,CHRCTR_REPORT,1,report_value},
	{48,2,CHARACTERTIC_CONFIGURATION,2,report1_ccc},
	{49,2,REPORT_REFERENCE,2,report1_rr},
	
	{50,2,GATT_CHARACTERISTIC,1,READ_WRITE_NOTIFY_PREMITTED}, //ccc 2
	{51,2,CHRCTR_REPORT,1,report_value},
	{52,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
	{53,2,REPORT_REFERENCE,2,report2_rr},

	{54,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED}, //ccc 3
	{55,2,CHRCTR_REPORT,1,report_value},
	{56,2,REPORT_REFERENCE,2,report3_rr},

	{57,2,GATT_CHARACTERISTIC,1,READ_WRITE_NOTIFY_PREMITTED}, //ccc 4
	{58,2,CHRCTR_REPORT,1,report_value},
	{59,2,CHARACTERTIC_CONFIGURATION,2,ATV_a001_ccc},
	{60,2,REPORT_REFERENCE,2,report4_rr},
	//0000d1ff3c17
	{61,2,PRIMARY_SERVICE,16,CHRCTR_ATV_000d1ff},

	{62,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{63,2,CHRCTR_ATV_a001,1,ATV_a001_value},
	{64,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
	{65,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{66,2,CHRCTR_ATV_a002,2,ATV_a002_value},
	//0000d1ff3c17
	{67,2,PRIMARY_SERVICE,16,CHRCTR_ATV_000d0ff},

	{68,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{69,2,CHRCTR_ATV_ffd1,1,ATV_ffd1_value},
	{70,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{71,2,CHRCTR_ATV_ffd2,sizeof(ATV_ffd2_value),ATV_ffd2_value},
	{72,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{73,2,CHRCTR_ATV_ffd3,sizeof(ATV_ffd3_value),ATV_ffd3_value},
	{74,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{75,2,CHRCTR_ATV_ffd4,sizeof(ATV_ffd4_value),ATV_ffd4_value},
	{76,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{77,2,CHRCTR_ATV_ffd5,1,ATV_ffd5_value},
	{78,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{79,2,CHRCTR_ATV_ffd8,1,ATV_ffd8_value},
	{80,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{81,2,CHRCTR_ATV_fff1,sizeof(ATV_fff1_value),ATV_fff1_value},
	{82,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{83,2,CHRCTR_ATV_fff2,1,ATV_fff2_value},
	{84,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{85,2,CHRCTR_ATV_ffe0,sizeof(ATV_ffe0_value),ATV_ffe0_value},
	//00006287
	{86,2,PRIMARY_SERVICE,16,CHRCTR_ATV_62_000d0ff},

	{87,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{88,16,CHRCTR_ATV_63_000d0ff,1,ATV_63_000d0ff_value},
	{89,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{90,16,CHRCTR_ATV_64_000d0ff,1,ATV_64_000d0ff_value},	
	{91,2,CHARACTERTIC_CONFIGURATION,1,ATV_64_000d0ff_ccc},	

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
