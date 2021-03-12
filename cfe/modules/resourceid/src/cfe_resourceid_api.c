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

/*
**  File:
**  cfe_resource_api.c
**
**  Purpose:
**      Function definitions related to CFE resource management
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**     cFE Flight Software Application Developers Guide
*/

/*
** Includes
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cfe.h"
#include "cfe_resourceid_api.h"
#include "cfe_resourceid_basevalue.h"


/*********************************************************************/
/*
 * CFE_ResourceId_GetBase
 *
 * For complete API information, see prototype in header
 */
uint32 CFE_ResourceId_GetBase(CFE_ResourceId_t ResourceId)
{
    return (CFE_ResourceId_ToInteger(ResourceId) & ~((uint32)CFE_RESOURCEID_MAX));
}

/*********************************************************************/
/*
 * CFE_ResourceId_GetSerial
 *
 * For complete API information, see prototype in header
 */
uint32 CFE_ResourceId_GetSerial(CFE_ResourceId_t ResourceId)
{
    return (CFE_ResourceId_ToInteger(ResourceId) & ((uint32)CFE_RESOURCEID_MAX));
}


/*********************************************************************/
/*
 * CFE_ResourceId_ToIndex
 *
 * For complete API information, see prototype in header
 */
int32 CFE_ResourceId_ToIndex(CFE_ResourceId_t Id, uint32 BaseValue, uint32 TableSize, uint32 *Idx)
{
    uint32 Serial;

    if (Idx == NULL)
    {
        return CFE_ES_ERR_BUFFER;
    }

    Serial = CFE_ResourceId_ToInteger(Id) - BaseValue;

    if (Serial > CFE_RESOURCEID_MAX || TableSize == 0)
    {
        return CFE_ES_ERR_RESOURCEID_NOT_VALID;
    }

    *Idx = Serial % TableSize;
    return CFE_SUCCESS;
}

/*********************************************************************/
/*
 * CFE_ResourceId_FindNext
 *
 * For complete API information, see prototype in header
 */
CFE_ResourceId_t CFE_ResourceId_FindNext(CFE_ResourceId_t StartId, uint32 TableSize, bool (*CheckFunc)(CFE_ResourceId_t))
{
    uint32           Serial;
    uint32           Count;
    uint32           ResourceType;
    CFE_ResourceId_t CheckId;
    bool             IsTaken;

    ResourceType = CFE_ResourceId_GetBase(StartId);
    Serial       = CFE_ResourceId_GetSerial(StartId);

    Count   = TableSize;
    IsTaken = true;

    do
    {
        if (Count == 0)
        {
            CheckId = CFE_RESOURCEID_UNDEFINED;
            break;
        }

        --Count;
        ++Serial;
        if (Serial >= CFE_RESOURCEID_MAX)
        {
            Serial %= TableSize;
        }

        CheckId = CFE_ResourceId_FromInteger(ResourceType + Serial);
        IsTaken = CheckFunc(CheckId);
    } while (IsTaken);

    return CheckId;
}
