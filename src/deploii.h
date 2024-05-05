#ifndef Deploii_h
#define Deploii_h

#include "Arduino.h"
#include "./handler/deploii_handler.h"

#include <MsgPack.h>

/*
    Constants for connection
*/

#define Deploii_HOST "deploii.no"
#define Deploii_PORT 443
#define Deploii_WS_URL "/ws"

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

   template <typename T, size_t length>
   void send(MsgPack::str_t dataStreamID, const T (&data)[length]);

   template <typename T>
   void send(MsgPack::str_t dataStreamID, T data);

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

#include "deploii.tpp"

#endif