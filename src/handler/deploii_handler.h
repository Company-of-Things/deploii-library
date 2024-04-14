#ifndef Deploii_handler_h
#define Deploii_handler_h

/*
    Constants for connection
*/

#define Deploii_HOST "deploii.no"
#define Deploii_PORT 443
#define Deploii_WS_URL "/ws"

#include <WiFi.h>
#include <WebSocketsClient.h>

class DeploiiHandler {
 public:
   DeploiiHandler();
   ~DeploiiHandler();

   virtual void send();
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
};

class DeploiiHandlerWiFiWS : public DeploiiHandler {
 public:
   DeploiiHandlerWiFiWS();

   virtual void send();
   virtual void loop();
   virtual void connect(char* boardID,
                        char* ssid,
                        const char* pwd,
                        const char* host,
                        const int port,
                        const char* url,
                        bool ssl);

 private:
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