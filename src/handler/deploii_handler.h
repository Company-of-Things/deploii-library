#ifndef Deploii_handler_h
#define Deploii_handler_h

#include "Arduino.h"
#include <WiFi.h>
#if defined(ESP32)
#include <WebSocketsClient.h>
#endif

/*
    Constants for connection
*/

#define Deploii_HOST "deploii.no"
#define Deploii_PORT 443
#define Deploii_WS_URL "/ws"

class DeploiiHandler {
 public:
   DeploiiHandler(bool debug = false);
   ~DeploiiHandler();

   virtual void send(const uint8_t* data, size_t size);
   virtual void loop();
   virtual void connect();
   virtual void connect(char* boardID,
                        char* ssid,
                        const char* pwd,
                        const char* host = Deploii_HOST,
                        const int port = Deploii_PORT,
                        const char* url = Deploii_WS_URL,
                        bool ssl = true);

 private:
   bool _debug;
};

class DeploiiHandlerWiFiWS : public DeploiiHandler {
 public:
   DeploiiHandlerWiFiWS(bool debug = false);
   ~DeploiiHandlerWiFiWS();

   virtual void send(const uint8_t* data, size_t size);
   virtual void loop();
   virtual void connect(char* boardID,
                        char* ssid,
                        const char* pwd,
                        const char* host,
                        const int port,
                        const char* url,
                        bool ssl);

 private:
   bool _debug;

   void connectWiFi(char* ssid, const char* pwd);
   void connectWS(char* boardID, const char* host, const int port, const char* url, bool ssl);

   /*
     Board specific private members
   */
#if defined(ESP32)
   WebSocketsClient _ws;

#elif defined(ARDUINO)
#else

#endif
};

#endif