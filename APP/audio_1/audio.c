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


#include "hd_sbc.h"
#include "adpcm.h"


#ifndef TIMEOUT_TIME
#define TIMEOUT_TIME        (8000)
#endif
#ifndef START_NTF_TIME
#define START_NTF_TIME      (10)
#endif


typedef struct {
    uint8_t VoiceEncodeBuf[ENCODE_OUTPUT_MSBC_SIZE];
}VoiceEncodeQue_TypeDef_MSBC;

typedef struct {
    uint8_t VoiceEncodeBuf[ENCODE_OUTPUT_ADCPCM_SIZE];
}VoiceEncodeQue_TypeDef_ADCPCM;

typedef struct {
    uint8_t *ReadPtr;
    uint8_t DataBuf[ENCODE_INPUT_MSBC_SIZE*MICRECORD_CACHE_MSBC_NUM];
}MicRecord_TypeDef_MSBC;

typedef struct {
    uint8_t *ReadPtr;
    uint8_t DataBuf[ENCODE_INPUT_ADCPCM_SIZE*MICRECORD_CACHE_ADCPCM_NUM];
}MicRecord_TypeDef_ADCPCM;
#ifdef FUNCTION_WATCH_DOG
extern IWDG_InitTypeDef gWdtInit;
#endif

static const uint8_t audio_end[] = {0x00};
static const uint8_t search_buf[] = {0x08};
static const uint8_t audio_start[] = {0x04};
static const uint8_t get_caps_respone_buf[] = {0x0B ,0x01 ,0x00 ,0x03 ,0x03 ,0x00 ,0x78 ,0x00,0x00};

static  uint8_t send_num_buf[3] = {0x0A,0X00,0X00};
static  uint16_t send_num_cnt;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t adpcm_sendbuf_offset;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static google_tv_audio_header audioHeader;

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t ntf_buf_offset;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t VoiceNTFBuf_MSBC[AUDIO_SEND_NTF_MSBC_SIZE];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t VoiceNTFBuf_ADCPCM[AUDIO_SEND_NTF_ADCPCM_SIZE];

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static QueMgr_TypeDef VoiceEncodeQueMgr;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static VoiceEncodeQue_TypeDef_MSBC VoiceEncodeQue_MSBC[ENCODE_CACHE_MSBC_NUM];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static VoiceEncodeQue_TypeDef_ADCPCM VoiceEncodeQue_ADCPCM[ENCODE_CACHE_MSBC_NUM];
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static MicRecord_TypeDef_MSBC MicRecord_MSBC;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static MicRecord_TypeDef_ADCPCM MicRecord_ADCPCM;
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
        int MicRecord_Len;
        if(List_state == MSBC_MODE)
        {
            MicRecord_Len = AdcWptr - TO_16BIT_ADDR(MicRecord_MSBC.ReadPtr);
            if (MicRecord_Len < 0) {            
                return (uint16_t)(MicRecord_Len + ENCODE_INPUT_MSBC_SIZE*ENCODE_CACHE_MSBC_NUM);
            }
            return (uint16_t)MicRecord_Len;
        }
        else{
            MicRecord_Len = AdcWptr - TO_16BIT_ADDR(MicRecord_ADCPCM.ReadPtr);
            if (MicRecord_Len < 0) {               
                return (uint16_t)(MicRecord_Len + ENCODE_INPUT_ADCPCM_SIZE*ENCODE_CACHE_ADCPCM_NUM);
            }
            return (uint16_t)MicRecord_Len;
        }

}

static void Audio_AdcConfig(void)
{
    AUDIO_ADCInitTypeDef adcparaminit;


    if(List_state == MSBC_MODE){
        adcparaminit.Adc_StartAddr = MicRecord_MSBC.DataBuf;
        adcparaminit.Adc_BufferSize = ENCODE_INPUT_MSBC_SIZE*MICRECORD_CACHE_MSBC_NUM;
    }
    else{
        adcparaminit.Adc_StartAddr = MicRecord_ADCPCM.DataBuf;
        adcparaminit.Adc_BufferSize = ENCODE_INPUT_ADCPCM_SIZE*MICRECORD_CACHE_ADCPCM_NUM;
    }
    adcparaminit.Adc_Sample = ADC_Sample16K;
    adcparaminit.Mic_Type = MicIsANA;              
    adcparaminit.Adc_VolFadeEnable = ENABLE;  
    adcparaminit.Adc_VolStep = 2;
    adcparaminit.Adc_Vol = 10;
    adcparaminit.Adc_MuteChannel = DISABLE;
    adcparaminit.Adc_RssiConfig = adc_RssiDisable;
    adcparaminit.Adc_HPFConfig = adc_HPFDisable;

    adcparaminit.Adc_OutputMode = AdcOutputMode;

    AUDIO_AdcInit(adcparaminit);
}

static void Voice_Send(void)
{
    while(IPC_TxBufferIsEnough(4) && Bt_HciGetUsedBufferNum() < HCI_QUEUE_MAX -3 && VoiceEncodeQueMgr.current_queue_len > 0)
    {
#ifdef FUNCTION_WATCH_DOG
        IWDG_ReloadCounter(&gWdtInit);
#endif 
        if(List_state == MSBC_MODE)
        {
            memcpy((void *)&VoiceNTFBuf_MSBC[ntf_buf_offset], (void *)&VoiceEncodeQue_MSBC[VoiceEncodeQueMgr.read_index++], ENCODE_OUTPUT_MSBC_SIZE - 3);
            ntf_buf_offset += ENCODE_OUTPUT_MSBC_SIZE;

            VoiceEncodeQueMgr.current_queue_len--;
            voice_status.loss_cnt--;
            if (ntf_buf_offset >= AUDIO_SEND_NTF_MSBC_SIZE)
            {
                ntf_buf_offset = AUDIO_FRAME_HEADER_MSBC_LEN;
                update_voice_packet(VoiceNTFBuf_MSBC);
                ATT_sendNotify(AUDIO_SNED_MSBC_HANDLE, VoiceNTFBuf_MSBC, AUDIO_SEND_NTF_MSBC_SIZE);
            }
            if (VoiceEncodeQueMgr.read_index == ENCODE_CACHE_MSBC_NUM - 1) {
                VoiceEncodeQueMgr.read_index = 0;
            }
        }
        else
        {
            ATT_sendNotify(AUDIO_SNED_ADCPCM_HANDLE, (void *)&VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.read_index++].VoiceEncodeBuf[0], ENCODE_OUTPUT_ADCPCM_SIZE);
            VoiceEncodeQueMgr.current_queue_len--;
            send_num_cnt++;
#if 1
            if(send_num_cnt % 16 == 0){
                send_num_buf[2] = send_num_cnt; /// send_num_cnt;
                send_num_buf[1] = send_num_cnt >>8; 
                ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, send_num_buf, 3);            
            }
#endif
            voice_status.loss_cnt--;

            if (VoiceEncodeQueMgr.read_index == ENCODE_CACHE_ADCPCM_NUM - 1) {
                VoiceEncodeQueMgr.read_index = 0;
            }
        }
    }
}

static void Voice_Encode(void)
{
    if(List_state == MSBC_MODE)
    {
        if (Get_MicReocrd_Len() >= ENCODE_INPUT_MSBC_SIZE && VoiceEncodeQueMgr.current_queue_len < ENCODE_CACHE_MSBC_NUM)
        {
            for(uint8_t i = 0; i < ENCODE_TIMES_ONCE; i++)
            {

                msbc_encode_process((int16_t *)MicRecord_MSBC.ReadPtr, (void *)&VoiceEncodeQue_MSBC[VoiceEncodeQueMgr.write_index++]);
                voice_status.loss_cnt++;
                VoiceEncodeQueMgr.current_queue_len++;
                if (VoiceEncodeQueMgr.write_index == ENCODE_CACHE_MSBC_NUM - 1)
                {
                    VoiceEncodeQueMgr.write_index = 0;
                }
                MicRecord_MSBC.ReadPtr += ENCODE_INPUT_MSBC_SIZE;
                if (MicRecord_MSBC.ReadPtr == &MicRecord_MSBC.DataBuf[ENCODE_INPUT_MSBC_SIZE*MICRECORD_CACHE_MSBC_NUM])
                {
                    MicRecord_MSBC.ReadPtr = MicRecord_MSBC.DataBuf;
                }
            }
        }
    }
    else
    {
        if (Get_MicReocrd_Len() >= ENCODE_INPUT_ADCPCM_SIZE && VoiceEncodeQueMgr.current_queue_len < ENCODE_CACHE_ADCPCM_NUM)
        {
            for(uint8_t i = 0; i < ENCODE_TIMES_ONCE; i++)
            {

                Adpcm_FrameEncode_Google_TV_Audio((int16_t *)MicRecord_ADCPCM.ReadPtr, (void *)&VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[6], &audioHeader, ENCODE_INPUT_ADCPCM_SIZE/2);
#if 1
                VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[0] = ((audioHeader.frame_number & 0xff00) >> 8);
                VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[1] = ((audioHeader.frame_number & 0xff) - 1);
                VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[2] = audioHeader.remote_id;
                VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[3] = (audioHeader.adpcmVal.previous_predict_adpcm & 0xff00) >> 8;
                VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[4] = (audioHeader.adpcmVal.previous_predict_adpcm & 0xff);
                VoiceEncodeQue_ADCPCM[VoiceEncodeQueMgr.write_index].VoiceEncodeBuf[5] = audioHeader.adpcmVal.tableIndex;
                VoiceEncodeQueMgr.write_index++;
                voice_status.loss_cnt += 1;
#endif

                VoiceEncodeQueMgr.current_queue_len++;
                if (VoiceEncodeQueMgr.write_index == ENCODE_CACHE_ADCPCM_NUM - 1)
                {
                    VoiceEncodeQueMgr.write_index = 0;
                }
                MicRecord_ADCPCM.ReadPtr += ENCODE_INPUT_ADCPCM_SIZE;
                if (MicRecord_ADCPCM.ReadPtr == &MicRecord_ADCPCM.DataBuf[ENCODE_INPUT_ADCPCM_SIZE*MICRECORD_CACHE_ADCPCM_NUM])
                {
                    MicRecord_ADCPCM.ReadPtr = MicRecord_ADCPCM.DataBuf;
                }
            }
        }    
    }
}

static void audio_task(void)
{

    if (bt_check_le_connected() && voice_status.mic_open_flag)
    {
        Voice_Encode();
        Voice_Send();
        app_queue_insert(audio_task);
    }
}

static void start_ntf_timer_cb(void)
{
    if (bt_check_le_connected() && voice_status.mic_open_flag)
    {
        DEBUG_LOG_STRING("start_ntf_timer_cb \r\n");
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
    return 0;
}

void voice_status_change(void)
{
    if (voice_status.get_caps) {
        voice_status.get_caps = false;
        ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, (void *)get_caps_respone_buf, sizeof(get_caps_respone_buf));
    }
    else if(voice_status.search) {
        voice_status.search = false;
        ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, (void *)search_buf, sizeof(search_buf));
    }
    else if (voice_status.mic_open) {
        voice_status.mic_open = false;  
        ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, (void *)audio_start, sizeof(audio_start));
        mic_open();
    }
    else if (voice_status.mic_close) {
        voice_status.mic_close = false;
        ATT_sendNotify(AUDIO_CTRL_ADCPCM_HANDLE, (void *)audio_end, sizeof(audio_end));
        mic_close();
    }
}

void mic_close(void)
{
    DEBUG_LOG_STRING("PACKET LOSS: %d \r\n", voice_status.loss_cnt);
    voice_status.mic_open_flag = false;
    voice_status.loss_cnt = 0;

    AUDIO_AdcDmaSet(false);
    AUDIO_AdcStop();
    bt_disable_le_tx_md();
    tx_power_switch_set(true);
    app_sleep_lock_set(AUDIO_LOCK, false);
    action_after_mic_close();
    System_ChangeXtal24M();
    DEBUG_LOG_STRING("mic close \r\n");
}

void mic_open(void)
{
    app_sleep_lock_set(AUDIO_LOCK, true);
    tx_power_switch_set(false);
    bt_set_tx_power(TX_POWER_5DB);
    bt_enable_le_tx_md();

    if(List_state == ADCPCM_MODE)
    {
        DEBUG_LOG_STRING("mic adcpcm open \r\n");
        send_num_cnt = 0;
        send_num_buf[1] = 0;
        send_num_buf[2] = 0;
        System_ChangeDPLL(CLOCK_DPLL_48M_multiple);
        Adpcm_FrameEncode_Restart(&audioHeader);
        voice_status.mic_open_flag = true;        
        MicRecord_ADCPCM.ReadPtr = MicRecord_ADCPCM.DataBuf;
        memset(MicRecord_ADCPCM.DataBuf, 0, ENCODE_INPUT_ADCPCM_SIZE*MICRECORD_CACHE_ADCPCM_NUM);
        memset(VoiceEncodeQue_ADCPCM, 0, ENCODE_OUTPUT_ADCPCM_SIZE*ENCODE_CACHE_ADCPCM_NUM);
        memset(&VoiceEncodeQueMgr, 0, sizeof(QueMgr_TypeDef));
    }
    else
    {
        DEBUG_LOG_STRING("mic msbc open \r\n");
        ntf_buf_offset = AUDIO_FRAME_HEADER_MSBC_LEN;
        voice_status.packet_cnt = 0;
        System_ChangeDPLL(CLOCK_DPLL_96M_multiple);
        memset(VoiceNTFBuf_MSBC, 0, AUDIO_SEND_NTF_MSBC_SIZE);
        voice_status.mic_open_flag = true;
        MicRecord_MSBC.ReadPtr = MicRecord_MSBC.DataBuf;
        memset(MicRecord_MSBC.DataBuf, 0, ENCODE_INPUT_MSBC_SIZE*MICRECORD_CACHE_MSBC_NUM);
        memset(VoiceEncodeQue_MSBC, 0, ENCODE_OUTPUT_MSBC_SIZE*ENCODE_CACHE_MSBC_NUM);
        memset(&VoiceEncodeQueMgr, 0, sizeof(QueMgr_TypeDef));        
    }

    Audio_ClkInit();
    Audio_AdcConfig();
    swtimer_start(start_ntf_timernum, START_NTF_TIME, TIMER_START_ONCE);
}

void voice_report_reinit(void)
{
    memset(&voice_status, 0, sizeof(voice_status_t));
    msbc_encoder_init();
}

uint8_t voice_report_init(void)
{
    uint8_t ret = 0;
    ret = voice_timer_init();
    msbc_encoder_init();
    return ret;
}