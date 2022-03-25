/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SoftwareSerial.h>
#include "UTFR_APPS.h"


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define HW_PIN_MSCOM1 2
#define HW_PIN_MSCOM2 3
#define HW_PIN_MSCOM4 4
#define HW_PIN_MSCOM5 5
#define HW_PIN_MSCOM3 6

#define HW_PIN_DAC_CS 7

#define HW_PIN_MEGA_TX 10
#define HW_PIN_MEGA_RX 13



/******************************************************************************
 *         F U N C T I O N   D E C L A R A T I O N S                          *
 *****************************************************************************/
 

/******************************************************************************
 *              D A T A   D E F I N I T I O N S                               * 
 *****************************************************************************/
// ================== APPS
UTFR_APPS APPS;

// ================== Serial comms 
SoftwareSerial megaSerial(HW_PIN_MEGA_TX, HW_PIN_MEGA_RX);
char rx_byte;
bool interrupt_flag = false;
uint8_t interrupt_rx_pin = HW_PIN_MSCOM4;
uint8_t interrupt_tx_pin = HW_PIN_MSCOM3;



/******************************************************************************
 *                     F U N C T I O N S                                      *
 *****************************************************************************/

void SDC_ISR(void){
  interrupt_flag = true;
}


/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup() {

  Serial.begin(115200);

// ================== APPS
  APPS.begin(HW_PIN_DAC_CS);


// ================== Serial comms 
  megaSerial.begin(115200);
  pinMode(interrupt_rx_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_rx_pin), SDC_ISR, RISING);   // Recieving INT from Mega on MS_COM4

  pinMode(interrupt_tx_pin, OUTPUT);     // Send INT to Mega on MS_COM3

}

void loop() {

// ================== APPS ===========================================
  APPS.processThrottlePosition();
  
// ================== Serial comms ===================================
 
  megaSerial.print("Hello from da Micro \n");       // Send msg to Mega
  
  while (megaSerial.available() > 0)                // Receive serial msg from Mega
  {               
    rx_byte = megaSerial.read();
    Serial.print(rec);
  } 
  Serial.println();

  if (interrupt_flag)                               // Check for interrupt from mega
  {
    interrupt_flag = false;
    Serial.print("Interrupt received on MSCOM"); Serial.println(
  }
  
  digitalWrite(interrupt_tx_pin,HIGH);              // Send interrupt to Mega
  delayMicroseconds(10);
  digitalWrite(interrupt_tx_pin,LOW);

  delay(1000);
}
