#ifndef _MCP2515_C_CONNECTOR_H_
#define _MCP2515_C_CONNECTOR_H_

#include "mcp2515.h"


typedef void* mcp2515_c;

#ifdef __cplusplus
extern "C" {
#endif
 
mcp2515_c mcp2515_c_init(const uint8_t CS);
void mcp2515_c_destroy(mcp2515_c device);
int mcp2515_c_reset(mcp2515_c device);
int mcp2515_c_setBitrate(mcp2515_c device, enum CAN_SPEED canSpeed);
int mcp2515_c_setNormalMode(mcp2515_c device);
int mcp2515_c_sendMessage(mcp2515_c device, const struct can_frame *frame);
int mcp2515_c_readMessage(mcp2515_c device, struct can_frame *frame);

#ifdef __cplusplus
}
#endif

#endif