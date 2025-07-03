/*************************************************************************************/
/*                                                                                   */
/* Arduino library for Deploii, an educational IoT platform, written 2025.           */
/* For documentation see https://github.com/Company-of-Things/deploii-library        */
/*                                                                                   */
/*************************************************************************************/

#ifndef DEPLOII_h
#define DEPLOII_h

/*************************************************************************************/

#include "Arduino.h"
#include <MsgPack.h>
#include "deploii_certs.h"
#include "./handler/deploii_handler.h"

/*************************************************************************************/

/*
 * Defines an action to be run at a set interval
 */
struct Interval
{
  unsigned long intervalLength;
  unsigned long previousTime;
  void (*cb)(void);
};

/*************************************************************************************/

class Deploii
{
public:
  /*
   * Constructor and destructor, initializes the microcontroller handler
   */
  Deploii(char *boardID)
      : _boardID(boardID),
        _handler(new DeploiiHandler),
        _intervalCount(0) {
        };
  ~Deploii()
  {
    free(_handler);
  };

  /*
   * Send data of type array to datastream
   */
  template <typename T, size_t length>
  void send(MsgPack::str_t dataStreamID, const T (&data)[length]);

  /*
   * Send data of single value type to datastream
   */
  template <typename T>
  void send(MsgPack::str_t dataStreamID, T data);

  /*
   * Connect to Deploii server over the selected medium
   * Different medium need different arguments, see handlers
   */
  template <typename... Args>
  void connect(Args &&...args);

  /*
   * Keeps connection to Deploii server alive
   * Must be called regularely
   */
  void loop()
  {
    _handler->loop();
    checkIntervals();
  };

  /*
   * Register a new interval defined in milliseconds
   */
  void interval(int intervalLength, void (*cb)(void))
  {
    if (_intervalCount == DEPLOII_MAX_INTERVALS)
      return;

    _intervals[_intervalCount].intervalLength = intervalLength;
    _intervals[_intervalCount].cb = cb;
    _intervals[_intervalCount].previousTime = millis();
    _intervalCount++;
  };

private:
  char *_boardID;
  DeploiiHandler *_handler;

  /*
   * Crude concurrency, keeps track of and calls interval callbacks
   */
  void checkIntervals()
  {
    for (int i = 0; i < _intervalCount; i++)
    {
      unsigned long currentTime = millis();
      if (currentTime >= _intervals[i].previousTime + _intervals[i].intervalLength)
      {
        _intervals[i].cb();
        _intervals[i].previousTime = currentTime;
      }
    }
  };

  /*
   * Interval metadata
   */
  struct Interval _intervals[DEPLOII_MAX_INTERVALS];
  int _intervalCount;
};

/*************************************************************************************/

/*
 * Type templates for send and connect functions
 */
#include "deploii.tpp"

/*************************************************************************************/

#endif // !DEPLOII_h

/*************************************************************************************/
