#include <string>
#include <SoftwareSerial.h>
#include "MCP_DAC.h"

/* Variables */
SoftwareSerial microSerial_1(2, 3); // (RX, TX)  --> need to change these pins
                                    // Note: RX pin on mega must correspond to TX pin on micro and vice-versa
                                    // SoftwareSerial port is used in a nearly identical way to standard Serial port
const int kRETRY_INTERVAL = 20;
unsigned long current_time, start_time = 0;
string serialData = " ";
MCP4911 DAC;

/*  Functions */
void shutdown()
{
  DAC.analogWrite(0, 0);
}

/* Program Execution */
void setup()
{

  Serial.begin(115200);
  microSerial_1.begin(9600);

  pinMode(40, OUTPUT);

  pinMode(44, INPUT);
}

void loop()
{

  digitalWrite(40, HIGH);
  start_time = millis()

      current_time = start_time;

  while ((current_time - start_time) < kRETRY_INTERVAL)
  {
    current_time = current_time + 1;

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
  digitalWrite(40, LOW);
}
