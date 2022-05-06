#include "UTFR_CAN.h"

#define HW_PIN_CS_TEST        47           // Change to proper CS pin
#define IGNITION_DIGITAL      38
#define IGNITION_LED_DIGITAL  46
#define INV_OK_DIGITAL        A14
#define CAN_INTERRUPT         3

UTFR_CAN NODE(HW_PIN_CS_TEST);       // Create CAN node object
 
void setup() {
  
  Serial.begin(9600);

  pinMode(IGNITION_DIGITAL, OUTPUT);
  pinMode(IGNITION_LED_DIGITAL, INPUT);
  pinMode(INV_OK_DIGITAL, INPUT);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_250KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h


  Serial.println("Starting...");

  Serial.println("IGNITION_DIGITAL IS HIGH");
  
  if (digitalRead(INV_OK_DIGITAL)) {
    Serial.println("INITAL INV_OK_DIGITAL IS HIGH");
  }

  else {
    Serial.println("INITAL INV_OK_DIGITAL IS LOW");
  }

  delay(1000);

  digitalWrite(IGNITION_DIGITAL, HIGH);

  while (!digitalRead(IGNITION_LED_DIGITAL)) {
    Serial.println("IGNITION_LED_DIGITAL IS LOW");
    delay(500);
  }

  Serial.println("IGNITION_LED_DIGITAL IS HIGH");

  while(digitalRead(INV_OK_DIGITAL)){
    Serial.println("INV_OK_DIGITAL IS HIGH");
    delay(500);
  }

  Serial.println("INV_OK_DIGITAL IS LOW");
}

void loop() {

  
  NODE.receiveMsgs();
  Serial.print("INV CAN MSG:");
  NODE.printMsgData(CAN_MSG_INV_INTERNAL_STATE);


  delay(1000);

  /*Serial.println("IGNITION_DIGITAL IS LOW");

  digitalWrite(IGNITION_DIGITAL, LOW);

  while (digitalRead(IGNITION_LED_DIGITAL)) {
    Serial.println("IGNITION_LED_DIGITAL IS HIGH");
    delay(500);
  }

  Serial.println("IGNITION_LED_DIGITAL IS LOW");

  while(!digitalRead(INV_OK_DIGITAL)){
    Serial.println("INV_OK_DIGITAL IS LOW");
    delay(500);
  }

  Serial.println("INV_OK_DIGITAL IS HIGH");

  delay(1000);*/

  

// ----------- SENDING ---------------------------------
  /*NODE.sendAll();                    // Send all messages defined in UTFR_CAN_version.h
  NODE.sendMsg(CAN_MSG_RF0);         // Send message by name, name defined in UTFR_CAN_version.h

// ----------- RECEIVING ------------------------------
  NODE.receiveMsgs();                // Receive messages waiting in Rx buffers (data will not be saved if not defined in UTFR_CAN_version.h
  // You can still receive via interrupts, see CAN-BUS Shield 1.0.0 library example "receive_interrupt" for logic

// ----------- GETTING/SETTING MSG DATA ---------------
  rf_out_tire_temp = NODE.getField(CAN_MSG_RF0, RF_OUT_TIRE_TEMP_F);       // Get field data by name
  NODE.setField(CAN_MSG_RF0, RF_OUT_TIRE_TEMP_F, rf_out_tire_temp+1);      // Set field data by name
  */
}
