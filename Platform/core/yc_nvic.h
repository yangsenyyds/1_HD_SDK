#ifndef __YC_NVIC_H
#define __YC_NVIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "yc11xx.h"


#define IS_IRQN(IRQN)  ((((int)IRQN) >= 0) && (((int)IRQN) <= 50))
#define IS_PRIORITY(priority)  (((priority) >= 0) && ((priority) <= 3))


void RV_EnableIRQ(IRQn_Type IRQn);
void RV_DisableIRQ(IRQn_Type IRQn);
void RV_SetPriority(IRQn_Type IRQn, uint32_t priority);

void NVIC_Configuration(IRQn_Type IRQn, uint32_t priority, FunctionalState newstate);
void nvic_cfg_init(void);

#ifdef __cplusplus
}
#endif

#endif
