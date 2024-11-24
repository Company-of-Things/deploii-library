#ifndef DEPLOII_handler_h
#define DEPLOII_handler_h

#include "deploii_certs.h"

#include "Arduino.h"
#include <WiFi.h>
#if defined(ESP32)
#include <WebSocketsClient.h>
#include <HTTPClient.h>
#endif

/*
    Constants for connection
*/

#define DEPLOII_HOST "deploii.no"
#define DEPLOII_PORT 443
#define DEPLOII_WS_URL "/mcu/ws"
#define DEPLOII_HTTP_URL "/mcu/data"

#define DEPLOII_WIFI_RECONNECT_TIME 1000
#define DEPLOII_WS_RECONNECT_TIME 2000

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
                        const char* host = DEPLOII_HOST,
                        const int port = DEPLOII_PORT,
                        const char* url = DEPLOII_WS_URL,
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

class DeploiiHandlerWiFiHTTP : public DeploiiHandler {
 public:
   DeploiiHandlerWiFiHTTP(bool debug = false);
   ~DeploiiHandlerWiFiHTTP();

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

#if defined(ESP32)
   HTTPClient _http;
#elif defined(ARDUINO)

#else

#endif
};
#endif
