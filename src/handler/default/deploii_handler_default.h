/*************************************************************************************/

#if defined(DEPLOII_DEFAULT)
#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

#include "deploii_config.h"
#include "Arduino.h"

/*************************************************************************************/

/*
 * Empty microcontroller handler for Deploii, can also be used as a template
 */
class DeploiiHandler
{
public:
  DeploiiHandler(char *boardID) {};
  ~DeploiiHandler() {};

  void send(const uint8_t *data, size_t size) {};

  template <typename... Args>
  void connect(Args &&...args) {};

  void loop() {};
};

/*************************************************************************************/

#endif // !DEPLOII_HANDLER_IMPLEMENTATION_h
#endif // !DEPLOII_DEFAULT

/*************************************************************************************/
