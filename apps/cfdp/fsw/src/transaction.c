#include "transaction.h"



int32 CFDP_GenerateMetadataPDU(CFDP_TransactionNode_t *TransNode, CFDP_TransactionPacket_t *Transaction, uint8 SourceLength, uint8 DestLength) {
    int32 Status;
    os_fstat_t FileStats;

    // check if file exists
    Status = OS_stat(TransNode->SourceFilename, &FileStats);

    if(Status != OS_SUCCESS) {
        return Status;
    }

    TransNode->FileSize = FileStats.FileSize;
    
    // determine if 32 or 64 bit value for filesize
    TransNode->LargeFileFlag = (TransNode->FileSize >= 0xFFFFFFFF ? PDU_LARGE_FILE : PDU_SMALL_FILE);

    // create the metadata PDU and collect it's length for the header
    uint8 PduDataLength = PDU_CreateMetadata((char*)&TransNode->Data, TransNode->LargeFileFlag,
                                TransNode->FileSize, SourceLength, TransNode->SourceFilename,
                                DestLength, TransNode->DestFilename);

    // create the header
    PDU_InitHeader(&TransNode->Header, PDU_TYPE_DIRECTIVE, PDU_TYPE_TO_RECV, PDU_TRANS_MODE_NAK,
                    TransNode->LargeFileFlag, PduDataLength, TransNode->TransNumber, TransNode->PeerEntityId);


    // copy the header and metadata over to the transaction payload
    memset(&Transaction->Payload, 0, sizeof(Transaction->Payload));
    memcpy(&Transaction->Payload, &TransNode->Header, sizeof(TransNode->Header));
    memcpy(&Transaction->Payload[sizeof(TransNode->Header)], &TransNode->Data, PduDataLength);

    return Status;
}

int32 CFDP_CalculateChecksum(CFDP_TransactionNode_t *TransNode) {
    int32 Status;
    osal_id_t FileHandle;

    Status = OS_OpenCreate(&FileHandle, TransNode->SourceFilename, OS_FILE_FLAG_NONE, OS_READ_ONLY);

    if(Status != OS_SUCCESS) {
        return Status;
    }

    uint8 buffer[4];
    while((Status = OS_read(FileHandle, &buffer, 4)) > 0) {
        if(Status < 4) {
            for(int i = 3; i >= Status; i--) {
                buffer[i] = 0;
            }
        }
        TransNode->Checksum += buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
    }

    OS_close(FileHandle);

    if(Status != OS_SUCCESS) {
        return Status;
    }

    return Status;
    
}

int32 CFDP_GenerateFiledataPDU(CFDP_TransactionNode_t *TransNode, CFDP_TransactionPacket_t *Transaction, uint64 FileOffset) {
    uint32 Status;
    uint64 PduDataLength;
    osal_id_t FileHandle;
    uint32 DataBufferStartLocation;

    DataBufferStartLocation = PDU_CreateFiledata((char*)&TransNode->Data,
                        TransNode->LargeFileFlag, FileOffset);

    Status = OS_OpenCreate(&FileHandle, TransNode->SourceFilename, OS_FILE_FLAG_NONE, OS_READ_ONLY);

    if(Status != OS_SUCCESS) {
        OS_printf("Open error\n");
        return Status;
    }

    Status = OS_lseek(FileHandle, FileOffset, OS_SEEK_SET);
    
    if(Status < 0) {
        OS_printf("Seek error (EID=%d)\n", Status);
        return Status;
    }
    else {
        Status = OS_SUCCESS;
    }

    uint64 OctetsToRead = (TransNode->FileSize - FileOffset > MAX_DATAFILE_SIZE) ?
                                MAX_DATAFILE_SIZE : TransNode->FileSize - FileOffset;
    Status = OS_read(FileHandle, &TransNode->Data[DataBufferStartLocation], OctetsToRead);

    if(Status < 0) {
        OS_printf("Read error\n");
        OS_close(FileHandle);
        return Status;
    }
    else {
        Status = OS_SUCCESS;
    }

    OS_close(FileHandle);

    PduDataLength = OctetsToRead + (TransNode->LargeFileFlag == PDU_LARGE_FILE ? 8 : 4);

    PDU_InitHeader(&TransNode->Header, PDU_TYPE_DATA, PDU_TYPE_TO_RECV, PDU_TRANS_MODE_NAK,
                    TransNode->LargeFileFlag, PduDataLength, TransNode->TransNumber, TransNode->PeerEntityId);

    memset(&Transaction->Payload, 0, sizeof(Transaction->Payload));
    memcpy(&Transaction->Payload, &TransNode->Header, sizeof(TransNode->Header));
    memcpy(&Transaction->Payload[sizeof(TransNode->Header)], &TransNode->Data, PduDataLength);

    return Status;
}

int32 CFDP_GenerateEofPDU(CFDP_TransactionNode_t *TransNode, CFDP_TransactionPacket_t *Transaction) {

    uint8 PduDataLength = PDU_CreateEOFdata((char*)&TransNode->Data, TransNode->Checksum,
                                TransNode->LargeFileFlag, TransNode->FileSize);

    PDU_InitHeader(&TransNode->Header, PDU_TYPE_DIRECTIVE, PDU_TYPE_TO_RECV, PDU_TRANS_MODE_NAK,
                    TransNode->LargeFileFlag, PduDataLength, TransNode->TransNumber, TransNode->PeerEntityId);

    memset(&Transaction->Payload, 0, sizeof(Transaction->Payload));
    memcpy(&Transaction->Payload, &TransNode->Header, sizeof(TransNode->Header));
    memcpy(&Transaction->Payload[sizeof(TransNode->Header)], &TransNode->Data, PduDataLength);

    return CFE_SUCCESS;
}

uint32 CFDP_CalcNumPackets(uint64 Filesize) {
    return (Filesize / MAX_DATAFILE_SIZE) + ((Filesize % MAX_DATAFILE_SIZE) != 0);
}