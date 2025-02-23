#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

#if defined(ESP32)

#include "Arduino.h"
#include "../deploii_constants.h"

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS

#include <WebSocketsClient.h>

#elif DEPLOII_PROTOCOL == DEPLOII_HTTP

#include <HTTPClient.h>

#endif  // DEPLOII_PROTOCOL

class DeploiiHandler {
 public:
   DeploiiHandler();

   ~DeploiiHandler();

   void loop();
   void send(const uint8_t* data, size_t size);

#if DEPLOII_MEDIUM == DEPLOII_WIFI
   void connect(
       char* boardID,
       const char* ssid,
       const char* pwd);

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS

 private:
   WebSocketsClient _ws;
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
 private:
   HTTP_client _http;
#endif  // DEPLOII_PROTOCOL

#else

#endif  // DEPLOII_MEDIUM
};

#endif // ESP32 
#endif // DEPLOII_HANDLER_IMPLEMENTATION_h
