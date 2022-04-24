/**
 * @file example.ino
 * @author Kelvin Cui
 * @brief UTFR_ERROR sample implementation - shows required setup, and sends a sample_error over CAN to the datalogger.
 *      
 * @version 1.0
 * @date 2022-02-16
 * 
 * @copyright University of Toronto Formula Racing 
 * 
 * 
 */
#include "UTFR_ERROR.h"
#include "UTFR_CAN.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_CAN NODE(HW_PIN_CS_TEST);       // Create CAN node object
UTFR_ERROR ERROR;

int bms_overvoltage_counter = 0;    //Dummy delay counter
 
void setup() {
  
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
}

void loop() {
  delay(500);
  bms_overvoltage_counter++;
  if (bms_overvoltage_counter == 5) { //Dummy fail state
    ERROR.sendError(NODE,BMS_UNDERVOLT);
  }
}
