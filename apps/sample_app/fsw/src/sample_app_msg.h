/*
** This defines the structre of the various messages
** the application can send/receive.
** 
** Some have no arguments (NO-OP, Reset, Process) while
** others have associated data/arguments (health keeping)
*/

#ifndef _sample_app_msg_h_
#define _sample_app_msg_h_

/*
** SAMPLE App command codes
*/
#define SAMPLE_APP_NOOP_CC           0
#define SAMPLE_APP_RESET_COUNTERS_CC 1
#define SAMPLE_APP_PROCESS_CC        2
#define SAMPLE_APP_BLINK_CC          3
#define SAMPLE_APP_WRITE_FILE_CC     4

/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct {
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} SAMPLE_APP_NoArgsCmd_t;

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef SAMPLE_APP_NoArgsCmd_t SAMPLE_APP_NoopCmd_t;
typedef SAMPLE_APP_NoArgsCmd_t SAMPLE_APP_ResetCountersCmd_t;
typedef SAMPLE_APP_NoArgsCmd_t SAMPLE_APP_ProcessCmd_t;
typedef SAMPLE_APP_NoArgsCmd_t SAMPLE_APP_BlinkCmd_t;

/*************************************************************************/
/*
** Type definition (SAMPLE App housekeeping)
*/

typedef struct {
    uint8 CommandErrorCounter;
    uint8 CommandCounter;
    uint8 ledStatus;
    uint8 spare[2];
} SAMPLE_APP_HkTlm_Payload_t;

typedef struct {
    CFE_MSG_TelemetryHeader_t  TlmHeader; /**< \brief Telemetry header */
    SAMPLE_APP_HkTlm_Payload_t Payload;   /**< \brief Telemetry payload */
} SAMPLE_APP_HkTlm_t;

/*************************************************************************/
/*
** Type definition (SAMPLE App filewrite)
*/
typedef struct {
    CFE_MSG_CommandHeader_t CmdHeader;
    uint16 length;
    uint16 message[3];
} SAMPLE_APP_File_Payload_t;

#endif 