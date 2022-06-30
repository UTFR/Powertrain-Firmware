// IMPORTANT NOTE - USE THE ATMEGA328P (OLD PROGRAMMER)
/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_HW_ACM.h"
#include "UTFR_IO_THERMISTORS_ACM.h"
#include "UTFR_IO_RELAYS_ACM.h"
//#include "UTFR_CAN_ACM.h"

#include <TimerOne.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define debug_ACM             // Uncomment for debug prints

#define CHARGE_DURATION 3000                // milliseconds to charge for
#define PRECHARGE_DURATION 5000             // milliseconds to wait for precharge
#define DIGITAL_LOGIC_THRESHOLD 2.5         // > == logic HIGH, < == logic LOW

/******************************************************************************
 *                      D A T A   D E F I N I T I O N S                       *
 *****************************************************************************/
//--------- State Machine -------------
enum carState_E
{                                       
  CAR_STATE_CHARGE = 0,
  CAR_STATE_SHUTDOWN,
};
carState_E carState = CAR_STATE_CHARGE;
                  
//--------- Charging? -----------------
bool charging = false;
unsigned long chargingStart = 0;

//--------- Thermals ------------------
const int boardTempPollEvery = 2000;            // Check PCB temps every 'boardTempPollEvery' milliseconds
unsigned long boardTempPollLast = 0;            // Marks last time board temps were polled

const uint8_t boardOvertempConfirm = 10;       
uint8_t boardOvertempCount = 0;                 // Counts up to 'boardOvertempConfirm' then shuts down car

//--------- Isolation -----------------
const uint16_t isoCheckEvery = 200000;     // Microseconds between checking TS != nTS 

//--------- Tractive System -----------
const uint8_t TS_errorConfirm = 10;              
uint8_t TS_errorCount = 0;                      // Counts up to TS_errorConfirm (each TS==nTS) then shuts down car

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/
void benchSelfTest(void);
void shutdownCar(void);
bool doPrecharge(void);
void stopCharging(void);

void isoCheckISR(void);

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/
void shutdownCar(void) // This function disables the shutdown circuit and illuminates the AMS fault LED
{
  carState = CAR_STATE_SHUTDOWN;                // TO DO: make sure ACM needs to do nothing in this state

  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);
    
  HW_digitalWrite(HW_PIN_AMS_FAULT_EN, true);   
  HW_digitalWrite(HW_PIN_SDC_EN, false);        // Rear Controller (Mega) will sense this and cut off power to all other LV electronics
}

void benchSelfTest(void) // only to be used on the bench with NO high voltage present. The self-test logic will be different in the car.
{
  #ifdef debug_ACM
  Serial.println("======== BEGINNING SELF TEST ========");
  #endif

  // Check for relay faults
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);

  delay(100);
  
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

  delay(1000);
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
    Serial.print("nTS: "); Serial.println(HW_readMux(HW_MUX_nTS_ENERGIZED));
    Serial.print("TS: "); Serial.println(HW_readMux(HW_MUX_TS_ENERGIZED));
    #endif
    
    shutdownCar();
    return false;
  }
  
  int startTime = millis();
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_ENERGIZED);
  while ((millis() - startTime) < PRECHARGE_DURATION) // Precharge takes about 3.5 seconds, tested in lab. 
  {                                     // Time can be reduced by placing another resistor in parallel.                               
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

  // Activate main contactor
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

void stopCharging(void)
{
  IO_setRelayState(IO_RELAY_AIRP, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_AIRN, IO_RELAY_SAFE);
  IO_setRelayState(IO_RELAY_PRECHARGE, IO_RELAY_SAFE);    // Why not
}

/******************************************************************************
 *                         I S R   D E F I N I T I O N S                      *
 *****************************************************************************/
/*
void CAN_msgWaitingISR()                  // Should only need to receive off the BMS bus 
{
  msgWaiting = true;                      // Rear controller bus only for sending logging data
}*/

void isoCheckISR()
{
  //CAN.sendMsg(CAN_MSG_BMS_HEARTBEAT, 1);              // TO DO: Update to correct node number (BMS node)
  
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

/*   REMOVE : CAN disabled for now
//------- Setup CAN ------------------
  CAN.begin(CAN_500KBPS, 3);              // TO DO: Is this the right rate?
  CAN.setFilters(1);                      // TO DO: Update to correct node number (should be BMS node)
  CAN.setFilters_permitNone(2);           // TO DO: Update to correct node number (should be RC/Powertrain node)
*/
//-------- Setup Interrupts -----------
  Timer1.initialize(isoCheckEvery);
  Timer1.attachInterrupt(isoCheckISR);

  //attachInterrupt(digitalPinToInterrupt(HW_pins[HW_PIN_CAN1_INT].pinNum),     // TO DO: Update to correct pin (should be BMS node)
  //                CAN_msgWaitingISR, LOW);

//-------- SDC ------------------------
  HW_digitalWrite(HW_PIN_SDC_EN, true);
}

void loop() {

  switch(carState)
  {   
//=========================================================================== 
    case CAR_STATE_CHARGE:
//===========================================================================
//-------- Attempt pre-charge sequence --------------------------------------
      if(!charging)
      {
        if(doPrecharge())
        {
          charging = true;
          chargingStart = millis();
          
          #ifdef debug_ACM
          Serial.println("!!!!!!!!!!! Charging started !!!!!!!!!!!!!");
          #endif
        }
        else
        {
          #ifdef debug_ACM
          Serial.println("Charging start FAILED.");
          #endif
          
          shutdownCar();
        }
      }
      else        // if charging
      {
        if (millis() - chargingStart > CHARGE_DURATION)   // if time elapsed since charging began > CHARGE_DURATION, stop charging
        {
          stopCharging();
        }
      }
      
      /*
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
      }*/

      break;

//=========================================================================== 
    case CAR_STATE_SHUTDOWN:
//===========================================================================
      #ifdef debug_ACM
      Serial.println("ACM in shutdown state. Charging stopped.");
      #endif
      shutdownCar();
      delay(1000);

      break;

  }
}
