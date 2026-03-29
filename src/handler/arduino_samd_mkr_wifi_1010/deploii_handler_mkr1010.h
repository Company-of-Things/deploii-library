/*************************************************************************************/

#ifndef DEPLOII_HANDLER_MKR1010_h
#define DEPLOII_HANDLER_MKR1010_h

/*************************************************************************************/

/*
 * Include correct handler for medium/protocol
 */

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
#include "handler/arduino_samd_mkr_wifi_1010/deploii_handler_mkr1010_wifi_ws.h"
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#include "handler/arduino_samd_mkr_wifi_1010/deploii_handler_mkr1010_wifi_http.h"
#endif // DEPLOII_PROTOCOL

#endif // DEPLOII_MEDIUM

/*************************************************************************************/

#endif // DEPLOII_HANDLER_MKR1010_h

/*************************************************************************************/
