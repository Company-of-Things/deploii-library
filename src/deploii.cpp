#include "deploii.h"

Deploii::Deploii(char* boardID, Medium medium, Protocol protocol, bool debug) : _boardID(boardID),
                                                                                _medium(medium),
                                                                                _protocol(protocol),
                                                                                _debug(debug),
                                                                                _handler(selectHandler()) {
}

Deploii::~Deploii() {
   delete _handler;
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
}

DeploiiHandler* Deploii::selectHandler() {
   if (_medium == Medium::WiFi && _protocol == Protocol::WebSockets) return new DeploiiHandlerWiFiWS(_debug);

   return new DeploiiHandler(_debug);
}