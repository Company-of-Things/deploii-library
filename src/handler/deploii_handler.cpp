#include "deploii_handler.h"

DeploiiHandler::DeploiiHandler(bool debug) : _debug(debug) {}
DeploiiHandler::~DeploiiHandler() {}

void DeploiiHandler::send(const uint8_t* data, size_t size) {}
void DeploiiHandler::loop() {}
void DeploiiHandler::connect() {}
void DeploiiHandler::connect(char* boardID,
                             char* ssid,
                             const char* pwd,
                             const char* host,
                             const int port,
                             const char* url,
                             bool ssl) {}
