#include "send_key.h"

#if (LG_Project == 5005)//rakuten tv
KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0}, // 0

    {0x0B, 0x00, 1, DAXIS_KEY_HANDLE}, // 信号源 1
    {0x08, 0x00, 1, DAXIS_KEY_HANDLE}, // 1 POWER    
    {0x40, 0x00, 1, DAXIS_KEY_HANDLE}, // UP
    {0x07, 0x00, 1, DAXIS_KEY_HANDLE}, // ZUO
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 

    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, //  6
    {0x44, 0x00, 1, DAXIS_KEY_HANDLE}, // OK
    {0x06, 0x00, 1, DAXIS_KEY_HANDLE}, // YOU
    {0x41, 0x00, 1, DAXIS_KEY_HANDLE}, // DOWN
    {0x28, 0x00, 1, DAXIS_KEY_HANDLE}, //BACK 

    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 11
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 
    {0x7C, 0x00, 1, DAXIS_KEY_HANDLE}, // HOME
    {0x8B, 0x00, 1, DAXIS_KEY_HANDLE}, // VOICE
    {0x01, 0x00, 1, DAXIS_KEY_HANDLE}, // CH-

    {0x00, 0x00, 1, DAXIS_KEY_HANDLE}, // CH+ 16
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 
    {0x03, 0x00, 1, DAXIS_KEY_HANDLE}, // VOL-
    {0x02, 0x00, 1, DAXIS_KEY_HANDLE}, // VOL+

    {0x56, 0x00, 1, DAXIS_KEY_HANDLE}, // 21  NETFLIX
    {0x5C, 0x00, 1, DAXIS_KEY_HANDLE}, //  PRIME VIDEO 
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 
    {0xFF, 0x00, 1, DAXIS_KEY_HANDLE}, // 
    {0x31, 0x00, 1, DAXIS_KEY_HANDLE}, // DISNEY

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
 uint8_t IRKeyBuf[][1] = {
    {0x00}, // 0


    {0x0B}, // 信号源 1
    {0x08}, // 1 POWER    
    {0x40}, // UP
    {0x07}, // ZUO
    {0xFF}, // 

    {0xFF}, //  6
    {0x44}, // OK
    {0x06}, // YOU
    {0x41}, // DOWN
    {0x28}, //BACK 

    {0xFF}, // 11
    {0xFF}, // 
    {0x7C}, // HOME
    {0xDE}, // VOICE
    {0x01}, // CH-

    {0x00}, // CH+ 16
    {0xFF}, // 
    {0xFF}, // 
    {0x03}, // VOL-
    {0x02}, // VOL+

    {0x56}, // 21  NETFLIX
    {0x5C}, //  PRIME VIDEO 
    {0xFF}, // 
    {0xFF}, // 
    {0x31}, // DISNEY

    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 

    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 

    {0xFF}, // 
    {0xFF}, // 
    {0xFF}, // 

};

#endif