#include <string>
#include <SoftwareSerial.h>
#include "MCP_DAC.h"
#define HW_PIN_MSCOM1 40
#define HW_PIN_MSCOM2 41
/* Variables */

const int kRETRY_INTERVAL = 20;
unsigned long current_time, start_time = 0;

string serialData = " ";

/* Program Execution */
void setup()
{

  Serial.begin(115200);
  Serial3.begin(9600);

  pinMode(HW_PIN_MSCOM1, OUTPUT);

  pinMode(HW_PIN_MSCOM2, INPUT);
}

void loop()
{

  digitalWrite(HW_PIN_MSCOM1, HIGH);
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
      if (serialData == "sucess") // change the success parameter later
      {
        shutdown();
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
