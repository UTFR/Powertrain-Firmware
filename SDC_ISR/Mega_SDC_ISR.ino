/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include <string>
#include <SoftwareSerial.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/

#define HW_PIN_MSCOM1 40
#define HW_PIN_MSCOM2 41

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

string serialData = " ";
bool SDC_TRIPPED = false;


/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/
void SDC_ISR(){
  digitalWrite(HW_PIN_MSCOM1, HIGH);
  SDC_TRIPPED = true;
  return SDC_TRIPPED;
}

void shutdown(){
  while(1){}
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
  attachInterrupt(digitalPinToInterrupt(19), SDC_ISR, RISING);

}

void loop()
{

if (SDC_TRIPPED == true) {

  start_time = millis()

      current_time = start_time;

  while ((current_time - start_time) < kRETRY_INTERVAL)
  {
    current_time = millis()

    if (Serial.available())
    {
      serialData = Serial.readStringUntil("/n");
    }
    if (serialData != " ")
    {
      if (serialData == "SDC1") 
      {
        shutdown();// need to make shutdown function
      }
      else
      {
        Serial.print(serialData);
      }
    }
  }

  serialData = " ";
  digitalWrite(HW_PIN_MSCOM1, LOW);
}
  
  
}
