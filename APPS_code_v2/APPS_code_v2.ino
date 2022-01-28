#include <SPI.h>

#include "MCP_DAC.h"

MCP4911 DAC; // HW DAC

// declare global constants
const int APPS_1_HIGH_ = 4.75;
const int APPS_2_HIGH_ = 4.275;
const int APPS_1_LOW_ = 1.25;
const int APPS_2_LOW_ = 1.125;

const int DAC_CS_ = 8;
const int TIME_ALLOWANCE_ = 100; // maximum error time allowed in milliseconds
const float THROTTLE_MAX_DEVIATION_ = 0.10;
const float OUTPUT_DEVIATION_ = 0.03; // abitrary value for time_now, will be determined experimentally
const long BASE_TIME_ = -1;

// declare variables
int APPS_1_in = 0;
int APPS_2_in = 0; //0 - 1023
int APPS_output = 0;
float APPS_out_verify = 0; //0 - 1023
float APPS_1_throttle = 0;
float APPS_2_throttle = 0;
long time_at_error = BASE_TIME_; // unsigned long type according to arduino docs, not sure about this
unsigned long time_now = 0;

bool throttle_good = false;
bool output_good = false;
bool exceed_time_allowance = false;
bool error_flag_set = false;

//========FUNCTION DECLARATIONS========//
int getDeviation(int value_1, int value_2);
void sendOutput();
void reportError();
void shutDown();


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
  APPS_1_throttle = 1023 * (APPS_1_in / 1023 - APPS_1_LOW_) / (APPS_1_HIGH_ - APPS_1_LOW_);
  APPS_2_throttle = 1023 * (APPS_2_in / 1023 - APPS_2_LOW_) / (APPS_2_HIGH_ - APPS_2_LOW_);

  // set flags
  throttle_good = getDeviation(APPS_1_in, APPS_2_in) < THROTTLE_MAX_DEVIATION_;
  output_good = getDeviation(APPS_output, APPS_out_verify) < OUTPUT_DEVIATION_;
  error_flag_set = time_at_error != BASE_TIME_;

  //=========OUTPUT PROCESSING==========//

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
      error_flag_set = false;
      time_at_error = BASE_TIME_;
    }
    else if (error_flag_set)
    {
      exceed_time_allowance = (time_now - time_at_error) >= TIME_ALLOWANCE_;
    }
    else
    {
      time_at_error = time_now;
    }
  }
}

//==========FUNCTION DEFINITIONS==========//

int getDeviation(int value_1, int value_2)
{
  return abs((value_1 - value_2) / value_1);
}

void sendOutput()
{
  APPS_output = (APPS_1_throttle + APPS_2_throttle) / 2; // send out the mean of both inputs
  DAC.analogWrite(APPS_output, 0);
}

void reportError()
{
  if (!throttle_good)
    Serial.println("[APPS] Error: throttle values not in expected range of each other.");
  else if (!output_good)
    Serial.println("[APPS] Error: throttle value was output correctly.");
}

void shutDown()
{
  //=======More post processing=======//
  // some logic to shutdown the car
  // might have to send other info over SPI
}