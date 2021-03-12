/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* PSP coverage stub replacement for stdlib.h */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utstubs.h"
#include "utassert.h"

#include <PCS_stdlib.h>

/*
 * The malloc emulator relies on two magic numbers;
 * one at the start of the pool, one for each block allocated.
 *
 * Note that the malloc emulator is not a real allocator,
 * it only allocates sequential blocks and does not recover
 * the space after free.
 */
#define MPOOL_START_SIGNATURE 0x8a458c6b
#define MPOOL_BLOCK_SIGNATURE 0x3ef65721
#define MPOOL_ALIGN           16

struct MPOOL_REC
{
    cpuaddr BlockAddr;
    uint32  Magic;
    uint32  Size;
};

void PCS_exit(int c)
{
    UT_DEFAULT_IMPL(PCS_exit);

    /*
     * This call is never supposed to return, but this stub will.
     * The application therefore must handle a return from exit()
     *
     * TBD: IT would be nice if this could use a setjmp-like
     * method to avoid returning here.
     */
}

unsigned long int PCS_strtoul(const char *nptr, char **endptr, int base)
{
    int32         Status;
    unsigned long Result = 0;

    Status = UT_DEFAULT_IMPL_RC(PCS_strtoul, -1);

    if (Status < 0)
    {
        /* do the real op */
        Result = strtoul(nptr, endptr, base);
    }
    else
    {
        Result = Status;
    }

    return Result;
}

int PCS_system(const char *command)
{
    return UT_DEFAULT_IMPL(PCS_system);
}

void *PCS_malloc(size_t sz)
{
    int32             Status;
    void *            PoolPtr;
    cpuaddr           PoolStart;
    cpuaddr           PoolEnd;
    cpuaddr           NextBlock;
    size_t            NextSize;
    size_t            PoolSize;
    uint32            CallCnt;
    struct MPOOL_REC *Rec;

    Rec     = NULL;
    CallCnt = UT_GetStubCount(UT_KEY(PCS_malloc));
    UT_GetDataBuffer(UT_KEY(PCS_malloc), &PoolPtr, &PoolSize, NULL);

    if (PoolPtr != NULL)
    {
        PoolStart = (cpuaddr)PoolPtr;
        PoolEnd   = PoolStart + PoolSize;
        PoolStart = (PoolStart + MPOOL_ALIGN - 1) & ~((cpuaddr)MPOOL_ALIGN - 1);
        PoolSize  = PoolEnd - PoolStart;

        if (PoolSize > (MPOOL_ALIGN * 2))
        {
            Rec       = (struct MPOOL_REC *)PoolStart;
            NextBlock = PoolStart + MPOOL_ALIGN;
            PoolSize -= MPOOL_ALIGN;
            if (CallCnt == 0)
            {
                Rec->Magic     = MPOOL_START_SIGNATURE;
                Rec->Size      = 0;
                Rec->BlockAddr = NextBlock;
            }
            else if (Rec->Magic != MPOOL_START_SIGNATURE)
            {
                UtAssert_Failed("PCS_malloc() heap corruption detected");
            }
        }
    }

    Status = UT_DEFAULT_IMPL(PCS_malloc);

    if (Status != 0 || Rec == NULL)
    {
        return NULL;
    }

    NextSize = Rec->Size + sz + MPOOL_ALIGN;
    if (NextSize > PoolSize)
    {
        /*
         * This indicates that the application is trying to allocate
         * a block larger than the pool.  It typically means that the
         * emulated heap size is too small, so it is prudent to generate
         * a message.
         */
        UtAssert_Failed("PCS_malloc() heap has been exhausted");
        return NULL;
    }

    NextSize  = (NextSize + MPOOL_ALIGN - 1) & ~((size_t)MPOOL_ALIGN);
    NextBlock = Rec->BlockAddr + MPOOL_ALIGN;
    Rec->BlockAddr += NextSize;
    Rec->Size += NextSize;

    Rec            = (struct MPOOL_REC *)(NextBlock - sizeof(struct MPOOL_REC));
    Rec->BlockAddr = NextBlock;
    Rec->Magic     = MPOOL_BLOCK_SIGNATURE;
    Rec->Size      = sz;

    return ((void *)NextBlock);
}

void PCS_free(void *ptr)
{
    int32             Status;
    cpuaddr           BlockAddr;
    void *            PoolPtr;
    size_t            PoolSize;
    struct MPOOL_REC *Rec;

    /*
     * If there is a data buffer associated with free() then this
     * will sanity-check that the block being freed came from that heap.
     */
    UT_GetDataBuffer(UT_KEY(PCS_free), &PoolPtr, &PoolSize, NULL);

    Status = UT_DEFAULT_IMPL(PCS_free);
    if (Status == 0 && PoolPtr != NULL)
    {
        BlockAddr = (cpuaddr)ptr;
        if (BlockAddr < (cpuaddr)PoolPtr || BlockAddr >= ((cpuaddr)PoolPtr + PoolSize))
        {
            UtAssert_Failed("PCS_free(): Heap corruption -- Non-Heap pointer");
        }
        else
        {
            Rec = (struct MPOOL_REC *)(BlockAddr - sizeof(struct MPOOL_REC));
            if (Rec->Magic == MPOOL_BLOCK_SIGNATURE)
            {
                Rec->Magic = ~MPOOL_BLOCK_SIGNATURE;
            }
            else if (Rec->Magic == ~MPOOL_BLOCK_SIGNATURE)
            {
                UtAssert_Failed("PCS_free(): Heap corruption -- Double free detected");
            }
            else
            {
                UtAssert_Failed("PCS_free(): Heap corruption -- Corrupted block detected");
            }
        }
    }
}
