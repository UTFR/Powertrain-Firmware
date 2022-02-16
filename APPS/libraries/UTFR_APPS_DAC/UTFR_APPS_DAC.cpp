#include "UTFR_APPS_DAC.h"

// ###################################################################################
// ######### Call in setup() --> Don't forget to also call APPS_DAC.begin(select) ####
// ###################################################################################
UTFR_APPS_DAC::UTFR_APPS_DAC(uint8_t dataOut, uint8_t clock) : MCP4911(dataOut, clock)
{
    // initalize the  data ready and chip select pins:
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A5, INPUT);

    // give the sensor time to set up:
    delay(100);
}

// #######################################################
// ################ Call in loop() #######################
// #######################################################

void UTFR_APPS_DAC::processThrottlePosition()
{
    // read in the voltage values and convert to digital
    _APPS_1_in = analogRead(A0);
    _APPS_2_in = analogRead(A1);
    _APPS_out_verify = analogRead(A5);

    // Normalize pedal position readings and multiply by _kANALOG_MAX so we can compare them
    _APPS_1_throttle = _kANALOG_MAX * ( (_APPS_1_in / _kANALOG_MAX) - _kAPPS_1_LOW) / (_kAPPS_1_HIGH - _kAPPS_1_LOW);
    _APPS_2_throttle = _kANALOG_MAX * ( (_APPS_2_in / _kANALOG_MAX) - _kAPPS_2_LOW) / (_kAPPS_2_HIGH - _kAPPS_2_LOW);

    // set flags
    _throttle_good = getDeviation(_APPS_1_throttle, _APPS_2_throttle) < _kTHROTTLE_MAX_DEVIATION;
    _output_good = getDeviation(_APPS_output, _APPS_out_verify) < _kOUTPUT_DEVIATION;
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
    return abs((value_1 - value_2) / value_1);
}

void UTFR_APPS_DAC::sendOutput()
{
    _APPS_output = round((_APPS_1_throttle + _APPS_2_throttle) / 2);      // take average and round
    
    analogWrite(_APPS_output, 0);
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
    analogWrite(0, 0);  // send out zero throttle
    // ADD COMMS WITH MEGA HERE (Tell mega to trip SDC/ disconnect LV from chassis ground)
}