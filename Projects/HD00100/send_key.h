#ifndef SEND_KEY_H
#define SEND_KEY_H
#include <string.h>
#include "remote_control.h"
#define LG_Project  (100)
typedef struct
{
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

extern const KeyBuf_TypeDef KeyBuf[];
extern const uint8_t IRKeyBuf[][1];
#endif
