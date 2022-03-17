/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/

#include "MCP_DAC.h"


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/
bool SDC_ISR();



/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/


MCP4911 DAC;                  // Digital-to-Analog converter -> this sends drive signals to the inverter which, in turn, powers the motor.

const int kCS_DAC = 8;
const int kSDC_INT = 2;

bool SDC_TRIPPED = false;     // if this is true, do nothing until 0 torque to inverter

bool written = false;
int readback = -1;


/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/

bool SDC_ISR(){
  DAC.analogWrite(0, 0);            // Write 0 throttle to DAC
  readback = analogRead(A5); 
  SDC_TRIPPED = true;
}

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

void setup() {

  Serial.begin(115200);
  apps.begin(kCS_DAC);
  
  attachInterrupt(digitalPinToInterrupt(kSDC_INT), SDC_ISR, mode);

}

void loop() {

 
  if (readback != 0){
       SDC_ISR()
    }
  
  if (SDC_TRIPPED) {
    if(apps.confirmShutdown() == true){
      Serial.println("SDC1");
    } else{
      apps.shutDown()
      Serial.println("SDC0");
    }
    
    // TO DO: while readback != 0, try to send 0 torque to inverter
    // TO DO: if timeout, send failure message to Mega - Mega alerts other systems
    // TO DO: if success, send success message to Mega

    // call shutdown function
    
  }

}
