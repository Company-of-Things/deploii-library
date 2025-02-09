#ifndef DEPLOII_HANDLER_DEFAULT_h
#define DEPLOII_HANDLER_DEFAULT_h

#include "../deploii_handler.h"

class Deploii_handler {
 public:
   Deploii_handler();
   ~Deploii_handler();

   void send(const uint8_t* data, size_t size) {}

   template <typename... Args>
   void connect(Args&&... args) {}

   void loop() {}
};

#endif