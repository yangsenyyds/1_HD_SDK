#ifndef _ATT_LIST_H_
#define _ATT_LIST_H_
#include <string.h>
#include "yc11xx.h"


typedef struct {
	uint8_t Len;
	uint8_t Att[16];
}UUID_TYPE;

typedef struct {
	uint16_t handle;
	uint8_t  uuid_len;
	const uint8_t *uuid; 
	uint16_t dataLen;
	const uint8_t *dataPtr;
}ATT_TABLE_TYPE;

#define ATT_LIST_DATA_LENGTH_BYTE 2

#define	ATTOP_ERROR_RESPONSE 				0x01
#define	ATTOP_EXCHANGE_MTU_REQUEST 			0x02
#define	ATTOP_EXCHANGE_MTU_RESPONSE 		0x03
#define	ATTOP_FIND_INFORMATION_REQUEST 		0x04
#define	ATTOP_FIND_INFORMATION_RESPONSE 	0x05
#define	ATTOP_FIND_BY_TYPE_VALUE_REQUEST 	0x06
#define	ATTOP_FIND_BY_TYPE_VALUE_RESPONSE 	0x07
#define	ATTOP_READ_BY_TYPE_REQUEST 			0x08
#define	ATTOP_READ_BY_TYPE_RESPONSE 		0x09
#define	ATTOP_READ_REQUEST 					0x0A
#define	ATTOP_READ_RESPONSE 				0x0B
#define	ATTOP_READ_BLOB_REQUEST 			0x0C
#define	ATTOP_READ_BLOB_RESPONSE 			0x0D
#define	ATTOP_READ_MULTIPLE_REQUEST 		0x0E
#define	ATTOP_READ_MULTIPLE_RESPONSE 		0x0F
#define	ATTOP_READ_BY_GROUP_TYPE_REQUEST 	0x10
#define	ATTOP_READ_BY_GROUP_TYPE_RESPONSE 	0x11
#define	ATTOP_WRITE_REQUEST 				0x12
#define	ATTOP_WRITE_RESPONSE 				0x13
#define	ATTOP_PREPARE_WRITE_REQUEST 		0x16
#define	ATTOP_PREPARE_WRITE_RESPONSE 		0x17
#define	ATTOP_EXECUTE_WRITE_REQUEST 		0x18
#define	ATTOP_EXECUTE_WRITE_RESPONSE 		0x19
#define	ATTOP_HANDLE_VALUE_NOTIFICATION		0x1B
#define	ATTOP_HANDLE_VALUE_INDICATION 		0x1D
#define	ATTOP_HANDLE_VALUE_CONFIRMATION 	0x1E
#define	ATTOP_WRITE_COMMAND 				0x52

#define ATT_ERR_ATTRIBUTE_NOT_FOUND 		0x0A
#define ATT_ERR_ATTRIBUTE_NOT_LONG 			0x0B

#define	UUID_GATT_PRIMARY_SERVICE 			0x2800
#define	UUID_GATT_SECONDARY_SERVICE 		0x2801
#define	UUID_GATT_INCLUDE 					0x2802
#define	UUID_GATT_CHARACTERISTIC 			0x2803

#define	CLIENT_CHARACTERTIC_CONFIGURATION 	0x2902
#define	EXTERNAL_REPORT_REFERENCE 			0x2907
#define	INTERNAL_REPORT_REFERENCE 		    0x2908

#define	UUID_SERVICE_GENERIC_ACC 			0x1800
#define	UUID_SERVICE_GENERIC_ATT 			0x1801
#define	UUID_Immediate_Alert				0x1802
#define	UUID_Link_Loss_ATT 					0x1803
#define	UUID_Tx_Power						0x1804
#define	UUID_SERVICE_DEVICE_INFO 			0x180a
#define	UUID_SERVICE_BATTERY 				0x180f
#define	UUID_SERVICE_HIDS 					0x1812
#define UUID_SERVICE_SCAN_PARAM             0x1813

#define	UUID_CHRCTR_DEVICE_NAME 			0x2A00
#define	UUID_CHRCTR_APPEARANCE 				0x2A01
#define	UUID_CHRCTR_PPCP					0x2A04
#define UUID_CHRCTR_SERVICE_CHANGED			0x2A05
#define UUID_CHRCTR_Alert_Level				0x2A06
#define UUID_CHRCTR_Tx_power_Level			0x2A07
#define	UUID_CHRCTR_BATTERY_LEVEL 			0x2A19

#define	UUID_CHRCTR_SYSTEM_ID  				0x2A23
#define	UUID_CHRCTR_MODEL_NUMBER 			0x2A24
#define	UUID_CHRCTR_SERIAL_NUMBER 			0x2A25
#define	UUID_CHRCTR_FIRMWARE 				0x2A26
#define	UUID_CHRCTR_HARDWARE 				0x2A27
#define	UUID_CHRCTR_SOFTWARE 				0x2A28
#define	UUID_CHRCTR_MANUFACTURE_NAME		0x2A29
#define	UUID_CHRCTR_IEEE 					0x2A2A
#define	UUID_CHRCTR_PNP_ID 					0x2A50

#define	UUID_CHRCTR_HID_INFO 				0x2A4A
#define UUID_CHRCTR_BOOT_KEYBOARD_INPUT_REPORT	0x2A22
#define UUID_CHRCTR_BOOT_KEYBOARD_OUTPUT_REPORT	0x2A32
#define	UUID_CHRCTR_REPORT_MAP 				0x2A4B
#define	UUID_CHRCTR_HID_CTRL_POINT 			0x2A4C
#define	UUID_CHRCTR_REPORT 					0x2A4D
#define	UUID_CHRCTR_PROTOCOL_MODE 			0x2A4E
#define	UUID_CENTRAL_ADDRESS_RESOLUTION 	0x2AA6

#define	READ_PREMITTED 						0x02
#define	WRITE_WITHOUT_RESPONSE_PREMITTED 	0x04
#define	WRITE_PREMITTED 					0x08
#define	NOTIFY_PREMITTED 					0x10
#define	INDICATE_PREMITTED 					0X20

#define UUID2ARRAY(uuid) uuid&0xff,uuid>>8

#define FIRST_HANDLE	(1)
#define LAST_HANDLE		(65505)

#define OTA_NOTIFY_HANDLE   (65502)
#define OTA_WRITE_HANDLE    (65505)

extern const uint8_t PRIMARY_SERVICE[];
extern const uint8_t GATT_INCLUDE[];
extern const uint8_t GATT_CHARACTERISTIC[];

extern const ATT_TABLE_TYPE Att_List[];

extern int Get_AttListIndex(uint16_t handle);
extern const ATT_TABLE_TYPE *Find_AttTable_ByHandle(uint16_t handle);
extern const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid);
extern const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type_Data(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid, uint8_t *dataPtr, uint8_t dataLen);

#endif
