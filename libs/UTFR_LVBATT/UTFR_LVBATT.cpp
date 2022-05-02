#include "UTFR_LVBATT.h"

UTFR_LVBATT::UTFR_LVBATT()
{
    pinMode(HW_PIN_MUX_SA, OUTPUT);     // TO DO: replace with pin abstraction
    pinMode(HW_PIN_MUX_SB, OUTPUT);
    pinMode(HW_PIN_MUX_SC, OUTPUT);

    pinMode(HW_PIN_BMSVOLT, INPUT);
    pinMode(HW_PIN_BMSTEMP_1, INPUT);
    pinMode(HW_PIN_BMSTEMP_2, INPUT);
}


void UTFR_LVBATT::readCellVoltage(uint8_t muxPin)
{
    bool SA_state = (muxPin & _SA_MASK);

    bool SB_state = (muxPin & _SB_MASK) >> 1;

    bool SC_state = (muxPin & _SC_MASK) >> 2;


    // Set the mux select pins

    digitalWrite(HW_PIN_MUX_SA, SA_state);

    digitalWrite(HW_PIN_MUX_SB, SB_state);

    digitalWrite(HW_PIN_MUX_SC, SC_state);

    //delayMicroseconds(5);                     // TO DO: determine if needed

    _readVoltage = map_Generic(static_cast<float>(analogRead(HW_PIN_BMSVOLT)), 0.0, _highVoltRaw[muxPin], 
                                                                               0.0, _highVolt[muxPin]);
    _readVoltage += _measured_analog_error;
}


void UTFR_LVBATT::readBattTemps()
{
    float temp1 = map_Generic(static_cast<float>(analogRead(HW_PIN_BMSTEMP_1)), _lowTempRaw, _highTempRaw, _lowTemp, _highTemp);  
    float temp2 = map_Generic(static_cast<float>(analogRead(HW_PIN_BMSTEMP_2)), _lowTempRaw, _highTempRaw, _lowTemp, _highTemp);

    if (temp1 > temp2)          // return max temp between the two LV batteries
    {
        _readTemp = temp1;
    }
    else
    {
        _readTemp = temp2;
    }
}


bool UTFR_LVBATT::checkLVBatt(UTFR_CAN_RC& CAN, UTFR_ERROR& ERROR)
{
    bool fail = false;

    for (_cell = 0; _cell<_total_cells; _cell++)        // Check cell voltages
    {
       readCellVoltage(_cell);
       if (_readVoltage < _volt_cutoff[_cell])
       {    
            #ifdef debugLVBatt
            Serial.println("BMS UNDERVOLTAGE ERROR");
            #endif
            
            ERROR.sendError(CAN, BMS_UNDERVOLT);
            fail = true;
       }
    }

    readBattTemps();                                    // Check battery temps
    if (_readTemp > _temp_cutoff)
    {
        #ifdef debugLVBatt
        Serial.println("BMS OVERTEMP ERROR");
        #endif

        ERROR.sendError(CAN, BMS_OVERTEMP);
        fail = true;
    }
    
    if (fail)
    {
        _failedChecks += 1;
        if (_failedChecks >= _maxFailed)
        {
            return false;
        }
        
    }
    else
    {
        _failedChecks = 0;
    }

    return true;
}