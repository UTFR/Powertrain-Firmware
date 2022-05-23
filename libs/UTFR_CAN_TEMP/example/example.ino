#include "UTFR_CAN_TEMP.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_CAN_TEMP NODE(HW_PIN_CS_TEST);       // Create CAN node object
 
void setup() {
  
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_TEMP_version.h


  //Set to default values from Ayrton's BMS_TEMP_CAN_TEST folder:
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_ID_MSB, 0x0);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_ID_LSB, 0xE);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_NUM_ENABLED, 0xE);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_LOWEST_VAL, 0xE);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_HIGHEST_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_HIGHEST_ID, 0xE);
  NODE.setField(CAN_MSG_THERM_GENERAL, THERM_GENERAL_LOWEST_ID, 0xE);


  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_MODULE_NUM, 0x0);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_LOWEST_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_HIGHEST_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_AVG_VAL, 0x15);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_NUM_ENABLED, 0x1);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_HIGHEST_ID, 0xE);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_LOWEST_ID, 0xE);
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_CHECKSUM, 0x9D);

  NODE.sendMsg(CAN_MSG_THERM_LEGACY);         // Send Legacy Message First
}

void loop() {

// ----------- SENDING ---------------------------------
  NODE.sendMsg(CAN_MSG_THERM_BROADCAST); 
  NODE.sendMsg(CAN_MSG_THERM_GENERAL);               

}
