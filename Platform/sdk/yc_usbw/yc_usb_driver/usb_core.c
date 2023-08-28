/*
 * Copyright (c) 2006-2021, YICHIP Development Team
 * @file     usb_core.c
 * @brief    source file for setting xxx
 *
 * Change Logs:
 * Date           Author      Version        Notes
 * 2021-03-29     chenqixiong V1.0.0         the first version
 */

/* Include ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usb_dcd_init.h"
#include "yc_debug.h"
#include "usbd_conf.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Ptivate function prototypes ----------------------------------------------*/

/*******************************************************************************
* @brief  USB_OTG_WritePacket : Writes a packet into the Tx FIFO associated
*         with the EP
* @param  pdev : Selected device
* @param  src : source pointer
* @param  ch_ep_num : end point number
* @param  bytes : No. of bytes
* @retval USB_OTG_STS : status
*/


extern uint8_t   usb_rxbuf [USB_RX_BUF_LEN] ;
extern uint8_t   usb_txbuf0[USB_TX0BUF_LEN] ;
extern uint8_t   usb_txbuf1[USB_TX1BUF_LEN] ;
extern uint8_t   usb_txbuf2[USB_TX2BUF_LEN] ;
extern uint8_t   usb_txbuf3[USB_TX3BUF_LEN] ;
extern byte *usb_rxptr;

/**
 * @method usb_getbyte
 * @brief  None
 * @param  None
 * @retval None
 */
byte usb_getbyte()
{
    byte t;

    if (usb_rxptr == usb_rxbuf + sizeof(usb_rxbuf))
    {
        //MyPrintf("test1 = %p", usb_rxptr);
        usb_rxptr = usb_rxbuf;
    }
    t = *usb_rxptr++;

    return t;
}


/**
 * @method usb_getword
 * @brief  None
 * @param  None
 * @retval None
 */
word usb_getword()
{
    return usb_getbyte() | (int) usb_getbyte() << 8;
}


void putbuf(uint8_t *buf, uint8_t *src, int len)
{
    int len0 = len - 1;

    buf[0] = len0, buf[1] = len0 >> 8;
    memcpy(buf + 2, src, len);
}

extern uint8_t test;
USB_OTG_STS USB_OTG_WritePacket(USB_OTG_CORE_HANDLE *pdev,
                                uint8_t *src,
                                uint8_t ch_ep_num, uint16_t len)
{
    USB_OTG_IRQ_TypeDef gintr_status;
    USB_OTG_STS status = USB_OTG_OK;
    uint16_t i = 0;

    if (ch_ep_num == 0)
    {
        if (HREAD(core_usb_txbusy) & 0x01)
            return 0;
        putbuf(usb_txbuf0, src, len);
        HWRITEW(core_usb_tx_saddr0, &usb_txbuf0);
    }
    else if (ch_ep_num == 1)
    {
        if (HREAD(core_usb_txbusy)  & 0x02)
            return 0;
        putbuf(usb_txbuf1, src, len);
        HWRITEW(core_usb_tx_saddr1, &usb_txbuf1);
    }
    else if (ch_ep_num == 2)
    {
        if (HREAD(core_usb_txbusy) & 0x04)
            return 0;
        putbuf(usb_txbuf2, src, len);
        HWRITEW(core_usb_tx_saddr2, &usb_txbuf2);
    }
    else
    {
        if (HREAD(core_usb_txbusy)  & 0x08)
            return 0;
        memcpy(usb_txbuf3 + 1, src, len);
        HWRITEW(core_usb_tx_saddr3, &usb_txbuf3);
    }
    HWRITE(core_usb_trig, 1 << ch_ep_num);
    return status;
}

extern byte *usb_rxptr;
extern uint16_t rx_DataLength;

/**
* @brief  USB_OTG_ReadPacket : Reads a packet from the Rx FIFO
* @param  pdev : Selected device
* @param  dest : Destination Pointer
* @param  bytes : No. of bytes
* @retval None
*/
void USB_OTG_ReadPacket(USB_OTG_CORE_HANDLE *pdev,
                        uint8_t *dest, uint8_t ch_ep_num, uint16_t len)
{
    uint16_t i = 0;
    uint16_t count8b = len;
    uint8_t *data_buff = (uint8_t *) dest;
    for (i = 0; i < count8b; i++, data_buff++)
    {
        *data_buff = usb_getbyte(ch_ep_num);
        //DEBUG_LOG_STRING("data count: %d  size:%d \r\n",i, count8b);
    }
    rx_DataLength = 0;
    if(len & 0x01)
    {
        usb_rxptr++;
        HWRITEW(core_usb_rxptr, usb_rxptr);
    }
    else
    {
        HWRITEW(core_usb_rxptr, usb_rxptr);
    }
}

extern uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev,
                                    uint8_t ep_intr);


/**
* @brief  USB_OTG_SelectCore
*         Initialize core registers address.
* @param  pdev : Selected device
* @param  coreID : USB OTG Core ID
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_SelectCore(USB_OTG_CORE_HANDLE *pdev,
                               USB_OTG_CORE_ID_TypeDef coreID)
{
    uint32_t i, baseAddress = 0;
    USB_OTG_STS status = USB_OTG_OK;

#ifdef  USB_OTG_DMA_ENABLE
    pdev->cfg.dma_enable = 1;
#else
    pdev->cfg.dma_enable = 0;
#endif

    /* at startup the core is in FS mode */
    pdev->cfg.speed = USB_OTG_SPEED_FULL;
    pdev->cfg.mps = USB_OTG_FS_MAX_PACKET_SIZE;


    /* initialize device cfg following its address */
    if (coreID == USB_OTG_FS_CORE_ID)
    {
        pdev->cfg.coreID = USB_OTG_FS_CORE_ID;
        pdev->cfg.dev_endpoints = 4;
        pdev->cfg.TotalFifoSize = 64;   /* in 8-bits */
        pdev->cfg.phy_itface = USB_OTG_EMBEDDED_PHY;

#ifdef USB_OTG_FS_SOF_OUTPUT_ENABLED
        pdev->cfg.Sof_output = 1;
#endif

#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
        pdev->cfg.low_power = 1;
#endif
    }
    else if (coreID == USB_OTG_HS_CORE_ID)
    {
        pdev->cfg.coreID = USB_OTG_HS_CORE_ID;
        pdev->cfg.host_channels = 8;
        pdev->cfg.dev_endpoints = 4;
        pdev->cfg.TotalFifoSize = 512;  /* in 8-bits */

#ifdef USB_OTG_ULPI_PHY_ENABLED
        pdev->cfg.phy_itface = USB_OTG_ULPI_PHY;
#else
# ifdef USB_OTG_EMBEDDED_PHY_ENABLED
        pdev->cfg.phy_itface = USB_OTG_EMBEDDED_PHY;
# endif
#endif

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        pdev->cfg.dma_enable = 1;
#endif

#ifdef USB_OTG_HS_SOF_OUTPUT_ENABLED
        pdev->cfg.Sof_output = 1;
#endif

#ifdef USB_OTG_HS_LOW_PWR_MGMT_SUPPORT
        pdev->cfg.low_power = 1;
#endif
    }

    return status;
}


/**
* @brief  USB_OTG_CoreInit
*         Initializes the USB_OTG controller registers and prepares the core
*         device mode or host mode operation.
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_CoreInit(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    return status;
}



/**
* @brief  USB_OTG_SetCurrentMode : Set ID line
* @param  pdev : Selected device
* @param  mode :  (Host/device)only device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_SetCurrentMode(USB_OTG_CORE_HANDLE *pdev, uint8_t mode)
{
    USB_OTG_STS status = USB_OTG_OK;
    if (mode == HOST_MODE)
    {
    }
    else if (mode == DEVICE_MODE)
    {
#ifdef USE_DEVICE_MODE
        pdev->dev.out_ep[0].xfer_buff = pdev->dev.setup_packet;
        pdev->dev.out_ep[0].xfer_len = 8;
#endif
    }
    return status;
}

/**
* @brief  USB_OTG_EPActivate : Activates an EP
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPActivate(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_IRQ_MASK1_TypeDef intr_rxtxe;

    /* Read DEPCTLn register */
    if (ep->is_in == 1)
    {
        intr_rxtxe.d8 = 1 << ep->num;
    }
    else
    {
        intr_rxtxe.d8 = 1 << ep->num;
    }
    return status;
}


/**
* @brief  USB_OTG_EPDeactivate : Deactivates an EP
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_IRQ_MASK1_TypeDef intr_rxtxe;

    /* Read DEPCTLn register */
    if (ep->is_in == 1)
    {
        intr_rxtxe.d8 = 1 << ep->num;
    }
    else
    {
        intr_rxtxe.d8 = 1 << ep->num;
    }
    return status;
}



/**
* @brief  USB_OTG_GetMode : Get current mode
* @param  pdev : Selected device
* @retval current mode
*/
uint8_t USB_OTG_GetMode(USB_OTG_CORE_HANDLE *pdev)
{
    return DEVICE_MODE;
}


/**
* @brief  USB_OTG_IsDeviceMode : Check if it is device mode
* @param  pdev : Selected device
* @retval num_in_ep
*/
uint8_t USB_OTG_IsDeviceMode(USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_GetMode(pdev) != HOST_MODE);
}

/**
* @brief  USB_OTG_EPStartXfer : Handle the setup for data xfer for an EP and
*         starts the xfer
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/

extern uint16_t rx_DataLength;
USB_OTG_STS USB_OTG_EPStartXfer(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    uint16_t rx_count;

    /* IN endpoint */
    if (ep->is_in == 1)
    {
        if ((pdev->cfg.dma_enable == 0)
                || ((USB_OTG_DEV_DMA_EP_NUM & 0x07) != ep->num))
        {
            ep->rem_data_len = ep->xfer_len - ep->xfer_count;
            if (ep->rem_data_len == ep->maxpacket)
            {
                USB_OTG_WritePacket(pdev,
                                    ep->xfer_buff + ep->xfer_count,
                                    ep->num, ep->maxpacket);
                ep->xfer_count += ep->maxpacket;
                ep->xfer_count = ep->xfer_len;
                ep->rem_data_len = 0;
                pdev->dev.zero_replay_flag = 1 << ep->num;
                usb_clear_nak(ep->num, 0);

            }
            /* Zero Length Packet? */
            else if (ep->rem_data_len == 0)
            {
                USB_OTG_WritePacket(pdev,
                                    ep->xfer_buff + ep->xfer_count, ep->num, 0);
                ep->xfer_count = ep->xfer_len;
                ep->rem_data_len = 0;
            }
            else
            {
                if (ep->rem_data_len > ep->maxpacket)
                {
                    USB_OTG_WritePacket(pdev,
                                        ep->xfer_buff + ep->xfer_count,
                                        ep->num, ep->maxpacket);
                    ep->xfer_count += ep->maxpacket;
                    if (ep->xfer_len >= ep->xfer_count)
                    {
                        ep->rem_data_len = ep->xfer_len - ep->xfer_count;
                    }
                    else
                    {
                        ep->rem_data_len = 0;
                        ep->xfer_count = ep->xfer_len;
                    }
                }
                else
                {
                    USB_OTG_WritePacket(pdev,
                                        ep->xfer_buff + ep->xfer_count,
                                        ep->num, ep->rem_data_len);
                    ep->xfer_count = ep->xfer_len;
                    ep->rem_data_len = 0;
                }
            }
        }
    }
    else
    {

        /* OUT endpoint */
        rx_count = rx_DataLength;

        USB_OTG_ReadPacket(pdev, ep->xfer_buff + ep->xfer_count, ep->num,
                           rx_count);
        ep->xfer_count += rx_count;
        if (ep->xfer_len <= ep->xfer_count)
        {
            ep->rem_data_len = ep->xfer_count - ep->xfer_len;
        }
        else
        {
            ep->rem_data_len = 0;
            ep->xfer_len = ep->xfer_count;
        }
    }
    return status;
}

uint8_t clear_ep0_out_nak;
/**
* @brief  USB_OTG_EP0StartXfer : Handle the setup for a data xfer for EP0 and
*         starts the xfer
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;

    uint32_t fifoemptymsk = 0;

    /* IN endpoint */
    if (ep->is_in == 1)
    {
        ep->rem_data_len = ep->xfer_len - ep->xfer_count;
        /* Zero Length Packet? */
        if (ep->rem_data_len == 0)
        {
            //  send zero packet
            //USB_OTG_EPReply_Zerolen(pdev, ep);
            usb0_clear_in_nak();
            ep->xfer_count = ep->xfer_len;
            ep->rem_data_len = 0;
        }
        else
        {
            if (ep->rem_data_len > ep->maxpacket)
            {
                USB_OTG_WritePacket(pdev,
                                    ep->xfer_buff + ep->xfer_count,
                                    0, ep->maxpacket);
                ep->xfer_count += ep->maxpacket;
                ep->rem_data_len = ep->xfer_len - ep->xfer_count;
            }
            else if(ep->rem_data_len == ep->maxpacket)
            {
                USB_OTG_WritePacket(pdev,
                                    ep->xfer_buff + ep->xfer_count,
                                    0, ep->maxpacket);
                ep->xfer_count = ep->xfer_len;
                ep->rem_data_len = 0;
                /****prepare receive outpacket *****/
                usb0_clear_in_nak();

            }

            else
            {
                USB_OTG_WritePacket(pdev,
                                    ep->xfer_buff + ep->xfer_count,
                                    0, ep->rem_data_len);
                ep->xfer_count = ep->xfer_len;
                ep->rem_data_len = 0;
                /****prepare receive outpacket *****/
                usb0_clear_out_nak();
                clear_ep0_out_nak = 1;

            }
        }
        if (pdev->cfg.dma_enable == 0)
        {
            /* Enable the Tx FIFO Empty Interrupt for this EP */
            if (ep->xfer_len > 0)
            {
            }
        }
    }
    else
    {
        /* OUT endpoint */
    }
    return status;
}

/**
* @brief  USB_OTG_TRIG : Handle start xfer and set tx trig
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
void usb_clear_nak(byte ep, byte out)
{
    HWRITE(core_usb_clear_nak, 1 << (ep << 1 | out & 1));
}

void usb0_clear_out_nak()
{
    usb_clear_nak(0, 1);
}

void usb0_clear_in_nak()
{
    usb_clear_nak(0, 0);
}

USB_OTG_STS USB_OTG_TRIG(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep)
{
    int i;
    USB_OTG_STS status = USB_OTG_OK;
    return status;
}

/**
* @brief  USB_OTG_EPSetStall : Set the EP STALL
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
USB_OTG_STS USB_OTG_EPSetStall(USB_OTG_CORE_HANDLE *pdev, USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_stall_TypeDef stcsrl;

    if (ep->num == 0)
        HWRITE(core_usb_stall, 0x01);
    //USB_OTG_EPReply_Zerolen(pdev, ep);
    return status;
}

/**
* @brief  USB_OTG_EPSetStall : ack zero  length packet
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/

USB_OTG_STS USB_OTG_EPReply_Zerolen(USB_OTG_CORE_HANDLE *pdev,
                                    USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    HWRITE(core_usb_trig, 0x10 << ep->num);
    return status;
}

/**
* @brief  USB_OTG_RemoteWakeup : active remote wakeup signalling
* @param  None
* @retval : None
*/
void USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_CTL_TypeDef power;

    /* Note: If CLK has been stopped,it will need be restarted before
     * this write can occur.
     */
    power.d8 = HREAD(core_usb_config);
    power.b.resume = 1;
    power.b.wakeup_enable = 1;
    HWRITE(core_usb_config, power.d8);
    //  USB_CONFIG = power.d8;
    /* The software should leave then this bit set for approximately 10ms
     * (minimum of 2ms, a maximum of 15ms) before resetting it to 0.
     */
    power.b.resume = 0;
    power.b.wakeup_enable = 0;
    // USB_CONFIG = power.d8;
    HWRITE(core_usb_config, power.d8);
}


/************************ (C) COPYRIGHT Yichip Microelectronics *****END OF FILE****/
