#include "mcp2515_c_connector.h"
#include "mcp2515.h"


mcp2515_c mcp2515_c_init(const uint8_t CS) {
    return new MCP2515(CS);
}

void mcp2515_c_destroy(mcp2515_c untyped_self) {
    MCP2515 *typed_self = static_cast<MCP2515*>(untyped_self);
    delete typed_self;
}

int mcp2515_c_reset(mcp2515_c untyped_self) {
    MCP2515* typed_self = static_cast<MCP2515*>(untyped_self);
    return typed_self->reset();
}

int mcp2515_c_setBitrate(mcp2515_c untyped_self, enum CAN_SPEED canSpeed) {
    MCP2515* typed_self = static_cast<MCP2515*>(untyped_self);
    return typed_self->setBitrate(canSpeed);
}

int mcp2515_c_setNormalMode(mcp2515_c untyped_self) {
    MCP2515* typed_self = static_cast<MCP2515*>(untyped_self);
    return typed_self->setNormalMode();
}

int mcp2515_c_sendMessage(mcp2515_c untyped_self, const struct can_frame *frame) {
    MCP2515* typed_self = static_cast<MCP2515*>(untyped_self);
    return typed_self->sendMessage(frame);
}

int mcp2515_c_readMessage(mcp2515_c untyped_self, struct can_frame *frame) {
    MCP2515* typed_self = static_cast<MCP2515*>(untyped_self);
    return typed_self->readMessage(frame);
}
