/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SoftwareSerial.h>
#include "UTFR_CAN_RC.h"
#include "TimerOne.h"


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define HW_PIN_MSCOM3 20
#define HW_PIN_MSCOM1 40
#define HW_PIN_MSCOM2 41
#define HW_PIN_MSCOM4 42
#define HW_PIN_MSCOM5 43


#define HW_PIN_CAN_DAQ_INT 2
#define HW_PIN_CAN_PWRTRN_INT 3
#define HW_PIN_CAN_PWRTRN_CS 47  
#define HW_PIN_CAN_DAQ_CS 48


#define DELAY_60 16667        // Delay used for 60Hz

/******************************************************************************
 *         F U N C T I O N   D E C L A R A T I O N S                          *
 *****************************************************************************/
 

/******************************************************************************
 *              D A T A   D E F I N I T I O N S                               * 
 *****************************************************************************/
// ================== CAN
UTFR_CAN_RC NODE(HW_PIN_CAN_DAQ_CS);

const int data_change_interval = 1000;            // delay before changing data bytes; milliseconds
unsigned long tick, tock, tx_count = 0;           
int log_after = 10;                               // send log msg every (data_change_interval)*(log_after) milliseconds
int change_count = 0;                             // counts up to log_after
unsigned char TEC, EFLG = 0;                      // Data to report node status stored in these
bool CAN_int_flag = false;

// ================== Serial comms 
char rx_byte;
bool serial_int_flag = false;

uint8_t interrupt_rx_pin = HW_PIN_MSCOM3;
uint8_t interrupt_tx_pin = HW_PIN_MSCOM4;



/******************************************************************************
 *                     F U N C T I O N S                                      *
 *****************************************************************************/

void SDC_ISR(void){
  serial_int_flag = true;
}

void send60()
{
  CAN_int_flag = true;
}


/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup() {

    Serial.begin(115200);

// ================== CAN
    NODE.begin(CAN_500KBPS);           // Initialize CAN node
    NODE.setFilters_permitAll();       // Set filters to accept all incoming messages
    pinMode(HW_PIN_CAN_DAQ_INT, INPUT);
    pinMode(HW_PIN_CAN_PWRTRN_INT, INPUT);
    Timer1.initialize(DELAY_60);
    Timer1.attachInterrupt(send60, DELAY_60);
                                    

// ================== Serial comms 
    Serial3.begin(115200);
    pinMode(interrupt_rx_pin, INPUT);
    pinMode(interrupt_tx_pin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(interrupt_rx_pin), SDC_ISR, RISING);   // Recieving INT from Mega on MS_COM4

    
    tick = millis();
}

void loop() {

// ================== CAN ===========================================
  if (CAN_int_flag)
  {
    NODE.sendMsg(CAN_MSG_INV_1);
    NODE.sendMsg(CAN_MSG_INV_2);
    tx_count += 2;
    
    if (tx_count % 15 == 0)
    {
      NODE.sendMsg(CAN_MSG_BMS_1);
      NODE.sendMsg(CAN_MSG_BMS_2);
      NODE.sendMsg(CAN_MSG_BMS_3);
      tx_count += 3;
    }
    CAN_int_flag = false;
  }

  tock = millis();
  if ((tock - tick) > data_change_interval)                           // Change data bytes once every second
  {
      if (NODE.getField(CAN_MSG_BMS_1, BMS_1_F) == 6969)
      {
          NODE.setField(CAN_MSG_BMS_1, BMS_1_F, 1111);
          NODE.setField(CAN_MSG_BMS_2, BMS_2_F, 1111);
          NODE.setField(CAN_MSG_BMS_3, BMS_3_F, 1111);
          NODE.setField(CAN_MSG_INV_1, INV_1_F, 1111);
          NODE.setField(CAN_MSG_INV_2, INV_2_F, 1111);
      }
      else
      {
          NODE.setField(CAN_MSG_BMS_1, BMS_1_F, 6969);
          NODE.setField(CAN_MSG_BMS_2, BMS_2_F, 6969);
          NODE.setField(CAN_MSG_BMS_3, BMS_3_F, 6969);
          NODE.setField(CAN_MSG_INV_1, INV_1_F, 6969);
          NODE.setField(CAN_MSG_INV_2, INV_2_F, 6969);
      }
      
      if (change_count >= log_after)                                  // Send status msg once every (data_change_interval)*(log_after) milliseconds  
      {        
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
  
  
// ================== Serial comms ===================================
 
  Serial3.print("Hello from da Mega \n");        // Send msg to Micro
  
  while (Serial3.available() > 0)                // Receive serial msg from Micro
  {               
    rx_byte = Serial3.read();
    Serial.print(rx_byte);
  } 
  Serial.println();

  if (serial_int_flag)                               // Check for interrupt from Micro
  {
    serial_int_flag = false;
    Serial.print("Interrupt received on MSCOM"); Serial.println(interrupt_rx_pin);
  }
  
  digitalWrite(interrupt_tx_pin,HIGH);              // Send interrupt to Micro
  delayMicroseconds(10);
  digitalWrite(interrupt_tx_pin,LOW);
}
