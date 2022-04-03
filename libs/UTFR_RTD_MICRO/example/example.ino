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

UTFR_RTD_MICRO RTD;
 
void setup() {
  Serial.begin(9600);
}

void loop() {
    while (!RTD.confirmReady()){
        //check bools again
        delay(500);
    }
    Serial.println("RTD Ready!");
}