//#include "UTFR_COMMS.h"
#include <SoftwareSerial.h>

//SoftwareSerial microSerial(14, 15); // RX, TX
char rec;
byte sent_bytes;

void setup() {

  Serial.begin(9600);
  //microSerial.begin(9600);
  Serial3.begin(9600);
  
}
void loop() {
  
  sent_bytes = Serial3.print("Hello from da mega");
  Serial.print("Sent bytes: "); Serial.println(sent_bytes);
  
  while(Serial3.available() > 0){
    rec = Serial3.read();
    Serial.print(rec);
  }
  Serial.println();
  
  delay(50);
  
}
