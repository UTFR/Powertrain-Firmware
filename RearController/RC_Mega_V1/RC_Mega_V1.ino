/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_PIN_DRIVER_MEGA.h"
#include "UTFR_CAN_RC.h"
#include "UTFR_ERROR.h"
#include "UTFR_COOLING.h"
#include "UTFR_LVBATT.h"

#include <TimerOne.h>         
#include <TimerThree.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define debug_RC_Mega             // Uncomment for debug prints


/******************************************************************************
 *                      D A T A   D E F I N I T I O N S                       *
 *****************************************************************************/
//--------- State Machines -------------
enum carState_E
{                                       
  CAR_STATE_INIT = 0,
  CAR_STATE_IGNITION,
  CAR_STATE_PRECHARGE,
  CAR_STATE_DRIVE,
  CAR_STATE_SHUTDOWN,
};
volatile carState_E carState = CAR_STATE_INIT;

enum invState_E                     // From the Cascadia Motion Software User Manual
{                                    
  INV_STATE_START = 0,              // State 0 is completed automatically (Power-up, load EEPROM, POST)
  INV_STATE_WAIT = 4,               // States 1-3 skipped via EEPROM settings (since ACM controls precharge)                
  INV_STATE_RUNNING = 6,            // State 5 is short and completed automatically after drive signals given
  INV_STATE_FAULT = 7                 
};
invState_E invState = INV_STATE_START; 

errorNames_E errState = ERR_NONE;   // Gets updated when errors happen 
                                    // --> Mainly so correct action can be taken in shutdown sequence
                                    
//------- Class Instantiation ---------
UTFR_CAN_RC CAN(Mega_HW_pins[HW_PIN_CS_CAN_1].pinNum,       // Node 1 is on the powertrain CANbus
                Mega_HW_pins[HW_PIN_CS_CAN_2].pinNum);      // Node 2 is on the DAQ CANbus      
UTFR_ERROR ERRORS;                      
UTFR_COOLING COOLING;
UTFR_LVBATT LVBATT;

//--------- Ignition ------------------
const int preIgnDelay = 1000;        // How long to check cooling loop before ignition (ms)
unsigned long preIgnStart;           

//--------- Precharge -----------------
const int prechargeDelay = 10000;    // How long to wait for ACM to confirm precharge (ms)
unsigned long prechargeStart;   
const int microRTDDelay = 10000;     // How long to wait for pedal inputs to be confirmed by Micro (ms) 
unsigned long microRTDStart;
                           
//--------- CAN -----------------------
const int CANPollRate = 20000;      // Check powertrain CAN node for new messages every 'CANPollRate' microseconds (10^-6)

//-------- Low Voltage BMS ------------
bool battOk = false;

//--------- Mega-Micro Comms ----------
volatile bool microInterruptMega = false;    // set to true once micro has set MS_COM_3 high (indicating it wants Mega to do something)

//----------- Shutdown ----------------
volatile bool sdcState = false;             // False when SDC tripped (car shutdown), true when SDC complete (normal operation)
                                            // Updated periodically with TimerOne calling routine which reads a SDC check pin
const unsigned long sdcPollRate = 200000;   // Check SDC every x microseconds (0.2s currently)

const int invShutdownDelay = 5000;          // How long to wait for inverter to shut itself off before continuing with shutdown
unsigned long invShutdownStart;             // Beginning of above wait period         


/******************************************************************************
 *                         I S R   D E F I N I T I O N S                      *
 *****************************************************************************/
void ignitionSwitchISR()
{
  if (carState == CAR_STATE_INIT)             // Should only enter ignition mode from initial state
  {                                           // Driver could bump ignition button accidentally at any time
    carState = CAR_STATE_IGNITION;
  }
}

void microInterruptMegaISR()                  // called once micro has set MS_COM_3 high (indicating it wants Mega to do something)
{                                             // Currently only for indicating APPS failure to Mega. Simply puts the car in shutdown state.
  shutdownSequence();
}

void sdcCheckISR()
{                                             // TO DO: Change to the right pin for shunt resistor sensing
  sdcState = (HW_digitalRead(HW_PIN_SDC_CURRENT_ANALOG) == HIGH);       
}

void CANRxISR()                               // Receive messages off powertrain CANBus if they're waiting
{
  CAN.receiveMsgs(1); 
}


/******************************************************************************
 *                   F U N C T I O N   D E F I N I T I O N S                  *
 *****************************************************************************/
void shutdownSequence(void)                             // This is a function because we need to enter it from ISRs sometimes
{
  carState = CAR_STATE_SHUTDOWN;                       
  
  HW_digitalWrite(HW_PIN_FORWARD_ENABLE_DIGITAL, LOW);  // Disable APPS inputs at inverter (command zero torque)
  HW_digitalWrite(HW_PIN_IGNITION_DIGITAL, LOW);        // Tell inverter to shut itself off when it's ready

  invShutdownStart = millis();
  while(!HW_digitalRead(HW_PIN_INV_OK_DIGITAL) ||      // Violating both of these conditions indicates inverter has finished self-shutdown
         HW_digitalRead(HW_PIN_IGNITION_LED_DIGITAL))     
  {
    if((millis() - invShutdownStart) > invShutdownDelay)
    {
      #ifdef debug_RC_Mega                              // TO DO: Add error msg for this case
      Serial.println("Inverter self-shutdown not completed.");
      #endif
      break;
    }
    #ifdef debug_RC_Mega
    Serial.println("Waiting for inverter to finish self-shutdown.");
    #endif
  }
                                                       // TO DO: Send errState to inverter
  HW_digitalWrite(HW_PIN_MEGA_MICRO_2_DIGITAL, LOW);   // Set low as redundant measure (should already be low)
  HW_digitalWrite(HW_PIN_MEGA_MICRO_2_DIGITAL, HIGH);  // MICRO Trigger interrupt on Micro to toggle off APPS outputs
  delay(10);
  HW_digitalWrite(HW_PIN_BMS_MOS_DIGITAL, LOW);        // Un-ground (turn off) all LV electronics except Mega

  while(1)
  {
    Serial.println("Car off. Must power cycle to exit this state.");  // TO DO: Make this state exitable in some cases
    delay(1000);                                                      //         --> Set carState and break to do so
  }                                                                   
}


/******************************************************************************
 *                   P R O G R A M   E X E C U T I O N                        *
 *****************************************************************************/

void setup() 
{
  Serial.begin(9600);
  while(!Serial){}                              // Wait until Serial port is initialized                     

//------- Pin Setup -------------------
  HW_setupPins();

//------- Mega-Micro Serial -----------
  Serial3.begin(9600);                          // Try to avoid using this (unreliable in face of interrupts)
  
//------- CAN Setup ------------------- 
  CAN.begin(CAN_500KBPS, 3);                    // TO DO: Is this the right rate?
  CAN.setFilters(1);                            // Only accepting inverter internal state message off powertrain CANbus (for now)
                                                // TO DO: add other inverter messages we want to monitor
  CAN.setFilters_permitNone(2);                 // Don't need to receive any messages on DAQ CANbus (for now)
  Timer3.initialize(CANPollRate);               // Check for received messages in powertrain CANbus every 'CANPollRate' microseconds
  Timer3.attachInterrupt(CANRxISR);

//------- SDC Check Timer -------------
  Timer1.initialize(sdcPollRate);               // Check SDC every 'sdcPollRate' microseconds (10^-6)
  Timer1.attachInterrupt(sdcCheckISR);

//------- Attach Interrupts -----------
  attachInterrupt(digitalPinToInterrupt(Mega_HW_pins[HW_PIN_IGNITION_SWITCH_INT].pinNum),
                                        ignitionSwitchISR, RISING);                           // called when driver presses ignition button                                       
  attachInterrupt(digitalPinToInterrupt(Mega_HW_pins[HW_PIN_MEGA_MICRO_3_DIGITAL].pinNum),
                                        microInterruptMegaISR, RISING);                       // called once micro has set MS_COM_3 high 
                                                                                              // (only job is indicating APPS failure for now)                                                                                            
}

void loop() 
{

  switch(carState)
  { 
    
//===========================================================================     
    case CAR_STATE_INIT:
//=========================================================================== 
//-------- Waiting for driver to press ignition -----------------------------  
    
      if(HW_digitalRead(HW_PIN_IGNITION_SWITCH_INT) == true)  // Redundantly monitored by interrupt routine
      {
        carState = CAR_STATE_IGNITION;
      }
      

//==========================================================================    
    case CAR_STATE_IGNITION:
//==========================================================================
//------- Start pumps, check cooling loop, inverter ignition ---------------
    
      HW_digitalWrite(HW_PIN_PUMP_DIGITAL, HIGH);           // Turn on pumps
      delay(500);                                           // Delay so pressures and flow rate stabilize
  
      preIgnStart = millis();                               // Make sure cooling loop is in safe state
      while((millis() - preIgnStart) < preIgnDelay)         // Must check multiple times for unsafe state to be confirmed if it exists
      {                 
        if(!COOLING.checkCoolingLoop(CAN, ERRORS))          // Note: Error msg sending done in library
        {
          carState = CAR_STATE_INIT;
          break;
        }
        if(!LVBATT.checkLVBatt(CAN, ERRORS))                // If LVBatt error, car should be shutdown
        {
          carState = CAR_STATE_SHUTDOWN;                    // Note: Error msg sending done in library
          break;
        }
      }
      
      if(carState == CAR_STATE_INIT)                        // If cooling loop unsafe, go back to waiting for ignition switch press from driver                 
      {                                                     // Fans and pumps are on now so cooling loop should be cooling off
        #ifdef debug_RC_Mega
        Serial.println("Pre-ignition cooling loop check failed. Waiting for ignition button press.");
        #endif                                              
        break;                                              // Break out of case block to go to desired state
      }

      if(carState == CAR_STATE_SHUTDOWN)
      {
        #ifdef debug_RC_Mega
        Serial.println("Pre-ignition LVBatt check failed. Car off.");
        #endif                                              
        break;
      }
  
      while(!HW_digitalRead(HW_PIN_IGNITION_LED_DIGITAL) ||     // While inverter is not getting 12V
             HW_digitalRead(HW_PIN_INV_OK_DIGITAL))             // OR inverter has not closed the secondary ignition relay
      {
        HW_digitalWrite(HW_PIN_IGNITION_DIGITAL, HIGH);         // Inverter ignition command
        delay(10);
        #ifdef debug_RC_Mega
        Serial.println("Waiting for inverter ignition.");
        #endif
      }
      
      carState = CAR_STATE_PRECHARGE;


//==========================================================================
    case CAR_STATE_PRECHARGE:
//==========================================================================
//------ Precharge Sequence ------------------------------------------------

      int invState_read = -69; 
      
      while(invState_read != INV_STATE_WAIT)              // Wait for inverter to be in the wait state (wait state == ready for precharge)
      {
        if(CAN.msgDirty(CAN_MSG_INV_INTERNAL_STATE))      // If we received inverter internal state message
        {
          invState_read = CAN.getField(CAN_MSG_INV_INTERNAL_STATE, INV_VSM_STATE_F);    // Get inverter state and set variable for loop condition 
          if (invState_read == INV_STATE_FAULT)           // If fault state, shutdown car
          {
            #ifdef debug_RC_Mega
            Serial.print("PRECHARGE  Want: Inverter state 4. Current state: "); 
            Serial.println(invState_read);
            #endif
                                                          // TO DO: Send inverter fault error msg to datalogger
            carState = CAR_STATE_SHUTDOWN;
            break;
          }
        }
        
        #ifdef debug_RC_Mega
        Serial.print("PRECHARGE  Want: Inverter state 4. Current state: "); 
        Serial.println(invState_read);
        #endif
      }

      if(carState == CAR_STATE_SHUTDOWN)
      {
        break;                                            // if inverter fault, don't attempt precharge (break out of switch block)
      }

      HW_digitalWrite(HW_PIN_READY_TO_DRIVE_OUT, HIGH);   // Tell ACM to start precharge
      delay(100);                                         // Delay to let ACM set READY_TO_DRIVE_IN low (if it wasn't already)

      prechargeStart = millis();
      while(!HW_digitalRead(HW_PIN_READY_TO_DRIVE_IN))    
      {
        if((millis() - prechargeStart) > prechargeDelay)  // If too much time has passed, conclude precharge failed
        {
          #ifdef debug_RC_Mega
          Serial.println("Precharge failed. ACM did not respond.");
          #endif
                                                          // TO DO: Send precharge failed error msg to datalogger
          carState = CAR_STATE_SHUTDOWN;
          break;
        }
      }

      if(carState == CAR_STATE_SHUTDOWN)
      {
        break;                                            // if precharge fail, shutdown car (break out of switch block)
      }

      microRTDStart = millis();
      while(!HW_digitalRead(HW_PIN_MEGA_MICRO_5_DIGITAL)) // Check pedal positions on Micro by reading this pin 
      {                                                   // HIGH == brake pressed, no throttle
        if((millis() - microRTDStart) > microRTDDelay)
        {
          #ifdef debug_RC_Mega
          Serial.println("RTD pedal check failed. Micro did not respond. Try ignition again.");
          #endif
                                                          // TO DO: Send pedal check failed error msg to datalogger
          carState = CAR_STATE_INIT;
          break;
        }
      }

      if(carState == CAR_STATE_INIT)
      {
        break;                     // if RTD pedal check failed, shutdown car (break out of switch block)
      }

      HW_digitalWrite(HW_PIN_FORWARD_ENABLE_DIGITAL, HIGH);   // Tell inverter to enter running state by setting these 2 pins
      HW_digitalWrite(HW_PIN_BRAKE_DIGITAL, HIGH);

      while(invState_read != INV_STATE_RUNNING)               // Wait for inverter to be in the RUNNING state
      {
        if(CAN.msgDirty(CAN_MSG_INV_INTERNAL_STATE))          // If we received inverter internal state message
        {
          invState_read = CAN.getField(CAN_MSG_INV_INTERNAL_STATE, INV_VSM_STATE_F);    // Get inverter state and set loop condition variable 
          if (invState_read == INV_STATE_FAULT)               // If fault state, shutdown car
          {
            #ifdef debug_RC_Mega
            Serial.print("PRECHARGE  Want: Inverter state 6. Current state: "); 
            Serial.println(invState_read);
            #endif
                                                              // TO DO: Send inverter fault error msg to datalogger
            carState = CAR_STATE_SHUTDOWN;
            break;
          }
        }
        
        #ifdef debug_RC_Mega
        Serial.print("PRECHARGE  Want: Inverter state 6. Current state: "); 
        Serial.println(invState_read);
        #endif
      }

      if(carState == CAR_STATE_SHUTDOWN)
      {
        break;                     // if inverter doesn't transition to RUNNING state successfully, shutdown car (break out of switch block)
      }
  
      HW_digitalWrite(HW_PIN_MEGA_MICRO_2_DIGITAL, HIGH);   // MICRO Tell micro to enable APPS outputs
      delay(100);
      HW_digitalWrite(HW_PIN_MEGA_MICRO_2_DIGITAL, LOW);    // Reset this pin so we can use it to toggle APPS outputs off later

  
//------ Main Drive Loop --------------
    case CAR_STATE_DRIVE:
      
      if(!COOLING.checkCoolingLoop(CAN, ERRORS))            // Check state of cooling loop      
      {
        #ifdef debug_RC_Mega
        Serial.println("Cooling loop check failed.");
        #endif
        
        carState = CAR_STATE_SHUTDOWN;
        break;                                              // Note: Error msg sending done in library
      }
        
      if(!LVBATT.checkLVBatt(CAN, ERRORS))
      { 
        #ifdef debug_RC_Mega
        Serial.println("LVBatt check failed.");
        #endif
        
        carState = CAR_STATE_SHUTDOWN;                      // Note: Error msg sending done in library
        break;
      }

      if(CAN.msgDirty(CAN_MSG_INV_INTERNAL_STATE))          // Check for inverter fault if we received inverter internal state message
      {
        invState_read = CAN.getField(CAN_MSG_INV_INTERNAL_STATE, INV_VSM_STATE_F);    // Get inverter state and set loop condition variable 
        if (invState_read == INV_STATE_FAULT)               // If fault state, shutdown car
        {
          #ifdef debug_RC_Mega
          Serial.println("Inverter fault. Shutting down car."); 
          #endif
                                                            // TO DO: Send inverter fault error msg to datalogger
          carState = CAR_STATE_SHUTDOWN;
          break;
        }
      }                                                     // TO DO: Add checks for other inverter message fields 
                                                            // --> Copy 'if(CAN.msgDirty(CAN_MSG_INV_INTERNAL_STATE))' block for other msg

      
      // TO DO: Send powertrain sensor data & relevant inverter data to datalogger
  

//------ Shutdown Sequence ------------
    case CAR_STATE_SHUTDOWN:
      shutdownSequence();
  }
}
