/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SoftwareSerial.h>
#include "UTFR_APPS.h"
#include "UTFR_RTD_MICRO.h"


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
// TO DO: Replace all of these with pin driver!

#define HW_PIN_MSCOM1 2     // Receive Zero Torque interrupt from Mega on this pin
#define HW_PIN_MSCOM2 3     // Receive other interrupt from Mega on this pin
#define HW_PIN_MSCOM4 4
#define HW_PIN_MSCOM5 5
#define HW_PIN_MSCOM3 6     // Send zero torque confirmation 
                            // OR tell Mega of APPS fault on this pin
                            // OR send RTD signal on this pin
                            // Depending on carState

#define HW_PIN_THROTTLE_OUT A5

#define HW_PIN_DAC_CS 7

#define HW_PIN_MEGA_TX 10
#define HW_PIN_MEGA_RX 13



/******************************************************************************
 *         F U N C T I O N   D E C L A R A T I O N S                          *
 *****************************************************************************/
 

/******************************************************************************
 *              D A T A   D E F I N I T I O N S                               * 
 *****************************************************************************/
//--------- State Machine -------------
enum carState_E                         // TO DO: Implement getting states from Mega (some inferred from digital pins)
{                                       // i.e. Send state whenever it changes
  CAR_STATE_INIT = 0,
  CAR_STATE_DRIVE,
  CAR_STATE_ZERO_TORQUE,
  CAR_STATE_OFF,
};

carState_E carState = CAR_STATE_INIT;

//------- Class Instantiation ---------
UTFR_RTD_MICRO RTD;
UTFR_APPS APPS;
SoftwareSerial megaSerial(HW_PIN_MEGA_TX, HW_PIN_MEGA_RX);      // Currently unused


/******************************************************************************
 *                         I S R   D E F I N I T I O N S                      *
 *****************************************************************************/
void sendZeroTorqueISR()
{
  carState = CAR_STATE_ZERO_TORQUE;
}


/******************************************************************************
 *                     F U N C T I O N S                                      *
 *****************************************************************************/


/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup() {

  Serial.begin(9600);
  while(!Serial){}
  
// -------- APPS -----------------------
  APPS.begin(HW_PIN_DAC_CS);

// -------- Serial comms ---------------
  megaSerial.begin(9600);

//--------- Setup Interrupts -----------
  attachInterrupt(digitalPinToInterrupt(HW_PIN_MSCOM1), sendZeroTorqueISR, RISING);   // Recieving INT from Mega on MS_COM4
  pinMode(HW_PIN_MSCOM3, OUTPUT);     // Send INT to Mega on MS_COM3
}

void loop() {
//--------- Ready to Drive Loop --------
  while(carState == CAR_STATE_INIT)
  {
     RTD.confirmReady(analogRead(HW_PIN_THROTTLE_OUT));
  }

//--------- Main Drive Loop ------------
  while(carState == CAR_STATE_DRIVE)
  {
    APPS.processThrottlePosition();
  }

}
