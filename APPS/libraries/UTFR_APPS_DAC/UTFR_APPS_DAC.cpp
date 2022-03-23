#include "UTFR_APPS_DAC.h"


UTFR_APPS_DAC::UTFR_APPS_DAC(uint8_t dataOut, uint8_t clock) : DAC(dataOut, clock)
{
    // initalize the  data ready and chip select pins:
    pinMode(A0, INPUT);                 // APPS_In_1_Analog pin
    pinMode(A1, INPUT);                 // APPS_In_2_Analog pin
    pinMode(A5, INPUT);                 // APPS_Out_Analog

    // give the sensor time to set up:
    delay(100);
}

void UTFR_APPS_DAC::begin(int CS){

    _APPS_1_high = getDigital(_kAPPS_1_HIGH);
    _APPS_1_low = getDigital(_kAPPS_1_LOW);
    _APPS_2_high = getDigital(_kAPPS_2_HIGH);
    _APPS_2_low = getDigital(_kAPPS_2_LOW);

    _DAC_CS = CS;
    DAC.begin(CS);
}


void UTFR_APPS_DAC::processThrottlePosition()
{
    // read in the voltage values and convert to digital
    _APPS_1_in = analogRead(A0);
    _APPS_2_in = analogRead(A1);
    _APPS_out_verify = analogRead(A5);

    // Normalize pedal position readings and multiply by _kANALOG_MAX so we can compare them
    _APPS_1_throttle = map_Generic(_APPS_1_in, _APPS_1_low, _APPS_1_high, 0.0, 255.0); 
    _APPS_2_throttle = map_Generic(_APPS_2_in, _APPS_2_low, _APPS_2_high, 0.0, 255.0);

    // set flags
    _throttle_good = GET_DEV(_APPS_1_throttle, _APPS_2_throttle) < _kTHROTTLE_MAX_DEVIATION;
    _output_good = GET_DEV(_APPS_output, _APPS_out_verify) < _kOUTPUT_DEVIATION;
    _error_flag_set = (_time_at_error != _kBASE_TIME);

    //=========OUTPUT PROCESSING==========//

    if (_exceed_time_allowance)
    {
        shutDown();
        reportError();
    }
    else
    {
        sendOutput();           // send output since we're still within error margin
        _time_now = millis();    // get the current time

        if (_throttle_good && _output_good)
        {
            _error_flag_set = false;
            _time_at_error = _kBASE_TIME;
        }
        else if (_error_flag_set)
        {
            _exceed_time_allowance = (_time_now - _time_at_error >= _kTIME_ALLOWANCE);
        }
        else
        {
            _time_at_error = _time_now;
        }
    }
}

int UTFR_APPS_DAC::getDeviation(int value_1, int value_2)
{
    return abs((value_1 - value_2) / (value_1+0.0001));
}

void UTFR_APPS_DAC::sendOutput()
{
    _APPS_output = round((_APPS_1_throttle + _APPS_2_throttle) / 2);      // take average and round
    
    DAC.analogWrite(_APPS_output, 0);
}

void UTFR_APPS_DAC::reportError()
{
    if (!_throttle_good)
        Serial.println("[APPS] Error: throttle values not in expected range of each other.");
    else if (!_output_good)
        Serial.println("[APPS] Error: throttle value was output incorrectly.");
}

void UTFR_APPS_DAC::shutDown()
{
    DAC.analogWrite(0, 0);  // send out zero throttle
    // ADD COMMS WITH MEGA HERE (Tell mega to trip SDC/ disconnect LV from chassis ground)
}

float getDigital(float voltage){
  return round((voltage * ANALOG_CONSTANT)/10)*2.0;        // did it this way to make it a float again
}