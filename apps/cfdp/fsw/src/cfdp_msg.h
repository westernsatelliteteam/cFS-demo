/*
** This defines the structre of the various messages
** the application can send/receive.
** 
** Some have no arguments (NO-OP, Reset, Process) while
** others have associated data/arguments (health keeping)
*/

#ifndef _cfdp_msg_h_
#define _cfdp_msg_h_

#include "cfdp_config.h"
#include "pdu.h"

/*
** CFDP command codes
*/
#define CFDP_NOOP_CC                    0
#define CFDP_RESET_COUNTERS_CC          1
#define CFDP_PLAYBACK_FILE_CC           2

/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct {
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} CFDP_NoArgsCmd_t;



typedef struct {
    CFE_MSG_CommandHeader_t CmdHeader;
    uint8   Class;
    uint8   Channel;
    uint8   Priority;
    uint8   Preserve;
    uint8   PeerEntityId;
    char    SrcFilename[OS_MAX_PATH_LEN];
    char    DstFilename[OS_MAX_PATH_LEN];

} CF_PlaybackFileCmd_t;

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef CFDP_NoArgsCmd_t CFDP_NoopCmd_t;
typedef CFDP_NoArgsCmd_t CFDP_ResetCountersCmd_t;

/*************************************************************************/
/*
** Type definition (SAMPLE App housekeeping)
*/

typedef struct {
    uint8 CommandCounter;
    uint8 CommandErrorCounter;
    uint8 spare[2];
} CFDP_HkTlm_Payload_t;

typedef struct {
    CFE_MSG_TelemetryHeader_t  TlmHeader; /**< \brief Telemetry header */
    CFDP_HkTlm_Payload_t Payload;   /**< \brief Telemetry payload */
} CFDP_HkTlm_t;

typedef struct {
    CFE_MSG_TelemetryHeader_t  TlmHeader; /**< \brief Telemetry header */
    uint8 Payload[MAX_PDU_SIZE + sizeof(PDU_Header_t)];   /**< \brief Telemetry payload */
} CFDP_TransactionPacket_t;

#endif 