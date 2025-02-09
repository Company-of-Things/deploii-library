#ifndef DEPLOII_HANDLER_ESP32_h
#define DEPLOII_HANDLER_ESP32_h

#include "../deploii_handler.h"

#if Deploii_protocol == WebSockets

#include <WebSocketsClient.h>

#elif Deploii_protocol == HTTP

#include <HTTPClient.h>

#endif  // Deploii_protocol

class DeploiiHandler {
 public:
   DeploiiHandler();
   ̈~DeploiiHandler();

   void loop();
   void send(const uint8_t* data, size_t size);

#if Deploii_medium == WiFi
   void connect(
       char* boardID,
       char* ssid,
       const char* pwd);

#if Deploii_protocol == WebSockets

 private:
   WebSocketsClient _ws;
#elif Deploii_protocol == HTTP
 private:
   HTTP_client _http;
#endif  // Deploii_protocol

#else

#endif  // Deploii_medium
};

#endif
