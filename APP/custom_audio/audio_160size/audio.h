#ifndef AUDIO_H
#define AUDIO_H
#include <string.h>
#include "yc11xx.h"

#include "remote_control.h"


#ifdef AUDIO_TEST_MODE
typedef enum {
    STANDARD_DATA,
    PCM_DATA,
    ENCODE_DATA,
}AUDIO_TSETMODE_TpyeDef;
#endif

typedef struct VOICE_STATUS {
#ifdef AUDIO_TEST_MODE
    AUDIO_TSETMODE_TpyeDef mode;
#endif
#if ADPCM
    bool search;
    bool get_caps;
    bool mic_open;
    bool mic_close;
#endif
    bool mic_open_flag;
#if MSBC
    uint8_t packet_cnt;
#endif
    uint16_t loss_cnt;
}voice_status_t;




extern voice_status_t voice_status;

#ifdef ADPCM
extern void voice_status_change(void);
#endif
extern void mic_close(void);
extern void mic_open(void);
extern void voice_report_reinit(void);
extern uint8_t voice_report_init(void);

#endif
