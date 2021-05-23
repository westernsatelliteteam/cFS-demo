#include "cfdp_events.h"
#include "cfdp_version.h"
#include "cfdp.h"
#include "cfdp_config.h"
#include "pdu.h"
#include "transaction.h"
#include "send_task.h"

#include <string.h>

CFDP_Data_t CFDP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/* CFDP_Main() -- Application entry point and main process loop               */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void CFDP_Main(void) {
    int32            status;   // return status of function calls
    CFE_SB_Buffer_t *SBBufPtr; // pointer to software bus

    CFE_ES_RegisterApp();

    CFE_ES_PerfLogEntry(CFDP_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = CFDP_Init();
    if (status != CFE_SUCCESS) {
        CFDP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    while (CFE_ES_RunLoop(&CFDP_Data.RunStatus) == true) {

        // Pend forever on receipt of command packet
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, CFDP_Data.CommandPipe, CFE_SB_PEND_FOREVER);
        if(status == CFE_SUCCESS) {
            CFDP_ProcessCommandPacket(SBBufPtr);
        }

        CFE_ES_PerfLogExit(CFDP_PERF_ID);
        CFE_ES_PerfLogEntry(CFDP_PERF_ID);

    }

    CFE_ES_PerfLogExit(CFDP_PERF_ID);

    CFE_ES_ExitApp(CFDP_Data.RunStatus);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* CFDP_Init() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 CFDP_Init(void) {
    int32 status;

    CFDP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    // Initialize app command execution counters
    CFDP_Data.CmdCounter = 0;
    CFDP_Data.ErrCounter = 0;

    // Initialize app configuration data
    CFDP_Data.PipeDepth = CFDP_PIPE_DEPTH;

    strncpy(CFDP_Data.PipeName, "CFDP_CMD_PIPE", sizeof(CFDP_Data.PipeName));
    CFDP_Data.PipeName[sizeof(CFDP_Data.PipeName) - 1] = 0;

    // Initialize event filter table...
    CFDP_Data.EventFilters[0].EventID = CFDP_STARTUP_INF_EID;
    CFDP_Data.EventFilters[0].Mask    = 0x0000;
    CFDP_Data.EventFilters[1].EventID = CFDP_COMMAND_ERR_EID;
    CFDP_Data.EventFilters[1].Mask    = 0x0000;
    CFDP_Data.EventFilters[2].EventID = CFDP_COMMANDNOP_INF_EID;
    CFDP_Data.EventFilters[2].Mask    = 0x0000;
    CFDP_Data.EventFilters[3].EventID = CFDP_COMMANDRST_INF_EID;
    CFDP_Data.EventFilters[3].Mask    = 0x0000;
    CFDP_Data.EventFilters[4].EventID = CFDP_INVALID_MSGID_ERR_EID;
    CFDP_Data.EventFilters[4].Mask    = 0x0000;
    CFDP_Data.EventFilters[5].EventID = CFDP_LEN_ERR_EID;
    CFDP_Data.EventFilters[5].Mask    = 0x0000;
    CFDP_Data.EventFilters[6].EventID = CFDP_PIPE_ERR_EID;
    CFDP_Data.EventFilters[6].Mask    = 0x0000;

    // Register the events
    status = CFE_EVS_Register(CFDP_Data.EventFilters, CFDP_EVENT_COUNTS, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    // Initialize housekeeping packet (clear user data area).
    CFE_MSG_Init(&CFDP_Data.HkTlm.TlmHeader.Msg, CFDP_HK_TLM_MID, sizeof(CFDP_Data.HkTlm));

    CFE_MSG_Init(&CFDP_Data.Transaction.TlmHeader.Msg, CFDP_PDU_OUT_MID, sizeof(CFDP_Data.Transaction)); 

    // Create Software Bus message pipe.
    status = CFE_SB_CreatePipe(&CFDP_Data.CommandPipe, CFDP_Data.PipeDepth, CFDP_Data.PipeName);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    // Subscribe to Housekeeping request commands
    status = CFE_SB_Subscribe(CFDP_SEND_HK_MID, CFDP_Data.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    // Subscribe to ground command packets
    status = CFE_SB_Subscribe(CFDP_CMD_MID, CFDP_Data.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);

        return (status);
    }

    // Subscribe to pdu packets
    status = CFE_SB_Subscribe(CFDP_INCOMING_PDU_MID, CFDP_Data.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error Subscribing to PDU, RC = 0x%08lX\n", (unsigned long)status);

        return (status);
    }

    status = OS_BinSemCreate(&CFDP_Data.SendTaskStartSem, SEND_TASK_SEM_START_NAME, OS_SEM_EMPTY, 0);

    if(status != OS_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error registering child start semaphore, RC = 0x%08lX\n", (unsigned long)status);
    }

    status = OS_BinSemCreate(&CFDP_Data.SendTaskDoneSem, SEND_TASK_SEM_DONE_NAME, OS_SEM_FULL, 0);

    if(status != OS_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error registering child end semaphore, RC = 0x%08lX\n", (unsigned long)status);
    }
    
    status = CFE_ES_CreateChildTask(&CFDP_Data.SendTaskId, SEND_TASK_NAME,
                                        CFDP_SendTask_Main, 0, SEND_TASK_STACK_SIZE,
                                        SEND_TASK_PRIORITY, 0);

    if(status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("CFDP: Error registering child task, RC = 0x%08lX\n", (unsigned long)status);
    }

    CFE_EVS_SendEvent(CFDP_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "CFDP App Initialized.%s",
                      CFDP_VERSION_STRING);

    return (CFE_SUCCESS);

} /* End of CFDP_Init() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  CFDP_ProcessCommandPacket                                          */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the           */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void CFDP_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (MsgId)
    {
        case CFDP_CMD_MID:
            CFDP_ProcessGroundCommand(SBBufPtr);
            break;

        case CFDP_SEND_HK_MID:
            CFDP_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;

        case CFDP_INCOMING_PDU_MID:
            OS_printf("CFDP: RECV MSG!\n");
            CFDP_ProcessPDU(SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(CFDP_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "CFDP: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }

    return;

} /* End CFDP_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* CFDP_ProcessGroundCommand() --                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void CFDP_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr) {
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process "known" CFDP app ground commands
    */
    switch (CommandCode) {
        case CFDP_NOOP_CC:
            if (CFDP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFDP_NoopCmd_t))) {
                CFDP_Noop((CFDP_NoopCmd_t *)SBBufPtr);
            }

            break;

        case CFDP_RESET_COUNTERS_CC:
            if (CFDP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CFDP_ResetCountersCmd_t))) {
                CFDP_ResetCounters((CFDP_ResetCountersCmd_t *)SBBufPtr);
            }

            break;

        case CFDP_PLAYBACK_FILE_CC:
            if (CFDP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(CF_PlaybackFileCmd_t))) {
                CFDP_PlaybackFile((CF_PlaybackFileCmd_t *)SBBufPtr);
            }
            break;

        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(CFDP_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
                              "CFDP: Invalid ground command code: CC = %d", CommandCode);
            break;
    }

    return;

} /* End of CFDP_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  CFDP_ReportHousekeeping                                            */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 CFDP_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    /*
    ** Get command execution counters...
    */
    CFDP_Data.HkTlm.Payload.CommandErrorCounter = CFDP_Data.ErrCounter;
    CFDP_Data.HkTlm.Payload.CommandCounter      = CFDP_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(&CFDP_Data.HkTlm.TlmHeader.Msg);
    CFE_SB_TransmitMsg(&CFDP_Data.HkTlm.TlmHeader.Msg, true);

    // /*
    // ** Manage any pending table loads, validations, etc.
    // */
    // for (i = 0; i < CFDP_NUMBER_OF_TABLES; i++)
    // {
    //     CFE_TBL_Manage(CFDP_Data.TblHandles[i]);
    // }

    return CFE_SUCCESS;

} /* End of CFDP_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* CFDP_Noop -- CFDP NOOP commands                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 CFDP_Noop(const CFDP_NoopCmd_t *Msg)
{

    CFDP_Data.CmdCounter++;

    CFE_EVS_SendEvent(CFDP_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "CFDP: NOOP command %s",
                      CFDP_VERSION);

    return CFE_SUCCESS;

} /* End of CFDP_Noop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  CFDP_ResetCounters                                                 */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 CFDP_ResetCounters(const CFDP_ResetCountersCmd_t *Msg)
{

    CFDP_Data.CmdCounter = 0;
    CFDP_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(CFDP_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "CFDP: RESET command");

    return CFE_SUCCESS;

} /* End of CFDP_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* CFDP_VerifyCmdLength() -- Verify command packet length                     */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool CFDP_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(CFDP_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        CFDP_Data.ErrCounter++;
    }

    return (result);

} /* End of CFDP_VerifyCmdLength() */

int32 CFDP_PlaybackFile(const CF_PlaybackFileCmd_t *Msg) {
    int32 Status;

    // check if a transaction is already in process
    // wait for 0ms - essentially just check if it's available
    Status = OS_BinSemTimedWait(CFDP_Data.SendTaskDoneSem, 0);

    if(Status == OS_SEM_TIMEOUT) {
        CFDP_Data.ErrCounter++;
        CFE_EVS_SendEvent(CFDP_TX_IN_PROGRESS_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Error, file transfer already in progress (EID=%d)", Status);
        
        CFDP_Data.ErrCounter++;

        return Status;
    }

    // init transaction node;
    CFDP_Data.TransNode.NumPDUSent = 0;
    CFDP_Data.TransNode.PeerEntityId = Msg->PeerEntityId;
    CFDP_Data.TransNode.Checksum = 0;
    CFDP_Data.TransNode.TransNumber = CFDP_Data.TransactionCounter++;
    CFDP_Data.TransNode.SourceLength = strlen(Msg->SrcFilename);
    strncpy((char*)CFDP_Data.TransNode.SourceFilename, Msg->SrcFilename, CFDP_Data.TransNode.SourceLength+1); 
    CFDP_Data.TransNode.DestLength = strlen(Msg->DstFilename);
    strncpy((char*)CFDP_Data.TransNode.DestFilename, Msg->DstFilename, CFDP_Data.TransNode.DestLength+1);

    CFE_EVS_SendEvent(CFDP_FILEPLAYBACK_INF_EID, CFE_EVS_EventType_INFORMATION,
                            "CFDP: Request of class %d, channel %d, priority %d, preserve %d - from %d for file %s to dest %s\n",
                            Msg->Class, Msg->Channel, Msg->Priority, Msg->Preserve, Msg->PeerEntityId,
                            CFDP_Data.TransNode.SourceFilename, CFDP_Data.TransNode.DestFilename);

    // let the child task begin processing the transaction
    Status = OS_BinSemGive(CFDP_Data.SendTaskStartSem);
    if(Status != OS_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_SEM_ERR_EID, CFE_EVS_EventType_ERROR, "CFDP: Parent task sem give error (EID=%d)", Status);
        
        CFDP_Data.ErrCounter++;

        return Status;
    }

    CFDP_Data.CmdCounter++;

    return CFE_SUCCESS;
}


// FOR TESTING ONLY -- TO BE REMOVED
int32 CFDP_ProcessPDU(CFE_SB_Buffer_t *SBBufPtr) {
    typedef struct {
        CFE_MSG_CommandHeader_t CmdHeader;
        char payload[128];
    } payload_t;
    payload_t* Msg = (payload_t*)SBBufPtr;

    int i;
    for(i = 0; i < 128; i++) {
        OS_printf("%02X ", (uint8)Msg->payload[i]);
    }
    OS_printf("\n");
    return CFE_SUCCESS;
}
