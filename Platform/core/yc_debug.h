#ifndef _YC_DEBUG_H_
#define _YC_DEBUG_H_
#include <stdarg.h>
#include <string.h>
#include "yc11xx.h"
#include "app_config.h"

typedef struct {
    uint32_t *log_bug_ptr;
    volatile uint8_t *log_initial_flag_ptr;
    // use Peterson's Algorithm
    volatile uint8_t *log_lock_flag_cm0_ptr;
    volatile uint8_t *log_lock_flag_respin_ptr;
    volatile uint8_t *log_lock_victim_ptr;
    volatile uint16_t *log_read_index_ptr;
    volatile uint16_t *log_write_index_ptr;
}YC_DEBUG_MANAGER;

/**
  *@brief Debug log buffer address.
  */
#define DEBUG_LOG_BUF_START_ADDR 				(reg_map(HREADADDR3(mem_param_debug_log_buffer_start_addr)))
#define DEBUG_LOG_BUF_END_ADDR 				    (reg_map(HREADADDR3(mem_param_debug_log_buffer_end_addr)))
#define DEBUG_LOG_BUF_INITIAL_FLAG_ADDR			(reg_map(HREADADDR3(mem_param_debug_log_initial_flag_addr)))
#define DEBUG_LOG_BUF_LOCK_FLAG_CM0_ADDR		(reg_map(HREADADDR3(mem_param_debug_log_lock_flag_cm0_addr)))
#define DEBUG_LOG_BUF_LOCK_FLAG_RESPIN_ADDR	    (reg_map(HREADADDR3(mem_param_debug_log_lock_flag_respin_addr)))
#define DEBUG_LOG_BUF_LOCK_VICTIM_ADDR			(reg_map(HREADADDR3(mem_param_debug_log_lock_victim_addr)))
#define DEBUG_LOG_BUF_READ_INDEX_ADDR			(reg_map(HREADADDR3(mem_param_debug_log_read_index_addr)))
#define DEBUG_LOG_BUF_WRITE_INDEX_ADDR			(reg_map(HREADADDR3(mem_param_debug_log_write_index_addr)))



#define LOG_PROTECT_OVERFLOW

// Macro Protocol
#define DEBUG_PROTOCL_TYPE_LOG				(0x00)   // The basic log type
#define DEBUG_PROTOCL_TYPE_ROUND_BUFFER	(0x01)   // 
#define DEBUG_PROTOCL_TYPE_STRING	(0x02)   // 
#define DEBUG_PROTOCL_TYPE_PROTOCOL	(0x03)   // 

#define DEBUG_PROTOCL_TYPE_HCI_H4	(0x80)   // 



#define DEBUG_PROTOCL_PAYLOAD_START_OFFSET 	    (3)
#define DEBUG_PROTOCL_HEADER_SIZE                            (4)
#define DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH      (50)
#define DEBUG_PROTOCL_PAYLOAD_ALLOW_LENGTH  (DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH-DEBUG_PROTOCL_HEADER_SIZE)

#define DEBUG_STRING_LOG_SINGLE_LINE_MAX_SIZE    (200)

#define DEBUG_PROTOCL_BIG_PAYLOAD_START_OFFSET   (4)
#define DEBUG_PROTOCL_BIG_HEADER_SIZE                            (5)

// Macro Log
#define LOG_BUFFER_SIZE  ((DEBUG_LOG_BUF_END_ADDR-DEBUG_LOG_BUF_START_ADDR)/4)
#define LOG_LEVEL_NORMAL    (0)
#define LOG_LEVEL_INFORM    (1)
#define LOG_LEVEL_CORE        (2)
#define LOG_LEVEL_ERROR     (3)

typedef void (*YC_DebugStringOut_CB)(const uint8_t *buf, int len);

int print_format_string(char *str, size_t size, char *format, va_list vaArgP);


#ifdef FUNCTION_CONTROL_DEBUG_ENABLE
void write_long_value(uint8_t *addr, uint32_t value);
void print_round_buffer(uint8_t index, uint32_t start_addr, uint32_t end_addr, uint32_t write_addr, uint32_t read_addr);
void uart_txBuffer(uint8_t *send_buf, uint32_t length);
void uart_txChar(unsigned char c);
void init_debugUart(void);
void debugUart_pollTx(void);
void yichip_debug_uart_protocol(uint8_t type, uint8_t length
                                , uint8_t *uart_send_buf, uint8_t *uart_send_length_ptr);
void init_print_log(void);
bool print_log_buffer_is_empty(void);
void print_log_to_uart(void);
void print_log(uint16_t flag, uint16_t value);
void print_log_get_lock(void);
void print_log_free_lock(void);
void print_string_log(char *format, ...);
void print_protocol_to_uart(uint16_t logLevel, uint16_t logType, uint8_t *buf, int len);
void print_hci_h4_to_uart(uint8_t isRcv, uint8_t *buf, int len);
void print_hci_h4_to_uart_raw(uint8_t isRcv, uint8_t type, uint8_t* buf, int len);
#define DEBUG_INIT()	init_debugUart()
#define DEBUG_LOG(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE)  /*print_log(FLAG,VALUE)*/
#define DEBUG_LOG_STRING  print_string_log
#define DEBUG_LOG_2(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1)  \
	print_log(FLAG,(((VALUE_0) & 0xff)<<8)|(((VALUE_1) & 0xff)<<0))
#define DEBUG_LOG_4(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3) \
	print_log(FLAG,(((VALUE_0) & 0x0f)<<12)|(((VALUE_1) & 0x0f)<<8)|(((VALUE_2) & 0x0f)<<4)|(((VALUE_3) & 0x0f)<<0))
#define DEBUG_LOG_8(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3,VALUE_4,VALUE_5,VALUE_6,VALUE_7)\
	print_log(FLAG,(((VALUE_0) & 0x03)<<14)|(((VALUE_1) & 0x03)<<12)|(((VALUE_2) & 0x03)<<10)|(((VALUE_3) & 0x03)<<8)|(((VALUE_4) & 0x03)<<6)|(((VALUE_5) & 0x03)<<4)|(((VALUE_6) & 0x03)<<2)|(((VALUE_3) & 0x07)<<0))
#define DEBUG_POLLING_PRINT() debugUart_pollTx()
#define DEBUG_LOG_BUFFER_IS_EMPTY() (print_log_buffer_is_empty())
#define DEBUG_LOG_PRINT_ROUND_BUFFER(INDEX, START, END, WRITE, READ) (print_round_buffer(INDEX, START, END, WRITE, READ))
#define DEBUG_LOG_PRINT_PROTOCOL(LEVEL, TYPE, BUFFER, LEN) (print_protocol_to_uart(LEVEL, TYPE, BUFFER, LEN))
#define DEBUG_LOG_PRINT_HCI_H4(RCV, BUFFER, LEN) (print_hci_h4_to_uart(RCV, BUFFER, LEN))
#else

void print_string_empty_fuction(char *format, ...);

#define DEBUG_INIT() {while(0);}
#define DEBUG_LOG_STRING print_string_empty_fuction
#define DEBUG_LOG(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE) {while(0);}
#define DEBUG_LOG_2(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1)  {while(0);}
#define DEBUG_LOG_4(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3) {while(0);}
#define DEBUG_LOG_8(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3,VALUE_4,VALUE_5,VALUE_6,VALUE_7) {while(0);}
#define DEBUG_POLLING_PRINT() {while(0);}
#define DEBUG_LOG_BUFFER_IS_EMPTY() (true)
#define DEBUG_LOG_PRINT_ROUND_BUFFER(INDEX, START, END, WRITE, READ) {while(0);}
#define DEBUG_LOG_PRINT_PROTOCOL(LEVEL, TYPE, BUFFER, LEN) {while(0);}
#define DEBUG_LOG_PRINT_HCI_H4(RCV, BUFFER, LEN) {while(0);}
#define DEBUG_LOG_PRINT_HCI_H4_RAW(RCV, TYPE, BUFFER, LEN) {while(0);}
#endif

// Debug Log Point Define, we should use log function by those macro
// To judge use, in respin, we only allow use point 0x1000-0x8FFF
// To judge use, in cm0, we only allow use point 0x9000-0xEFFF
// 0xF000-0xFFFF reserve for some special use case
enum
{
    // For IPC info
    LOG_POINT_9000 = 0x9000,
    LOG_POINT_9001 = 0x9001,
    LOG_POINT_9002 = 0x9002,
    LOG_POINT_9003 = 0x9003,
    LOG_POINT_9004 = 0x9004,
    LOG_POINT_9005 = 0x9005,
    LOG_POINT_9006 = 0x9006,
    LOG_POINT_9007 = 0x9007,
    LOG_POINT_9008 = 0x9008,
    LOG_POINT_9009 = 0x9009,
    LOG_POINT_900A = 0x900A,
    LOG_POINT_900B = 0x900B,
    LOG_POINT_900C = 0x900C,
    LOG_POINT_900D = 0x900D,
    LOG_POINT_900E = 0x900E,
    LOG_POINT_900F = 0x900F,

    LOG_POINT_9010 = 0x9010,
    LOG_POINT_9011 = 0x9011,
    LOG_POINT_9012 = 0x9012,
    LOG_POINT_9013 = 0x9013,
    LOG_POINT_9014 = 0x9014,
    LOG_POINT_9015 = 0x9015,
    LOG_POINT_9016 = 0x9016,
    LOG_POINT_9017 = 0x9017,
    LOG_POINT_9018 = 0x9018,
    LOG_POINT_9019 = 0x9019,
    LOG_POINT_901A = 0x901A,
    LOG_POINT_901B = 0x901B,
    LOG_POINT_901C = 0x901C,
    LOG_POINT_901D = 0x901D,
    LOG_POINT_901E = 0x901E,
    LOG_POINT_901F = 0x901F,

    LOG_POINT_9020 = 0x9020,
    LOG_POINT_9021 = 0x9021,
    LOG_POINT_9022 = 0x9022,


    LOG_POINT_9028 = 0x9028,
    LOG_POINT_9029 = 0x9029,

    LOG_POINT_9030 = 0x9030,
    LOG_POINT_9031 = 0x9031,
    LOG_POINT_9032 = 0x9032,
    LOG_POINT_9033 = 0x9033,

    // For UI info
    // Voice
    LOG_POINT_A000 = 0xA000,
    LOG_POINT_A001 = 0xA001,
    LOG_POINT_A002 = 0xA002,
    LOG_POINT_A003 = 0xA003,
    LOG_POINT_A004 = 0xA004,
    LOG_POINT_A005 = 0xA005,
    LOG_POINT_A006 = 0xA006,
    LOG_POINT_A007 = 0xA007,
    LOG_POINT_A008 = 0xA008,
    LOG_POINT_A009 = 0xA009,
    LOG_POINT_A00A = 0xA00A,
    LOG_POINT_A00B = 0xA00B,
    LOG_POINT_A00C = 0xA00C,
    LOG_POINT_A00D = 0xA00D,
    LOG_POINT_A00E = 0xA00E,
    LOG_POINT_A00F = 0xA00F,
    // Led
    LOG_POINT_A100 = 0xA100,
    LOG_POINT_A101 = 0xA101,
    LOG_POINT_A102 = 0xA102,
    // Battery
    LOG_POINT_A200 = 0xA200,
    LOG_POINT_A201 = 0xA201,
    LOG_POINT_A202 = 0xA202,
    LOG_POINT_A203 = 0xA203,
    LOG_POINT_A204 = 0xA204,
    LOG_POINT_A205 = 0xA205,
    LOG_POINT_A206 = 0xA206,
    LOG_POINT_A207 = 0xA207,
    LOG_POINT_A208 = 0xA208,
    LOG_POINT_A209 = 0xA209,
    LOG_POINT_A20A = 0xA20A,
    LOG_POINT_A20B = 0xA20B,

    LOG_POINT_A210 = 0xA210,
    LOG_POINT_A211 = 0xA211,
    LOG_POINT_A212 = 0xA212,
    LOG_POINT_A213 = 0xA213,
    LOG_POINT_A214 = 0xA214,
    LOG_POINT_A215 = 0xA215,
    LOG_POINT_A216 = 0xA216,
    LOG_POINT_A217 = 0xA217,
    LOG_POINT_A218 = 0xA218,
    LOG_POINT_A219 = 0xA219,
    LOG_POINT_A21A = 0xA21A,
    // Key
    LOG_POINT_A300 = 0xA300,
    LOG_POINT_A301 = 0xA301,
    LOG_POINT_A302 = 0xA302,
    // BLE ADV
    LOG_POINT_A400 = 0xA400,
    // Timer
    LOG_POINT_A500 = 0xA500,
    LOG_POINT_A501 = 0xA501,
    LOG_POINT_A502 = 0xA502,
    LOG_POINT_A503 = 0xA503,
    LOG_POINT_A504 = 0xA504,
    LOG_POINT_A505 = 0xA505,

    // Audio
    LOG_POINT_A600 = 0xA600,
    LOG_POINT_A601 = 0xA601,
    LOG_POINT_A602 = 0xA602,
    LOG_POINT_A603 = 0xA603,
    LOG_POINT_A604 = 0xA604,
    LOG_POINT_A605 = 0xA605,
    LOG_POINT_A606 = 0xA606,
    LOG_POINT_A607 = 0xA607,
    LOG_POINT_A608 = 0xA608,
    LOG_POINT_A609 = 0xA609,
    LOG_POINT_A60A = 0xA60A,
    LOG_POINT_A60B = 0xA60B,
    LOG_POINT_A60C = 0xA60C,
    LOG_POINT_A60D = 0xA60D,
    LOG_POINT_A60E = 0xA60E,
    LOG_POINT_A60F = 0xA60F,

    LOG_POINT_A610 = 0xA610,
    LOG_POINT_A611 = 0xA611,
    LOG_POINT_A612 = 0xA612,
    LOG_POINT_A613 = 0xA613,

    LOG_POINT_A620 = 0xA620,
    LOG_POINT_A621 = 0xA621,
    LOG_POINT_A622 = 0xA622,
    LOG_POINT_A623 = 0xA623,
    LOG_POINT_A624 = 0xA624,

    LOG_POINT_A630 = 0xA630,
    LOG_POINT_A631 = 0xA631,
    LOG_POINT_A632 = 0xA632,
    LOG_POINT_A633 = 0xA633,
    LOG_POINT_A634 = 0xA634,
    LOG_POINT_A635 = 0xA635,

    LOG_POINT_A640 = 0xA640,
    LOG_POINT_A641 = 0xA641,
    LOG_POINT_A642 = 0xA642,
    LOG_POINT_A643 = 0xA643,
    LOG_POINT_A644 = 0xA644,

    LOG_POINT_A650 = 0xA650,
    LOG_POINT_A651 = 0xA651,
    LOG_POINT_A652 = 0xA652,
    LOG_POINT_A653 = 0xA653,
    LOG_POINT_A654 = 0xA654,
    //charge uart
    LOG_POINT_A700 = 0xA700,
    LOG_POINT_A701 = 0xA701,
    LOG_POINT_A702 = 0xA702,
    LOG_POINT_A703 = 0xA703,
    LOG_POINT_A704 = 0xA704,
    LOG_POINT_A705 = 0xA705,
    LOG_POINT_A706 = 0xA706,
    LOG_POINT_A707 = 0xA707,
    LOG_POINT_A708 = 0xA708,
    LOG_POINT_A709 = 0xA709,
    LOG_POINT_A70A = 0xA70a,
    LOG_POINT_A70B = 0xA70b,
    LOG_POINT_A70C = 0xA70c,
    LOG_POINT_A70D = 0xA70d,
    LOG_POINT_A70E = 0xA70e,
    LOG_POINT_A70F = 0xA70f,
    LOG_POINT_A710 = 0xA710,
    LOG_POINT_A711 = 0xA711,
    LOG_POINT_A712 = 0xA712,
    LOG_POINT_A713 = 0xA713,
    LOG_POINT_A714 = 0xA714,
    // mSBC
    LOG_POINT_A800 = 0xA800,
    LOG_POINT_A801 = 0xA801,
    LOG_POINT_A802 = 0xA802,
    LOG_POINT_A803 = 0xA803,
    LOG_POINT_A804 = 0xA804,
    LOG_POINT_A805 = 0xA805,
    LOG_POINT_A806 = 0xA806,
    LOG_POINT_A807 = 0xA807,
    // TWS
    // Audio sync
    LOG_POINT_B000 = 0xB000,
    LOG_POINT_B001 = 0xB001,
    LOG_POINT_B002 = 0xB002,
    LOG_POINT_B003 = 0xB003,
    LOG_POINT_B004 = 0xB004,
    LOG_POINT_B005 = 0xB005,
    LOG_POINT_B006 = 0xB006,
    LOG_POINT_B007 = 0xB007,
    LOG_POINT_B008 = 0xB008,
    LOG_POINT_B009 = 0xB009,
    LOG_POINT_B00A = 0xB00A,
    LOG_POINT_B00B = 0xB00B,
    LOG_POINT_B00C = 0xB00C,
    LOG_POINT_B00D = 0xB00D,
    LOG_POINT_B00E = 0xB00E,
    LOG_POINT_B00F = 0xB00F,
    LOG_POINT_B010 = 0xB010,
    LOG_POINT_B011 = 0xB011,
    LOG_POINT_B012 = 0xB012,
    LOG_POINT_B013 = 0xB013,
    LOG_POINT_B014 = 0xB014,
    LOG_POINT_B015 = 0xB015,
    LOG_POINT_B016 = 0xB016,
    LOG_POINT_B017 = 0xB017,
    LOG_POINT_B018 = 0xB018,

    LOG_POINT_B020 = 0xB020,
    LOG_POINT_B021 = 0xB021,

    LOG_POINT_B030 = 0xB030,
    LOG_POINT_B031 = 0xB031,
    LOG_POINT_B032 = 0xB032,
    LOG_POINT_B033 = 0xB033,
    LOG_POINT_B034 = 0xB034,
    LOG_POINT_B035 = 0xB035,
    LOG_POINT_B036 = 0xB036,
    LOG_POINT_B037 = 0xB037,

    LOG_POINT_B040 = 0xB040,
    LOG_POINT_B041 = 0xB041,
    LOG_POINT_B042 = 0xB042,
    LOG_POINT_B043 = 0xB043,
    LOG_POINT_B044 = 0xB044,

    LOG_POINT_B050 = 0xB050,
    LOG_POINT_B051 = 0xB051,
    LOG_POINT_B052 = 0xB052,
    LOG_POINT_B053 = 0xB053,
    LOG_POINT_B054 = 0xB054,
    LOG_POINT_B055 = 0xB055,

    LOG_POINT_B05A = 0xB05A,
    LOG_POINT_B05B = 0xB05B,
    LOG_POINT_B05C = 0xB05C,
    LOG_POINT_B05D = 0xB05D,

    LOG_POINT_B060 = 0xB060,
    LOG_POINT_B061 = 0xB061,

    LOG_POINT_B070 = 0xB070,
    LOG_POINT_B071 = 0xB071,
    LOG_POINT_B072 = 0xB072,
    LOG_POINT_B073 = 0xB073,
    LOG_POINT_B074 = 0xB074,
    LOG_POINT_B075 = 0xB075,

    LOG_POINT_B080 = 0xB080,
    LOG_POINT_B081 = 0xB081,
    LOG_POINT_B082 = 0xB082,
    LOG_POINT_B083 = 0xB083,
    LOG_POINT_B084 = 0xB084,
    LOG_POINT_B085 = 0xB085,
    LOG_POINT_B086 = 0xB086,
    LOG_POINT_B087 = 0xB087,
    LOG_POINT_B088 = 0xB088,
    LOG_POINT_B089 = 0xB089,
    LOG_POINT_B08A = 0xB08A,
    LOG_POINT_B08F = 0xB08F,

    LOG_POINT_B090 = 0xB090,
    LOG_POINT_B091 = 0xB091,
    LOG_POINT_B092 = 0xB092,
    LOG_POINT_B093 = 0xB093,

    LOG_POINT_B0A0 = 0xB0A0,
    LOG_POINT_B0A1 = 0xB0A1,
    LOG_POINT_B0A2 = 0xB0A2,
    LOG_POINT_B0A3 = 0xB0A3,
    LOG_POINT_B0A4 = 0xB0A4,
    LOG_POINT_B0A5 = 0xB0A5,

    LOG_POINT_B0B0 = 0xB0B0,
    LOG_POINT_B0B1 = 0xB0B1,
    LOG_POINT_B0B2 = 0xB0B2,
    LOG_POINT_B0B3 = 0xB0B3,
    LOG_POINT_B0B4 = 0xB0B4,
    LOG_POINT_B0B5 = 0xB0B5,
    LOG_POINT_B0B6 = 0xB0B6,
    LOG_POINT_B0B7 = 0xB0B7,
    LOG_POINT_B0B8 = 0xB0B8,
    LOG_POINT_B0B9 = 0xB0B9,
    LOG_POINT_B0BF = 0xB0BF,


    LOG_POINT_B100 = 0xB100,
    LOG_POINT_B101 = 0xB101,
    LOG_POINT_B102 = 0xB102,
    LOG_POINT_B103 = 0xB103,
    LOG_POINT_B104 = 0xB104,
    LOG_POINT_B105 = 0xB105,
    LOG_POINT_B106 = 0xB106,
    LOG_POINT_B107 = 0xB107,
    LOG_POINT_B108 = 0xB108,
    // Key Remap
    LOG_POINT_C000 = 0xC000,
    LOG_POINT_C001 = 0xC001,
    LOG_POINT_C002 = 0xC002,
    LOG_POINT_C003 = 0xC003,
    LOG_POINT_C004 = 0xC004,
    LOG_POINT_C005 = 0xC005,
    LOG_POINT_C006 = 0xC006,

    LOG_POINT_C010 = 0xC010,

    //Led Remap
    LOG_POINT_C100 = 0xC100,
    LOG_POINT_C101 = 0xC101,
    LOG_POINT_C102 = 0xC102,
    LOG_POINT_C103 = 0xC103,
    //battery
    LOG_POINT_C200 = 0xC200,
    LOG_POINT_C201 = 0xC201,
    LOG_POINT_C202 = 0xC202,
    LOG_POINT_C203 = 0xC203,
    LOG_POINT_C204 = 0xC204,
    LOG_POINT_C205 = 0xC205,
    LOG_POINT_C210 = 0xC210,
    LOG_POINT_C211 = 0xC211,
    LOG_POINT_C212 = 0xC212,
    LOG_POINT_C213 = 0xC213,
    LOG_POINT_C214 = 0xC214,
    LOG_POINT_C215 = 0xC215,
    LOG_POINT_C216 = 0xC216,
    LOG_POINT_C220 = 0xC220,
    LOG_POINT_C221 = 0xC221,
    LOG_POINT_C222 = 0xC222,
    LOG_POINT_C223 = 0xC223,
    LOG_POINT_C224 = 0xC224,
    //ATCMD
    LOG_POINT_C300 = 0xC300,
    LOG_POINT_C301 = 0xC301,
    //BLE OTA
    LOG_POINT_C400 = 0xC400,
    LOG_POINT_C401 = 0xC401,
    LOG_POINT_C402 = 0xC402,
    //OAL
    LOG_POINT_C500 = 0xC500,
    LOG_POINT_C501 = 0xC501,
    LOG_POINT_C502 = 0xC502,
    LOG_POINT_C503 = 0xC503,
    LOG_POINT_C504 = 0xC504,
    LOG_POINT_C505 = 0xC505,
    LOG_POINT_C506 = 0xC506,
    // SCO
    LOG_POINT_D000 = 0xD000,
    LOG_POINT_D001 = 0xD001,
    LOG_POINT_D002 = 0xD002,
    LOG_POINT_D003 = 0xD003,
    LOG_POINT_D004 = 0xD004,
    LOG_POINT_D005 = 0xD005,
    LOG_POINT_D006 = 0xD006,
    LOG_POINT_D007 = 0xD007,
    LOG_POINT_D008 = 0xD008,
    LOG_POINT_D009 = 0xD009,
    LOG_POINT_D00A = 0xD00A,
    LOG_POINT_D00B = 0xD00B,
    LOG_POINT_D00C = 0xD00C,
    LOG_POINT_D00D = 0xD00D,
    LOG_POINT_D00E = 0xD00E,
    LOG_POINT_D00F = 0xD00F,

    // CVSD
    LOG_POINT_D010 = 0xD010,
    LOG_POINT_D011 = 0xD011,
    LOG_POINT_D012 = 0xD012,
    LOG_POINT_D013 = 0xD013,
    LOG_POINT_D014 = 0xD014,
    LOG_POINT_D015 = 0xD015,



    //malloc
    LOG_POINT_D200 = 0xD200,
    LOG_POINT_D201 = 0xD201,
    LOG_POINT_D202 = 0xD202,
    LOG_POINT_D203 = 0xD203,
    LOG_POINT_D204 = 0xD204,
    LOG_POINT_D205 = 0xD205,
    LOG_POINT_D206 = 0xD206,
    LOG_POINT_D207 = 0xD207,
    LOG_POINT_D208 = 0xD208,
    LOG_POINT_D209 = 0xD209,
    LOG_POINT_D20A = 0xD20a,
    LOG_POINT_D20B = 0xD20b,
    LOG_POINT_D20C = 0xD20c,
    LOG_POINT_D20D = 0xD20d,
    LOG_POINT_D20E = 0xD20e,
    LOG_POINT_D20F = 0xD20f,
    LOG_POINT_D210 = 0xD210,
    LOG_POINT_D211 = 0xD211,
    LOG_POINT_D212 = 0xD212,
    LOG_POINT_D213 = 0xD213,
    LOG_POINT_D214 = 0xD214,
    // HCI
    // hci snd.
    // evt
    LOG_POINT_E000 = 0xE000,
    LOG_POINT_E001 = 0xE001,
    LOG_POINT_E002 = 0xE002,
    LOG_POINT_E003 = 0xE003,
    LOG_POINT_E004 = 0xE004,
    LOG_POINT_E005 = 0xE005,

    // acl
    LOG_POINT_E010 = 0xE010,
    LOG_POINT_E011 = 0xE011,
    // hci rcv.
    // cmd
    LOG_POINT_E100 = 0xE100,
    LOG_POINT_E101 = 0xE101,
    // acl
    LOG_POINT_E110 = 0xE110,
    LOG_POINT_E111 = 0xE111,
    // For Error Debug, from 0xfff9 to 0xfffe use for cm0
    LOG_POINT_FFF9 = 0xFFF9,
};

enum
{
    // For cm0 error code
    ERROR_CODE_WATCH_DOG = 0x0001,
};






















// Debug Round Buffer Index Define, one buffer should only use one
enum
{
    DEBUG_ROUND_BUFFER_INDEX_00 = 0x00,
    DEBUG_ROUND_BUFFER_INDEX_01 = 0x01,
    DEBUG_ROUND_BUFFER_INDEX_02 = 0x02,
    DEBUG_ROUND_BUFFER_INDEX_03 = 0x03,
    DEBUG_ROUND_BUFFER_INDEX_04 = 0x04,
    DEBUG_ROUND_BUFFER_INDEX_05 = 0x05,
    DEBUG_ROUND_BUFFER_INDEX_06 = 0x06,
    DEBUG_ROUND_BUFFER_INDEX_07 = 0x07,
    DEBUG_ROUND_BUFFER_INDEX_08 = 0x08,
    DEBUG_ROUND_BUFFER_INDEX_09 = 0x09,
    DEBUG_ROUND_BUFFER_INDEX_0A = 0x0A,
    DEBUG_ROUND_BUFFER_INDEX_0B = 0x0B,
};




uint8_t get_float_data_array(float input_value,uint8_t * output_str);


#endif //_YC_DEBUG_H_

