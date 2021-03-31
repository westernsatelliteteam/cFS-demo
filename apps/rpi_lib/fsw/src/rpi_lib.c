

#include "rpi_lib_version.h"
#include "rpi_lib_internal.h"


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

#include <stdio.h>
#include <string.h>

#ifdef WIN32
    #define DIRECTORY_SEPARATOR "\\"
#else 
    #define DIRECTORY_SEPARATOR "/"
#endif



void path_join(char* destination, const char* path1, const char* path2) {
  if(path1 == NULL && path2 == NULL) {
    strcpy(destination, "");;
  }
  else if(path2 == NULL || strlen(path2) == 0) {
    strcpy(destination, path1);
  }
  else if(path1 == NULL || strlen(path1) == 0) {
    strcpy(destination, path2);
  } 
  else {
    const char *last_char = path1;
    while(*last_char != '\0')
      last_char++;        
    int append_directory_separator = 0;
    if(strcmp(last_char, DIRECTORY_SEPARATOR) != 0) {
      append_directory_separator = 1;
    }
    strcpy(destination, path1);
    if(append_directory_separator)
      strcat(destination, DIRECTORY_SEPARATOR);
    strcat(destination, path2);
  }
}

