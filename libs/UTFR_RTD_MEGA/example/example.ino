/**
 * @file example.ino
 * @author Kelvin Cui
 * @brief UTFR_RTD_MEGA sample implementation - shows required setup, and how to call it in the starting loop
 *      
 * @version 1.0
 * @date 2022-04-03
 * 
 * @copyright University of Toronto Formula Racing 
 * 
 * 
 */
#include "UTFR_RTD_MEGA.h"

UTFR_RTD_MEGA RTD();

int fake_startup_counter = 0;
bool cooling = false;
bool sdc = true;
bool inv = true;
bool micro = true;
 
void setup() {
  Serial.begin(9600);
}

void loop() {
    while (!RTD.confirmReady(sdc, inv, micro, cooling)){
        //check bools again
        fake_startup_counter += 1;
        if (fake_startup_counter == 5){
            cooling = true; //Set Cooling to be ready for RTD
        }
    }
    Serial.println("RTD Ready!");
}