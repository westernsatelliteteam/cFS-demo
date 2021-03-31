
#include "rpi_lib_version.h"
#include "rpi_lib_internal.h"



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

int32 RPI_Write_File(const char* file, const char* text) {
    FILE *fptr;
    fptr = fopen(file,"w");
    if(fptr == NULL) {
        return CFE_EVS_FILE_WRITE_ERROR;
    }
    fprintf(fptr, "%s", text);
    fclose(fptr);
    return CFE_SUCCESS;
}

int32 RPI_Remove_File(const char* file) {
    return CFE_SUCCESS;
}