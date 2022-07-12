#include "UTFR_IO_RELAYS_ACM.h"

IO_relayState_E IO_getRelayState(IO_relay_E relay)
{
  IO_relayState_E ret;
  bool intendedState; // 0 represents not conducting, 1 represents relay conducting
  bool measuredState; // 0 represents not conducting, 1 represents relay conducting
  switch(relay)
  {
    case IO_RELAY_AIRP:
      intendedState = HW_pins[HW_PIN_EN_AIRP].physicalState;
      measuredState = (HW_readMux(HW_MUX_AIRP_CONDUCTING) < 0.5); // Signal is active low
      break;      
    case IO_RELAY_AIRN:
      intendedState = HW_pins[HW_PIN_EN_AIRN].physicalState;
      measuredState = (HW_readMux(HW_MUX_AIRN_CONDUCTING) < 0.5); // Signal is active low
      break;
    case IO_RELAY_PRECHARGE:
      intendedState = HW_pins[HW_PIN_EN_PRECHARGE].physicalState;
      measuredState = (HW_readMux(HW_MUX_PRECHARGE_CONDUCTING) < 0.5); // Signal is active low
      break;
    case IO_RELAY_COUNT:
    default: // Set condition to trigger error
      intendedState = false;
      measuredState = true;
      break;
  }

  if ((intendedState == false) && (measuredState == false))
  {
    ret = IO_RELAY_SAFE;
  }
  else if ((intendedState == true) && (measuredState == true))
  {
    ret = IO_RELAY_ENERGIZED;
  }
  else
  {
    ret = IO_RELAY_FAULT;
  }

  return ret;
}

void IO_setRelayState(IO_relay_E relay, IO_relayState_E state)
{
  if ((relay < IO_RELAY_COUNT) && (state < IO_RELAY_FAULT))
  {
    if (relay == IO_RELAY_AIRP)
    {
      HW_digitalWrite(HW_PIN_EN_AIRP, state);
    }
    else if (relay == IO_RELAY_AIRN)
    {
      HW_digitalWrite(HW_PIN_EN_AIRN, state);
    }
    else if (relay == IO_RELAY_PRECHARGE)
    {
      HW_digitalWrite(HW_PIN_EN_PRECHARGE, state);
    }

    if (state == IO_RELAY_ENERGIZED)
    {
      delay(100); // To prevent large inrush currents
    }
  }
}

void printRelayState(IO_relay_E relay, IO_relayState_E state)
{
  String relayName;
  String relayState;

  if (relay == IO_RELAY_AIRP)
  {
    relayName = "AIRP";
  }
  else if (relay == IO_RELAY_AIRN)
  {
    relayName = "AIRN";
  }
  else
  {
    relayName = "PREC";
  }

  if (state == IO_RELAY_SAFE)
  {
    relayState = "SAFE";
  }
  else if (state == IO_RELAY_ENERGIZED)
  {
    relayState = "ENERGIZED";
  }
  else
  {
    relayState = "FAULT";
  }

  Serial.print(relayName);
  Serial.print(" STATE: ");
  Serial.println(relayState);
}