/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_ERROR.h"


void UTFR_ERROR::UTFR_ERROR(uint8_t CS) : ERROR_NODE(pin);

void UTFR_ERROR::begin(){

    ERROR_NODE.begin(CAN_500KBPS);
    ERROR_NODE.setFilters_permitNone();

    #ifdef debugMode
    Serial.println("UTFR_ERROR::begin: ERROR_CAN Init success!");
    #endif
}

void UTFR_ERROR::sendError(errorNAMES_E error){
    for (uint8_t msgIndex = 0; msgIndex < CAN_MSG_COUNT; msgIndex++) {
        for (uint8_t fieldIndex = 0; fieldIndex < 8; fieldIndex++) {
            if (error_map_[msgIndex][fieldIndex] == error) {
                ERROR_NODE.setField(msgIndex, fieldIndex, 0x00);
                ERROR_NODE.sendMsg(msgIndex);
                #ifdef debugMode
                Serial.print("UTFR_ERROR::sendError: Sent Error at Msg Idx ");
                Serial.print(msgIndex);
                Serial.print(" with Field Idx ");
                Serial.print(fieldIndex);
                #endif
                return;
            }
        }
    }
    Serial.print("UTFR_ERROR::sendError: inputted error not found in error map");
}