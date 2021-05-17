
#ifndef _transaction_h_
#define _transaction_h_

#include "cfe.h"
#include "cfdp_config.h"
#include "pdu.h"

typedef struct {
    
    uint8 NumPDUSent;
    uint64 FileSize;
    const char SourceFilename[OS_MAX_PATH_LEN];
    const char DestFilename[OS_MAX_PATH_LEN];
    uint8 PeerEntityId;
    PDU_Header_t Header;
    char Data[MAX_PDU_SIZE]; 

} CFDP_TransactionNode_t;

#endif