#ifndef _YC11XX_LPM_H_
#define _YC11XX_LPM_H_
#include "yc11xx.h"
#include "btreg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LPM_ENABLE  1
#define LPM_DISABLE 0

#define NONE_LPM_FLAG   0x0000
#define LPM_ALL_LOCK    0xFFFF

#define APP_LPM_LOCK    0x01
#define KEY_LPM_FLAG    0x02
#define LED_LPM_FLAG    0x04
#define IR_LPM_FLAG     0x08
#define AUDIO_LPM_FLAG  0x10
#define MOUSE_LPM_FLAG  0x20
#define M0_LPM_FLAG     0x40

#define M0_LPM_REG  mem_m0_lpm_flag

extern void Lpm_SleepWork(void);
extern uint16_t Lpm_GetM0LpmReg(void);
extern uint8_t Lpm_GetWakeFlag(void);
extern bool Lpm_CheckLpmFlag(void);
extern void Lpm_PollingWork(void);
extern void Lpm_unLockLpm(uint16_t lpmNum);
extern void Lpm_LockLpm(uint16_t lpmNum);
extern void Lpm_EnableSet(uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif
