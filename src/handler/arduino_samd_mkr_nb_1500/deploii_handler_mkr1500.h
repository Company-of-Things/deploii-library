/*************************************************************************************/

#ifndef DEPLOII_HANDLER_MKR1500_h
#define DEPLOII_HANDLER_MKR1500_h

/*************************************************************************************/

/*
 * Include correct handler for medium/protocol
 */

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_NARROWBAND

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#include "handler/arduino_samd_mkr_nb_1500/deploii_handler_mkr1500_nb_http.h"
#endif // DEPLOII_PROTOCOL

#endif // DEPLOII_MEDIUM

/*************************************************************************************/

#endif // DEPLOII_HANDLER_MKR1500_h

/*************************************************************************************/
