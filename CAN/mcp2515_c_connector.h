#ifndef _MCP2515_C_CONNECTOR_H_
#define _MCP2515_C_CONNECTOR_H_

#include "mcp2515.h"
#include "Arduino.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void *mcp2515_c;

// The below typedefs move enums outside of MCP2515 class for access in C

typedef enum {
    ERROR_OK_C        = 0,
    ERROR_FAIL_C      = 1,
    ERROR_ALLTXBUSY_C = 2,
    ERROR_FAILINIT_C  = 3,
    ERROR_FAILTX_C    = 4,
    ERROR_NOMSG_C     = 5,
    ERROR_NULLSELF_C  = 6,
} ERROR_C;

typedef enum {
    MASK0_C = 0,
    MASK1_C = 1,
} MASK_C;

typedef enum {
    RXF0_C = 0,
    RXF1_C = 1,
    RXF2_C = 2,
    RXF3_C = 3,
    RXF4_C = 4,
    RXF5_C = 5,
} RXF_C;

typedef enum {
    RXB0_C = 0,
    RXB1_C = 1,
} RXBn_C;

typedef enum {
    TXB0_C = 0,
    TXB1_C = 1,
    TXB2_C = 2,
} TXBn_C;

typedef enum {
    CANINTF_RX0IF_C = 0x01,
    CANINTF_RX1IF_C = 0x02,
    CANINTF_TX0IF_C = 0x04,
    CANINTF_TX1IF_C = 0x08,
    CANINTF_TX2IF_C = 0x10,
    CANINTF_ERRIF_C = 0x20,
    CANINTF_WAKIF_C = 0x40,
    CANINTF_MERRF_C = 0x80
} CANINTF_C;

typedef enum {
    EFLG_RX1OVR_C = (1<<7),
    EFLG_RX0OVR_C = (1<<6),
    EFLG_TXBO_C   = (1<<5),
    EFLG_TXEP_C   = (1<<4),
    EFLG_RXEP_C   = (1<<3),
    EFLG_TXWAR_C  = (1<<2),
    EFLG_RXWAR_C  = (1<<1),
    EFLG_EWARN_C  = (1<<0)
} EFLG_C; 


/*!
* @brief    Convert MCP2515 object pointer to void pointer.
*
* @param[in,out]    self_cpp       Pointer to instance of MCP2515 class.
*
* @return        void*          Opaque/untyped pointer to instance of MCP2515 class for use by C API wrappers.
*/
void* mcp2515_c_init(MCP2515 *self_cpp);


/*!
* @brief    Reset the MCP2515.
*
* @param[in,out]    self_c         Void pointer to instance of MCP2515 class
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_reset(mcp2515_c self_c);


/*!
* @brief    Put the MCP2515 in config mode. Must be in config mode to change bit
*           timing parameters, configure filters, or change TXnRTS pin assignment.
*           This is the default mode upon power-up/reset.
*
* @param[in,out]    self_c         Void pointer to instance of MCP2515 class
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setConfigMode(mcp2515_c self_c);


/*!
* @brief    Put the MCP2515 in Listen Only mode. All messages are recieved in this
*           mode regardless of filter settings. Useful for bus monitoring or automatic
*           baud rate detection. Nothing is transmitted onto the bus in this mode.
*
* @param[in,out]    self_c         Void pointer to instance of MCP2515 class
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setListenOnlyMode(mcp2515_c self_c);


/*!
* @brief    Put the MCP2515 in Sleep mode. Enter this mode to save power. SPI 
*           interface is still active and registers can still be read in this mode.
*
* @param[in,out]    self_c         Void pointer to instance of MCP2515 class
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setSleepMode(mcp2515_c self_c);


/*!
* @brief    Put the MCP2515 in Loopback mode. Allows messages to be sent internally
*           from Tx buffers to Rx buffers. Nothing is transmitted to the bus in this mode.
*           Useful for testing and debug.
*
* @param[in,out]    self_c         Void pointer to instance of MCP2515 class
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setLoopbackMode(mcp2515_c self_c);


/*!
* @brief    Put the MCP2515 in Normal mode. This is the standard operating mode where
*           the device actively monitors the bus, generates acknowledge bits, error frames,
*           etc. This is the only mode in which the MCP2515 will transmit messages onto the bus.
*
* @param[in,out]    self_c         Void pointer to instance of MCP2515 class
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setNormalMode(mcp2515_c self_c);


/*!
* @brief    Set prescaler divide ratio for CLKOUT signal.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        divisor    Default is divide-by-8. Maximum clock freq is 
*                              25MHz (at divide-by-1). @see CAN_CLKOUT for options
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setClkOut(mcp2515_c self_c, const CAN_CLKOUT divisor);


/*!
* @brief    Set CAN communication bitrate.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        canSpeed   Bitrate @see CAN_SPEED for options
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setBitrate(mcp2515_c self_c, const CAN_SPEED canSpeed);


/*!
* @brief    Set CAN communication bitrate and internal clock frequency.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        canSpeed   CAN communication bitrate @see CAN_SPEED for options
* @param[in]        canClock   Internal MCU clock frequency @see CAN_CLOCK for options
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setBitrateAndClock(mcp2515_c self_c, const CAN_SPEED canSpeed, const CAN_CLOCK canClock);


/*!
* @brief    Set specified filter mask. This mask sets which bits of the filter are actually 
*           considered when choosing whether to accept a message.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        num        MASK0: Rx buffer 0, MASK1: Rx buffer 1    
* @param[in]        ext        True - Mask for extended message, False - Mask for normal message
* @param[in]        ulData     Mask @see Table 4-2 in datasheet
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setFilterMask(mcp2515_c self_c, const MASK_C num, const bool ext, const uint32_t ulData);


/*!
* @brief    Set specified filter.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        num        RXF0 and RXF1 apply to RXB0. RXF2, 3, 4, 5, apply to RXB1
*                              @see RXF for details
* @param[in]        ext        True - Filter for extended message, False - Filter for normal message
* @param[in]        ulData     Mask @see Table 4-2 in datasheet
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_setFilter(mcp2515_c self_c, const RXF_C num, const bool ext, const uint32_t ulData);


/*!
* @brief    Send message from a specified Tx buffer.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        txbn       Specify Tx buffer (TXB0, TXB1 or TXB2)
* @param[in,out]    frame      Can message content to be sent
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_sendMessageFrom(mcp2515_c self_c, const TXBn_C txbn, const struct can_frame *frame);


/*!
* @brief    Send message from next empty Tx buffer. Lowest numbered empty buffer will be chosen first.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in,out]    frame      Can message content to be sent
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_sendMessage(mcp2515_c self_c, const struct can_frame *frame);


/*!
* @brief    Read message from a specified Rx buffer.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[in]        rxbn       Specify Rx buffer (RXB0 or RXB1)
* @param[out]       frame      Store read message data here
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_readMessageFrom(mcp2515_c self_c, const RXBn_C rxbn, struct can_frame *frame);


/*!
* @brief    Read message from next Rx buffer with message. RXB0 has priority over RXB1.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* @param[out]       frame      Store read message data here
*
* @return       ERROR_OK_C     Successful API return.
* @return       >0             Unsuccessful API call @see ERROR_C for details.
*/
ERROR_C mcp2515_c_readMessage(mcp2515_c self_c, struct can_frame *frame);


/*!
* @brief    Gets RX buffer status.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
* @return       True           At least one RX buffer has a message.
* @return       False          No message in either RX buffer.
*/
bool mcp2515_c_checkReceive(mcp2515_c self_c);


/*!
* @brief    Checks if any error flags are raised.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
* @return       True           At least one error flag is raised.
* @return       False          No error flags raised.
*/
bool mcp2515_c_checkError(mcp2515_c self_c);


/*!
* @brief    Gets error status flags from MCP_EFLG register (0x2D). 
*           See page 50 of datasheet for details.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
* @return   8 bit register data (1 = bit set, 0 = bit cleared):
*               bit 0 - Error warning (Set if RX Error Count or TX Error Count > 96),
*               bit 1 - RX warning (Set if REC > 96),
*               bit 2 - TX warning (Set if TEC > 96),
*               bit 3 - RX enter passive mode (REC > 128, sent error msgs are now fully reccessive),
*               bit 4 - TX enter passive mode (TEC > 128, sent error msgs are now fully reccessive),
*               bit 5 - Bus-off error bit (TEC > 255, TX off until bus-off recovery sequence),
*               bit 6 - Rx buffer 0 overflow,
*               bit 7 - Rx buffer 1 overflow
*/
uint8_t mcp2515_c_getErrorFlags(mcp2515_c self_c);


/*!
* @brief    Clears Rx buffer overflow flag bits in MCP_EFLG register.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
*/
void mcp2515_c_clearRXnOVRFlags(mcp2515_c self_c);


/*!
* @brief    Gets interrupt flags from MCP_CANINTF register (0x2C).
*           See page 54 of datasheet for details.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
* @return   8 bit register data (1 = interrupt pending and must be cleared by MCU, 
*                                0 = no interrupt pending):
*               bit 0 - 1 when RXB0 full,
*               bit 1 - 1 when RXB1 full,
*               bit 2 - 1 when TXB0 empty,
*               bit 3 - 1 when TXB1 empty,
*               bit 4 - 1 when TXB2 empty,
*               bit 5 - 1 when EFLG reg raises flag which needs interrupt to resolve,
*               bit 6 - Wake-up interrupt flag,
*               bit 7 - Message error interrupt flag.      
*/
uint8_t mcp2515_c_getInterrupts(mcp2515_c self_c);


/*!
* @brief    Gets interrupt enable bits from MCP_CANINTE register (0x2B).
*           See page 53 of datasheet for details.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
* @return   8 bit register data (1 = interrupt type enabled, 0 = disabled):
*               bit 0 - RXB0 full,
*               bit 1 - RXB1 full,
*               bit 2 - TXB0 empty,
*               bit 3 - TXB1 empty,
*               bit 4 - TXB2 empty,
*               bit 5 - EFLG reg raises flag which needs interrupt to resolve,
*               bit 6 - Wake-up interrupt flag,
*               bit 7 - Message error interrupt flag.      
*/
uint8_t mcp2515_c_getInterruptMask(mcp2515_c self_c);


/*!
* @brief    Tries to clear interrupt flags from MCP_CANINTF register.
*           A flag cannot be cleared if respective condition prevails.
*           WARNING: Uses WRITE instruction which can cause others interrupts 
*           to be deleted/missed, therfore only call in non-critical sections.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*    
*/
void mcp2515_c_clearInterrupts(mcp2515_c self_c);


/*!
* @brief    Tries to clear TXBn interrupt flags. Uses BITMOD instruction 
*           therefore will not cause other interrupts to be deleted/missed.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*    
*/
void mcp2515_c_clearTXInterrupts(mcp2515_c self_c);


/*!
* @brief    Reads useful status bits.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* 
* @return   8 bits of useful data (1 = bit set, 0 = bit cleared):
*               bit 0 - RXB0 full (CANINTF[0]),
*               bit 1 - RXB1 full (CANINTF[1]),
*               bit 2 - TXB0 TX request bit (TXB0CNTRL[3]),
*               bit 3 - TXB0 empty (CANINTF[2]), 
*               bit 4 - TXB1 TX request bit (TXB1CNTRL[3]),
*               bit 5 - TXB1 empty (CANINTF[3]),
*               bit 6 - TXB2 TX request bit (TXB2CNTRL[3]),
*               bit 7 - TXB2 empty (CANINTF[4]). 
*/
uint8_t mcp2515_c_getStatus(mcp2515_c self_c);


/*!
* @brief    Tries to clear RX overflow error flags (MCP_EFLG[6] and 
*           MCP_EFLG[7]). Uses BITMOD instruction which will not cause 
*           other interrupts to be deleted/missed.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*    
*/
void mcp2515_c_clearRXnOVR(mcp2515_c self_c);


/*!
* @brief    Tries to clear message error interrupt flag (MCP_CANINTF[7]). Uses 
*           BITMOD instruction which will not cause other interrupts to be 
*           deleted/missed.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*    
*/
void mcp2515_c_clearMERR(mcp2515_c self_c);


/*!
* @brief    Tries to clear error interrupt flag (MCP_CANINTF[5]). Uses BITMOD 
*           instruction which will not cause other interrupts to be deleted/missed.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*    
*/
void mcp2515_c_clearERRIF(mcp2515_c self_c);


/*!
* @brief    Gets RX error count (MCP_REC reg).
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* 
* @return   RX error count.
*/
uint8_t mcp2515_c_errorCountRX(mcp2515_c self_c);


/*!
* @brief    Gets TX error count (MCP_TEC reg).
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
* 
* @return   TX error count.
*/
uint8_t mcp2515_c_errorCountTX(mcp2515_c self_c);


/*!
* @brief    Gets error flags (MCP_EFLG) and prints them in readable form to serial.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
*/
void mcp2515_c_printErrorFlags(mcp2515_c self_c);


/*!
* @brief    Gets interrupt flags (MCP_CANINTF) and prints them in readable form to serial.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
*/
void mcp2515_c_printInterrupts(mcp2515_c self_c);


/*!
* @brief    Gets interrupt mask (MCP_CANINTE) and prints it in readable form to serial.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
*/
void mcp2515_c_printInterruptMask(mcp2515_c self_c);


/*!
* @brief    Gets MCP2515 status and prints it in readable form to serial.
*
* @param[in,out]    self_c     Void pointer to instance of MCP2515 class
*
*/
void mcp2515_c_printStatus(mcp2515_c self_c);


#ifdef __cplusplus
}
#endif

#endif