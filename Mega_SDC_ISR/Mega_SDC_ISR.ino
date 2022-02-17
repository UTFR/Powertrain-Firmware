#include <string>
#include <SoftwareSerial.h>

SoftwareSerial microSerial_1(2, 3); // (RX, TX)  --> need to change these pins
                                    // Note: RX pin on mega must correspond to TX pin on micro and vice-versa
                                    // SoftwareSerial port is used in a nearly identical way to standard Serial port

const int kRETRY_INTERVAL = 20; // Time between retries; milliseconds

unsigned long current_time, start_time = 0; // Times must be of type long because number of milliseconds quickly gets huge

string serialData = " ";

void setup()
{

  Serial.begin(115200);      // Start the hardware serial port (pins 0 and 1)
  microSerial_1.begin(9600); // Start the software serial port
                             // Note: Corresponding port on Micro must match this baud rate (I arbitrarily chose 9600)

  // set MS_COM_1_Digital pin as output (Micro is monitoring this pin for interrupts)
  pinMode(40, OUTPUT);

  // set MS_COM_3_Digital pin as input (we will read response from Micro on here)
  pinMode(44, INPUT);
}

void loop()
{

  // set MS_COM_1_Digital HIGH to trigger interrupt routine on Micro
  digitalWrite(40, HIGH);
  start_time = millis()

      // get start_time using millis()
      // while ( (current_time - start_time) < kRETRY_INTERVAL){
      //    if software serial port is available{
      //        read data from port (probably use readStringUntil method)
      //    }
      // }
      current_time = start_time;

  while ((current_time - start_time) < kRETRY_INTERVAL)
  {
    current_time = current_time + 1;

    if (mySerial.available() > 0)
    {
      serialData = Serial.readStringUntil();
    }
    if (serialData != " ")
    {
    }
  }

  //if (string_just_read != " "){
  //  if success{ call shutdown() function }
  //  else { print error message to hardware serial port and continue in loop() }
  //}

  // set MS_COM_1_Digital LOW to try triggering interrupt again
  serialData = " ";
}

void shutdown()
{
  // add logic later
}
