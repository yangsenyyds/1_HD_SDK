#include "yc_ble_ll.h"
#include "yc_debug.h"


WEAK void LE_Master_Feature(uint8_t *data, uint8_t len){}


void Bt_LmpCallBack(uint8_t len, uint8_t *dataPtr)
{
	uint8_t  i = 0;

	switch(*dataPtr)
	{
		case LL_VERSION_OPCODE:
			break;
		case LL_FEATURE_OPCODE:
			LE_Master_Feature(&dataPtr[1],len);
			break;
		case LL_SLAVE_FEATURE_OPCODE:
			break;
		case LL_LENGTH_OPCODE:
			break;
		default:
			break;
	}
	// for(i = 0; i < len;  i++)
	// {
	// 	DEBUG_LOG_STRING("data = %x  \r\n", *(dataPtr++));
	// }
}









