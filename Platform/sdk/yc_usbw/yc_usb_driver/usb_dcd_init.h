/**
  ******************************************************************************
  * @file    usbd_dcd_init.h
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

#ifndef __USB_DCD_INT_H__
#define __USB_DCD_INT_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Include ------------------------------------------------------------------*/
#include "usb_main.h"
#include "usb_core.h"
#include "usbd_conf.h"
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/** @addtogroup USB_OTG_DRIVER
  * @{
  */

/** @defgroup USB_SET_CUR
  * @brief usb_volue_listren
  * @{
  */
extern USB_SET_CUR usb_set_cur_buf[40];
extern int setcur_num;

/** @defgroup USB_DCD_INT_Exported_Defines
  * @{
  */

typedef struct _USBD_DCD_INT
{
    uint8_t (* DataOutStage) (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
    uint8_t (* DataInStage)  (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
    uint8_t (* SetupStage) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* SOF) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* Reset) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* Suspend) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* Resume) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* IsoINIncomplete) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* IsoOUTIncomplete) (USB_OTG_CORE_HANDLE *pdev);

    uint8_t (* DevConnected) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* DevDisconnected) (USB_OTG_CORE_HANDLE *pdev);

} USBD_DCD_INT_cb_TypeDef;

extern USBD_DCD_INT_cb_TypeDef *USBD_DCD_INT_fops;
/**
  * @}
  */


/** @defgroup USB_DCD_INT_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_DCD_INT_Exported_Macros
  * @{
  */
//#define CLEAR_IN_EP_INTR(epnum,intr)
/**
  * @}
  */
#define USB_OTG_EP_CONTROL                       0
#define USB_OTG_EP_ISOC                          1
#define USB_OTG_EP_BULK                          2
#define USB_OTG_EP_INT                           3
#define USB_OTG_EP_MASK                          3

/*  Device Status */
#define USB_OTG_DEFAULT                          1
#define USB_OTG_ADDRESSED                        2
#define USB_OTG_CONFIGURED                       3
#define USB_OTG_SUSPENDED                        4
#define USB_OTG_END                        5
#define USB_OTG_READ10                        6
/**
* @}
*/


/** @defgroup USB_DCD_Exported_Types
* @{
*/
/********************************************************************************
Data structure type
********************************************************************************/
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
}
EP_DESCRIPTOR, *PEP_DESCRIPTOR;

/**
* @}
*/


/** @defgroup USB_DCD_Exported_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USB_DCD_Exported_Variables
* @{
*/
/**
* @}
*/

/** @defgroup USB_DCD_Exported_FunctionsPrototype
* @{
*/
/********************************************************************************
EXPORTED FUNCTION FROM THE USB-OTG LAYER
********************************************************************************/
void       DCD_Init(USB_OTG_CORE_HANDLE *pdev,
                    USB_OTG_CORE_ID_TypeDef coreID);

void        DCD_DevConnect (USB_OTG_CORE_HANDLE *pdev);
void        DCD_DevDisconnect (USB_OTG_CORE_HANDLE *pdev);
void        DCD_EP_SetAddress (USB_OTG_CORE_HANDLE *pdev,
                               uint8_t address);
uint32_t    DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev,
                        uint8_t ep_addr,
                        uint16_t ep_mps,
                        uint8_t ep_type);

uint32_t    DCD_EP_Close  (USB_OTG_CORE_HANDLE *pdev,
                           uint8_t  ep_addr);


uint32_t   DCD_EP_PrepareRx ( USB_OTG_CORE_HANDLE *pdev,
                              uint8_t   ep_addr,
                              uint8_t *pbuf,
                              uint16_t  buf_len);

uint32_t    DCD_EP_Tx (USB_OTG_CORE_HANDLE *pdev,
                       uint8_t  ep_addr,
                       uint8_t  *pbuf,
                       uint32_t   buf_len);
uint32_t    DCD_EP_Stall (USB_OTG_CORE_HANDLE *pdev,
                          uint8_t   epnum);
uint32_t    DCD_EP_ClrStall (USB_OTG_CORE_HANDLE *pdev,
                             uint8_t epnum);
uint32_t    DCD_EP_Flush (USB_OTG_CORE_HANDLE *pdev,
                          uint8_t epnum);
uint32_t    DCD_Handle_ISR(USB_OTG_CORE_HANDLE *pdev);

uint32_t DCD_GetEPStatus(USB_OTG_CORE_HANDLE *pdev,
                         uint8_t epnum);

void DCD_SetEPStatus (USB_OTG_CORE_HANDLE *pdev,
                      uint8_t epnum,
                      uint32_t Status);

/**
* @}
*/

/** @defgroup USB_DCD_INT_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_DCD_INT_Exported_FunctionsPrototype
  * @{
  */
uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

//uint32_t USBD_OTG_DMA_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
uint32_t USBD_OTG_DMA_ISR_Handler (USB_OTG_CORE_HANDLE *pdev, uint8_t dma_intr_value);

byte usb_getbyte();
word usb_getword();
void usb_clkcal_done();

void usb0_control_data(USB_OTG_CORE_HANDLE *pdev, uint16_t len);
void usb_reset();
void _delay_(uint32_t t);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif	/* __USB_DCD_INT_H__ */

/************************ (C) COPYRIGHT 2014 Megahuntmicro ****END OF FILE****/
