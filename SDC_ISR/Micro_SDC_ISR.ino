#include <SoftwareSerial.h>
#include "MCP_DAC.h"

MCP4911 DAC;


MCP4911 DAC;

/*  Functions */
void shutdown()
{
  DAC.analogWrite(0, 0);
}





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

    digitalWrite(40, LOW);
}
