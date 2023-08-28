/*
 * Copyright (c) 2006-2021, YICHIP Development Team
 * @file     yc_NVIC.c
 * @brief    source file for setting rv_irq
 *
 * Change Logs:
 * Date            Author             Version        Notes
 * 2021-3-3      ZHANGMING	          V1.0.0         the first version
 */
#include "yc_nvic.h"

void RV_EnableIRQ(IRQn_Type IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        RV_IRQ_ENAB(IRQn) |= (uint32_t)(1UL << (uint8_t)((uint8_t)(IRQn) % 8));
    }
}
void RV_DisableIRQ(IRQn_Type IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        RV_IRQ_ENAB(IRQn) &= ~(uint32_t)(1UL << (uint8_t)((uint8_t)(IRQn) % 8));
    }
}
void RV_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    if ((int32_t)(IRQn) >= 0 && priority > 0 && priority < 16)
    {
        RV_IRQ_PRIO(IRQn) |= (priority << (uint8_t)((uint8_t)(IRQn) % 2) * 4);
    }
}

/**
 * @method NVIC_Configuration
 * @brief  NVIC initialization function for risc-v.
 * @param  IRQn: Interrupt vector numbers could be 0~31.
 * @param  priority: Interrupt priority numbers must be 1~15.
 * @param  newstate: enable or disable
 * @retval None
 */
void NVIC_Configuration(IRQn_Type IRQn, uint32_t priority, FunctionalState newstate)
{
    // _ASSERT(IS_IRQN(IRQn));
    // _ASSERT(IS_PRIORITY((int)priority));
    // _ASSERT(IS_FUNCTIONAL_STATE(newstate));
    if(newstate == ENABLE )
    {
        RV_EnableIRQ(IRQn);
        RV_SetPriority(IRQn, priority);
    }
    else
    {
        RV_DisableIRQ(IRQn);
        RV_SetPriority(IRQn, priority);
    }
}


/**
 * @method nvic_cfg_init
 * @brief  NVIC Configuration initialization for risc-v.
 * @param  None
 * @retval None
 */
void nvic_cfg_init(void)
{
    NVIC_Configuration(uart_IRQn, 0, DISABLE);
    NVIC_Configuration(uartb_IRQn, 0, DISABLE);
    NVIC_Configuration(USB_IRQn, 0, DISABLE);
}
