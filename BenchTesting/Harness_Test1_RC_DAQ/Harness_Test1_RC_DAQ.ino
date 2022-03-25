#include "UTFR_CAN_RC.h"
#include "TimerOne.h"


#define HW_PIN_CAN_DAQ_INT 2
#define HW_PIN_CAN_PWRTRAIN_INT 3
#define HW_PIN_CAN_PWRTRAIN_CS 47  
#define HW_PIN_CAN_DAQ_CS 48

#define DELAY_10 100000
#define DELAY_50 20000                       

const int data_change_interval = 1000;            // delay before changing data bytes; milliseconds

unsigned long tick, tock, tx_count = 0;
int log_after = 10;                               // send log msg every (data_change_interval)*(log_after) milliseconds
int change_count = 0;                             // counts up to log_after

unsigned char TEC, EFLG = 0;                      // Data to report node status stored in these

UTFR_CAN_RC NODE_DAQ(HW_PIN_CAN_DAQ_CS);


void send10()
{
  NODE_DAQ.sendMsg(CAN_MSG_BMS_1);
  NODE_DAQ.sendMsg(CAN_MSG_BMS_2);
  NODE_DAQ.sendMsg(CAN_MSG_BMS_3);
  tx_count += 3;
}

void send50()
{
  
  NODE_DAQ.sendMsg(CAN_MSG_INV_1);
  NODE_DAQ.sendMsg(CAN_MSG_INV_2);
  tx_count += 2;
  
  if (tx_count % 10 == 0)
  {
    NODE_DAQ.sendMsg(CAN_MSG_BMS_1);
    NODE_DAQ.sendMsg(CAN_MSG_BMS_2);
    NODE_DAQ.sendMsg(CAN_MSG_BMS_3);
    tx_count += 2;
  }
  
}
 
void setup() {

  pinMode(HW_PIN_CAN_DAQ_INT, INPUT);
  pinMode(HW_PIN_CAN_PWRTRAIN_INT, INPUT);
  
  Serial.begin(9600);
  while(!Serial);

  // ----------- SIMPLE SETUP ----------------------------
  NODE_DAQ.begin(CAN_500KBPS);          // Initialize CAN node
  NODE_DAQ.setFilters_permitNone();     // Set filters to accept all incoming messages
                                    // ** Note: Msgs will not be saved if not defined int UTFR_CAN_version.h
  
  Timer1.initialize(DELAY_50);
  Timer1.attachInterrupt(send50, DELAY_50);
  //Timer1.attachInterrupt(send10, DELAY_10);
                                    
  tick = millis();
  
}

void loop() {

  // Change data bytes once every second, send log msg once every (data_change_interval)*(log_after) milliseconds
  tock = millis();
  if ((tock - tick) > data_change_interval)
  {
      if (NODE_DAQ.getField(CAN_MSG_BMS_1, BMS_1_F) == 6969)
      {
          NODE_DAQ.setField(CAN_MSG_BMS_1, BMS_1_F, 1111);
          NODE_DAQ.setField(CAN_MSG_BMS_2, BMS_2_F, 1111);
          NODE_DAQ.setField(CAN_MSG_BMS_3, BMS_3_F, 1111);
          NODE_DAQ.setField(CAN_MSG_INV_1, INV_1_F, 1111);
          NODE_DAQ.setField(CAN_MSG_INV_2, INV_2_F, 1111);
      }
      else
      {
          NODE_DAQ.setField(CAN_MSG_BMS_1, BMS_1_F, 6969);
          NODE_DAQ.setField(CAN_MSG_BMS_2, BMS_2_F, 6969);
          NODE_DAQ.setField(CAN_MSG_BMS_3, BMS_3_F, 6969);
          NODE_DAQ.setField(CAN_MSG_INV_1, INV_1_F, 6969);
          NODE_DAQ.setField(CAN_MSG_INV_2, INV_2_F, 6969);
      }
      
      if (change_count >= log_after)                    // log mcp2515 Transmit Error Count (TEC) and Error Flags (EFLG)  
      {        
          TEC = NODE_DAQ.readReg(MCP_TEC);
          EFLG = NODE_DAQ.readReg(MCP_EFLG);

          NODE_DAQ.setField(CAN_MSG_STATUS, TEC_F, TEC);
          NODE_DAQ.setField(CAN_MSG_STATUS, EFLG_F, EFLG);
          NODE_DAQ.setField(CAN_MSG_STATUS, TX_CNT_F, tx_count);
          
          NODE_DAQ.sendMsg(CAN_MSG_STATUS);

          change_count = 0;
      }   

       NODE_DAQ.sendAll();

      change_count++;
      tick = millis();

      delay(20);
  }
  
}
