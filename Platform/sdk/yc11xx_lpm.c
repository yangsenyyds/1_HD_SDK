#include "yc11xx_lpm.h"
#include "yc11xx_ipc.h"
#include "app_config.h"

#include "yc_debug.h"

WEAK bool app_sleep_check(void){return true;}
WEAK void prepare_before_sleep(void){}

void Lpm_SleepWork(void)
{
    HWRITE(IPC_MCU_STATE, IPC_MCU_STATE_STOP);
    while(HREAD(IPC_MCU_STATE) == IPC_MCU_STATE_STOP);
}

uint16_t Lpm_GetM0LpmReg(void)
{
    return HREADW(M0_LPM_REG);
}

uint8_t Lpm_GetWakeFlag(void)
{
    return HREAD(mem_wake_flag);
}

bool Lpm_CheckLpmFlag(void)
{
    return ((NONE_LPM_FLAG == HREADW(M0_LPM_REG)) && HREAD(mem_lpm_mode));
}

void Lpm_PollingWork(void)
{
    switch (HREAD(IPC_MCU_STATE))
    {
        
        case IPC_MCU_STATE_RUNNING:
        // DEBUG_LOG_STRING("37\r\n");
            Lpm_unLockLpm(M0_LPM_FLAG);
            break;

        case IPC_MCU_STATE_LMP:
            OS_ENTER_CRITICAL();
            // DEBUG_LOG_STRING("41 %d %d %d %d %d\r\n", IPC_IsTxBuffEmpty(),Lpm_CheckLpmFlag(),app_sleep_check(),HREADW(M0_LPM_REG),HREAD(mem_lpm_mode));
            if (IPC_IsTxBuffEmpty() && Lpm_CheckLpmFlag() && app_sleep_check()) {
                // DEBUG_LOG_STRING("45\r\n");
                prepare_before_sleep();
                Lpm_SleepWork();
            }
            else {
                HWRITE(IPC_MCU_STATE, IPC_MCU_STATE_RUNNING);
            }
            OS_EXIT_CRITICAL();
            break;

        case IPC_MCU_STATE_HIBERNATE:
        
            OS_ENTER_CRITICAL();
            prepare_before_sleep();
            Lpm_SleepWork();
            break;
            
        case IPC_MCU_STATE_STOP:
                // DEBUG_LOG_STRING("62\r\n");
            break;
        
        default:
        // DEBUG_LOG_STRING("66\r\n");
          	break;
    }
}

void Lpm_unLockLpm(uint16_t lpmNum)
{
    uint16_t temp = HREADW(M0_LPM_REG) & (~lpmNum);
    HWRITEW(M0_LPM_REG, temp);
}

void Lpm_LockLpm(uint16_t lpmNum)
{
    uint16_t temp = HREADW(M0_LPM_REG) | lpmNum;
    HWRITEW(M0_LPM_REG, temp);
}

void Lpm_EnableSet(uint8_t enable)
{
    HWRITE(mem_lpm_mode, enable);
}
