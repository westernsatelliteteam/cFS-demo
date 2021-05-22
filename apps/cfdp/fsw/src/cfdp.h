/*******************************************************************************
**
** File: cfdp.h
**
** Purpose:
**   This file is main hdr file for the CFDP application.
**
**
*******************************************************************************/

#ifndef _cfdp_h_
#define _cfdp_h_

/*
** Required header files.
*/
#include "cfe.h"
#include "common_types.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"
#include "cfe_msg_api.h"

#include "cfdp_perfids.h"
#include "cfdp_msgids.h"
#include "cfdp_msg.h"
#include "cfdp_events.h"
#include "pdu.h"
#include "transaction.h"

/***********************************************************************/
#define CFDP_PIPE_DEPTH 32 /* Depth of the Command Pipe for Application */

#define CFDP_NUMBER_OF_TABLES 1 /* Number of Table(s) */

/* Define filenames of default data images for tables */
#define CFDP_TABLE_FILE "/cf/sample_app_tbl.tbl"

#define CFDP_TABLE_OUT_OF_RANGE_ERR_CODE -1

#define CFDP_TBL_ELEMENT_1_MAX 10
/************************************************************************
** Type Definitions
*************************************************************************/

/*
** Global Data
*/
typedef struct
{
    /*
    ** Command interface counters...
    */
    uint8 CmdCounter;
    uint8 ErrCounter;
    uint8 TransactionCounter;

    /*
    ** Housekeeping telemetry packet...
    */
    CFDP_HkTlm_t HkTlm;

    CFDP_TransactionPacket_t Transaction;
    CFDP_TransactionNode_t TransNode;
    CFE_ES_TaskId_t SendTaskId;
    osal_id_t SendTaskStartSem;
    osal_id_t SendTaskDoneSem;

    /*
    ** Run Status variable used in the main processing loop
    */
    uint32 RunStatus;

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_PipeId_t CommandPipe;

    /*
    ** Initialization data (not reported in housekeeping)...
    */
    char   PipeName[CFE_MISSION_MAX_API_LEN];
    uint16 PipeDepth;

    CFE_EVS_BinFilter_t EventFilters[CFDP_EVENT_COUNTS];
    CFE_TBL_Handle_t    TblHandles[CFDP_NUMBER_OF_TABLES];

} CFDP_Data_t;

extern CFDP_Data_t CFDP_Data;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (CFDP_Main), these
**       functions are not called from any other source module.
*/
void  CFDP_Main(void);
int32 CFDP_Init(void);
void  CFDP_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr);
void  CFDP_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr);
int32 CFDP_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg);
int32 CFDP_ResetCounters(const CFDP_ResetCountersCmd_t *Msg);
int32 CFDP_Noop(const CFDP_NoopCmd_t *Msg);
int32 CFDP_PlaybackFile(const CF_PlaybackFileCmd_t *Msg);
int32 CFDP_ProcessPDU(CFE_SB_Buffer_t *SBBufPtr);

int32 CFDP_TblValidationFunc(void *TblData);

bool CFDP_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);

#endif
