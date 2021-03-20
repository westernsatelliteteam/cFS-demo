
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
