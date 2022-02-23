#include <SoftwareSerial.h>
//#include APPS

#define BRAKE_THRESHOLD_ 128

byte sent_bytes;
char serial_recv;


SoftwareSerial megaSerial(10,13);

void setup(){

    Serial.begin(115200);
    megaSerial.begin(9600);

    pinMode(A2, INPUT); //Brake input

    //APPS Init

}


void loop() {
    if ((analogRead(A2) > BRAKE_THRESHOLD_)){ // && APPS->GetThrottle == 0
        sent_bytes = megaSerial.print("MICRO_READY");
        Serial.print("Sent MICRO_READY in ");
        Serial.print(sent_bytes);
        Serial.println(" bytes to Mega");
    }

    while (megaSerial.available() > 0){
        rec = megaSerial.read();
        if (rec == "MEGA_READY") {
            Serial.println("RTD Complete!");
            //APPS->enable input
        }
    }

    delay(50);
}