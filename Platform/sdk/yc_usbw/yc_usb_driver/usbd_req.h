/**
  ******************************************************************************
  * @file    usbd_req.h
  * @author  WJJ
  * @version V1.0.1
  * @date    10-29-2021
  * @brief   This file provides all the Application firmware functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


#ifndef __USB_REQUEST_H_
#define __USB_REQUEST_H_


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include  "usbd_core.h"
#include  "usbd_conf.h"

/**
  * @brief  USBD_StdDevReq
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted string buffer (unicode)
  * @param  len : descriptor length
  * @retval None
  */
USBD_Status  USBD_StdDevReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
USBD_Status  USBD_StdItfReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
USBD_Status  USBD_StdEPReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req);
/**
  * @brief  USBD_ParseSetupRequest
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted string buffer (unicode)
  * @param  len : descriptor length
  * @retval None
  */
void USBD_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ *req);
void USBD_CtlError( USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ *req);
void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);
/**
  * @}
  */

#endif /* __USB_REQUEST_H_ */

/************************ (C) COPYRIGHT 2014 YICHIP ****END OF FILE****/
