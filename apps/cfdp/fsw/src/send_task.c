#include "send_task.h"
#include "cfdp.h"

int32 CFDP_SendTask_Init(void) {
    int32 Status;
    
    Status = CFE_ES_RegisterChildTask();

    if(Status != CFE_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_CHILD_INIT_ERR_EID, CFE_EVS_EventType_ERROR,
                            "CFDP: Child task creation failure (EID=%d)", Status);
        return Status;
    }

    return CFE_SUCCESS;
}

void CFDP_SendTask_Main(void) {
    int32 Status;

    Status = CFDP_SendTask_Init();

    if(Status != OS_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_CHILD_INIT_ERR_EID, CFE_EVS_EventType_ERROR,
                            "CFDP: Child task init failure (EID=%d)", Status);
        return;
    }

    CFE_EVS_SendEvent(CFDP_CHILD_INIT_INF_EID, CFE_EVS_EventType_INFORMATION,
                            "CFDP: Child task initalized");

    while(Status == OS_SUCCESS) {
        // wait for signal from main thread to start processing
        Status = OS_BinSemTake(CFDP_Data.SendTaskStartSem);
        if(Status != OS_SUCCESS) {
            CFE_EVS_SendEvent(CFDP_COMMANDNOP_INF_EID, CFE_EVS_EventType_ERROR,
                            "CFDP: Child task init failure (EID=%d)", Status);
            continue;
        }

        // start processing
        CFDP_SendTask_Process();

        // signal parent that task is complete
        Status = OS_BinSemGive(CFDP_Data.SendTaskDoneSem);
        if(Status != OS_SUCCESS) {
            CFE_EVS_SendEvent(CFDP_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                            "CFDP: Child task sem give error (EID=%d)", Status);
            continue;
        }
    }

    // error has occured, cleanup task
    CFE_ES_ExitChildTask();

}

int32 CFDP_SendTask_Process(void) {
    int32 Status;

    // generate metadata pdu to initiate downlink
    Status = CFDP_GenerateMetadataPDU(&CFDP_Data.TransNode, &CFDP_Data.Transaction,
                            CFDP_Data.TransNode.SourceLength, CFDP_Data.TransNode.DestLength);

    if(Status == OS_INVALID_POINTER) {
        CFE_EVS_SendEvent(CFDP_INVALID_PATH_ERR_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Invalid Filepath %s", CFDP_Data.TransNode.SourceFilename);
        return Status;
    }
    else if(Status != OS_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_FILE_ACCESS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Error accessing file (EID=%d) %s", Status, CFDP_Data.TransNode.SourceFilename);
        return Status;
    }

    // send the metadata PDU
    CFE_SB_TimeStampMsg(&CFDP_Data.Transaction.TlmHeader.Msg);
    CFE_SB_TransmitMsg(&CFDP_Data.Transaction.TlmHeader.Msg, true);

    CFE_EVS_SendEvent(CFDP_META_SEND_INF_EID, CFE_EVS_EventType_INFORMATION, "CFDP: Sent metadata pdu for %s to destination %d",
                        CFDP_Data.TransNode.SourceFilename, CFDP_Data.TransNode.PeerEntityId);


    // calculate the checksum for later (would rather fail now than later)
    Status = CFDP_CalculateChecksum(&CFDP_Data.TransNode);

    if(Status != OS_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_CHECKSUM_ERR_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Error calculating checksum (EID=%d) %s", Status, CFDP_Data.TransNode.SourceFilename);
        return Status;
    }

    // packetize and send the PDUs
    uint32 NumPackets = CFDP_CalcNumPackets(CFDP_Data.TransNode.FileSize);

    int64 FileOffset = 0;

    for(int i = 0; i < NumPackets; i++, FileOffset += MAX_DATAFILE_SIZE) {
        Status = CFDP_GenerateFiledataPDU(&CFDP_Data.TransNode, &CFDP_Data.Transaction, FileOffset);

        if(Status != OS_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_FILE_SEND_ERR_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Error generating data pdu for %s", CFDP_Data.TransNode.SourceFilename);
            return Status;
        }

        CFE_SB_TimeStampMsg(&CFDP_Data.Transaction.TlmHeader.Msg);
        CFE_SB_TransmitMsg(&CFDP_Data.Transaction.TlmHeader.Msg, true);

        CFE_EVS_SendEvent(CFDP_FILE_SEND_INF_EID, CFE_EVS_EventType_DEBUG,
                          "CFDP: Sent data pdu for %s", CFDP_Data.TransNode.SourceFilename);

        OS_TaskDelay(100);
    }

    CFE_EVS_SendEvent(CFDP_FILE_COMPLETE_INF_EID, CFE_EVS_EventType_INFORMATION,
                        "CFDP: Sent %d filedata pdus for %s to destination %d",
                                NumPackets, CFDP_Data.TransNode.SourceFilename, CFDP_Data.TransNode.PeerEntityId);


    // generate the End-Of-File PDU to indicate we are done with the transfer
    Status = CFDP_GenerateEofPDU(&CFDP_Data.TransNode, &CFDP_Data.Transaction);

    if(Status != CFE_SUCCESS) {
        CFE_EVS_SendEvent(CFDP_FILE_ACCESS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "CFDP: Error generating EOF PDU (EID=%d) %s",
                          Status, CFDP_Data.TransNode.SourceFilename);
        return Status;
    }

    CFE_SB_TimeStampMsg(&CFDP_Data.Transaction.TlmHeader.Msg);
    CFE_SB_TransmitMsg(&CFDP_Data.Transaction.TlmHeader.Msg, true);

    CFE_EVS_SendEvent(CFDP_EOF_SEND_INF_EID, CFE_EVS_EventType_INFORMATION, "CFDP: Sent EOF for %s to destination %d",
                            CFDP_Data.TransNode.SourceFilename, CFDP_Data.TransNode.PeerEntityId);

    return CFE_SUCCESS;
}