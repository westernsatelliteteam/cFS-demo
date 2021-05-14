
/*! @file cfdp_version.h
 * @brief Purpose:
 *
 *  The CFDP header file containing version information
 *
 */

#ifndef CFDP_VERSION_H
#define CFDP_VERSION_H

/* Development Build Macro Definitions */

#define CFDP_BUILD_NUMBER 1 /*!< Development Build: Number of commits since baseline */
#define CFDP_BUILD_BASELINE \
    "v0.0.1" /*!< Development Build: git tag that is the base for the current development */

/* Version Macro Definitions */

#define CFDP_MAJOR_VERSION 0  /*!< @brief ONLY APPLY for OFFICIAL releases. Major version number. */
#define CFDP_MINOR_VERSION 0  /*!< @brief ONLY APPLY for OFFICIAL releases. Minor version number. */
#define CFDP_REVISION      1  /*!< @brief ONLY APPLY for OFFICIAL releases. Revision version number. */
#define CFDP_MISSION_REV   0  /*!< @brief ONLY USED by MISSION Implementations. Mission revision */

#define CFDP_STR_HELPER(x) #x /*!< @brief Helper function to concatenate strings from integer macros */
#define CFDP_STR(x) \
    CFDP_STR_HELPER(x) /*!< @brief Helper function to concatenate strings from integer macros */

/*! @brief Development Build Version Number.
 * @details Baseline git tag + Number of commits since baseline. @n
 * See @ref cfsversions for format differences between development and release versions.
 */
#define CFDP_VERSION CFDP_BUILD_BASELINE "+dev" CFDP_STR(CFDP_BUILD_NUMBER)

/*! @brief Development Build Version String.
 * @details Reports the current development build's baseline, number, and name. Also includes a note about the latest
 * official version. @n See @ref cfsversions for format differences between development and release versions.
 */
#define CFDP_VERSION_STRING                       \
    " CFDP DEVELOPMENT BUILD " CFDP_VERSION \
    ", Last Official Release: v0.0.1" /* For full support please use this version */

#endif 
