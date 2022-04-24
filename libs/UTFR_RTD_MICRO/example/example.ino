/**
 * @file example.ino
 * @author Kelvin Cui
 * @brief UTFR_RTD_MICRO sample implementation - shows required setup, and how to call it in the starting loop
 *      
 * @version 1.0
 * @date 2022-04-03
 * 
 * @copyright University of Toronto Formula Racing 
 * 
 * 
 */
#include "UTFR_RTD_MICRO.h"
#include "UTFR_APPS.h"

UTFR_RTD_MICRO RTD;
UTFR_APPS APPS;

int throttle = 100;

const int DAC_CS = 7;      // DAC CS pin
 
void setup() {
  Serial.begin(9600);
  APPS.begin(DAC_CS);
}

void loop() {
  //APPS.processThrottlePosition();
  //int throttle = APPS.getThrottlePosition();
  while (!RTD.confirmReady(throttle)){
      //check bools again
      delay(1000);
      //APPS.processThrottlePosition();
      //throttle = APPS.getThrottlePosition();
      throttle -= 10; //Throttle should go within valid range in 5s
      Serial.println(throttle);
  }
  Serial.println("RTD Ready!");
  delay(2000);
}