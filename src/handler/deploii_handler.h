#ifndef DEPLOII_HANDLER_h
#define DEPLOII_HANDLER_h

#define DEPLOII_HOST "deploii.no"
#define DEPLOII_PORT 443
#define DEPLOII_WS_URL "/mcu/ws"
#define DEPLOII_HTTP_URL "/mcu/data"

#define DEPLOII_WIFI_RECONNECT_TIME 1000
#define DEPLOII_WS_RECONNECT_TIME 2000

#if defined(ESP32)
#include "./esp32/deploii_handler_esp32.h"
#else
#warning "The device you are using is currently not supported by Deploii."
#include "./default/deploii_handler_default.h"
#endif

#endif  // DEPLOII_HANDLER_h
