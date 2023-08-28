#include "yc11xx.h"
#include "yc11xx_lpm.h"
#include "yc11xx_ipc.h"
#include "yc11xx_dev_bt_data_trans.h"
#include "yc11xx_iwdg.h"
#include "yc11xx_dev_qspi.h"
#include "yc_debug.h"
#include "malloc.h"
#include "yc_dev_bt.h"
#include "app_config.h"

#ifdef FUNCTION_WATCH_DOG
IWDG_InitTypeDef gWdtInit = {
   .WDTx = WDT2,
   .MODE= RESET_MODE,
   .SETLOAD = WDT_timer_1_5s,
};
#endif

MEMORY_NOT_PROTECT_UNDER_LPM_ATT static uint8_t gHeapBuffer[HEAP_BUFFER_SIZE];

static IPCHandleCb_TypeDef IPCHandleCb[IPC_TYPE_NUM] = {
    IpcDefaultCallBack,
    IpcDefaultCallBack,//cmd

    Bt_EvtCallBack,//evt

    IpcDefaultCallBack,//hid
    IpcDefaultCallBack,//spp
    Bt_BleCallBack,//ble
    Bt_DataBufferCallBack,//buffer
    IpcDefaultCallBack,//mesh
    IpcDefaultCallBack,//mesh
    IpcDefaultCallBack,//mesh
    IpcDefaultCallBack,//a2dp
    IpcDefaultCallBack,//hfp
    IpcDefaultCallBack,//tws
};

static void bt_task_run(void)
{
    IPC_HandleRxPacket();
}

static void bt_init(void)
{    
    Memory_InitHeap((uint *)gHeapBuffer, HEAP_BUFFER_SIZE);
    Bt_HciFifo_Init();
    
    if(!Lpm_GetWakeFlag())
    {
        IPC_Init(&IPCHandleCb);

        bt_set_adv_random(0x3);
        bt_set_tx_power(TX_POWER_5DB);
        bt_state_init();
        bt_set_local_dle(251,2120,251,2120);
    }
}

static void chip_task_run(void)
{
    DEBUG_POLLING_PRINT();
    Lpm_PollingWork();
#ifdef FUNCTION_WATCH_DOG
    IWDG_ReloadCounter(&gWdtInit);
#endif 
}

static void chip_init(void)
{
#ifdef FUNCTION_WATCH_DOG
    IWDG_ReloadCounter(&gWdtInit); 
#else
    IWDG_Disable(WDT);   //bt watch dog     
    IWDG_Disable(WDT2);  //riscv watch dog
#endif
    Lpm_EnableSet(LPM_ENABLE);
    DEBUG_INIT();
    OS_INITIAL_CRITICAL();
    
    if(!Lpm_GetWakeFlag()) {
        QSPI_FlashWriteLock(true, LOCK_FLASH_AREA_ALL);
    }
}

void main(void)
{
    chip_init();
    bt_init();
    app_init();

    while (1)
    {
        app_task_run();
        bt_task_run();
        chip_task_run();
    }
}
