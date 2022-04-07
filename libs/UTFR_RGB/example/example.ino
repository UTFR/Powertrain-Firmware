/**
 * @file example.ino
 * @author Kelvin Cui
 * @brief UTFR_LED sample implementation - loops through different colors possible for LED
 *      
 * @version 1.0
 * @date 2022-04-07
 * 
 * @copyright University of Toronto Formula Racing 
 * 
 * 
 */
#include "UTFR_RGB.h"

UTFR_LED LED;
 
void setup() {
    //No setup required for LEDS
}

void loop() {
    LED->setColor(OFF);
    delay(500);
    LED->setColor(GREEN);
    delay(500);
    LED->setColor(YELLOW);
    delay(500);
    LED->setColor(RED);
    delay(500);
}
