#ifndef DEPLOII_h
#define DEPLOII_h

#include "Arduino.h"
#include <utility>
#include <MsgPack.h>

#define DEPLOII_MAX_INTERVALS 10

enum Medium {
   Deploii_WiFi,
   Deploii_NarrowBand
};

enum Protocol {
   Deploii_WebSockets,
   Deploii_HTTP,
   Deploii_MQTT
};

#ifndef Deploii_medium
#define Deploii_medium Deploii_WiFi
#endif  // !Deploii_medium
#ifndef Deploii_protocol
#define Deploii_protocol Deploii_WebSockets
#endif  // !Deploii_protocol
#ifndef Deploii_debug
#define Deploii_debug 0
#endif  // !Deploii_debug
#define Deploii_debug_interface Serial
#include "./handler/deploii_handler.h"

struct Interval {
   int intervalLength;
   int previousTime;
   void (*cb)(void);
};

class Deploii {
 public:
   Deploii(char* boardID);
   ~Deploii();

   template <typename T, size_t length>
   void send(MsgPack::str_t dataStreamID, const T (&data)[length]);

   template <typename T>
   void send(MsgPack::str_t dataStreamID, T data);

   template <typename... Args>
   void connect(Args&&... args);

   void loop();
   void interval(int intervalLength, void (*cb)(void));

 private:
   char* _boardID;
   DeploiiHandler* _handler;

   void checkIntervals();
   struct Interval _intervals[DEPLOII_MAX_INTERVALS];
   int _intervalCount;
};

#include "deploii.tpp"

#endif
