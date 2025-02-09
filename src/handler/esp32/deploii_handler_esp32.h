#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

#include "Arduino.h"
#include "../deploii_constants.h"

#if Deploii_protocol == Deploii_WebSockets

#include <WebSocketsClient.h>

#elif Deploii_protocol == Deploii_HTTP

#include <HTTPClient.h>

#endif  // Deploii_protocol

class DeploiiHandler {
 public:
   DeploiiHandler();

   ~DeploiiHandler();

   void loop();
   void send(const uint8_t* data, size_t size);

#if Deploii_medium == Deploii_WiFi
   void connect(
       char* boardID,
       const char* ssid,
       const char* pwd);

#if Deploii_protocol == Deploii_WebSockets

 private:
   WebSocketsClient _ws;
#elif Deploii_protocol == Deploii_HTTP
 private:
   HTTP_client _http;
#endif  // Deploii_protocol

#else

#endif  // Deploii_medium
};

#endif
