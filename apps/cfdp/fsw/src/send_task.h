#ifndef _send_task_h_
#define _send_task_h_

#include "cfe.h"

#define SEND_TASK_NAME "CFDP_Send_Task"
#define SEND_TASK_SEM_START_NAME "CFDP_Start"
#define SEND_TASK_SEM_DONE_NAME "CFDP_Done"
#define SEND_TASK_STACK_SIZE 65535
#define SEND_TASK_PRIORITY 200

int32 CFDP_SendTask_Init(void);
void CFDP_SendTask_Main(void);
int32 CFDP_SendTask_Process(void);


#endif