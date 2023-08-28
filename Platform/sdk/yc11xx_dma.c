/**
***************************************************************************
* @file:  yc11xx_dma.c
* @Date:  2021-8-9-16:37:51
* @brief:
**************************************************************************
*/
/**
***************************************************************************
* @file:  yc11xx_dma.c
* @Date:  2021-8-9-16:35:20
* @brief:
* Change Logs:
* Date           Author      Version        Notes
* 2021-06-04     zhou         V1.0.0         the first version

**************************************************************************
*/
#include "yc11xx_dma.h"
#include "reg_addr.h"
/**
 * @brief  dma copy,Supports automatic determination of buffer size and circular copy
 * @param    dest    Destination address of the copy
 * @param    src     Source address of the copy
 * @param    len     data length of copy
 * @param    dest_start  Destination buffer start address
 * @param    dest_size   Destination buffer size
 * @param    src_start   Source buffer start address
 * @param    src_size    Source buffer size
 * @return  None
 */
void DMA_COPY(void *dest, void *src, int len, void *dest_start, int dest_size, void *src_start, int src_size)
{
    uint8_t sdma_ctrl = 0;
    if(len == 0)return;
    HWRITEW(CORE_SDMA_DST, (int)dest);
    HWRITEW(CORE_SDMA_SRC, (int)src);
    HWRITEW(CORE_SDMA_LEN, len);
    HWRITEW(CORE_SDMA_DST_START, (int)dest_start);
    HWRITEW(CORE_SDMA_DST_END, (int)dest_start + (int)dest_size);
    HWRITEW(CORE_SDMA_SRC_START, (int)src_start);
    HWRITEW(CORE_SDMA_SRC_END, (int)src_start + (int)src_size);

    sdma_ctrl = SDMA_CLEAR_INT;
    if(reg_check_ram_m0((int)src))
    {
        sdma_ctrl |= SDMA_SRC_MRAM;
    }
    if(reg_check_ram_m0((int)dest))
    {
        sdma_ctrl |= SDMA_DST_MRAM;
    }
    HWRITE(CORE_SDMA_CTRL, sdma_ctrl);

    HWRITE(CORE_DMA_START, BIT5);
    //while(!(HREAD(CORE_SDMA_DONE) & SDMA_DMA_DONE != 0));
	while(!(HREAD(CORE_SDMA_DONE) & SDMA_DMA_DONE));
}
/**
 * @brief   dma buffer copy
 * @param    dest    estination address of the copy
 * @param    src     Source address of the copy
 * @param    len     data length of copy
 * @return  None
 */
void dmacpy(void *dest, void *src, int len)
{
    DMA_COPY(dest, src, len, 0, -1, 0, -1);
}






/**
 * @brief  dma task init
 * @param    tasknum dma task  num0-7
 * @param    len     data length of copy
 * @param    src     Source address of the copy
 * @param    des     Destination address of the copy
 * @return  None
 */
void DMAM0_TableInit(uint8_t task_num, uint32_t len, uint32_t src, uint32_t des)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        cfg_table[i] = (uint32_t)&soft_dma_taskcb[i].task_cfg[0];
    }

    /*step1:soft dma init*/
    DMAM0_AddNormalCopy(task_num, SDMA_COPY_CONFIG| len, src, des);
}
/**
 * @brief  copy enable
 * @param    task_num :bit0-7:task0-7
 * @return  None
 */
void DMAM0_NormalCopyStart(uint16_t task_num)
{
    DMAM0_AddTaskRun(task_num);
    while(HREAD2(DMA_TASK_STATUS));
}
/**
 * @brief    dma run,start copy
 * @param    list :   task_num :bit0-7:task0-7
 * @return  None
 */
void DMAM0_AddTaskRun(uint8_t list)
{
    HWRITE2(DMA_TASK_VAL, list);
    HWRITE24BIT(DMA_TASK_TABLE_ADDR, ((uint32_t)&cfg_table[0] & 0xfffff));
    HWRITE2(DMA_CTRL, SDMA_TSAK_ADD);
    HWRITE2(DMA_CTRL, SDMA_TSAK_BACK);
}
/**
 * @brief  record dma parameter
 * @param    idx  dma task num
 * @param    cfg  reg parameter
 * @param    src  Source address of the copy
 * @param    dst  Destination address of the copy
 * @return  None
 */
void DMAM0_AddNormalCopy(uint8_t idx, uint32_t cfg, uint32_t src, uint32_t dst)
{
    soft_dma_taskcb[idx].task_cfg[0] = cfg;
    soft_dma_taskcb[idx].task_cfg[1] = src;
    soft_dma_taskcb[idx].task_cfg[2] = dst;
}
/**
 * @brief  clear dma interrupt
 * @param    tast_num:tast number
 * @return  None
 */
void DMA0_InterruptClear(uint8_t tast_num)
{
    HWRITE(DMA_TASK_INT_CLEAR, tast_num);
}
