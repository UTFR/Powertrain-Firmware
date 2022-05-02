/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_CAN_RC.h"
#include "UTFR_ERROR.h"
#include "UTFR_RTD_MEGA.h"
#include "UTFR_COOLING.h"
#include "UTFR_LVBATT.h"
//#include "UTFR_COMMS_MEGA.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define debug_RC_Mega             // Uncomment for debug prints

//------- Comms with Micro ---------
#define HW_PIN_MSCOM1 40          // Reserved for sending INTERRUPT to Micro when SDC trips
#define HW_PIN_MSCOM2 41          // Reserved for sending INTERRUPT to Micro (not sure in what case)
#define HW_PIN_MSCOM3 20          // Reserved for receiving INTERRUPT from Micro to confirm zero torque
                                  // OR to indicate APPS fault
                                  // OR to indicate RTD
                                  // Depending on carState
#define HW_PIN_MSCOM4 42          
#define HW_PIN_MSCOM5 43          

//------- CAN Nodes ----------------
#define HW_PIN_CAN_DAQ_INT 2      // For receiving INTERRUPTS from DAQ CAN node when message received 
#define HW_PIN_CAN_PWRTRN_INT 3   // For receiving INTERRUPTS from powertrain CAN node when message received 
#define HW_PIN_CAN_PWRTRN_CS 47   // Chip Select powertrain CAN node  
#define HW_PIN_CAN_DAQ_CS 48      // Chip Select DAQ CAN node

//------- Inverter -----------------
#define HW_PIN_INV_INT 18
#define HW_PIN_INV_IGNITION 38

//------- SDC Check ----------------
#define HW_PIN_SDC_INT 19

//------- LV Batt ------------------
#define HW_PIN_BMS_DRIVER 7


/******************************************************************************
 *                      D A T A   D E F I N I T I O N S                       *
 *****************************************************************************/
//--------- State Machine -------------
enum carState_E                         // TO DO: Implement sending states to Micro (some inferred from digital pins)
{                                       // i.e. Send state whenever it changes
  CAR_STATE_INIT = 0,
  CAR_STATE_DRIVE,
  CAR_STATE_ZERO_TORQUE,
  CAR_STATE_OFF,
};

carState_E carState = CAR_STATE_INIT;
 
//------- Class Instantiation ---------
UTFR_CAN_RC CAN(HW_PIN_CAN_DAQ_CS, HW_PIN_CAN_PWRTRN_CS);               // All caps for class instances
UTFR_ERROR ERRORS;
UTFR_RTD_MEGA RTD;                         
UTFR_COOLING COOLING;
UTFR_LVBATT LVBATT;
// UTFR_COMMS_MEGA COMMS_MEGA;

//---------- Inverter -----------------
bool invState = false;         // Important inverter faults bubble up here making this false. True when all good.
                               // TO DO: Inverter CAN msg checking. 
                                                      
//--------- Coolant Loop --------------
bool coolantTempOk = false;
bool coolantPressOk = false;
bool coolantFlowOk = false;

//-------- Low Voltage BMS ------------
bool battOk = false;

//----------- Comms -------------------
volatile bool microInterruptMega = false;    // set to true once micro has set MS_COM_3 high (indicating it wants Mega to do something)

//----------- Shutdown ----------------
bool sdcState = false;              // false when SDC open (car shutdown), true when SDC closed (normal operation) 

const uint8_t retryAfter = 25;      // Retry zero torque command after this many milliseconds 
const uint8_t retryUpTo = 4;        // Retry zero torque command this many times

unsigned long retryTimer;           // Keeps track of when last zero torque command was tried
uint8_t retryCount = 0;             // Tracks number of zero torque command retries (until it hits retryUpTo)


/******************************************************************************
 *                         I S R   D E F I N I T I O N S                      *
 *****************************************************************************/
void microInterruptMegaISR()                        // called once micro has set MS_COM_3 high (indicating it wants Mega to do something)
{
  if (carState == CAR_STATE_ZERO_TORQUE)       // if waiting for micro to confirm zero torque
  {
    microInterruptMega = true;
  }
  else                                              // if normal operation when pin goes high
  {
    carState = CAR_STATE_OFF;
  }
}

void sdcInterruptISR()
{
  carState = CAR_STATE_ZERO_TORQUE;
  sdcState = false;
}


/******************************************************************************
 *                   F U N C T I O N   D E F I N I T I O N S                  *
 *****************************************************************************/


/******************************************************************************
 *                   P R O G R A M   E X E C U T I O N                        *
 *****************************************************************************/

void setup() 
{
  Serial.begin(9600);
  while(!Serial){}                              // Wait until Serial port is ready                      

//------- Micro Serial ----------------
  Serial3.begin(9600);                          // TO DO: Move to UTFR_COMMS_MEGA.h
  
//------- CAN Setup ------------------- 
  CAN.begin(CAN_500KBPS, 3);

//------- Attach Interrupts -----------
  attachInterrupt(digitalPinToInterrupt(HW_PIN_MSCOM3), microInterruptMegaISR, RISING); // called once micro has set MS_COM_3 high 
                                                                                        // (indicating it wants Mega to do something)
  attachInterrupt(digitalPinToInterrupt(HW_PIN_SDC_INT), sdcInterruptISR, RISING);      // called once SDC is tripped/open (car shutdown)
}

void loop() 
{
//------- Ready To Drive Loop ---------     
  while(carState == CAR_STATE_INIT)                       // Returns true when RTD outputs are high (car is being told to start) 
  {
    sdcState = (digitalRead(HW_PIN_SDC_INT) == LOW);
    invState = (digitalRead(HW_PIN_INV_INT) == LOW);      // TO DO: logic here should check CAN messages as well as pin (disregard this pin even?)
    if (RTD.confirmReady(sdcState, invState, COOLING.checkCoolingLoop(CAN, ERRORS))
        && (digitalRead(HW_PIN_MSCOM3) == HIGH) )
    {
      carState = CAR_STATE_DRIVE;
    }
  }

//------ Main Drive Loop --------------
  while(carState == CAR_STATE_DRIVE)
  {
    if(!COOLING.checkCoolingLoop(CAN, ERRORS) || !LVBATT.checkLVBatt(CAN, ERRORS))   // Check state of cooling loop and LV battery       
    {
      carState = CAR_STATE_ZERO_TORQUE;
    }
    // TO DO: Check for messages from powertrain CAN node, act accordingly
  }

//------ Zero Torque Sequence ---------
  while (carState == CAR_STATE_ZERO_TORQUE)
  {
    digitalWrite(HW_PIN_MSCOM1, LOW);
    digitalWrite(HW_PIN_MSCOM1, HIGH);                    // Triggers interrupt on Micro
    
    retryTimer = millis();
    while (retryCount < retryUpTo)                        // Try sending zero torque retryUpTo times 
    {
      if (microInterruptMega)                             // If micro responds, continue with shutdown
      {
        break;
      }
      if ((millis() - retryTimer) > retryAfter)
      {
        digitalWrite(HW_PIN_MSCOM1, LOW);
        digitalWrite(HW_PIN_MSCOM1, HIGH);                // Retry triggering interrupt on Micro
        retryTimer = millis();
        retryCount += 1;
      }
    }
    carState = CAR_STATE_OFF;
  }

//------ Car Off Loop ----------------
  while(carState == CAR_STATE_OFF)                // Car state can only change in an ISR now (not implemented yet)
  { 
    digitalWrite(HW_PIN_INV_IGNITION, LOW);       // Tell inverter to shutdown
                                                  // TO DO: Should there be a delay here? Should we be checking inverter CAN messages?
    digitalWrite(HW_PIN_BMS_DRIVER, LOW);         // LV Battery Mosfet -> turn off most LV systems
    
    #ifdef debug_RC_Mega
    Serial.print("Car off. \n");
    #endif
    
    delay(100);
  }
}
