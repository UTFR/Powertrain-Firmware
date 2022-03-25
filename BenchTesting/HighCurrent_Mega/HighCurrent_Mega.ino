/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SPI.h>
#include "SCL3300.h"      // Accelerometer

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
// Input pins
#define HW_PIN_SDC_INT 19
#define HW_PIN_BMSVOLT A12

// Output pins
#define HW_PIN_PWM_MOTOR_MOSFET 8
#define HW_PIN_PWM_INVERTER_MOSFET 9
#define HW_PIN_OE 32
#define HW_PIN_MUX_SA 35        
#define HW_PIN_MUX_SB 34
#define HW_PIN_MUX_SC 36
#define HW_PIN_INVERTER_IGNITION 38
#define HW_PIN_CHASSIS_DSC 39
#define HW_PIN_MS_COM_1 40
#define HW_PIN_ACCEL_CS 49

// Constants
#define ANALOG_CONSTANT 1023           // 10-bit ADC resolution
#define MEASURED_ANALOG_ERROR 0.01     // Determine experimentally

#define SA_MASK 0b001
#define SB_MASK 0b010
#define SC_MASK 0b100

#define SERIAL3_BUFFER_SIZE 4

#define SERIAL_HEARTBEAT_TIMEOUT 1000


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
typedef enum                  
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
 *              F U N C T I O N   D E C L A R A T I O N S                     *
 *****************************************************************************/
// ACCELEROMETER
void getAccel(void);

// BMS MUX
float HW_readMux(HW_MUX_E muxPin);  
HW_MUX_E EnumOfIndex(int i);     
void readMuxAll(void);

// SDC
void SDC_ISR(void);

// Serial Comms
void readSerial3(void);
bool heartbearSerial3(void);

/******************************************************************************
 *              D A T A   D E F I N I T I O N S                               *
 *****************************************************************************/
// GENERAL
bool success = false;     // holds function outcomes
 
// INVERTER IGNITION
bool preStartup = true;
 
// ACCELEROMETER
SCL3300 accel;

// BMS MUX
float readVoltage = 0.0;
HW_MUX_E cell_E = HW_MUX_COUNT;

// SDC
bool SDC_tripped = false;
int SDC_pin_state = 0;

// MEGA-MICRO COMMS
char serial3_buffer[SERIAL3_BUFFER_SIZE];

/******************************************************************************
 *              F U N C T I O N   D E F I N I T I O N S                       *
 *****************************************************************************/
// ACCELEROMETER
void getAccel(){
  if (accel.available()) { //Get next block of data from sensor
    Serial.print("X Accelerometer: ");
    Serial.print(accel.getCalculatedAccelerometerX());
    Serial.print("\t");
    Serial.print("Y Accelerometer: ");
    Serial.print(accel.getCalculatedAccelerometerY());
    Serial.print("\t");
    Serial.print("Z Accelerometer: ");
    Serial.println(accel.getCalculatedAccelerometerZ());
    delay(250); //Allow a little time to see the output
  } else accel.reset();
}

// BMS MUX
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

HW_MUX_E EnumOfIndex(int i)     // Converts integer value to corresponding enum
{ 
  return static_cast<HW_MUX_E>(i); 
}

void readMuxAll(){
    for (int cell = 0; cell != static_cast<int>(HW_MUX_COUNT); cell++){
    cell_E = EnumOfIndex(cell);
    readVoltage = HW_readMux(cell_E);
    Serial.print(cell_E); Serial.print(" reading: "); Serial.print(readVoltage); Serial.print("V \n");
  }
  Serial.println("=================================================================================");
}

// SDC
void SDC_ISR(){
  SDC_tripped = true;
}

bool shutdown(){
  
  digitalWrite(HW_PIN_MS_COM_1, HIGH);    // tell Micro SDC tripped
  
  Serial.println("Waiting for Micro response.");
  while (Serial3.available() == 0){
  }
  readSerial3();
  
  while (serial3_buffer != "SDC1"){    // if successful shutdown
    if (Serial3.available() > 0){
      readSerial3();
    }
  }
  
  digitalWrite(HW_PIN_INVERTER_IGNITION, LOW);
  return true;
}

// Serial Comms
void readSerial3(){
  Serial3.readBytesUntil('\n', serial3_buffer, SERIAL3_BUFFER_SIZE);
  Serial.print("Micro response: "); Serial.println(serial3_buffer);
}

bool heartbeatSerial3(){
  unsigned long tick, tock = 0;
  Serial3.println("beep");
  tick = millis();
  tock = millis();
  while( Serial3.available() == 0 ){
    tock = millis();
    if (tock-tick > SERIAL_HEARTBEAT_TIMEOUT){
      return false;
    }
  }
  readSerial3();
  if (serial3_buffer == "boop"){
    return true;
  }
  else {
    Serial.println("Incorrect heartbeat response.");
    return false;
  }
}

 /*****************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
 
void setup() {

   /*************************************************
   *          OBJECT  SETUP                        *
   *************************************************/

  // Mega-PC serial port
  Serial.begin(9600);

  // Mega-Micro serial port
  Serial3.begin(9600);

  // Accelerometer
  while (accel.begin(HW_PIN_ACCEL_CS) == false){
    Serial.println("Failed to initialize accelerometer.");
    delay(1000);
  }

  /*************************************************
   *            PIN  SETUP                         *
   *************************************************/
  // Connect BMS GND to GND
  pinMode(HW_PIN_CHASSIS_DSC, OUTPUT);
  digitalWrite(HW_PIN_CHASSIS_DSC, HIGH);

  // INVERTER IGNITION
  pinMode(HW_PIN_INVERTER_IGNITION, OUTPUT);
  
  // PWM-DRIVEN MOSFETS
  pinMode(HW_PIN_PWM_MOTOR_MOSFET, OUTPUT);
  pinMode(HW_PIN_PWM_INVERTER_MOSFET, OUTPUT);
  analogWrite(HW_PIN_PWM_INVERTER_MOSFET, HIGH);    // Constantly running pumps at max (5V)
  analogWrite(HW_PIN_PWM_MOTOR_MOSFET, HIGH);

  // ACCELEROMETER - enable level shifter output
  pinMode(HW_PIN_ACCEL_CS, OUTPUT);
  pinMode(HW_PIN_OE, OUTPUT);         
  digitalWrite(HW_PIN_OE, HIGH);    

  // BMS MUX
  pinMode(HW_PIN_MUX_SA, OUTPUT); 
  pinMode(HW_PIN_MUX_SB, OUTPUT);
  pinMode(HW_PIN_MUX_SC, OUTPUT);
  pinMode(HW_PIN_BMSVOLT, INPUT);

  // SDC
  pinMode(HW_PIN_MS_COM_1, OUTPUT);
  pinMode(HW_PIN_SDC_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(HW_PIN_SDC_INT), SDC_ISR, RISING);
}

void loop() {

  // Turn on inverter at program start
  if (preStartup){
    delay(2000);  // give things a couple seconds to setup
    preStartup = false;
    Serial.println("Inverter ignition: ON");
    digitalWrite(HW_PIN_INVERTER_IGNITION, HIGH);
  }
  
  // Check for SDC trip
  if (SDC_tripped){
    success = shutdown();
    if (success){
      Serial.println("=========================== Shutdown successsful. ===================================");
      while(true){
      }
    }
    else{
      Serial.println("!!! SHUTDOWN FAILED !!!");
    }
  }
  // Normal task execution in no SDC trip
  else {

    // Accelerometer
    getAccel();

    // BMS Mux
    readMuxAll();

    // Serial3 Comms
    if (heartbeatSerial3() == true){
      Serial.print("Serial3 comms good.");
    }
    else{
      Serial.print("Serial3 heartbeat failure.");
    }
  }
}
