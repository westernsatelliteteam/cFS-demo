
#include "rpi_lib_version.h"
#include "rpi_lib_internal.h"

#include <stdio.h>
#include <stdlib.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RPI_LIB_Init(void) {

    // mocked function so just return successful
    OS_printf("RPI Mock Initialized.%s\n", RPI_LIB_VERSION_STRING);
    return CFE_SUCCESS;

}

int32 RPI_Set_LED(bool state) {   

    // mocked function so just return successful
    return CFE_SUCCESS;

} 

int32 RPI_Take_Picture(const char* filename) {
    int32 Status;
    FILE* fp;
    char filepath[OS_MAX_PATH_LEN + 1] = {'\0'};
    strcat(filepath, (char*)".");
    strcat(filepath, filename);

    fp = fopen(filepath, "w");

    OS_printf("%s\n", filepath);

    if(fp == NULL) {
        return -1;
    }

    Status = fprintf(fp, "Pretend this is a picture!\n");

    if(Status < 0) {
        return -1;
    }

    Status = fclose(fp);

    if(Status < 0) {
        return -1;
    }
    
    return CFE_SUCCESS;
}