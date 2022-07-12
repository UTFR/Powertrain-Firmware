#ifndef IO_RELAYS_H
#define IO_RELAYS_H

/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include "UTFR_HW_ACM.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/

typedef enum
{
  IO_RELAY_SAFE,
  IO_RELAY_ENERGIZED,
  IO_RELAY_FAULT, // Can happen if intended state does not match measured state
} IO_relayState_E;

typedef enum
{
  IO_RELAY_AIRP,
  IO_RELAY_AIRN,
  IO_RELAY_PRECHARGE,
  IO_RELAY_COUNT
} IO_relay_E;

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

IO_relayState_E IO_getRelayState(IO_relay_E relay);
void IO_setRelayState(IO_relay_E relay, IO_relayState_E state);
void printRelayState(IO_relay_E relay, IO_relayState_E state);

#endif /* IO_RELAYS_H */
