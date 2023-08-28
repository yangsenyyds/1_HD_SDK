#ifndef APP_QUEUE
#define APP_QUEUE
#include <string.h>
#include "yc11xx.h"


typedef struct {
    uint8_t read_index;
    uint8_t write_index;
    uint8_t current_queue_len;
} QueMgr_TypeDef;

typedef void (*polling_func_t)(void);

typedef struct {
    polling_func_t AppEvent;
}AppEvent_TypeDef;


extern bool app_queue_is_empty(void);
extern uint8_t app_queue_insert(polling_func_t func);
extern void app_queue_task(void);
extern void app_queue_reset(void);
extern void app_queue_clr(polling_func_t func);
#endif
