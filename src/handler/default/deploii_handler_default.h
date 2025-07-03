#if defined(DEPLOII_DEFAULT)

#include "Arduino.h"
#include "../deploii_constants.h"

class DeploiiHandler {
 public:
   DeploiiHandler(char* boardID) {};
   ~DeploiiHandler() {};

   void send(const uint8_t* data, size_t size) {};

   template <typename... Args>
   void connect(Args&&... args) {

   };

   void loop() {};
};

#endif  // DEPLOII_DEFAULT
