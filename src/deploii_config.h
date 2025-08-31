/*************************************************************************************/

#ifndef DEPLOII_CONFIG_h
#define DEPLOII_CONFIG_h

/*************************************************************************************/

#include "Arduino.h"

/*************************************************************************************/

// Mediums
#define DEPLOII_MEDIUM_WIFI 0
#define DEPLOII_MEDIUM_NARROWBAND 1

// Protocols
#define DEPLOII_PROTOCOL_WEBSOCKETS 0
#define DEPLOII_PROTOCOL_HTTP 1
#define DEPLOII_PROTOCOL_MQTT 2

typedef enum
{
  DEPLOII_DEBUG_NONE,
  DEPLOII_DEBUG_INFO,
  DEPLOII_DEBUG_VERBOSE,
} deploii_debug_t;

/*************************************************************************************/

// type of deploii_medium_t
#ifndef DEPLOII_MEDIUM // default medium
#define DEPLOII_MEDIUM DEPLOII_MEDIUM_WIFI
#endif // !DEPLOII_MEDIUM
static_assert(DEPLOII_MEDIUM >= DEPLOII_MEDIUM_WIFI && DEPLOII_MEDIUM <= DEPLOII_MEDIUM_NARROWBAND,
              "-E- invalid Deploii medium");

// type of deploii_protocol_t
#ifndef DEPLOII_PROTOCOL // default protocol
#define DEPLOII_PROTOCOL DEPLOII_PROTOCOL_WEBSOCKETS
#endif // !DEPLOII_PROTOCOL
static_assert(DEPLOII_PROTOCOL >= DEPLOII_PROTOCOL_WEBSOCKETS && DEPLOII_PROTOCOL <= DEPLOII_PROTOCOL_MQTT,
              "-E- invalid Deploii protocol");

#ifndef DEPLOII_DEBUG // default debugging logging is info
#define DEPLOII_DEBUG DEPLOII_DEBUG_INFO
#endif // !DEPLOII_DEBUG
static_assert(DEPLOII_DEBUG >= DEPLOII_DEBUG_NONE && DEPLOII_DEBUG <= DEPLOII_DEBUG_VERBOSE,
              "-E- invalid Deploii debug logging level");

#ifndef DEPLOII_HOST
#define DEPLOII_HOST "deploii.no"
#endif // !DEPLOII_HOST

#ifndef DEPLOII_PORT
#define DEPLOII_PORT 443
#endif // !DEPLOII_PORT

#ifndef DEPLOII_PORT_NO_SSL
#define DEPLOII_PORT_NO_SSL 80
#endif // !DEPLOII_PORT_NO_SSL

#ifndef DEPLOII_WS_URL
#define DEPLOII_WS_URL "/mcu/ws"
#endif // !DEPLOII_WS_URL

#ifndef DEPLOII_HTTP_URL
#define DEPLOII_HTTP_URL "/mcu/data"
#endif // !DEPLOII_HTTP_URL

#ifndef DEPLOII_SSL
#if (defined(ARDUINO_UNOR4_WIFI) && (DEPLOII_PROTOCOL == DEPLOII_PROTOCOL_WEBSOCKETS))
#define DEPLOII_SSL false
#else
#define DEPLOII_SSL true
#endif
#endif // !DEPLOII_SSL

/*************************************************************************************/

#define DEPLOII_SERIAL_BAUDRATE 115200
#define DEPLOII_MAX_INTERVALS 10
#define DEPLOII_WIFI_RECONNECT_TIME 1000
#define DEPLOII_WS_RECONNECT_TIME 2000
#define DEPLOII_HTTP_RESPONSE_TIMEOUT 5000

/*************************************************************************************/

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

/*************************************************************************************/

#endif // !DEPLOII_CONSTANSTS_h

/*************************************************************************************/
