#include "mcp2515_c_connector.h"
#include "mcp2515.h"
#include "Arduino.h"


void* mcp2515_c_init(MCP2515 *self_cpp) {
    return static_cast<void*>(self_cpp);
} 


ERROR_C mcp2515_c_reset(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->reset());
}


ERROR_C mcp2515_c_setListenOnlyMode(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setListenOnlyMode());
}


ERROR_C mcp2515_c_setSleepMode(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setSleepMode());
}


ERROR_C mcp2515_c_setLoopbackMode(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setLoopbackMode());
}


ERROR_C mcp2515_c_setNormalMode(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setNormalMode());
}


ERROR_C mcp2515_c_setClkOut(mcp2515_c self_c, const CAN_CLKOUT divisor) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setClkOut(divisor));
}


ERROR_C mcp2515_c_setBitrate(mcp2515_c self_c, enum CAN_SPEED canSpeed) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setBitrate(canSpeed));
}


ERROR_C mcp2515_c_setBitrateAndClock(mcp2515_c self_c, enum CAN_SPEED canSpeed, const CAN_CLOCK canClock) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->setBitrate(canSpeed, canClock));
}


ERROR_C mcp2515_c_setFilterMask(mcp2515_c self_c, const MASK_C num, const bool ext, const uint32_t ulData) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    MCP2515::MASK num_cpp = static_cast<MCP2515::MASK>(num);
    
    return static_cast<ERROR_C>(self_cpp->setFilterMask(num_cpp, ext, ulData));
}


ERROR_C mcp2515_c_setFilter(mcp2515_c self_c, const RXF_C num, const bool ext, const uint32_t ulData) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    MCP2515::RXF num_cpp = static_cast<MCP2515::RXF>(num);

    return static_cast<ERROR_C>(self_cpp->setFilter(num_cpp, ext, ulData));
}


ERROR_C mcp2515_c_sendMessageFrom(mcp2515_c self_c, const TXBn_C txbn, const struct can_frame *frame) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    MCP2515::TXBn txbn_cpp = static_cast<MCP2515::TXBn>(txbn);

    return static_cast<ERROR_C>(self_cpp->sendMessage(txbn_cpp, frame));
}


ERROR_C mcp2515_c_sendMessage(mcp2515_c self_c, const struct can_frame *frame) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->sendMessage(frame));
}


ERROR_C mcp2515_c_readMessageFrom(mcp2515_c self_c, const RXBn_C rxbn, struct can_frame *frame) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    MCP2515::RXBn rxbn_cpp = static_cast<MCP2515::RXBn>(rxbn);
    return static_cast<ERROR_C>(self_cpp->readMessage(rxbn_cpp, frame));
}


ERROR_C mcp2515_c_readMessage(mcp2515_c self_c, struct can_frame *frame) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return static_cast<ERROR_C>(self_cpp->readMessage(frame));
}


bool mcp2515_c_checkReceive(mcp2515_c self_c) {

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->checkReceive();
}


bool mcp2515_c_checkError(mcp2515_c self_c) {

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->checkError();
}


uint8_t mcp2515_c_getErrorFlags(mcp2515_c self_c) {

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->getErrorFlags();
}


void mcp2515_c_clearRXnOVRFlags(mcp2515_c self_c) {

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->clearRXnOVRFlags();
}


uint8_t mcp2515_c_getInterrupts(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->getInterrupts();
}


uint8_t mcp2515_c_getInterruptMask(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->getInterruptMask();
}


void mcp2515_c_clearInterrupts(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->clearInterrupts();
}


void mcp2515_c_clearTXInterrupts(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->clearTXInterrupts();
}


uint8_t mcp2515_c_getStatus(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->getStatus();
}


void mcp2515_c_clearRXnOVR(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->clearRXnOVR();
}

void mcp2515_c_clearMERR(mcp2515_c self_c) {

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->clearMERR();
}

void mcp2515_c_clearERRIF(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->clearERRIF();
}

uint8_t mcp2515_c_errorCountRX(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->errorCountRX();
}

uint8_t mcp2515_c_errorCountTX(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);
    return self_cpp->errorCountTX();
}

void mcp2515_c_printErrorFlags(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);

    uint8_t flags;
    flags = self_cpp->getErrorFlags();

    Serial.write("################### ERROR FLAGS ###################\n");
    // if rx or tx error warning
    if (flags & EFLG_EWARN_C){
        // if rx error warning
        if (flags & EFLG_RXWAR_C){
            // if rx errors caused passive mode to be entered
            if (flags & EFLG_RXEP_C){
                Serial.write("RX errors > 128. MCP2515 is error-passive.\n");
            }
            else{
                 Serial.write("RX errors > 96. RX error warning active.\n");
            }
        }
        // if tx error warning
        if (flags & EFLG_TXWAR_C){
            // if tx errors caused passive mode to be entered
            if (flags & EFLG_TXEP_C){
                // if tx errors caused bus-off mode to be entered
                if (flags & EFLG_TXBO_C){
                    Serial.write("TX errors > 256. MCP2515 is in BUS-OFF state.\n");
                }
                else{
                    Serial.write("TX errors > 128. MCP2515 is error-passive.\n");
                }
            }
            else{
                Serial.write("TX errors > 96. TX error warning active.\n");
            }   
        }
    }

    if (flags & EFLG_RX0OVR_C){
        Serial.write("RX buffer 0 overflow.\n");
    }
    if (flags & EFLG_RX1OVR_C){
        Serial.write("RX buffer 1 overflow.\n");
    }
    Serial.write("###################################################\n");
}

void mcp2515_c_printInterrupts(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);

    uint8_t flags;
    flags = self_cpp->getInterrupts();

    Serial.write("################# INTERRUPT FLAGS #################\n");
    if (flags & CANINTF_RX0IF_C){
        Serial.write("RX buffer 0 full, waiting for interrupt service.\n");
    }
    if (flags & CANINTF_RX1IF_C){
        Serial.write("RX buffer 1 full, waiting for interrupt service.\n");
    }
    if (flags & CANINTF_TX0IF_C){
        Serial.write("TX buffer 0 empty, waiting for interrupt service.\n");
    }
    if (flags & CANINTF_TX1IF_C){
        Serial.write("TX buffer 1 empty, waiting for interrupt service.\n");
    }
    if (flags & CANINTF_TX2IF_C){
        Serial.write("TX buffer 2 empty, waiting for interrupt service.\n");
    }
    if (flags & CANINTF_ERRIF_C){
        Serial.write("Error flag waiting for interrupt service.\n");
    }
    if (flags & CANINTF_WAKIF_C){
        Serial.write("Wake-up flag waiting for interrupt service.\n");
    }
    if (flags & CANINTF_MERRF_C){
        Serial.write("Message error flag waiting for interrupt service.\n");
    }
    Serial.write("###################################################\n");
}

void mcp2515_c_printInterruptMask(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);

    uint8_t mask;
    mask = self_cpp->getInterruptMask();

    Serial.write("################# INTERRUPT MASK #################\n");
    if (mask & CANINTF_RX0IF_C){
        Serial.write("RX buffer 0 full interrupts:  ENABLED\n");
    }
    else{
        Serial.write("RX buffer 0 full interrupts:  DISABLED\n");
    }
    if (mask & CANINTF_RX1IF_C){
        Serial.write("RX buffer 1 full interrupts:  ENABLED\n");
    }
    else{
        Serial.write("RX buffer 1 full interrupts:  DISABLED\n");
    }
    if (mask & CANINTF_TX0IF_C){
        Serial.write("TX buffer 0 empty interrupts: ENABLED\n");
    }
    else{
        Serial.write("TX buffer 0 empty interrupts: DISABLED\n");
    }
    if (mask & CANINTF_TX1IF_C){
        Serial.write("TX buffer 1 empty interrupts: ENABLED\n");
    }
    else{
        Serial.write("TX buffer 1 empty interrupts: DISABLED\n");
    }
    if (mask & CANINTF_TX2IF_C){
        Serial.write("TX buffer 2 empty interrupts: ENABLED\n");
    }
    else{
        Serial.write("TX buffer 2 empty interrupts: DISABLED\n");
    }
    if (mask & CANINTF_ERRIF_C){
        Serial.write("Error flag interrupts:        ENABLED\n");
    }
    else{
        Serial.write("Error flag interrupts:        DISABLED\n");
    }
    if (mask & CANINTF_WAKIF_C){
        Serial.write("Wake-up interrupts:           ENABLED\n");
    }
    else{
        Serial.write("Wake-up interrupts:           DISABLED\n");
    }
    if (mask & CANINTF_MERRF_C){
        Serial.write("Message error interrupts:     ENABLED\n");
    }
    else{
        Serial.write("Message error interrupts:     DISABLED\n");
    }
    Serial.write("###################################################\n");

}

void mcp2515_c_printStatus(mcp2515_c self_c){

    if (self_c == nullptr) {
        return ERROR_NULLSELF_C;
    }

    MCP2515* self_cpp = static_cast<MCP2515*>(self_c);

    uint8_t status;
    status = self_cpp->getStatus();

    Serial.write("################# MCP2515 STATUS #################\n");
    if (status & 0x01){
        Serial.write("RX buffer 0 full, waiting for MCU to read message.\n");
    }
    if (status & 0x02){
        Serial.write("RX buffer 1 full, waiting for MCU to read message.\n");
    }
    if (status & 0x04){
        Serial.write("Message in TX buffer 0, waiting for free bus to send.\n");
    }
    if (status & 0x08){
        Serial.write("TX buffer 0 empty, available for sending a message.\n");
    }
    if (status & 0x10){
        Serial.write("Message in TX buffer 1, waiting for free bus to send.\n");
    }
    if (status & 0x20){
        Serial.write("TX buffer 1 empty, available for sending a message.\n");
    }
    if (status & 0x40){
        Serial.write("Message in TX buffer 2, waiting for free bus to send.\n");
    }
    if (status & 0x80){
        Serial.write("TX buffer 2 empty, available for sending a message.\n");
    }
    Serial.write("##################################################\n");
}