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
#include "UTFR_COOLING.h"
#include "UTFR_CAN.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_COOLING COOLING();

UTFR_CAN CAN(HW_PIN_CS_TEST);       // Create CAN node object  

bool coolant_temp_ok, coolant_flow_ok, coolant_press_ok;


void setup() {
  CAN.begin(CAN_500KBPS);           // Initialize CAN node
  CAN.setFilters_permitAll();       // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h
}

void loop() {
  coolant_temp_ok = COOLING.checkTemp(CAN);       // Gets coolant temps, stores values in CAN array, returns false if critical overheating.
  if (!coolant_temps_ok)                            // Returns true if in safe range, turns on pumps if over a certain temp threshold (Ask Cristian)                 
  {                                                 // Pass CAN object BY REFERENCE to allow us to modify the array
    Serial.println("coolant_temp issue!")
  }

  coolant_press_ok = COOLING.checkPress(CAN);     // Gets coolant pressures, returns false if overpressure (stores values in CAN array)
  if (!coolant_press_ok)                            // returns false also if pump is ON && pressure is too low/zero
  {
    Serial.println("coolant_pressure issue!")
  }

  coolant_flow_ok = COOLING.checkFlow(CAN);       // Gets coolant flow rate, returns false if pump is ON && no flow, true otherwise
  if (!coolant_flow_ok)
  {
    Serial.println("coolant_flow issue!")
  }
}