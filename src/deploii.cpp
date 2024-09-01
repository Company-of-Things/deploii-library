#include "deploii.h"

Deploii::Deploii(char* boardID, Medium medium, Protocol protocol, bool debug) : _boardID(boardID),
                                                                                _medium(medium),
                                                                                _protocol(protocol),
                                                                                _debug(debug),
                                                                                _intervals((struct Interval*)malloc(0)),
                                                                                _intervalCount(0),
                                                                                _handler(selectHandler()) {
}

Deploii::~Deploii() {
   delete _handler;
   free(_intervals);
}

void Deploii::connect() {
   _handler->connect();
}

void Deploii::connect(char* ssid,
                      const char* pwd,
                      const char* host,
                      const int port,
                      const char* url,
                      bool ssl) {
   _handler->connect(_boardID, ssid, pwd, host, port, url, ssl);
}

void Deploii::loop() {
   _handler->loop();
   checkIntervals();
}

DeploiiHandler* Deploii::selectHandler() {
   if (_medium == Medium::WiFi && _protocol == Protocol::WebSockets) return new DeploiiHandlerWiFiWS(_debug);

   return new DeploiiHandler(_debug);
}

void Deploii::interval(int intervalLength, void (*cb)(void)) {
   _intervals = (struct Interval*)realloc(_intervals, sizeof(struct Interval) * (_intervalCount + 1));
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