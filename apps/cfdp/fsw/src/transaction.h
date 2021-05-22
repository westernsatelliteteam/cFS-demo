
#ifndef _transaction_h_
#define _transaction_h_

#include "cfe.h"
#include "cfdp_config.h"
#include "pdu.h"
#include "cfdp_msg.h"

typedef struct {
    uint8 TransNumber;
    uint8 NumPDUSent;
    uint64 FileSize;
    uint32 Checksum;
    const char SourceFilename[OS_MAX_PATH_LEN];
    size_t SourceLength;
    const char DestFilename[OS_MAX_PATH_LEN];
    size_t DestLength;
    uint8 LargeFileFlag;
    uint8 PeerEntityId;
    PDU_Header_t Header;
    char Data[MAX_PDU_SIZE]; 

} CFDP_TransactionNode_t;

int32 CFDP_GenerateMetadataPDU(CFDP_TransactionNode_t *TransNode, CFDP_TransactionPacket_t *Transaction, uint8 SourceLength, uint8 DestLength);
int32 CFDP_CalculateChecksum(CFDP_TransactionNode_t *TransNode);
int32 CFDP_GenerateFiledataPDU(CFDP_TransactionNode_t *TransNode, CFDP_TransactionPacket_t *Transaction, uint64 FileOffset);
int32 CFDP_GenerateEofPDU(CFDP_TransactionNode_t *TransNode, CFDP_TransactionPacket_t *Transaction);
uint32 CFDP_CalcNumPackets(uint64 Filesize);

#endif