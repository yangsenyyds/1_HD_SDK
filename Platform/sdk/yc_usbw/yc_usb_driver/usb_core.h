/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : usbd_core.h
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 21-October-2014
 * Description          : Specific api's relative to the used hardware platform.
 *****************************************************************************/


#ifndef __USB_CORE_H__
#define __USB_CORE_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Include ------------------------------------------------------------------*/

//#include "usb_conf.h"

#include "yc11xx.h"

/****************** USB OTG HS CONFIGURATION **********************************/
#ifdef USB_OTG_HS_CORE
#define RX_FIFO_HS_SIZE                          512
#define TX0_FIFO_HS_SIZE                         128
#define TX1_FIFO_HS_SIZE                         372
#define TX2_FIFO_HS_SIZE                          0
#define TX3_FIFO_HS_SIZE                          0
#define TX4_FIFO_HS_SIZE                          0
#define TX5_FIFO_HS_SIZE                          0

// #define USB_OTG_HS_SOF_OUTPUT_ENABLED

#ifdef USE_ULPI_PHY
#define USB_OTG_ULPI_PHY_ENABLED
#endif
#ifdef USE_EMBEDDED_PHY
#define USB_OTG_EMBEDDED_PHY_ENABLED
#endif
#define USB_OTG_HS_INTERNAL_DMA_ENABLED
#define USB_OTG_HS_DEDICATED_EP1_ENABLED
#define USB_OTG_HS_LOW_PWR_MGMT_SUPPORT
#endif

/****************** USB OTG FS CONFIGURATION **********************************/
// total fifo size <= 448 (512 - 64)
#define RX1_FIFO_FS_SIZE                         	0
#define RX2_FIFO_FS_SIZE                          	0
#define RX3_FIFO_FS_SIZE                          	0
#define RX4_FIFO_FS_SIZE                          	0
#define RX5_FIFO_FS_SIZE                          	0
#define RX6_FIFO_FS_SIZE                          	0
#define RX7_FIFO_FS_SIZE                          	0

#define TX1_FIFO_FS_SIZE                         	64
#define TX2_FIFO_FS_SIZE                          	0
#define TX3_FIFO_FS_SIZE                          	0
#define TX4_FIFO_FS_SIZE                          	0
#define TX5_FIFO_FS_SIZE                          	0
#define TX6_FIFO_FS_SIZE                          	0
#define TX7_FIFO_FS_SIZE                          	0

#define TX0_FIFO_FS_SIZE                           64
#define RX0_FIFO_FS_SIZE                           64

#define IS_FIFO_SIZE(size)          (((size) == 0)   || \
                                     ((size) == 8)   || \
                                     ((size) == 16)  || \
                                     ((size) == 32)  || \
                                     ((size) == 64)  || \
                                     ((size) == 128) || \
                                     ((size) == 256) || \
                                     ((size) == 512))
//#define USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
// #define USB_OTG_FS_SOF_OUTPUT_ENABLED

/****************** USB OTG MISC CONFIGURATION ********************************/
#define VBUS_SENSING_ENABLED

/****************** USB OTG MODE CONFIGURATION ********************************/
//#define USE_HOST_MODE
#define USE_DEVICE_MODE
#define USB_OTG_DEV_DMA_EP_NUM              0x81
#define USBD_CFG_MAX_NUM           1
#define USBD_ITF_MAX_NUM           4

#define USB_MAX_STR_DESC_SIZ       64

#define USBD_SELF_POWERED



#define USBD_DYNAMIC_DESCRIPTOR_CHANGE_ENABLED

/** @defgroup USB_String_Descriptors
  * @{
  */


/** @defgroup USB_HID_Class_Layer_Parameter
  * @{
  */

/** @defgroup USB_CORE_Exported_Defines
  * @{
  */

#define USB_OTG_EP0_IDLE                          0
#define USB_OTG_EP0_SETUP                         1
#define USB_OTG_EP0_DATA_IN                       2
#define USB_OTG_EP0_DATA_OUT                      3
#define USB_OTG_EP0_STATUS_IN                     4
#define USB_OTG_EP0_STATUS_OUT                    5
#define USB_OTG_EP0_STALL                         6

#define USB_OTG_EP_TX_DIS                         0x0000
#define USB_OTG_EP_TX_STALL                       0x0010
#define USB_OTG_EP_TX_NAK                         0x0020
#define USB_OTG_EP_TX_VALID                       0x0030

#define USB_OTG_EP_RX_DIS                         0x0000
#define USB_OTG_EP_RX_STALL                       0x1000
#define USB_OTG_EP_RX_NAK                         0x2000
#define USB_OTG_EP_RX_VALID                       0x3000
/**
  * @}
  */
#define MAX_DATA_LENGTH                           0x200

#define USB_OTG_FS_BASE_ADDR               core_usb_config
#define NUM_EP_FIFO                               4
#define USB_OTG_EP_FIFO_REG_OFFSET                0x0001

#define USB_OTG_MAX_TX_FIFOS                 15

#define USB_OTG_HS_MAX_PACKET_SIZE           512
#define USB_OTG_FS_MAX_PACKET_SIZE           64
#define USB_OTG_MAX_EP0_SIZE                 64

#define USB_BASEADDR            	      0x8c00
#define core_usb_config          		  0x8c00
#define core_usb_addr                     0x8c01
#define core_usb_int_mask                 0x8c02
#define core_usb_rx_saddr                 0x8c04
#define core_usb_rx_buflen                0x8c06
#define core_usb_rxptr                    0x8c08
#define core_usb_tx_saddr0                0x8c0a
#define core_usb_tx_saddr1                0x8c0c
#define core_usb_tx_saddr2                0x8c0e
#define core_usb_tx_saddr3                0x8c10
#define core_usb_hmode                    0x8c12
#define core_usb_trig                     0x8c20
#define core_usb_stall                 	  0x8c21
#define core_usb_clear                    0x8c22
#define core_usb_clear_nak                0x8c23
#define core_usb_rx_wptr                  0x8c30
#define core_usb_status                   0x8c32
#define core_usb_status_stall             0x8c34
#define core_usb_txbusy                   0x8c35
#define core_usb_sof_cnt                  0x8c36
#define core_usb_ccnt                     0x8c38
#define core_usb_rx_cnt                	  0x8c3a


#define USB_EPM0    0x00
#define USB_EPM1    0x01
#define USB_EPM2    0x02
#define USB_EPM3    0x03

typedef struct _USB_OTG_CTRLREGS
{
    volatile uint8_t USB_CTL;
    volatile uint8_t IRQ_MASK;
    volatile uint8_t FADDR;
} USB_OTG_CTRLREGS;


typedef struct _USB_OTG_DMA_RXCTL
{
    volatile uint8_t USB_RX_SADDR[3];
    volatile uint8_t USB_RX_EADDR[3];
    volatile uint8_t USB_RXptr[3];

} USB_OTG_DMA_RXCTL;


typedef struct _USB_OTG_XFERCTL
{
    volatile uint8_t TRIG;
    volatile uint8_t STALL;
    volatile uint8_t TOGGLE;
} USB_OTG_XFERCTL;


typedef struct _USB_OTG_STATUSREGS
{
    volatile uint8_t STALL_STATUS;
    volatile uint8_t STATUS;
    volatile uint8_t USB_TX_BUSY;
    volatile uint8_t SOF_CNTL;
    volatile uint8_t SPF_CNTH;
} USB_OTG_STATUSREGS;


typedef struct _USB_OTG_HMODE
{
    volatile uint8_t Hmode;
    volatile uint8_t DRV;
} USB_OTG_HMODE;
typedef struct
{
    USB_OTG_CTRLREGS 	*CTRLREGS;
    USB_OTG_DMA_RXCTL	*RXCTL;
    uint16_t					*TXADDR[NUM_EP_FIFO];
    USB_OTG_HMODE		*Hmode;
    USB_OTG_XFERCTL		*xferctl;
    volatile uint8_t 			RX_Wptr[3];
    USB_OTG_STATUSREGS	*STATUSEGS;

} USB_OTG_CORE_REGS, *PUSB_OTG_CORE_REGS;




typedef union _USB_OTG_CTL_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t enable_iso_outep2			: 1;
        uint8_t enable_iso_inep2			: 1;
        uint8_t pad_bias_ctl         		: 2;
        uint8_t enable_usb_function			: 1;
        uint8_t speed						: 1;
        uint8_t resume  					: 1;
        uint8_t wakeup_enable				: 1;

    } b;


} USB_OTG_CTL_TypeDef;


typedef union _USB_OTG_IRQ_MASK1_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t setup_mask				: 1;
        uint8_t suspend_mask				: 1;
        uint8_t nak_mask				: 1;
        uint8_t reset_mask				: 1;
        uint8_t ack_mask					: 1;
        uint8_t rx_ready_mask				: 1;
        uint8_t tx_empty					: 1;
        uint8_t reserved1					: 1;
    }		b;

}		USB_OTG_IRQ_MASK1_TypeDef;








typedef union _USB_OTG_trig_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t send_ep0_data										: 1;
        uint8_t send_ep1_data										: 1;
        uint8_t send_ep2_data										: 1;
        uint8_t send_ep3_data										: 1;
        uint8_t reply_endpiont0_zero_packet			: 1;
        uint8_t reply_endpiont1_zero_packet			: 1;
        uint8_t reply_endpiont2_zero_packet			: 1;
        uint8_t reply_endpiont3_zero_packet			: 1;
    } b;

} USB_OTG_trig_TypeDef;


typedef union _USB_OTG_stall_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t ep0_stall				: 1;
        uint8_t inep1_stall				: 1;
        uint8_t outep1_stall				: 1;
        uint8_t inep2_stall				: 1;
        uint8_t outep2_stall				: 1;
        uint8_t inep3_stall				: 1;
        uint8_t outep3_stall				: 1;
        uint8_t host_mode_start				: 1;
    } b;

} USB_OTG_stall_TypeDef;

typedef union _USB_OTG_toggle_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t clear_inep1_to_data0		: 1;
        uint8_t clear_outep1_to_data0	: 1;
        uint8_t clear_inep2_to_data0		: 1;
        uint8_t clear_outep2_to_data0	: 1;
        uint8_t clear_inep3_to_data0		: 1;
        uint8_t clear_outep3_to_data0	: 1;
        uint8_t reset						: 1;
        uint8_t reserved						: 1;
    } b;

} USB_OTG_toggle_TypeDef;

typedef union _USB_OTG_stall_status_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t ep0_is_stall				: 1;
        uint8_t inep1_is_stall			: 1;
        uint8_t outep1_is_stall			: 1;
        uint8_t inep2_is_stall			: 1;
        uint8_t outep2_is_stall			: 1;
        uint8_t inep3_is_stall			: 1;
        uint8_t outep3_is_stall			: 1;
        uint8_t reserved				: 1;
    } b;

} USB_OTG_stall_status_TypeDef;

typedef union _USB_OTG_IRQ_TypeDef
{
    uint16_t d16;
    struct
    {
        uint16_t setup				: 1;
        uint16_t suspend				: 1;
        uint16_t nak				: 1;
        uint16_t reset				: 1;
        uint16_t ack					: 1;
        uint16_t rx_ready				: 1;
        uint16_t rx_full				: 1;
        uint16_t clkcal					: 1;
        uint16_t tx_done0			: 1;
        uint16_t tx_done1			: 1;
        uint16_t tx_done2			: 1;
        uint16_t tx_done3			: 1;

    } b;

} USB_OTG_IRQ_TypeDef;

typedef union _USB_OTG_TXBUSY_TypeDef
{
    uint8_t d8;
    struct
    {
        uint8_t endpiont0_busy				: 1;
        uint8_t endpoint1_busy				: 1;
        uint8_t endpoint2_busy				: 1;
        uint8_t endpoint3_busy				: 1;
        uint8_t dn_status					: 1;
        uint8_t dp_status				    : 1;
        uint8_t rx_empty_state				: 1;
        uint8_t reserved					: 1;
    } b;

} USB_OTG_TXBUSY_TypeDef;
/** @defgroup USB_CORE_Exported_Types
  * @{
  */


typedef enum
{
    USB_OTG_OK = 0,
    USB_OTG_FAIL
} USB_OTG_STS;

typedef enum
{
    HC_IDLE = 0,
    HC_XFRC,
    HC_HALTED,
    HC_NAK,
    HC_NYET,
    HC_STALL,
    HC_XACTERR,
    HC_BBLERR,
    HC_DATATGLERR,
} HC_STATUS;

typedef enum
{
    URB_IDLE = 0,
    URB_DONE,
    URB_NOTREADY,
    URB_ERROR,
    URB_STALL
} URB_STATE;

typedef enum
{
    CTRL_START = 0,
    CTRL_XFRC,
    CTRL_HALTED,
    CTRL_NAK,
    CTRL_STALL,
    CTRL_XACTERR,
    CTRL_BBLERR,
    CTRL_DATATGLERR,
    CTRL_FAIL
} CTRL_STATUS;


typedef struct USB_OTG_hc
{
    uint8_t       dev_addr ;
    uint8_t       ep_num;
    uint8_t       ep_is_in;
    uint8_t       speed;
    uint8_t       do_ping;
    uint8_t       ep_type;
    uint16_t      max_packet;
    uint8_t       data_pid;
    uint8_t       *xfer_buff;
    uint32_t      xfer_len;
    uint32_t      xfer_count;
    uint8_t       toggle_in;
    uint8_t       toggle_out;
    uint32_t       dma_addr;
}
USB_OTG_HC, *PUSB_OTG_HC;

typedef struct USB_OTG_ep
{
    uint8_t        num;
    uint8_t        is_in;
    uint8_t        is_stall;
    uint8_t        type;
    uint8_t        data_pid_start;
    uint8_t        even_odd_frame;
    uint8_t       tx_fifo_num;
    uint8_t       maxpacket;
    /* transaction level variables*/
    uint8_t        *xfer_buff;
    uint32_t       dma_addr;
    uint8_t       xfer_len;
    uint8_t       xfer_count;
    /* Transfer level variables*/
    uint8_t       rem_data_len;
    uint8_t       total_data_len;
    uint8_t       ctl_data_len;

}
USB_OTG_EP, *PUSB_OTG_EP;

typedef struct USB_OTG_core_cfg
{
    uint8_t       host_channels;
    uint8_t       dev_endpoints;
    uint8_t       speed;
    uint8_t       dma_enable;
    uint16_t      mps;
    uint16_t      TotalFifoSize;
    uint8_t       phy_itface;
    uint8_t       Sof_output;
    uint8_t       low_power;
    uint8_t       coreID;
}
USB_OTG_CORE_CFGS, *PUSB_OTG_CORE_CFGS;



typedef  struct  usb_setup_req
{

    uint8_t   bmRequest;
    uint8_t   bRequest;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
} USB_SETUP_REQ;

typedef struct _Device_TypeDef
{
    uint8_t  *(*GetDeviceDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetLangIDStrDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetManufacturerStrDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetProductStrDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetSerialStrDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetConfigurationStrDescriptor)( uint8_t speed, uint16_t *length);
    uint8_t  *(*GetInterfaceStrDescriptor)( uint8_t speed, uint16_t *length);

} USBD_DEVICE, *pUSBD_DEVICE;

//typedef struct USB_OTG_hPort
//{
//  void (*Disconnect) (void *phost);
//  void (*Connect) (void *phost);
//  uint8_t ConnStatus;
//  uint8_t DisconnStatus;
//  uint8_t ConnHandled;
//  uint8_t DisconnHandled;
//} USB_OTG_hPort_TypeDef;

typedef struct _Device_cb
{
    uint8_t  (*Init)         (void *pdev, uint8_t cfgidx);
    uint8_t  (*DeInit)       (void *pdev, uint8_t cfgidx);
    /* Control Endpoints*/
    uint8_t  (*Setup)        (void *pdev, USB_SETUP_REQ  *req);
    uint8_t  (*EP0_TxSent)   (void *pdev );
    uint8_t  (*EP0_RxReady)  (void *pdev );
    /* Class Specific Endpoints*/
    uint8_t  (*DataIn)       (void *pdev, uint8_t epnum);
    uint8_t  (*DataOut)      (void *pdev, uint8_t epnum);
    uint8_t  (*SOF)          (void *pdev);
    uint8_t  (*IsoINIncomplete)  (void *pdev);
    uint8_t  (*IsoOUTIncomplete)  (void *pdev);

    uint8_t  *(*GetConfigDescriptor)( uint8_t speed, uint16_t *length);
#ifdef USB_OTG_HS_CORE
    uint8_t  *(*GetOtherConfigDescriptor)( uint8_t speed, uint16_t *length);
#endif

#ifdef USB_SUPPORT_USER_STRING_DESC
    uint8_t  *(*GetUsrStrDescriptor)( uint8_t speed, uint8_t index,  uint16_t *length);
#endif

} USBD_Class_cb_TypeDef;



typedef struct _USBD_USR_PROP
{
    void (*Init)(void);
    void (*DeviceReset)(uint8_t speed);
    void (*DeviceConfigured)(void);
    void (*DeviceSuspended)(void);
    void (*DeviceResumed)(void);

    void (*DeviceConnected)(void);
    void (*DeviceDisconnected)(void);

}
USBD_Usr_cb_TypeDef;

typedef struct _DCD
{
    uint8_t        device_config;                   /* Hold the current USB device configuration */
    uint8_t        device_state;                    /* EP0 State */
    uint8_t        device_status;                   /* Define the connection,configuration and power status */
    uint8_t        device_status_new;
    uint8_t 	   device_SETUP_OUT;
    uint8_t        device_old_status;
    uint8_t        device_address;
    uint8_t        connection_status;
    uint8_t        test_mode;
    uint32_t       DevRemoteWakeup;
    USB_OTG_EP     in_ep   [USB_OTG_MAX_TX_FIFOS];
    USB_OTG_EP     out_ep  [USB_OTG_MAX_TX_FIFOS];
    uint8_t        setup_packet [8 * 3];
    USBD_Class_cb_TypeDef         *class_cb;
    USBD_Usr_cb_TypeDef           *usr_cb;
    USBD_DEVICE                   *usr_device;
    uint8_t        *pConfig_descriptor;
    uint8_t	      zero_replay_flag;
    uint8_t	      last_rxcnt;
    int		rxcnt;
    int 	EP0Setcmd_num1; //use for voice change 0 ->  dac    1 -¡·adc
    int 	EP0Setcmd_num2; //use for voice change 0 ->  dac    1 -¡·adc
    int 	EP0Setcmd2;
    uint8_t       Channel;
}
DCD_DEV, *DCD_PDEV;


typedef struct _HCD
{
    uint8_t                  Rx_Buffer [MAX_DATA_LENGTH];
    volatile uint32_t            ConnSts;
    volatile uint32_t            ErrCnt[USB_OTG_MAX_TX_FIFOS];
    volatile uint32_t            XferCnt[USB_OTG_MAX_TX_FIFOS];
    volatile HC_STATUS           HC_Status[USB_OTG_MAX_TX_FIFOS];
    volatile URB_STATE           URB_State[USB_OTG_MAX_TX_FIFOS];
    USB_OTG_HC               hc [USB_OTG_MAX_TX_FIFOS];
    uint16_t                 channel [USB_OTG_MAX_TX_FIFOS];
    //  USB_OTG_hPort_TypeDef    *port_cb;
}
HCD_DEV, *USB_OTG_USBH_PDEV;


typedef struct _OTG
{
    uint8_t    OTG_State;
    uint8_t    OTG_PrevState;
    uint8_t    OTG_Mode;
}
OTG_DEV, *USB_OTG_USBO_PDEV;

typedef struct USB_OTG_handle
{
    USB_OTG_CORE_CFGS    cfg;
#ifdef USE_DEVICE_MODE
    DCD_DEV     dev;
#endif
#ifdef USE_HOST_MODE
    HCD_DEV     host;
#endif
#ifdef USE_OTG_MODE
    OTG_DEV     otg;
#endif
}
USB_OTG_CORE_HANDLE, *PUSB_OTG_CORE_HANDLE;

/**
  * @}
  */
#define USB_OTG_SPEED_PARAM_HIGH 0
#define USB_OTG_SPEED_PARAM_HIGH_IN_FULL 1
#define USB_OTG_SPEED_PARAM_FULL 3

#define USB_OTG_SPEED_HIGH      0
#define USB_OTG_SPEED_FULL      1

#define USB_OTG_ULPI_PHY      1
#define USB_OTG_EMBEDDED_PHY  2

/**
  * @}
  */


/** @defgroup _GLOBAL_DEFINES_
  * @{
  */
#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0
#define GAHBCFG_GLBINT_ENABLE                  1
#define GAHBCFG_INT_DMA_BURST_SINGLE           0
#define GAHBCFG_INT_DMA_BURST_INCR             1
#define GAHBCFG_INT_DMA_BURST_INCR4            3
#define GAHBCFG_INT_DMA_BURST_INCR8            5
#define GAHBCFG_INT_DMA_BURST_INCR16           7
#define GAHBCFG_DMAENABLE                      1
#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0
#define GRXSTS_PKTSTS_IN                       2
#define GRXSTS_PKTSTS_IN_XFER_COMP             3
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5
#define GRXSTS_PKTSTS_CH_HALTED                7
/**
  * @}
  */


/** @defgroup _OnTheGo_DEFINES_
  * @{
  */
#define MODE_HNP_SRP_CAPABLE                   0
#define MODE_SRP_ONLY_CAPABLE                  1
#define MODE_NO_HNP_SRP_CAPABLE                2
#define MODE_SRP_CAPABLE_DEVICE                3
#define MODE_NO_SRP_CAPABLE_DEVICE             4
#define MODE_SRP_CAPABLE_HOST                  5
#define MODE_NO_SRP_CAPABLE_HOST               6
#define A_HOST                                 1
#define A_SUSPEND                              2
#define A_PERIPHERAL                           3
#define B_PERIPHERAL                           4
#define B_HOST                                 5

#define DEVICE_MODE                            0
#define HOST_MODE                              1
#define OTG_MODE                               2
/**
  * @}
  */


/** @defgroup __DEVICE_DEFINES_
  * @{
  */
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     0
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     1
#define DSTS_ENUMSPD_LS_PHY_6MHZ               2
#define DSTS_ENUMSPD_FS_PHY_48MHZ              3

#define DCFG_FRAME_INTERVAL_80                 0
#define DCFG_FRAME_INTERVAL_85                 1
#define DCFG_FRAME_INTERVAL_90                 2
#define DCFG_FRAME_INTERVAL_95                 3

#define DEP0CTL_MPS_64                         0
#define DEP0CTL_MPS_32                         1
#define DEP0CTL_MPS_16                         2
#define DEP0CTL_MPS_8                          3

#define EP_SPEED_LOW                           0
#define EP_SPEED_FULL                          1
#define EP_SPEED_HIGH                          2

#define EP_TYPE_CTRL                           0
#define EP_TYPE_ISOC                           1
#define EP_TYPE_BULK                           2
#define EP_TYPE_INTR                           3
#define EP_TYPE_MSK                            3

#define STS_GOUT_NAK                           1
#define STS_DATA_UPDT                          2
#define STS_XFER_COMP                          3
#define STS_SETUP_COMP                         4
#define STS_SETUP_UPDT                         6
/**
  * @}
  */


/** @defgroup __HOST_DEFINES_
  * @{
  */
#define HC_PID_DATA0                           0
#define HC_PID_DATA2                           1
#define HC_PID_DATA1                           2
#define HC_PID_SETUP                           3

#define HPRT0_PRTSPD_HIGH_SPEED                1
#define HPRT0_PRTSPD_FULL_SPEED                2
#define HPRT0_PRTSPD_LOW_SPEED                 3
//#define HPRT0_PRTSPD_HIGH_SPEED                0
//#define HPRT0_PRTSPD_FULL_SPEED                1
//#define HPRT0_PRTSPD_LOW_SPEED                 2

#define HCFG_30_60_MHZ                         0
#define HCFG_48_MHZ                            1
#define HCFG_6_MHZ                             2

#define HCCHAR_CTRL                            0
#define HCCHAR_ISOC                            1
#define HCCHAR_BULK                            2
#define HCCHAR_INTR                            3

#define  MIN(a, b)      (((a) < (b)) ? (a) : (b))

/**
  * @}
  */


/** @defgroup USB_DEFINES_Exported_Types
  * @{
  */

typedef enum
{
    USB_OTG_HS_CORE_ID = 0,
    USB_OTG_FS_CORE_ID = 1
} USB_OTG_CORE_ID_TypeDef;
/**
  * @}
  */


/** @defgroup USB_DEFINES_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_DEFINES_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_DEFINES_Exported_FunctionsPrototype
  * @{
  */
/**
  * @}
  */


/** @defgroup Internal_Macro's
  * @{
  */
//#define HREAD(reg)         ( *(volatile uint8_t *)  reg)
//#define HREADW(reg)         (*(volatile uint16_t *) reg)
//#define USB_OTG_READ_REG32(reg)         (*(volatile uint32_t *) reg)
//#define HWRITE(reg,value)   (*(volatile uint8_t *)reg = value)
//#define HWRITEW(reg,value)  (*(volatile uint16_t *) reg = value)
//#define USB_OTG_WRITE_REG32(reg,value)  (*(volatile uint32_t *) reg = value)

//#define USB_OTG_MODIFY_REG8(reg, clear_mask, set_mask)  \
//        HWRITE(reg, (((HREAD(reg)) & ~clear_mask) | set_mask))

//#define USB_OTG_MODIFY_REG16(reg, clear_mask, set_mask)  \
//        HWRITEW(reg, (((HREADW(reg)) & ~clear_mask) | set_mask))

//#define USB_OTG_MODIFY_REG32(reg, clear_mask, set_mask)  \
//        USB_OTG_WRITE_REG32(reg, (((USB_OTG_READ_REG32(reg)) & ~clear_mask) | set_mask))



#define USB_OTG_MODIFY_REG8(reg, clear_mask, set_mask)  \
        HWRITE(reg, (((HREAD(reg)) & ~clear_mask) | set_mask))

#define USB_OTG_MODIFY_REG16(reg, clear_mask, set_mask)  \
        HWRITEW(reg, (((HREADW(reg)) & ~clear_mask) | set_mask))
/********************************************************************************
                              ENUMERATION TYPE
********************************************************************************/
enum USB_OTG_SPEED
{
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH
};





/** @defgroup USB_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_FunctionsPrototype
  * @{
  */
USB_OTG_STS  USB_OTG_TRIG(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
USB_OTG_STS USB_OTG_EPReply_Zerolen(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);

USB_OTG_STS  USB_OTG_CoreInit        (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_SelectCore      (USB_OTG_CORE_HANDLE *pdev,
                                      USB_OTG_CORE_ID_TypeDef coreID);
USB_OTG_STS  USB_OTG_EnableGlobalInt (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_ReadPacket(USB_OTG_CORE_HANDLE *pdev,
                        uint8_t *dest,
                        uint8_t ch_ep_num,
                        uint16_t len);
void USB_RX_CONTINUE(USB_OTG_CORE_HANDLE *pdev);

USB_OTG_STS  USB_OTG_WritePacket     (USB_OTG_CORE_HANDLE *pdev,
                                      uint8_t *src,
                                      uint8_t ch_ep_num,
                                      uint16_t len);
USB_OTG_STS  USB_OTG_FlushTxFifo     (USB_OTG_CORE_HANDLE *pdev, uint32_t num);
USB_OTG_STS  USB_OTG_FlushRxFifo     (USB_OTG_CORE_HANDLE *pdev);

uint8_t      USB_OTG_ReadCoreItr     (USB_OTG_CORE_HANDLE *pdev);
uint8_t      USB_OTG_ReadOtgItr      (USB_OTG_CORE_HANDLE *pdev);
uint8_t      USB_OTG_IsHostMode      (USB_OTG_CORE_HANDLE *pdev);
uint8_t      USB_OTG_IsDeviceMode    (USB_OTG_CORE_HANDLE *pdev);
uint8_t      USB_OTG_GetMode         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_PhyInit         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_SetCurrentMode  (USB_OTG_CORE_HANDLE *pdev,
                                      uint8_t mode);

/*********************** HOST APIs ********************************************/
#ifdef USE_HOST_MODE
USB_OTG_STS  USB_OTG_CoreInitHost    (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableHostInt   (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_HC_Init         (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_Halt         (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_StartXfer    (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_DoPing       (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
uint8_t      USB_OTG_ReadHostAllChannels_intr    (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ResetPort       (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ReadHPRT0       (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_DriveVbus       (USB_OTG_CORE_HANDLE *pdev, uint8_t state);
void         USB_OTG_InitFSLSPClkSel (USB_OTG_CORE_HANDLE *pdev, uint8_t freq);
uint8_t      USB_OTG_IsEvenFrame     (USB_OTG_CORE_HANDLE *pdev) ;
void         USB_OTG_StopHost        (USB_OTG_CORE_HANDLE *pdev);
#endif
/********************* DEVICE APIs ********************************************/
#ifdef USE_DEVICE_MODE
USB_OTG_STS  USB_OTG_CoreInitDev         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableDevInt        (USB_OTG_CORE_HANDLE *pdev);
enum USB_OTG_SPEED USB_OTG_GetDeviceSpeed (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EP0Activate (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EPActivate  (USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPStartXfer (USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPSetStall          (USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPClearStall        (USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
uint16_t     USB_OTG_ReadDevAllOutEp_itr (USB_OTG_CORE_HANDLE *pdev);
uint16_t     USB_OTG_ReadDevOutEP_itr    (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint16_t     USB_OTG_ReadDevAllInEPItr   (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_InitDevSpeed        (USB_OTG_CORE_HANDLE *pdev, uint8_t speed);
uint8_t      USBH_IsEvenFrame (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_UngateClock(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_StopDevice(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep, uint32_t Status);
uint32_t     USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep);
void usb_clear_nak(byte ep, byte out);
void usb0_clear_out_nak();
void usb0_clear_in_nak();
#endif


/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#ifdef __cplusplus
}
#endif

#endif	/* __USB_CORE_H__ */

/************************ (C) COPYRIGHT 2014 Megahuntmicro ****END OF FILE****/
