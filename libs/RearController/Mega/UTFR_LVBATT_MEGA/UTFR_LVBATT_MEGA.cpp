#include "UTFR_LVBATT_MEGA.h"

UTFR_LVBATT_MEGA::UTFR_LVBATT_MEGA()
{
}

void UTFR_LVBATT_MEGA::readBattVoltage(void)
{
    _readVoltage = map_Generic(static_cast<float>(HW_analogRead(HW_PIN_BMS_CHARGE_MEGA_ANALOG)), _lowVoltRaw, _highVoltRaw, 
                                                                                              _lowVolt, _highVolt);
    _readVoltage += _measured_analog_error;
}

void UTFR_LVBATT_MEGA::readBattTemps()
{
    float temp1 = map_Generic(static_cast<float>(HW_analogRead(HW_PIN_BMS_TEMP_ANALOG_1)), _lowTempRaw, _highTempRaw, 
                                                                                        _lowTemp, _highTemp);  
    float temp2 = map_Generic(static_cast<float>(HW_analogRead(HW_PIN_BMS_TEMP_ANALOG_2)), _lowTempRaw, _highTempRaw,
                                                                                        _lowTemp, _highTemp);

    if (temp1 > temp2)          // return max temp between the two LV batteries
    {
        _readTemp = temp1;
    }
    else
    {
        _readTemp = temp2;
    }
}


bool UTFR_LVBATT_MEGA::checkLVBatt(UTFR_CAN_MEGA& CAN)
{
    /* Old way of muxing thru individual cells, not used now  (mux fucked)
    for (_cell = 0; _cell<_total_cells; _cell++)        // Check cell voltages
    {
       readCellVoltage(_cell);
       if (_readVoltage < _volt_cutoff)
       {    
            #ifdef debug_LVBatt
            Serial.println("BMS UNDERVOLTAGE ERROR");
            #endif
            
            ERROR.sendError(CAN, ERR_BMS_UNDERVOLT);
            fail = true;
       }
    }*/

    bool fail = false;

    readBattVoltage();                                     // Check battery charge
    if (_readVoltage < _volt_cutoff)
    {    
        #ifdef debug_LVBatt
        Serial.println("BMS UNDERVOLTAGE ERROR");
        #endif
        
        CAN.sendError(ERR_BMS_UNDERVOLT);
        fail = true;
    }
    readBattTemps();                                    // Check battery temps
    if (_readTemp > _temp_cutoff)
    {
        #ifdef debug_LVBatt
        Serial.println("BMS OVERTEMP ERROR");
        #endif

        CAN.sendError(ERR_BMS_OVERTEMP);
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

 
/*   Old way of muxing thru individual cells, not used now (mux fucked)
void UTFR_LVBATT_MEGA::readCellVoltage(uint8_t muxPin)
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
}*/ 