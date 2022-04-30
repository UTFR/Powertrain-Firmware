/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_ERROR.h"

void UTFR_ERROR::sendError(UTFR_CAN_RC& error_node, errorNAMES_E error){
    for (uint8_t msgIndex = 0; msgIndex < CAN_MSG_COUNT; msgIndex++) {
        for (uint8_t fieldIndex = 0; fieldIndex < 8; fieldIndex++) {
            if (error_map_[msgIndex][fieldIndex] == error) {
                error_node.setField(msgIndex, fieldIndex, 0x00);
                error_node.sendMsg(msgIndex, 2);
                #ifdef debugMode
                Serial.print("UTFR_ERROR::sendError: Set Error at Msg Idx ");
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