#ifndef __OPUS_CODEC_DRV_H__
#define __OPUS_CODEC_DRV_H__
#include "opus_config.h"


// OPUS modes:
#define CONFIG_OPUS_MODE_CELT                   (1)
#define CONFIG_OPUS_MODE_SILK                   (2)
#define CONFIG_OPUS_MODE_HYBRID                 (CONFIG_OPUS_MODE_CELT | CONFIG_OPUS_MODE_SILK)

// <h> Opus Options
// <o> Mode
// <i> SILK mode is specifically dedicated for voice but requires more CPU and memory resources than CELT.
// <i> CELT is a general-purpose mode that is more power efficient and uses less resources.
// <i> Hybrid mode is not supported.
// <i> Note: When using Keil, use the option "Rebuild all target files" to change stack size between CELT/SILK.
// <1=>CELT Only
// <2=>SILK Only
/**@brief Opus Options: Mode */
// #define CONFIG_OPUS_MODE 1

// <o> Bit Rate
// <i> VBR - Variable Bit Rate is fully controlled by the codec.
// <i> CVBR - Constrained Variable Bit Rate is variable to some extent but allows you to set an average bit rate.
// <i> CBR - Constant Bit Rate allows you to set a specific bit rate that remains the same throughout the transmission.
//  <0=>VBR
//  <16000=>CVBR: 16 kbit/s
//  <24000=>CVBR: 24 kbit/s
//  <32000=>CVBR: 32 kbit/s
//  <40000=>CVBR: 40 kbit/s
//  <48000=>CVBR: 48 kbit/s
//  <56000=>CVBR: 56 kbit/s
//  <64000=>CVBR: 64 kbit/s
//  <80000=>CVBR: 80 kbit/s
//  <96000=>CVBR: 96 kbit/s
//  <112000=>CVBR: 112 kbit/s
//  <128000=>CVBR: 128 kbit/s
//  <16001=>CBR: 16 kbit/s
//  <24001=>CBR: 24 kbit/s
//  <32001=>CBR: 32 kbit/s
//  <40001=>CBR: 40 kbit/s
//  <48001=>CBR: 48 kbit/s
//  <56001=>CBR: 56 kbit/s
//  <64001=>CBR: 64 kbit/s
//  <80001=>CBR: 80 kbit/s
//  <96001=>CBR: 96 kbit/s
//  <112001=>CBR: 112 kbit/s
//  <128001=>CBR: 128 kbit/s
//
//  Bit rate:   (CONFIG_OPUS_BITRATE_CFG & ~0x0F)
//  Flags:      (CONFIG_OPUS_BITRATE_CFG & 0x0F)
//  Bit 0:      0 = VBR/CVBR
//              1 = CBR
/**@brief Opus Options: Bit Rate */
#define CONFIG_OPUS_BITRATE_CFG 32001  //  <32001=>CBR: 32 kbit/s

// <o> Bit Rate Limit
// <i> Set a bit rate limit that cannot be exceeded during the transmission. Must be equal or higher than the configured bit rate.
/**@brief Opus Options: Bit Rate Limit */
#define CONFIG_OPUS_BITRATE_LIMIT 40000     //40 kbit/s

// <o> Complexity <0-10>
// <i> A number from range 0-10. Higher complexity assures better quality but also higher CPU and memory resources consumption.
/**@brief Opus Options: Complexity <0-10> */
#define CONFIG_OPUS_COMPLEXITY 0

// <o> Sampling Frequency
// <i> Note that not all combinations of sampling frequency and codec are supported.
// <8000=>8 kHz
// <16000=>16 kHz
// <24000=>24 kHz
// <32000=>32 kHz
/**@brief Sampling Frequency */
#define CONFIG_AUDIO_SAMPLING_FREQUENCY 16000

//CELT supports 5 ms - 40 ms audio frames.
//SILK provides support for 10 ms - 60 ms frame sizes.
#define CONFIG_AUDIO_FRAME_SIZE_MS 20

#if (defined(CONFIG_AUDIO_FRAME_SIZE_SAMPLES) && !defined(CONFIG_AUDIO_FRAME_SIZE_MS))
# define CONFIG_AUDIO_FRAME_SIZE_MS (1000 * CONFIG_AUDIO_FRAME_SIZE_SAMPLES / CONFIG_AUDIO_SAMPLING_FREQUENCY)
#elif (defined(CONFIG_AUDIO_FRAME_SIZE_MS) && !defined(CONFIG_AUDIO_FRAME_SIZE_SAMPLES))
# define CONFIG_AUDIO_FRAME_SIZE_SAMPLES (CONFIG_AUDIO_FRAME_SIZE_MS * CONFIG_AUDIO_SAMPLING_FREQUENCY / 1000)
#else
# error "Either CONFIG_AUDIO_FRAME_SIZE_SAMPLES or CONFIG_AUDIO_FRAME_SIZE_MS has to be defined!"
#endif

# define CONFIG_AUDIO_FRAME_SIZE_BYTES      (CONFIG_OPUS_BITRATE_LIMIT * CONFIG_AUDIO_FRAME_SIZE_SAMPLES / (8 * CONFIG_AUDIO_SAMPLING_FREQUENCY))
# define CONFIG_OPUS_VBR_ENABLED            ((CONFIG_OPUS_BITRATE_CFG & 0x01) == 0x00)
# define CONFIG_OPUS_BITRATE                (CONFIG_OPUS_BITRATE_CFG & ~0x0F)
# if (CONFIG_OPUS_BITRATE > CONFIG_OPUS_BITRATE_LIMIT)
#  error "Bitrate limit cannot be lower than selected codec bitrate"
# endif


extern int drv_audio_codec_encode(int16_t *PCMBuffer, void *EncodeBuffer);
extern void drv_audio_codec_init(void);

#endif
