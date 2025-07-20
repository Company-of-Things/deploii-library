/*************************************************************************************/

#ifndef DEPLOII_CONFIG_h
#define DEPLOII_CONFIG_h

/*************************************************************************************/

#include "Arduino.h"

/*************************************************************************************/

typedef enum
{
  DEPLOII_MEDIUM_WIFI,
  DEPLOII_MEDIUM_NARROWBAND,
} deploii_medium_t;

typedef enum
{
  DEPLOII_PROTOCOL_WEBSOCKETS,
  DEPLOII_PROTOCOL_HTTP,
  DEPLOII_PROTOCOL_MQTT,
} deploii_protocol_t;

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

#ifndef DEPLOII_WS_URL
#define DEPLOII_WS_URL "/mcu/ws"
#endif // !DEPLOII_WS_URL

#ifndef DEPLOII_HTTP_URL
#define DEPLOII_HTTP_URL "/mcu/data"
#endif // !DEPLOII_HTTP_URL

#ifndef DEPLOII_SSL
#define DEPLOII_SSL true
#endif // !DEPLOII_SSL

/*************************************************************************************/

#define DEPLOII_SERIAL_BAUDRATE 115200
#define DEPLOII_MAX_INTERVALS 10
#define DEPLOII_WIFI_RECONNECT_TIME 1000
#define DEPLOII_WS_RECONNECT_TIME 2000

/*************************************************************************************/

#endif // !DEPLOII_CONSTANSTS_h

/*************************************************************************************/
