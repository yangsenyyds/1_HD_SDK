#include "remote_control.h"
#include "yc11xx_ipc.h"
#include "yc11xx_lpm.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_bt_interface.h"
#include "hci_dtm_test.h"
#include "yc_ota.h"
#include "yc_debug.h"
#include "att.h"
#include "app_config.h"
#include "app_queue.h"
#include "app_sleep.h"
#include "software_timer.h"
#include "vbat.h"
#include "led.h"
#include "keyscan.h"
#include "audio.h"
#include "flash.h"
#include "ir_send.h"
#include "ir_load.h"
#include "ir_lib.h"

#include "yc11xx_dev_qspi.h"
#include "SecretKey.h"
#include "yc11xx_iwdg.h"
typedef struct {
    uint8_t keyvalue[2];
    uint8_t key_send_len;
    uint16_t handle;
} KeyBuf_TypeDef;

typedef struct {
    uint8_t keyvalue;
    uint8_t Ir_TypeDef;
} IRBuf_TypeDef;

enum{
    Power_Keynum = 1,
    Voice_Keynum = 24,
    Voice_Col = 2,
    Vol__Keynum = 40,
    Vol_Col = 4,

    ir_state_01 = 101,
    ir_state_C4 = 104,
    ir_state_1A = 111,
    ir_state_97 = 197,
    ir_state_A4 = 113,
	CONN_PARAM = 99,
};
#if (Project_key == 500)
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},
 
    {0x15, 0x01},//1
    {0x00, 0x01},
    {0x01, 0x01},
    {0x02, 0x01},
    {0x03, 0x01},
 
    {0x04, 0x01},//6
    {0x1C, 0x97},
    {0x05, 0x01},
    {0x06, 0x01},
    {0x07, 0x01},
 
    {0x08, 0x01},//11
    {0x1D, 0x97},
    {0X09, 0x01},
    {0X7C, 0x1A},//NETFLIX
    {0X46, 0xC4},
 
    {0X2A, 0xC4},//16
    {0X27, 0x97},
    {0X24, 0x97},
    {0X25, 0x97},
    {0X26, 0x97},
 
    {0x20, 0x97},//21
    {0x5B, 0xA4},
    {0X65, 0x1A},
    {0X00, 0x00},
    {0X25, 0x01},
 
    {0X4B, 0xC4},//26
    {0X74, 0x01},
    {0X19, 0x97},
    {0X34, 0x01},
    {0X65, 0x01},
 
    {0X33, 0x01},//31
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X3A, 0x01},
 
    {0X24, 0x01},//36
    {0X12, 0x01},
    {0X10, 0x01},
    {0X3B, 0x01},
    {0X13, 0x01},
 
    {0X14, 0x01},//41
    {0X00, 0x00},
    {0X11, 0x01},//
    {0X17, 0x01},
    {0X28, 0x97},
 
    {0X4D, 0xC4},//46
    {0X1B, 0x97},
    {0X1A, 0x97},
};

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x00, 0x00, 0,   0},//1
    {0x1e, 0x00, 8, 121},
    {0x1f, 0x00, 8, 121},
    {0x20, 0x00, 8, 121},
    {0x21, 0x00, 8, 121},

    {0x22, 0x00, 8, 121},//6
    {0xb3, 0x00, 2, 125},
    {0x23, 0x00, 8, 121},
    {0x24, 0x00, 8, 121},
    {0x25, 0x00, 8, 121},   

    {0x26, 0x00, 8, 121}, //11
    {0x37, 0x00, 8, 121}, 
    {0x27, 0x00, 8, 121},
    {0x47, 0x05, 2, 125},//netflix
    {0x4b, 0x05, 2, 125}, 

    {0x44, 0x05, 2, 125},//16
    {0x90, 0x04, 2, 125},
    {0x91, 0x04, 2, 125},
    {0x8e, 0x04, 2, 125},
    {0x8f, 0x04, 2, 125},

    {0xb2, 0x00, 2, 125},//21
    {0x8d, 0x00, 2, 125},
    {0x8b, 0x04, 2, 125},
    {0x21, 0x02, 2, 125},
    {0x33, 0x05, 2, 125},

    {0xc3, 0x03, 2, 125},//26
    {0x42, 0x00, 2, 125},
    {0xcd, 0x00, 2, 125},
    {0x44, 0x00, 2, 125},
    {0x41, 0x00, 2, 125}, 

    {0x45, 0x00, 2, 125},//31
    {0x43, 0x00, 2, 125},
    {0x24, 0x02, 2, 125},           
    {0x23, 0x02, 2, 125},
    {0x66, 0x04, 2, 125},   

    {0x89, 0x00, 2, 125},//36
    {0xe9, 0x00, 2, 125},
    {0x9c, 0x00, 2, 125},
    {0x2f, 0x05, 2, 125},
    {0xea, 0x00, 2, 125},

    {0xe2, 0x00, 2, 125},//41
    {0x00, 0x00, 2, 125},    
    {0x9d, 0x00, 2, 125},
    {0x2e, 0x05, 2, 125},
    {0x61, 0x00, 2, 125},

    {0x95, 0x00, 2, 125},
    {0xb4, 0x00, 2, 125},
    {0xb0, 0x00, 2, 125},
};
#elif (Project_key == 501)
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},
 
    {0x15, 0x01},//1
    {0x00, 0x01},
    {0x01, 0x01},
    {0x02, 0x01},
    {0x03, 0x01},
 
    {0x04, 0x01},//6
    {0x1C, 0x97},
    {0x05, 0x01},
    {0x06, 0x01},
    {0x07, 0x01},
 
    {0x08, 0x01},//11
    {0x28, 0x97},//gai
    {0X09, 0x01},
    {0X45, 0xC4},//222
    {0X44, 0xC4},//111

    {0X3A, 0x01},//16
    {0X25, 0x97},//red
    {0X26, 0x97},//green
    {0X27, 0x97},//yellow
    {0X24, 0x97},//blue
 
    {0x3C, 0x97},//21
    {0x5B, 0xA4},
    {0X2A, 0xC4},
    {0X00, 0x00},
    {0X25, 0x01},
 
    {0X4B, 0xC4},//26
    {0X74, 0x01},
    {0X19, 0x97},
    {0X34, 0x01},
    {0X65, 0x01},
 
    {0X33, 0x01},//31
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X3D, 0x97},//
 
    {0X24, 0x01},//36
    {0X12, 0x01},
    {0X10, 0x01},
    {0X3B, 0x01},
    {0X13, 0x01},
 
    {0X14, 0x01},//41
    {0X00, 0x00},
    {0X11, 0x01},//
    {0X17, 0x01},
    {0X4F, 0xC4},
 
    {0X4D, 0xC4},//46
    {0X1B, 0x97},
    {0X1A, 0x97},
};

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x00, 0x00, 0,   0},//1
    {0x1e, 0x00, 8, 121},
    {0x1f, 0x00, 8, 121},
    {0x20, 0x00, 8, 121},
    {0x21, 0x00, 8, 121},

    {0x22, 0x00, 8, 121},//6
    {0xb3, 0x00, 2, 125},
    {0x23, 0x00, 8, 121},
    {0x24, 0x00, 8, 121},
    {0x25, 0x00, 8, 121},   

    {0x26, 0x00, 8, 121}, //11
    {0x61, 0x00, 8, 121}, 
    {0x27, 0x00, 8, 121},
    {0x53, 0x05, 2, 125},//netflix
    {0x52, 0x05, 2, 125}, 

    {0x66, 0x04, 2, 125},//16
    {0x8E, 0x04, 2, 125},
    {0x8F, 0x04, 2, 125},
    {0x90, 0x04, 2, 125},
    {0x91, 0x04, 2, 125},

    {0xb6, 0x00, 2, 125},//21//
    {0x8d, 0x00, 2, 125},
    {0x44, 0x05, 2, 125},
    {0x21, 0x02, 2, 125},
    {0x33, 0x05, 2, 125},

    {0xc3, 0x03, 2, 125},//26
    {0x42, 0x00, 2, 125},
    {0xcd, 0x00, 2, 125},
    {0x44, 0x00, 2, 125},
    {0x41, 0x00, 2, 125}, 

    {0x45, 0x00, 2, 125},//31
    {0x43, 0x00, 2, 125},
    {0x24, 0x02, 2, 125},           
    {0x23, 0x02, 2, 125},
    {0xB5, 0x00, 2, 125},   

    {0x89, 0x00, 2, 125},//36
    {0xe9, 0x00, 2, 125},
    {0x9c, 0x00, 2, 125},
    {0x2f, 0x05, 2, 125},
    {0xea, 0x00, 2, 125},

    {0xe2, 0x00, 2, 125},//41
    {0x00, 0x00, 2, 125},    
    {0x9d, 0x00, 2, 125},
    {0x2e, 0x05, 2, 125},
    {0x8B, 0x03, 2, 125},

    {0x95, 0x00, 2, 125},
    {0xb4, 0x00, 2, 125},
    {0xb0, 0x00, 2, 125},
};

#elif (Project_key == 502)
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},
 
    {0x15, 0x01},//1
    {0x00, 0x01},
    {0x01, 0x01},
    {0x02, 0x01},
    {0x03, 0x01},
 
    {0x04, 0x01},//6
    {0x1C, 0x97},
    {0x05, 0x01},
    {0x06, 0x01},
    {0x07, 0x01},
 
    {0x08, 0x01},//11
    {0x1D, 0x97},
    {0X09, 0x01},
    {0X7C, 0x1A},//NETFLIX
    {0X46, 0xC4},
 
    {0X3F, 0x01},//16//CAIDAN
    {0X25, 0x97},//red
    {0X26, 0x97},//green
    {0X27, 0x97},//yellow
    {0X24, 0x97},//blue
 
    {0x63, 0x01},//21
    {0x5B, 0xA4},
    {0X2A, 0xC4},//yingyong
    {0X00, 0x00},
    {0X25, 0x01},
 
    {0X4B, 0xC4},//26
    {0X74, 0x01},
    {0X19, 0x97},
    {0X34, 0x01},
    {0X65, 0x01},
 
    {0X33, 0x01},//31
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X3A, 0x01},
 
    {0X24, 0x01},//36
    {0X12, 0x01},
    {0X10, 0x01},
    {0X3B, 0x01},
    {0X13, 0x01},
 
    {0X14, 0x01},//41
    {0X00, 0x00},
    {0X11, 0x01},//
    {0X17, 0x01},
    {0X28, 0x97},
 
    {0X4D, 0xC4},//46
    {0X1B, 0x97},
    {0X1A, 0x97},
};

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x00, 0x00, 0,   0},//1
    {0x1e, 0x00, 8, 121},
    {0x1f, 0x00, 8, 121},
    {0x20, 0x00, 8, 121},
    {0x21, 0x00, 8, 121},

    {0x22, 0x00, 8, 121},//6
    {0xb3, 0x00, 2, 125},
    {0x23, 0x00, 8, 121},
    {0x24, 0x00, 8, 121},
    {0x25, 0x00, 8, 121},   

    {0x26, 0x00, 8, 121}, //11
    {0x37, 0x00, 8, 121}, 
    {0x27, 0x00, 8, 121},
    {0x47, 0x05, 2, 125},//netflix
    {0x4b, 0x05, 2, 125}, 

    {0x84, 0x04, 2, 125},//16
    {0x8E, 0x04, 2, 125},
    {0x8F, 0x04, 2, 125},
    {0x90, 0x04, 2, 125},
    {0x91, 0x04, 2, 125},

    {0x03, 0x29, 8, 121},//21
    {0x8d, 0x00, 2, 125},
    {0x44, 0x05, 2, 125},
    {0x21, 0x02, 2, 125},
    {0x33, 0x05, 2, 125},

    {0xC3, 0x03, 2, 125},//26
    {0x42, 0x00, 2, 125},
    {0xcd, 0x00, 2, 125},
    {0x44, 0x00, 2, 125},
    {0x41, 0x00, 2, 125}, 

    {0x45, 0x00, 2, 125},//31
    {0x43, 0x00, 2, 125},
    {0x24, 0x02, 2, 125},           
    {0x23, 0x02, 2, 125},
    {0x66, 0x04, 2, 125},   

    {0x89, 0x00, 2, 125},//36
    {0xe9, 0x00, 2, 125},
    {0x9c, 0x00, 2, 125},
    {0x2f, 0x05, 2, 125},
    {0xea, 0x00, 2, 125},

    {0xe2, 0x00, 2, 125},//41
    {0x00, 0x00, 2, 125},    
    {0x9d, 0x00, 2, 125},
    {0x2e, 0x05, 2, 125},
    {0x61, 0x00, 2, 125},

    {0x95, 0x00, 2, 125},
    {0xb4, 0x00, 2, 125},
    {0xb0, 0x00, 2, 125},
};
#elif (Project_key == 503)
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},
 
    {0x15, 0x01},//1
    {0x00, 0x01},
    {0x01, 0x01},
    {0x02, 0x01},
    {0x03, 0x01},
 
    {0x04, 0x01},//6
    {0x1C, 0x97},
    {0x05, 0x01},
    {0x06, 0x01},
    {0x07, 0x01},
 
    {0x08, 0x01},//11
    {0x3A, 0x01},
    {0X09, 0x01},
    {0X7C, 0x1A},//NETFLIX
    {0X46, 0xC4},
 
    {0X3F, 0x01},//16
    {0X25, 0x97},//red
    {0X26, 0x97},//green
    {0X27, 0x97},//yellow
    {0X24, 0x97},//blue
 
    {0x20, 0x97},//21
    {0x5B, 0xA4},
    {0X65, 0x1A},
    {0X6E, 0xC4},
    {0X25, 0x01},
 
    {0X4B, 0xC4},//26
    {0X74, 0x01},
    {0X19, 0x97},
    {0X34, 0x01},
    {0X65, 0x01},
 
    {0X33, 0x01},//31
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X63, 0x01},
 
    {0X24, 0x01},//36
    {0X12, 0x01},
    {0X10, 0x01},
    {0X3B, 0x01},
    {0X13, 0x01},
 
    {0X14, 0x01},//41
    {0X00, 0x00},
    {0X11, 0x01},//
    {0X17, 0x01},
    {0X28, 0x97},
 
    {0X4D, 0xC4},//46
    {0X1B, 0x97},
    {0X1A, 0x97},
};

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x00, 0x00, 0,   0},//1
    {0x1e, 0x00, 8, 121},
    {0x1f, 0x00, 8, 121},
    {0x20, 0x00, 8, 121},
    {0x21, 0x00, 8, 121},

    {0x22, 0x00, 8, 121},//6
    {0xb3, 0x00, 2, 125},
    {0x23, 0x00, 8, 121},
    {0x24, 0x00, 8, 121},
    {0x25, 0x00, 8, 121},   

    {0x26, 0x00, 8, 121}, //11
    {0x66, 0x04, 2, 125}, 
    {0x27, 0x00, 8, 121},
    {0x47, 0x05, 2, 125},//netflix
    {0x4b, 0x05, 2, 125}, 

    {0x84, 0x04, 2, 125},//16
    {0x8E, 0x04, 2, 125},
    {0x8F, 0x04, 2, 125},
    {0x90, 0x04, 2, 125},
    {0x91, 0x04, 2, 125},

    {0xb2, 0x00, 2, 125},//21
    {0x8d, 0x00, 2, 125},
    {0x8b, 0x04, 2, 125},
    {0x21, 0x02, 2, 125},
    {0x33, 0x05, 2, 125},

    {0xc3, 0x03, 2, 125},//26
    {0x42, 0x00, 2, 125},
    {0xcd, 0x00, 2, 125},
    {0x44, 0x00, 2, 125},
    {0x41, 0x00, 2, 125}, 

    {0x45, 0x00, 2, 125},//31
    {0x43, 0x00, 2, 125},
    {0x24, 0x02, 2, 125},           
    {0x23, 0x02, 2, 125},
    {0x03, 0x29, 8, 121},   //35

    {0x89, 0x00, 2, 125},//36
    {0xe9, 0x00, 2, 125},
    {0x9c, 0x00, 2, 125},
    {0x2f, 0x05, 2, 125},
    {0xea, 0x00, 2, 125},

    {0xe2, 0x00, 2, 125},//41
    {0x00, 0x00, 2, 125},    
    {0x9d, 0x00, 2, 125},
    {0x2e, 0x05, 2, 125},
    {0x61, 0x00, 2, 125},

    {0x95, 0x00, 2, 125},
    {0xb4, 0x00, 2, 125},
    {0xb0, 0x00, 2, 125},
};
#elif (Project_key == 552)
static const IRBuf_TypeDef ir_data[] = {
    {0x00, 0x00},
 
    {0x15, 0x01},//1
    {0x00, 0x01},
    {0x01, 0x01},
    {0x02, 0x01},
    {0x03, 0x01},
 
    {0x04, 0x01},//6
    {0x1C, 0x97},
    {0x05, 0x01},
    {0x06, 0x01},
    {0x07, 0x01},
 
    {0x08, 0x01},//11
    {0x1D, 0x97},
    {0X09, 0x01},
    {0X7C, 0x1A},//NETFLIX
    {0X46, 0xC4},
 
    {0X63, 0x01},//16//CAIDAN
    {0X25, 0x97},//red
    {0X26, 0x97},//green
    {0X27, 0x97},//yellow
    {0X24, 0x97},//blue
 
    {0x3A, 0x01},//21
    {0x5B, 0xA4},
    {0X2A, 0xC4},//yingyong
    {0X00, 0x00},
    {0X25, 0x01},
 
    {0X4B, 0xC4},//26
    {0X74, 0x01},
    {0X19, 0x97},
    {0X34, 0x01},
    {0X65, 0x01},
 
    {0X33, 0x01},//31
    {0X75, 0x01},
    {0X23, 0x97},
    {0X60, 0x01},
    {0X18, 0x97},
 
    {0X24, 0x01},//36
    {0X12, 0x01},
    {0X10, 0x01},
    {0X3B, 0x01},
    {0X13, 0x01},
 
    {0X14, 0x01},//41
    {0X00, 0x00},
    {0X11, 0x01},//
    {0X17, 0x01},
    {0X28, 0x97},
 
    {0X4D, 0xC4},//46
    {0X1B, 0x97},
    {0X1A, 0x97},
};

static const KeyBuf_TypeDef KeyBuf[] = {
    {0x00, 0x00, 0, 0},

    {0x00, 0x00, 0,   0},//1
    {0x1e, 0x00, 8, 121},
    {0x1f, 0x00, 8, 121},
    {0x20, 0x00, 8, 121},
    {0x21, 0x00, 8, 121},

    {0x22, 0x00, 8, 121},//6
    {0xb3, 0x00, 2, 125},
    {0x23, 0x00, 8, 121},
    {0x24, 0x00, 8, 121},
    {0x25, 0x00, 8, 121},   

    {0x26, 0x00, 8, 121}, //11
    {0x37, 0x00, 8, 121}, 
    {0x27, 0x00, 8, 121},
    {0x47, 0x05, 2, 125},//netflix
    {0x4b, 0x05, 2, 125}, 

    {0x03, 0x29, 2, 125},//16
    {0x8E, 0x04, 2, 125},
    {0x8F, 0x04, 2, 125},
    {0x90, 0x04, 2, 125},
    {0x91, 0x04, 2, 125},

    {0x66, 0x04, 8, 121},//21
    {0x8d, 0x00, 2, 125},
    {0x44, 0x05, 2, 125},
    {0x21, 0x02, 2, 125},
    {0x33, 0x05, 2, 125},

    {0xC3, 0x03, 2, 125},//26
    {0x42, 0x00, 2, 125},
    {0xcd, 0x00, 2, 125},
    {0x44, 0x00, 2, 125},
    {0x41, 0x00, 2, 125}, 

    {0x45, 0x00, 2, 125},//31
    {0x43, 0x00, 2, 125},
    {0x24, 0x02, 2, 125},           
    {0x23, 0x02, 2, 125},
    {0x26, 0x02, 2, 125},   

    {0x89, 0x00, 2, 125},//36
    {0xe9, 0x00, 2, 125},
    {0x9c, 0x00, 2, 125},
    {0x2f, 0x05, 2, 125},
    {0xea, 0x00, 2, 125},

    {0xe2, 0x00, 2, 125},//41
    {0x00, 0x00, 2, 125},    
    {0x9d, 0x00, 2, 125},
    {0x2e, 0x05, 2, 125},
    {0x61, 0x00, 2, 125},

    {0x95, 0x00, 2, 125},
    {0xb4, 0x00, 2, 125},
    {0xb0, 0x00, 2, 125},
};


#endif
static const uint8_t MIC_CLOSE[] = {0x0D};
static const uint8_t MIC_OPEN[] = {0x0C};
static const uint8_t GET_CAPS[] = {0x0A};

static const uint8_t adv_data_buf[] = {
 0x10 ,0x09 ,0x53 ,0x4F ,0x4E ,0x59 ,0x20 
,0x54 ,0x56 ,0x20 ,0x56 ,0x52 ,0x43 ,0x20 
,0x30 ,0x30 ,0x31 ,0x03 ,0x19 ,0x80 ,0x01 
,0x02 ,0x01 ,0x05 ,0x03 ,0x03 ,0x12 ,0x18};

static const uint8_t scan_rsp_data_buf[] = {
 0x10 ,0x09 ,0x53 ,0x4F ,0x4E ,0x59 ,0x20 
,0x54 ,0x56 ,0x20 ,0x56 ,0x52 ,0x43 ,0x20 
,0x30 ,0x30 ,0x31};

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t key_pressed_num;
static bool tx_power_switch = true;
static uint8_t vioce_timernum = 0xFF;
static uint8_t key_pressed_timernum = 0xFF;
static uint8_t vioce_send_timernum = 0xFF;
static uint8_t low_power_timernum = 0xFF;
static uint16_t key_pressed_time;
static uint8_t keynum;
static uint8_t keynum_second;

static uint8_t ir_state;
static bool led_state;
static bool conn_param_state;
static uint8_t updata_connect_state;
static uint32_t sleep_time_state;
/// 0600 04 00 01 30 07
static uint8_t f_buf[] = {0x00, 0x01, 0x30, 0x09};
static uint8_t f_ack[] = {0x07, 0x00, 0x03, 0x00, 0x01, 0x31, 0x09, 0x13}; /// 第3字节为自增ack_idx++
/// 060097 00 01 30 07
static uint8_t second_buf[] = {0x00, 0x01, 0x30, 0x07};
static uint8_t secondbuf_ack[] = {0x08, 0x00, 0x00, 0x00, 0x01, 0x32, 0x07, 0xFC, 0x03}; /// 第3字节为自增ack_idx++
/// 060098 00 01 30 02
static uint8_t thr_buf[] = {0x00, 0x01, 0x30, 0x02};
static uint8_t thr_buf_ack1[] = {0x13, 0x38, 0x00, 0x00, 0x01, 0x01, 0x2E, 0x02, 0x46, 0x3B, 0x20, 0x03, 0x53, 0x4F, 0x4E, 0x59, 0x20, 0x56, 0x52, 0x43}; /// 第3字节为自增ack_idx++
static uint8_t thr_buf_ack2[] = {0x06, 0x10, 0x08, 0x00, 0x00, 0x20, 0x20}; 

static uint8_t ack_idx = 0;

static bool voice_key_state;
static bool voice_send_state;
static bool encrypt_state;
static bool dis_encrypt_state;
static uint8_t wake_up_state;
static bool SecretKey_Check(void)
{
    uint8_t adcbuf[12];
    uint8_t secretkey_Ori[16];
    uint32_t secretkey_SN_Addr = 0 ;
    uint8_t secretkey_SN[2];
    PublicKey_TypeE secretkey_Type;
    uint8_t secretkey_Gen[16];

    for (uint8_t i = 0; i < 12; i++) {
        adcbuf[i] = HREAD(mem_0_3_6v_adc_io_data + i);
    }
    QSPI_ReadFlashData(SecretKey_Addr, 16, secretkey_Ori);

    QSPI_ReadFlashData(0, 3, (uint8_t *)&secretkey_SN_Addr);
    secretkey_SN_Addr = REVERSE_3BYTE_DEFINE((secretkey_SN_Addr & 0x00ffffff)) - 4;
    QSPI_ReadFlashData(secretkey_SN_Addr, 2, secretkey_SN);
    switch ((secretkey_SN[0] << 8) + secretkey_SN[1])
    {
    case 0x1228:
        secretkey_Type = Key_1228;
        break;
    case 0x3527:
        secretkey_Type = Key_3527;
        break;
    case 0x5815:
        secretkey_Type = Key_5815;
        break;
    
    default:
        return false;
    }
    SecretKey_Generate(secretkey_Type, adcbuf, 12, secretkey_Gen);

    return (memcmp((void *)secretkey_Ori, (void *)secretkey_Gen, 16) == 0) ? true : false;
}

static void low_power_handle(void){
    if(HREADW(mem_le_slave_latency) != CONN_PARAM && bt_check_le_connected()){
        if(key_pressed_num == 0) {
            sleep_time_state++;
            if(sleep_time_state == 600){
                bt_send_le_disconnect(0x13);
                sleep_time_state = 0;
            }
        }else{
            sleep_time_state = 0;
        }
        DEBUG_LOG_STRING("low_power_handle : %d \r\n", sleep_time_state);
    }else{
        sleep_time_state = 0;
        swtimer_stop(low_power_timernum);
    }
}

static void voice_send_handle(void)
{
    if(key_pressed_num == 1 && voice_send_state == true) {
        voice_send_state = false;
        uint8_t hid_send_buf[KeyBuf[Voice_Keynum].key_send_len];
        memset(hid_send_buf,0,sizeof(hid_send_buf));
        ATT_sendNotify(KeyBuf[Voice_Keynum].handle, (void*)hid_send_buf, KeyBuf[Voice_Keynum].key_send_len);
    }
}

static void key_pressed_handle(void)
{
    if(key_pressed_num == 0 || key_pressed_num == 1)
    {
        if(keynum == Power_Keynum)
        {
            if(ir_data[keynum].Ir_TypeDef == 0x01 && ir_state_01 != ir_state ) {
                ir_state = ir_state_01;
                ir_type_init(UPD6124_D7C5_69,CUSTOM_01_00_A);
            }
            ir_comm_send(ir_data[keynum].keyvalue);
            if(key_pressed_num == 1) {
                set_key_press_state(true);
            }
        }
        else if(!bt_check_le_connected() && ir_data[keynum].Ir_TypeDef != 0x00)
        {
            if(ir_data[keynum].Ir_TypeDef == 0x97 && ir_state_97 != ir_state) {
                ir_state = ir_state_97;
                ir_type_init(UPD6124_D7C8_70,CUSTOM_97_00_A);
            }
            else if(ir_data[keynum].Ir_TypeDef == 0x01 && ir_state_01 != ir_state) {
                ir_state = ir_state_01;
                ir_type_init(UPD6124_D7C5_69,CUSTOM_01_00_A);
            }
            else if(ir_data[keynum].Ir_TypeDef == 0x1A && ir_state_C4 != ir_state) {
                ir_state = ir_state_C4;
                ir_type_init(UPD6124_D7C8_70,CUSTOM_1A_00_A);
            }
            else if(ir_data[keynum].Ir_TypeDef == 0xC4 && ir_state_1A != ir_state) {
                ir_state = ir_state_1A;
                ir_type_init(UPD6124_D7C8_70,CUSTOM_C4_00_A);
            }
            else if(ir_data[keynum].Ir_TypeDef == 0xA4 && ir_state_A4 != ir_state) {
                ir_state = ir_state_A4;
                ir_type_init(UPD6124_D7C8_70,CUSTOM_A4_00_A);
            }
            ir_comm_send(ir_data[keynum].keyvalue);
            if(key_pressed_num == 1) {
                set_key_press_state(true);
            }

        }
        
        if(key_pressed_num == 0){
            key_pressed_time = 0;
        }
    }
    else if(key_pressed_num == 2) {
        if (keynum == Voice_Keynum && keynum_second == Vol__Keynum)
        {
            key_pressed_time++;

            if(key_pressed_time == 3)
            {
                keynum_second = 0;
                key_pressed_time = 0;
                led_state = true;
                led_on(LED_1,200,60000);
                Bt_ClearRemoteDevInfo();
                Bt_ClearDeviceNvdataInfo();
                start_adv(ADV_TYPE_NOMAL, 0x10,true);
                return ;
            }else{
                swtimer_restart(key_pressed_timernum);
            }
        }
    }
}

static void keyvalue_handle(key_report_t* key_report)
{
    key_pressed_num = key_report->key_press_cnt;
    
    if (key_pressed_num == 0)
    {
        if (bt_check_le_connected() && encrypt_state && keynum != Power_Keynum)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }

            if(keynum == Voice_Keynum && voice_send_state == true) {
                voice_send_state = false;
                swtimer_stop(vioce_send_timernum);
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }else if(keynum != Voice_Keynum) {
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }
        }else {
            set_key_press_state(false);
        }
        if(wake_up_state != 0) {
            wake_up_state = 0;
        }
    }
    else if (key_pressed_num == 1)
    {
        keynum = 0;
        for(uint8_t i = 0; i < KEY_ROW_NUM; i++) {
            keynum += key_report->keynum_report_buf[i];
        }

        factory_KeyProcess(keynum==Voice_Keynum?0xff:keynum);

        DEBUG_LOG_STRING("KEY [%d][%d][%d][%d][%d][%d][%d]\r\n", key_report->keynum_report_buf[0]
        ,key_report->keynum_report_buf[1],key_report->keynum_report_buf[2],key_report->keynum_report_buf[3],key_report->keynum_report_buf[4],key_report->keynum_report_buf[5],key_report->keynum_report_buf[6]);
        if (bt_check_le_connected() && keynum != Power_Keynum && encrypt_state)
        {
            uint8_t hid_send_buf[KeyBuf[keynum].key_send_len];
            memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
            memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);

            if (KeyBuf[keynum].key_send_len == 8) {
                hid_send_buf[2] = hid_send_buf[0];
                hid_send_buf[0] = 0x00;
            }
#if (Project_key == 502)
            if(keynum == 21){
                memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);
                hid_send_buf[3] = hid_send_buf[1];
                hid_send_buf[1] = 0x00;
            }
#elif (Project_key == 503 || Project_key == 552)
            if(keynum == 35){
                memset((void*)hid_send_buf, 0, KeyBuf[keynum].key_send_len);
                memcpy((void*)hid_send_buf, (void*)KeyBuf[keynum].keyvalue, 2);
                hid_send_buf[3] = hid_send_buf[1];
                hid_send_buf[1] = 0x00;
            }
#endif            
            DEBUG_LOG_STRING("att send  [%x] [%x] [%d] \r\n",KeyBuf[keynum].keyvalue[0],KeyBuf[keynum].keyvalue[1],KeyBuf[keynum].key_send_len);

            if (keynum == Voice_Keynum && !voice_key_state) {
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
                voice_key_state = true;
                voice_send_state = true;
                voice_status.search = true;
                voice_status_change();
                app_sleep_lock_set(AUDIO_LOCK, true);

                swtimer_start(vioce_timernum, 8000, TIMER_START_ONCE);
                swtimer_start(vioce_send_timernum, 100, TIMER_START_ONCE);
                led_on(LED_1,0,0);
                
            }else if(keynum != Voice_Keynum) {
                ATT_sendNotify(KeyBuf[keynum].handle, (void*)hid_send_buf, KeyBuf[keynum].key_send_len);
            }
        }else {
            swtimer_start(key_pressed_timernum, 100, TIMER_START_ONCE);
        }
    }
    else if (key_pressed_num == 2)
    {
        if (key_report->keynum_report_buf[Voice_Col] == Voice_Keynum && key_report->keynum_report_buf[Vol_Col] == Vol__Keynum)
        {
            if(!bt_check_le_connected()){
                keynum = Voice_Keynum;
                keynum_second = Vol__Keynum;
                swtimer_start(key_pressed_timernum,1000,TIMER_START_ONCE);
            }
        }
    }   
}

void action_after_mic_close(void)
{
    led_off(LED_1);
    voice_key_state = false;
}

void action_after_led_blk(void)
{
    if (led_state) {
        led_state = false;
        stop_adv();
        Bt_SndCmdPwroff();
    }
}

static void power_handle(uint8_t batlevel)
{
    DEBUG_LOG_STRING("BAT LEVEL: %d \r\n", batlevel);
    
    if (batlevel <= 20 && tx_power_switch) {
        tx_power_switch = false;
        bt_set_tx_power(TX_POWER_5DB);
    }
    
    if (batlevel <= 5) {
        if (bt_check_le_connected()) {
            ATT_sendNotify(BAT_REPORT_HANDLE, &batlevel, 1);
        }
    }
    else {
        memcpy((void *)Att_List[BAT_ATTLIST_INDEX].dataPtr, (void *)&batlevel, 1);
    }
}

static void remote_control_reinit(void)
{
    keyscan_start();
    software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
    vbat_reinit();
    led_reinit();
    voice_report_reinit();
}

void update_conn_param(bool is_sleep)
{
    if (is_sleep) {
        if(!conn_param_state) {
            conn_param_state = true;
            swtimer_start(low_power_timernum, 100, TIMER_START_REPEAT);
        }
        bt_le_conn_updata_param_req(0x08, 0x08, CONN_PARAM, 400);
    }
    else {
        bt_le_conn_updata_param_req(0x08, 0x08, 0, 400);
    }
}

void CONN_PARAM_ACCEPTED(void)
{
    DEBUG_LOG_STRING("SLAVE LATENCY : %d \r\n", HREADW(mem_le_slave_latency));
    if (HREADW(mem_le_slave_latency) == CONN_PARAM) {
        swtimer_stop(low_power_timernum);
        if(!conn_param_state) {
            conn_param_state = true;
        }
        sleep_time_state = 0;
        bt_set_connect_lpm_period(800);
        app_sleep_lock_set(LATENCY_LOCK, true);
    }
}

void stop_adv(void)
{
    if (bt_check_le_connected()) return;
    else
    {
        if(bt_get_le_state() == BLE_DIRECT_ADV) {
            IPC_TxControlCmd(BT_CMD_STOP_DIRECT_ADV);
        }
        else {
            IPC_TxControlCmd(BT_CMD_STOP_ADV);
        }

        if(Bt_CheckIsPaired()) {
            bt_set_le_state(BLE_PAIRED);
        }
        else {
            bt_set_le_state(BLE_IDLE);
        }
    }
}

void start_adv(enum advType type, uint16_t adv_interval, bool timeout)
{
    struct bt_le_adv_param bt_adv_param;
    bt_adv_param.Type = type;
    bt_adv_param.adv_max_interval = adv_interval;


    if (bt_adv_param.Type == ADV_TYPE_NOMAL) {
        if(bt_get_le_state() == BLE_DIRECT_ADV) {
            stop_adv();
        }        
        bt_set_le_state(BLE_ADV);
        bt_start_le_adv(&bt_adv_param, adv_data_buf, sizeof(adv_data_buf));
        if (timeout) {
            app_sleep_lock_set(ADV_LOCK, true);
            app_sleep_timer_set(SHUTDOWN_TIME);
        }
    }
    else if(bt_adv_param.Type == ADV_TYPE_DIRECT) {
        bt_set_le_state(BLE_DIRECT_ADV);
        bt_start_le_adv(&bt_adv_param, NULL, 0);
        if (timeout) {
            app_sleep_lock_set(ADV_LOCK, true);            
            app_sleep_timer_set(DIRECT_ADV_TIME);
        }
    }
}

void Read_Blob_Parse(const ATT_TABLE_TYPE *table, uint16_t offset)
{
    uint8_t senddata[251] = {ATTOP_READ_BLOB_RESPONSE};
    uint16_t sendlen = ((table->dataLen - offset) >= ((HREADW(mem_mtu_size)) - 1)) ? ((HREADW(mem_mtu_size)) - 1) : (table->dataLen - offset);

    DEBUG_LOG_STRING("READ BLOB HANDLE: %d , OFFSET: %d \r\n", table->handle, offset);

    memcpy(senddata + 1, table->dataPtr + offset, sendlen);
    ATT_Send(senddata, sendlen + 1);
}

void Read_Parse(const ATT_TABLE_TYPE *table)
{
    uint8_t senddata[251] = {ATTOP_READ_RESPONSE};
    uint16_t sendlen = (table->dataLen >= (HREADW(mem_mtu_size)) - 1)? ((HREADW(mem_mtu_size)) - 1) : table->dataLen;
    
    DEBUG_LOG_STRING("READ HANDLE: %d \r\n", table->handle);
    
    memcpy((void *)&senddata[1], table->dataPtr, sendlen);
    ATT_Send(senddata, sendlen+1);
}

void Write_DataParse(const ATT_TABLE_TYPE *table, uint8_t *data, uint8_t len)
{
    DEBUG_LOG_STRING("WRITE HANDLE: %d  LEN: %d\r\n", table->handle, len);
    
    factory_WriteDataParse(table->handle, data, len);

    if (table->handle == 261) /// 2141E102
    {
        if (!memcmp(&data[3], f_buf, sizeof(f_buf)))
        {
            f_ack[2] = ack_idx++;
            ATT_sendNotify(258,f_ack, sizeof(f_ack)); /// 2141E101
        }

        if (!memcmp(&data[3], second_buf, sizeof(second_buf)))
        {
            secondbuf_ack[2] = ack_idx++;
            ATT_sendNotify(258,secondbuf_ack, sizeof(secondbuf_ack)); /// 2141E101
        }

        if (!memcmp(thr_buf, &data[3], sizeof(thr_buf)))
        {
            thr_buf_ack1[2] = ack_idx++;
            ATT_sendNotify(258, thr_buf_ack1, sizeof(thr_buf_ack1));
            thr_buf_ack2[2] = ack_idx - 1;
            ATT_sendNotify(258,thr_buf_ack2, sizeof(thr_buf_ack2));
        }
    }
    else if (table->handle == AUDIO_CMD_HANDLE)
    {
        if(!memcmp(MIC_OPEN, data, 1) && voice_key_state == false) {
            DEBUG_LOG_STRING("keep voice open\r\n");
            uint8_t open_error[3] = {0x0c,0X0f,0x01};
            ATT_sendNotify(AUDIO_CTRL_HANDLE, (void*)open_error, 3);
        }
        else
        {
            if (!memcmp(GET_CAPS, data, 1)) {
                voice_status.get_caps = true;
                voice_status_change();
            }
            else if (!memcmp(MIC_OPEN, data, 1)) {
                voice_status.mic_open = true;
                voice_status_change();
                swtimer_stop(vioce_timernum);
            }
            else if (!memcmp(MIC_CLOSE, data, 1)) {
                voice_status.mic_close = true;
                voice_status_change();
            }
        }
    }
    else if (table->handle == OTA_WRITE_HANDLE)
    {
        if (app_sleep_check()) {
            software_timer_start(SYSTEM_CURRENT_CLOCK, TIMER_UNIT_MS);
        }
        app_sleep_lock_set(APP_LOCK, true);
        app_sleep_lock_set(ADV_LOCK,false);
        OS_ENTER_CRITICAL();
        if(OTA_EVT_HANDLE(len, data) != OTA_NO_ERROR) {
            OTA_RESET();
            app_sleep_lock_set(APP_LOCK, false);
        }
        OS_EXIT_CRITICAL();
    }
    else if (table->dataLen >= len) {
        memcpy((void *)table->dataPtr, (void *)data, len);
    }
    else {
        DEBUG_LOG_STRING("ATT WRITE DATA ERROR, HANDLE: %d, WRITE LEN: %d \r\n", table->handle, len);
    }
}

void LE_LTK_LOST(void)
{
    DEBUG_LOG_BT("BT_EVT_LE_LTK_LOST \r\n");
    if(bt_check_le_connected()) {
        bt_send_le_disconnect(0x06);
        Bt_ClearRemoteDevInfo();
    }
    Bt_ClearDeviceNvdataInfo();
    start_adv(ADV_TYPE_NOMAL, 0x30, true);
}

void ENCRYPT_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("ENCRYPT FAIL! REASON : %x \r\n", reason);
}

void ENCRYPT_DONE(void)
{
    DEBUG_LOG_STRING("ENCRYPT_DONE \r\n");
    app_sleep_timer_set(ENCRYPT_DONE_DELAY);
    set_key_press_state(false);
    update_conn_param(false);
    encrypt_state = true;
    dis_encrypt_state = false;
    led_on(LED_1,200,1200);

}

void PAIR_FAIL(uint8_t reason)
{
    DEBUG_LOG_STRING("PAIR FAIL! REASON : %x \r\n", reason);
}

void PAIR_DONE(void)
{
    DEBUG_LOG_STRING("PAIR_DONE \r\n");
    app_sleep_timer_set(PAIR_DONE_DELAY);
}

void LE_DISCONNECTED(uint8_t reason)
{
    DEBUG_LOG_STRING("LE DISCONNECTED! REASON : %x \r\n", reason);
    encrypt_state = false;
    conn_param_state = false;

    bt_set_tx_power(TX_POWER_5DB);
    Bt_HciFifo_Init();
    app_sleep_lock_set(LATENCY_LOCK, false);
    bt_set_le_state(BLE_IDLE);
    
    if(voice_key_state){
        led_off(LED_NUM);
        voice_key_state = false;
        swtimer_stop(vioce_send_timernum);
        swtimer_stop(vioce_timernum);
    }    
    System_ChangeXtal24M();
    if (reason == 0x13 || reason == 0x16 || dis_encrypt_state) {
        DEBUG_LOG_STRING("dis_encrypt_state %d \r\n",dis_encrypt_state);
        enter_deep_sleep();
        return;
    }
    else if(!dis_encrypt_state && Bt_CheckIsPaired()){
        DEBUG_LOG_STRING("disconnect dis_encrypt_state %d \r\n",dis_encrypt_state);
        dis_encrypt_state = true;
    }

    app_queue_reset();
    remote_control_reinit();
    
    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x10,true);
    }
}

void LE_CONNECTED(void)
{
    DEBUG_LOG_STRING("LE CONNECTED \r\n");
    led_state =false;
    led_off(LED_1);

    bt_set_lpm_overhead_wake_time(0x10);
    bt_set_le_state(BLE_CONNECTED);
    bt_disable_le_tx_md();
	if(bt_check_save_connect_info()){
        Bt_SaveBleDeviceInfoToFlash(BLE_CONNECT_ADDR);
    }
}

void Dev_PowerOn(void)
{
    DEBUG_LOG_STRING("DEVICE POWER ON \r\n");

    IPC_TxControlCmd(BT_CMD_FIRST_POWER_ON);         
    
    Flash_LoadFlashRecord();
    Bt_ReadAddrFromFlash();

    if (!Lpm_GetWakeFlag())
    {
        bt_renew_scan_rsp((void *)scan_rsp_data_buf, sizeof(scan_rsp_data_buf));
    }

    if (Bt_CheckIsPaired()) {
        start_adv(ADV_TYPE_DIRECT, 0x08, true);
    }
    else {
        start_adv(ADV_TYPE_NOMAL, 0x10, true);
    }
}

void tx_power_switch_set(bool switch_enable)
{
    tx_power_switch = switch_enable;
}

void app_task_run(void)
{
    if (tx_power_switch) {
        bt_dynamic_switch_power();
    }
    app_queue_task();
}

void app_init(void)
{
    if (!Lpm_GetWakeFlag())
    {
        bt_set_ce_length_num(0x0F);//小包用  大包用0x08
        software_timer_start(SYSTEM_CURRENT_CLOCK, 10);
        app_sleep_init();
        vbat_init(power_handle);
        led_init();
        voice_report_init();
        ir_init(UPD6121F_LIAN_66, CUSTOM_01_00_A);
        keyscan_init(KEY_MODE_SINGLE, keyvalue_handle);
        
        key_pressed_timernum = swtimer_add(key_pressed_handle);
        vioce_timernum = swtimer_add(mic_close);
        vioce_send_timernum = swtimer_add(voice_send_handle);
		low_power_timernum = swtimer_add(low_power_handle);
        if (!SecretKey_Check())
        {
#ifdef SecretKey_Check_enable
            DEBUG_LOG_STRING("SecretKey_Check fail \r\n");
#ifdef FUNCTION_WATCH_DOG
            IWDG_Disable(WDT);   //bt watch dog     
            IWDG_Disable(WDT2);  //riscv watch dog
#endif
            while(1)
            {
                led_on(LED_1, 0, 0);
                SysTick_DelayMs(100);
                led_off(LED_1);
                SysTick_DelayMs(100);
            }
#endif
        }

        DEBUG_LOG_STRING("APP INIT DONE \r\n");
    }
    else
    {
        app_queue_reset();

        if (key_wakeup_get())
        {
            wake_up_state++;
            if(wake_up_state == 100){
                prepare_before_sleep();
                enter_deep_sleep();                
            }
            sleep_time_state = 0;
            remote_control_reinit();
            DEBUG_LOG_STRING("WAKE UP %d\r\n" ,wake_up_state);

        }
        else 
        {
#ifdef SLEEP_ONE_HOUR
            sleep_time_state++;
            if(sleep_time_state > 3800){
                bt_send_le_disconnect(0x13);
            }
#endif            
            enter_low_sleep();
            DEBUG_LOG_STRING("KEEP CONNECT\r\n");
        }
    }
}
