/*************************************************************************************/

#ifndef DEPLOII_HANDLER_ESP32_h
#define DEPLOII_HANDLER_ESP32_h

/*************************************************************************************/

/*
 * Include correct handler for medium/protocol
 */

#if DEPLOII_MEDIUM == DEPLOII_MEDIUM_WIFI

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS
#include "handler/esp32/deploii_handler_esp32_wifi_ws.h"
#elif DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_HTTP
#include "handler/esp32/deploii_handler_esp32_wifi_http.h"
#else
#error "Unsupported protocol for ESP32"
#endif // DEPLOII_PROTOCOL

#else
#error "Unsupported medium for ESP32"
#endif // DEPLOII_MEDIUM

/*************************************************************************************/

#endif // DEPLOII_HANDLER_ESP32_h

/*************************************************************************************/
