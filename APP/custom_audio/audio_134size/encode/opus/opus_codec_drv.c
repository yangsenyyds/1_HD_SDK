#include "opus_codec_drv.h"
#include "opus.h"


#if (CONFIG_OPUS_MODE == CONFIG_OPUS_MODE_CELT)
     #define OPUS_APPLICATION    OPUS_APPLICATION_RESTRICTED_LOWDELAY
     #define OPUS_ENCODER_SIZE   2096   //2200
     #define OPUS_MODE           "CELT"
     #if  (CONFIG_AUDIO_SAMPLING_FREQUENCY != 8000) && \
          (CONFIG_AUDIO_SAMPLING_FREQUENCY != 16000) && \
          (CONFIG_AUDIO_SAMPLING_FREQUENCY != 24000)
     #error "Selected sampling frequency is not supported by CELT codec."
     #endif
     #if  (CONFIG_AUDIO_FRAME_SIZE_MS != 5) && \
          (CONFIG_AUDIO_FRAME_SIZE_MS != 10) && \
          (CONFIG_AUDIO_FRAME_SIZE_MS != 20)
     #error "Selected audio frame size is not supported by CELT codec."
     #endif
#elif (CONFIG_OPUS_MODE == CONFIG_OPUS_MODE_SILK)
     #define   OPUS_APPLICATION    OPUS_APPLICATION_VOIP
     #define   OPUS_ENCODER_SIZE   10916
     #define   OPUS_MODE           "SILK"
     #if       (CONFIG_AUDIO_SAMPLING_FREQUENCY != 8000) && \
               (CONFIG_AUDIO_SAMPLING_FREQUENCY != 16000)
     #error "Selected sampling frequency is not supported by SILK codec."
     #endif
     #if  (CONFIG_AUDIO_FRAME_SIZE_MS != 10) && \
          (CONFIG_AUDIO_FRAME_SIZE_MS != 20) && \
          (CONFIG_AUDIO_FRAME_SIZE_MS != 40) && \
          (CONFIG_AUDIO_FRAME_SIZE_MS != 60)
     #error "Selected audio frame size is not supported by SILK codec"
     #endif
#else
#error "Unsupported OPUS Mode"
#endif

#define     m_opus_complexity   CONFIG_OPUS_COMPLEXITY
#define     m_opus_bitrate      ((CONFIG_OPUS_BITRATE != 0) ? CONFIG_OPUS_BITRATE : OPUS_AUTO)
#define     m_opus_vbr          ((CONFIG_OPUS_BITRATE == 0) || (CONFIG_OPUS_VBR_ENABLED != 0))

MEMORY_NOT_PROTECT_UNDER_LPM_ATT __attribute__((aligned(4))) static uint8_t m_opus_encoder[OPUS_ENCODER_SIZE];
static OpusEncoder *const m_opus_state __attribute__((aligned(4))) = (OpusEncoder *)m_opus_encoder;


int drv_audio_codec_encode(int16_t *PCMBuffer, void *EncodeBuffer)
{
    return opus_encode(m_opus_state, PCMBuffer, CONFIG_AUDIO_FRAME_SIZE_SAMPLES, EncodeBuffer, CONFIG_AUDIO_FRAME_SIZE_BYTES);
}

void drv_audio_codec_init(void)
{
    opus_encoder_init(m_opus_state, CONFIG_AUDIO_SAMPLING_FREQUENCY, 1, OPUS_APPLICATION);

    opus_encoder_ctl(m_opus_state, OPUS_SET_BITRATE(m_opus_bitrate));
    opus_encoder_ctl(m_opus_state, OPUS_SET_VBR(m_opus_vbr));
    opus_encoder_ctl(m_opus_state, OPUS_SET_VBR_CONSTRAINT((m_opus_bitrate != OPUS_AUTO)));

    opus_encoder_ctl(m_opus_state, OPUS_SET_COMPLEXITY(m_opus_complexity));
    opus_encoder_ctl(m_opus_state, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
    opus_encoder_ctl(m_opus_state, OPUS_SET_LSB_DEPTH(16));
    opus_encoder_ctl(m_opus_state, OPUS_SET_DTX(0));
    opus_encoder_ctl(m_opus_state, OPUS_SET_INBAND_FEC(0));
    opus_encoder_ctl(m_opus_state, OPUS_SET_PACKET_LOSS_PERC(0));

    opus_encoder_ctl(m_opus_state, OPUS_SET_EXPERT_FRAME_DURATION(OPUS_FRAMESIZE_20_MS));
    opus_encoder_ctl(m_opus_state, OPUS_SET_BANDWIDTH(OPUS_BANDWIDTH_WIDEBAND));
    opus_encoder_ctl(m_opus_state, OPUS_SET_APPLICATION(OPUS_APPLICATION_RESTRICTED_LOWDELAY));
    // opus_encoder_ctl(m_opus_state, OPUS_SET_COMPLEXITY(4)); /// complexity = 4  打开此选项会增加编码
}
