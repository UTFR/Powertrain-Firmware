#include "UTFR_CAN_TEMP.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_CAN_TEMP NODE(HW_PIN_CS_TEST);       // Create CAN node object
 
void setup() {
  
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages

  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_HIGHEST_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_LOWEST_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_AVG_VAL, 0x15);
}

void loop() {

// ----------- SENDING ---------------------------------
  NODE.sendMsg(CAN_MSG_THERM_BROADCAST); 

  NODE.printMsgData(CAN_MSG_THERM_BROADCAST);             
}
