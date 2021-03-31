#ifndef _rpi_lib_internal_h_
#define _rpi_lib_internal_h_

// Include all external/public definitions
#include <rpi_lib.h>


/**
 * Library initialization routine/entry point
 */
int32 RPI_LIB_Init(void);

/**
 * Set the value of the onboard LED
 */
int32 RPI_Set_LED(bool state);

/**
 * Write to a file
 */
int32 RPI_Write_File(const char* file, const char* text);

/**
 * Remove a file
 */
int32 RPI_Remove_File(const char* file);

void path_join(char* destination, const char* path1, const char* path2);

#endif

