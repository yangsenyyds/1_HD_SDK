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

#ifndef __USBD_CORE_H
#define __USBD_CORE_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <usb_dcd_init.h>

/* Define to prevent recursive inclusion -------------------------------------*/


/**
 * @method string
 * @brief  USB VBUS_SENSING_ENABLED define
 * @param  None
 * @retval None
*/
#define VBUS_SENSING_ENABLED
/**
 * @method string
 * @brief  USB High and low bit inversion
 * @param  None
 * @retval None
*/
#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + (((uint16_t)(*(((uint8_t *)(addr)) + 1))) << 8))
#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))

/**
 * @method string
 * @brief  USB USB_REQ_TYPE_IN
 * @param  None
 * @retval None
*/
#define  USB_REQ_TYPE_IN   								0x80

/**
 * @method string
 * @brief  USB High and low bit inversion
 * @param  None
 * @retval None
*/


/**
 * @method string
 * @brief  USB USBD_IDX_
 * @param  None
 * @retval None
*/
#define  USBD_IDX_LANGID_STR                            0x00
#define  USBD_IDX_MFC_STR                               0x01
#define  USBD_IDX_PRODUCT_STR                           0x02
#define  USBD_IDX_SERIAL_STR                            0x03
#define  USBD_IDX_CONFIG_STR                            0x04
#define  USBD_IDX_INTERFACE_STR                         0x05
/**
 * @method string
 * @brief  USB USB_REQ_TYPE_STANDARD
 * @param  None
 * @retval None
*/
#define  USB_REQ_TYPE_STANDARD                          0x00
#define  USB_REQ_TYPE_CLASS                             0x20
#define  USB_REQ_TYPE_VENDOR                            0x40
#define  USB_REQ_TYPE_MASK                              0x60
/**
 * @method string
 * @brief  USB USB_REQ_RECIPIENT_
 * @param  None
 * @retval None
*/
#define  USB_REQ_RECIPIENT_DEVICE                       0x00
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02
#define  USB_REQ_RECIPIENT_MASK                         0x03
#define  USB_REQ_VENDOR         						0xc0
/**
 * @method string
 * @brief  USB USB_REQ_GET/SET
 * @param  None
 * @retval None
*/
#define  USB_REQ_GET_STATUS                             0x00
#define  USB_REQ_CLEAR_FEATURE                          0x01
#define  USB_REQ_SET_FEATURE                            0x03
#define  USB_REQ_SET_ADDRESS                            0x05
#define  USB_REQ_GET_DESCRIPTOR                         0x06
#define  USB_REQ_SET_DESCRIPTOR                         0x07
#define  USB_REQ_GET_CONFIGURATION                      0x08
#define  USB_REQ_SET_CONFIGURATION                      0x09
#define  USB_REQ_GET_INTERFACE                          0x0A
#define  USB_REQ_SET_INTERFACE                          0x0B
#define  USB_REQ_SYNCH_FRAME                            0x0C
/**
 * @method string
 * @brief  USB USB_DESC_TYPE
 * @param  None
 * @retval None
*/
#define  USB_DESC_TYPE_DEVICE                              1
#define  USB_DESC_TYPE_CONFIGURATION                       2
#define  USB_DESC_TYPE_STRING                              3
#define  USB_DESC_TYPE_INTERFACE                           4
#define  USB_DESC_TYPE_ENDPOINT                            5
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    6
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           7
#define  USB_DESC_TYPE_INTERFACE_POWER                     8
#define  USB_DESC_TYPE_OTGTYPE                             9
/**
 * @method string
 * @brief  USB USB_CONFIG_
 * @param  None
 * @retval None
*/
#define USB_CONFIG_REMOTE_WAKEUP                           2
#define USB_CONFIG_SELF_POWERED                            1
/**
 * @method string
 * @brief  USB USB_FEATURE_
 * @param  None
 * @retval None
*/
#define USB_FEATURE_EP_HALT                                0
#define USB_FEATURE_REMOTE_WAKEUP                          1
#define USB_FEATURE_TEST_MODE                              2

/**
 * @method string
 * @brief  USB USBD_Status
 * @param  None
 * @retval None
*/




/**
 * @method string
 * @brief  USB usbd_init
 * @param  None
 * @retval None
*/
void usbd_init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBD_DEVICE *pDevice,
               USBD_Class_cb_TypeDef *class_cb,
               USBD_Usr_cb_TypeDef *usr_cb);



/**
  * @}
  */

#endif /* __USBD_CORE_H */


/************************ (C) COPYRIGHT 2014 YICHIP ****END OF FILE****/
