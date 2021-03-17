#ifndef _rpi_test_h_
#define _rpi_test_h_

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"

#include "rpi_test_perfids.h"
#include "rpi_test_msgids.h"
#include "rpi_test_msg.h"

/***********************************************************************/
#define RPI_TEST_PIPE_DEPTH 32 /* Depth of the Command Pipe for Application */

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

    /*
    ** Housekeeping telemetry packet...
    */
    RPI_TEST_HkTlm_t HkTlm;

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

    CFE_EVS_BinFilter_t EventFilters[RPI_TEST_EVENT_COUNTS];
    // CFE_TBL_Handle_t    TblHandles[SAMPLE_APP_NUMBER_OF_TABLES];

} RPI_TEST_Data_t;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (SAMPLE_APP_Main), these
**       functions are not called from any other source module.
*/
void  RPI_TEST_Main(void);
int32 RPI_TEST_Init(void);
// void  SAMPLE_APP_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr);
// void  SAMPLE_APP_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr);
// int32 SAMPLE_APP_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg);
// int32 SAMPLE_APP_ResetCounters(const SAMPLE_APP_ResetCountersCmd_t *Msg);
// int32 SAMPLE_APP_Process(const SAMPLE_APP_ProcessCmd_t *Msg);
// int32 SAMPLE_APP_Noop(const SAMPLE_APP_NoopCmd_t *Msg);
// void  SAMPLE_APP_GetCrc(const char *TableName);


// bool SAMPLE_APP_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);

#endif /* _sample_app_h_ */
