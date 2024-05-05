#ifndef Deploii_h
#define Deploii_h

#include "Arduino.h"
#include "./handler/deploii_handler.h"

enum class Medium {
   None,
   WiFi,
   NarrowBand
};

enum class Protocol {
   None,
   WebSockets,
   HTTP,
   MQTT
};

class Deploii {
 public:
   Deploii(char* boardID, Medium medium, Protocol protocol, bool debug = false);
   ~Deploii();

   void send();
   void loop();
   void connect();
   void connect(char* ssid,
                const char* pwd,
                const char* host = Deploii_HOST,
                const int port = Deploii_PORT,
                const char* url = Deploii_WS_URL,
                bool ssl = true);

 private:
   Medium _medium;
   Protocol _protocol;
   bool _debug;
   char* _boardID;
   DeploiiHandler* _handler;
   DeploiiHandler* selectHandler();
};

#endif