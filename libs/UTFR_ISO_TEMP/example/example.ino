#include "UTFR_ISO_TEMP.h"
#include "UTFR_CAN_TEMP.h"

#define HW_PIN_CS_TEST 10           // Change to proper CS pin

UTFR_CAN_TEMP NODE(HW_PIN_CS_TEST); 

UTFR_ISO_TEMP ISO_TEMP;

temp_S current_temps; 

void setup(){
  Serial.begin(9600);

// ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);           // Initialize CAN node
  NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_TEMP_version.h
  ISO_TEMP.begin();
}

void loop() {
  ISO_TEMP.sensorLoop();
  current_temps = ISO_TEMP.getTemps();
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_HIGHEST_VAL, static_cast<int8_t>(current_temps.highest_temp));
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_LOWEST_VAL, static_cast<int8_t>(current_temps.lowest_temp));
  NODE.setField(CAN_MSG_THERM_BROADCAST, THERM_BROADCAST_AVG_VAL, static_cast<int8_t>(current_temps.avg_temp));

  NODE.sendMsg(CAN_MSG_THERM_BROADCAST); 
  delay(250); 
}