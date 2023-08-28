#ifndef __YC11XX_DMA__
#define __YC11XX_DMA__
#include "yc11xx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t task_cfg[6];
    uint32_t status;
} SoftDma_TaskCbDef;

uint32_t cfg_table[8];
SoftDma_TaskCbDef soft_dma_taskcb[8];

#define SDMA_CLEAR_INT   BIT0
#define SDMA_SRC_MRAM    BIT1
#define SDMA_DST_MRAM    BIT2
#define SDMA_DMA_DONE    BIT7
#define SDMA_COPY_CONFIG 0x81fc0000
#define SDMA_TSAK_ADD    0x34
#define SDMA_TSAK_BACK   0x30

#define DMA_START_SDMA  1 << 0
#define DMA_STATUS_SDMA 1 << 0
#define DMA_STATUS_QSPI 1 << 1



void DMA_COPY(void *dest, void *src, int len, void *dest_start, int dest_size, void *src_start, int src_size);
void dmacpy(void *dest, void *src, int len);
void DMAM0_AddTaskRun(uint8_t list);
void DMAM0_AddNormalCopy(uint8_t idx, uint32_t cfg, uint32_t src, uint32_t dst);
void DMAM0_TableInit(uint8_t task_num, uint32_t len, uint32_t src, uint32_t des);
void DMAM0_NormalCopyStart(uint16_t task_num);
void DMA0_InterruptClear(uint8_t tast_num);


#endif /* _YC11XX_DMA_H_ */


