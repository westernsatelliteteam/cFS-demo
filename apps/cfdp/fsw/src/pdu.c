#include "cfe.h"
#include "pdu.h"
#include <string.h>

void PDU_InitHeader(PDU_Header_t *header, uint8 Type, uint8 Direction, uint8 TxMode, uint8 LargeFileFlag, uint16 NumOctets, uint8 TransOffset, uint8 DestId) {
    header->Flags  = (CFDP_PDU_VERSION & 0x7) << 5;
    header->Flags |= (Type & 0x1) << 4;
    header->Flags |= (Direction & 0x1) << 3;
    header->Flags |= (TxMode & 0x1) << 2;
    header->Flags |= (PDU_CRC_NOT_PRESENT & 0x1) << 1;
    header->Flags |= (LargeFileFlag & 0x1); // Large file flag

    header->PduLength = (NumOctets << 8) | (NumOctets >> 8);

    header->Lengths  = (PDU_SEG_NO_RESPECT_BOUNDS & 0x1) << 7; 
    header->Lengths |= ((CFDP_MAX_CFG_VALUE_OCTETS-1) & 0x7) << 4;  // number of octets less one (zero is one octet)
    header->Lengths |= (PDU_SEG_META_NOT_PRESENT & 0x1) << 3;
    header->Lengths |= ((CDFP_TRANS_LENGTH_OCTETS-1) & 0x7);       // number of octets less one (zero is one octet)


    header->PeerEntityId = CFDP_LOCAL_ID;
    header->TransactionNumber =  TransOffset;
    header->DestinationID = DestId;
}

int32 PDU_CreateMetadata(char *Data, uint8 LargeFileFlag, uint64 FileSize, uint8 SourceLength, const char* SourceFilename, uint8 DestLength, const char* DestFilename) {
    uint8 offset = 0; // offset into buffer
    uint8 FileSizeSensitive = (LargeFileFlag == PDU_LARGE_FILE) ? PDU_LARGE_FILE_OCTETS : PDU_SMALL_FILE_OCTETS;

    Data[offset++] = PDU_FDC_METADATA;
    Data[offset++] = PDU_CLOSURE_NOT_REQ << 6 | PDU_CHECKSUM_UNUSED;

    if(LargeFileFlag) {
        // place endian
        memcpy(&Data[offset], &FileSize, FileSizeSensitive);
    }
    else {
        uint32 FileSize_32 = (int32)FileSize;
        // memcpy(&Data[offset], &FileSize_32, FileSizeSensitive);
        Data[offset++] = (FileSize_32 & 0xff000000UL) >> 24;
        Data[offset++] = (FileSize_32 & 0x00ff0000UL) >> 16;
        Data[offset++] = (FileSize_32 & 0x0000ff00UL) >>  8;
        Data[offset++] = (FileSize_32 & 0x000000ffUL);
    }
    // offset += FileSizeSensitive;

    Data[offset++] = SourceLength;
    strncpy(&Data[offset], SourceFilename, SourceLength);
    offset += SourceLength;

    Data[offset++] = DestLength;
    strncpy(&Data[offset], DestFilename, DestLength);
    offset += DestLength;

    return offset;
}

