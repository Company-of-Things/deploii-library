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
#else
#error "Unsupported protocol for Arduino UNO R4 Wifi"
#endif // DEPLOII_PROTOCOL

#elif DEPLOII_MEDIUM == DEPLOII_MEDIUM_BLUETOOTH

#if DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_BLE
#include "handler/arduino_uno_r4_wifi/deploii_handler_unoR4_bluetooth_ble.h"
#else
#error "Unsupported protocol for Arduino UNO R4 Wifi"
#endif // DEPLOII_PROTOCOL

#else
#error "Unsupported medium for Arduino UNO R4 Wifi"
#endif // DEPLOII_MEDIUM

/*************************************************************************************/

#endif // DEPLOII_HANDLER_UNOR4_h

/*************************************************************************************/
