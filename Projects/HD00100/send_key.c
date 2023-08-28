#include "send_key.h"

#if (LG_Project == 100)//rakuten tv
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0xab, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 PRIME VIDEO
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 disney

    {0x49, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 rakuten tv
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 AMAZON


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0xab}, // 11 'LIST'
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x56}, // 33 NETFLIX
    {0x5C}, // 34 PRIME VIDEO
    {0x31}, // 35 DISNEY

    {0x49}, // 36 rakuten tv
    {0xDE}, // 37 GOOGLE
    {0xDE}, // 38 OPUS

};
#elif (LG_Project == 101)//lg channel
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0xab, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 PRIME VIDEO
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 disney

    {0x48, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 LG CHANNEL
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 AMAZON


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0xab}, // 11 'LIST'
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x56}, // 33 NETFLIX
    {0x5C}, // 34 PRIME VIDEO
    {0x31}, // 35 DISNEY

    {0x48}, // 36 lg  channel
    {0xDE}, // 37 GOOGLE
    {0xDE}, // 38 OPUS

};
#elif (LG_Project == 102)//FAV ?
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0x53, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x3E, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 HOME
    {0x81, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 MOVIES
    {0x7A, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 ?

    {0x1E, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 FAV
    {0xB0, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 ZANTING
    {0xBA, 0x00, 1, DAXIS_KEY_HANDLE}, // 37||


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0x53}, // 11 'LIST'
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x3E}, // 33 HOME
    {0x81}, // 34 MOVIES
    {0x7A}, // 35 ?

    {0x1E}, // 36 FAV
    {0xB0}, // 37 ZANTING
    {0xBA}, // 38 |

};
#elif (LG_Project == 103)//-//LIST
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0x4C, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 -//LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 PRIME VIDEO
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 disney

    {0x48, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 LG CHANNEL
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 AMAZON


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0x4C}, // 11 -//LIST
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x56}, // 33 NETFLIX
    {0x5C}, // 34 PRIME VIDEO
    {0x31}, // 35 DISNEY

    {0x48}, // 36 lg  channel
    {0xDE}, // 37 GOOGLE
    {0xDE}, // 38 OPUS

};
#elif (LG_Project == 104)//rakuten tv
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0x53, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 LIST ===
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 PRIME VIDEO
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 disney

    {0x49, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 rakuten tv
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 AMAZON


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0x53}, // 11 LIST ===
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x56}, // 33 NETFLIX
    {0x5C}, // 34 PRIME VIDEO
    {0x31}, // 35 DISNEY

    {0x49}, // 36 rakuten tv
    {0xDE}, // 37 GOOGLE
    {0xDE}, // 38 OPUS

};

#elif (LG_Project == 105)//
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0x4c, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 LIST
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 PRIME VIDEO
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 disney

    {0x81, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 MOVIES
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 AMAZON


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0x4c}, // 11 'LIST'
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x56}, // 33 NETFLIX
    {0x5C}, // 34 PRIME VIDEO
    {0x31}, // 35 DISNEY

    {0x81}, // 36 MOVIES
    {0xDE}, // 37 GOOGLE
    {0xDE}, // 38 OPUS

};
#elif (LG_Project == 106)//fpt play
const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER
    {0x11, 0x00, 1, DAXIS_KEY_HANDLE}, // 2 '1'
    {0x12, 0x00, 1, DAXIS_KEY_HANDLE}, // 3 '2'
    {0x13, 0x00, 1, DAXIS_KEY_HANDLE}, // 4 '3'
    {0x14, 0x00, 1, DAXIS_KEY_HANDLE}, // 5 '4'

    {0x15, 0x00, 1, DAXIS_KEY_HANDLE}, // 6 '5'
    {0x16, 0x00, 1, DAXIS_KEY_HANDLE}, // 7 '6'
    {0x17, 0x00, 1, DAXIS_KEY_HANDLE}, // 8 '7'
    {0x18, 0x00, 1, DAXIS_KEY_HANDLE}, // 9 '8'
    {0x19, 0x00, 1, DAXIS_KEY_HANDLE}, // 10 '9'

    {0xab, 0x00, 1, DAXIS_KEY_HANDLE}, // 11 guide //-
    {0x10, 0x00, 1, DAXIS_KEY_HANDLE}, // 12 '0'
    {0x45, 0x00, 1, DAXIS_KEY_HANDLE}, // 13 '...'
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // 14 VOL+
    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // 15 CH+

    {0x09, 0x00, 1, DAXIS_KEY_HANDLE}, // 16 MUTE
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // 17 VOL-
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // 18 CH-    
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // 19 VOICE
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // 20 HOME

    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // 21 UP
    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 22 INPUT
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // 23 LEFT
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // 24 RIGHT
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // 25 OK -->PAIR 83

    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // 26 DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, // 27 BACK
    {0x43, 0x00, 1, DAXIS_KEY_HANDLE}, // 28 SETTING
    {0x72, 0x00, 1, DAXIS_KEY_HANDLE}, // 29 RED
    {0x71, 0x00, 1, DAXIS_KEY_HANDLE}, // 30 GREEN

    {0x63, 0x00, 1, DAXIS_KEY_HANDLE}, // 31 YELLOW
    {0x61, 0x00, 1, DAXIS_KEY_HANDLE}, // 32 BLUE
    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 33 NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, // 34 PRIME VIDEO
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // 35 disney

    {0x73, 0x00, 1, DAXIS_KEY_HANDLE}, // 36 fpt play
    {0x05, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 PLAY
    {0x0c, 0x00, 1, DAXIS_KEY_HANDLE}, // 37 AMAZON


};
const uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0

    {0x08}, // 1 POWER
    {0x11}, // 2 1
    {0x12}, // 3 '2'
    {0x13}, // 4 '3'
    {0x14}, // 5 '4'

    {0x15}, // 6 '5'
    {0x16}, // 7 '6'
    {0x17}, // 8 '7'
    {0x18}, // 9 '8'
    {0x19}, // 10 '9'

    {0xab}, // 11 'LIST'
    {0x10}, // 12 0
    {0xDE}, // 13 '...'
    {0x02}, // 14 VOL+
    {0x00}, // 15 CH+

    {0x09}, // 16 MUTE
    {0x03}, // 17 VOL-
    {0x01}, // 18 CH-
    {0xDE}, // 19 VOICE
    {0x7C}, // 20 HOME

    {0x40}, // 21 UP
    {0x0B}, // 22 SETTING
    {0x07}, // 23 LEFT
    {0x06}, // 24 RIGHT
    {0x44}, // 25 OK

    {0x41}, // 26 DOWN
    {0x28}, // 27 BACK
    {0x43}, // 28 SET
    {0x72}, // 29 RED
    {0x71}, // 30 GREEN

    {0x63}, // 31 YELLOW
    {0x61}, // 32 BLUE
    {0x56}, // 33 NETFLIX
    {0x5C}, // 34 PRIME VIDEO
    {0x31}, // 35 DISNEY

    {0x73}, // 36 fpt play
    {0xDE}, // 37 GOOGLE
    {0xDE}, // 38 OPUS

};

#endif