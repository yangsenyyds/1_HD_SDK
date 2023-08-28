#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {WRITE_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t NOTIFY_PREMITTED_ONLY[] = {NOTIFY_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED};
static const uint8_t WRITE_NOTIFY_PREMITTED[] = {WRITE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_WITHOUT_RESPONSE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_INDICATE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | INDICATE_PREMITTED};
static const uint8_t READ_WRITE_NOTIFY_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED | WRITE_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED | WRITE_WITHOUT_RESPONSE_PREMITTED};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t DEVICE_NAME_VALUE[] = {0x43, 0x48,0x5F,0x48,0x44,0x30,0x30,0x30,0x31};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance_value[] = {0xC0, 0x03};
static const uint8_t CHRCTR_PPCP[] = {UUID2ARRAY(UUID_CHRCTR_PPCP)};
static const uint8_t ppcp_value[] = {0x08, 0x00, 0x0C, 0x00, 0x63, 0x00, 0xA0, 0x0F};

static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_Service_Change[] = {UUID2ARRAY(Service_Change)};
static uint8_t Service_Change_value[1];
static const uint8_t CHRCTR_CCC_1[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static uint8_t ccc_1_value[2];

static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_PNP_ID[] = {UUID2ARRAY(UUID_CHRCTR_PNP_ID)};
static const uint8_t pnp_id_value[] = {0x02, 0x5A, 0x1d, 0x80, 0xc0, 0x02, 0x00};
static const uint8_t SERVICE_Manufacturer_Name_str[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t manufacture_number_string_value[] = {0x58, 0x49, 0x48, 0x41, 0x4F, 0x5F, 0x54, 0x65, 0x63, 0x68};
static const uint8_t CHRCTR_MODEL_NUMBER_String[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number_string_value[] = {0x58, 0x49, 0x48, 0x41, 0x4F};
static const uint8_t CHRCTR_Serial_number_String[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t Serial_number_string_value[] = {0x73, 0x65, 0x72, 0x69, 0x61, 0x6C};
static const uint8_t CHRCTR_Hardwar_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t Hardwar_Revision_String_value[] = {0x48, 0x41, 0x52, 0x44, 0x57, 0x41, 0x52, 0x45, 0x33, 0x30};
static const uint8_t CHRCTR_Firmware_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t Firmware_Revision_String_value[] = {0x30, 0x2E, 0x31, 0x30};
static const uint8_t CHRCTR_Software_Revision_String[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t Software_Revision_String_value[] = {0x56, 0x30, 0x2E, 0x31, 0x2E, 0x38};
static const uint8_t CHRCTR_System_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t System_ID_value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t IEEE_value[] = {0x00};

static const uint8_t SERVICE_Human_Interface_Device[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_Protocol_Mode[] = {UUID2ARRAY(UUID_CHRCTR_PROTOCOL_MODE)};
static const uint8_t Protocol_Mode_value[] = {0x00};
static const uint8_t CHRCTR_Boot_keyboard_Input_Report[] = {UUID2ARRAY(Boot_keyboard_Input_Report)};
static const uint8_t Boot_keyboard_Input_Report_value[] = {0x00};
static const uint8_t CHRCTR_CCC_2[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static uint8_t CCC_2_Value[2];
static const uint8_t CHRCTR_Boot_keyboard_Output_Report[] = {UUID2ARRAY(Boot_keyboard_Output_Report)};
static const uint8_t Boot_keyboard_Output_Report_value[] = {0x00};
static const uint8_t CHRCTR_Report[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t CHRCTR_CCC[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t CHRCTR_Report_Reference[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};
static const uint8_t CHRCTR_Report_Map[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t hid_report_map[] = {
    0x05, 0x0C,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x02,
    0x75, 0x10,
    0x95, 0x02,
    0x15, 0x01,
    0x26, 0x8C, 0x02,
    0x19, 0x01,
    0x2A, 0x8C, 0x02,
    0x81, 0x00,
    0xC0,

    0x05, 0x01,
    0x09, 0x06,
    0xA1, 0x01,
    0x85, 0x01,
    0x05, 0x07,
    0x19, 0xE0,
    0x29, 0xE7,
    0x15, 0x00,
    0x25, 0x01,
    0x75, 0x01,
    0x95, 0x08,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x08,
    0x81, 0x01,
    0x95, 0x05,
    0x75, 0x01,
    0x05, 0x08,
    0x19, 0x01,
    0x29, 0x05,
    0x91, 0x02,
    0x95, 0x01,
    0x75, 0x03,
    0x91, 0x01,
    0x95, 0x06,
    0x75, 0x08,
    0x15, 0x00,
    0x25, 0xF1,
    0x05, 0x07,
    0x19, 0x00,
    0x29, 0xF1,
    0x81, 0x00,
    0xC0,

    0x05, 0x0C,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x0C,
    0x95, 0x14,
    0x75, 0x08,
    0x15, 0x00,
    0x26, 0xFF, 0x00,
    0x81, 0x00,
    0xC0,

    0x05, 0x0C,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x0A,
    0x95, 0x14,
    0x75, 0x08,
    0x15, 0x00,
    0x26, 0xFF, 0x00,
    0x81, 0x00,
    0xC0,

    0x05, 0x0C,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x0B,
    0x95, 0x14,
    0x75, 0x08,
    0x15, 0x00,
    0x26, 0xFF, 0x00,
    0x81, 0x00,
    0xC0,

    0x05, 0x01,
    0x09, 0x02,
    0xA1, 0x01,
    0x85, 0x03,
    0x09, 0x01,
    0xA1, 0x00,
    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x03,
    0x15, 0x00,
    0x25, 0x01,
    0x75, 0x01,
    0x95, 0x05,
    0x81, 0x02,
    0x75, 0x03,
    0x95, 0x01,
    0x81, 0x01,
    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08,
    0x95, 0x02,
    0x81, 0x06,
    0x09, 0x38,
    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08,
    0x95, 0x01,
    0x81, 0x06,
    0xC0,
    0xC0,

    0x06, 0x01, 0xFF,
    0x09, 0x01,
    0xA1, 0x01,
    0x85, 0x05,
    0x09, 0x05,
    0x15, 0x00,
    0x26, 0xFF, 0x00,
    0x75, 0x08,
    0x95, 0x04,
    0xB1, 0x02,
    0xC0
};
static const uint8_t CHRCTR_External_Report_Refer[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static uint8_t report_map_value[2];
static const uint8_t CHRCTR_HID_Info[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
static uint8_t HID_Info_value[2];
static const uint8_t CHRCTR_HID_Control_Point[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t HID_Control_Point_value[2];
static const uint8_t report1_rr[] = {0x02, 0x01};
static const uint8_t report2_rr[] = {0x01, 0x01};
static const uint8_t report3_rr[] = {0x01, 0x02};
static const uint8_t report4_rr[] = {0x03, 0x01};
static const uint8_t report5_rr[] = {0x05, 0x01};
static const uint8_t report6_rr[] = {0x0A, 0x01};
static const uint8_t report7_rr[] = {0x0B, 0x01};
static const uint8_t report8_rr[] = {0x0C, 0x01};
static uint8_t report1_ccc[2];
static uint8_t report2_ccc[2];
static uint8_t report3_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report5_ccc[2];
static uint8_t report6_ccc[2];
static uint8_t report7_ccc[2];
static uint8_t report8_ccc[2];
static uint8_t report_value[2];

static const uint8_t CHRCTR_Battery_Service[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_Battery_Level[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static const uint8_t CHRCTR_Battery_ccc[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static uint8_t Battery_Level_value[2];
static uint8_t Battery_ccc_value[2];

// Unknow Service 00010203-0405-0607-0809-0A0B0C0D1912
static const uint8_t CHRCTR_Unknow_Service_00010203[] = {0x12, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static const uint8_t CHRCTR_Unknow_Service_00010203_2b_12[] = {0x12, 0x2b, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static uint8_t Unknow_Service_00010203_2b_12_value[] = {0x00};
static const uint8_t CHRCTR_Unknow_Service_CCC[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static uint8_t Unknow_Service_CCC_value[2];
static const uint8_t CHRCTR_Characteristic_User_Des[] = {UUID2ARRAY(Characteristic_User_Des)};
static uint8_t Characteristic_User_Des_value[1];

//0000 d1ff 3c
static const uint8_t CHRCTR_Unknow_Service_0000d1ff3c[] = {0x12, 0xa2, 0x4d, 0x2e, 0xfe, 0x14, 0x48, 0x8e, 0x93, 0xd2, 0x17, 0x3c, 0xff, 0xd1, 0x00, 0x00};
static const uint8_t CHRCTR_Unknow_Service_0000a002[] = {0xfd, 0x34, 0x9d, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0xa0, 0x00, 0x00};
static uint8_t Unknow_Service_0000a002_value[] = {0x01};

// Google Voice
static const uint8_t CHRCTR_ATV_Voice_Service_uuid_128[] = {0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x01,0x00,0x5E,0xAB};
static const uint8_t CHRCTR_Write_uuid_128[] = {0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x02,0x00,0x5E,0xAB};
static const uint8_t CHRCTR_Read_uuid_128[] = {0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x03,0x00,0x5E,0xAB};
static const uint8_t CHRCTR_Control_uuid_128[] = {0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x04,0x00,0x5E,0xAB};
static uint8_t voice_Read_ccc[2];
static uint8_t voice_Control_ccc[2];

const ATT_TABLE_TYPE Att_List[] =
{
    /* Generic Access */
    {1, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ACC},

    {2, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
    {3, 2, CHRCTR_DEVICE_NAME, 9, DEVICE_NAME_VALUE},
    {4, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {5, 2, CHRCTR_APPEARANCE, 2, appearance_value},
    {6, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {7, 2, CHRCTR_PPCP, 8, ppcp_value},

    /* Generic Attribute */
    {8, 2, PRIMARY_SERVICE, 2, SERVICE_GENERIC_ATT},

    {9, 2, GATT_CHARACTERISTIC, 2, INDICATE_PREMITTED_ONLY},
    {10, 2, CHRCTR_Service_Change, 1, Service_Change_value},
    {11, 2, CHRCTR_CCC_1, 2, ccc_1_value},

    /* Device Information */
    {12, 2, PRIMARY_SERVICE, 2, SERVICE_DEVICE_INFO},
    
    {13, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {14, 2, CHRCTR_PNP_ID, 7, pnp_id_value},
    {15, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {16, 2, SERVICE_Manufacturer_Name_str, 10, manufacture_number_string_value},
    {17, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {18, 2, CHRCTR_MODEL_NUMBER_String, 5, model_number_string_value},
    {19, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {20, 2, CHRCTR_Serial_number_String, 6, Serial_number_string_value},
    {21, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {22, 2, CHRCTR_Hardwar_Revision_String, 10, Hardwar_Revision_String_value},
    {23, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {24, 2, CHRCTR_Firmware_Revision_String, 4, Firmware_Revision_String_value},
    {25, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {26, 2, CHRCTR_Software_Revision_String, 6, Software_Revision_String_value},
    {27, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {28, 2, CHRCTR_System_ID, 8, System_ID_value},
    {29, 2, GATT_CHARACTERISTIC, 1, READ_PREMITTED_ONLY},
    {30, 2, CHRCTR_IEEE, 1, IEEE_value},

    /* Human Interface Device */
    {31, 2, PRIMARY_SERVICE, 2, SERVICE_Human_Interface_Device}, // human

    {33, 2, GATT_CHARACTERISTIC, 2, READ_WITHOUT_RESPONSE_PREMITTED}, //protocol mode
    {34, 2, CHRCTR_Protocol_Mode, 1, Protocol_Mode_value},
    {35, 2, GATT_CHARACTERISTIC, 2, READ_NOTIFY_PREMITTED}, //boot keyboard input
    {36, 2, CHRCTR_Boot_keyboard_Input_Report, 1, Boot_keyboard_Input_Report_value},
    {37, 2, CHRCTR_CCC_2, 2, CCC_2_Value},
    {38, 2, GATT_CHARACTERISTIC, 2, READ_WRITE_WITHOUT_RESPONSE_PREMITTED}, //boot keyboard output
    {39, 2, CHRCTR_Boot_keyboard_Output_Report, 1, Boot_keyboard_Output_Report_value},

    {40, 2, GATT_CHARACTERISTIC, 2, READ_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 1
    {41, 2, CHRCTR_Report, 1, report_value},
    {42, 2, CHRCTR_CCC, 2, report1_ccc},
    {43, 2, CHRCTR_Report_Reference, 2, report1_rr},
    
    {44, 2, GATT_CHARACTERISTIC, 2, READ_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 2
    {45, 2, CHRCTR_Report, 1, report_value},
    {46, 2, CHRCTR_CCC, 2, report2_ccc},
    {47, 2, CHRCTR_Report_Reference, 2, report2_rr},
    {48, 2, GATT_CHARACTERISTIC, 2, READ_WRITE_WITHOUT_RESPONSE_PREMITTED}, // UUID_CHRCTR_REPORT 3
    {49, 2, CHRCTR_Report, 2, report_value},
    {50, 2, CHRCTR_Report_Reference, 2, report3_rr},
    {51, 2, GATT_CHARACTERISTIC, 2, READ_WRITE_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 4
    {52, 2, CHRCTR_Report, 2, report_value},
    {53, 2, CHRCTR_CCC, 2, report4_ccc},
    {54, 2, CHRCTR_Report_Reference, 2, report4_rr},
    {55, 2, GATT_CHARACTERISTIC, 2, READ_WRITE_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 5
    {56, 2, CHRCTR_Report, 2, report_value},
    {57, 2, CHRCTR_CCC, 2, report5_ccc},
    {58, 2, CHRCTR_Report_Reference, 2, report5_rr},
    {59, 2, GATT_CHARACTERISTIC, 2, READ_PREMITTED_ONLY}, // UUID_CHRCTR_REPORT MAP
    {60, 2, CHRCTR_Report_Map, 237, hid_report_map},
    {61, 2, CHRCTR_External_Report_Refer, 2, report_map_value},
    {62, 2, GATT_CHARACTERISTIC, 2, READ_PREMITTED_ONLY}, //hid info
    {63, 2, CHRCTR_HID_Info, 2, HID_Info_value},
    {64, 2, GATT_CHARACTERISTIC, 2, WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY}, //hid control point
    {65, 2, CHRCTR_HID_Control_Point, 2, HID_Control_Point_value},
    {66, 2, GATT_CHARACTERISTIC, 2, READ_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 6
    {67, 2, CHRCTR_Report, 2, report_value},
    {68, 2, CHRCTR_CCC, 2, report6_ccc},
    {69, 2, CHRCTR_Report_Reference, 2, report6_rr},
    {70, 2, GATT_CHARACTERISTIC, 2, READ_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 7
    {71, 2, CHRCTR_Report, 2, report_value},
    {72, 2, CHRCTR_CCC, 2, report7_ccc},
    {73, 2, CHRCTR_Report_Reference, 2, report7_rr},
    {74, 2, GATT_CHARACTERISTIC, 2, READ_NOTIFY_PREMITTED}, // UUID_CHRCTR_REPORT 8
    {75, 2, CHRCTR_Report, 2, report_value},
    {76, 2, CHRCTR_CCC, 2, report8_ccc},
    {77, 2, CHRCTR_Report_Reference, 2, report8_rr},

    /* Battery Service */
    {78, 2, PRIMARY_SERVICE, 2, CHRCTR_Battery_Service},

    {79, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
    {80, 2, CHRCTR_Battery_Level, 1, Battery_Level_value},
    {81, 2, CHRCTR_Battery_ccc, 2, Battery_ccc_value},

    /* UUID	00010203-0405-0607-0809-0A0B0C0D1912 */
    {82, 2, PRIMARY_SERVICE, 16, CHRCTR_Unknow_Service_00010203},

    {83, 2, GATT_CHARACTERISTIC, 1, READ_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {84, 16, CHRCTR_Unknow_Service_00010203_2b_12, 1, Unknow_Service_00010203_2b_12_value},
    {85, 2, CHRCTR_Unknow_Service_CCC, 2, Unknow_Service_CCC_value},
    {86, 2, CHRCTR_Characteristic_User_Des, 1, Characteristic_User_Des_value},

    /* UUID	0000 d1ff 3c */
    {87, 2, PRIMARY_SERVICE, 16, CHRCTR_Unknow_Service_0000d1ff3c},

    {88, 2, GATT_CHARACTERISTIC, 1, READ_WITHOUT_RESPONSE_PREMITTED},
    {89, 17, CHRCTR_Unknow_Service_0000a002, 1, Unknow_Service_0000a002_value},

    /* Google Voice */
    {90, 2, PRIMARY_SERVICE, 16, CHRCTR_ATV_Voice_Service_uuid_128},

    {91, 2, GATT_CHARACTERISTIC, 1, WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {92, 16, CHRCTR_Write_uuid_128, 0, NULL},
    {93, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
    {94, 16, CHRCTR_Read_uuid_128, 0, NULL},
    {95, 2, CHRCTR_CCC, 2, voice_Read_ccc},
    {96, 2, GATT_CHARACTERISTIC, 1, READ_NOTIFY_PREMITTED},
    {97, 16, CHRCTR_Control_uuid_128, 0, NULL},
    {98, 2, CHRCTR_CCC, 2, voice_Control_ccc},
};

static uint16_t Get_AttListSize(void)
{
    return (sizeof(Att_List) / sizeof(ATT_TABLE_TYPE));
}

const ATT_TABLE_TYPE* Find_AttTable_ByHandle(uint16_t handle)
{
    for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++) {
        if (Att_List[list_index].handle == handle) {
            return &Att_List[list_index];
        }
    }

    return NULL;
}

const ATT_TABLE_TYPE* Find_AttTable_ByHandleRange_Type(uint16_t starthandle, uint16_t endhandle, UUID_TYPE* uuid)
{
    if (starthandle > endhandle || starthandle > Att_List[Get_AttListSize() - 1].handle) {
        return NULL;
    }

    for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++) {
        if (Att_List[list_index].handle >= starthandle && Att_List[list_index].handle <= endhandle
            && Att_List[list_index].uuid_len == uuid->Len && !memcmp(Att_List[list_index].uuid, uuid->Att, uuid->Len)) {
            return &Att_List[list_index];
        }
    }

    return NULL;
}

const ATT_TABLE_TYPE* Find_AttTable_ByHandleRange_Type_Data(uint16_t starthandle, uint16_t endhandle, UUID_TYPE* uuid, uint8_t* dataPtr, uint8_t dataLen)
{
    if (starthandle > endhandle || starthandle > Att_List[Get_AttListSize() - 1].handle) {
        return NULL;
    }

    for (uint16_t list_index = 0; list_index < Get_AttListSize(); list_index++) {
        if (Att_List[list_index].handle >= starthandle && Att_List[list_index].handle <= endhandle
            && Att_List[list_index].uuid_len == uuid->Len && !memcmp(Att_List[list_index].uuid, uuid->Att, uuid->Len)
            && Att_List[list_index].dataLen == dataLen && !memcmp(Att_List[list_index].dataPtr, dataPtr, dataLen)) {
            return &Att_List[list_index];
        }
    }

    return NULL;
}
