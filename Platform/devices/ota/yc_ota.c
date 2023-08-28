#include "yc_ota.h"
#include "yc11xx_dev_qspi.h"
#include "yc_dev_bt.h"
#include "yc11xx_systick.h"
#include "yc_debug.h"
#include "att.h"
#include "att_list.h"
#include "app_config.h"

#ifdef FUNCTION_WATCH_DOG
#include "yc11xx_iwdg.h"
#endif


typedef struct {
    bool ota_is_starting;
    uint16_t ota_packetid;
    uint32_t ota_flash_start_addr;
    uint32_t ota_flash_length;
    uint32_t ota_datasum;
    uint32_t ota_datalen; 
}OTA_DataStruct_TypeDef;

#ifdef FUNCTION_WATCH_DOG
extern IWDG_InitTypeDef gWdtInit;
#endif
static OTA_DataStruct_TypeDef ota_data_structure;

static uint8_t SYS_CodeChange(void)
{
	if(ota_data_structure.ota_flash_start_addr != CODE_BURN_FLASHADDR1 && ota_data_structure.ota_flash_start_addr != CODE_BURN_FLASHADDR2)
    {
	    return OTA_FLASHADDR_ERROR;
	}

    uint8_t datatemp[3];
    uint32_t code_start_addr = ota_data_structure.ota_flash_start_addr + CODE_OFFSET_LEN + SERIAL_NAME_LEN;
	datatemp[0] = (uint8_t)(code_start_addr >> 16);
	datatemp[1] = (uint8_t)((code_start_addr & 0xff00) >> 8);
	datatemp[2] = (uint8_t)code_start_addr;

    QSPI_SectorEraseFlash(0);
    QSPI_WritePageFlashData(0, 3, datatemp);

    SYS_RESET();
	while(1);
}

static bool OTA_FlashAddr_Init(void)
{
    uint32_t code_burn_addr;
    QSPI_ReadFlashData(0, 3, (uint8_t *)(&ota_data_structure.ota_flash_start_addr));
    code_burn_addr = REVERSE_3BYTE_DEFINE(ota_data_structure.ota_flash_start_addr) - CODE_OFFSET_LEN - SERIAL_NAME_LEN;
    if (code_burn_addr == CODE_BURN_FLASHADDR1) {
        ota_data_structure.ota_flash_start_addr = CODE_BURN_FLASHADDR2;
    }
    else if (code_burn_addr == CODE_BURN_FLASHADDR2) {
        ota_data_structure.ota_flash_start_addr = CODE_BURN_FLASHADDR1;
    }
    else return false;

    uint32_t EraseSectorSum = ota_data_structure.ota_flash_length / 4096;
    for(uint32_t i = 0; i <= EraseSectorSum; i++)
    {
#ifdef FUNCTION_WATCH_DOG
        IWDG_ReloadCounter(&gWdtInit);
#endif
        QSPI_SectorEraseFlash(ota_data_structure.ota_flash_start_addr + (i << 12));
    }
    
    return true;
}

void OTA_RESET(void)
{
    memset(&ota_data_structure, 0, sizeof(ota_data_structure));
}

uint8_t OTA_EVT_HANDLE(uint8_t len, uint8_t *dataPtr)
{
    uint8_t data[7];
    uint8_t datatemp[32];
    uint8_t datalen = 0;

    memset((void *)data, 0, sizeof(data));
    memset((void *)datatemp, 0, sizeof(datatemp));
    
    if(OTA_EVT != *dataPtr) return OTA_APK_ERROR;

    switch(*(dataPtr+1))
    {
    case OTA_EVT_START:
    {
        if (len != 5) return OTA_APK_ERROR;

        if (ota_data_structure.ota_is_starting)
        {
            data[datalen++] = OTA_CMD;
            data[datalen++] = OTA_EVT_START;	
            data[datalen++] = OTA_PACKET_LEN & 0xff;
            data[datalen++] = OTA_PACKET_LEN >> 8;
            ATT_sendNotify(OTA_NOTIFY_HANDLE, data, datalen);
        }
         
        ota_data_structure.ota_is_starting = true;
        ota_data_structure.ota_flash_length = *(dataPtr+2) + (*(dataPtr+3) << 8) + (*(dataPtr+4) << 16);

        if(!OTA_FlashAddr_Init()) return OTA_FLASHADDR_ERROR;

        data[datalen++] = OTA_CMD;
        data[datalen++] = OTA_EVT_START;	
        data[datalen++] = OTA_PACKET_LEN & 0xff;
        data[datalen++] = OTA_PACKET_LEN >> 8;
        ATT_sendNotify(OTA_NOTIFY_HANDLE, data, datalen);
    }
        break;

    case OTA_EVT_DATA:
    {
        if(!ota_data_structure.ota_is_starting) return OTA_APK_ERROR;
        
        if(ota_data_structure.ota_packetid == 0 || (ota_data_structure.ota_packetid + 1) == (*(dataPtr+2) + (*(dataPtr+3) << 8)))
        {
            ota_data_structure.ota_packetid = *(dataPtr+2) + (*(dataPtr+3) << 8);
        }
        else return OTA_PACKET_ID_ERROR;
        
        if((*(dataPtr+4) + ((*(dataPtr+5)) << 8)) > 0)
        {
            QSPI_WritePageFlashData(ota_data_structure.ota_flash_start_addr + ota_data_structure.ota_datalen, (*(dataPtr+4) + ((*(dataPtr+5)) << 8)), dataPtr+6);
            ota_data_structure.ota_datalen += (*(dataPtr+4) + ((*(dataPtr+5)) << 8));
        }
        else return OTA_PACKET_LEN_ERROR;

        data[datalen++] = OTA_CMD;
        data[datalen++] = OTA_EVT_DATA;	
        data[datalen++] = *(dataPtr+2);
        data[datalen++] = *(dataPtr+3);
        ATT_sendNotify(OTA_NOTIFY_HANDLE, data, datalen);
    }
        break;
    
    case OTA_EVT_END:
    {
        data[datalen++] = OTA_CMD;
        data[datalen++] = OTA_EVT_END;	
        data[datalen++] = *(dataPtr+2);
        data[datalen++] = *(dataPtr+3);

        DEBUG_LOG_STRING("OTA DATALEN[%4x] \r\n", ota_data_structure.ota_datalen);
        for (uint32_t i = 0; i < ota_data_structure.ota_datalen; )
        {
            if(ota_data_structure.ota_datalen - i >= 32)
            {
                QSPI_ReadFlashData(ota_data_structure.ota_flash_start_addr + i, 32, datatemp);	
                for (uint8_t j = 0; j < 32; j++)
                {
                    ota_data_structure.ota_datasum += datatemp[j];
                }

                i += 32;
            }
            else
            {
                QSPI_ReadFlashData(ota_data_structure.ota_flash_start_addr + i, ota_data_structure.ota_datalen - i, datatemp);
                for(uint8_t j = 0; j < ota_data_structure.ota_datalen - i; j++)
                {
                    ota_data_structure.ota_datasum += datatemp[j];
                }

                break;
            }
        }
        DEBUG_LOG_STRING("OTA DATASUM[%4x] \r\n", ota_data_structure.ota_datasum);

        if(ota_data_structure.ota_datasum  == *(dataPtr+4) + (*(dataPtr+5) << 8) + (*(dataPtr+6) << 16)
            && ota_data_structure.ota_datalen  ==  *(dataPtr+7) + (*(dataPtr+8) << 8) + (*(dataPtr+9) << 16))
        {
            data[datalen++] = OTA_END_CHECK_OK;
            ATT_sendNotify(OTA_NOTIFY_HANDLE, data, datalen);
            bt_send_le_disconnect(0x16);
#ifdef FUNCTION_WATCH_DOG
            IWDG_ReloadCounter(&gWdtInit);
#endif
            SysTick_DelayMs(700);
#ifdef FACTORY_MODE
            SYS_RESET();
#else
            return SYS_CodeChange();
#endif
        }
        else {
            data[datalen++]= OTA_END_CHECK_FAIL;
            ATT_sendNotify(OTA_NOTIFY_HANDLE, data, datalen);

            memset(&ota_data_structure, 0, sizeof(ota_data_structure));
            return OTA_DATA_ERROR;
        }
    }
        break;
    
    case OTA_EVT_RESET:
        DEBUG_LOG_STRING("OTA_EVT_RESET \r\n");
        memset(&ota_data_structure, 0, sizeof(ota_data_structure));
        break;
        
    default:
        memset(&ota_data_structure,0, sizeof(ota_data_structure));
        break;
    }

    return OTA_NO_ERROR;
}
