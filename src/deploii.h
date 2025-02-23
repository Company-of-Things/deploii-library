#ifndef DEPLOII_h
#define DEPLOII_h

#include "Arduino.h"
#include <utility>
#include <MsgPack.h>

#define DEPLOII_MAX_INTERVALS 10

enum Medium {
   DEPLOII_WIFI,
   DEPLOII_NARROWBAND
};

enum Protocol {
   DEPLOII_WEBSOCKETS,
   DEPLOII_HTTP,
   DEPLOII_MQTT
};

#ifndef DEPLOII_MEDIUM
#define DEPLOII_MEDIUM DEPLOII_WIFI
#endif  // !DEPLOII_MEDIUM
#ifndef DEPLOII_PROTOCOL
#define DEPLOII_PROTOCOL DEPLOII_WEBSOCKETS
#endif  // !DEPLOII_PROTOCOL
#ifndef DEPLOII_DEBUG
#define DEPLOII_DEBUG 0
#endif  // !DEPLOII_DEBUG
#define DEPLOII_DEBUG_INTERFACE Serial
#include "./handler/deploii_handler.h"

struct Interval {
  unsigned long intervalLength;
  unsigned long previousTime;
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
