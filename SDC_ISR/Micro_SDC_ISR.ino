/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include "MCP_DAC.h"
#define HW_PIN_MSCOM3 6

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/

/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/
bool SDC_ISR();

/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

MCP4911 DAC; // Digital-to-Analog converter -> this sends drive signals to the inverter which, in turn, powers the motor.

const int kCS_DAC = 8;
const int HW_PIN_MSCOM1 = 2;

bool SDC_TRIPPED = false; // if this is true, do nothing until 0 torque to inverter

bool written = false;
int readback = -1;
int kREADBACK_DELAY = 10;

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/

bool SDC_ISR()
{
  DAC.analogWrite(0, 0); // Write 0 throttle to DAC
  delayMicroseconds(kREADBACK_DELAY);
  SDC_TRIPPED = true;
  readback = analogRead(A5);
}

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

void setup()
{

  Serial.begin(115200);
  apps.begin(kCS_DAC);

  attachInterrupt(digitalPinToInterrupt(HW_PIN_MSCOM1), SDC_ISR, RISING);
  pinmode(HW_PIN_MSCOM3, OUTPUT);
}

void loop()
{

  if (SDC_TRIPPED)
  {
    if (readback != 0)
    {
      SDC_ISR()
    }
    else
    {
      digitalwrite(HW_PIN_MSCOM3, LOW);
    }
  }
}
