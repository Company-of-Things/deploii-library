/*************************************************************************************/

#ifndef DEPLOII_DEBUG_h
#define DEPLOII_DEBUG_h

/*************************************************************************************/

#include "Arduino.h"
#include "deploii_config.h"

/*************************************************************************************/

//__attribute__((constructor))
static void deploii_debug_init(void)
{
  if (DEPLOII_DEBUG != DEPLOII_DEBUG_NONE)
  {
    // TODO: allow user to specify output stream for debug prints
    Serial.begin(DEPLOII_SERIAL_BAUDRATE);
  }
}

/*************************************************************************************/

/*
 * Debug print function with specified logging level (see deploii_config.h).
 * Takes args like printf, example:
 * DEPLOII_DPRINT(DEPLOII_DEBUG_VERBOSE, "Test %d\n", 42)
 */
template <typename... Args>
void DEPLOII_DPRINT(deploii_debug_t log_level, Args &&...args)
{
  if (log_level <= DEPLOII_DEBUG)
  {
    char str_buff[100];
    sprintf(str_buff, std::forward<Args>(args)...);

    if (log_level == DEPLOII_DEBUG_INFO)
      Serial.print("DEPLOII DEBUG INFO: ");
    else if (log_level == DEPLOII_DEBUG_VERBOSE)
      Serial.print("DEPLOII DEBUG VERBOSE: ");

    Serial.println(String(str_buff));
  }
}

/*************************************************************************************/

#endif // !DEPLOII_DEBUG_h

/*************************************************************************************/
