#ifndef _rpi_lib_h_
#define _rpi_lib_h_

#include "cfe.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILENAME_LENGTH 100
#define MAX_FILEWRITE_LENGTH 1000

/************************************************************************/
/** \brief Library Initialization Function
**
**  \par Description
**        This function is required by CFE to initialize the library
**        It should be specified in the cfe_es_startup.scr file as part
**        of loading this library.  It is not directly invoked by
**        applications.
**
**  \par Assumptions, External Events, and Notes:
**        None
**
**  \return Execution status, see \ref CFEReturnCodes
**
**
*************************************************************************/
int32 RPI_LIB_Init(void);


/************************************************************************/
/** \brief Set the value of the onboard LED
**
**  \par Description
**        Allows to set the RPI's onboard LED to either on (true)
**        or off (false)
**
**  \par Assumptions, External Events, and Notes:
**        None
**
**  \return Execution status, see \ref CFEReturnCodes
**
**
*************************************************************************/
int32 RPI_Set_LED(bool state);

/************************************************************************/
/** \brief Write to a file
**
**  \par Description
**        Allows writing to a local file
**
**  \par Assumptions, External Events, and Notes:
**        None
**
**  \return Execution status, see \ref CFEReturnCodes
**
**
*************************************************************************/
int32 RPI_Write_File(const char* file, const char* text);

/************************************************************************/
/** \brief Remove to a file
**
**  \par Description
**        Allows removal of a local file
**
**  \par Assumptions, External Events, and Notes:
**        None
**
**  \return Execution status, see \ref CFEReturnCodes
**
**
*************************************************************************/
int32 RPI_Remove_File(const char* file);


void path_join(char* destination, const char* path1, const char* path2);

#endif

