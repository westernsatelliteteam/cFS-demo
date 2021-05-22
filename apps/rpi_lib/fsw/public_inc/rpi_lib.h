#ifndef _rpi_lib_h_
#define _rpi_lib_h_

#include "cfe.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
/** \brief Take a picture with the onboard camera
**
**  \par Description
**        Takes a picture and saves it under the given filename
**
**  \par Assumptions, External Events, and Notes:
**        None
**
**  \return Execution status, see \ref CFEReturnCodes
**
**
*************************************************************************/
int32 RPI_Take_Picture(const char* filename);

#endif

