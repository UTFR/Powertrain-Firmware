#include "UTFR_APPS.h"


UTFR_APPS::UTFR_APPS(uint8_t dataOut, uint8_t clock)  : DAC(dataOut, clock)
{

}

void UTFR_APPS::begin(int CS){

    // give the sensor time to set up:
    delay(100);

    // initalize the  data ready and chip select pins:
    pinMode(A0, INPUT);                 // APPS_In_1_Analog pin
    pinMode(A1, INPUT);                 // APPS_In_2_Analog pin
    pinMode(A2, INPUT);                 // _Brake_in pin
    pinMode(A5, INPUT);                 // APPS_Out_Analog

    _APPS_1_high = getDigital(_kAPPS_1_HIGH);
    _APPS_1_low = getDigital(_kAPPS_1_LOW);
    _APPS_2_high = getDigital(_kAPPS_2_HIGH);
    _APPS_2_low = getDigital(_kAPPS_2_LOW);
    _Brake_threshold = getDigital(_kBRAKE_THRESHOLD);

    _DAC_CS = CS;
    DAC.begin(CS);
}


void UTFR_APPS::processThrottlePosition()
{   
    // read in the voltage values and convert to digital
    _APPS_1_in = analogRead(A0);
    _APPS_2_in = analogRead(A1);
    _Brake_in = analogRead(A2);
    _APPS_out_verify = analogRead(A5);

    #ifdef debugMode
    Serial.print("Throttle out of DAC: "); Serial.println(_APPS_out_verify);
    #endif

    // Normalize pedal position readings and multiply by _kANALOG_MAX so we can compare them
    _APPS_1_throttle = map_Generic(_APPS_1_in, _APPS_1_low, _APPS_1_high, 0.0, static_cast<float>(_kANALOG_MAX)); 
    _APPS_2_throttle = map_Generic(_APPS_2_in, _APPS_2_low, _APPS_2_high, 0.0, static_cast<float>(_kANALOG_MAX));

    #ifdef debugMode
    Serial.print("APPS_1_throttle: "); Serial.println(_APPS_1_throttle);
    Serial.print("APPS_2_throttle: "); Serial.println(_APPS_2_throttle);
    #endif


    // set flags
    _brakes_good = ( (_Brake_in > _kBRAKE_THRESHOLD) && 
                    ((_APPS_out_verify/_kANALOG_MAX > _kBRAKE_DEVIATION) || 
                    (_APPS_output /_kANALOG_MAX > _kBRAKE_DEVIATION)) 
                    );
    _throttle_good = GET_DEV(_APPS_1_throttle, _APPS_2_throttle) < _kTHROTTLE_MAX_DEVIATION;
    _output_good = GET_DEV(_APPS_output, _APPS_out_verify) < _kOUTPUT_DEVIATION;
    _error_flag_set = (_time_at_error != _kBASE_TIME);

    //=========OUTPUT PROCESSING==========//

    // while brakes are bad, output is set to 0
    if (!_brakes_good) // EV.5.7.1
    {
        if (_APPS_out_verify/_kANALOG_MAX > 0.05)   // EV.5.7.2
        {
            shutdown();
            reportError();
        }
        else
        {
            _brakes_good = true;
        }
    }

    if (_exceed_time_allowance)
    {
        shutdown();
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


void UTFR_APPS::sendOutput()
{
    _APPS_output = round((_APPS_1_throttle + _APPS_2_throttle) / 2);      // Take average and round

    if (_shutdown == true)              // Ensures 0 throttle after shutdown() called (redundant on purpose) 
    {                                               
        _APPS_output = 0;                                                   
    }
    
    DAC.analogWrite(_APPS_output, 0);

    #ifdef debugMode
    Serial.print("Throttle into DAC: "); Serial.println(_APPS_output);
    #endif
}

void UTFR_APPS::reportError()
{
    if (!_throttle_good)
        Serial.println("[APPS] Error: throttle values not in expected range of each other.");
    else if (!_output_good)
        Serial.println("[APPS] Error: throttle value was output incorrectly.");
    else if (!_brakes_good)
        Serial.println("[APPS] Error: brake pedal failed plausibilty check.");
}


void UTFR_APPS::shutdown()
{
    DAC.analogWrite(0, 0);          // Command zero throttle to inverter
    _shutdown = true;
}


bool UTFR_APPS::getShutdownState()
{
    return _shutdown;
}


bool UTFR_APPS::confirmShutdown()
{
    uint8_t retry_count = 0;

    while(retry_count < _confirm_shutdown_retries)
    {
        if (analogRead(A5) == 0)
        {
            return true;
        }
        else
        {
            retry_count += 1;
        }
    }

    return false;
}


float UTFR_APPS::getDigital(float voltage){
  return map_Generic(voltage, 0.0, 5.0, 0.0, static_cast<float>(_kANALOG_MAX));
}