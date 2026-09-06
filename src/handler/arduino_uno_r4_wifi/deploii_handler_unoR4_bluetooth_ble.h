/*************************************************************************************/

#ifndef DEPLOII_HANDLER_IMPLEMENTATION_h
#define DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/

/*
 * A note on Callbacks
 *
 * ArduinoBLE uses c-style function pointers for its callbacks, which allows small
 * binary sizes, but makes integrating with an idiomatic C++ OOP style clucky.
 * Thus we need to use some static member function, meaning the class must be a singleton
 */

#include "Arduino.h"
#include "deploii_debug.h"
#include "deploii_utils.h"

#include <ArduinoBLE.h>

void _defaultCallback(uint8_t *data, size_t size) {};
void (*_dataCallback)(uint8_t *data, size_t size) = &_defaultCallback;

/*************************************************************************************/

class DeploiiHandler
{
public:
  /*************************************************************************************/

  DeploiiHandler(char *boardID)
      : _boardID(boardID)
  {
    // use hash function to convert Deploii boardID to BLE Service UUID string
    snprintf(_serviceUUID, 5, "%04x", DEPLOII_fnv1a16(_boardID));
  };

  /*************************************************************************************/

  ~DeploiiHandler() {};

  /*************************************************************************************/

  void loop()
  {
    BLE.poll();
  };

  /*************************************************************************************/

  void send(const uint8_t *data, size_t size)
  {
    if (!BLE.connected())
      return;

    // data must not exceed 512 bytes, so we need to split it up

    size_t remaining_bytes = size;
    uint8_t *data_start = (uint8_t *)data;

    size_t payload_max_size = DEPLOII_BLE_BUFFER_MAX_SIZE - 1; // account for header_byte
    size_t packet_number = size / payload_max_size + (size % payload_max_size != 0);

    while (packet_number > 0xFF)
      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[BLUETOOTH] Packet size is too large");

    // the header byte is the first byte of each transferred packet
    // it contains the number of remaining packets to expect
    // if the server does not receive the expected number of packets, the transfer is discarded
    uint8_t header_byte = packet_number - 1;

    uint8_t *packet_buffer = (uint8_t *)malloc(DEPLOII_BLE_BUFFER_MAX_SIZE);

    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[BLUETOOTH] Total packet size is %dB", size);

    while (true)
    {
      size_t transfer_size = remaining_bytes >= payload_max_size ? payload_max_size : remaining_bytes;

      packet_buffer[0] = header_byte;
      memcpy(packet_buffer + 1, data_start, transfer_size);

      DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[BLUETOOTH] Transmitting payload of size %dB", transfer_size);
      _sendCharacteristic->writeValue(packet_buffer, transfer_size + 1);

      remaining_bytes -= transfer_size;
      data_start += transfer_size;

      if (remaining_bytes == 0)
        break;

      header_byte--;
    }

    free(packet_buffer);
  };

  void setDataCallback(void (*cb)(uint8_t *data, size_t size))
  {
    _dataCallback = cb;
  }

  /*************************************************************************************/

  void connect()
  {
    BLEService deploiiService(_serviceUUID);

    _receiveCharacteristic = new BLECharacteristic{DEPLOII_BLE_RECEIVE_CHARACTERISTIC_UUID, BLEWrite | BLENotify, DEPLOII_BLE_BUFFER_MAX_SIZE};
    _sendCharacteristic = new BLECharacteristic{DEPLOII_BLE_SEND_CHARACTERISTIC_UUID, BLERead | BLENotify, DEPLOII_BLE_BUFFER_MAX_SIZE};

    if (!BLE.begin())
    {
      DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[BLUETOOTH] Could not initialize BLE, try again");
      while (true)
        ;
    }

    // displayed in device list in browser
    BLE.setLocalName("Arduino UNO R4");
    BLE.setDeviceName("Arduino UNO R4");

    BLE.setAdvertisedService(deploiiService);

    deploiiService.addCharacteristic(*_sendCharacteristic);
    deploiiService.addCharacteristic(*_receiveCharacteristic);

    BLE.addService(deploiiService);

    BLE.setEventHandler(BLEConnected, _bleConnectCallback);
    BLE.setEventHandler(BLEDisconnected, _bleDisconnectCallback);

    _receiveCharacteristic->setEventHandler(BLEWritten, _receiveCharacteristicWrittenCallback);

    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[BLUEOOTH] Starting BLE advertising with UUID 0x%s", _serviceUUID);
    BLE.advertise();

    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[BLUEOOTH] Awaiting BLE connection...");
    while (!BLE.connected())
      ;
    DEPLOII_DPRINT(DEPLOII_DEBUG_INFO, "[BLUEOOTH] BLE connection established");
  };

  /*************************************************************************************/

private:
  char *_boardID;
  char _serviceUUID[5];

  BLECharacteristic *_receiveCharacteristic;
  BLECharacteristic *_sendCharacteristic;

  static void _bleConnectCallback(BLEDevice central)
  {
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[BLUEOOTH] Central with address %s connected", central.address().c_str());
  }

  static void _bleDisconnectCallback(BLEDevice central)
  {
    DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "[BLUEOOTH] Central with address %s disconnected", central.address().c_str());
  }

  static void _receiveCharacteristicWrittenCallback(BLEDevice central, BLECharacteristic characteristic)
  {
    // unused parameters
    (void)central;
    _dataCallback((uint8_t *)characteristic.value(), characteristic.valueLength());
  }
};

/*************************************************************************************/

#endif // DEPLOII_HANDLER_IMPLEMENTATION_h

/*************************************************************************************/
