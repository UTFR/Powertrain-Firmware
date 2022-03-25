#include "UTFR_CAN_CORNER_RR.h"

#define HW_PIN_CS_TEST 10     
#define HW_PIN_SENSOR_1 A0  
#define HW_PIN_SENSOR_2 A1
#define HW_PIN_SENSOR_3 A2 
#define HW_PIN_SENSOR_4 A3 
#define HW_PIN_SENSOR_5 A4  
#define HW_PIN_SENSOR_6 A5
#define HW_PIN_SENSOR_7 A6
#define HW_PIN_SENSOR_8 A7
#define HW_PIN_SENSOR_9 A8              

const int data_change_interval = 100;             // delay before changing data bytes; milliseconds

unsigned long tick, tock, tx_count = 0;
int log_after = 10;                               // send log msg every (data_change_interval)*(log_after) milliseconds
int change_count = 0;                             // counts up to log_after
int sensor_data = 0;

unsigned char TEC, EFLG = 0;                      // Data to report node status stored in these

UTFR_CAN_CORNER_RR NODE(HW_PIN_CS_TEST);          // Create CAN node object

 
void setup() {
  
  Serial.begin(9600);
  while(!Serial);

  pinMode(HW_PIN_SENSOR_1, INPUT);
  pinMode(HW_PIN_SENSOR_2, INPUT);
  pinMode(HW_PIN_SENSOR_3, INPUT);
  pinMode(HW_PIN_SENSOR_4, INPUT);
  pinMode(HW_PIN_SENSOR_5, INPUT);
  pinMode(HW_PIN_SENSOR_6, INPUT);
  pinMode(HW_PIN_SENSOR_7, INPUT);
  pinMode(HW_PIN_SENSOR_8, INPUT);
  pinMode(HW_PIN_SENSOR_9, INPUT);
  
  // ----------- SIMPLE SETUP ----------------------------
  NODE.begin(CAN_500KBPS);          // Initialize CAN node
  NODE.setFilters_permitNone();     // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h
  tick = millis();
  
}

void loop() {

// ----------- SENDING ---------------------------------
  NODE.sendMsg(CAN_MSG_50Hz);      // 65 Hz min rx        
  delay(17);

  NODE.sendMsg(CAN_MSG_50Hz); 
  delay(17);

  NODE.sendMsg(CAN_MSG_50Hz);
  
  NODE.sendMsg(CAN_MSG_20Hz_1);                 
  NODE.sendMsg(CAN_MSG_20Hz_2);
  delay(16);

  tx_count = tx_count + 5;

  // Change data bytes once every second, send log msg once every (data_change_interval)*(log_after) milliseconds
  tock = millis();
  if ((tock - tick) > data_change_interval){
      /*
      switch(change_count)
      {
        case 0:
          sensor_data = analogRead(HW_PIN_SENSOR_1);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 1:
          sensor_data = analogRead(HW_PIN_SENSOR_2);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 2:
          sensor_data = analogRead(HW_PIN_SENSOR_3);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 3:
          sensor_data = analogRead(HW_PIN_SENSOR_4);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 4:
          sensor_data = analogRead(HW_PIN_SENSOR_5);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 5:
          sensor_data = analogRead(HW_PIN_SENSOR_6);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 6:
          sensor_data = analogRead(HW_PIN_SENSOR_7);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 7:
          sensor_data = analogRead(HW_PIN_SENSOR_8);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
        case 8:
          sensor_data = analogRead(HW_PIN_SENSOR_9);
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, sensor_data);
      }
      */
      
      if (NODE.getField(CAN_MSG_20Hz_1, RR_20Hz_1_F) == 6969){
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, 1111);
          NODE.setField(CAN_MSG_20Hz_2, RR_20Hz_2_F, 1111);
          NODE.setField(CAN_MSG_50Hz, RR_50Hz_F, 1111);
      }
      else{
          NODE.setField(CAN_MSG_20Hz_1, RR_20Hz_1_F, 6969);
          NODE.setField(CAN_MSG_20Hz_2, RR_20Hz_2_F, 6969);
          NODE.setField(CAN_MSG_50Hz, RR_50Hz_F, 6969);
      } 

      if (change_count >= log_after) {        // log mcp2515 Transmit Error Count (TEC) and Error Flags (EFLG)  
          
          TEC = NODE.readReg(MCP_TEC);
          EFLG = NODE.readReg(MCP_EFLG);

          NODE.setField(CAN_MSG_STATUS, TEC_F, TEC);
          NODE.setField(CAN_MSG_STATUS, EFLG_F, EFLG);
          NODE.setField(CAN_MSG_STATUS, TX_CNT_F, tx_count);
          
          NODE.sendMsg(CAN_MSG_STATUS);
          
          change_count = 0;
      }    
      
      change_count++;
      tick = millis();
  }
  
}
