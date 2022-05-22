/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "ArduinoSTL.h"
#include "mcp_can.h"       


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/

#ifndef _UTFR_CAN_ACM_H_
#define _UTFR_CAN_ACM_H_

//#define debugMode                         // Uncomment this line to enable debug prints


// ---------->> CHANGE THESE FOR EACH SPECIFIC IMPLEMENTATION OF THIS LIBRARY
// ----------------------------------------------------------------------------------------------------->>

//#define _1_NODE_                            // Comment out for MCU that uses two nodes
#define _2_NODES_                         // Comment out for MCU that uses only one node

#define UNUSED_F 0                          // Define your message data fields here so you can access them by name later

// BMS0 - BMS Data Logging Message #1 (Logging)
#define BMS_CURRENT_F    1                  // Instantaneous pack current
#define BMS_VOLTAGE_F    2                  // Instananeous pack voltage
#define BMS_SOC_F        3                  // Pack state-of-charge
#define BMS_HIGHTEMP_F   4                  // Highest temp in pack
#define BMS_CHECKSUM_1_F 5

// BMS1 - BMS DTC Flags Message (Critical)
#define BMS_DTC_1_F         1               // BMS fault flags #1
#define BMS_DTC_2_F         2               // BMS fault flags #2
#define BMS_ROLLING_COUNT_F 3               // Increments by 1 every 100ms until 255 then resets to zero
#define BMS_CHECKSUM_2_F    4

// BMS2 - BMS Heartbeat Message (Critical)
// No settable fields in here


enum CAN_msgNames_E                          // Define all CAN message names here so you can access them by name later                                    
{                                          
    CAN_MSG_BMS_LOG,
    CAN_MSG_BMS_FAULTS,
    CAN_MSG_BMS_HEARTBEAT,    
    
    CAN_MSG_COUNT
};

enum errorNames_E {                         // Define all error types here
  ERR_NONE,
};

const uint8_t error_map[2][8] =             // using 0xFF to represent empty fields, 0x00 to represent active error
{ 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};
// ----------------------------------------------------------------------------------------------------->>

class UTFR_CAN_ACM
{   

    private:

        MCP_CAN _NODE1;                              // 1st CAN NODE object from "mcp_can.h"
        MCP_CAN _NODE2;                              // 2nd CAN NODE object from "mcp_can.h"

        uint8_t _CS1;                               // Chip Select Pin for node 1 (Initialized by constructor)
        uint8_t _CS2;                               // Chip Select Pin for node 2 (Initialized by constructor)
        uint8_t _busSpeed;                          // bus speed, pre-defined values in "mcp_can_dfs.h"
        bool _ext = false;                          // Extended format? 11-bit ID for normal format, 29-bit ID for extended

        struct CAN_msg_S                            // Don't change this. Structure of a single CAN message.
        {                      
            const unsigned long msgID;              // 11-bit identifier for normal format, 29-bit identifier for extended format
            volatile uint8_t msgData[8];            // 8-byte data payload
            const uint8_t msgFields[8];             // What is each byte of the payload? --> Use defined field names at top of file
            const bool isTx;                        // true if message is transmitted
            const bool isRx;                        // true if message is received
            volatile bool isDirty;                  // true if new data received since last read
        };

        enum CAN_filter_E                           // Don't change this. It's the same for all mcp2515 nodes.
        {                           
            CAN_MASK_0,         
            CAN_MASK_1,         
                                
            CAN_FILTER_0,  
            CAN_FILTER_1,       
            CAN_FILTER_2,     
            CAN_FILTER_3,      
            CAN_FILTER_4,       
            CAN_FILTER_5,      
                                
            CAN_MASK_FILTER_COUNT
        };


        // ---------->> CHANGE THESE FOR EACH SPECIFIC IMPLEMENTATION OF THIS LIBRARY
        // ----------------------------------------------------------------------------------------------------->>

        CAN_msg_S _CAN_msgArray[CAN_MSG_COUNT] =                        // Initialize CAN message array   
        {
            [CAN_MSG_BMS_LOG] = 
            {
                .msgID = 0x100, 
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {BMS_CURRENT_F,  BMS_CURRENT_F, 
                              BMS_VOLTAGE_F,  BMS_VOLTAGE_F,
                              BMS_SOC_F,      BMS_HIGHTEMP_F,
                              BMS_HIGHTEMP_F, BMS_CHECKSUM_1_F},
                .isTx = true,
                .isRx = true,
                .isDirty = false,
            },
            [CAN_MSG_BMS_FAULTS] =
            {
                .msgID = 0x101,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {BMS_DTC_1_F,          BMS_DTC_2_F, 
                              BMS_ROLLING_COUNT_F,  UNUSED_F,
                              UNUSED_F,             UNUSED_F,
                              UNUSED_F,             BMS_CHECKSUM_2_F},
                .isTx = false,
                .isRx = true,
                .isDirty = false,
            },
            [CAN_MSG_BMS_HEARTBEAT] =
            {
                .msgID = 0x102,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {UNUSED_F, UNUSED_F, 
                              UNUSED_F, UNUSED_F,
                              UNUSED_F, UNUSED_F,
                              UNUSED_F, UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
        };


        unsigned long _CAN_filterArray[CAN_MASK_FILTER_COUNT] =         // Define the filters you want to apply to incoming messages here
        {
            // Configured to accept only the inverter internal state message
            [CAN_MASK_0] = 0b11111111111,             // Applies to CAN_FILTER_0 and 1  (Receive buffer 0, RXB0)
            [CAN_MASK_1] = 0b11111111111,             // Applies to CAN_FILTER_2 to 5   (Recieve buffer 1, RXB1)

            [CAN_FILTER_0] = 0x100,                   // Current set up only for BMS error flag message and BMS logging data
            [CAN_FILTER_1] = 0x101,
            [CAN_FILTER_2] = 0x100,
            [CAN_FILTER_3] = 0x101,
            [CAN_FILTER_4] = 0x100,
            [CAN_FILTER_5] = 0x101
        }; 

        //    ----------------- Filter-Mask Truth Table ------------------------------    
        //    ------------------------------------------------------------------------
        //    |  Mask bit n  |  Filter bit n  |  Msg ID bit n |  Accept/Reject bit n |
        //    ------------------------------------------------------------------------
        //    |      0       |        x       |       x       |        Accept        |
        //    |      1       |        0       |       0       |        Accept        |
        //    |      1       |        0       |       1       |        Reject        |
        //    |      1       |        1       |       0       |        Reject        |
        //    |      1       |        1       |       1       |        Accept        |
        //    ------------------------------------------------------------------------
        //      **Note: All bits of ID must be accepted for message to be received
        //       (Table from page 33 of MCP2515 datasheet)


        // ----------------------------------------------------------------------------------------------------->>


    /******************************************************************************
    *          P U B L I C   F U N C T I O N   D E C L A R A T I O N S            *
    ******************************************************************************/
    public:
        UTFR_CAN_ACM(uint8_t CS1);                                                      // Constructor for one node
        UTFR_CAN_ACM(uint8_t CS1, uint8_t CS2);                                         // Constructor for two nodes

        #ifdef _1_NODE_
        void begin(uint8_t busSpeed);                                               // Initialize CAN node
        void sendAll(void);                                                         // Send all messages in _CAN_msgArray where .isTx == true
        void sendMsg(CAN_msgNames_E msgName);                                       // Send message by name
        void receiveMsgs(void);                                                     // get all incoming messages from Rx buffers and update _CAN_msgArray
        void setFilters(void);                                              // set masks and filters according to CAN_filterArray
        void setFilters_permitAll(void);                                    // set masks and filters to receive ALL messages sent by other nodes
        void setFilters_permitNone(void);                                   // set masks and filters to receive NO messages sent by other nodes
        #endif

        #ifdef _2_NODES_
        void begin(uint8_t busSpeed, uint8_t nodeNumber);                           // Initialize CAN node
                                                                                    // nodeNumber: 1 for node 1, 2 for node 2, 3 for both nodes
        void sendAll(uint8_t nodeNumber);                                           // Send all messages in _CAN_msgArray where .isTx == true
        void sendMsg(CAN_msgNames_E msgName, uint8_t nodeNumber);                   // Send message by name
        void receiveMsgs(uint8_t nodeNumber);                                       // get all incoming messages from Rx buffers and update _CAN_msgArray
        void setFilters(uint8_t nodeNumber);                                        // set masks and filters according to CAN_filterArray
        void setFilters_permitAll(uint8_t nodeNumber);                              // set masks and filters to receive ALL messages sent by other nodes
        void setFilters_permitNone(uint8_t nodeNumber);                             // set masks and filters to receive NO messages sent by other nodes
        void sendError(errorNames_E error);                                         // Sends error message to datalogger.
        #endif
 

        unsigned long getField(CAN_msgNames_E msgName, uint8_t fieldName);          // Get data from field by name (defined at top of this file)
        void setField(CAN_msgNames_E msgName, uint8_t fieldName, long fieldData);   // Set field data by name, pass in data you want to set as well
        void printMsgData(CAN_msgNames_E msgName);                                  // Prints all data stored in a given message (Note: will be in decimal format)
        bool msgDirty(CAN_msgNames_E msgName);                                      // Checks if this message has been received since last getField


    /******************************************************************************
     *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
     *****************************************************************************/
    private:
        void receiveMsgsCommonFcn(unsigned long canID, uint8_t dataLength, uint8_t buf[8]); // parts of receiveMsgs common to all nodes
};

#endif