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
 * Take a picture and save it
 */
int32 RPI_Take_Picture(const char* filename);

#endif

