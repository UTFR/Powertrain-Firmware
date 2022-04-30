/************************
 * Only works if #define _2_NODESS_ is uncommented in UTFR_CAN.h
 *************************/

#include "UTFR_CAN_RC.h"

#define HW_PIN_CS_TEST1 10                             // Change to proper CS pin
#define HW_PIN_CS_TEST2 11                             // Change to proper CS pin

UTFR_CAN_RC NODES(HW_PIN_CS_TEST1, HW_PIN_CS_TEST2);       // Create CAN NODES object

int throttle_pos;
 
void setup() {
  
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  NODES.begin(CAN_500KBPS, 3);      // Initialize both CAN nodes
  NODES.setFilters_permitAll(3);    // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h
}

void loop() {

// ----------- SENDING ---------------------------------
// --- NODES 1 ---
  NODES.sendAll(1);                   // Send all messages defined in UTFR_CAN_version.h
  NODES.sendMsg(CAN_MSG_MICRO, 1);      // Send message by name, name defined in UTFR_CAN_version.h
// --- NODES 2 ---
  NODES.sendAll(2);                   // Send all messages defined in UTFR_CAN_version.h
  NODES.sendMsg(CAN_MSG_MICRO, 2);      // Send message by name, name defined in UTFR_CAN_version.h
// --- Both ---
  NODES.sendAll(3);
  NODES.sendMsg(CAN_MSG_MICRO, 3);

// ----------- RECEIVING ------------------------------
  NODES.receiveMsgs(3);                // Receive messages waiting in Rx buffers (data will not be saved if not defined in UTFR_CAN_version.h
  // You can still receive via interrupts, see CAN-BUS Shield 1.0.0 library example "receive_interrupt" for logic

// ----------- GETTING/SETTING MSG DATA ---------------
  throttle_pos = NODES.getField(CAN_MSG_MICRO, THROTTLE_POS_F);       // Get field data by name
  NODES.setField(CAN_MSG_MICRO, THROTTLE_POS_F, throttle_pos+1);      // Set field data by name
  
}