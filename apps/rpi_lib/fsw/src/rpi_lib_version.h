
/*! @file rpi_lib_version.h
 * @brief Purpose:
 *
 *  The RPI Lib header file containing version information
 *
 */

#ifndef RPI_LIB_VERSION_H
#define RPI_LIB_VERSION_H

/* Development Build Macro Definitions */

#define RPI_LIB_BUILD_NUMBER 1 /*!< Development Build: Number of commits since baseline */
#define RPI_LIB_BUILD_BASELINE \
    "v0.0.1" /*!< Development Build: git tag that is the base for the current development */

/* Version Macro Definitions */

#define RPI_LIB_MAJOR_VERSION 0 /*!< @brief ONLY APPLY for OFFICIAL releases. Major version number. */
#define RPI_LIB_MINOR_VERSION 0 /*!< @brief ONLY APPLY for OFFICIAL releases. Minor version number. */
#define RPI_LIB_REVISION                                                                                         \
    1 /*!< @brief ONLY APPLY for OFFICIAL releases. Revision version number. Value of "99" indicates an unreleased \
          development version.  */
#define RPI_LIB_MISSION_REV 1 /*!< @brief ONLY USED by MISSION Implementations. Mission revision */

#define RPI_LIB_STR_HELPER(x) #x /*!< @brief Helper function to concatenate strings from integer macros */
#define RPI_LIB_STR(x) \
    RPI_LIB_STR_HELPER(x) /*!< @brief Helper function to concatenate strings from integer macros */

/*! @brief Development Build Version Number.
 * @details Baseline git tag + Number of commits since baseline. @n
 * See @ref cfsversions for format differences between development and release versions.
 */
#define RPI_LIB_VERSION RPI_LIB_BUILD_BASELINE "+dev" RPI_LIB_STR(RPI_LIB_BUILD_NUMBER)

/*! @brief Development Build Version String.
 * @details Reports the current development build's baseline, number, and name. Also includes a note about the latest
 * official version. @n See @ref cfsversions for format differences between development and release versions.
 */
#define RPI_LIB_VERSION_STRING                       \
    " RPI LIB DEVELOPMENT BUILD " RPI_LIB_VERSION \
    ", Last Official Release: v0.0.1" /* For full support please use this version */

#endif

