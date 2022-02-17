/**
 * @file UTFR_ERROR_CAN.ino
 * @author Kelvin Cui
 * @brief UTFR_ERROR_CAN sample code - shows required setup, and sends a sample_error over CAN to the datalogger.
 *        sample_errors will be defined per device/error_code
 * @version 1.0
 * @date 2022-02-16
 * 
 * @copyright University of Toronto Formula Racing 
 * 
 * 
 */

#include "libaries/UTFR_ERROR_CAN.h"

UTFR_ERROR_CAN error_can_(5);
unsigned char sample_error_[8] = {0, 1, 0, 1, 0, 1, 0, 1};


void setup () 
{
    Serial.begin(9600);
    error_can_.initCan();
}

void loop() 
{
    error_can_.sendError(sample_error);
    delay(1000);
}

