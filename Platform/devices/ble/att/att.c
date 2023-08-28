
#include "att.h"
#include "att_list.h"
#include "yc11xx_ipc.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc_dev_bt.h"
#include "yc_debug.h"
#include "btreg.h"
#include "app_config.h"


#define ATT_MODULE_NAME "ATT_LOG"
#define DEBUG_LOG_ATT(fmt, ...)    do {	\
                DEBUG_LOG_STRING("%s: "fmt"",ATT_MODULE_NAME,  ##__VA_ARGS__);	\
                } while (false);


WEAK void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len){}
WEAK void Read_Parse(const ATT_TABLE_TYPE *table){}
WEAK void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset){}

typedef struct {
	uint16_t StartHandle;
	uint16_t EndHandle;
	UUID_TYPE AttData;
}ATTRIBUTE_GROUP_TYPE;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT uint8_t prepareRcv[256]; 
uint8_t prepareLen;   
uint8_t prepareHandle; 

static uint16_t u8_to_u16(const uint8_t *ptr)
{
	return (*ptr+(*(ptr+1)<<8));
}

static void ATT_SendErrorRsp(uint8_t opcode, uint16_t handle, uint8_t errorcode)
{
	uint8_t senddata[5] = {0};
	senddata[0] = ATTOP_ERROR_RESPONSE;
	senddata[1] = opcode;
	senddata[2] = handle;
	senddata[3] = handle>>8;
	senddata[4] = errorcode;
	ATT_Send(senddata, 5);
}

static void ATT_WriteCmd(uint16_t handle, uint8_t *dataPtr, uint8_t len)
{
	const ATT_TABLE_TYPE *foundTable;

	foundTable = Find_AttTable_ByHandle(handle);
	if (foundTable != NULL) {
		Write_DataParse(foundTable, dataPtr, len);
	}
}

static void ATT_ExecuteWriteReq(void)
{
	const ATT_TABLE_TYPE *foundTable;

	foundTable = Find_AttTable_ByHandle(prepareHandle);
	if (foundTable != NULL) {
		uint8_t senddata[1]= {ATTOP_EXECUTE_WRITE_RESPONSE};   
		ATT_Send(senddata,1);

		Write_DataParse(foundTable, prepareRcv, prepareLen);
		prepareLen = 0;
	}
}

static void ATT_PrepareWriteReq(uint8_t len, uint8_t *dataPtr)
{
	uint8_t senddata[256]={0};
	uint16_t sendlen;
	uint16_t handle = u8_to_u16 (dataPtr);
	if(prepareLen != u8_to_u16 (dataPtr+2) || len < 4)
		while(1);
	
	memcpy(prepareRcv+prepareLen,dataPtr+4,len-4);
	senddata[0]=ATTOP_PREPARE_WRITE_RESPONSE;   
	memcpy(senddata+1,dataPtr,len);
	ATT_Send(senddata,len+1);
	prepareHandle = handle;
	prepareLen = prepareLen + len - 4;
	if(prepareLen >= 255)
		while(1);
}

static void ATT_WriteReq(uint8_t len, uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	uint16_t handle = u8_to_u16(dataPtr);

	foundTable = Find_AttTable_ByHandle(handle);
	if (foundTable != NULL) {
		uint8_t senddata[1] = {ATTOP_WRITE_RESPONSE};
		ATT_Send(senddata,1);

		Write_DataParse(foundTable, dataPtr+2, len-2);
	}
}

static void ATT_ReadReq_ByGroupType(uint8_t len,uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	ATTRIBUTE_GROUP_TYPE AttGroup;
	
	uint16_t startHandle = u8_to_u16(dataPtr), endHandle = u8_to_u16(dataPtr+2);
	UUID_TYPE uuid;
	uuid.Len = len - 4;
	memcpy(uuid.Att, dataPtr+4, uuid.Len);

	uint16_t mtu_size = HREADW(mem_mtu_size);
	uint8_t senddata[256] = {0};
	uint16_t datalen = 0;

	bool shortuuid_flag = false, longuuid_flag = false;

	foundTable = Find_AttTable_ByHandleRange_Type(startHandle, endHandle, &uuid);
	if(foundTable == NULL) {
		ATT_SendErrorRsp(ATTOP_READ_BY_GROUP_TYPE_REQUEST, startHandle, ATT_ERR_ATTRIBUTE_NOT_FOUND);
		return;
	}
	else {
		if (foundTable->dataLen == 0x02) {
			shortuuid_flag = true;
		}
		else if(foundTable->dataLen == 0x10) {
			longuuid_flag = true;
		}
	}

	senddata[datalen++] = ATTOP_READ_BY_GROUP_TYPE_RESPONSE;
    senddata[datalen++] = foundTable->dataLen + 4;

	for(datalen; datalen < mtu_size; )
	{
		AttGroup.StartHandle = foundTable->handle;
		AttGroup.AttData.Len = foundTable->dataLen;
		memcpy((void *)AttGroup.AttData.Att, (void *)foundTable->dataPtr, foundTable->dataLen);
		if ((datalen + 4 + AttGroup.AttData.Len) > mtu_size) {
			break;
		}

		foundTable = Find_AttTable_ByHandleRange_Type(AttGroup.StartHandle + 1, endHandle, &uuid);
		if (foundTable == NULL) {
			AttGroup.EndHandle = LAST_HANDLE;
		}
		else {
			AttGroup.EndHandle = (foundTable-1)->handle;
		}

		senddata[datalen++] = AttGroup.StartHandle;
		senddata[datalen++] = AttGroup.StartHandle >> 8;
		senddata[datalen++] = AttGroup.EndHandle;
		senddata[datalen++] = AttGroup.EndHandle >> 8;
		memcpy((void *)&senddata[datalen], (void *)AttGroup.AttData.Att, AttGroup.AttData.Len);
		datalen += AttGroup.AttData.Len;

		if (foundTable->dataLen == 0x02) {
			shortuuid_flag = true;
		}
		else if(foundTable->dataLen == 0x10) {
			longuuid_flag = true;
		}
		if (AttGroup.EndHandle == LAST_HANDLE || !(shortuuid_flag ^ longuuid_flag)) {
			break;
		}
	}

	ATT_Send(senddata, datalen);
}

static void ATT_ReadBlobReq(uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	uint16_t handle = u8_to_u16(dataPtr);
	uint16_t offset = u8_to_u16(dataPtr+2);

	foundTable = Find_AttTable_ByHandle(handle);
	if(foundTable != NULL)
	{
		if (foundTable->dataLen >= offset) {
			Read_Blob_Parse(foundTable, offset);
		}
		else {
			ATT_SendErrorRsp(ATTOP_READ_BLOB_REQUEST, handle, ATT_ERR_ATTRIBUTE_NOT_LONG);
		}
	}
	else {
		ATT_SendErrorRsp(ATTOP_READ_BLOB_REQUEST, handle, ATT_ERR_ATTRIBUTE_NOT_FOUND);
	}	
}

static void ATT_ReadReq(uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	uint16_t handle = u8_to_u16(dataPtr);

	foundTable = Find_AttTable_ByHandle(handle);
	if (foundTable != NULL) {
		Read_Parse(foundTable);
	}
	else {
		ATT_SendErrorRsp(ATTOP_READ_REQUEST, handle, ATT_ERR_ATTRIBUTE_NOT_FOUND);
	}
}

static void ATT_ReadReq_ByType(uint8_t len, uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	
	uint16_t startHandle = u8_to_u16(dataPtr), endHandle = u8_to_u16(dataPtr+2);
	UUID_TYPE uuid;
	uuid.Len = len - 4;
	memcpy(uuid.Att, dataPtr+4, uuid.Len);

	uint16_t mtu_size = HREADW(mem_mtu_size);
	uint8_t senddata[256] = {0};
	uint16_t datalen = 0;

	foundTable = Find_AttTable_ByHandleRange_Type(startHandle, endHandle, &uuid);
	if(foundTable == NULL) {
		ATT_SendErrorRsp(ATTOP_READ_BY_TYPE_REQUEST, startHandle, ATT_ERR_ATTRIBUTE_NOT_FOUND);
		return;
	}

	if (!memcmp(uuid.Att, GATT_CHARACTERISTIC, 2))
	{
		senddata[datalen++] = ATTOP_READ_BY_TYPE_RESPONSE;
    	senddata[datalen++] = (foundTable+1)->uuid_len + 5;

		for(datalen; datalen < mtu_size; )
		{
			if ((datalen + 5 + (foundTable+1)->uuid_len) > mtu_size) {
				break;
			}
			senddata[datalen++] = foundTable->handle;
			senddata[datalen++] = foundTable->handle >> 8;
			senddata[datalen++] = foundTable->dataPtr[0];
			senddata[datalen++] = foundTable->handle + 1;
			senddata[datalen++] = (foundTable->handle + 1) >> 8;
			memcpy((void *)&senddata[datalen], (foundTable+1)->uuid, (foundTable+1)->uuid_len);
    	    datalen += (foundTable+1)->uuid_len;

			foundTable = Find_AttTable_ByHandleRange_Type(foundTable->handle + 2, endHandle, &uuid);
			if (foundTable == NULL || foundTable->handle == endHandle) {
				break;
			}
		}
	}
	else if (!memcmp(uuid.Att, GATT_INCLUDE, 2))
	{
		senddata[datalen++] = ATTOP_READ_BY_TYPE_RESPONSE;
		senddata[datalen++] = foundTable->dataLen + 6;
		senddata[datalen++] = foundTable->handle;
		senddata[datalen++] = foundTable->handle >> 8;
		datalen += 4;
		memcpy((void *)&senddata[datalen], foundTable->dataPtr, foundTable->dataLen);
		datalen += foundTable->dataLen;

		uuid.Len = 2;
		memcpy(uuid.Att, PRIMARY_SERVICE, uuid.Len);
		foundTable = Find_AttTable_ByHandleRange_Type_Data(1, LAST_HANDLE, &uuid, (void *)foundTable->dataPtr, foundTable->dataLen);
		senddata[4] = foundTable->handle;
		senddata[5] = foundTable->handle >> 8;

		foundTable = Find_AttTable_ByHandleRange_Type(foundTable->handle + 1, LAST_HANDLE, &uuid);
		senddata[6] = (foundTable-1)->handle;
		senddata[7] = (foundTable-1)->handle >> 8;
	}
	else
	{
		senddata[datalen++] = ATTOP_READ_BY_TYPE_RESPONSE;
    	senddata[datalen++] = foundTable->dataLen + 2;
		senddata[datalen++] = foundTable->handle;
		senddata[datalen++] = foundTable->handle >> 8;
		memcpy((void *)&senddata[datalen], (void *)foundTable->dataPtr, foundTable->dataLen);
		datalen += foundTable->dataLen;
	}
	
	ATT_Send(senddata, datalen);
}

static void ATT_Find_ByType_Value(uint8_t len, uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	
	uint16_t startHandle = u8_to_u16(dataPtr), endHandle = u8_to_u16(dataPtr+2);
	UUID_TYPE uuid;
	uuid.Len = 2;
	memcpy(uuid.Att, dataPtr+4, uuid.Len);

	uint8_t senddata[5] = {0};
	uint16_t datalen = 0;

	foundTable = Find_AttTable_ByHandleRange_Type_Data(startHandle, endHandle, &uuid, dataPtr+6, len-6);
	if(foundTable == NULL) {
		ATT_SendErrorRsp(ATTOP_FIND_INFORMATION_REQUEST, startHandle, ATT_ERR_ATTRIBUTE_NOT_FOUND);
		return;
	}

	senddata[datalen++] = ATTOP_FIND_BY_TYPE_VALUE_RESPONSE;
	senddata[datalen++] = foundTable->handle;
	senddata[datalen++] = foundTable->handle >> 8;

	foundTable = Find_AttTable_ByHandleRange_Type(foundTable->handle + 1, endHandle, &uuid);
	if(foundTable == NULL) {
		senddata[datalen++] = LAST_HANDLE & 0xFF;
		senddata[datalen++] = (LAST_HANDLE >> 8) & 0xFF;
	}
	else {
		senddata[datalen++] = (foundTable-1)->handle;
		senddata[datalen++] = (foundTable-1)->handle >> 8;
	}

	ATT_Send(senddata, datalen);
}

static void ATT_FindInformationReq(uint8_t *dataPtr)
{
	const ATT_TABLE_TYPE *foundTable;
	
	uint16_t startHandle = u8_to_u16(dataPtr), endHandle = u8_to_u16(dataPtr+2);

	uint16_t mtu_size = HREADW(mem_mtu_size);
	uint8_t senddata[256] = {0};
	uint16_t datalen = 0;

	foundTable = Find_AttTable_ByHandle(startHandle);
	if(foundTable == NULL) {
		ATT_SendErrorRsp(ATTOP_FIND_INFORMATION_REQUEST, startHandle, ATT_ERR_ATTRIBUTE_NOT_FOUND);
		return;
	}

	senddata[datalen++] = ATTOP_FIND_INFORMATION_RESPONSE;
	senddata[datalen++] = (foundTable->uuid_len == 16? 2:1);
    
	for(datalen; datalen < mtu_size; )
	{
		if ((datalen + 2 + foundTable->uuid_len) > mtu_size) {
			break;
		}
		senddata[datalen++] = foundTable->handle;
		senddata[datalen++] = foundTable->handle >> 8;
		memcpy((void *)&senddata[datalen], (void *)foundTable->uuid, foundTable->uuid_len);
        datalen += foundTable->uuid_len;
		
		if ((foundTable->handle + 1) > endHandle) {
			break;
		}
		foundTable = Find_AttTable_ByHandle(foundTable->handle + 1);
		if (foundTable == NULL) {
			break;
		}
	}

	ATT_Send(senddata, datalen);
}

static void ATT_UpdateMtu(uint16_t exchangemtu, uint16_t localmtu)
{
	DEBUG_LOG_ATT("ATT UPDATA MUT, exchangemtu: %d  localmtu: %d \r\n", exchangemtu, localmtu);
	HWRITEW(mem_mtu_size, exchangemtu>localmtu? localmtu:exchangemtu);
}

static void ATT_ExchangeMtuReq(uint8_t *dataPtr)
{
	uint8_t senddata[3] = {0};
	senddata[0] = ATTOP_EXCHANGE_MTU_RESPONSE;
	senddata[1] = HREADW(mem_le_local_mtu);
	senddata[2] = HREADW(mem_le_local_mtu) >> 8;
	ATT_Send(senddata, 3);
	ATT_UpdateMtu(u8_to_u16(dataPtr), HREADW(mem_le_local_mtu));
}

void ATT_Send(uint8_t *packet, uint8_t len)
{
	if (!bt_check_le_connected())
		return;

	Bt_BleSendData(packet, len);  
}

void ATT_SendExchangeMtuReq(void)
{
	uint8_t senddata[3]={0};
	senddata[0] = ATTOP_EXCHANGE_MTU_REQUEST;
	senddata[1] = HREADW(mem_le_local_mtu);
	senddata[2] = HREADW(mem_le_local_mtu) >> 8;
	ATT_Send(senddata, 3);
}

void ATT_sendWriteReq(uint16_t handle, uint8_t *dataptr, uint8_t len)
{
	uint8_t senddata[256] = {0};
	senddata[0] = ATTOP_WRITE_REQUEST;
	senddata[1] = handle;
	senddata[2] = handle>>8;
	memcpy((void *)&senddata[3], (void *)dataptr, len);
	ATT_Send(senddata, len+3);	
}

void ATT_sendNotify(uint16_t handle, uint8_t *dataptr, uint8_t len)
{
	if (!bt_check_le_connected()) {
		return;
	}
	
	uint8_t senddata[256] = {0};
	senddata[0] = ATTOP_HANDLE_VALUE_NOTIFICATION;
	senddata[1] = handle;
	senddata[2] = handle>>8;
	memcpy((void *)&senddata[3], (void *)dataptr, len);
	ATT_Send(senddata, len+3);
}

void ATT_Parse(uint8_t len,uint8_t *dataPtr)
{
	switch(dataPtr[0])
	{
	case ATTOP_EXCHANGE_MTU_REQUEST :
		ATT_ExchangeMtuReq(dataPtr+1);
		break;
	case ATTOP_EXCHANGE_MTU_RESPONSE :	
		ATT_UpdateMtu(u8_to_u16(dataPtr+1), HREADW(mem_le_local_mtu));
		break;
	case ATTOP_FIND_INFORMATION_REQUEST :
		ATT_FindInformationReq(dataPtr+1);
		break;
	case ATTOP_FIND_BY_TYPE_VALUE_REQUEST :
		ATT_Find_ByType_Value(len-1, dataPtr+1);
		break;
	case ATTOP_READ_BY_TYPE_REQUEST :
		ATT_ReadReq_ByType(len-1, dataPtr+1);
		break;
	case ATTOP_READ_REQUEST :
		ATT_ReadReq(dataPtr+1);
		break;
	case ATTOP_READ_BLOB_REQUEST :
		ATT_ReadBlobReq(dataPtr+1);
		break;
	case ATTOP_READ_BY_GROUP_TYPE_REQUEST :
		ATT_ReadReq_ByGroupType(len-1, dataPtr+1);
		break;
	case ATTOP_WRITE_REQUEST :
		ATT_WriteReq(len-1, dataPtr+1);
		break;
	case ATTOP_PREPARE_WRITE_REQUEST :
		ATT_PrepareWriteReq(len-1, dataPtr+1);
		break;
	case ATTOP_EXECUTE_WRITE_REQUEST :
		ATT_ExecuteWriteReq();
		break;
	case ATTOP_WRITE_COMMAND :
		ATT_WriteCmd(u8_to_u16(dataPtr+1), dataPtr+3, len-3);
		break;
	
	default:
		DEBUG_LOG_ATT("ATTOP NO CMD: 0x%x \r\n", dataPtr[0]);
		break;
	}
}
