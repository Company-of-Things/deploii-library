/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h
#if defined(DEPLOII_DEFAULT)

/*************************************************************************************/

#include "Arduino.h"

/*************************************************************************************/

/*
 * Empty microcontroller handler for Deploii, can also be used as a template
 */
class DeploiiHandler
{
public:
  DeploiiHandler() {};
  ~DeploiiHandler() {};

  void send(const uint8_t *data, size_t size) {};

  template <typename... Args>
  void connect(Args &&...args) {};
  void setDataCallback(void (*cb)(uint8_t* data, size_t size)){};

  void loop() {};
};

/*************************************************************************************/

#endif // !DEPLOII_DEFAULT
#endif // !DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
