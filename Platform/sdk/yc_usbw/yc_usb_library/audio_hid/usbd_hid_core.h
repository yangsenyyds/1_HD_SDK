/**
  ******************************************************************************
  * @file    usbd_hid_core.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   header file for the usbd_hid_core.c file.
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

#ifndef __USB_HID_CORE_H_
#define __USB_HID_CORE_H_


#include "app_config.h"
#include "usb_core.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_HID
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */

#define HID_EPIN_SIZE          HID_EP_MAX_PACKET
#define HID_EPOUT_SIZE        HID_EP_MAX_PACKET
#define HID_EP0_BUFF_SIZ            64
#define HID_MAX_PACKET_SIZE  64
extern uint8_t HidOut_Data_Buff[HID_MAX_PACKET_SIZE];
#define USB_HID_CONFIG_DESC_SIZ       41//41//34	//DT??
#define USB_HID_DESC_SIZ              9
#define HID_MOUSE_REPORT_DESC_SIZE   67//27//33//74	//DT??
//#define HID_MOUSE_REPORT_DESC_SIZE    52

#define HID_DESCRIPTOR_TYPE           0x21
#define HID_REPORT_DESC               0x22


#define HID_REQ_SET_PROTOCOL          0x0B
#define HID_REQ_GET_PROTOCOL          0x03

#define HID_REQ_SET_IDLE              0x0A
#define HID_REQ_GET_IDLE              0x02

#define HID_REQ_SET_REPORT            0x09
#define HID_REQ_GET_REPORT            0x01


#define AUDIO_OUT_INTERFACENUM    0X01
#define AUDIO_IN_INTERFACENUM    0X02
#define HID_IN_INTERFACENUM    0X03

#define INTERFACE_ENABLE       0X01
#define INTERFACE_DISABLE       0X00

#ifndef FUNCTION_24BIT_DAC
#define AUDIO_OUTFRAME_SIZE 0x02
#define AUDIO_BITRESOLUTION 0x10
#define AUDIO_OUTMAX_PACKETSIZE_HIGH 0x00
#define AUDIO_OUTMAX_PACKETSIZE_LOW 0xc0
#else
#define AUDIO_OUTFRAME_SIZE 0x03
#define AUDIO_BITRESOLUTION 0x18
#define AUDIO_OUTMAX_PACKETSIZE_HIGH 0x01
#define AUDIO_OUTMAX_PACKETSIZE_LOW 0x20



#define TOTAL_OUT_BUF_SIZE                           ((uint32_t)(AUDIO_OUT_PACKET * OUT_PACKET_NUM))

#define AUDIO_CONFIG_DESC_SIZE                        218
#define AUDIO_INTERFACE_DESC_SIZE                     9
#define USB_AUDIO_DESC_SIZ                            0x09
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07

#define AUDIO_DESCRIPTOR_TYPE                         0x21
#define USB_DEVICE_CLASS_AUDIO                        0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02
#define AUDIO_PROTOCOL_UNDEFINED                      0x00
#define AUDIO_STREAMING_GENERAL                       0x01
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25

/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07

#define AUDIO_CONTROL_MUTE                            0x0001

#define AUDIO_FORMAT_TYPE_I                           0x01
#define AUDIO_FORMAT_TYPE_III                         0x03

#define USB_ENDPOINT_TYPE_ISOCHRONOUS                 0x0D
#define AUDIO_ENDPOINT_GENERAL                        0x01

#define AUDIO_REQ_GET_CUR                             0x81
#define AUDIO_REQ_SET_CUR                             0x01
#define AUDIO_REQ_GET_MAX                             0x83
#define AUDIO_REQ_GET_MIN                             0x82
#define AUDIO_REQ_GET_RES                             0x84

#define AUDIO_OUT_STREAMING_CTRL                      0x02

#define AUDIO_TOTAL_IF_NUM   0x03
#endif
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */


/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */
USBD_Class_cb_TypeDef  USBD_cb;

/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_HID_SendReport (USB_OTG_CORE_HANDLE  *pdev,
                             uint8_t *report,
                             uint16_t len);
/**
  * @}
  */

#endif  // __USB_HID_CORE_H_
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
