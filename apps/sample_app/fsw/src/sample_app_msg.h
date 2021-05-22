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
#define SAMPLE_APP_BLINK_CC          2
#define SAMPLE_APP_TAKE_PICTURE_CC   3

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
typedef SAMPLE_APP_NoArgsCmd_t SAMPLE_APP_BlinkCmd_t;

typedef struct {
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
    char filepath[OS_MAX_PATH_LEN]; /**< \brief Path to save the picture to */
} SAMPLE_APP_PictureCmd_t;

/*************************************************************************/
/*
** Type definition (SAMPLE App housekeeping)
*/

typedef struct {
    uint8 CommandCounter;
    uint8 CommandErrorCounter;
    uint8 ledStatus;
    uint8 spare[1];
    char last_picture[OS_MAX_PATH_LEN];
} SAMPLE_APP_HkTlm_Payload_t;

typedef struct {
    CFE_MSG_TelemetryHeader_t  TlmHeader; /**< \brief Telemetry header */
    SAMPLE_APP_HkTlm_Payload_t Payload;   /**< \brief Telemetry payload */
} SAMPLE_APP_HkTlm_t;

#endif 