/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/******************************************************************************
** File: cfe_sb_buf.c
**
** Purpose:
**      This file contains the source code for the SB memory management
**      functions.
**
** Author:   R.McGraw/SSI
**
******************************************************************************/

/*
**  Include Files
*/

#include "cfe_sb_priv.h"
#include "cfe_sb_events.h"
#include "osapi.h"
#include "cfe_es.h"
#include "cfe_error.h"

/*
 * The actual message content of a SB Buffer Descriptor is the
 * offset of the content member.  This will be auto-aligned by 
 * the compiler according to the requirements of the machine.
 */
#define CFE_SB_BUFFERD_CONTENT_OFFSET   (offsetof(CFE_SB_BufferD_t,Content))


/******************************************************************************
 *
 * Helper function to reset/clear the links on a list node (make empty)
 */
void CFE_SB_TrackingListReset(CFE_SB_BufferLink_t *Link)
{
    /* A singleton node/empty list points to itself */
    Link->Prev = Link;
    Link->Next = Link;
}

/******************************************************************************
 *
 * Helper function to remove a node from a tracking list
 */
void CFE_SB_TrackingListRemove(CFE_SB_BufferLink_t *Node)
{
    /* Remove from list */
    Node->Prev->Next = Node->Next;
    Node->Next->Prev = Node->Prev;

    /* The node is now a singleton */
    CFE_SB_TrackingListReset(Node);
}

/******************************************************************************
 *
 * Helper function to add a node to a tracking list
 */
void CFE_SB_TrackingListAdd(CFE_SB_BufferLink_t *List, CFE_SB_BufferLink_t *Node)
{
    /* Connect this node to the list at "prev" position (tail) */
    Node->Prev = List->Prev;
    Node->Next = List;

    /* Connect list nodes to this node */
    Node->Prev->Next = Node;
    Node->Next->Prev = Node;
}


/******************************************************************************
**  Function:   CFE_SB_GetBufferFromPool()
**
**  Purpose:
**    Request a buffer from the SB buffer pool. The SB buffer pool is a
**    pre-allocated block of memory of size CFE_PLATFORM_SB_BUF_MEMORY_BYTES. It is used
**    by the SB to dynamically allocate memory to hold the message and a buffer
**    descriptor associated with the message during the sending of a message.
**
**  Note:
**    This must only be invoked while holding the SB global lock
**
**  Arguments:
**    MaxMsgSize         : Size of the buffer content area in bytes.
**
**  Return:
**    Pointer to the buffer descriptor for the new buffer, or NULL if the buffer
**    could not be allocated.
*/

CFE_SB_BufferD_t *CFE_SB_GetBufferFromPool(size_t MaxMsgSize)
{
    int32             stat1;
    size_t            AllocSize;
    CFE_SB_BufferD_t *bd = NULL;

    /* The allocation needs to include enough space for the descriptor object */
    AllocSize = MaxMsgSize + CFE_SB_BUFFERD_CONTENT_OFFSET;

    /* Allocate a new buffer descriptor from the SB memory pool.*/
    stat1 = CFE_ES_GetPoolBuf((CFE_ES_MemPoolBuf_t *)&bd, CFE_SB_Global.Mem.PoolHdl, AllocSize);
    if (stat1 < 0)
    {
        return NULL;
    }

    /* increment the number of buffers in use and adjust the high water mark if needed */
    CFE_SB_Global.StatTlmMsg.Payload.SBBuffersInUse++;
    if (CFE_SB_Global.StatTlmMsg.Payload.SBBuffersInUse > CFE_SB_Global.StatTlmMsg.Payload.PeakSBBuffersInUse)
    {
        CFE_SB_Global.StatTlmMsg.Payload.PeakSBBuffersInUse = CFE_SB_Global.StatTlmMsg.Payload.SBBuffersInUse;
    } /* end if */

    /* Add the size of the actual buffer to the memory-in-use ctr and */
    /* adjust the high water mark if needed */
    CFE_SB_Global.StatTlmMsg.Payload.MemInUse += AllocSize;
    if (CFE_SB_Global.StatTlmMsg.Payload.MemInUse > CFE_SB_Global.StatTlmMsg.Payload.PeakMemInUse)
    {
        CFE_SB_Global.StatTlmMsg.Payload.PeakMemInUse = CFE_SB_Global.StatTlmMsg.Payload.MemInUse;
    } /* end if */

    /* Initialize the buffer descriptor structure. */
    memset(bd, 0, CFE_SB_BUFFERD_CONTENT_OFFSET);

    bd->MsgId          = CFE_SB_INVALID_MSG_ID;
    bd->UseCount       = 1;
    bd->AllocatedSize  = AllocSize;

    CFE_SB_TrackingListReset(&bd->Link);

    return bd;

} /* CFE_SB_GetBufferFromPool */


/******************************************************************************
**  Function:   CFE_SB_ReturnBufferToPool()
**
**  Purpose:
**    This function will return two blocks of memory back to the memory pool.
**    One block is the memory used to store the actual message, the other block
**    was used to store the buffer descriptor for the message.
**
**  Note:
**    This must only be invoked while holding the SB global lock
**
**  Arguments:
**    bd     : Pointer to the buffer descriptor.
**
**  Return:
**    None
*/
void CFE_SB_ReturnBufferToPool(CFE_SB_BufferD_t *bd)
{
    /* Remove from any tracking list (no effect if not in a list) */
    CFE_SB_TrackingListRemove(&bd->Link);

    --CFE_SB_Global.StatTlmMsg.Payload.SBBuffersInUse;
    CFE_SB_Global.StatTlmMsg.Payload.MemInUse -= bd->AllocatedSize;

    /* finally give the buf descriptor back to the buf descriptor pool */
    CFE_ES_PutPoolBuf(CFE_SB_Global.Mem.PoolHdl, bd);

}/* end CFE_SB_ReturnBufferToPool */


/******************************************************************************
**  Function:   CFE_SB_IncrBufUseCnt()
**
**  Purpose:
**    This function will increment the UseCount of a particular buffer.
**
**  Note:
**    UseCount is a variable in the CFE_SB_BufferD_t and is used only to
**    determine when a buffer may be returned to the memory pool.
**
**    This must only be invoked while holding the SB global lock
**
**  Arguments:
**    bd : Pointer to the buffer descriptor.
**
**  Return:
**    CFE_SUCCESS for normal operation.
*/
void CFE_SB_IncrBufUseCnt(CFE_SB_BufferD_t *bd)
{
    /* range check the UseCount variable */
    if(bd->UseCount < 0x7FFF)
    {
        ++bd->UseCount;
    }

}/* end CFE_SB_DecrBufUseCnt */


/******************************************************************************
**  Function:   CFE_SB_DecrBufUseCnt()
**
**  Purpose:
**    This function will decrement the UseCount of a particular buffer. If the
**    the UseCount is decremented to zero, it will return the buffer to the
**    memory pool.
**
**  Note:
**    UseCount is a variable in the CFE_SB_BufferD_t and is used only to
**    determine when a buffer may be returned to the memory pool.
**
**    This must only be invoked while holding the SB global lock
**
**  Arguments:
**    bd : Pointer to the buffer descriptor.
**
**  Return:
**    CFE_SUCCESS for normal operation.
*/
void CFE_SB_DecrBufUseCnt(CFE_SB_BufferD_t *bd)
{
    /* range check the UseCount variable */
    if(bd->UseCount > 0)
    {
        --bd->UseCount;

        if (bd->UseCount == 0) 
        {
           CFE_SB_ReturnBufferToPool(bd);
        }
    }

}/* end CFE_SB_DecrBufUseCnt */



/******************************************************************************
**  Function:   CFE_SB_GetDestinationBlk()
**
**  Purpose:
**    This function gets a destination descriptor from the SB memory pool.
**
**  Note:
**    This must only be invoked while holding the SB global lock
**
**  Arguments:
**    None
**
**  Return:
**    Pointer to the destination descriptor
*/
CFE_SB_DestinationD_t *CFE_SB_GetDestinationBlk(void)
{
    int32 Stat;
    CFE_SB_DestinationD_t *Dest = NULL;

    /* Allocate a new destination descriptor from the SB memory pool.*/
    Stat = CFE_ES_GetPoolBuf((CFE_ES_MemPoolBuf_t*)&Dest, CFE_SB_Global.Mem.PoolHdl,  sizeof(CFE_SB_DestinationD_t));
    if(Stat < 0){
        return NULL;
    }

    /* Add the size of a destination descriptor to the memory-in-use ctr and */
    /* adjust the high water mark if needed */
    CFE_SB_Global.StatTlmMsg.Payload.MemInUse+=Stat;
    if(CFE_SB_Global.StatTlmMsg.Payload.MemInUse > CFE_SB_Global.StatTlmMsg.Payload.PeakMemInUse){
       CFE_SB_Global.StatTlmMsg.Payload.PeakMemInUse = CFE_SB_Global.StatTlmMsg.Payload.MemInUse;
    }/* end if */

    return Dest;

}/* end CFE_SB_GetDestinationBlk */


/******************************************************************************
**  Function:   CFE_SB_PutDestinationBlk()
**
**  Purpose:
**    This function returns a destination descriptor to the SB memory pool.
**
**  Note:
**    This must only be invoked while holding the SB global lock
**
**  Arguments:
**    None
**
**  Return:
**    Pointer to the destination descriptor
*/
int32 CFE_SB_PutDestinationBlk(CFE_SB_DestinationD_t *Dest)
{
    int32 Stat;

    if(Dest==NULL){
        return CFE_SB_BAD_ARGUMENT;
    }/* end if */

    /* give the destination block back to the SB memory pool */
    Stat = CFE_ES_PutPoolBuf(CFE_SB_Global.Mem.PoolHdl, Dest);
    if(Stat > 0){
        /* Substract the size of the destination block from the Memory in use ctr */
        CFE_SB_Global.StatTlmMsg.Payload.MemInUse-=Stat;
    }/* end if */

    return CFE_SUCCESS;

}/* end CFE_SB_PutDestinationBlk */


/*****************************************************************************/
