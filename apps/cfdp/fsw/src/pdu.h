#ifndef _pdu_h_
#define _pdu_h_

#include "cfe.h"
#include "cfdp_config.h"

// edianness
#define PDU_LITTLE_ENDIAN   0
#define PDU_BIG_ENDIAN      1
#define PDU_ENDIAN          PDU_LITTLE_ENDIAN

// header flags
#define CFDP_PDU_VERSION                001
#define PDU_TYPE_DIRECTIVE              0
#define PDU_TYPE_DATA                   1
#define PDU_TYPE_TO_RECV                0
#define PDU_TYPE_TO_SENDER              1
#define PDU_TRANS_MODE_ACK              0
#define PDU_TRANS_MODE_NAK              1
#define PDU_CRC_NOT_PRESENT             0
#define PDU_CRC_PRESENT                 1
#define PDU_SMALL_FILE                  0
#define PDU_LARGE_FILE                  1
#define PDU_SMALL_FILE_OCTETS           4
#define PDU_LARGE_FILE_OCTETS           8

// header lengths
#define PDU_SEG_NO_RESPECT_BOUNDS       0
#define PDU_SEG_RESPECT_BOUNDS          1
#define PDU_SEG_META_NOT_PRESENT        0
#define PDU_SEG_META_PRESENT            1

#define PDU_RESERVED                    0

// directive codes
#define PDU_FDC_EOF             0x04
#define PDU_FDC_FINISHED        0x05
#define PDU_FDC_ACK             0x06
#define PDU_FDC_METADATA        0x07
#define PDU_FDC_NAK             0x08
#define PDU_FDC_PROMPT          0x09
#define PDU_FDC_KEEPALIVE       0x0C

// metadata PDU flags
#define PDU_CLOSURE_NOT_REQ     0
#define PDU_CLOSURE_REQ         1
#define PDU_CHECKSUM_UNUSED     0x0

// EOF PDU flags
#define PDU_EOF_CC_SUCCESS      0x00
#define PDU_EOF_CC_ACK_LIM      0x01
#define PDU_EOF_CC_KEEP_LIM     0x02
#define PDU_EOF_CC_INVALID_TX   0x03
#define PDU_EOF_CC_FS_REJECT    0x04
#define PDU_EOF_CC_CS_FAIL      0x05
#define PDU_EOF_CC_SIZE_ERROR   0x05
#define PDU_EOF_CC_NAK_LIM      0x07
#define PDU_EOF_CC_INACTIVITY   0x08
#define PDU_EOF_CC_INVALID_FILE 0x09
#define PDU_EOF_CC_CHECK_LIM    0x0A
#define PDU_EOF_CC_INVALID_CS   0x0B
#define PDU_EOF_CC_SUSPEND      0x0E
#define PDU_EOF_CC_CANCEL       0x0F

typedef struct {
    uint8 Flags;
    uint16 PduLength;
    uint8 Lengths;
    uint8 PeerEntityId;
    uint8 TransactionNumber;
    uint8 DestinationID;
} __attribute__((packed)) PDU_Header_t;


void PDU_InitHeader(PDU_Header_t *header, uint8 Type, uint8 Direction, uint8 TxMode, uint8 LargeFileFlag, uint16 NumOctets, uint8 TransOffset, uint8 DestId);
int32 PDU_CreateMetadata(char *Data, uint8 LargeFileFlag, uint64 FileSize, uint8 SourceLength, const char* SourceFilename, uint8 DestLength, const char* DestFilename);
uint32 PDU_CreateFiledata(char *Data, uint8 LargeFileFlag, uint64 FileOffset);
int32 PDU_CreateEOFdata(char *Data, uint32 Checksum, uint8 LargeFileFlag, uint64 FileSize);


#endif