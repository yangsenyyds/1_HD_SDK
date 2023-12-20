#include "hd_sbc.h"
#include "sbc_encoder.h"

static SBC_ENC_PARAMS context;

void msbc_encode_process(int16_t *input_buf, uint8_t *output_buf)
{
    context.ps16PcmBuffer = input_buf;
    context.pu8Packet = output_buf;
    context.pu8Packet[0] = 0xad;
    SBC_Encoder(&context);
}

void msbc_encoder_init(void)
{
    context.s16NumOfBlocks = 15;
    context.s16NumOfSubBands = 8;
    context.s16AllocationMethod = SBC_LOUDNESS;
    context.s16BitPool = 26;
    context.s16ChannelMode = SBC_MONO;
    context.s16NumOfChannels = 1;
    context.s16SamplingFreq = SBC_sf16000;
    context.u8NumPacketToEncode = 1;
    context.FrameHeader = 0x21;
    
    SBC_Encoder_Init();
}
