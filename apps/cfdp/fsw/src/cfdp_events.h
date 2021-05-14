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
#define CFDP_INVALID_MSGID_ERR_EID 5  // Invalid Message ID
#define CFDP_LEN_ERR_EID           6  // Invalid Length
#define CFDP_PIPE_ERR_EID          7  // Pipe Error (Software Bus)

#define CFDP_EVENT_COUNTS 7

#endif 
