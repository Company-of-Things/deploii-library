#include "deploii.h"

Deploii::Deploii(char* boardID) : _boardID(boardID), _handler(), _intervalCount(0) {
}

Deploii::~Deploii() {
   free(_handler);
}

void Deploii::loop() {
   _handler->loop();
   checkIntervals();
}

void Deploii::interval(int intervalLength, void (*cb)(void)) {
   if (_intervalCount == DEPLOII_MAX_INTERVALS) return;

   _intervals[_intervalCount].intervalLength = intervalLength;
   _intervals[_intervalCount].cb = cb;
   _intervals[_intervalCount].previousTime = millis();
   _intervalCount++;
}

void Deploii::checkIntervals() {
   for (int i = 0; i < _intervalCount; i++) {
      unsigned long currentTime = millis();
      if (currentTime >= _intervals[i].previousTime + _intervals[i].intervalLength) {
         _intervals[i].cb();
         _intervals[i].previousTime = currentTime;
      }
   }
}
