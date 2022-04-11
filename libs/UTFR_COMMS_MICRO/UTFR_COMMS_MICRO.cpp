#include "UTFR_COMMS_MICRO.h"


UTFR_COMMS_MICRO::UTFR_COMMS_MICRO()
{
    megaSerial.begin(115200);
}

void UTFR_COMMS_MICRO::sendSerialMega(UTFR_APPS &APPS, _COMMS_msgNames_E msgName)
{
    if (msgName == COMMS_MSG_LOGDATA)
    {
        //The below is not allowed in cpp - the loop works, but 
        // I'm sure there's a more efficent way to do this with std::copy

        /*_sendData = {COMMS_MSG_LOGDATA,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF};  */ 
        _sendData[0] = COMMS_MSG_LOGDATA;
        for (size_t i = 1; i < 9; ++i) {
            _sendData[i] = 0xFF;
        }


        // Get data
        _throttle_out = APPS.getThrottlePosition();
        _brake_in = analogRead(HW_PIN_BRAKE_IN);        
        _whlspd_fl = analogRead(HW_PIN_WHLSPD_FL);
        _whlspd_fr = analogRead(HW_PIN_WHLSPD_FR);

        // Set throttle
        _sendData[1] = _throttle_out & 0xFF;
        _sendData[2] = _throttle_out & 0xFF00;

        // Set brake
        _sendData[3] = _brake_in & 0xFF;
        _sendData[4] = _brake_in & 0xFF00;

        // Set wheelspeed FL
        _sendData[5] = _whlspd_fl & 0xFF;
        _sendData[6] = _whlspd_fl & 0xFF00;
        
        // Set wheelspeed FR
        _sendData[7] = _whlspd_fl & 0xFF;
        _sendData[8] = _whlspd_fl & 0xFF00;

        // Send data
        megaSerial.println(_sendData);                  // println adds '\n' character to end of message, this is the delimiter we're using
    }
}