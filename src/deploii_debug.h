/*************************************************************************************/

#ifndef DEPLOII_DEBUG_h
#define DEPLOII_DEBUG_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_config.h"

/*************************************************************************************/

__attribute__((constructor))
static void deploii_debug_init(void)
{
  if (DEPLOII_DEBUG != DEPLOII_DEBUG_NONE)
  {
    // TODO: allow user to specify output stream for debug prints
    Serial.begin(DEPLOII_SERIAL_BAUDRATE);
  }
}

/*************************************************************************************/

template <typename... Args>
void DEPLOII_DPRINT(deploii_debug_t log_level, Args &&...args)
{
  if (log_level <= DEPLOII_DEBUG)
  {
    Serial.print("DEPLOII DEBUG: ");
    Serial.println(std::forward<Args>(args)...);
  }
}

/*************************************************************************************/

#endif // !DEPLOII_DEBUG_h

/*************************************************************************************/
