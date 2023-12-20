#include "att_list.h"
#include "app_config.h"

const uint8_t PRIMARY_SERVICE[] = {UUID2ARRAY(UUID_GATT_PRIMARY_SERVICE)};
const uint8_t GATT_INCLUDE[] = {UUID2ARRAY(UUID_GATT_INCLUDE)};
const uint8_t GATT_CHARACTERISTIC[] = {UUID2ARRAY(UUID_GATT_CHARACTERISTIC)};

static const uint8_t READ_PREMITTED_ONLY[] = {READ_PREMITTED};
static const uint8_t WRITE_PREMITTED_ONLY[] = {WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY[] = {WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t INDICATE_PREMITTED_ONLY[] = {INDICATE_PREMITTED};
static const uint8_t READ_NOTIFY_PREMITTED[] = {READ_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED};
static const uint8_t WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t READ_WRITE_NOTIFY_INDICATE_PREMITTED[] = {READ_PREMITTED|WRITE_PREMITTED|NOTIFY_PREMITTED|INDICATE_PREMITTED};

static const uint8_t CHARACTERTIC_CONFIGURATION[] = {UUID2ARRAY(CLIENT_CHARACTERTIC_CONFIGURATION)};
static const uint8_t EXTERNAL_REPORT[] = {UUID2ARRAY(EXTERNAL_REPORT_REFERENCE)};
static const uint8_t REPORT_REFERENCE[] = {UUID2ARRAY(INTERNAL_REPORT_REFERENCE)};

/* Remote Control Service */
static const uint8_t SERVICE_GENERIC_ACC[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ACC)};
static const uint8_t CHRCTR_DEVICE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_DEVICE_NAME)};
static const uint8_t device_name[] = {"Smart Control 2016"};
static const uint8_t CHRCTR_APPEARANCE[] = {UUID2ARRAY(UUID_CHRCTR_APPEARANCE)};
static const uint8_t appearance[] = {0x00, 0x00};
static const uint8_t CHRCTR_CENTRAL_ADDRESS_RESOLUTION[] = {UUID2ARRAY(UUID_CHRCTR_CENTRAL_ADDRESS_RESOLUTION)};
static const uint8_t central_address_resolution[] = {0x00};

static const uint8_t SERVICE_GENERIC_ATT[] = {UUID2ARRAY(UUID_SERVICE_GENERIC_ATT)};
static const uint8_t CHRCTR_SERVICE_CHANGED[] = {UUID2ARRAY(UUID_CHRCTR_SERVICE_CHANGED)};
static uint8_t Service_Changed_ccc[2];

static const uint8_t SERVICE_DEVICE_INFO[] = {UUID2ARRAY(UUID_SERVICE_DEVICE_INFO)};
static const uint8_t CHRCTR_MANUFACTURE_NAME[] = {UUID2ARRAY(UUID_CHRCTR_MANUFACTURE_NAME)};
static const uint8_t manufacture_name[] = {""};
static const uint8_t CHRCTR_SYSTEM_ID[] = {UUID2ARRAY(UUID_CHRCTR_SYSTEM_ID)};
static const uint8_t system_id[] = {0x01,0x02,0x03,0x04,0x05,0x53,0x04,0x00};
static const uint8_t CHRCTR_MODEL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_MODEL_NUMBER)};
static const uint8_t model_number[] = {"0.1"};
static const uint8_t CHRCTR_SERIAL_NUMBER[] = {UUID2ARRAY(UUID_CHRCTR_SERIAL_NUMBER)};
static const uint8_t serial_number[] = {"1.2.3.4.5.6"};
static const uint8_t CHRCTR_FIRMWARE[] = {UUID2ARRAY(UUID_CHRCTR_FIRMWARE)};
static const uint8_t firmware[] = {"0.1"};
static const uint8_t CHRCTR_HARDWARE[] = {UUID2ARRAY(UUID_CHRCTR_HARDWARE)};
static const uint8_t hardware[] = {"UE878"};
static const uint8_t CHRCTR_SOFTWARE[] = {UUID2ARRAY(UUID_CHRCTR_SOFTWARE)};
static const uint8_t software[] = {"0.1"};
static const uint8_t CHRCTR_IEEE[] = {UUID2ARRAY(UUID_CHRCTR_IEEE)};
static const uint8_t ieee[] = {0x00,0x00,0x00,0x00,0x00,0x00};

static const uint8_t SERVICE_LINK_LOSS[] = {UUID2ARRAY(UUID_SERVICE_LINK_LOSS)};
static const uint8_t CHRCTR_ALERT_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_ALERT_LEVEL)};
static uint8_t alert_level[] = {0x00};

static const uint8_t SERVICE_IMMEDIATE_ALERT[] = {UUID2ARRAY(UUID_SERVICE_IMMEDIATE_ALERT)};

static const uint8_t SERVICE_TX_POWER[] = {UUID2ARRAY(UUID_SERVICE_TX_POWER)};
static const uint8_t CHRCTR_TX_POWER_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_TX_POWER_LEVEL)};
static uint8_t tx_power_level[] = {0x00};

static const uint8_t SERVICE_BATTERY[] = {UUID2ARRAY(UUID_SERVICE_BATTERY)};
static const uint8_t CHRCTR_BATTERY_LEVEL[] = {UUID2ARRAY(UUID_CHRCTR_BATTERY_LEVEL)};
static uint8_t battery_level[] = {100};
static uint8_t battery_ccc[2];

static const uint8_t SERVICE_HIDS[] = {UUID2ARRAY(UUID_SERVICE_HIDS)};
static const uint8_t CHRCTR_HID_CTRL_POINT[] = {UUID2ARRAY(UUID_CHRCTR_HID_CTRL_POINT)};
static uint8_t hid_ctrl_point[1];
static const uint8_t CHRCTR_HID_INFO[] = {UUID2ARRAY(UUID_CHRCTR_HID_INFO)};
// static const uint8_t hid_info_value[]= {};
static const uint8_t CHRCTR_REPORT_MAP[] = {UUID2ARRAY(UUID_CHRCTR_REPORT_MAP)};
static const uint8_t report_map_value[] = {
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x06,       // Usage (Keyboard)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x01,       // Report Id (1)
    0x75, 0x01,       // Report Size (1)
    0x95, 0x08,       // Report Count (8)
    0x05, 0x07,       // Usage Page (Keyboard)
    0x19, 0xE0,       // Usage Minimum (Keyboard Left Control)
    0x29, 0xE7,       // Usage Maximum (Keyboard Right GUI)
    0x15, 0x00,       // Logical minimum (0)
    0x25, 0x01,       // Logical maximum (1)
    0x81, 0x02,       // Input (Data,Value,Absolute,Bit Field)
    0x95, 0x01,       // Report Count (1)
    0x75, 0x08,       // Report Size (8)
    0x81, 0x03,       // Input (Constant,Value,Absolute,Bit Field)
    0x95, 0x05,       // Report Count (5)
    0x75, 0x01,       // Report Size (1)
    0x05, 0x08,       // Usage Page (LEDs)
    0x19, 0x01,       // Usage Minimum (Num Lock)
    0x29, 0x05,       // Usage Maximum (Kana)
    0x91, 0x02,       // Output (Data,Value,Absolute,Non-volatile,Bit Field)
    0x95, 0x01,       // Report Count (1)
    0x75, 0x03,       // Report Size (3)
    0x91, 0x03,       // Output (Constant,Value,Absolute,Non-volatile,Bit Field)
    0x95, 0x06,       // Report Count (6)
    0x75, 0x08,       // Report Size (8)
    0x15, 0x00,       // Logical minimum (0)
    0x25, 0x65,       // Logical maximum (101)
    0x05, 0x07,       // Usage Page (Keyboard)
    0x19, 0x00,       // Usage Minimum (No event indicated)
    0x29, 0x65,       // Usage Maximum (Keyboard Application)
    0x81, 0x00,       // Input (Data,Array,Absolute,Bit Field)
    0xC0,             // End Collection
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x02,       // Usage (Mouse)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x02,       // Report Id (2)
    0x09, 0x01,       // Usage (Pointer)
    0xA1, 0x00,       // Collection (Physical)
    0x05, 0x09,       // Usage Page (Button)
    0x19, 0x01,       // Usage Minimum (Button 1)
    0x29, 0x03,       // Usage Maximum (Button 3)
    0x15, 0x00,       // Logical minimum (0)
    0x25, 0x01,       // Logical maximum (1)
    0x75, 0x01,       // Report Size (1)
    0x95, 0x03,       // Report Count (3)
    0x81, 0x02,       // Input (Data,Value,Absolute,Bit Field)
    0x75, 0x05,       // Report Size (5)
    0x95, 0x01,       // Report Count (1)
    0x81, 0x01,       // Input (Constant,Array,Absolute,Bit Field)
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x30,       // Usage (X)
    0x09, 0x31,       // Usage (Y)
    0x15, 0x81,       // Logical minimum (-127)
    0x25, 0x7F,       // Logical maximum (127)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x02,       // Report Count (2)
    0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
    0xC0,             // End Collection
    0xC0,             // End Collection
    0x05, 0x0C,       // Usage Page (Consumer)
    0x09, 0x01,       // Usage (Consumer Control)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xF6,       // Report Id (246)
    0x95, 0xB4,       // Report Count (180)
    0x75, 0x08,       // Report Size (8)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x00,       // Input (Data,Array,Absolute,Bit Field)
    0xC0,             // End Collection
    0x06, 0xF0, 0xFF, // Usage Page (Vendor-defined 0xFFF0)
    0x09, 0x01,       // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xF7,       // Report Id (247)
    0x09, 0x2F,       // Usage (Vendor-defined 0x002F)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x06,       // Report Count (6)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
    0xC0,             // End Collection
    0x06, 0xF0, 0xFF, // Usage Page (Vendor-defined 0xFFF0)
    0x09, 0x01,       // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xF8,       // Report Id (248)
    0x09, 0x2F,       // Usage (Vendor-defined 0x002F)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x06,       // Report Count (6)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
    0xC0,             // End Collection
    0x06, 0xF0, 0xFF, // Usage Page (Vendor-defined 0xFFF0)
    0x09, 0x01,       // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xF9,       // Report Id (249)
    0x09, 0x2F,       // Usage (Vendor-defined 0x002F)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x12,       // Report Count (18)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x06,       // Input (Data,Value,Relative,Bit Field)
    0xC0,             // End Collection
    0x09, 0x01,       // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xFB,       // Report Id (251)
    0x95, 0xC0,       // Report Count (192)
    0x75, 0x08,       // Report Size (8)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x00,       // Input (Data,Array,Absolute,Bit Field)
    0x95, 0x0B,       // Report Count (11)
    0x75, 0x01,       // Report Size (1)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x00,       // Input (Data,Array,Absolute,Bit Field)
    0xC0,             // End Collection
    0x05, 0x0C,       // Usage Page (Consumer)
    0x09, 0x01,       // Usage (Consumer Control)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0xFC,       // Report Id (252)
    0x95, 0xB4,       // Report Count (180)
    0x75, 0x08,       // Report Size (8)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    0x81, 0x00,       // Input (Data,Array,Absolute,Bit Field)
    0xC0,             // End Collection
    0x06, 0x01, 0xFF, // Usage Page (Vendor-defined 0xFF01)
    0x09, 0x01,       // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,       // Collection (Application)
    0xA1, 0x02,       // Collection (Logical)
    0x85, 0x07,       // Report Id (7)
    0x09, 0x14,       // Usage (Vendor-defined 0x0014)
    0x15, 0x80,       // Logical minimum (-128)
    0x25, 0x7F,       // Logical maximum (127)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x14,       // Report Count (20)
    0x81, 0x22,       // Input (Data,Value,Absolute,Bit Field)
    0xC0,             // End Collection
    0xC0,             // End Collection
    0x06, 0x01, 0xFF, // Usage Page (Vendor-defined 0xFF01)
    0x09, 0x01,       // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,       // Collection (Application)
    0xA1, 0x02,       // Collection (Logical)
    0x85, 0x08,       // Report Id (8)
    0x09, 0x14,       // Usage (Vendor-defined 0x0014)
    0x15, 0x80,       // Logical minimum (-128)
    0x25, 0x7F,       // Logical maximum (127)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x14,       // Report Count (20)
    0x91, 0x22,       // Output (Data,Value,Absolute,Non-volatile,Bit Field)
    0xC0,             // End Collection
    0xC0,             // End Collection
};
static const uint8_t CHRCTR_REPORT[] = {UUID2ARRAY(UUID_CHRCTR_REPORT)};
static const uint8_t report1_rr[] = {0xFB, 0x03};
static const uint8_t report2_rr[] = {0x01, 0x01};
static const uint8_t report3_rr[] = {0x01, 0x02};
static const uint8_t report4_rr[] = {0x02, 0x01};
static const uint8_t report5_rr[] = {0xF9, 0x01};
static const uint8_t report6_rr[] = {0xF7, 0x01};
static const uint8_t report7_rr[] = {0xF6, 0x01};
static const uint8_t report8_rr[] = {0xFB, 0x01};
static const uint8_t report9_rr[] = {0xCC, 0x01};
static const uint8_t report10_rr[] = {0xFC, 0x01};
static const uint8_t report11_rr[] = {0x07, 0x01};
static const uint8_t report12_rr[] = {0x08, 0x02};
static uint8_t report2_ccc[2];
static uint8_t report4_ccc[2];
static uint8_t report5_ccc[2];
static uint8_t report6_ccc[2];
static uint8_t report7_ccc[2];
static uint8_t report8_ccc[2];
static uint8_t report10_ccc[2];
static uint8_t report11_ccc[2];
static uint8_t report_value[2];

static const uint8_t Unknown_Service_0D91[] = {0x01, 0x10, 0xA5, 0x4D, 0x57, 0x20, 0x4F, 0xA3, 0x50, 0x4D, 0x70, 0x85, 0x56, 0x6F, 0x91, 0x0D};
static const uint8_t CHRCTR_0D91_01[] = {0x01, 0x00, 0xA5, 0x4D, 0x57, 0x20, 0x4F, 0xA3, 0x50, 0x4D, 0x70, 0x85, 0x56, 0x6F, 0x91, 0x0D};
static const uint8_t CHRCTR_0D91_02[] = {0x02, 0x00, 0xA5, 0x4D, 0x57, 0x20, 0x4F, 0xA3, 0x50, 0x4D, 0x70, 0x85, 0x56, 0x6F, 0x91, 0x0D};
static uint8_t chrctr_0d91_01_ccc[2];
static uint8_t chrctr_0d91_01_value[2];
static uint8_t chrctr_0d91_02_value[2];

static const uint8_t Unknown_Service_9E5D[] = {0x6F, 0x38, 0xA1, 0x38, 0xAD, 0x82, 0x35, 0x86, 0xA0, 0x43, 0x13, 0x5C, 0x47, 0x1E, 0x5D, 0x9E};
static const uint8_t CHRCTR_E3DD[] = {0x6B, 0x66, 0x6C, 0x08, 0x0A, 0x57, 0x8E, 0x83, 0x99, 0x4E, 0xA7, 0xF7, 0xBF, 0x50, 0xDD, 0xE3};
static const uint8_t CHRCTR_92E8[] = {0x36, 0xFE, 0x2E, 0xE7, 0x09, 0x24, 0x4F, 0xB7, 0x91, 0x40, 0x61, 0xD9, 0x7A, 0x6C, 0xE8, 0x92};
static uint8_t chrctr_e3dd_ccc[2];
static uint8_t chrctr_e3dd_value[2];
static uint8_t chrctr_92e8_value[2];

static const uint8_t READ_WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED[] = {READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED|NOTIFY_PREMITTED};
static const uint8_t OTA_SERVICE[] = {0xf0, 0xff};
static const uint8_t CHRCTR_OTA_NOTIFY[] = {0xf4, 0xff};
static const uint8_t CHRCTR_OTA_WRITE[] = {0xf5, 0xff};
static uint8_t chrctr_ota_ccc[2];

//factory test   d07c0000-9037-4f23-a1fb-220cbd11163a
static const uint8_t NOTIFY_READ_WRITE_WITHOUT_RESPONSE_PREMITTED[] = {NOTIFY_PREMITTED|READ_PREMITTED|WRITE_WITHOUT_RESPONSE_PREMITTED};
static const uint8_t factory_primary_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x00,0x00,0x7C,0xD0};
static const uint8_t factory_characteristic_uuid[] = {0x3A,0x16,0x11,0xBD,0x0C,0x22,0xFB,0xA1,0x23,0x4F,0x37,0x90,0x01,0x00,0x7C,0xD0};
static const uint8_t product_id_s[] = "203";
const ATT_TABLE_TYPE Att_List[] =
{
	/* Generic Access */
    {1,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ACC},

	{2,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{3,2,CHRCTR_DEVICE_NAME,18,device_name},
	{4,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{5,2,CHRCTR_APPEARANCE,2,appearance},
	{6,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{7,2,CHRCTR_CENTRAL_ADDRESS_RESOLUTION,1,central_address_resolution},//6

	/* Generic Attribute */
    {16,2,PRIMARY_SERVICE,2,SERVICE_GENERIC_ATT},

	{17,2,GATT_CHARACTERISTIC,1,INDICATE_PREMITTED_ONLY},
	{18,2,CHRCTR_SERVICE_CHANGED,0,NULL},
	{19,2,CHARACTERTIC_CONFIGURATION,2,Service_Changed_ccc},//10

    /* Device Information */
	{48,2,PRIMARY_SERVICE,2,SERVICE_DEVICE_INFO},

	{49,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{50,2,CHRCTR_MANUFACTURE_NAME,18,manufacture_name},
	{51,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{52,2,CHRCTR_SYSTEM_ID,8,system_id},
	{53,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{54,2,CHRCTR_MODEL_NUMBER,3,model_number},
	{55,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{56,2,CHRCTR_SERIAL_NUMBER,11,serial_number},
	{57,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{58,2,CHRCTR_FIRMWARE,3,firmware},
	{59,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{60,2,CHRCTR_HARDWARE,5,hardware},
	{61,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{62,2,CHRCTR_SOFTWARE,3,software},
	{63,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{64,2,CHRCTR_IEEE,6,ieee},//27

    /* Link Loss */
	{80,2,PRIMARY_SERVICE,2,SERVICE_LINK_LOSS},

	{81,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
	{82,2,CHRCTR_ALERT_LEVEL,1,alert_level},//30
	
	/* Immediate Alert */
	{83,2,PRIMARY_SERVICE,2,SERVICE_IMMEDIATE_ALERT},

	{84,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{85,2,CHRCTR_ALERT_LEVEL,1,alert_level},//33

	/* Tx Power */
	{86,2,PRIMARY_SERVICE,2,SERVICE_TX_POWER},

	{87,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{88,2,CHRCTR_TX_POWER_LEVEL,1,tx_power_level},//36

	/* Battery Service */
    {96,2,PRIMARY_SERVICE,2,SERVICE_BATTERY},

	{97,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
	{98,2,CHRCTR_BATTERY_LEVEL,1,battery_level},//39
	{99,2,CHARACTERTIC_CONFIGURATION,2,battery_ccc},

	/* Human Interface Device */
	{112,2,PRIMARY_SERVICE,2,SERVICE_HIDS},

	{113,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_PREMITTED_ONLY},
	{114,2,CHRCTR_HID_CTRL_POINT,1,hid_ctrl_point},
	{115,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{116,2,CHRCTR_HID_INFO,0,NULL},
	{117,2,GATT_CHARACTERISTIC,1,READ_PREMITTED_ONLY},
	{118,2,CHRCTR_REPORT_MAP,305,report_map_value},
	{119,2,EXTERNAL_REPORT,2,CHRCTR_BATTERY_LEVEL},
	{120,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
	{121,2,CHRCTR_REPORT,2,report_value},
    {122,2,REPORT_REFERENCE,2,report1_rr},
    {123,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {124,2,CHRCTR_REPORT,2,report_value},
	{125,2,CHARACTERTIC_CONFIGURATION,2,report2_ccc},
    {126,2,REPORT_REFERENCE,2,report2_rr},
    {127,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {128,2,CHRCTR_REPORT,2,report_value},
    {129,2,REPORT_REFERENCE,2,report3_rr},
    {130,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {131,2,CHRCTR_REPORT,2,report_value},
	{132,2,CHARACTERTIC_CONFIGURATION,2,report4_ccc},
    {133,2,REPORT_REFERENCE,2,report4_rr},
    {134,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {135,2,CHRCTR_REPORT,2,report_value},
	{136,2,CHARACTERTIC_CONFIGURATION,2,report5_ccc},
    {137,2,REPORT_REFERENCE,2,report5_rr},
    {138,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {139,2,CHRCTR_REPORT,2,report_value},
	{140,2,CHARACTERTIC_CONFIGURATION,2,report6_ccc},
    {141,2,REPORT_REFERENCE,2,report6_rr},
    {142,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {143,2,CHRCTR_REPORT,2,report_value},
	{144,2,CHARACTERTIC_CONFIGURATION,2,report7_ccc},
    {145,2,REPORT_REFERENCE,2,report7_rr},
    {146,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {147,2,CHRCTR_REPORT,2,report_value},
	{148,2,CHARACTERTIC_CONFIGURATION,2,report8_ccc},
    {149,2,REPORT_REFERENCE,2,report8_rr},
	{150,2,GATT_CHARACTERISTIC,1,READ_WRITE_PREMITTED},
    {151,2,CHRCTR_REPORT,2,report_value},
    {152,2,REPORT_REFERENCE,2,report9_rr},
	{153,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {154,2,CHRCTR_REPORT,2,report_value},
	{155,2,CHARACTERTIC_CONFIGURATION,2,report10_ccc},
    {156,2,REPORT_REFERENCE,2,report10_rr},
	{157,2,GATT_CHARACTERISTIC,1,READ_NOTIFY_PREMITTED},
    {158,2,CHRCTR_REPORT,2,report_value},
	{159,2,CHARACTERTIC_CONFIGURATION,2,report11_ccc},
    {160,2,REPORT_REFERENCE,2,report11_rr},
	{161,2,GATT_CHARACTERISTIC,1,READ_WRITE_WRITE_WITHOUT_RESPONSE_PREMITTED},
    {162,2,CHRCTR_REPORT,2,report_value},
    {163,2,REPORT_REFERENCE,2,report12_rr},

    /* Unknown Service 0D91 */
	{20480,2,PRIMARY_SERVICE,16,Unknown_Service_0D91},

	{20481,2,GATT_CHARACTERISTIC,1,WRITE_WITHOUT_RESPONSE_NOTIFY_PREMITTED},
	{20482,16,CHRCTR_0D91_01,1,chrctr_0d91_01_value},
	{20483,2,CHARACTERTIC_CONFIGURATION,2,chrctr_0d91_01_ccc},
	{20484,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
	{20485,16,CHRCTR_0D91_02,2,chrctr_0d91_02_value},

    /* Unknown Service 9E5D */
	{65280,2,PRIMARY_SERVICE,16,Unknown_Service_9E5D},

	{65281,2,GATT_CHARACTERISTIC,1,READ_WRITE_NOTIFY_INDICATE_PREMITTED},
	{65282,16,CHRCTR_E3DD,1,chrctr_e3dd_value},
	{65283,2,CHARACTERTIC_CONFIGURATION,2,chrctr_e3dd_ccc},
	{65284,2,GATT_CHARACTERISTIC,1,WRITE_PREMITTED_ONLY},
	{65285,16,CHRCTR_92E8,2,chrctr_92e8_value},

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
