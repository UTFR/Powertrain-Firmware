#include "UTFR_CAN_ERROR.h";

void UTFR_CAN_ERROR::init_can(){
    while (CAN_OK != this->begin(CAN_500KBPS)) {
        Serial.println("ERROR_CAN init fail - trying again...");
        delay(100);
    }

    this->init_Mask(0, 0, 0b11111111111);
    this->init_Mask(1, 0, 0b11111111111);

    Serial.println("ERROR_CAN Init success!");
}

bool UTFR_CAN_ERROR::send_can_error(const unsigned char &msg){
    if (msg.length != 8) {
        Serial.println("UTFR_CAN_ERROR::send_can_error : error message length incorrect!")
        return false;
    }
    this->sendMsgBuf(_kERROR_MSG_ID, 0, 8, msg)
    return true;
}