#include "yc11xx_lpm.h"
#include "hci_dtm_test.h"
#include "yc_debug.h"


void app_task_run(void){}

void app_init(void)
{
    DEBUG_LOG_STRING("HD DTM MODE \r\n");
    Lpm_EnableSet(LPM_DISABLE);
    Lpm_LockLpm(LPM_ALL_LOCK);
    hci_freq_tx_test();
    DEBUG_LOG_STRING("START FREQ TX TEST \r\n");
}
