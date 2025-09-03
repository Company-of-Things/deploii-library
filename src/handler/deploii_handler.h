/*************************************************************************************/

#ifndef DEPLOII_HANDLER_h
#define DEPLOII_HANDLER_h

/*************************************************************************************/

/*
 * Include correct microcontroller handler
 */
#if defined(ESP32)
#include "handler/esp32/deploii_handler_esp32.h"

#elif defined(ARDUINO_UNOR4_WIFI)
#include "handler/arduino_uno_r4_wifi/deploii_handler_arduino_uno_r4_wifi.h"

#elif defined(ARDUINO_SAMD_MKRWIFI1010)
#include "handler/arduino_samd_mkr_wifi_1010/deploii_handler_arduino_samd_mkr_wifi_1010.h"


#else
#warning "The device you are using is currently not supported by Deploii."
#define DEPLOII_DEFAULT
#include "handler/default/deploii_handler_default.h"

#endif

/*************************************************************************************/

#endif // !DEPLOII_HANDLER_h

/*************************************************************************************/
