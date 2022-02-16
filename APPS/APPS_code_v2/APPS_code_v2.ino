/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <SPI.h>
#include "MCP_DAC.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define APPS_1_HIGH_ 4.75
#define APPS_2_HIGH_ 4.275
#define APPS_1_LOW_ 1.25
#define APPS_2_LOW_ 1.125

#define DAC_CS_ = 8
#define TIME_ALLOWANCE_ 100 // maximum error time allowed in milliseconds. rule T.4.2.5
#define THROTTLE_MAX_DEVIATION_ 0.10 // rule T.4.2.4
#define OUTPUT_DEVIATION_ 0.03 // abitrary value for time_now, will be determined experimentally

#define BASE_TIME_ -1
#define ANALOG_CONSTANT 1023

#define GET_DEV(x, y) abs((x-y)/x)

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


/******************************************************************************
 *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
 *****************************************************************************/
void sendOutput();
void reportError();
void shutDown();

/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/
MCP4911 DAC; // HW DAC

int APPS_1_in = 0;
int APPS_2_in = 0; 
int APPS_output = 0;
float APPS_out_verify = 0; 
float APPS_1_throttle = 0;
float APPS_2_throttle = 0;
long time_at_error = BASE_TIME_; // unsigned long type according to arduino docs, not sure about this
unsigned long time_now = 0;

bool throttle_good = false;
bool output_good = false;
bool exceed_time_allowance = false;
bool error_flag_set = false;

/******************************************************************************
 *                     P R I V A T E   F U N C T I O N S                      *
 *****************************************************************************/
void sendOutput()
{
  APPS_output = (APPS_1_throttle + APPS_2_throttle) / 2; // send out the mean of both inputs
  DAC.analogWrite(APPS_output, 0);
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
    Serial.println("[APPS] Error: throttle value was output correctly.");
  }
}

// Some logic to shutdown the car
// Might have to send other info over SPI
void shutDown()
{
  // More post processing
}

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/


/******************************************************************************
 *                    P R O G R A M   E X E C U T I O N                       *
 *****************************************************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initialize DAC:
  DAC.begin(DAC_CS_);

  // initalize the  data ready and chip select pins:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A5, INPUT);
  // give the sensor time to set up:
  delay(100);
}

void loop()
{
  // read in the voltage values and convert to digital
  APPS_1_in = analogRead(A0);
  APPS_2_in = analogRead(A1);
  APPS_out_verify = analogRead(A5);

  // model each APPS as a straight line
  APPS_1_throttle = ANALOG_CONSTANT * (APPS_1_in / ANALOG_CONSTANT - APPS_1_LOW_) / (APPS_1_HIGH_ - APPS_1_LOW_);
  APPS_2_throttle = ANALOG_CONSTANT * (APPS_2_in / ANALOG_CONSTANT - APPS_2_LOW_) / (APPS_2_HIGH_ - APPS_2_LOW_);

  // set flags
  throttle_good = GET_DEV(APPS_1_throttle, APPS_2_throttle) < THROTTLE_MAX_DEVIATION_;
  output_good = GET_DEV(APPS_output, APPS_out_verify) < OUTPUT_DEVIATION_;
  error_flag_set = (time_at_error != BASE_TIME_);

  //========= O U T P U T    P R O C E S S I N G ==========//

  if (exceed_time_allowance)
  {
    DAC.analogWrite(0, 0); // send out zero throttle
    shutDown();
    reportError();
  }
  else
  {
    sendOutput();        // send output since we're still within error margin
    time_now = millis(); // get the current time

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
}