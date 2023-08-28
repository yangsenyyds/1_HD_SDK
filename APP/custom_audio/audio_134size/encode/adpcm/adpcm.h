#ifndef ADPCM_H
#define ADPCM_H
#include <string.h>
#include "yc11xx.h"


typedef struct {
    uint32_t previous_predict_adpcm;
    uint16_t tableIndex;
} google_tv_audio_adpcm;

typedef struct {
    uint16_t frame_number;
    uint8_t remote_id;
    google_tv_audio_adpcm adpcmVal;
} google_tv_audio_header;


void Adpcm_FrameEncode_Restart(google_tv_audio_header *AudioHeader);
void Adpcm_FrameEncode_Google_TV_Audio(const int16_t *PCMBuffer, void *EncodeBuffer, google_tv_audio_header *AudioHeader, int32_t Len);

#endif
