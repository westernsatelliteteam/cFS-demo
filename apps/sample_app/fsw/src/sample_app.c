#include "sample_app_events.h"
#include "sample_app_version.h"
#include "sample_app.h"
#include "sample_app_table.h"

// Raspberry Pi library
#include "rpi_lib.h"

#include <stdbool.h>

SAMPLE_APP_Data_t SAMPLE_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/* SAMPLE_APP_Main() -- Application entry point and main process loop         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void SAMPLE_APP_Main(void) {
    int32            status;   // return status of function calls
    CFE_SB_Buffer_t *SBBufPtr; // pointer to software bus

    // Register the app with Executive services
    CFE_ES_RegisterApp();

    // Create the first Performance Log entry
    CFE_ES_PerfLogEntry(SAMPLE_APP_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = SAMPLE_APP_Init();
    if (status != CFE_SUCCESS) {
        SAMPLE_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    // SAMPLE Runloop
    while (CFE_ES_RunLoop(&SAMPLE_APP_Data.RunStatus) == true) {

        // Pend on receipt of command packet
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, SAMPLE_APP_Data.CommandPipe, CFE_SB_PEND_FOREVER);
        if(status == CFE_SUCCESS) {
            SAMPLE_APP_ProcessCommandPacket(SBBufPtr);
        }

        // Performance Log Exit Stamp
        CFE_ES_PerfLogExit(SAMPLE_APP_PERF_ID);

        // Performance Log Entry Stamp
        CFE_ES_PerfLogEntry(SAMPLE_APP_PERF_ID);

    }

    // Performance Log Exit Stamp
    CFE_ES_PerfLogExit(SAMPLE_APP_PERF_ID);

    // Exit application
    CFE_ES_ExitApp(SAMPLE_APP_Data.RunStatus);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* SAMPLE_APP_Init() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 SAMPLE_APP_Init(void) {
    int32 status;

    SAMPLE_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    // Initialize app command execution counters
    SAMPLE_APP_Data.CmdCounter = 0;
    SAMPLE_APP_Data.ErrCounter = 0;

    // Initialize app configuration data
    SAMPLE_APP_Data.PipeDepth = SAMPLE_APP_PIPE_DEPTH;

    strncpy(SAMPLE_APP_Data.PipeName, "SAMPLE_APP_CMD_PIPE", sizeof(SAMPLE_APP_Data.PipeName));
    SAMPLE_APP_Data.PipeName[sizeof(SAMPLE_APP_Data.PipeName) - 1] = 0;

    // Initialize event filter table...
    SAMPLE_APP_Data.EventFilters[0].EventID = SAMPLE_APP_STARTUP_INF_EID;
    SAMPLE_APP_Data.EventFilters[0].Mask    = 0x0000;
    SAMPLE_APP_Data.EventFilters[1].EventID = SAMPLE_APP_COMMAND_ERR_EID;
    SAMPLE_APP_Data.EventFilters[1].Mask    = 0x0000;
    SAMPLE_APP_Data.EventFilters[2].EventID = SAMPLE_APP_COMMANDNOP_INF_EID;
    SAMPLE_APP_Data.EventFilters[2].Mask    = 0x0000;
    SAMPLE_APP_Data.EventFilters[3].EventID = SAMPLE_APP_COMMANDRST_INF_EID;
    SAMPLE_APP_Data.EventFilters[3].Mask    = 0x0000;
    SAMPLE_APP_Data.EventFilters[4].EventID = SAMPLE_APP_INVALID_MSGID_ERR_EID;
    SAMPLE_APP_Data.EventFilters[4].Mask    = 0x0000;
    SAMPLE_APP_Data.EventFilters[5].EventID = SAMPLE_APP_LEN_ERR_EID;
    SAMPLE_APP_Data.EventFilters[5].Mask    = 0x0000;
    SAMPLE_APP_Data.EventFilters[6].EventID = SAMPLE_APP_PIPE_ERR_EID;
    SAMPLE_APP_Data.EventFilters[6].Mask    = 0x0000;

    SAMPLE_APP_Data.led_status = 0;

    // Register the events
    status = CFE_EVS_Register(SAMPLE_APP_Data.EventFilters, SAMPLE_APP_EVENT_COUNTS, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("Sample App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    // Initialize housekeeping packet (clear user data area).
    CFE_MSG_Init(&SAMPLE_APP_Data.HkTlm.TlmHeader.Msg, SAMPLE_APP_HK_TLM_MID, sizeof(SAMPLE_APP_Data.HkTlm));

    // Create Software Bus message pipe.
    status = CFE_SB_CreatePipe(&SAMPLE_APP_Data.CommandPipe, SAMPLE_APP_Data.PipeDepth, SAMPLE_APP_Data.PipeName);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("Sample App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    // Subscribe to Housekeeping request commands
    status = CFE_SB_Subscribe(SAMPLE_APP_SEND_HK_MID, SAMPLE_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("Sample App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    // Subscribe to ground command packets
    status = CFE_SB_Subscribe(SAMPLE_APP_CMD_MID, SAMPLE_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("Sample App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);

        return (status);
    }

    // Register Table
    status = CFE_TBL_Register(&SAMPLE_APP_Data.TblHandles[0], "SampleAppTable", sizeof(SAMPLE_APP_Table_t),
                              CFE_TBL_OPT_DEFAULT, SAMPLE_APP_TblValidationFunc);
    if (status != CFE_SUCCESS) {
        CFE_ES_WriteToSysLog("Sample App: Error Registering Table, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }
    else {
        status = CFE_TBL_Load(SAMPLE_APP_Data.TblHandles[0], CFE_TBL_SRC_FILE, SAMPLE_APP_TABLE_FILE);
    }

    // Successful initialization
    CFE_EVS_SendEvent(SAMPLE_APP_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "SAMPLE App Initialized.%s",
                      SAMPLE_APP_VERSION_STRING);

    return (CFE_SUCCESS);

} /* End of SAMPLE_APP_Init() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_APP_ProcessCommandPacket                                    */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the SAMPLE    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void SAMPLE_APP_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (MsgId)
    {
        case SAMPLE_APP_CMD_MID:
            SAMPLE_APP_ProcessGroundCommand(SBBufPtr);
            break;

        case SAMPLE_APP_SEND_HK_MID:
            SAMPLE_APP_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(SAMPLE_APP_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "SAMPLE: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }

    return;

} /* End SAMPLE_APP_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_APP_ProcessGroundCommand() -- SAMPLE ground commands                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void SAMPLE_APP_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr) {
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process "known" SAMPLE app ground commands
    */
    switch (CommandCode) {
        case SAMPLE_APP_NOOP_CC:
            if (SAMPLE_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(SAMPLE_APP_NoopCmd_t))) {
                SAMPLE_APP_Noop((SAMPLE_APP_NoopCmd_t *)SBBufPtr);
            }

            break;

        case SAMPLE_APP_RESET_COUNTERS_CC:
            if (SAMPLE_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(SAMPLE_APP_ResetCountersCmd_t))) {
                SAMPLE_APP_ResetCounters((SAMPLE_APP_ResetCountersCmd_t *)SBBufPtr);
            }

            break;

        case SAMPLE_APP_PROCESS_CC:
            if (SAMPLE_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(SAMPLE_APP_ProcessCmd_t))) {
                SAMPLE_APP_Process((SAMPLE_APP_ProcessCmd_t *)SBBufPtr);
            }

            break;

        case SAMPLE_APP_BLINK_CC:
            if (SAMPLE_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(SAMPLE_APP_BlinkCmd_t))) {
                SAMPLE_APP_Blink((SAMPLE_APP_BlinkCmd_t *)SBBufPtr);
            }

            break;

        case SAMPLE_APP_WRITE_FILE_CC:
            if (SAMPLE_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(SAMPLE_APP_File_Payload_t))) {
                SAMPLE_APP_File((SAMPLE_APP_File_Payload_t *)SBBufPtr);
            }
            break;

        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(SAMPLE_APP_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid ground command code: CC = %d", CommandCode);
            break;
    }

    return;

} /* End of SAMPLE_APP_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_APP_ReportHousekeeping                                          */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 SAMPLE_APP_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    SAMPLE_APP_Data.HkTlm.Payload.CommandErrorCounter = SAMPLE_APP_Data.ErrCounter;
    SAMPLE_APP_Data.HkTlm.Payload.CommandCounter      = SAMPLE_APP_Data.CmdCounter;

    SAMPLE_APP_Data.HkTlm.Payload.ledStatus = SAMPLE_APP_Data.led_status;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(&SAMPLE_APP_Data.HkTlm.TlmHeader.Msg);
    CFE_SB_TransmitMsg(&SAMPLE_APP_Data.HkTlm.TlmHeader.Msg, true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < SAMPLE_APP_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(SAMPLE_APP_Data.TblHandles[i]);
    }

    return CFE_SUCCESS;

} /* End of SAMPLE_APP_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_APP_Noop -- SAMPLE NOOP commands                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 SAMPLE_APP_Noop(const SAMPLE_APP_NoopCmd_t *Msg)
{

    SAMPLE_APP_Data.CmdCounter++;

    CFE_EVS_SendEvent(SAMPLE_APP_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "SAMPLE: NOOP command %s",
                      SAMPLE_APP_VERSION);

    return CFE_SUCCESS;

} /* End of SAMPLE_APP_Noop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_APP_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 SAMPLE_APP_ResetCounters(const SAMPLE_APP_ResetCountersCmd_t *Msg)
{

    SAMPLE_APP_Data.CmdCounter = 0;
    SAMPLE_APP_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(SAMPLE_APP_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "SAMPLE: RESET command");

    return CFE_SUCCESS;

} /* End of SAMPLE_APP_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_APP_Process                                                     */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Ground Station Command                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 SAMPLE_APP_Process(const SAMPLE_APP_ProcessCmd_t *Msg)
{
    int32               status;
    SAMPLE_APP_Table_t *TblPtr;
    const char *        TableName = "SAMPLE_APP.SampleAppTable";

    /* Sample Use of Table */

    status = CFE_TBL_GetAddress((void *)&TblPtr, SAMPLE_APP_Data.TblHandles[0]);

    if (status < CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Sample App: Fail to get table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    CFE_ES_WriteToSysLog("Sample App: Table Value 1: %d  Value 2: %d", TblPtr->Int1, TblPtr->Int2);

    SAMPLE_APP_GetCrc(TableName);

    status = CFE_TBL_ReleaseAddress(SAMPLE_APP_Data.TblHandles[0]);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Sample App: Fail to release table address: 0x%08lx", (unsigned long)status);
        return status;
    }


    return CFE_SUCCESS;

} /* End of SAMPLE_APP_ProcessCC */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_APP_Blink                                                   */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Blink Command                                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 SAMPLE_APP_Blink(const SAMPLE_APP_BlinkCmd_t *Msg) {
    int32 status;

    SAMPLE_APP_Data.CmdCounter++;

    // update led status
    SAMPLE_APP_Data.led_status = !SAMPLE_APP_Data.led_status;

    // Use RPI library to access hardware
    status = RPI_Set_LED(SAMPLE_APP_Data.led_status);
    if(status == CFE_SUCCESS) {
        CFE_EVS_SendEvent(SAMPLE_APP_BLINK_INF_EID, CFE_EVS_EventType_INFORMATION, "SAMPLE: Blink command (%d) %s",
                      SAMPLE_APP_Data.led_status, SAMPLE_APP_VERSION);
    }
    else {
        // Error writing to RPI files
        CFE_EVS_SendEvent(SAMPLE_APP_FILE_ERR_EID, CFE_EVS_EventType_ERROR, "SAMPLE: Unable to write to file");
    }


    return status;

} /* End of SAMPLE_APP_BlinkCC */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  SAMPLE_APP_File                                                    */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process File Command                                 */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 SAMPLE_APP_File(const SAMPLE_APP_File_Payload_t *Msg) {
    SAMPLE_APP_Data.CmdCounter++;

    CFE_EVS_SendEvent(SAMPLE_APP_WRITE_INF_EID, CFE_EVS_EventType_INFORMATION,
            "SAMPLE: File command (%d) %s", Msg->length, SAMPLE_APP_VERSION);
    CFE_EVS_SendEvent(SAMPLE_APP_WRITE_INF_EID, CFE_EVS_EventType_INFORMATION,
            "SAMPLE: File command (%d) %s", Msg->message[0], SAMPLE_APP_VERSION);
    CFE_EVS_SendEvent(SAMPLE_APP_WRITE_INF_EID, CFE_EVS_EventType_INFORMATION,
            "SAMPLE: File command (%d) %s", Msg->message[1], SAMPLE_APP_VERSION);
    CFE_EVS_SendEvent(SAMPLE_APP_WRITE_INF_EID, CFE_EVS_EventType_INFORMATION,
            "SAMPLE: File command (%d) %s", Msg->message[2], SAMPLE_APP_VERSION);


    return CFE_SUCCESS;

} /* End of SAMPLE_APP_FileCC */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* SAMPLE_APP_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool SAMPLE_APP_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
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

        CFE_EVS_SendEvent(SAMPLE_APP_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        SAMPLE_APP_Data.ErrCounter++;
    }

    return (result);

} /* End of SAMPLE_APP_VerifyCmdLength() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SAMPLE_APP_TblValidationFunc -- Verify contents of First Table      */
/* buffer contents                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SAMPLE_APP_TblValidationFunc(void *TblData)
{
    int32               ReturnCode = CFE_SUCCESS;
    SAMPLE_APP_Table_t *TblDataPtr = (SAMPLE_APP_Table_t *)TblData;

    /*
    ** Sample Table Validation
    */
    if (TblDataPtr->Int1 > SAMPLE_APP_TBL_ELEMENT_1_MAX)
    {
        /* First element is out of range, return an appropriate error code */
        ReturnCode = SAMPLE_APP_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;

} /* End of SAMPLE_APP_TBLValidationFunc() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SAMPLE_APP_GetCrc -- Output CRC                                     */
/*                                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SAMPLE_APP_GetCrc(const char *TableName)
{
    int32          status;
    uint32         Crc;
    CFE_TBL_Info_t TblInfoPtr;

    status = CFE_TBL_GetInfo(&TblInfoPtr, TableName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Sample App: Error Getting Table Info");
    }
    else
    {
        Crc = TblInfoPtr.Crc;
        CFE_ES_WriteToSysLog("Sample App: CRC: 0x%08lX\n\n", (unsigned long)Crc);
    }

    return;

} /* End of SAMPLE_APP_GetCrc */
