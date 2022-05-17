// IMPORTANT NOTE - USE THE ATMEGA328P (OLD PROGRAMMER)
/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include "HW_ACM.h"

#include "IO_thermistors.h"
#include "IO_relays.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/

#define ANALOG_CONSTANT 1023 // Arduino ADC resolution
#define S0_MASK 0b00001
#define S1_MASK 0b00010
#define S2_MASK 0b00100
#define S3_MASK 0b01000
#define MEASURED_ANALOG_ERROR 0.1

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/


/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/


/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/

static void app_failedPrecharge(void);
static bool app_doPrecharge(void);

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

void HW_setupPins(void)
{
  for (int pin = 0; pin < HW_PIN_COUNT; ++pin)
  {
    if (HW_pins[pin].isOutput)
    {
      pinMode(HW_pins[pin].pinNum, OUTPUT);
    }
    else
    {
      pinMode(HW_pins[pin].pinNum, INPUT);
    }
  }
}

void HW_digitalWrite(HW_pin_E pin, bool physState)
{
  if (pin < HW_PIN_COUNT)
  {
    digitalWrite(HW_pins[pin].pinNum, physState);
    HW_pins[pin].physicalState = physState;
  }
}

bool HW_digitalRead(HW_pin_E pin)
{
  bool state = false;
  if (pin < HW_PIN_COUNT)
  {
    state = digitalRead(HW_pins[pin].pinNum);
    HW_pins[pin].physicalState = state;
  }
  
  return state;
}

float HW_readMux(HW_MUX_E muxPin) // Returns the voltage read for a given mux pin, from 0-5V
{
  bool S0_state = (muxPin & S0_MASK);
  bool S1_state = (muxPin & S1_MASK) >> 1;
  bool S2_state = (muxPin & S2_MASK) >> 2;
  bool S3_state = (muxPin & S3_MASK) >> 3;

  // Set the mux select pins
  HW_digitalWrite(HW_PIN_MUX_S0, S0_state);
  HW_digitalWrite(HW_PIN_MUX_S1, S1_state);
  HW_digitalWrite(HW_PIN_MUX_S2, S2_state);
  HW_digitalWrite(HW_PIN_MUX_S3, S3_state);

  float readVal = analogRead(HW_pins[HW_PIN_MUX].pinNum);
  float analogVoltage = (readVal / ANALOG_CONSTANT) * 5.0;
  analogVoltage += MEASURED_ANALOG_ERROR;
  return analogVoltage;
}

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

void app_benchSelfTest(void) // only to be used on the bench with NO high voltage present. The self-test logic will be different in the car.
{
  Serial.println("======== BEGINNING SELF TEST ========");
  // Enable SDC
  HW_digitalWrite(HW_PIN_SDC_EN, true);
  Serial.println("Shutdown circuit enabled");

  // Check for relay faults
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);

  // Read and display relay states
  IO_relayState_E airpState = IO_getRelayState(IO_RELAY_AIRP);
  IO_relayState_E airnState = IO_getRelayState(IO_RELAY_AIRN);
  IO_relayState_E prechargeState = IO_getRelayState(IO_RELAY_PRECHARGE);

  if (airpState == IO_RELAY_SAFE)
  {
    IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_ENERGIZED);
  }

  if (airnState == IO_RELAY_SAFE)
  {
    IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_ENERGIZED);
  }

  if (prechargeState == IO_RELAY_SAFE)
  {
    IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_ENERGIZED);
  }

  // Get new states
  airpState = IO_getRelayState(IO_RELAY_AIRP);
  airnState = IO_getRelayState(IO_RELAY_AIRN);
  prechargeState = IO_getRelayState(IO_RELAY_PRECHARGE);

  // Respond to error states
  if (airpState == IO_RELAY_FAULT)
  {
    IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  }
  
  if (airnState == IO_RELAY_FAULT)
  {
    IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  }

  if (prechargeState == IO_RELAY_FAULT)
  {
    IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
  }
  
  Serial.println("=========== RELAY STATES ===========");
  printRelayState(IO_RELAY_AIRP, airpState);
  printRelayState(IO_RELAY_AIRN, airnState);
  printRelayState(IO_RELAY_PRECHARGE, prechargeState);

  if ((airpState == IO_RELAY_FAULT) | (airnState == IO_RELAY_FAULT) | (prechargeState == IO_RELAY_FAULT))
  {
    HW_digitalWrite(HW_PIN_AMS_FAULT_EN, true); // If any relay faults present, illuminate LED
  }
  
}

void app_failedPrecharge(void) // This function disables the shutdown circuit and illuminates the AMS fault LED
{
  HW_digitalWrite(HW_PIN_SDC_EN, false);
  HW_digitalWrite(HW_PIN_AMS_FAULT_EN, true);
}

bool app_doPrecharge(void)
{
  
  if ((HW_readMux(HW_MUX_nTS_ENERGIZED) < 2.5) || (HW_readMux(HW_MUX_PRECHARGE_DONE) > 2.5) || (IO_getRelayState(IO_RELAY_PRECHARGE) != IO_RELAY_SAFE))
  {
    // FAILURE: off-state signals are incorrect
    app_failedPrecharge();
    return false;
  }
  
  int startTime = millis();
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_ENERGIZED);
  while ((millis() - startTime) < 3500) // Precharge takes about 3.5 seconds, tested in lab. Time can be reduced by placing another resistor in parallel.
  {
    // Do nothing. Wait for precharge to complete
  }

  // Check precharge signals
  if ((HW_readMux(HW_MUX_nTS_ENERGIZED) > 2.5))
  {
    // FAILURE: system not detected to be energized
    app_failedPrecharge();
    return false;
  }
  if ((HW_readMux(HW_MUX_PRECHARGE_DONE) < 2.5))
  {
    // FAILURE: system not detected to be precharged
    app_failedPrecharge();
    return false;
  }
  if ((HW_readMux(HW_MUX_PRECHARGE_CONDUCTING) > 2.5))
  {
    // FAILURE: precharge relay detected to be not conducting
    app_failedPrecharge();
    return false;
  }

  // All good up until this point. Precharge is complete, activate main contactor
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_ENERGIZED);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
  delay(100);
  if (!((IO_getRelayState(IO_RELAY_AIRP) == IO_RELAY_ENERGIZED) && (IO_getRelayState(IO_RELAY_PRECHARGE) == IO_RELAY_SAFE)))
  {
    // FAILURE: incorrect relay states at end of precharge
    app_failedPrecharge();
    return false;
  }

  // Success
  return true;
    
}

/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/

void setup() {
  Serial.begin(9600);
  Serial.println("ACM booting up");
  HW_setupPins();
  HW_digitalWrite(HW_PIN_SDC_EN, true);
  //app_benchSelfTest();
  
}

void loop() {



}
