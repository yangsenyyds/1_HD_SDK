#include "audio.h"
#include "software_timer.h"
#include "app_sleep.h"
#include "app_queue.h"
#include "yc11xx_audio_adc.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_ipc.h"
#include "yc_dev_bt.h"
#include "yc11xx_systick.h"
#include "att.h"
#include "reg_addr.h"
#include "app_config.h"
#include "yc_debug.h"

#ifdef FUNCTION_WATCH_DOG
#include "yc11xx_iwdg.h"
#endif

#if OPUS
#include "opus_codec_drv.h"
#elif MSBC
#include "hd_sbc.h"
#elif ADPCM 
#include "adpcm.h"
#endif

#ifdef AUDIO_TEST_MODE
#include "yc11xx_uart.h"
#include "yc11xx_gpio.h"
#endif

#ifdef FACTORY_MODE
#include "yc11xx_dev_qspi.h"
#include "hd_sbc.h"
#endif

#ifndef TIMEOUT_TIME
#define TIMEOUT_TIME        (8000)
#endif
#ifndef START_NTF_TIME
#define START_NTF_TIME      (10)
#endif

#ifdef AUDIO_TEST_MODE
#define uart_DMA_TXBuf_len  (1024)
#define uart_DMA_RXBuf_len  (2)
#endif

typedef struct {
    uint8_t VoiceEncodeBuf[ENCODE_OUTPUT_SIZE];
}VoiceEncodeQue_TypeDef;

typedef struct {
    uint8_t *ReadPtr;
    uint8_t DataBuf[ENCODE_INPUT_SIZE*MICRECORD_CACHE_NUM];
}MicRecord_TypeDef;

#ifdef FUNCTION_WATCH_DOG
extern IWDG_InitTypeDef gWdtInit;
#endif
#ifdef FACTORY_MODE
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t adc_outputmode;
#endif
#ifdef AUDIO_TEST_MODE
#if OPUS
static const int16_t input_sample1khz[320]={
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce,
	0x0000, 0x2730, 0x4868,	0x5e9c,	0x6665,	0x5e9c,	0x4868,	0x272f,	0x0001,	0xd8cf,	0xb79a,	0xa161,	0x999e,	0xa161,	0xb79b,	0xd8ce
};
#endif
MEMORY_NOT_PROTECT_UNDER_LPM_ATT uint8_t UART_TxBuf[uart_DMA_TXBuf_len];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT uint8_t UART_RxBuf[uart_DMA_RXBuf_len];
#endif
#ifdef ADPCM
static const uint8_t audio_end[] = {0x00};
#ifdef audio_start_20
static const uint8_t search_buf[20] = {0x08};
static const uint8_t audio_start[20] = {0x04};
static const uint8_t get_caps_respone_buf[20] = {0x0B,0x00,0x04,0x00,0x03,0x00,0x86,0x00,0x14};
#else
static const uint8_t search_buf[] = {0x08};
static const uint8_t audio_start[] = {0x04};
// static const uint8_t get_caps_respone_buf[] = {0x0B,0x00,0x04,0x00,0x03,0x00,0x86,0x00,0x14};
static const uint8_t get_caps_respone_buf[] = {0x0B ,0x00 ,0x04 ,0x00 ,0x03 ,0x00 ,0x86 ,0x00,0x86};
#endif


static  uint8_t send_num_buf[3] = {0x0A,0X00,0X00};
static  uint16_t send_num_cnt;


MEMORY_NOT_PROTECT_UNDER_LPM_ATT static google_tv_audio_header audioHeader;
#elif MSBC
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t ntf_buf_offset;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t VoiceNTFBuf[AUDIO_SEND_NTF_SIZE];
#endif
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static QueMgr_TypeDef VoiceEncodeQueMgr;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static VoiceEncodeQue_TypeDef VoiceEncodeQue[ENCODE_CACHE_NUM];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static MicRecord_TypeDef MicRecord;
voice_status_t voice_status;
static uint8_t timeout_timernum = 0xFF;
static uint8_t start_ntf_timernum = 0xFF;

WEAK void update_voice_packet(uint8_t *voice_buf){}
WEAK void action_after_mic_close(void){}
WEAK void tx_power_switch_set(bool switch_enable){}
WEAK void app_sleep_lock_set(DEV_LOCK_TypeDef dev_lock, bool state){}

static uint16_t Get_MicReocrd_Len(void)
{
    uint32_t AdcWptr = HREADW(CORE_ADCD_ADDR);
    int MicRecord_Len = AdcWptr - TO_16BIT_ADDR(MicRecord.ReadPtr);

    if (MicRecord_Len < 0) {
        return (uint16_t)(MicRecord_Len + ENCODE_INPUT_SIZE*MICRECORD_CACHE_NUM);
    }

    return (uint16_t)MicRecord_Len;
}

static void Audio_AdcConfig(void)
{
    AUDIO_ADCInitTypeDef adcparaminit;

    adcparaminit.Adc_StartAddr = MicRecord.DataBuf; 
    adcparaminit.Adc_BufferSize = ENCODE_INPUT_SIZE*MICRECORD_CACHE_NUM;
    adcparaminit.Adc_Sample = ADC_Sample16K;
    adcparaminit.Mic_Type = MicIsANA;              
    adcparaminit.Adc_VolFadeEnable = ENABLE;  
    adcparaminit.Adc_VolStep = 2;
    adcparaminit.Adc_Vol = 10;
    adcparaminit.Adc_MuteChannel = DISABLE;
    adcparaminit.Adc_RssiConfig = adc_RssiDisable;
    adcparaminit.Adc_HPFConfig = adc_HPFDisable;
#ifdef FACTORY_MODE
    adcparaminit.Adc_OutputMode = adc_outputmode;
#else
    adcparaminit.Adc_OutputMode = AdcOutputMode;
#endif
    AUDIO_AdcInit(adcparaminit);
}


static void Voice_Send(void)
{
    while(IPC_TxBufferIsEnough(4) && Bt_HciGetUsedBufferNum() < HCI_QUEUE_MAX -3 && VoiceEncodeQueMgr.current_queue_len > 0)
    {
#ifdef FUNCTION_WATCH_DOG
        IWDG_ReloadCounter(&gWdtInit);
#endif 

        ATT_sendNotify(AUDIO_SNED_HANDLE, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.read_index++].VoiceEncodeBuf[0], ENCODE_OUTPUT_SIZE);
        VoiceEncodeQueMgr.current_queue_len--;
        send_num_cnt++;
        if(send_num_cnt % 16 == 0){
            send_num_buf[2] = send_num_cnt; /// send_num_cnt;
            send_num_buf[1] = send_num_cnt >>8; 
            ATT_sendNotify(AUDIO_CTRL_HANDLE, send_num_buf, 3);            
        }

        
        voice_status.loss_cnt--;

        if (VoiceEncodeQueMgr.read_index == ENCODE_CACHE_NUM - 1) {
            VoiceEncodeQueMgr.read_index = 0;
        }
    }
}


static void Voice_Encode(void)
{ 
    if (Get_MicReocrd_Len() >= ENCODE_INPUT_SIZE && VoiceEncodeQueMgr.current_queue_len < ENCODE_CACHE_NUM)
    {
        for(uint8_t i = 0; i < ENCODE_TIMES_ONCE; i++)
        {
#ifdef AUDIO_TEST_MODE
            if (voice_status.mode == STANDARD_DATA)
            {
#if OPUS
                int frame_size = drv_audio_codec_encode((int16_t *)input_sample1khz, (void *)&VoiceEncodeQue[0].VoiceEncodeBuf[2]);
                VoiceEncodeQue[0].VoiceEncodeBuf[0] = (uint8_t)(frame_size >> 8);
                VoiceEncodeQue[0].VoiceEncodeBuf[1] = (uint8_t)frame_size;
                VoiceEncodeQue[0].VoiceEncodeBuf[82] = '\n';
                UART_SendDataFromBuff(AUDIO_UART, (void *)&VoiceEncodeQue[0], AUDIO_SEND_NTF_SIZE+3);
#endif
                return;
            }
            else if (voice_status.mode == PCM_DATA)
            {
                UART_SendDataFromBuff(AUDIO_UART, MicRecord.ReadPtr, ENCODE_INPUT_SIZE);
                MicRecord.ReadPtr += ENCODE_INPUT_SIZE;
                if (MicRecord.ReadPtr == &MicRecord.DataBuf[ENCODE_INPUT_SIZE*MICRECORD_CACHE_NUM])
                {
                    MicRecord.ReadPtr = MicRecord.DataBuf;
                }

                return;
            } 
#endif

#ifdef FACTORY_MODE
            msbc_encode_process((int16_t *)MicRecord.ReadPtr, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.write_index++]);
            voice_status.loss_cnt++;
#endif

#if OPUS
            drv_audio_codec_encode((int16_t *)MicRecord.ReadPtr, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.write_index++]);
            voice_status.loss_cnt++;
#elif MSBC
            msbc_encode_process((int16_t *)MicRecord.ReadPtr, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.write_index++]);
            voice_status.loss_cnt++;
#elif ADPCM
            

            Adpcm_FrameEncode_Google_TV_Audio((int16_t *)MicRecord.ReadPtr, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[6], &audioHeader, ENCODE_INPUT_SIZE/2);
            VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[0] = ((audioHeader.frame_number & 0xff00) >> 8);
            VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[1] = ((audioHeader.frame_number & 0xff) - 1);
            VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[2] = audioHeader.remote_id;
            VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[3] = (audioHeader.adpcmVal.previous_predict_adpcm & 0xff00) >> 8;
            VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[4] = (audioHeader.adpcmVal.previous_predict_adpcm & 0xff);
            VoiceEncodeQue[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[5] = audioHeader.adpcmVal.tableIndex;
            VoiceEncodeQueMgr.write_index++;
            voice_status.loss_cnt += 1;

#endif
            VoiceEncodeQueMgr.current_queue_len++;
            if (VoiceEncodeQueMgr.write_index == ENCODE_CACHE_NUM - 1)
            {
                VoiceEncodeQueMgr.write_index = 0;
            }
            MicRecord.ReadPtr += ENCODE_INPUT_SIZE;
            if (MicRecord.ReadPtr == &MicRecord.DataBuf[ENCODE_INPUT_SIZE*MICRECORD_CACHE_NUM])
            {
                MicRecord.ReadPtr = MicRecord.DataBuf;
            }
#ifdef AUDIO_TEST_MODE
            if (voice_status.mode == ENCODE_DATA)
            {
#ifdef FACTORY_MODE
                UART_SendDataFromBuff(AUDIO_UART, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.read_index++], ENCODE_SEND_SIZE);
#endif
#if ADPCM
                uint8_t uart_send_buf[ENCODE_OUTPUT_SIZE];
                memcpy(uart_send_buf, (void *)&VoiceEncodeQue[VoiceEncodeQueMgr.read_index++].VoiceEncodeBuf[3], ENCODE_OUTPUT_SIZE-3);
                uart_send_buf[ENCODE_OUTPUT_SIZE-3] = '\r';
                uart_send_buf[ENCODE_OUTPUT_SIZE-2] = '\n';
                UART_SendDataFromBuff(AUDIO_UART, uart_send_buf, ENCODE_OUTPUT_SIZE-1);
#endif
                VoiceEncodeQueMgr.current_queue_len--;
                if (VoiceEncodeQueMgr.read_index == ENCODE_CACHE_NUM - 1) {
                    VoiceEncodeQueMgr.read_index = 0;
                }
            }
#endif
        }
    }
}

static void audio_task(void)
{
#ifdef AUDIO_TEST_MODE
    if (voice_status.mic_open_flag) {
        Voice_Encode();
        app_queue_insert(audio_task);
    }
#else
    if (bt_check_le_connected() && voice_status.mic_open_flag)
    {
        Voice_Encode();
        Voice_Send();

        app_queue_insert(audio_task);
    }
#endif
}

#ifndef AUDIO_TEST_MODE
#ifndef FACTORY_MODE
static void timeout_timer_cb(void)
{
    if (voice_status.mic_open_flag)
    {
        DEBUG_LOG_STRING("TIMEOUT \r\n");
        mic_close();
#if OPUS
#elif MSBC
#elif ADPCM
        ATT_sendNotify(AUDIO_CTRL_HANDLE, (void *)audio_end, sizeof(audio_end));
#endif
    }
}
#endif

static void start_ntf_timer_cb(void)
{
    if (bt_check_le_connected() && voice_status.mic_open_flag)
    {
        AUDIO_AdcDmaSet(true);
        audio_task();
    }
    else {
        mic_close();
    }
}

static uint8_t voice_timer_init(void)
{
    start_ntf_timernum = swtimer_add(start_ntf_timer_cb);
    if (start_ntf_timernum > TIMER_NUM) {
        DEBUG_LOG_STRING("VOICE NTF TIMER ADD FAIL: %d \r\n", start_ntf_timernum);
        return start_ntf_timernum;
    }
#ifndef FACTORY_MODE
    timeout_timernum = swtimer_add(timeout_timer_cb);
    if (timeout_timernum > TIMER_NUM) {
        DEBUG_LOG_STRING("VOICE TIMEOUT TIMER ADD FAIL: %d \r\n", timeout_timernum);
        return timeout_timernum;
    }
#endif

    return 0;
}
#endif

#ifdef ADPCM
void voice_status_change(void)
{
    if (voice_status.get_caps) {
        voice_status.get_caps = false;
        ATT_sendNotify(AUDIO_CTRL_HANDLE, (void *)get_caps_respone_buf, sizeof(get_caps_respone_buf));
    }
    else if(voice_status.search) {
        voice_status.search = false;
        ATT_sendNotify(AUDIO_CTRL_HANDLE, (void *)search_buf, sizeof(search_buf));
    }
    else if (voice_status.mic_open) {
        voice_status.mic_open = false;  
        ATT_sendNotify(AUDIO_CTRL_HANDLE, (void *)audio_start, sizeof(audio_start));
        mic_open();
    }
    else if (voice_status.mic_close) {
        voice_status.mic_close = false;
        ATT_sendNotify(AUDIO_CTRL_HANDLE, (void *)audio_end, sizeof(audio_end));
        mic_close();
    }
}
#endif

void mic_close(void)
{
    DEBUG_LOG_STRING("PACKET LOSS: %d \r\n", voice_status.loss_cnt);
    voice_status.mic_open_flag = false;
    voice_status.loss_cnt = 0;

#ifndef AUDIO_TEST_MODE
    swtimer_stop(timeout_timernum);
#endif
    AUDIO_AdcDmaSet(false);
    AUDIO_AdcStop();
    bt_disable_le_tx_md();
    tx_power_switch_set(true);
    app_sleep_lock_set(AUDIO_LOCK, false);
    action_after_mic_close();
    System_ChangeXtal24M();
    DEBUG_LOG_STRING("mic close \r\n");
    // DEBUG_LOG_STRING("HciUsedBufferNum [%d] \r\n", Bt_HciGetUsedBufferNum());
}

void mic_open(void)
{
    DEBUG_LOG_STRING("mic open \r\n");
    app_sleep_lock_set(AUDIO_LOCK, true);
    tx_power_switch_set(false);
    bt_set_tx_power(TX_POWER_5DB);
    bt_enable_le_tx_md();
    send_num_cnt = 0;
    send_num_buf[1] = 0;
    send_num_buf[2] = 0;
    System_ChangeDPLL(CLOCK_DPLL_48M_multiple);
    Adpcm_FrameEncode_Restart(&audioHeader);

    voice_status.mic_open_flag = true;
    MicRecord.ReadPtr = MicRecord.DataBuf;
    memset(MicRecord.DataBuf, 0, ENCODE_INPUT_SIZE*MICRECORD_CACHE_NUM);
    memset(VoiceEncodeQue, 0, ENCODE_OUTPUT_SIZE*ENCODE_CACHE_NUM);
    memset(&VoiceEncodeQueMgr, 0, sizeof(QueMgr_TypeDef));

    Audio_ClkInit();
    Audio_AdcConfig();

    swtimer_start(start_ntf_timernum, START_NTF_TIME, TIMER_START_ONCE);
#ifndef FACTORY_MODE
    swtimer_start(timeout_timernum, TIMEOUT_TIME, TIMER_START_ONCE);
#endif
}

#ifndef FACTORY_MODE
void voice_report_reinit(void)
{
    memset(&voice_status, 0, sizeof(voice_status_t));
#if OPUS
    drv_audio_codec_init();
#elif MSBC
    msbc_encoder_init();
#endif
}
#endif

uint8_t voice_report_init(void)
{
    uint8_t ret = 0;

#ifdef AUDIO_TEST_MODE
    GPIO_Init(AUDIO_UART_RX_GPIO, GPIO_Mode_Uart_Rxd);
    GPIO_Init(AUDIO_UART_TX_GPIO, GPIO_Mode_Uart_Txd);

    UART_InitTypeDef UART_InitStruct ;
    UART_InitStruct.UART_BaudRate             = 921600;
    UART_InitStruct.UART_RamSelect            = RAM_UseM0MRam;
    UART_InitStruct.UART_ClkSource            = UART_DPLL;
   	UART_InitStruct.UART_HardwareFlowControl  = UART_HardwareFlowControl_None;
   	UART_InitStruct.UART_StopBits             = UART_StopBits_1;
   	UART_InitStruct.UART_Mode                 = UART_Mode_duplex;
   	UART_InitStruct.UART_Parity               = UART_Parity_None ;
   	UART_InitStruct.UART_TxBuffer             = (uint32_t)UART_TxBuf;
   	UART_InitStruct.UART_TXLen                = sizeof(UART_TxBuf);
    UART_InitStruct.UART_RxBuffer             = (uint32_t)UART_RxBuf;
    UART_InitStruct.UART_RXLen                = sizeof(UART_RxBuf);

    UART_Init(AUDIO_UART, &UART_InitStruct);
#else
    ret = voice_timer_init();
#endif

#ifdef FACTORY_MODE
    QSPI_ReadFlashData(CODE_BURN_FLASHADDR2 + CODE_OFFSET_LEN + SERIAL_NAME_LEN - 1, 1, &adc_outputmode);
    msbc_encoder_init();
#endif

#if OPUS
    drv_audio_codec_init();
#elif MSBC
    msbc_encoder_init();
#endif

    return ret;
}
