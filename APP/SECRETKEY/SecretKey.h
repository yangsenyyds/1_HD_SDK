#ifndef _SECRETKEY_
#define _SECRETKEY_

typedef enum {
    Key_1228,
    Key_3527,
    Key_5815,
}PublicKey_TypeE;

extern unsigned int SecretKey_Generate(PublicKey_TypeE type, unsigned char *adcbuf, unsigned int adcbuf_len, unsigned char *secretkey);

#endif
