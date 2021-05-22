/*
** The various events that the application can
** emit to the software bus
*/

#ifndef _cfdp_events_h_
#define _cfdp_events_h_

#define CFDP_RESERVED_EID          0  // Reserved
#define CFDP_STARTUP_INF_EID       1  // Startup
#define CFDP_COMMAND_ERR_EID       2  // Command Error
#define CFDP_COMMANDNOP_INF_EID    3  // Command NO-OP
#define CFDP_COMMANDRST_INF_EID    4  // Command Reset
#define CFDP_FILEPLAYBACK_INF_EID  5  // File playback
#define CFDP_META_SEND_INF_EID     6  // Metadata PDU sent
#define CFDP_FILE_SEND_INF_EID     7  // Filedata PDU sent
#define CFDP_FILE_COMPLETE_INF_EID 8  // Filedata PDUs done
#define CFDP_EOF_SEND_INF_EID      9  // EOF PDU sent
#define CFDP_CHILD_INIT_INF_EID    10 // Child init success
#define CFDP_INVALID_MSGID_ERR_EID 11 // Invalid Message ID
#define CFDP_LEN_ERR_EID           12 // Invalid Length
#define CFDP_PIPE_ERR_EID          13 // Pipe Error (Software Bus)
#define CFDP_INVALID_PATH_ERR_EID  14 // File doesn't exist
#define CFDP_FILE_ACCESS_ERR_EID   15 // File could not be accessed
#define CFDP_TX_IN_PROGRESS_EID    16 // Transfer already in progress
#define CFDP_CHILD_INIT_ERR_EID    17 // Child init failed
#define CFDP_SEM_ERR_EID           18 // Error giving or taking semaphore
#define CFDP_CHECKSUM_ERR_EID      19 // Error calculating checksum
#define CFDP_FILE_SEND_ERR_EID     20 // Error generating file PDU

#define CFDP_EVENT_COUNTS 20

#endif 
