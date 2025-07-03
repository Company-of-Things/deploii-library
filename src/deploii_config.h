/*************************************************************************************/

#ifndef DEPLOII_CONSTANTS_h
#define DEPLOII_CONSTANTS_h

/*************************************************************************************/

typedef enum
{
  DEPLOII_WIFI,
  DEPLOII_NARROWBAND,
} deploii_medium_t;

typedef enum
{
  DEPLOII_WEBSOCKETS,
  DEPLOII_HTTP,
  DEPLOII_MQTT,
} deploii_protocol_t;

#ifndef DEPLOII_MEDIUM // default medium
#define DEPLOII_MEDIUM DEPLOII_WIFI
#endif // !DEPLOII_MEDIUM

#ifndef DEPLOII_PROTOCOL // default protocol
#define DEPLOII_PROTOCOL DEPLOII_WEBSOCKETS
#endif // !DEPLOII_PROTOCOL

#ifndef DEPLOII_DEBUG
#define DEPLOII_DEBUG 0
#endif // !DEPLOII_DEBUG

/*************************************************************************************/

#define DEPLOII_DEBUG_INTERFACE Serial
#define DEPLOII_MAX_INTERVALS 10

/*************************************************************************************/

#define DEPLOII_HOST "deploii.no"
#define DEPLOII_PORT 443
#define DEPLOII_WS_URL "/mcu/ws"
#define DEPLOII_HTTP_URL "/mcu"

#define DEPLOII_WIFI_RECONNECT_TIME 1000
#define DEPLOII_WS_RECONNECT_TIME 2000

/*************************************************************************************/

#endif // !DEPLOII_CONSTANSTS_h

/*************************************************************************************/
