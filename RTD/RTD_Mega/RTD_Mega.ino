//#include DASHRGB CLASS
//#include CAN ERROR CLSS
//#include COOLING LOOP CLASS
#include <SoftwareSerial.h>


//Interrupt Pins

const int SDC_INT_PIN = 19;
const int INV_INT_PIN = 18;

//Polling Pins
const int IGNITION_IN_PIN = 21;
const int RTD_IN_PIN = 25;

//Output Pins
const int RTD_OUT_PIN = 24;
const int IGNITION_OUT_PIN = 38;

bool inverter_ = true;
bool sdc_ = true;
bool micro_ = false;

char serial_rec;
byte sent_bytes;



bool rtd_fail_ = false;


void setup()
{
    Serial.begin(9600);
    Serial3.begin(9600);

    pinMode(RTD_IN_PIN, INPUT);

    pinMode(BRAKE_PIN, INPUT);
    pinMode(IGNITION_INT_PIN, INPUT);

    pinMode(RTD_OUT_PIN, OUTPUT);
    pinMode(IGNITION_OUT_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(SDC_INT_PIN), sdcISR, RISING);
    attachInterrupt(digitalPinToInterrupt(INV_INT_PIN), invISR, RISING);

    //COOLING->initalize
    //ERROR->initalize
    //RGB->initalize
}

void loop()
{
    while(Serial3.available() > 0){
        rec = Serial3.read()
        if (rec == "MICRO_READY") {
            micro_ = true;
        }
    }

    if (digitalRead(RTD_IN_PIN) == 1)
    {
        /*if (COOLING->getStatus == false) {
            //CAN_ERROR->sendMsg(COOLANT_RTD_MSG);
            //DASHRGB->sendERROR
            rtd_fail_ = true;
        }*/
        if (!sdc_) {
            //CAN_ERROR->sendMsg(SDC_RTD_MSG);
            //DASHRGB->sendERROR
            rtd_fail_ = true;
        } 
        if (!inverter_) {
            //CAN_ERROR->sendMsg(INVERTER_RTD_MSG);
            //DASHRGB->sendERROR
            rtd_fail_ = true;
        }
        if (!micro_) {
            //CAN_ERROR->sendMsg(THROTTLE_BRAKE_RTD_MSG);
            //DASHRGB->sendERROR
            rtd_fail_ = true; 
        }
        

        if (rtd_fail_){
            //Check if Ingnition Switch is still high:
            if (digitalRead(IGNITION_IN_PIN) == HIGH) {
                resetBools();
            }
        }
        else {
            digitalWrite(RTD_OUT_PIN, HIGH);
            digitalWrite(IGNITION_OUT_PIN, HIGH);
            sent_bytes = Serial3.print("MEGA_READY");
            Serial.print("RTD Done - sent ");
            Serial.print(sent_bytes);
            Serial.print(" bytes to Micro for APPS activation");
            //DASHRGB->sendActive
            //Shutdown(?)
        }

    }
    delay(1000);
}

void resetBools()
{
    bool inverter_ = true;
    bool sdc_ = true;
    bool micro_ = false; 
}

void sdcISR()
{
    sdc_ = false;
}

void invISR()
{
    inverter_ = false;
}

