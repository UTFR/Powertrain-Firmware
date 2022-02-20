/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SPI.h>

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define ANALOG_CONSTANT 1023           // 10-bit ADC resolution
#define MEASURED_ANALOG_ERROR 0.01     // Determine experimentally

#define SA_MASK 0b001
#define SB_MASK 0b010
#define SC_MASK 0b100

#define HW_PIN_MUX_SA 35        // TO DO: replace with pin abstraction
#define HW_PIN_MUX_SB 34
#define HW_PIN_MUX_SC 36
#define HW_PIN_BMSVOLT A12


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
typedef enum                  // TO DO: move to header file
{
  HW_MUX_BMS_A6 = 0,
  HW_MUX_BMS_A7,
  HW_MUX_BMS_A8,
  HW_MUX_BMS_A5,
  HW_MUX_BMS_A4,
  HW_MUX_BMS_A1,
  HW_MUX_BMS_A3,
  HW_MUX_BMS_A2,
  
  HW_MUX_COUNT
} HW_MUX_E;


/******************************************************************************
 *         F O R W A R D   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/
float HW_readMux(HW_MUX_E muxPin);    // TO DO: Move to header file


/******************************************************************************
 *                  D A T A   D E F I N I T I O N S                           *
 *****************************************************************************/
float readVoltage = 0.0;


/******************************************************************************
 *              F U N C T I O N S  D E F I N I T I O N S                      *
 *****************************************************************************/
float HW_readMux(HW_MUX_E muxPin) // Returns the voltage read for a given mux pin, from 0-5V
{
  bool SA_state = (muxPin & SA_MASK);
  bool SB_state = (muxPin & SB_MASK) >> 1;
  bool SC_state = (muxPin & SC_MASK) >> 2;

  // Set the mux select pins
  digitalWrite(HW_PIN_MUX_SA, SA_state);
  digitalWrite(HW_PIN_MUX_SB, SB_state);
  digitalWrite(HW_PIN_MUX_SC, SC_state);

  float readVal = analogRead(HW_PIN_BMSVOLT);
  float analogVoltage = (readVal / ANALOG_CONSTANT) * 5.0;
  analogVoltage += MEASURED_ANALOG_ERROR;
  return analogVoltage;
}


/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(HW_PIN_MUX_SA, OUTPUT);     // TO DO: replace with pin abstraction
  pinMode(HW_PIN_MUX_SB, OUTPUT);
  pinMode(HW_PIN_MUX_SC, OUTPUT);
  pinMode(HW_PIN_BMS_VOLT, INPUT);
}

void loop()
{
  for (HW_MUX_E cell = HW_MUX_BMS_A6; cell != HW_MUX_COUNT; cell++){
    
    readVoltage = HW_readMux(cell);
    
    Serial.print(cell); Serial.print(" reading: "); Serial.print(readVoltage); Serial.print("V \n");
  }
}
