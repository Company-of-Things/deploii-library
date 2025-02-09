#ifndef DEPLOII_h
#define DEPLOII_h

#include "Arduino.h"
#include <utility>
#include <MsgPack.h>

#define DEPLOII_MAX_INTERVALS 10

enum Medium {
   None,
   WiFi,
   NarrowBand
};

enum Protocol {
   None,
   WebSockets,
   HTTP,
   MQTT
};
#ifndef Deploii_medium
#define Deploii_medium None
#endif  // !Deploii_medium
#ifndef Deploii_protocol
#define Deploii_protocol None
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
   constexpr Deploii(const char* boardID);
   ~Deploii();

   template <typename T, size_t length>
   void send(MsgPack::str_t dataStreamID, const T (&data)[length]) const;

   template <typename T>
   void send(MsgPack::str_t dataStreamID, T data) const;

   template <typename... Args>
   void connect(Args&&... args) const {
      _handler->connect(_boardID, std::forward<Args>(args)...);
   }

   void loop() const;
   void interval(int intervalLength, void (*cb)(void)) const;

 private:
   const char* _boardID;
   const DeploiiHandler* _handler;

   void checkIntervals();
   mutable struct Interval intervals[DEPLOII_MAX_INTERVALS];
   mutable int _intervalCount;
}

#include "deploii.tpp"

#endif
