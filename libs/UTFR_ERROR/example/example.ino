#include "UTFR_ERROR.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_ERROR ERROR_NODE(HW_PIN_CS_TEST);       // Create Error Node Object

int bms_overvoltage_counter = 0; //Dummy delay counter
 
void setup() {
  
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  ERROR_NODE.begin(CAN_500KBPS);           // Initialize CAN node
}

void loop() {
  delay(500);
  bms_overvoltage_counter++;
  if (bms_overvoltage_counter == 5) { //Dummy fail state
    ERROR_NODE.sendError(BMS_UNDERVOLT);
  }
}
