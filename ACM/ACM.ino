// IMPORTANT NOTE - USE THE ATMEGA328P (OLD PROGRAMMER)
/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_HW_ACM.h"
#include "UTFR_IO_THERMISTORS_ACM.h"
#include "UTFR_IO_RELAYS_ACM.h"
#include "UTFR_CAN_ACM.h"

#include <TimerOne.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define debug_ACM             // Uncomment for debug prints

#define PRECHARGE_DURATION 3500
#define DIGITAL_LOGIC_THRESHOLD 2.5

/******************************************************************************
 *                      D A T A   D E F I N I T I O N S                       *
 *****************************************************************************/
//--------- State Machine -------------
enum carState_E
{                                       
  CAR_STATE_INIT = 0,
  CAR_STATE_PRECHARGE,
  CAR_STATE_DRIVE,
  CAR_STATE_SHUTDOWN,
};
carState_E carState = CAR_STATE_INIT;

//------- Class Instantiation ---------
UTFR_CAN_ACM CAN(HW_pins[HW_PIN_CAN1_CS].pinNum,       // TO DO: Which one's which? Node 1 is on the BMS CANbus?
                 HW_pins[HW_PIN_CAN2_CS].pinNum);      // Node 2 is on the powertrain CANbus? 

//--------- RTD -----------------------
const uint8_t RTD_reqConfirm = 20;
uint8_t RTD_reqCount = 0;
const uint16_t buzzerDelay = 1500;              // Milliseconds to turn RTD buzzer on for after precharge success
unsigned long buzzerStart = 0;                  // Marks start time when buzzer turns on

//--------- Thermals ------------------
const int boardTempPollEvery = 2000;            // Check PCB temps every 'boardTempPollEvery' milliseconds
unsigned long boardTempPollLast = 0;            // Marks last time board temps were polled

const uint8_t boardOvertempConfirm = 3;       
uint8_t boardOvertempCount = 0;                 // Counts up to 'boardOvertempConfirm' then shuts down car

//--------- CAN -----------------------
volatile bool msgWaiting = false;               // Flipped true in interrupt routine when CAN message waiting in Rx buffer

//--------- BMS -----------------------
const uint16_t BMS_heartbeatEvery = 200000;     // Microseconds between sending heartbeat msg to BMS
                                                // TS != nTS also checked in this ISR (only have TimerOne on Nano)

//--------- Tractive System -----------
const uint8_t TS_errorConfirm = 3;              
uint8_t TS_errorCount = 0;                      // Counts up to TS_errorConfirm (each TS==nTS) then shuts down car

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/
void benchSelfTest(void);
bool doPrecharge(void);
void shutdownCar(void);
bool BMS_checkFaults(void);

void CAN_msgWaitingISR(void);
void BMS_heartbeatISR(void);

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/
void benchSelfTest(void) // only to be used on the bench with NO high voltage present. The self-test logic will be different in the car.
{
  #ifdef debug_ACM
  Serial.println("======== BEGINNING SELF TEST ========");
  #endif

  // Check for relay faults
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);

  // Read and display relay states
  IO_relayState_E airpState = IO_getRelayState(IO_RELAY_AIRP);
  IO_relayState_E airnState = IO_getRelayState(IO_RELAY_AIRN);
  IO_relayState_E prechargeState = IO_getRelayState(IO_RELAY_PRECHARGE);

  if (airpState == IO_RELAY_SAFE)
  {
    #ifdef debug_ACM
    Serial.println("AIRp safe.");
    #endif
  }

  if (airnState == IO_RELAY_SAFE)
  {
    #ifdef debug_ACM
    Serial.println("AIRn safe.");
    #endif
  }

  if (prechargeState == IO_RELAY_SAFE)
  {
    #ifdef debug_ACM
    Serial.println("Precharge relay safe.");
    #endif
  }

  // Respond to error states
  if (airpState == IO_RELAY_FAULT)
  {
    #ifdef debug_ACM
    Serial.println("AIRp fault!");
    #endif
    IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  }
  
  if (airnState == IO_RELAY_FAULT)
  {
    #ifdef debug_ACM
    Serial.println("AIRn fault!");
    #endif
    IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  }

  if (prechargeState == IO_RELAY_FAULT)
  {
    #ifdef debug_ACM
    Serial.println("Precharge relay fault!");
    #endif
    IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
  }

  #ifdef debug_ACM
  Serial.println("=========== RELAY STATES ===========");
  printRelayState(IO_RELAY_AIRP, airpState);
  printRelayState(IO_RELAY_AIRN, airnState);
  printRelayState(IO_RELAY_PRECHARGE, prechargeState);
  #endif
}

bool doPrecharge(void)
{
  if ((HW_readMux(HW_MUX_nTS_ENERGIZED) < DIGITAL_LOGIC_THRESHOLD) || 
      (HW_readMux(HW_MUX_PRECHARGE_DONE) > DIGITAL_LOGIC_THRESHOLD) || 
      (IO_getRelayState(IO_RELAY_PRECHARGE) != IO_RELAY_SAFE) ||
      (IO_getRelayState(IO_RELAY_AIRP) != IO_RELAY_SAFE) ||
      (IO_getRelayState(IO_RELAY_AIRN) != IO_RELAY_SAFE))
  {
    // FAILURE: off-state signals are incorrect
    #ifdef debug_ACM
    Serial.println("PRECHARGE: Off state signals are incorrect.");
    Serial.println(HW_readMux(HW_MUX_nTS_ENERGIZED));
    Serial.println(HW_readMux(HW_MUX_TS_ENERGIZED));
    #endif
    
    shutdownCar();
    return false;
  }
  
  int startTime = millis();
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_ENERGIZED);
  while ((millis() - startTime) < PRECHARGE_DURATION) // Precharge takes about 3.5 seconds, tested in lab. 
  {                                     // Time can be reduced by placing another resistor in parallel.
    if(msgWaiting)                      // Wait for precharge to complete. Do nothing other than receive BMS messages. 
    {                                   
      CAN.receiveMsgs(1);               // TO DO: Update to correct node number (BMS node)
    }                                   
  }

  // Check precharge signals
  if ((HW_readMux(HW_MUX_nTS_ENERGIZED) > DIGITAL_LOGIC_THRESHOLD))
  {
    // FAILURE: system not detected to be energized
    #ifdef debug_ACM
    Serial.println("PRECHARGE - system not energized");
    #endif
    
    shutdownCar();
    return false;
  }
  if ((HW_readMux(HW_MUX_PRECHARGE_DONE) < DIGITAL_LOGIC_THRESHOLD))
  {
    // FAILURE: system not detected to be precharged
    #ifdef debug_ACM
    Serial.println("PRECHARGE - system not precharged");
    #endif
    
    shutdownCar();
    return false;
  }
  if ((HW_readMux(HW_MUX_PRECHARGE_CONDUCTING) > DIGITAL_LOGIC_THRESHOLD))
  {
    // FAILURE: precharge relay detected to be not conducting
    #ifdef debug_ACM
    Serial.println("PRECHARGE - precharge relay not conducting");
    #endif
    
    shutdownCar();
    return false;
  }

  // All good up until this point. Precharge is complete, activate main contactor
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_ENERGIZED);
  IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_ENERGIZED);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
  delay(100);
  if (!((IO_getRelayState(IO_RELAY_AIRP) == IO_RELAY_ENERGIZED) && 
        (IO_getRelayState(IO_RELAY_AIRP) == IO_RELAY_ENERGIZED) && 
        (IO_getRelayState(IO_RELAY_PRECHARGE) == IO_RELAY_SAFE)))
  {
    // FAILURE: incorrect relay states at end of precharge
    #ifdef debug_ACM
    Serial.println("Wrong relay states at end of precharge");
    #endif
    shutdownCar();
    return false;
  }

  // Success
  return true;
}

void shutdownCar(void) // This function disables the shutdown circuit and illuminates the AMS fault LED
{
  carState = CAR_STATE_SHUTDOWN;                // TO DO: make sure ACM needs to do nothing in this state
  HW_digitalWrite(HW_PIN_AMS_FAULT_EN, false);   
  HW_digitalWrite(HW_PIN_SDC_EN, false);        // Rear Controller (Mega) will sense this and cut off power to all other LV electronics
}

bool BMS_checkFaults()          
{
  uint8_t DTC_1 = CAN.getField(CAN_MSG_BMS_FAULTS, BMS_DTC_1_F);
  uint16_t DTC_2 = CAN.getField(CAN_MSG_BMS_FAULTS, BMS_DTC_2_F);
  
  if((DTC_1 > 0) || (DTC_2 > 0))      // TO DO: Will likely need modification (care about less faults)
  {                                   // Use lib_util GET_BITS() to extract specific fault codes we care about
    #ifdef debug_ACM
    Serial.print("DTC Status 1: "); Serial.println(DTC_1);
    Serial.print("DTC Status 2: "); Serial.println(DTC_2);
    #endif
    
    return false;
  }

  return true; 
}

/******************************************************************************
 *                         I S R   D E F I N I T I O N S                      *
 *****************************************************************************/

void CAN_msgWaitingISR()                  // Should only need to receive off the BMS bus 
{
  msgWaiting = true;                      // Rear controller bus only for sending logging data
}

void BMS_heartbeatISR()
{
  CAN.sendMsg(CAN_MSG_BMS_HEARTBEAT, 1);              // TO DO: Update to correct node number (BMS node)
  
  if((HW_readMux(HW_MUX_TS_ENERGIZED) < DIGITAL_LOGIC_THRESHOLD) ==       // Check TS pins while we're here
     (HW_readMux(HW_MUX_nTS_ENERGIZED) < DIGITAL_LOGIC_THRESHOLD))        // These two should always be inverse of each other
  { 

    TS_errorCount += 1;

    if (TS_errorCount >= TS_errorConfirm)
    {
      #ifdef debug_ACM
      Serial.print("nTS ="); Serial.println(HW_readMux(HW_MUX_nTS_ENERGIZED));
      Serial.print("TS = "); Serial.println(HW_readMux(HW_MUX_TS_ENERGIZED));
      Serial.println("TS == nTS. Shutting down.");
      #endif
      
      shutdownCar();
    }
  }  
  else
  {
    TS_errorCount = 0;
  }
}

/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/

void setup() {
  
  Serial.begin(9600);
  while(!Serial){}                        // Wait for serial port to initialize

  #ifdef debug_ACM
  Serial.println("ACM booting up");
  #endif

//------- Pin Setup ------------------
  HW_setupPins();

//------- Relay Self Test ------------  
  benchSelfTest();

//------- Setup CAN ------------------
  CAN.begin(CAN_500KBPS, 3);              // TO DO: Is this the right rate?
  CAN.setFilters(1);                      // TO DO: Update to correct node number (should be BMS node)
  CAN.setFilters_permitNone(2);           // TO DO: Update to correct node number (should be RC/Powertrain node)

//------- Setup Interrupts -----------
  Timer1.initialize(BMS_heartbeatEvery);
  Timer1.attachInterrupt(BMS_heartbeatISR);

  attachInterrupt(digitalPinToInterrupt(HW_pins[HW_PIN_CAN1_INT].pinNum),     // TO DO: Update to correct pin (should be BMS node)
                  CAN_msgWaitingISR, LOW);
}

void loop() {

  switch(carState)
  {
    
//===========================================================================  
    case CAR_STATE_INIT:
//===========================================================================  
//-------- Waiting for RTD_REQUEST to go high -------------------------------
      #ifdef debug_ACM
      Serial.println("Waiting for RTD req.");
      Serial.print("RTD_REQ reading: ");  Serial.println(HW_readMux(HW_MUX_RTD_REQUEST));
      #endif
      
      if(HW_readMux(HW_MUX_RTD_REQUEST) > DIGITAL_LOGIC_THRESHOLD)
      {
        RTD_reqCount += 1;

        if(RTD_reqCount >= RTD_reqConfirm)
        {
          #ifdef debug_ACM
          Serial.println("Attempting precharge.");
          #endif
          carState = CAR_STATE_PRECHARGE;
        }
      }
      else
      {
        RTD_reqCount = 0;
      }

      if(msgWaiting)                              // Receive incoming CAN messages
      {
        CAN.receiveMsgs(1);                       // TO DO: Update to correct node number (BMS node)
      }

      if(CAN.msgDirty(CAN_MSG_BMS_FAULTS))        // Check for BMS fault codes
      {
        if(!BMS_checkFaults())
        {
          #ifdef debug_ACM
          Serial.println("BMS fault. Shutting down.");
          #endif

          shutdownCar();
        }
      }
      
      break;
      
//=========================================================================== 
    case CAR_STATE_PRECHARGE:
//===========================================================================
//-------- Attempt pre-charge sequence --------------------------------------
      if(doPrecharge())
      {
        #ifdef debug_ACM
        Serial.println("Pre-charge success!");
        #endif

        HW_digitalWrite(HW_PIN_RTD_CONFIRM, true);    // Let rear controller know pre-charge succeeded
        delay(100);                                   // Allow inverter to transition to RUNNING state
        HW_digitalWrite(HW_PIN_RTD_BUZZER_EN, true);  // Let driver know pedals will respond now
        buzzerStart = millis();
        while((millis() - buzzerStart) < buzzerDelay)
        {
          if(msgWaiting)
          {
            CAN.receiveMsgs(1);               // TO DO: Update to correct node number (BMS node)
          }
        }
        HW_digitalWrite(HW_PIN_RTD_BUZZER_EN, false);
        
        carState = CAR_STATE_DRIVE;
      }
      else
      {
        #ifdef debug_ACM
        Serial.println("Pre-charge FAILED.");
        #endif
        
        shutdownCar();
      }

      break;

//=========================================================================== 
    case CAR_STATE_DRIVE:
//===========================================================================
      if(CAN.msgDirty(CAN_MSG_BMS_FAULTS))        // Check for BMS fault codes
      {
        if(!BMS_checkFaults())
        {
          #ifdef debug_ACM
          Serial.println("BMS fault. Shutting down.");
          #endif

          shutdownCar();
        }
      }
      
      if((millis() - boardTempPollLast) > boardTempPollEvery)  
      {
        if(!checkBoardTemps(CAN))                 // Check PCB temps
        {
          if(boardOvertempCount > boardOvertempConfirm)
          {
            #ifdef debug_ACM                      // TO DO: Store temps in CAN library
            Serial.println("PCB Temp too high. Shutting down.");
            #endif
            shutdownCar();
          }
          boardOvertempCount += 1;
        }
        else
        {
          boardOvertempCount = 0;
        }
        boardTempPollLast = millis();             // Reset timer
      }
      
      if(msgWaiting)
      {
        CAN.receiveMsgs(1);                       // TO DO: Update to correct node number (BMS node)
      }

      break;

//=========================================================================== 
    case CAR_STATE_SHUTDOWN:
//===========================================================================
      #ifdef debug_ACM
      Serial.println("ACM in shutdown state.");
      #endif
      shutdownCar();
      delay(1000);

      break;

  }
}
