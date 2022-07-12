/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_PIN_DRIVER_MICRO.h"
#include "UTFR_APPS.h"
#include "UTFR_RTD_MICRO.h"

#include <SoftwareSerial.h>


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define debug_RC_Micro              // Uncomment for debug serial prints


/******************************************************************************
 *              D A T A   D E F I N I T I O N S                               * 
 *****************************************************************************/
//--------- State Machine -------------
enum carState_E
{                             
  CAR_STATE_INIT = 0,
  CAR_STATE_DRIVE,
};

carState_E carState = CAR_STATE_INIT;

//------- Class Instantiation ---------
UTFR_RTD_MICRO RTD;
UTFR_APPS APPS;
SoftwareSerial megaSerial(Micro_HW_pins[HW_PIN_MEGA_MICRO_TX].pinNum,         // Port currently unused due to unreliability during interrupts
                          Micro_HW_pins[HW_PIN_MEGA_MICRO_RX].pinNum); 


/******************************************************************************
 *                         I S R   D E F I N I T I O N S                      *
 *****************************************************************************/


/******************************************************************************
 *                     F U N C T I O N S                                      *
 *****************************************************************************/
void toggleAPPS_ISR()                   // Enables or disables APPS output based on state of car (flips state)
{
  if (carState == CAR_STATE_INIT)
  {
    #ifdef debug_RC_Micro
    Serial.println("Micro transition to DRIVE state.");
    #endif
    
    carState = CAR_STATE_DRIVE;
  }
  if (carState == CAR_STATE_DRIVE)
  {
    carState = CAR_STATE_INIT;
  }
}

/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup() 
{

  Serial.begin(9600);
  while(!Serial){}                          // Wait for serial port to initialize
  #ifdef debug_RC_Micro
  Serial.println("============ Starting Micro. ==================");
  #endif
//------- Pin Setup -------------------
  HW_setupPins();
  
// -------- APPS -----------------------
  APPS.begin(Micro_HW_pins[HW_PIN_CS_APPS].pinNum);

// -------- Serial comms ---------------
  megaSerial.begin(9600);                   // Port currently unused due to unreliability during interrupts

//--------- Setup Interrupts -----------
  attachInterrupt(digitalPinToInterrupt(Micro_HW_pins[HW_PIN_MEGA_MICRO_2_DIGITAL].pinNum),
                  toggleAPPS_ISR, RISING);                     
}

void loop() {

  switch(carState)
  {
//===========================================================================  
    case CAR_STATE_INIT:
//===========================================================================  
// ------------ APPS outputs disabled, waiting for Mega input --------------- 
      RTD.confirmReady();               // Constantly checks brake and throttle inputs and updates a pin indicating this to the Mega 
      break;
//===========================================================================  
    case CAR_STATE_DRIVE:
//===========================================================================  
// ------------ APPS outputs enabled ----------------------------------------
      APPS.processThrottlePosition();   // Note: library alerts Mega of APPS implausibility errors, don't have to do that here
      delay(50); 
      break;  
  }
}
