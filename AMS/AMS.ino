/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "HW_AMS.h"
#include "MCP_DAC.h"

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

void HW_setupDAC(void)
{
    DAC.begin(HW_PIN_CS_DAC);
}

bool HW_writeToDAC(uint16_t value, uint8_t channel)
{
    return DAC.analogWrite(value, channel);
}

/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/

void setup() {
    Serial.begin(9600);
    Serial.println("[AMS]: booting up...");

    HW_setupPins();
    HW_setupDAC();
}


void loop() {
  

}