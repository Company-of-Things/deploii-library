#ifndef DEPLOII_HANDLER_h
#define DEPLOII_HANDLER_h

#if defined(ESP32)
#include "./esp32/deploii_handler_esp32.h"
#else
#warning "The device you are using is currently not supported by Deploii."
#define DEPLOII_DEFAULT
#include "./default/deploii_handler_default.h"
#endif

#endif  // DEPLOII_HANDLER_h
