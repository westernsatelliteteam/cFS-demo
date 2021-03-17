#ifndef _rpi_test_msg_h_
#define _rpi_test_msg_h_

/*
** SAMPLE App command codes
*/
#define RPI_TEST_NOOP_CC           0
#define RPI_TEST_RESET_COUNTERS_CC 1
#define RPI_TEST_PROCESS_CC        2

/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} RPI_TEST_NoArgsCmd_t;

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef RPI_TEST_NoArgsCmd_t RPI_TEST_NoopCmd_t;
typedef RPI_TEST_NoArgsCmd_t RPI_TEST_ResetCountersCmd_t;
typedef RPI_TEST_NoArgsCmd_t RPI_TEST_ProcessCmd_t;

/*************************************************************************/
/*
** Type definition (SAMPLE App housekeeping)
*/

typedef struct
{
    uint8 CommandErrorCounter;
    uint8 CommandCounter;
    uint8 spare[2];
} RPI_TEST_HkTlm_Payload_t;

typedef struct
{
    CFE_MSG_TelemetryHeader_t  TlmHeader; /**< \brief Telemetry header */
    RPI_TEST_HkTlm_Payload_t Payload;   /**< \brief Telemetry payload */
} RPI_TEST_HkTlm_t;

#endif /* _rpi_test_msg_h_ */
