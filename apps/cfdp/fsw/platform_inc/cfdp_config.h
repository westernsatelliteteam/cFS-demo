
#ifndef _cfdp_config_h_
#define _cfdp_config_h_

#define CF_MAX_CFG_VALUE_CHARS 8 // must be a multiple of 8
#define CFDP_MAX_CFG_VALUE_OCTETS CF_MAX_CFG_VALUE_CHARS/8
#define MAX_PDU_SIZE 1024
#define MAX_DATAFILE_SIZE (MAX_PDU_SIZE - 16)
#define CFDP_LOCAL_ID 24
#define CDFP_TRANS_LENGTH_OCTETS    1

#endif