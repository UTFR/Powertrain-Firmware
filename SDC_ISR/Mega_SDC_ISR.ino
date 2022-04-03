/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include <SoftwareSerial.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/

#define HW_PIN_MSCOM1 40
#define HW_PIN_MSCOM2 41
#define HW_PIN_MSCOM3 20

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/

/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/

/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

const int kRETRY_INTERVAL = 20;
unsigned long current_time, start_time = 0;

bool SDC_TRIPPED = false;
int checkState = 0; // Checks the state for the  20 pin to ensure that zero torque has been sent to the inverter

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/
void SDC_ISR()
{
  digitalWrite(HW_PIN_MSCOM1, HIGH);
  SDC_TRIPPED = true;
  return SDC_TRIPPED;
}

void shutdown_carOff()
{
  digitalWrite(38, LOW); // Tell inverter to shutdown
  digitalWrite(7, LOW);  // LV Battery Mosfet -> turn off most LV systems
  while (1)
  {
// ERROR.sendErr(carOff);                        // Kelvin's error library
#ifdef debug_RC_Mega
    Serial.print("Car off. \n";)
#endif
        delay(1000);
  }
}

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

void setup()
{

  Serial.begin(115200);
  Serial3.begin(9600);

  pinMode(HW_PIN_MSCOM1, OUTPUT);

  pinMode(HW_PIN_MSCOM2, INPUT);
  pinMode(HW_PIN_MSCOM3, INPUT);
  attachInterrupt(digitalPinToInterrupt(19), SDC_ISR, RISING);
}

void loop()
{

  if (SDC_TRIPPED == true)
  {

    start_time = millis()

        if (digitalRead(HW_PIN_MSCOM3) == HIGH)
    { // Checks to make sure that zero torque has been sent to the inverter

      shutdown_carOff(); // If not send signal back to micro to shutdown
    }

    current_time = start_time;

    if ((current_time - start_time) < kRETRY_INTERVAL)
    {
      current_time = millis();
      digitalWrite(HW_PIN_MSCOM1, LOW);
      digitalWrite(HW_PIN_MSCOM1, HIGH);
    }

    start_time = millis();
    digitalWrite(HW_PIN_MSCOM1, LOW);
  }
}
