#include <SPI.h>

#include "MCP_DAC.h"

MCP4911 MCP;  //HW SPI

//declare global constants
const int APPS_1_HIGH = ;
const int APPS_2_HIGH = ;
const int APPS_1_LOW = ;
const int APPS_2_LOW = ;

//declare variables
int APPS_1_in = 0;
int APPS_2_in = 0; //0 - 255
float APPS_1_throttle = 0; 
float APPS_2_throttle = 0;
int APPS_out = 0;
bool all_good = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initialize SPI:
  SPI.begin();

  // initalize the  data ready and chip select pins:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A5, OUTPUT);
  // give the sensor time to set up:
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

  //read in the voltage values and convert to digital
  APPS_1_in = analogRead(A0);
  APPS_2_in = analogRead(A1);

  //model each APPS as a straight line
  APPS_1_throttle = 1023 * (APPS_1_in - APPS_1_LOW)/(APPS_1_HIGH - APPS_1_LOW);
  APPS_2_throttle = 1023 * (APPS_2_in - APPS_2_LOW)/(APPS_2_HIGH - APPS_2_LOW);
  
  //check if APPS_2 is within 4% of APPS_1
  all_good = abs((APPS_2_throttle - APPS_1_throttle)/APPS_1_throttle) < 0.04;

  //send out the values
  if (all_good) {
    APPS_out = (APPS_1_throttle + APPS_2_throttle) / 2;
    MCP.analogWrite(APPS_out, A5);
  } else {
    Serial.println("[APPS] Error: throttle values not in expected range of each other.");
    MCP.analogWrite(0, A5);
  }
  
}
