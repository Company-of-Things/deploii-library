#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

#if defined(ESP32)

#pragma message("esp32" DEPLOII_PROTOCOL)

#include "Arduino.h"
#include "../deploii_constants.h"

#if DEPLOII_MEDIUM == DEPLOII_WIFI
#include <WiFi.h>
#endif

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS

#include <WebSocketsClient.h>

#elif DEPLOII_PROTOCOL == DEPLOII_HTTP

#include <HTTPClient.h>

#endif  // DEPLOII_PROTOCOL

class DeploiiHandler {
 public:
   DeploiiHandler()
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
       : _ws()
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
       : _http()
#endif  // DEPLOII_PROTOCOL
   {
   };

   ~DeploiiHandler() {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
      _ws.~WebSocketsClient();
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
      _http.~HTTPClient();
#endif  // DEPLOII_PROTOCOL
   };

   void loop() {
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
      _ws.loop();
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP

#endif  // DEPLOII_PROTOCOL
   };
   void send(const uint8_t* data, size_t size) {
      Serial.println("Sending...");
#pragma message(strx(DEPLOII_PROTOCOL))
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
      Serial.println("WS");
      Serial.println(_ws.isConnected());
      _ws.sendBIN(data, size);
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
      Serial.println("HTTP");
      Serial.println(_http.POST((uint8_t*)data, size));
#endif  // DEPLOII_PROTOCOL
   };

#if DEPLOII_MEDIUM == DEPLOII_WIFI
   void connect(
       char* boardID,
       const char* ssid,
       const char* pwd) {
      Serial.println("Handler_connect");
      Serial.print("Board ID: ");
      Serial.println(boardID);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, pwd);
      while (WiFi.status() != WL_CONNECTED) {
         delay(DEPLOII_WIFI_RECONNECT_TIME);
      }
      Serial.println("Wifi connected");
#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS
      static char authHeader[60];
      sprintf(authHeader, "%s%s", "Authorization: ", boardID);
      Serial.print("_ws address: ");
      Serial.println(reinterpret_cast<uintptr_t>(&_ws));
      Serial.println("Setting auth header...");
      _ws.setExtraHeaders(authHeader);
      Serial.println("Beginning ws...");
      Serial.println(DEPLOII_HOST DEPLOII_WS_URL);
      Serial.println(DEPLOII_PORT);
      _ws.beginSSL(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_WS_URL);
      Serial.println(_ws.isConnected());

#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
      Serial.println("Test123");
      Serial.println(_http.begin(DEPLOII_HOST, DEPLOII_PORT, DEPLOII_HTTP_URL, buypass_cert));
      _http.addHeader("Authorization", boardID, false, false);
#endif  // DEPLOII_PROTOCOL
   };

#if DEPLOII_PROTOCOL == DEPLOII_WEBSOCKETS

 private:
   WebSocketsClient _ws;
#elif DEPLOII_PROTOCOL == DEPLOII_HTTP
 private:
   HTTPClient _http;
#endif  // DEPLOII_PROTOCOL

#else

#endif  // DEPLOII_MEDIUM
};

#endif  // ESP32
#endif  // DEPLOII_HANDLER_IMPLEMENTATION_h
