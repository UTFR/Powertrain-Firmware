#include "UTFR_CAN.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_CAN NODE(HW_PIN_CS_TEST);       // Create CAN node object

int rf_out_tire_temp;
 
void setup() {
  
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h
}

void loop() {

// ----------- SENDING ---------------------------------
  NODE.sendAll();                    // Send all messages defined in UTFR_CAN_version.h
  NODE.sendMsg(CAN_MSG_RF0);         // Send message by name, name defined in UTFR_CAN_version.h

// ----------- RECEIVING ------------------------------
  NODE.receiveMsgs();                // Receive messages waiting in Rx buffers (data will not be saved if not defined in UTFR_CAN_version.h
  // You can still receive via interrupts, see CAN-BUS Shield 1.0.0 library example "receive_interrupt" for logic

// ----------- GETTING/SETTING MSG DATA ---------------
  rf_out_tire_temp = NODE.getField(CAN_MSG_RF0, RF_OUT_TIRE_TEMP_F);       // Get field data by name
  NODE.setField(CAN_MSG_RF0, RF_OUT_TIRE_TEMP_F, rf_out_tire_temp+1);      // Set field data by name
  
}
