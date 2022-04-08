#include "UTFR_COMMS_MEGA.h"


UTFR_COMMS_MEGA::UTFR_COMMS_MEGA()
{
    Serial3.begin(115200);
}


void UTFR_COMMS_MEGA::receiveSerialMicro(UTFR_CAN_RC &CAN)
{
    if(Serial3.available() > 0)                       // Receive serial msg from Micro
    {               
        _rx_msg = Serial3.readStringUntil('\n');

        #ifdef debug_CommsMega
        Serial.print("Serial msg rx from Micro: "); Serial.println(_rx_msg);
        #endif
    }

    if (_rx_msg[0] == COMMS_MSG_LOGDATA)
    {
        int fieldData = (_rx_msg[1] << 8) | _rx_msg[2];     // Throttle position: first 2 bytes after msg ID
        CAN.setField(CAN_MSG_MICRO, THROTTLE_POS_F, fieldData);

        fieldData = (_rx_msg[3] << 8) | _rx_msg[4];         // Brake pressure
        CAN.setField(CAN_MSG_MICRO, BRAKE_PRESS_F, fieldData);

        fieldData = (_rx_msg[5] << 8) | _rx_msg[6];         // Front Left wheel speed
        CAN.setField(CAN_MSG_MICRO, WHLSPD_FL_F, fieldData);
        
        fieldData = (_rx_msg[7] << 8) | _rx_msg[8];         // Front Right wheel speed
        CAN.setField(CAN_MSG_MICRO, WHLSPD_FR_F, fieldData); 
    }
}




