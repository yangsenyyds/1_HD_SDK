/**
  ******************************************************************************
  * @file    usbd_core.h
  * @author  WJJ
  * @version V1.0.1
  * @date    10-29-2021
  * @brief   This file provides all the Application firmware functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Include ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_req.h"
/**
 * @method string
 * @brief  USB  Callback function initialization
 * @param  None
 * @retval None
*/

static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev);
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev);

//static uint8_t  USBD_RunTestMode (USB_OTG_CORE_HANDLE  *pdev) ;

/**
 * @method string
 * @brief  USB  VBUS_SENSING_ENABLED
 * @param  None
 * @retval None
*/

#ifdef VBUS_SENSING_ENABLED
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev);
#endif
/**
 * @method string
 * @brief  USB  Callback function initialization
 * @param  None
 * @retval None
*/
USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb =
{
    USBD_DataOutStage,		/*USBD_DataOutStage callback function*/
    USBD_DataInStage,		/*USBD_DataInStage callback function*/
    USBD_SetupStage,		/*USBD_SetupStage callback function*/
    USBD_SOF,				/*USBD_SOF callback function*/
    USBD_Reset,				/*USBD_Reset callback function*/
    USBD_Suspend,			/*USBD_Suspend callback function*/
    USBD_Resume,			/*USBD_Resume callback function*/
    USBD_IsoINIncomplete,	/*USBD_IsoINIncomplete callback function*/
    USBD_IsoOUTIncomplete,	/*USBD_IsoOUTIncomplete callback function*/
#ifdef VBUS_SENSING_ENABLED
    USBD_DevConnected,		/*USBD_DevConnected callback function*/
    USBD_DevDisconnected,	/*USBD_DevDisconnected callback function*/
#endif
};
USBD_DCD_INT_cb_TypeDef  *USBD_DCD_INT_fops = &USBD_DCD_INT_cb;

/**
 * @method string
 * @brief  USB  Callback function initialization
 * @param  None
 * @retval None
*/
static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev)
{


    USB_SETUP_REQ req;
    //  USB_OTG_CSR0L_IN_PERIPHERAL_TypeDef csr0l;
    //  csr0l.d8 = HREAD(&pdev->regs.INDEXREGS->CSRL.CSR0L);
    USBD_ParseSetupRequest(pdev, &req);
    if(!(req.bmRequest & USB_REQ_TYPE_IN) && req.wLength > 0)
    {
        usb0_clear_out_nak();
        pdev->dev.device_SETUP_OUT = 1;
    }
    switch (req.bmRequest & 0x1F)
    {
    case USB_REQ_RECIPIENT_DEVICE:
        USBD_StdDevReq(pdev, &req);
        break;

    case USB_REQ_RECIPIENT_INTERFACE:
        USBD_StdItfReq(pdev, &req);
        break;

    case USB_REQ_RECIPIENT_ENDPOINT:
        USBD_StdEPReq(pdev, &req);
        break;

    default:
        //        csr0l.b.serviced_rxpktrdy = 1;
        //        csr0l.b.data_end = 1;
        //        HWRITE(&pdev->regs.INDEXREGS->CSRL.CSR0L, csr0l.d8);
        DCD_EP_Stall(pdev, req.bmRequest & 0x80);
        break;
    }
    return USBD_OK;
}

/**
 * @method string
 * @brief  USB  Callback function initialization
 * @param  None
 * @retval None
*/
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    USB_OTG_EP *ep;

    if(epnum == 0)
    {
        ep = &pdev->dev.out_ep[0];
        if ( pdev->dev.device_state == USB_OTG_EP0_DATA_OUT)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;

                if(pdev->cfg.dma_enable == 1)
                {
                    /* in slave mode this, is handled by the RxSTSQLvl ISR */
                    ep->xfer_buff += ep->maxpacket;
                }
                USBD_CtlContinueRx (pdev,
                                    ep->xfer_buff,
                                    MIN(ep->rem_data_len, ep->maxpacket));
            }
            else
            {
                if((pdev->dev.class_cb->EP0_RxReady != NULL) &&
                        (pdev->dev.device_status == USB_OTG_CONFIGURED))
                {
                    pdev->dev.class_cb->EP0_RxReady(pdev);
                }
                USBD_CtlSendStatus(pdev);
            }
        }
    }
    else if((pdev->dev.class_cb->DataOut != NULL) &&
            (pdev->dev.device_status == USB_OTG_CONFIGURED))
    {

        pdev->dev.class_cb->DataOut(pdev, epnum);
    }
    return USBD_OK;
}
/**
 * @method string
 * @brief  USB  Callback USBD_DataInStage
 * @param  None
 * @retval None
*/
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    //USB_OTG_EP *ep;

    if (epnum == 0)
    {
    }
    else if ((pdev->dev.class_cb->DataIn != NULL) && (pdev->dev.device_status == USB_OTG_CONFIGURED))
    {
        pdev->dev.class_cb->DataIn(pdev, epnum);
    }
    return USBD_OK;
}
/**
 * @method string
 * @brief  USB  Callback USBD_DataInStage
 * @param  None
 * @retval None
*/
void usbd_init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBD_DEVICE *pDevice,
               USBD_Class_cb_TypeDef *class_cb,
               USBD_Usr_cb_TypeDef *usr_cb)
{

    /* Hardware Init */
    USBD_DeInit(pdev);
    /*Register class and user callbacks */
    pdev->dev.class_cb = class_cb;
    pdev->dev.usr_cb = usr_cb;
    pdev->dev.usr_device = pDevice;
    /* set USB OTG core params */
    DCD_Init(pdev, coreID);
    /* Upon Init call usr callback */
    //      pdev->dev.usr_cb->Init();



}



/**
* @brief  USBD_DeInit
*         Re-Initialize th device library
* @param  pdev: device instance
* @retval status: status
*/



static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev)
{
    /* Open EP0 OUT */
    DCD_EP_Open(pdev,
                0x00,
                USB_OTG_MAX_EP0_SIZE,
                EP_TYPE_CTRL);

    /* Open EP0 IN */
    DCD_EP_Open(pdev,
                0x80,
                USB_OTG_MAX_EP0_SIZE,
                EP_TYPE_CTRL);

    /* Upon Reset call usr call back */
    pdev->dev.device_status = USB_OTG_DEFAULT;
    pdev->dev.usr_cb->DeviceReset(pdev->cfg.speed);

    return USBD_OK;
}

/**
* @brief  USBD_Resume
*         Handle Resume event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev)
{
    /* Upon Resume call usr call back */
    pdev->dev.usr_cb->DeviceResumed();
    pdev->dev.device_status = pdev->dev.device_old_status;
    pdev->dev.device_status = USB_OTG_CONFIGURED;
    return USBD_OK;
}


/**
* @brief  USBD_Suspend
*         Handle Suspend event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.device_old_status = pdev->dev.device_status;
    pdev->dev.device_status  = USB_OTG_SUSPENDED;
    /* Upon Resume call usr call back */
    pdev->dev.usr_cb->DeviceSuspended();
    return USBD_OK;
}


/**
* @brief  USBD_SOF
*         Handle SOF event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev)
{
    if (pdev->dev.class_cb->SOF)
    {
        pdev->dev.class_cb->SOF(pdev);
    }
    return USBD_OK;
}
/**
* @brief  USBD_SetCfg
*        Configure device and start the interface
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status
*/



/**
* @brief  USBD_IsoINIncomplete
*         Handle iso in incomplete event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.class_cb->IsoINIncomplete(pdev);
    return USBD_OK;
}

/**
* @brief  USBD_IsoOUTIncomplete
*         Handle iso out incomplete event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.class_cb->IsoOUTIncomplete(pdev);
    return USBD_OK;
}

#ifdef VBUS_SENSING_ENABLED
/**
* @brief  USBD_DevConnected
*         Handle device connection event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.usr_cb->DeviceConnected();
    pdev->dev.connection_status = 1;
    return USBD_OK;
}

/**
* @brief  USBD_DevDisconnected
*         Handle device disconnection event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.usr_cb->DeviceDisconnected();
    pdev->dev.class_cb->DeInit(pdev, 0);
    pdev->dev.connection_status = 0;
    return USBD_OK;
}


#endif
