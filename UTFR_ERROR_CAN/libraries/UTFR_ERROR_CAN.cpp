#include "UTFR_ERROR_CAN.h";

void UTFR_ERROR_CAN::UTFR_ERROR_CAN(const int pin) : CAN(pin);

void UTFR_ERROR_CAN::initCan(){
    while (CAN_OK != CAN.begin(CAN_500KBPS)) {
        Serial.println("ERROR_CAN init fail - trying again...");
        delay(100);
    }

    CAN.init_Mask(0, 0, 0b11111111111);
    CAN.init_Mask(1, 0, 0b11111111111);

    Serial.println("ERROR_CAN Init success!");
}

bool UTFR_ERROR_CAN::sendError(const unsigned char &msg){
    if (msg.length != 8) {
        Serial.println("UTFR_ERROR_CAN::sendError: Error message length incorrect!")
        return false;
    }
    CAN.sendMsgBuf(_kERROR_MSG_ID, 0, 8, msg)
    return true;
}