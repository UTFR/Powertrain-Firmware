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

//--------- RTD Buzzer ----------------
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

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/
void benchSelfTest(void);   // TO DO: Implement version for use with HV
void shutdownCar(void);
bool doPrecharge(void);
bool BMS_checkFaults(void);

void CAN_msgWaitingISR(void);
void BMS_heartbeatISR(void);

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/
void shutdownCar(void) // This function disables the shutdown circuit and illuminates the AMS fault LED
{
  carState = CAR_STATE_SHUTDOWN;                // TO DO: What does ACM do in this state? 
  HW_digitalWrite(HW_PIN_AMS_FAULT_EN, true);
  HW_digitalWrite(HW_PIN_SDC_EN, false);
}

/* 
void benchSelfTest(void) // only to be used on the bench with NO high voltage present. The self-test logic will be different in the car.
{
  Serial.println("======== BEGINNING SELF TEST ========");
  // Enable SDC
  HW_digitalWrite(HW_PIN_SDC_EN, true);
  Serial.println("Shutdown circuit enabled");

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
    //IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_ENERGIZED);
  }

  if (airnState == IO_RELAY_SAFE)
  {
    //IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_ENERGIZED);
  }

  if (prechargeState == IO_RELAY_SAFE)
  {
    //IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_ENERGIZED);
  }

  // Get new states
  airpState = IO_getRelayState(IO_RELAY_AIRP);
  airnState = IO_getRelayState(IO_RELAY_AIRN);
  prechargeState = IO_getRelayState(IO_RELAY_PRECHARGE);

  // Respond to error states
  if (airpState == IO_RELAY_FAULT)
  {
    IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  }
  
  if (airnState == IO_RELAY_FAULT)
  {
    IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  }

  if (prechargeState == IO_RELAY_FAULT)
  {
    IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
  }
  
  Serial.println("=========== RELAY STATES ===========");
  printRelayState(IO_RELAY_AIRP, airpState);
  printRelayState(IO_RELAY_AIRN, airnState);
  printRelayState(IO_RELAY_PRECHARGE, prechargeState);

  if ((airpState == IO_RELAY_FAULT) | (airnState == IO_RELAY_FAULT) | (prechargeState == IO_RELAY_FAULT))
  {
    shutdownCar; // If any relay faults present, trip SDC and illuminate AMS fault LED
  }
} */

bool doPrecharge(void)
{
  if ((HW_readMux(HW_MUX_nTS_ENERGIZED) < 2.5) || (HW_readMux(HW_MUX_PRECHARGE_DONE) > 2.5) || (IO_getRelayState(IO_RELAY_PRECHARGE) != IO_RELAY_SAFE))
  {
    // FAILURE: off-state signals are incorrect
    shutdownCar();
    return false;
  }
  
  int startTime = millis();
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_ENERGIZED);
  while ((millis() - startTime) < 3500) // Precharge takes about 3.5 seconds, tested in lab. 
  {                                     // Time can be reduced by placing another resistor in parallel.
    if(msgWaiting)                      // Wait for precharge to complete. Do nothing other than receive BMS messages. 
    {                                   
      CAN.receiveMsgs(1);               // TO DO: Update to correct node number (BMS node)
    }                                   
  }

  // Check precharge signals
  if ((HW_readMux(HW_MUX_nTS_ENERGIZED) > 2.5))
  {
    // FAILURE: system not detected to be energized
    shutdownCar();
    return false;
  }
  if ((HW_readMux(HW_MUX_PRECHARGE_DONE) < 2.5))
  {
    // FAILURE: system not detected to be precharged
    shutdownCar();
    return false;
  }
  if ((HW_readMux(HW_MUX_PRECHARGE_CONDUCTING) > 2.5))
  {
    // FAILURE: precharge relay detected to be not conducting
    shutdownCar();
    return false;
  }

  // All good up until this point. Precharge is complete, activate main contactor
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_ENERGIZED);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
  delay(100);
  if (!((IO_getRelayState(IO_RELAY_AIRP) == IO_RELAY_ENERGIZED) && (IO_getRelayState(IO_RELAY_PRECHARGE) == IO_RELAY_SAFE)))
  {
    // FAILURE: incorrect relay states at end of precharge
    shutdownCar();
    return false;
  }

  // Success
  return true;
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
  CAN.sendMsg(CAN_MSG_BMS_HEARTBEAT, 1);            // TO DO: Update to correct node number (BMS node) 
  
  if( (HW_readMux(HW_MUX_nTS_ENERGIZED) < 2.5) ==   // Check TS pins while we're here
      (HW_readMux(HW_MUX_TS_ENERGIZED) < 2.5))      // These two should always be inverse of each other
  {
    #ifdef debug_ACM
    Serial.println("TS != nTS. Shutting down.");
    #endif
    
    shutdownCar();
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
  //benchSelfTest();                      // TO DO: Uncomment once ready for use with HV

//------- Setup CAN ------------------
  CAN.begin(CAN_500KBPS, 3);              // TO DO: Is this the right rate?
  CAN.setFilters(1);                      // TO DO: Update to correct node number (BMS node)
  CAN.setFilters_permitNone(2);           // TO DO: Update to correct node number (RC/Powertrain node)

//------- Setup Interrupts -----------
  Timer1.initialize(BMS_heartbeatEvery);
  Timer1.attachInterrupt(BMS_heartbeatISR);

  attachInterrupt(digitalPinToInterrupt(HW_pins[HW_PIN_CAN1_INT].pinNum),     // TO DO: Update to correct pin (BMS node)
                  CAN_msgWaitingISR, LOW);
}

void loop() {

  switch(carState)
  {
    
//===========================================================================  
    case CAR_STATE_INIT:
//===========================================================================  
//-------- Waiting for RTD_REQUEST to go high -------------------------------

      if(HW_readMux(HW_MUX_RTD_REQUEST) > 2.5)
      {
        #ifdef debug_ACM
        Serial.println("Attempting precharge.");
        #endif
        
        carState = CAR_STATE_PRECHARGE;
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
          Serial.println("BMS fault code detected. Shutting down.");
          #endif

          shutdownCar();
        }
      }
      
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
            CAN.receiveMsgs(1);                       // TO DO: Update to correct node number (BMS node)
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

//=========================================================================== 
    case CAR_STATE_DRIVE:
//===========================================================================

      if(CAN.msgDirty(CAN_MSG_BMS_FAULTS))        // Check for BMS fault codes
      {
        if(!BMS_checkFaults())
        {
          #ifdef debug_ACM
          Serial.println("BMS fault code detected. Shutting down.");
          #endif

          shutdownCar();
        }
      }
      
      if((millis() - boardTempPollLast) > boardTempPollEvery)  
      {
        if(!checkBoardTemps(CAN))                 // Check PCB temps
        {                                         // TO DO: Find broken thermistor
          if(boardOvertempCount > boardOvertempConfirm)
          {
            #ifdef debug_ACM                        // TO DO: Store temps in CAN library
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
  }
}
