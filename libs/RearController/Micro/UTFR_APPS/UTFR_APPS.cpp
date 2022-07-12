#include "UTFR_APPS.h"


UTFR_APPS::UTFR_APPS(uint8_t dataOut, uint8_t clock)  : DAC(dataOut, clock)
{
}

void UTFR_APPS::begin(int CS)
{
    delay(10);                          // Give the DAC time to set up
    DAC.begin(CS);
}


void UTFR_APPS::processThrottlePosition()
{   
    _APPS_1_in = HW_analogRead(HW_PIN_APPS_IN_1);
    _APPS_2_in = HW_analogRead(HW_PIN_APPS_IN_2);

    #ifdef debug_APPS                   //Automatic calibration - TESTING ONLY
    if (_APPS_1_in > _APPS_1_high) {
        _APPS_1_high = _APPS_1_in;
    }
    if (_APPS_2_in > _APPS_2_high) {
        _APPS_2_high = _APPS_2_in;
    }
    if (_APPS_1_in < _APPS_1_low) {
        _APPS_1_low = _APPS_1_in;
    }
    if (_APPS_2_in < _APPS_2_low) {
        _APPS_2_low = _APPS_2_in;
    }

    Serial.print("APPS_1_Low : ");Serial.print(_APPS_1_low);Serial.print("APPS_1_High : ");Serial.println(_APPS_1_high);
    Serial.print("APPS_2_Low : ");Serial.print(_APPS_2_low);Serial.print("APPS_2_High : ");Serial.println(_APPS_2_high);
    #endif

    _Brake_in = HW_analogRead(HW_PIN_BRAKE_IN);
    _APPS_out_verify = HW_analogRead(HW_PIN_APPS_OUT);

    #ifdef debug_APPS
    Serial.print("Throttle into DAC: "); Serial.println(_APPS_output);
    #endif

    // Normalize pedal position readings and multiply by _kANALOG_MAX so we can compare them
    
    _APPS_1_throttle = map_Generic(_APPS_1_in, _APPS_1_low, _APPS_1_high, 0.0, static_cast<float>(_kANALOG_MAX)); 
    _APPS_2_throttle = map_Generic(_APPS_2_in, _APPS_2_low, _APPS_2_high, 0.0, static_cast<float>(_kANALOG_MAX));

    #ifdef debug_APPS
    Serial.print("APPS_1_in: "); Serial.println(_APPS_1_in);
    Serial.print("APPS_2_in: "); Serial.println(_APPS_2_in);
    Serial.print("APPS_1_throttle: "); Serial.println(_APPS_1_throttle);
    Serial.print("APPS_2_throttle: "); Serial.println(_APPS_2_throttle);
    #endif

    if(!_brake_and_throttle)
    {
        _brake_and_throttle = ((_Brake_in > _kBRAKE_THRESHOLD) &&           // DO NOT CHANGE: Rule EV.5.7.1
                            ((_APPS_out_verify/_kANALOG_MAX > _kTHROTTLE_WHILE_BRAKE_LIMIT) ||  
                            (_APPS_output /_kANALOG_MAX > _kTHROTTLE_WHILE_BRAKE_LIMIT)) 
                            );        
    }    

    _throttle_good = (_APPS_1_throttle - _APPS_2_throttle) < static_cast<float>(_kANALOG_MAX)*_kTHROTTLE_MAX_DEVIATION; 
    _output_good = (_APPS_output - _APPS_out_verify) < static_cast<float>(_kANALOG_MAX)*_kOUTPUT_MAX_DEVIATION; //Changed both to a percent of total range
    _error_flag_set = (_time_at_error != _kBASE_TIME);


    if (_brake_and_throttle)                        // Rule EV.5.7.1
    {
        if (_APPS_out_verify/_kANALOG_MAX > 0.05)   // Rule EV.5.7.2
        {
            DAC.analogWrite(0, 0);                  // Send zero torque to inverter
            reportError();
        }
        else
        {
            _brake_and_throttle = false;
        }
    }

    if (_exceed_time_allowance)
    {
        shutdown();
        reportError();
    }
    else
    {
        sendOutput();               // send output since we're still within error margin
        _time_now = millis();       // get the current time

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
    _APPS_output_array[_APPS_output_idx] = static_cast<float>(_kANALOG_MAX) - round((_APPS_1_throttle + _APPS_2_throttle) / 2);      // Take average and round

    for (int idx_offset = 0; idx_offset < kLOW_PASS_WINDOW-1; ++idx_offset) {
        int curr_idx = _APPS_output_idx - idx_offset;
        if (curr_idx < 0) {
            curr_idx = kAPPS_OUTPUT_ARRAY_SIZE + curr_idx;
        }
        _APPS_output += _APPS_output_array[curr_idx]; 
    }

    _APPS_output = (_APPS_output / kLOW_PASS_WINDOW);

    if (_shutdown || _brake_and_throttle)        // Ensures 0 throttle in both shutdown and brake+throttle cases
    {                                                   
        _APPS_output = 0;                                                   
    }
    
    DAC.analogWrite(_APPS_output, 0);

    ++_APPS_output_idx;

    if (_APPS_output_idx >=  kAPPS_OUTPUT_ARRAY_SIZE) {
        _APPS_output_idx = 0;
    }

    #ifdef debug_APPS
    Serial.print("Throttle from DAC: "); Serial.println(_APPS_out_verify);
    #endif
}


void UTFR_APPS::reportError()
{   
    #ifdef debug_APPS               // Will only see these prints when laptop hooked up anyways
    if (!_throttle_good)
        Serial.println("[APPS] Error: throttle values not in expected range of each other.");
    else if (!_output_good)
        Serial.println("[APPS] Error: throttle value was output incorrectly.");
    else if (_brake_and_throttle)
        Serial.println("[APPS] Error: brake pedal and throttle both pressed. Zero torque ouput.");
    #endif
}


void UTFR_APPS::shutdown()
{
    DAC.analogWrite(0, 0);                                  // Command zero torque to inverter
    HW_digitalWrite(HW_PIN_MEGA_MICRO_3_DIGITAL, true);     // Tell Mega of pedal implausibility
    _shutdown = true;                                       // Car will not exit shutdown state in this case
}