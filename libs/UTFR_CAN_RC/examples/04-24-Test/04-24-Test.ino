/************************
 * Only works if #define _1_NODE_ is uncommented in UTFR_CAN.h
 *************************/

#include "UTFR_CAN_RC.h"

#define HW_PIN_CS_TEST 48           // Change to proper CS pin
#define CAN_RECEIVE_INT_PIN 2       // CAN interrupt pin

UTFR_CAN NODE(HW_PIN_CS_TEST);       // Create CAN node object

int rf_out_tire_temp;
 
void setup() {
  
  Serial.begin(9600);
  pinMode(CAN_RECEIVE_INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CAN_RECEIVE_INT_PIN), receive, HIGH);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h
}

void loop() {

  /*if (digitalRead(CAN_RECEIVE_INT_PIN) == HIGH) {
    /*unsigned long timeBegin = micros();
    NODE.receiveMsgs();
    rf_out_tire_temp = NODE.getField(CAN_MSG_RF0, RF_OUT_TIRE_TEMP_F);
    Serial.println(rf_out_tire_temp);  
    double averageDuration = (double)(micros() - timeBegin) / 1000.0;
    Serial.print("HIGH ");
    Serial.println(averageDuration);  
  //}*/
  

// ----------- SENDING ---------------------------------
  //NODE.sendAll();                    // Send all messages defined in UTFR_CAN_version.h
  //NODE.sendMsg(CAN_MSG_RF0);         // Send message by name, name defined in UTFR_CAN_version.h
}

void receive() {
  unsigned long timeBegin = micros();
  NODE.receiveMsgs();
  rf_out_tire_temp = NODE.getField(CAN_MSG_RF0, RF_OUT_TIRE_TEMP_F);    
  double averageDuration = (double)(micros() - timeBegin) / 1000.0;
  Serial.println(averageDuration);
}