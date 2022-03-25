//#include "UTFR_COMMS.h"
#include <SoftwareSerial.h>

char rec;
byte sent_bytes;

void SDC_ISR(void){
  Serial.println("INTERRUPT RECIEVED SON!");
}

void setup() {

  Serial.begin(9600);
  //microSerial.begin(9600);
  Serial3.begin(9600);

  pinMode(42, OUTPUT);      // sending INT to Micro on MS_COM4

  pinMode(43, INPUT);       // receiving INT from Micro on MS_COM5
  attachInterrupt(digitalPinToInterrupt(43), SDC_ISR, RISING);
  
}
void loop() {
  
  sent_bytes = Serial3.print("Hello from da mega");
  Serial.print("Sent bytes: "); Serial.println(sent_bytes);
  
  while(Serial3.available() > 0){
    rec = Serial3.read();
    Serial.print(rec);
  }
  Serial.println();
  
  delay(1000);
  
  digitalWrite(42,HIGH);
  delay(200);
  digitalWrite(42, LOW);

}
