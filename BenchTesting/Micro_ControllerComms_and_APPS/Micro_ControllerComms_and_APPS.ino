/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SoftwareSerial.h>
#include "MCP_DAC.h"


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define APPS_1_HIGH_ 4.75        
#define APPS_2_HIGH_ 4.75          //4.275
#define APPS_1_LOW_ 1.25
#define APPS_2_LOW_ 1.25          // 1.125

#define DAC_CS_ 7
#define TIME_ALLOWANCE_ 100 // maximum error time allowed in milliseconds. rule T.4.2.5
#define THROTTLE_MAX_DEVIATION_ 0.10 // rule T.4.2.4
#define OUTPUT_DEVIATION_ 0.03 // abitrary value for time_now, will be determined experimentally

#define BASE_TIME_ -1
#define ANALOG_CONSTANT 1023

#define GET_DEV(x, y) abs((x-y)/(x+0.0001))


/******************************************************************************
 *         F U N C T I O N   D E C L A R A T I O N S                          *
 *****************************************************************************/
float getDigital(float voltage);
void sendOutput();
void reportError();
void shutDown();


/******************************************************************************
 *              D A T A   D E F I N I T I O N S                               * 
 *****************************************************************************/
// =================== APPS things
MCP4911 apps;

float APPS_1_high = 0;
float APPS_2_high = 0;
float APPS_1_low = 0;
float APPS_2_low = 0;

float APPS_1_in = 0;
float APPS_2_in = 0; 
int APPS_output = 0;
int APPS_out_verify = 0; 
float APPS_1_throttle = 0;
float APPS_2_throttle = 0;
long time_at_error = BASE_TIME_; // unsigned long type according to arduino docs, not sure about this
unsigned long time_now = 0;

bool throttle_good = false;
bool output_good = false;
bool exceed_time_allowance = false;
bool error_flag_set = false;

// ================== Serial comms 
SoftwareSerial megaSerial(10, 13);
char rec;

byte sent_bytes;


/******************************************************************************
 *                     F U N C T I O N S                                      *
 *****************************************************************************/
 
float getDigital(float voltage){
  // converts voltage limits to digital values for use in calculations
  return round((voltage * ANALOG_CONSTANT)/10)*2.0;         // equivalent to dividing by 5 (logic high), did it this way to make it a float again
}
 
void sendOutput()
{
  // send out the mean of both inputs
  APPS_output = (APPS_1_throttle + APPS_2_throttle) / 2;    
  apps.analogWrite(APPS_output, 0);
  Serial.print("Throttle into DAC: "); Serial.println(APPS_output);
}

// Reports when implausibility occurs for more than 100ms 
// Basic implementation for now
void reportError()
{
  if (!throttle_good)
  {
    Serial.println("[APPS] Error: throttle values not in expected range of each other.");
  }
  else if (!output_good)
  {
    Serial.println("[APPS] Error: throttle value was output incorrectly.");
  }
}

// Some logic to shutdown the car
// Might have to send other info over SPI
void shutDown()
{
  // More post processing
}


/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/
void setup() {
  
  Serial.begin(115200);
  megaSerial.begin(9600);

  apps.begin(DAC_CS_);
  apps.analogWrite(0,0);

  //pinMode(A3, OUTPUT);    // These are fake pedal position sensor outputs
  //pinMode(A4, OUTPUT);

  //analogWrite(A3, 155);
  //analogWrite(A4, 155);

  //pinMode(A0, INPUT);    // Route A3 to here
  //pinMode(A1, INPUT);    // Route A4 to here
  pinMode(A5, INPUT);    // output of DAC fed back here

  APPS_1_high = getDigital(APPS_1_HIGH_);
  APPS_2_high = getDigital(APPS_2_HIGH_);
  APPS_1_low = getDigital(APPS_1_LOW_);
  APPS_2_low = getDigital(APPS_2_LOW_);
  
  delay(100);            // give the sensor time to set up
 
}

void loop() {

  // ================= PROCESS THROTTLE POSITION ===========================================================
  // =======================================================================================================
  
  // read in the voltage values and convert to digital
  APPS_1_in = 800;      //analogRead(A0);
  APPS_2_in = 799;      //analogRead(A1);
  APPS_out_verify = analogRead(A5);

  Serial.print("Throttle out of DAC: "); Serial.println(APPS_out_verify);

  // model each APPS as a straight line
  APPS_1_throttle = ANALOG_CONSTANT * ((APPS_1_in  - APPS_1_low)/(APPS_1_high - APPS_1_low));
  APPS_2_throttle = ANALOG_CONSTANT * ((APPS_2_in - APPS_2_low)/(APPS_2_high - APPS_2_low));  

  Serial.print("APPS_1_throttle: "); Serial.println(APPS_1_throttle);
  Serial.print("APPS_2_throttle: "); Serial.println(APPS_2_throttle);

  //Serial.print("MAMA MIA PIZZARIA: "); Serial.print(APPS_1_in); Serial.print(" | "); Serial.print(APPS_2_in); Serial.println();
  
  // set flags
  throttle_good = GET_DEV(APPS_1_throttle, APPS_2_throttle) < THROTTLE_MAX_DEVIATION_;
  output_good = GET_DEV(APPS_output, APPS_out_verify) < OUTPUT_DEVIATION_;
  error_flag_set = (time_at_error != BASE_TIME_);

  if (exceed_time_allowance)
  {
    apps.analogWrite(0, 0); // send out zero throttle
    shutDown();
    reportError();
  }
  else
  {
    Serial.println("Sending output");
    sendOutput();           // send output since we're still within error margin
    time_now = millis();    // get the current time

    if (throttle_good && output_good)
    {
      // reset the error flags
      error_flag_set = false;
      time_at_error = BASE_TIME_;
    }
    else if (error_flag_set)
    {
      exceed_time_allowance = (time_now - time_at_error >= TIME_ALLOWANCE_);
    }
    else
    {
      time_at_error = time_now;
    }
  } 

  
  // ================== SERIAL COMMS WITH MEGA =============================================================
  // =======================================================================================================
  /*
  sent_bytes = megaSerial.print("Hello from da Micro");
  Serial.print("Sent bytes: "); Serial.println(sent_bytes);
  
  while (megaSerial.available() > 0){
    rec = megaSerial.read();
    Serial.print(rec);
  } 
  Serial.println();

  delay(50);
  */
}
