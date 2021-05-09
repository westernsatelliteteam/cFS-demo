/*
** The various events that the application can
** emit to the software bus
*/

#ifndef _sample_app_events_h_
#define _sample_app_events_h_

#define SAMPLE_APP_RESERVED_EID          0  // Reserved
#define SAMPLE_APP_STARTUP_INF_EID       1  // Startup
#define SAMPLE_APP_COMMAND_ERR_EID       2  // Command Error
#define SAMPLE_APP_COMMANDNOP_INF_EID    3  // Command NO-OP
#define SAMPLE_APP_COMMANDRST_INF_EID    4  // Command Reset
#define SAMPLE_APP_INVALID_MSGID_ERR_EID 5  // Invalid Message ID
#define SAMPLE_APP_LEN_ERR_EID           6  // Invalid Length
#define SAMPLE_APP_PIPE_ERR_EID          7  // Pipe Error (Software Bus)
#define SAMPLE_APP_RPI_ACCESS_EID        8  // Error accessing RPI Hardware
#define SAMPLE_APP_BLINK_INF_EID         9  // Command Blink

#define SAMPLE_APP_EVENT_COUNTS 9

#endif 
