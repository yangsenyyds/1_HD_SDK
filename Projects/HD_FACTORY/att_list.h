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

#define	READ_PREMITTED 						0x02
#define	WRITE_WITHOUT_RESPONSE_PREMITTED 	0x04
#define	WRITE_PREMITTED 					0x08
#define	NOTIFY_PREMITTED 					0x10
#define	INDICATE_PREMITTED 					0X20

#define UUID2ARRAY(uuid) uuid&0xff,uuid>>8

#define FIRST_HANDLE	(1)
#define LAST_HANDLE		(0xFF)

#define OTA_NOTIFY_HANDLE   (3)
#define OTA_WRITE_HANDLE    (6)

extern const uint8_t PRIMARY_SERVICE[];
extern const uint8_t GATT_INCLUDE[];
extern const uint8_t GATT_CHARACTERISTIC[];

extern const ATT_TABLE_TYPE Att_List[];

extern const ATT_TABLE_TYPE *Find_AttTable_ByHandle(uint16_t handle);
extern const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid);
extern const ATT_TABLE_TYPE *Find_AttTable_ByHandleRange_Type_Data(uint16_t starthandle, uint16_t endhandle, UUID_TYPE *uuid, uint8_t *dataPtr, uint8_t dataLen);

#endif
