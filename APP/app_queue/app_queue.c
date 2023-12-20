#include "app_queue.h"
#include "app_config.h"
#include "yc_debug.h"

#include "remote_control.h"

#define APPQUELEN (TIMER_NUM * 3) 

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static QueMgr_TypeDef AppQueMgr;
MEMORY_NOT_PROTECT_UNDER_LPM_ATT static AppEvent_TypeDef AppEventQue[APPQUELEN];

bool app_queue_is_empty(void)
{
    return (AppQueMgr.current_queue_len ? false:true);
}

uint8_t app_queue_insert(polling_func_t func)
{
    if (AppQueMgr.current_queue_len >= APPQUELEN || func == NULL) {
        DEBUG_LOG_STRING("APP QUEUE IS FULL \r\n");
        return 1;
    }

    OS_ENTER_CRITICAL();
    AppEventQue[AppQueMgr.write_index++].AppEvent = func;
    AppQueMgr.current_queue_len++;
    if (AppQueMgr.write_index >= APPQUELEN - 1) {
        AppQueMgr.write_index = 0;
    }
    OS_EXIT_CRITICAL();
    
    return 0;
}

void app_queue_task(void)
{
    if (AppQueMgr.current_queue_len > 0)
    {
        AppEventQue[AppQueMgr.read_index].AppEvent();

        OS_ENTER_CRITICAL(); /// add 20230522
        AppQueMgr.read_index++;
        AppQueMgr.current_queue_len--;
        if (AppQueMgr.read_index >= APPQUELEN - 1)
        {
            AppQueMgr.read_index = 0;
        }
        OS_EXIT_CRITICAL();
    }
}

void app_queue_reset(void)
{
    memset(&AppQueMgr, 0, sizeof(AppQueMgr));
    memset(AppEventQue, 0, sizeof(AppEventQue));
}

void app_queue_clr(polling_func_t func)
{
    DEBUG_LOG_STRING("AppQueMgr.current_queue_len %d\r\n",AppQueMgr.current_queue_len);
    for(uint8_t i = 0; i < AppQueMgr.current_queue_len; i++) {
        DEBUG_LOG_STRING("111 \r\n");
        if(AppEventQue[i].AppEvent == func){
            AppEventQue[i].AppEvent = NULL;
             DEBUG_LOG_STRING("app_queue_clr \r\n");
        }
    }
}