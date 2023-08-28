#include "malloc.h"
#include "yc11xx.h"
#include "yc_debug.h"


//#define Malloc_DetailLogEnable

#define BLOCK_ALLOCATED		0x80000000
#define portBYTE_ALIGNMENT    	4
#define portBYTE_ALIGNMENT_MASK     (portBYTE_ALIGNMENT - 1)
#define HEAP_STRUCT_SIZE	        sizeof(BlockLink_t)
#define heapMINIMUM_BLOCK_SIZE      ((uint)(HEAP_STRUCT_SIZE << 1))

/* Define the linked list structure.  This is used to link free blocks in order
 * of their memory address. */
typedef struct A_BLOCK_LINK {
    struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
    uint xBlockSize;						/*<< The size of the free block. */
} BlockLink_t;

/* Keeps track of the number of calls to allocate and free memory as well as the
* number of free bytes remaining, but says nothing about fragmentation. */
static uint xFreeBytesRemaining = 0U;
static uint xMinimumEverFreeBytesRemaining = 0U;
static uint xNumberOfSuccessfulAllocations = 0;
static uint xNumberOfSuccessfulFrees = 0;
static BlockLink_t xStart = {0}, *pxEnd = 0;

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList(BlockLink_t *pxBlockToInsert)
{
    /* */
    BlockLink_t *pxIterator;
    byte *puc;

    /* Iterate through the list until a block is found that has a higher address
     * than the block being inserted. */
    for (pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert;
            pxIterator = pxIterator->pxNextFreeBlock)
    {
        /* Nothing to do here, just iterate to the right position. */
        DEBUG_LOG(LOG_LEVEL_CORE, "UI", "InsertBlockIntoFreeListIterator:0x%04x", LOG_POINT_D212, (uint32_t)pxIterator);
        DEBUG_LOG(LOG_LEVEL_CORE, "UI", "InsertBlockIntoFreeListIteratorNextFreeBlock:0x%04x", LOG_POINT_D213, (uint32_t)pxIterator->pxNextFreeBlock);
    }

    /* Do the block being inserted, and the block it is being inserted after
     * make a contiguous block of memory? */
    puc = (byte *) pxIterator;

    if ((puc + pxIterator->xBlockSize) == (byte *) pxBlockToInsert)
    {
        pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
        pxBlockToInsert = pxIterator;
    }

    /* Do the block being inserted, and the block it is being inserted before
     * make a contiguous block of memory? */
    puc = (byte *) pxBlockToInsert;

    if ((puc + pxBlockToInsert->xBlockSize) ==
            (byte *) pxIterator->pxNextFreeBlock)
    {
        if (pxIterator->pxNextFreeBlock != pxEnd)
        {
            /* Form one big block from the two blocks. */
            pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
            pxBlockToInsert->pxNextFreeBlock =
                pxIterator->pxNextFreeBlock->pxNextFreeBlock;
        }
        else
        {
            pxBlockToInsert->pxNextFreeBlock = pxEnd;
        }
    }
    else
    {
        pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
    }

    /* If the block being inserted plugged a gab, so was merged with the block
     * before and the block after, then it's pxNextFreeBlock pointer will have
     * already been set, and should not be set here as that would make it point
     * to itself. */
    if (pxIterator != pxBlockToInsert)
    {
        pxIterator->pxNextFreeBlock = pxBlockToInsert;
    }
}

/* Create a couple of list links to mark the start and end of the list. */
void *Memory_Malloc(uint xWantedSize)
{
    BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = 0;
#ifdef Malloc_DetailLogEnable
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocCreatSize:0x%04x", LOG_POINT_D206, xWantedSize);
#endif
    xWantedSize = xWantedSize + HEAP_STRUCT_SIZE + portBYTE_ALIGNMENT_MASK & ~portBYTE_ALIGNMENT_MASK;

    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocCreatSize:0x%04x", LOG_POINT_D206, xWantedSize);
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "FreeBytesRemainSize:0x%04x", LOG_POINT_D207, xFreeBytesRemaining);

    if (xWantedSize <= xFreeBytesRemaining)
    {
        /* Traverse the list from the start (lowest address) block until
         * one  of adequate size is found. */
        pxPreviousBlock = &xStart;
        pxBlock = xStart.pxNextFreeBlock;
#ifdef Malloc_DetailLogEnable
        DEBUG_LOG(LOG_LEVEL_CORE, "UI", "Malloc xStart Addr:0x%04x", LOG_POINT_D208, (uint32_t)pxPreviousBlock);
        DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocTempBlockAddr:0x%04x", LOG_POINT_D209, (uint32_t)pxBlock);
#endif
        while ((pxBlock->xBlockSize < xWantedSize) && (pxBlock->pxNextFreeBlock != 0))
        {
#ifdef Malloc_DetailLogEnable
            DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocErgodicBlockSize:0x%04x", LOG_POINT_D20A, (uint32_t)pxBlock->xBlockSize);
#endif
            pxPreviousBlock = pxBlock;
            pxBlock = pxBlock->pxNextFreeBlock;
#ifdef Malloc_DetailLogEnable
            DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocErgodicPreviousBlock:0x%04x", LOG_POINT_D20B, (uint32_t)pxPreviousBlock);
            DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocErgodicTempBlock:0x%04x", LOG_POINT_D20C, (uint32_t)pxBlock);
#endif
        }

        /* If the end marker was reached then a block of adequate size
         * was  not found. */
        if (pxBlock != pxEnd)
        {
            /* Return the memory space pointed to - jumping over the
             * BlockLink_t structure at its start. */
            pvReturn =  (byte *) pxPreviousBlock->pxNextFreeBlock + HEAP_STRUCT_SIZE;

            DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocSuccessBlockAddr:0x%04x", LOG_POINT_D20D, (uint32_t)pvReturn);

            /* This block is being returned for use so must be taken out
             * of the list of free blocks. */
            pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;
#ifdef Malloc_DetailLogEnable
            DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocSuccessxBlockSize:0x%04x", LOG_POINT_D20E, (uint32_t)pxBlock->xBlockSize);
#endif
            /* If the block is larger than required it can be split into
             * two. */
            if ((pxBlock->xBlockSize - xWantedSize) > heapMINIMUM_BLOCK_SIZE)
            {
                /* This block is to be split into two.  Create a new
                 * block following the number of bytes requested. The void
                 * cast is used to prevent byte alignment warnings from the
                 * compiler. */
                pxNewBlockLink = (void *) (((byte *) pxBlock) + xWantedSize);


                DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocSuccessxNewBlockLinkAddr:0x%04x", LOG_POINT_D20F, (uint32_t)pxNewBlockLink);

                /* Calculate the sizes of two blocks split from the
                 * single block. */
                pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
                pxBlock->xBlockSize = xWantedSize;
#ifdef Malloc_DetailLogEnable
                DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocSuccessxBlockAddr:0x%04x", LOG_POINT_D210, (uint32_t)pxBlock);
#endif
                /* Insert the new block into the list of free blocks. */
                prvInsertBlockIntoFreeList (pxNewBlockLink);
            }

            xFreeBytesRemaining -= pxBlock->xBlockSize;

            if (xFreeBytesRemaining < xMinimumEverFreeBytesRemaining)
            {
                xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
            }

            /* The block is being returned - it is allocated and owned
             * by the application and has no "next" block. */
            pxBlock->xBlockSize |= BLOCK_ALLOCATED;
            pxBlock->pxNextFreeBlock = 0;
            xNumberOfSuccessfulAllocations++;
        }
    }
    else
    {
        // Force assert, avoid other perople not know.
        DEBUG_LOG(LOG_LEVEL_CORE, "UI", "Remain Size Insufficient", LOG_POINT_D214, 0);
        _ASSERT_FAULT();
    }

    return pvReturn;
}

void Memory_Free(void *pv)
{
    byte *puc = (byte *) pv;
    BlockLink_t *pxLink;
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "InsertBlockIntoFreeListAddr:0x%04x", LOG_POINT_D211, (uint32_t)(pv));

    if (pv != 0)
    {
        /* The memory being freed will have an BlockLink_t structure immediately
         * before it. */
        puc -= HEAP_STRUCT_SIZE;

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = (void *) puc;

        /* Check the block is actually allocated. */
        // _ASSERT((pxLink->xBlockSize & BLOCK_ALLOCATED) != 0);
        // _ASSERT(pxLink->pxNextFreeBlock == 0);

        if ((pxLink->xBlockSize & BLOCK_ALLOCATED) != 0)
        {
            pxLink->xBlockSize &= ~BLOCK_ALLOCATED;
            if (pxLink->pxNextFreeBlock == 0)
            {
                /* Add this block to the list of free blocks. */
                xFreeBytesRemaining += pxLink->xBlockSize;
                prvInsertBlockIntoFreeList (((BlockLink_t *) pxLink));
                xNumberOfSuccessfulFrees++;
            }
        }
    }
}

void Memory_GetHeapStats(HeapStats_t *pxHeapStats)
{
    BlockLink_t *pxBlock;
    uint xBlocks = 0, xMaxSize = 0, xMinSize = 0xffffffff;

    /* pxBlock will be 0 if the heap has not been initialised.  The heap
     * is initialised automatically when the first allocation is made. */
    for (pxBlock = xStart.pxNextFreeBlock, xBlocks = 0; pxBlock != 0 && pxBlock != pxEnd; xBlocks++, pxBlock = pxBlock->pxNextFreeBlock)
    {
        if (pxBlock->xBlockSize > xMaxSize)
            xMaxSize = pxBlock->xBlockSize;
        if (pxBlock->xBlockSize < xMinSize)
            xMinSize = pxBlock->xBlockSize;
    }

    pxHeapStats->xSizeOfLargestFreeBlockInBytes = xMaxSize;
    pxHeapStats->xSizeOfSmallestFreeBlockInBytes = xMinSize;
    pxHeapStats->xNumberOfFreeBlocks = xBlocks;
    pxHeapStats->xAvailableHeapSpaceInBytes = xFreeBytesRemaining;
    pxHeapStats->xNumberOfSuccessfulAllocations = xNumberOfSuccessfulAllocations;
    pxHeapStats->xNumberOfSuccessfulFrees = xNumberOfSuccessfulFrees;
    pxHeapStats->xMinimumEverFreeBytesRemaining = xMinimumEverFreeBytesRemaining;
}

void Memory_InitHeap(uint *heap, uint size)
{
    BlockLink_t *pxFirstFreeBlock;
    uint uxAddress;

    /* Ensure the heap starts on a correctly aligned boundary. */
    uxAddress = (uint) heap;
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocHeapInitStartAddr: 0x%04x", LOG_POINT_D200, uxAddress);


    /* xStart is used to hold a pointer to the first item in the list of free
     * blocks.  The void cast is used to prevent compiler warnings. */
    xStart.pxNextFreeBlock = (void *) uxAddress;
    xStart.xBlockSize = (uint) 0;
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocHeapInitNextFreeBlockAddr:0x04x", LOG_POINT_D201, (uint32_t)xStart.pxNextFreeBlock);

    /* pxEnd is used to mark the end of the list of free blocks and is inserted
     * at the end of the heap space. */
    pxEnd = (void *) (uxAddress + size - HEAP_STRUCT_SIZE);
    pxEnd->xBlockSize = 0;
    pxEnd->pxNextFreeBlock = 0;
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocHeapInitEndAddr:0x04x", LOG_POINT_D202, (uint32_t)pxEnd);

    /* To start with there is a single free block that is sized to take up the
     * entire heap space, minus the space taken by pxEnd. */
    pxFirstFreeBlock = (void *) uxAddress;
    pxFirstFreeBlock->xBlockSize = (pxEnd - pxFirstFreeBlock) * HEAP_STRUCT_SIZE;
    pxFirstFreeBlock->pxNextFreeBlock = pxEnd;
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocHeapInitFirstFreeBlockAddr:0x04x", LOG_POINT_D203, (uint32_t)pxFirstFreeBlock);
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocHeapInitFirstFreeBlockSize:0x04x", LOG_POINT_D204, (uint32_t)pxFirstFreeBlock->xBlockSize);
    DEBUG_LOG(LOG_LEVEL_CORE, "UI", "MallocHeapInitNextFreeAddr:0x04x", LOG_POINT_D205, (uint32_t)pxFirstFreeBlock->pxNextFreeBlock);

    /* Only one block exists - and it covers the entire usable heap space. */
    xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
    xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
}
