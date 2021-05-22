

#include "rpi_lib_version.h"
#include "rpi_lib_internal.h"

#include <stdio.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 RPI_LIB_Init(void) {
    FILE *fptr;   

    // open the file
    fptr = fopen("/sys/class/leds/led0/trigger","w");
    if(fptr == NULL) {
        return CFE_EVS_FILE_WRITE_ERROR;
    }

    // overwrite the onboard LED's current function
    fprintf(fptr,"none"); // none allows us to control it
    fclose(fptr);


    OS_printf("RPI Lib Initialized.%s\n", RPI_LIB_VERSION_STRING);

    return CFE_SUCCESS;

} 

int32 RPI_Set_LED(bool state) {  
    FILE *fptr; 

    // open the file
    fptr = fopen("/sys/class/leds/led0/brightness","w");
    if(fptr == NULL) {
        return CFE_EVS_FILE_WRITE_ERROR;
    }

    // set the brightness (state) of the current LED to the required state
    // true == 1 == ON | false == 0 == OFF
    fprintf(fptr, state ? "1" : "0");
    fclose(fptr);

    return CFE_SUCCESS;

}

int32 RPI_Take_Picture(const char* filename) {
    int32 Status;
    FILE* fp;
    char command[OS_MAX_PATH_LEN + 26] = {'\0'};
    strcat(command, (char*)"raspistill -o /home/pi/cfs");
    strcat(command, filename);

    OS_printf("%s\n", command);

    fp = popen(command, "r");

    if(fp == NULL) {
        return -1;
    }

    Status = pclose(fp);

    if(Status == -1) {
		return Status;
	}

    return CFE_SUCCESS;
}

