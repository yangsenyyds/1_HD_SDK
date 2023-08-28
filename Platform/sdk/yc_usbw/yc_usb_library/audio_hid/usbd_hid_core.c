/**
  ******************************************************************************
  * @file    usbd_hid_core.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                HID Class  Description
  *          ===================================================================
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick)
  *             - Collection : Application
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid_core.h"
#include "usbd_audio_core.h"
#include "usbd_audio_out_if.h"
#include "usbd_desc.h"

#include "usbd_req.h"

#include "yc11xx.h"


/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_HID_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_HID_Private_Macros
  * @{
  */
/**
  * @}
  */




/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */

static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req);
static void  AUDIO_Req_GetVolumeMax(void *pdev, USB_SETUP_REQ *req);
static void  AUDIO_Req_GetVolumeMin(void *pdev, USB_SETUP_REQ *req);
static void  AUDIO_Req_GetVolumeRes(void *pdev, USB_SETUP_REQ *req);

static uint8_t  USBD_HID_Init (void  *pdev,
                               uint8_t cfgidx);

static uint8_t  USBD_HID_DeInit (void  *pdev,
                                 uint8_t cfgidx);

static uint8_t  USBD_HID_Setup (USB_OTG_CORE_HANDLE  *pdev,
                                USB_SETUP_REQ *req);

static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length);

static uint8_t  USBD_HID_DataIn (void  *pdev, uint8_t epnum);
uint8_t USBD_HID_DataOut(void *pdev, uint8_t epnum);
/**
  * @}
  */

/** @defgroup USBD_HID_Private_Variables
  * @{
  */

uint8_t gAudioStart = 0;

USBD_Class_cb_TypeDef  USBD_cb =
{
    USBD_HID_Init,
    USBD_HID_DeInit,
    USBD_HID_Setup,
    NULL, /*EP0_TxSent*/
    NULL, /*EP0_RxReady*/
    USBD_HID_DataIn, /*DataIn*/
    USBD_HID_DataOut, /*DataOut*/
    NULL, /*SOF */
    NULL,
    NULL,
    USBD_HID_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
    USBD_HID_GetCfgDesc, /* use same config as per FS */
#endif
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static uint32_t  USBD_HID_AltSet  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static uint32_t  USBD_HID_Protocol  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static uint32_t  USBD_HID_IdleState __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB HID device Configuration Descriptor */
//static uint8_t usbd_audio_CfgDesc[AUDIO_CONFIG_DESC_SIZE];

static uint8_t usbd_audio_CfgDesc[AUDIO_CONFIG_DESC_SIZE] =
{
    0x09, 0x02, 0xDA, 0x00, 0x04, 0x01, 0x00, 0x80, 0xC8, 0x09, 0x04, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00, 0x0A, 0x24, 0x01, 0x00, 0x01, 0x47, 0x00, 0x02, 0x01, 0x02,
    0x0C, 0x24, 0x02, 0x01, 0x01, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x0A, 0x24,
    0x06, 0x09, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x09, 0x24, 0x03, 0x03, 0x01, 0x03,
    0x00, 0x09, 0x00, 0x0C, 0x24, 0x02, 0x02, 0x01, 0x02, 0x00, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x09, 0x24, 0x06, 0x0A, 0x02, 0x01, 0x03, 0x00, 0x00, 0x09, 0x24, 0x03, 0x04,
    0x01, 0x01, 0x00, 0x0A, 0x00, 0x09, 0x04, AUDIO_OUT_INTERFACENUM, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,
    0x09, 0x04, AUDIO_OUT_INTERFACENUM, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 0x07, 0x24, 0x01, 0x01, 0x01,
    0x01, 0x00, 0x0B, 0x24, 0x02, 0x01, 0x02, AUDIO_OUTFRAME_SIZE, AUDIO_BITRESOLUTION, 0x01, 0x80, 0xBB, 0x00, 0x09,
    0x05, 0x02, 0x09, AUDIO_OUTMAX_PACKETSIZE_LOW, AUDIO_OUTMAX_PACKETSIZE_HIGH, 0x01, 0x00, 0x00, 0x07, 0x25, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x09, 0x04, AUDIO_IN_INTERFACENUM, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x09, 0x04, AUDIO_IN_INTERFACENUM, 0x01,
    0x01, 0x01, 0x02, 0x00, 0x00, 0x07, 0x24, 0x01, 0x04, 0x01, 0x01, 0x00, 0x0B, 0x24,
    0x02, 0x01, 0x01, 0x02, 0x10, 0x01, 0x80, 0xBB, 0x00, 0x09, 0x05, 0x82, 0x09, 0x60,
    0x00, 0x01, 0x00, 0x00, 0x07, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x09, 0x04, HID_IN_INTERFACENUM,
    0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x09, 0x21, 0x01, 0x02, 0x00, 0x01, 0x22, HID_MOUSE_REPORT_DESC_SIZE,
    0x00, 0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x01
} ;
static uint8_t HID_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE]  =
{
    0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x01, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01,
    0x95, 0x01, 0x09, 0xE9, 0x81, 0x02, 0x09, 0xEA, 0x81, 0x02, 0x09, 0xCD, 0x81, 0x02,
    0x09, 0xB5, 0x81, 0x02, 0x09, 0xB6, 0x81, 0x02, 0x09, 0xB7, 0x81, 0x02, 0x09, 0xB3,
    0x81, 0x02, 0x09, 0xB4, 0x81, 0x02, 0x05, 0x0B, 0x09, 0x24, 0x81, 0x02, 0x09, 0x20,
    0x81, 0x02, 0x09, 0x2F, 0x81, 0x06, 0x95, 0x05, 0x81, 0x01, 0xC0,

};




/******************************************************************************
     AUDIO Class requests management
******************************************************************************/


uint8_t  Audiovolmute[2];
uint8_t AudioVolumeMax[2] = {0xff, 0x80};
uint8_t AudioVolumeMin[2] = {0x00, 0x80};
uint8_t AudioVolumeRes[2] = {0x01, 0x00};
uint8_t  AudioCtl[64] = {0x3E, 0x80, 00};
uint8_t  AudioCtlCmd = 0;
uint32_t AudioCtlLen = 0;
uint8_t  AudioCtlUnit = 0;

/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req)
{
    /* Send the current mute state */
    USBD_CtlSendData (pdev,
                      AudioCtl,
                      req->wLength);
}


static void  AUDIO_Req_GetVolumeMax(void *pdev, USB_SETUP_REQ *req)
{
    USBD_CtlSendData (pdev,
                      AudioVolumeMax,
                      req->wLength);

}
static void  AUDIO_Req_GetVolumeMin(void *pdev, USB_SETUP_REQ *req)
{
    USBD_CtlSendData (pdev,
                      AudioVolumeMin,
                      req->wLength);

}

static void  AUDIO_Req_GetVolumeRes(void *pdev, USB_SETUP_REQ *req)
{
    USBD_CtlSendData (pdev,
                      AudioVolumeRes,
                      req->wLength);

}



/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */

static void AUDIO_Req_SetCurrent(void *pdev, USB_SETUP_REQ *req)
{
    if (req->wLength)
    {
        /* Prepare the reception of the buffer over EP0 */
        HWRITE(core_usb_trig, 0x10);
        /* Set the global variables indicating current request and its length
        to the function usbd_audio_EP0_RxReady() which will process the request */
        /* Set the request value */
        AudioCtlLen = req->wLength;          /* Set the request data length */
        AudioCtlUnit = HIBYTE(req->wIndex);  /* Set the request target unit */
    }
}


/**
  * @}
  */

/** @defgroup USBD_HID_Private_Functions
  * @{
  */

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_Init (void  *pdev,
                               uint8_t cfgidx)
{

    /* Open EP IN */
    DCD_EP_Open(pdev,
                HID_IN_EP,
                HID_IN_PACKET,
                USB_OTG_EP_INT);

    /* Open EP OUT */
    DCD_EP_Open(pdev,
                HID_OUT_EP,
                HID_OUT_PACKET,
                USB_OTG_EP_INT);

    return USBD_OK;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_DeInit (void  *pdev,
                                 uint8_t cfgidx)
{
    /* Close HID EPs */
    DCD_EP_Close (pdev, HID_IN_EP);
    DCD_EP_Close (pdev, HID_OUT_EP);


    return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
uint8_t test;
extern USB_OTG_CORE_HANDLE  USB_OTG_dev;
static uint8_t  USBD_HID_Setup (USB_OTG_CORE_HANDLE  *pdev,
                                USB_SETUP_REQ *req)
{
    uint16_t len = 0;
    uint8_t rx_count;
    uint8_t  *pbuf = NULL;
    uint8_t discard_buf[8];

    if((req->bmRequest == 0xA2) || (req->bmRequest == 0x22) || (req->bmRequest == 0xA1) || (req->bmRequest == 0x21))
    {
        if(req->bmRequest == 0xA1)
        {
            if(!(req ->bRequest & 0x80))
            {
                USBD_CtlError (pdev, req);
                return USBD_FAIL;
            }
        }
        switch (req->bRequest)
        {
        case AUDIO_REQ_GET_CUR:
            AUDIO_Req_GetCurrent(pdev, req);
            break;
        case AUDIO_REQ_SET_CUR:
            AudioCtlCmd = AUDIO_REQ_SET_CUR;
            switch(HIBYTE(req->wIndex))
            {
            case 0x09:
                USB_OTG_dev.dev.EP0Setcmd_num1++;
                switch(LOBYTE(req->wValue))
                {
                case 0x01:
                    USB_OTG_dev.dev.Channel = 1;
                    break;
                case 0x02:
                    USB_OTG_dev.dev.Channel = 2;
                    break;
                }
                break;
            case 0x0a:
                USB_OTG_dev.dev.EP0Setcmd_num2++;
                break;
            }
            //    	 usb0_clear_in_nak();
            USBD_CtlSendData (pdev,
                              0,
                              0);
            break;
        case AUDIO_REQ_GET_MAX:
            AUDIO_Req_GetVolumeMax(pdev, req);
            break;
        case AUDIO_REQ_GET_MIN:
            AUDIO_Req_GetVolumeMin(pdev, req);
            break;
        case AUDIO_REQ_GET_RES:
            AUDIO_Req_GetVolumeRes(pdev, req);
            break;
        case HID_REQ_SET_IDLE:
            //     	 USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
            //       	 USB_OTG_dev.dev.device_status_new = USB_OTG_END;
            //         usb0_clear_in_nak();
            USBD_CtlSendData (pdev,
                              0,
                              0);
            return USBD_FAIL;
            break;
        default:
            USBD_CtlError (pdev, req);
            return USBD_FAIL;
        }

    }
    else
    {
        switch (req->bmRequest & USB_REQ_TYPE_MASK)
        {
        case USB_REQ_TYPE_CLASS :
            switch (req->bRequest)
            {

            case HID_REQ_SET_PROTOCOL:
                USBD_HID_Protocol = (uint8_t)(req->wValue);
                break;

            case HID_REQ_GET_PROTOCOL:
                USBD_CtlSendData (pdev,
                                  (uint8_t *)&USBD_HID_Protocol,
                                  1);
                break;

            case HID_REQ_SET_IDLE:
                return USBD_FAIL;
            //break;
            case HID_REQ_SET_REPORT:
                //
                break;

            case HID_REQ_GET_IDLE:
                USBD_CtlSendData (pdev,
                                  (uint8_t *)&USBD_HID_IdleState,
                                  1);
                break;

            default:
                USBD_CtlError (pdev, req);
                return USBD_FAIL;
            }
            break;

        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
            case USB_REQ_GET_DESCRIPTOR:
                if( req->wValue >> 8 == HID_REPORT_DESC)
                {
                    len = MIN(HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
                    pbuf = HID_ReportDesc;
                }
                else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
                {

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
                    pbuf = USBD_HID_Desc;
#else
                    pbuf = usbd_audio_CfgDesc + 0x12;
#endif
                    len = MIN(USB_HID_DESC_SIZ, req->wLength);
                }

                USBD_CtlSendData (pdev,
                                  pbuf,
                                  len);
                return USBD_FAIL;

                break;

            case USB_REQ_GET_INTERFACE :
                USBD_CtlSendData (pdev,
                                  (uint8_t *)&USBD_HID_AltSet,
                                  1);
                usb0_clear_in_nak();
                break;

            case USB_REQ_SET_INTERFACE :
                pdev->dev.device_status = USB_OTG_CONFIGURED;
                USBD_HID_AltSet = (uint8_t)(req->wValue);
                USBD_HID_Handle_Set_Interface((req->wIndex), USBD_HID_AltSet);
                usb0_clear_in_nak();
                break;
            }
        }
        return USBD_OK;
    }
}

/**
  * @brief  USBD_HID_SendReport
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport     (USB_OTG_CORE_HANDLE  *pdev,
                                 uint8_t *report,
                                 uint16_t len)
{
    if (pdev->dev.device_status == USB_OTG_CONFIGURED )
    {
        DCD_EP_Tx (pdev, HID_IN_EP, report, len);
    }
    return USBD_OK;
}

/**
  * @brief  USBD_HID_GetCfgDesc
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = sizeof (usbd_audio_CfgDesc);
    return usbd_audio_CfgDesc;
}

/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_HID_DataIn (void  *pdev,
                                 uint8_t epnum)
{

    /* Ensure that the FIFO is empty before a new transfer, this condition could
    be caused by  a new transfer before the end of the previous transfer */

    // DCD_EP_Flush(pdev, HID_IN_EP);
    return USBD_OK;
}

/**
  * @}
  */

uint8_t receive_data_flag;
uint8_t HidOut_Data_Buff[HID_MAX_PACKET_SIZE];

uint8_t USBD_HID_DataOut(void *pdev,
                         uint8_t epnum)
{

    //printf("333333\n");

    DCD_EP_PrepareRx(pdev,
                     HID_OUT_EP,
                     (uint8_t *)&HidOut_Data_Buff[0],
                     HID_EPOUT_SIZE);
    receive_data_flag = 1;
    return USBD_OK;
}


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
