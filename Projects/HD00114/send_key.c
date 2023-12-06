#include "send_key.h"
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x08, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 1
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 2
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 3
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 4

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 5
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 6
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 7

    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 8
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 9
    {0x4C, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 0
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 15

    {0x1E, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 FAE
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 ...
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 PIN +
    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 mute

    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 PIN-
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 23
    {0x0C, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 alexa
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 voice

    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 HOME
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 LIGHT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 RIGHT

    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 DISNEP 
    {0x73, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 FPT PLAY
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 OK
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 BACK

    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 SET
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 hong
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 38lv
    {0xB5, 0x00, 1, DAXIS_KEY_HANDLE}, // 39 time
    {0x7A, 0x00, 1, DAXIS_KEY_HANDLE}, // 40 ?

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 41 huang
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 42 lan
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 43 google
    {0x81, 0x00, 1, DAXIS_KEY_HANDLE}, // 44 MOVIES
    {0x5F, 0x00, 1, DAXIS_KEY_HANDLE}, // 45 ?

    {0x3E, 0x00, 1, DAXIS_KEY_HANDLE}, //  46 HOME QUAN
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 47 NETFLIX
    {0x48, 0x00, 1, DAXIS_KEY_HANDLE}, //  48 LG CHANNELS
    {0x5D, 0x00, 1, DAXIS_KEY_HANDLE}, // // 49 JVJ
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, //  50 ZANTING

    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 51 
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 52
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 53 PRIME VIDEO
    {0xAB, 0x00, 1, DAXIS_KEY_HANDLE}, // 54 guide
    {0x49, 0x00, 1, DAXIS_KEY_HANDLE}, // 55 RAKUTEN TV

    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 56 POFANG
};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 2
    {0x13}, // 4 3
    {0x14}, // 5 4

    {0x15}, // 6 5
    {0xFF}, // 7 
    {0xFF}, // 8 
    {0x16}, // 9 6
    {0x17}, // 10 7

    {0x18}, // 11 8
    {0x19}, // 12 9
    {0x4C}, // 13 LIST
    {0x10}, // 14 0
    {0xFF}, // 15 

    {0x1E}, // 16 FAE
    {0xDE}, // 17 ...
    {0x02}, // 18 VOL+
    {0x00}, // 19 PIN +
    {0x09}, // 20 mute

    {0x03}, // 21 VOL-
    {0x01}, // 22 PIN-
    {0xFF}, // 23 
    {0xDE}, // 24 alexa
    {0xDE}, // 25 voice

    {0x7C}, // 26 HOME
    {0x40}, // 27 UP
    {0x0B}, // 28 INPUT
    {0x07}, // 29 LIGHT
    {0x06}, // 30 RIGHT

    {0x31}, // 31 DISNEP 
    {0x73}, // 32 FPT PLAY
    {0x44}, // 33 OK
    {0x41}, // 34 DOWN
    {0x28}, // 35BACK

    {0x43}, // 36 SET
    {0x72}, // 37 hong
    {0x71}, // 38 lv
    {0xB5}, // 39 time
    {0x7A}, // 40 ?

    {0x63}, // 41 huang
    {0x61}, // 42 lan
    {0xDE}, // 43 google
    {0x81}, // 44 MOVIES
    {0x5F}, // 45 

    {0x3E}, // 46 HOME QUAN
    {0x56}, // 47 NETFLIX
    {0x48}, // 48 LG CHANNELS
    {0x5D}, // 49 JVJ
    {0xBA}, // 50 ZANTING

    {0xFF}, // 51 
    {0xFF}, // 52
    {0x5C}, // 53 PRIME VIDEO
    {0xAB}, // 54 guide
    {0x49}, // 55 RAKUTEN TV

    {0xB0}, // 56 POFANG
};
