/*************************************************************************************/

#ifndef DEPLOII_HANDLER_UNOR4_h
#define DEPLOII_HANDLER_UNOR4_h

/*************************************************************************************/

/*
 * Include correct handler for medium/protocol
 */

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
#include "handler/arduino_uno_r4_wifi/deploii_handler_unoR4_wifi_ws.h"
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#include "handler/arduino_uno_r4_wifi/deploii_handler_unoR4_wifi_http.h"
#endif // DEPLOII_PROTOCOL

#endif // DEPLOII_MEDIUM

/*************************************************************************************/

#endif // DEPLOII_HANDLER_UNOR4_h

/*************************************************************************************/
