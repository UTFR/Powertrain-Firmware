#include "SCL3300.h"

#define HW_PIN_ACCEL_CS 49
#define HW_PIN_OE 32
//#define HW_PIN_CHASSIS_DSC 39

SCL3300 accel;


void setup() {
  
  Serial.begin(9600);
  while(!Serial){};  //pinMode(HW_PIN_CHASSIS_DSC, OUTPUT);
  //digitalWrite(HW_PIN_CHASSIS_DSC, HIGH);     // connect BMS ground to chassis ground

  pinMode(HW_PIN_OE, OUTPUT);
  digitalWrite(HW_PIN_OE, HIGH);              // enable level shifting on accel & gyro breakout boards
  
  pinMode(HW_PIN_ACCEL_CS, OUTPUT);
  
  while (accel.begin(HW_PIN_ACCEL_CS) == false){
    Serial.println("Failed to initialize accelerometer.");
    delay(1000);
  }
}

void loop() {
  if (accel.available()) { //Get next block of data from sensor
    Serial.print("X Accelerometer: ");
    Serial.print(accel.getCalculatedAccelerometerX());
    Serial.print("\t");
    Serial.print("Y Accelerometer: ");
    Serial.print(accel.getCalculatedAccelerometerY());
    Serial.print("\t");
    Serial.print("Z Accelerometer: ");
    Serial.println(accel.getCalculatedAccelerometerZ());
    delay(250); //Allow a little time to see the output
    /*
    Serial.print("Raw X Tilt: ");
    Serial.print(accel.sclData.AngX);
    Serial.print("\t");
    Serial.print("Y Tilt: ");
    Serial.print(accel.sclData.AngY);
    Serial.print("\t");
    Serial.print("Z Tilt: ");
    Serial.println(accel.sclData.AngZ);
    Serial.print("Raw X Accel: ");
    Serial.print(accel.sclData.AccX);
    Serial.print("\t");
    Serial.print("Y Accel: ");
    Serial.print(accel.sclData.AccY);
    Serial.print("\t");
    Serial.print("Z Accel: ");
    Serial.println(accel.sclData.AccZ);
    Serial.print("SCL3300 STO register: ");
    Serial.print((accel.sclData.STO >> 8) & 0xff, HEX);
    Serial.print("\t");
    Serial.print("WHOAMI register: ");
    Serial.println(accel.sclData.WHOAMI, HEX);
    Serial.print("Raw Temperature: ");
    Serial.println(accel.sclData.TEMP);
    Serial.print("Serial Number Register: ");
    Serial.print(accel.getSerialNumber());
    Serial.print("B33");
    Serial.print("\t");
    Serial.print("SL3300 Status Summary Register: ");
    Serial.println(accel.sclData.StatusSum, HEX);*/
    delay(50);
  } else accel.reset();
}
