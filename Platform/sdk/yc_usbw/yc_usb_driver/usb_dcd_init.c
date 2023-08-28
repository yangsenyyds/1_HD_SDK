/**
  ******************************************************************************
  * @file    usbd_dcd_init.c
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
#include "usb_dcd_init.h"

#ifdef HID_AUDIO_DEMO
#include "reg_addr.h"
#include "app_config.h"
#include "yc_debug.h"
#endif
/** @addtogroup USB_OTG_DRIVER
* @{
*/

/** @defgroup USB_DCD_INT
* @brief This file contains the interrupt subroutines for the Device mode.
* @{
*/

/** @defgroup USB_DCD_INT_Private_Defines
* @{
*/
/**
* @}
*/

/* Interrupt Handlers */
static uint32_t DCD_HandleInEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint16_t ep_intr);
uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint8_t ep_intr);
uint32_t DCD_HandleUsbReset_ISR();
void _delay_(uint32_t t)
{
    ((void (*)())(0xc6 + 1))(t);
}

/**
* @brief  USBD_OTG_ISR_Handler
*         handles all USB Interrupts
* @param  pdev: device instance
* @retval status
*/
volatile uint8_t *usb_rxptr;
uint8_t setup_cnt = 0;
uint8_t SetAddress_Flag = 0;
uint8_t Address_Value = 0;
volatile uint16_t rx_DataLength = 0;
#ifdef HID_AUDIO_DEMO
uint8_t AudioIn_Data_Buf[100], USBD_OTG_ISR_Handler_CNT;
#endif
/**
* @brief  USBD_OTG_ISR_Handler
*         handles all USB Interrupts
* @param  pdev: device instance
* @retval status
*/
extern USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb;
extern uint8_t  AudioCtl[64];
extern uint8_t  AudioCtlCmd;
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint8_t clear_ep0_out_nak;
extern AUDIO_INFO gaudio;


/**
* @brief  USBD_OTG_ISR_Handler
*         handles all USB Interrupts
* @param  pdev: device instance
* @retval status
*/
uint32_t USBD_OTG_ISR_Handler(USB_OTG_CORE_HANDLE *pdev)
{
    USBD_OTG_ISR_Handler_CNT ++;
    if(USBD_OTG_ISR_Handler_CNT > 100000)
    {
        USBD_OTG_ISR_Handler_CNT = 0;
    }
    USB_OTG_IRQ_TypeDef gintr_status;
    USB_OTG_trig_TypeDef trig;
    uint32_t retval = 0, rxdmaw, rxdmar;
    uint16_t rx_ep, rx_ep1, test_len;
    uint8_t ep_num, i, rxcnt;
    uint8_t j[100];

    rx_DataLength = 0;
    if (USB_OTG_IsDeviceMode(pdev))     /* ensure that we are in device mode */
    {
        gintr_status.d16 = HREADW(core_usb_status);  /* get USB_STATUS value */
        if (!gintr_status.d16)
            return 0;
        HWRITEW(core_usb_status, gintr_status.d16); /* clear USB_STATUS value */
        if (gintr_status.b.reset)
        {
            retval |= DCD_HandleUsbReset_ISR(pdev);
        }
        if(gintr_status.b.clkcal)
        {
            usb_clkcal_done();
        }
        if (gintr_status.b.rx_full)
        {
#ifdef HID_AUDIO_DEMO
            DEBUG_LOG_STRING("gintr_status.b.rx_full = \r\n");
#endif
        }
        /*every packet into rx_dma,USB_RXCNT +1 */
        for (i = 0, rxcnt = HREAD(core_usb_rx_cnt); gintr_status.b.rx_ready; i++)
        {
            //DEBUG_LOG_STRING("rxcnt = 0x%x, last_rxcnt: 0x%x, i = %d\r\n",rxcnt, pdev->dev.last_rxcnt, i);
            if (rxcnt == (i + pdev->dev.last_rxcnt & 0xff))
            {
                pdev->dev.rxcnt = rxcnt;
                pdev->dev.last_rxcnt += i;
                break;
            }
#ifndef USB_USER_MRAM
            usb_rxptr = (byte *)(HREADW(core_usb_rxptr) | 0x10000000);
#else
            usb_rxptr = (byte *)(HREADW(core_usb_rxptr) | 0x10010000);
#endif

            rx_ep = usb_getword();          /*16bit = 4bit ep_num + 12bit data_len*/
            rx_DataLength = rx_ep & 0xfff;
            ep_num = (uint8_t)(rx_ep >> 12);
            if (ep_num == 0)
            {
                if (SetAddress_Flag)
                {
                    SetAddress_Flag = 0;
                }
                if(pdev->dev.device_SETUP_OUT == 1)
                {
                    usb0_control_data(pdev, rx_DataLength);
                    pdev->dev.device_SETUP_OUT = 0;
                }
                if(rx_DataLength == 8)
                {
                    if ((rx_DataLength < pdev->dev.out_ep[0].maxpacket) ||
                            (pdev->dev.out_ep[0].xfer_count + rx_DataLength ==
                             pdev->dev.out_ep[0].xfer_len))
                    {
                        pdev->dev.out_ep[0].xfer_buff = pdev->dev.setup_packet;
                        pdev->dev.out_ep[0].xfer_count = 0;
                        pdev->dev.out_ep[0].xfer_len = 8;
                    }
                    USB_OTG_ReadPacket(pdev,
                                       pdev->dev.out_ep[0].xfer_buff +
                                       pdev->dev.out_ep[0].xfer_count, 0,
                                       rx_DataLength);
                    USBD_DCD_INT_fops->SetupStage(pdev);
                }
                else
                {
                    usb0_control_data(pdev, rx_DataLength);
                    //usb0_clear_in_nak();
                }
            }
            else
            {
                retval |= DCD_HandleOutEP_ISR(pdev, ep_num);
            }
        }
        if (gintr_status.b.tx_done0)
        {
            retval |= DCD_HandleInEP_ISR(pdev, USB_EPM0);
        }
        if (gintr_status.b.tx_done1)
        {
            retval |= DCD_HandleInEP_ISR(pdev, USB_EPM1);
        }
        if (gintr_status.b.tx_done2)
        {
            retval |= DCD_HandleInEP_ISR(pdev, USB_EPM2);
        }
        if (gintr_status.b.tx_done3)
        {
            retval |= DCD_HandleInEP_ISR(pdev, USB_EPM3);
        }
    }

    return retval;
}
/**
* @brief  DCD_HandleInEP_ISR
*         Indicates that an IN EP has a pending Interrupt
* @param  pdev: device instance
* @retval status
*/
static uint32_t DCD_HandleInEP_ISR(USB_OTG_CORE_HANDLE *pdev,
                                   uint16_t ep_intr)
{
    USB_OTG_EP *ep;
    uint16_t epnum = ep_intr;
    int i = 0;
    //DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"Bt_Reset: 0x%04X", 0xe00f, ep_intr);
    ep = &pdev->dev.in_ep[epnum];

    /* Setup and start the Transfer */
    ep->is_in = 1;
    ep->num = epnum;
    if (pdev->dev.in_ep[epnum].rem_data_len == 0)
    {
        if (epnum == (HID_IN_EP & 0x7f))
        {
            //                    if ((pdev->dev.zero_replay_flag) & 1 << ep->num)
            //                    {
            //   USB_OTG_EPReply_Zerolen(pdev, ep);
            //  pdev->dev.zero_replay_flag = 0;
            //                    }
        }
#ifdef HID_AUDIO_DEMO
        else if(epnum == (AUDIO_IN_EP & 0x7f))
        {
            AUDIO_INFO *prxDacFifo = &gaudio;
            if (HREAD(core_usb_txbusy) & 0x04)
            {
                DEBUG_LOG(LOG_LEVEL_CORE, "UI", "Bt_Reset: 0x%04X", 0xe031, HREAD(core_usb_txbusy));
                return 1;
            }
#if 0
            uint16_t bufferSize = ADCGetSize();
            if(prxDacFifo->gAudioAdcState == AUDIO_ADC_CONTINUE)
            {
                if(prxDacFifo ->gAdcflag == 1) // Cache buffer
                {
                    if(bufferSize < (AUDIO_ADC_48K_LEN * 2))
                    {
                        USB_OTG_WritePacket(pdev, AudioIn_Data_Buf, 0x02, AUDIO_ADC_48K_TXLEN);
                        return 1;
                    }
                }
                prxDacFifo ->gAdcflag = 0;
                if(bufferSize > AUDIO_ADC_48K_TXLEN)
                {
                    USB_OTG_WritePacket(pdev, (uint8_t *) & (prxDacFifo->gUSBadcbuf[prxDacFifo->gUSBadcRptr]), 0x02, AUDIO_ADC_48K_TXLEN);
                    gaudio.gUSBadcRptr += AUDIO_ADC_48K_TXLEN;
                    if(gaudio.gUSBadcRptr >= AUDIO_ADC_BUF_48KSIZE)
                    {
                        gaudio.gUSBadcRptr = 0;
                    }
                }
                else
                {
                    USB_OTG_WritePacket(pdev, AudioIn_Data_Buf, 0x02, AUDIO_ADC_48K_TXLEN);
                }
            }
#else
            if(prxDacFifo->gAudioAdcState == AUDIO_ADC_CONTINUE)
            {
                if(prxDacFifo ->gAdcflag == 1)
                {
                    if(ADCGetSize() < (AUDIO_ADC_48K_LEN * 3))
                    {

                        USB_OTG_WritePacket(pdev, AudioIn_Data_Buf, 0x02, AUDIO_ADC_48K_LEN);
                    }
                    else
                    {
                        USB_OTG_WritePacket(pdev, (uint8_t *) & (prxDacFifo->gUSBadcbuf[prxDacFifo->gUSBadcRptr]), 0x02, AUDIO_ADC_48K_LEN);
                        gaudio.gUSBadcRptr += AUDIO_ADC_48K_LEN;
                        prxDacFifo ->gAdcflag = 0;
                    }

                }
                else
                {

                    if(prxDacFifo ->gUSBadcWptr >= prxDacFifo ->gUSBadcRptr)
                    {
                        if(prxDacFifo->gUSBadcWptr - prxDacFifo->gUSBadcRptr > AUDIO_ADC_48K_LEN)
                        {
                            USB_OTG_WritePacket(pdev, (uint8_t *) & (prxDacFifo->gUSBadcbuf[prxDacFifo->gUSBadcRptr]), 0x02, AUDIO_ADC_48K_LEN);
                            gaudio.gUSBadcRptr += AUDIO_ADC_48K_LEN;
                            if(gaudio.gUSBadcRptr >= AUDIO_ADC_BUF_48KSIZE)
                            {
                                gaudio.gUSBadcRptr = 0;
                            }
                            gaudio.gAdcflag = 0;
                        }
                        else
                        {
                            USB_OTG_WritePacket(pdev, AudioIn_Data_Buf, 0x02, AUDIO_ADC_48K_LEN);

                        }

                    }
                    else
                    {
                        if ((AUDIO_ADC_BUF_48KSIZE - (prxDacFifo->gUSBadcRptr - prxDacFifo->gUSBadcWptr)) > AUDIO_ADC_48K_LEN)
                        {
                            USB_OTG_WritePacket(pdev, (uint8_t *) & (prxDacFifo->gUSBadcbuf[prxDacFifo->gUSBadcRptr]), 0x02, AUDIO_ADC_48K_LEN);
                            gaudio.gUSBadcRptr += AUDIO_ADC_48K_LEN;
                            if(gaudio.gUSBadcRptr >= AUDIO_ADC_BUF_48KSIZE)
                            {
                                gaudio.gUSBadcRptr = 0;
                            }

                        }
                        else
                        {
                            USB_OTG_WritePacket(pdev, AudioIn_Data_Buf, 0x02, AUDIO_ADC_48K_LEN);
                        }


                    }
                }
            }
#endif


            return 1;
        }
#endif
    }
    else
    {
        if (pdev->dev.in_ep[epnum].xfer_len -
                pdev->dev.in_ep[epnum].xfer_count ==
                pdev->dev.in_ep[epnum].maxpacket)
        {
            USB_OTG_WritePacket(pdev,
                                pdev->dev.in_ep[epnum].xfer_buff +
                                pdev->dev.in_ep[epnum].xfer_count, epnum,
                                pdev->dev.in_ep[epnum].maxpacket);

            pdev->dev.in_ep[epnum].xfer_count = pdev->dev.in_ep[epnum].xfer_len;
            pdev->dev.in_ep[epnum].rem_data_len = 0;
            pdev->dev.zero_replay_flag = 1 << ep->num;
            usb_clear_nak(epnum, 0);
        }

        else if (pdev->dev.in_ep[epnum].xfer_len -
                 pdev->dev.in_ep[epnum].xfer_count >
                 pdev->dev.in_ep[epnum].maxpacket)
        {
            USB_OTG_WritePacket(pdev,
                                pdev->dev.in_ep[epnum].xfer_buff +
                                pdev->dev.in_ep[epnum].xfer_count, epnum,
                                pdev->dev.in_ep[epnum].maxpacket);

            pdev->dev.in_ep[epnum].xfer_count +=
                pdev->dev.in_ep[epnum].maxpacket;
            pdev->dev.in_ep[epnum].rem_data_len =
                pdev->dev.in_ep[epnum].xfer_len -
                pdev->dev.in_ep[epnum].xfer_count;
        }

        else
        {
            USB_OTG_WritePacket(pdev,
                                pdev->dev.in_ep[epnum].xfer_buff +
                                pdev->dev.in_ep[epnum].xfer_count, epnum,
                                pdev->dev.in_ep[epnum].xfer_len -
                                pdev->dev.in_ep[epnum].xfer_count);

            pdev->dev.in_ep[epnum].xfer_count = pdev->dev.in_ep[epnum].xfer_len;
            pdev->dev.in_ep[epnum].rem_data_len = 0;
            /****prepare receive outpacket *****/
            usb0_clear_out_nak();
            //HWRITE(core_usb_trig,0x10 << 0);
            /* TX COMPLETE */
            USBD_DCD_INT_fops->DataInStage(pdev, epnum);
            pdev->dev.zero_replay_flag = 0;
        }
    }
    return 1;
}

uint8_t gIsFirstBufferCache = 10;

#ifdef USB_SOFT_DMA_WORK
extern volatile uint8_t   usb_rxbuf[800] ;
extern volatile uint8_t   usb_txbuf0[100] ;
extern volatile uint8_t   usb_txbuf1[100] ;
extern volatile uint8_t   usb_txbuf2[100] ;
extern volatile uint8_t   usb_txbuf3[100];

void usbcpy_wrap(void *dst, int len, void *dst_start, int dst_len)
{
    dmacpy_wrap(dst, usb_rxptr, len, dst_start, dst_len, usb_rxbuf, sizeof (usb_rxbuf));
}

void usbcpy(void *dst, int len)
{
    dmacpy_wrap(dst, usb_rxptr, len, 0, -1, usb_rxbuf, sizeof (usb_rxbuf));
}
#define INCPTR(p, inc, buf)		if((p += inc) >= buf + sizeof(buf)) p -= sizeof(buf)
#define INC_RPTR(x)				INCPTR(usb_rxptr, x, usb_rxbuf)
#endif
/**
* @brief  DCD_HandleOutEP_ISR
*         Indicates that an OUT EP has a pending Interrupt
* @param  pdev: device instance
* @retval status
*/
uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev, uint8_t ep_intr)
{
    volatile AUDIO_INFO *prxDacFifo = &gaudio;
    uint16_t len = rx_DataLength;
    if(ep_intr == HID_OUT_EP)
    {
        if (rx_DataLength)
        {
            USBD_DCD_INT_fops->DataOutStage(pdev, ep_intr);
        }
    }
    else if(ep_intr == AUDIO_OUT_EP)
    {

#ifdef USB_SOFT_DMA_WORK //?
        usbcpy_wrap(prxDacFifo->gdacRptr, rx_DataLength
                    , prxDacFifo->gAudiodacbuf, sizeof(prxDacFifo->gAudiodacbuf));
        INC_RPTR(rx_DataLength);
        //HWRITEW(core_usb_rxptr, (int)usb_rxptr);
        if(rx_DataLength & 0x01)
        {
            usb_rxptr++;
            HWRITEW(core_usb_rxptr, usb_rxptr);
        }
        else
        {
            HWRITEW(core_usb_rxptr, usb_rxptr);
        }
        rx_DataLength = 0;
#else
        if( prxDacFifo->gdacRptr >= (uint32_t)(prxDacFifo->gAudiodacbuf + sizeof(prxDacFifo->gAudiodacbuf)))
        {
            prxDacFifo->gdacRptr = (uint32_t)(&prxDacFifo->gAudiodacbuf[0]);
        }
        DCD_EP_PrepareRx(pdev,
                         AUDIO_OUT_EP,
                         (uint8_t *) prxDacFifo->gdacRptr,
                         rx_DataLength);
        prxDacFifo->gdacRptr +=  len;
        HWRITEW(CORE_DAC_WPTR, prxDacFifo->gdacRptr);
#endif

#if 0
#ifdef FUNCTION_IIS_MODE
        if(Audio_IISCheckEmpty())
        {
            DEBUG_LOG_STRING("dac play empty: 0x%x\r\n", HREADW(CORE_IIS_TX_WRPTR));
        }
        uint16_t bufferRemind = Audio_IISGetRemindBufferSize();
        uint16_t totalBufferSize = HREADW(CORE_I2S_LEN) + 1;
#ifdef FUNCTION_ADJUST_FREQ_BY_BUFFER
        if(bufferRemind > ((totalBufferSize >> 1) + 0x100))
        {
            DEBUG_LOG_STRING("S R: %x, r: %x, w: %x\r\n", bufferRemind, HREADW(CORE_I2S_RDPTR), HREADW(CORE_IIS_TX_WRPTR));
            dsmdiv_12m_adjust_value(32);//slow down
        }
        else if(bufferRemind < ((totalBufferSize >> 1) - 0x100))
        {
            DEBUG_LOG_STRING("F R: %x, r: %x, w: %x\r\n", bufferRemind, HREADW(CORE_I2S_RDPTR), HREADW(CORE_IIS_TX_WRPTR));
            dsmdiv_12m_adjust_value(-32);//be quick
        }
#endif
        if(Audio_IISGetRemindBufferSize() < rxLen + 100)
        {
            DEBUG_LOG_STRING("Remind not enough, R: %x, r: %x, w: %x\r\n", bufferRemind, HREADW(CORE_I2S_RDPTR), HREADW(CORE_IIS_TX_WRPTR));
            return 1;
        }
        prxDacFifo->gdacRptr += rxLen;
        if( prxDacFifo->gdacRptr >= (uint32_t)(prxDacFifo->gAudiodacbuf + sizeof(prxDacFifo->gAudiodacbuf)))
            prxDacFifo->gdacRptr = (uint32_t)(&prxDacFifo->gAudiodacbuf[0]);
        // Cache buffer
        if(gIsFirstBufferCache != 0)
        {
            gIsFirstBufferCache--;
            return 1;
        }
        HWRITEW(CORE_IIS_TX_WRPTR, prxDacFifo->gdacRptr);
        DEBUG_LOG_STRING("Set: %x\r\n", prxDacFifo->gdacRptr);
#else
        if(Audio_DacCheckEmpty())
        {
            DEBUG_LOG_STRING("dac play empty: 0x%x\r\n", prxDacFifo->gdacRptr);
        }
        uint16_t bufferRemind = Audio_DacGetRemindBufferSize();
        uint16_t totalBufferSize = HREADW(CORE_DAC_LEN) + 1;
        if(Audio_DacGetRemindBufferSize() < AUDIO_DAC_48K_LEN)
        {
            DEBUG_LOG_STRING("remain not enough.\r\n");
            return 1;
        }
#ifdef FUNCTION_ADJUST_FREQ_BY_BUFFER
        if(bufferRemind > ((totalBufferSize >> 1) + 0x100))
        {
            //DEBUG_LOG_STRING("S R: %x, r: %x, w: %x\r\n", bufferRemind, HREADW(CORE_I2S_RDPTR), HREADW(CORE_IIS_TX_WRPTR));
            dsmdiv_12m_adjust_value(32);//slow down
        }
        else if(bufferRemind < ((totalBufferSize >> 1) - 0x100))
        {
            //DEBUG_LOG_STRING("F R: %x, r: %x, w: %x\r\n", bufferRemind, HREADW(CORE_I2S_RDPTR), HREADW(CORE_IIS_TX_WRPTR));
            dsmdiv_12m_adjust_value(-32);//be quick
        }
#endif

        prxDacFifo->gdacRptr += rxLen;
        if( prxDacFifo->gdacRptr >= (uint32_t)(prxDacFifo->gAudiodacbuf + sizeof(prxDacFifo->gAudiodacbuf)))
            prxDacFifo->gdacRptr = (uint32_t)(&prxDacFifo->gAudiodacbuf[0]);
        // Cache buffer
        if(gIsFirstBufferCache != 0)
        {
            gIsFirstBufferCache--;
            return 1;
        }
        HWRITEW(CORE_DAC_WPTR, prxDacFifo->gdacRptr);
        //DEBUG_LOG_STRING("Set: %x\r\n", prxDacFifo->gdacRptr);
#endif
#endif
    }
    return USBD_OK;
}

#ifdef HID_AUDIO_DEMO
void usb0_control_data(USB_OTG_CORE_HANDLE *pdev, uint16_t len)
{
    if(AudioCtlCmd == 0x01 && len > 0)
    {
        USB_OTG_ReadPacket(pdev,
                           AudioCtl,
                           0,
                           len);
        AudioCtlCmd = 0;
    }
    if(((pdev ->dev.EP0Setcmd_num1) || (pdev ->dev.EP0Setcmd_num2)) && ((len == 0x01) || (len == 0x02)))
    {
        if(pdev ->dev.EP0Setcmd_num1)
        {
            usb_set_cur_buf[setcur_num].usb_setcur_rxptr = AudioCtl;
            usb_set_cur_buf[setcur_num].usb_setcur_len = len;
            usb_set_cur_buf[setcur_num].setcur_type = 1;
            usb_set_cur_buf[setcur_num].voice_channel = pdev ->dev.Channel;
            setcur_num++;
            pdev ->dev.EP0Setcmd_num1 = 0;
        }
        else if(pdev ->dev.EP0Setcmd_num2)
        {
            usb_set_cur_buf[setcur_num].usb_setcur_rxptr = AudioCtl;
            usb_set_cur_buf[setcur_num].usb_setcur_len = len;
            usb_set_cur_buf[setcur_num].setcur_type = 2;
            setcur_num++;
        }

        return ;
    }
}
#endif

#ifdef CCID_DEMO

#endif
/**
* @brief  DCD_HandleUsbReset_ISR
*         This interrupt occurs when a USB Reset is detected
* @param  pdev: device instance
* @retval status
*/
/**
 * @method usb_reset
 * @brief  None
 * @param  None
 * @retval None
 */
void usb_reset()
{
    HWRITE(core_usb_hmode, 0x00);
    HWRITE(core_usb_trig, 0x70);
    HWRITEW(core_usb_status, 0x7ff);
    usb_main();
    //  MyPrintf("usb reset\n");
}
/**
 * @method usb_reset
 * @brief  None
 * @param  None
 * @retval None
 */

uint32_t DCD_HandleUsbReset_ISR()
{
    usb_reset();
    return 1;
}

void usb_clkcal_done()
{
#ifdef FUNCTION_ADJUST_FREQ_BY_USB
    dsmdiv_12m_adjust_by_usb_sof(HREADW(core_usb_ccnt));
#endif
}


void DCD_Init(USB_OTG_CORE_HANDLE *pdev,
              USB_OTG_CORE_ID_TypeDef coreID)
{
    uint32_t i;
    USB_OTG_EP *ep;
    /* Set Register Address */
    USB_OTG_SelectCore (pdev, coreID);

    //    /* Force_FS */
    //    pdev->regs.COMMREGS->TESTMODE |= BIT5;

    pdev->dev.device_status = USB_OTG_DEFAULT;
    pdev->dev.device_address = 0;

    /* Init ep structure */
    for (i = 0; i < pdev->cfg.dev_endpoints; i++)
    {
        ep = &pdev->dev.in_ep[i];
        /* Init ep structure */
        ep->is_in = 1;
        ep->num = i;
        ep->tx_fifo_num = i;
        /* Control until ep is actvated */
        ep->type = EP_TYPE_CTRL;
        ep->maxpacket =  USB_OTG_MAX_EP0_SIZE;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }

    for (i = 0; i < pdev->cfg.dev_endpoints; i++)
    {
        ep = &pdev->dev.out_ep[i];
        /* Init ep structure */
        ep->is_in = 0;
        ep->num = i;
        ep->tx_fifo_num = i;
        /* Control until ep is activated */
        ep->type = EP_TYPE_CTRL;
        ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }


    /* Init the Core (common init.) */
    USB_OTG_CoreInit(pdev);

    //	 /* Force Device Mode*/
    USB_OTG_SetCurrentMode(pdev, DEVICE_MODE);


    //    /* Init Device */
    //    USB_OTG_CoreInitDev(pdev);

}



/**
* @brief  Configure an EP
* @param pdev : Device instance
* @param epdesc : Endpoint Descriptor
* @retval : status
*/
uint32_t DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev,
                     uint8_t ep_addr,
                     uint16_t ep_mps,
                     uint8_t ep_type)
{
    USB_OTG_EP *ep;

    if ((ep_addr & 0x80) == 0x80)
    {
        ep = &pdev->dev.in_ep[ep_addr & 0x7F];
    }
    else
    {
        ep = &pdev->dev.out_ep[ep_addr & 0x7F];
    }
    ep->num   = ep_addr & 0x7F;

    ep->is_in = (0x80 & ep_addr) != 0;
    ep->maxpacket = ep_mps;
    ep->type = ep_type;
    if (ep->is_in)
    {
        /* Assign a Tx FIFO */
        ep->tx_fifo_num = ep->num;
    }
    /* Set initial data PID. */
    if (ep_type == USB_OTG_EP_BULK )
    {
        ep->data_pid_start = 0;
    }
    // USB_OTG_EPActivate(pdev , ep );
    return 0;
}
/**
* @brief  called when an EP is disabled
* @param pdev: device instance
* @param ep_addr: endpoint address
* @retval : status
*/
uint32_t DCD_EP_Close(USB_OTG_CORE_HANDLE *pdev, uint8_t  ep_addr)
{
    USB_OTG_EP *ep;

    if ((ep_addr & 0x80) == 0x80)
    {
        ep = &pdev->dev.in_ep[ep_addr & 0x7F];
    }
    else
    {
        ep = &pdev->dev.out_ep[ep_addr & 0x7F];
    }
    ep->num   = ep_addr & 0x7F;
    ep->is_in = (0x80 & ep_addr) != 0;
    USB_OTG_EPDeactivate(pdev, ep );
    return 0;
}



/**
* @brief  DCD_EP_PrepareRx
* @param pdev: device instance
* @param ep_addr: endpoint address
* @param pbuf: pointer to Rx buffer
* @param buf_len: data length
* @retval : status
*/
uint32_t   DCD_EP_PrepareRx( USB_OTG_CORE_HANDLE *pdev,
                             uint8_t   ep_addr,
                             uint8_t *pbuf,
                             uint16_t  buf_len)
{
    USB_OTG_EP *ep;

    ep = &pdev->dev.out_ep[ep_addr & 0x7F];

    /*setup and start the Xfer */
    ep->xfer_buff = pbuf;
    ep->xfer_len = buf_len;
    ep->xfer_count = 0;
    ep->is_in = 0;
    ep->num = ep_addr & 0x7F;

    if (pdev->cfg.dma_enable == 1)
    {
        ep->dma_addr = (uint32_t)pbuf;
    }


    if ( ep->num == 0 )
    {
        USB_OTG_EP0StartXfer(pdev, ep);
    }
    else
    {

        USB_OTG_EPStartXfer(pdev, ep );
    }
    return 0;
}


/**
* @brief  Transmit data over USB
* @param pdev: device instance
* @param ep_addr: endpoint address
* @param pbuf: pointer to Tx buffer
* @param buf_len: data length
* @retval : status
*/
uint32_t  DCD_EP_Tx ( USB_OTG_CORE_HANDLE *pdev,
                      uint8_t   ep_addr,
                      uint8_t   *pbuf,
                      uint32_t   buf_len)
{
    USB_OTG_EP *ep;

    ep = &pdev->dev.in_ep[ep_addr & 0x7F];

    /* Setup and start the Transfer */
    ep->is_in = 1;
    ep->num = ep_addr & 0x7F;
    ep->xfer_buff = pbuf;
    //    ep->dma_addr = (uint32_t)pbuf;
    ep->xfer_count = 0;
    ep->xfer_len  = buf_len;

    if ( ep->num == 0 )
    {
        USB_OTG_EP0StartXfer(pdev, ep);
    }
    else
    {
        USB_OTG_EPStartXfer(pdev, ep );
    }
    return 0;
}






/**
* @brief  Stall an endpoint.
* @param pdev: device instance
* @param epnum: endpoint address
* @retval : status
*/

uint32_t  DCD_EP_Stall (USB_OTG_CORE_HANDLE *pdev, uint8_t   epnum)
{
    USB_OTG_EP *ep;
    if ((0x80 & epnum) == 0x80)
    {
        ep = &pdev->dev.in_ep[epnum & 0x7F];
    }
    else
    {
        ep = &pdev->dev.out_ep[epnum];
    }

    ep->is_stall = 1;
    ep->num   = epnum & 0x7F;
    ep->is_in = ((epnum & 0x80) == 0x80);

    USB_OTG_EPSetStall(pdev, ep);
    return (0);
}



/************************ (C) COPYRIGHT Yichip Microelectronics *****END OF FILE****/
