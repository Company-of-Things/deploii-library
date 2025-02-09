#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

#include "Arduino.h"
#include "deploii_constants.h"

class DeploiiHandler {
 public:
   DeploiiHandler();
   ~DeploiiHandler();

   void send(const uint8_t* data, size_t size) {}

   template <typename... Args>
   void connect(Args&&... args) {}

   void loop() {}
};

#endif