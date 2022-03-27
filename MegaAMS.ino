/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "Mega_HW_AMS.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define ANALOG_CONSTANT 1023 // Arduino ADC resolution

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


/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

// directly from Jake Sprenger's ACM.ino
void Mega_HW_setupPins(void)
{
  for (int pin = 0; pin < HW_PIN_COUNT; ++pin)
  {
    if (Mega_HW_pins[pin].isOutput)
    {
      pinMode(Mega_HW_pins[pin].pinNum, OUTPUT);
    }
    else
    {
      pinMode(Mega_HW_pins[pin].pinNum, INPUT);
    }
  }
}

void Mega_HW_digitalWrite(HW_pin_E pin, bool physState)
{
  if (pin < HW_PIN_COUNT)
  {
    Mega_digitalWrite(Mega_HW_pins[pin].pinNum, physState);
    Mega_HW_pins[pin].physicalState = physState;
  }
}

bool Mega_HW_digitalRead(HW_pin_E pin)
{
  bool state = false;
  if (pin < HW_PIN_COUNT)
  {
    state = Mega_digitalRead(Mega_HW_pins[pin].pinNum);
    Mega_HW_pins[pin].physicalState = state;
  }
  
  return state;
}

/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/

void setup() {
    Serial.begin(9600);
    Serial.println("[AMS]: booting up...");

    Mega_HW_setupPins();
}

void loop() {
  
}