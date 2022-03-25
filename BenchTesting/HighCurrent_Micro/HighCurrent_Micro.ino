/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_APPS_DAC.h"
#include <SoftwareSerial.h>


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
 //Input pins
 #define HW_PIN_MS_COM_1 2

 //Output pins
 #define HW_PIN_DAC_CS 7

 // Constants
 #define SERIAL3_BUFFER_SIZE 4


 /******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


 
/******************************************************************************
 *              F U N C T I O N   D E C L A R A T I O N S                     *
 *****************************************************************************/
void SDC_ISR(void);

 /******************************************************************************
 *              D A T A   D E F I N I T I O N S                               *
 *****************************************************************************/
SoftwareSerial megaSerial(10, 13);

UTFR_APPS_DAC APPS;

// MEGA-MICRO COMMS
char serial3_buffer[SERIAL3_BUFFER_SIZE];
bool success;


 /******************************************************************************
 *              F U N C T I O N   D E F I N I T I O N S                       *
 *****************************************************************************/
// SDC
void SDC_ISR(){
  APPS.shutDown();       // writes 0 torque to inverter and sets _shutdown flag to true
}

// Serial Comms
void readSerial3(){
  megaSerial.readBytesUntil('\n', serial3_buffer, SERIAL3_BUFFER_SIZE);
  Serial.print("Mega message: "); Serial.println(serial3_buffer);
}

bool heartbeatSerial3(){
  megaSerial.println("boop");
  return true;
}

  /*****************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/

void setup() {

   /*************************************************
   *            PIN  SETUP                         *
   *************************************************/
   // SDC
   pinMode(HW_PIN_MS_COM_1, INPUT);
   attachInterrupt(digitalPinToInterrupt(HW_PIN_MS_COM_1), SDC_ISR, RISING);

   // APPS
   pinMode(HW_PIN_DAC_CS, OUTPUT);

     /*************************************************
   *          OBJECT  SETUP                        *
   *************************************************/
  // Micro-PC serial port
  Serial.begin(9600);

  // Micro-Mega serial port
  megaSerial.begin(9600);

  // APPS
  APPS.begin(HW_PIN_DAC_CS);

}

void loop() {

  // Shutdown check
  if (APPS.getShutdownState() == true){      // if shutdown() has been called
    if (APPS.confirmShutdown() == true){     // if 0 torque has been commanded successfully
      megaSerial.println("SDC1");           // SDC1 indicates success
      Serial.println("================================== Shutdown successful. ===============================================");
      while(1){}                          
    }
    else {
      APPS.shutDown();
      megaSerial.println("SDC0");           // SDC0 indicates failure
    }
  }

  // APPS output
  APPS.processThrottlePosition();

  // Micro-Mega Serial Comms
  if (megaSerial.available() > 0){
    readSerial3();
    if (serial3_buffer == "beep"){
      Serial.println("Sending heartbeat response.");
      success = heartbeatSerial3();
    }
  }
  
}
