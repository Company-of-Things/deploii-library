#ifndef DEPLOII_h
#define DEPLOII_h

#include "Arduino.h"
#include <utility>
#include <MsgPack.h>

#define DEPLOII_MAX_INTERVALS 10

// Just so we can compare macro values
#define DEPLOII_WIFI 0
#define DEPLOII_NARROWBAND 1

#define DEPLOII_WEBSOCKETS 0
#define DEPLOII_HTTP 1
#define DEPLOII_MQTT 2

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
#include "deploii_certs.h"
#include "./handler/deploii_handler.h"

struct Interval {
   unsigned long intervalLength;
   unsigned long previousTime;
   void (*cb)(void);
};

class Deploii {
 public:
   Deploii(char* boardID) : _boardID(boardID), _handler(new DeploiiHandler), _intervalCount(0) {
                            };
   ~Deploii() {
      free(_handler);
   };

   template <typename T, size_t length>
   void send(MsgPack::str_t dataStreamID, const T (&data)[length]);

   template <typename T>
   void send(MsgPack::str_t dataStreamID, T data);

   template <typename... Args>
   void connect(Args&&... args);

   void loop() {
      _handler->loop();
      checkIntervals();
   };

   void interval(int intervalLength, void (*cb)(void)) {
      if (_intervalCount == DEPLOII_MAX_INTERVALS) return;

      _intervals[_intervalCount].intervalLength = intervalLength;
      _intervals[_intervalCount].cb = cb;
      _intervals[_intervalCount].previousTime = millis();
      _intervalCount++;
   };

 private:
   char* _boardID;
   DeploiiHandler* _handler;

   void checkIntervals() {
      for (int i = 0; i < _intervalCount; i++) {
         unsigned long currentTime = millis();
         if (currentTime >= _intervals[i].previousTime + _intervals[i].intervalLength) {
            _intervals[i].cb();
            _intervals[i].previousTime = currentTime;
         }
      }
   };
   struct Interval _intervals[DEPLOII_MAX_INTERVALS];
   int _intervalCount;
};

#include "deploii.tpp"

#endif