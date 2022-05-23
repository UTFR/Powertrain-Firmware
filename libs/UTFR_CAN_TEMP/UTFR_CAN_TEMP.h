/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "ArduinoSTL.h"       
#include "mcp_can.h"


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/

#ifndef _UTFR_CAN_TEMP_H_
#define _UTFR_CAN_TEMP_H_

//#define debugMode                         // Uncomment this line to enable debug prints


// ---------->> CHANGE THESE FOR EACH SPECIFIC IMPLEMENTATION OF THIS LIBRARY
// ----------------------------------------------------------------------------------------------------->>

#define _1_NODE_                            // Comment out for MCU that uses two nodes
//#define _2_NODES_                           // Comment out for MCU that uses only one node

#define UNUSED_F 0                          // Define your message data fields here so you can access them by name later

// THERM_BROADCAST 
#define THERM_BROADCAST_MODULE_NUM    1
#define THERM_BROADCAST_LOWEST_VAL    2
#define THERM_BROADCAST_HIGHEST_VAL   3
#define THERM_BROADCAST_AVG_VAL       4
#define THERM_BROADCAST_NUM_ENABLED   5
#define THERM_BROADCAST_HIGHEST_ID    6
#define THERM_BROADCAST_LOWEST_ID     7
#define THERM_BROADCAST_CHECKSUM      8

// THERM_GENERAL   
#define THERM_GENERAL_ID_MSB            1
#define THERM_GENERAL_ID_LSB            2
#define THERM_GENERAL_VAL               3
#define THERM_GENERAL_NUM_ENABLED       4
#define THERM_GENERAL_LOWEST_VAL        5
#define THERM_GENERAL_HIGHEST_VAL       6
#define THERM_GENERAL_HIGHEST_ID        7
#define THERM_GENERAL_LOWEST_ID         8           

enum CAN_msgNames_E                          // Define all CAN message names here so you can access them by name later                                    
{     
    CAN_MSG_THERM_BROADCAST,                // Thermistor to BMS Broadcast
    CAN_MSG_THERM_GENERAL,                  // Thermistor General Broadcase
    CAN_MSG_THERM_LEGACY,                   // Legacy Reserved field that's required for monitoring to work

    CAN_MSG_COUNT
};

// ----------------------------------------------------------------------------------------------------->>

class UTFR_CAN_TEMP
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
            volatile uint8_t msgData[8];            // 8-byte data payload. Volatile because can be changed in ISR.
            const uint8_t msgFields[8];             // What is each byte of the payload? --> Use defined field names at top of file
            const bool isTx;                        // true if message is transmitted
            const bool isRx;                        // true if message is received
            bool isDirty;                           // true when message in array contains new, un-read data
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

        CAN_msg_S _CAN_msgArray[CAN_MSG_COUNT] =            // Initialize CAN message array   
        {                                                   // Error messages need to be first to work with the CAN Library
            [CAN_MSG_THERM_BROADCAST] =         
            {
                .msgID = 0x1839F380, //TBD
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {THERM_BROADCAST_MODULE_NUM,  THERM_BROADCAST_LOWEST_VAL, 
                              THERM_BROADCAST_HIGHEST_VAL, THERM_BROADCAST_AVG_VAL,
                              THERM_BROADCAST_NUM_ENABLED,     THERM_BROADCAST_HIGHEST_ID,
                              THERM_BROADCAST_LOWEST_ID,     THERM_BROADCAST_CHECKSUM},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
        
            [CAN_MSG_THERM_GENERAL] =         
            {
                .msgID = 0x1838F380, //TBD
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {THERM_GENERAL_ID_MSB,  THERM_GENERAL_ID_LSB, 
                              THERM_GENERAL_VAL, THERM_GENERAL_NUM_ENABLED,
                              THERM_GENERAL_LOWEST_VAL,     THERM_GENERAL_HIGHEST_VAL,
                              THERM_GENERAL_HIGHEST_ID,     THERM_GENERAL_LOWEST_ID},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },

            [CAN_MSG_THERM_LEGACY] =         
            {
                .msgID = 0x80, //TBD
                .msgData = {0x05, 0x15, 0x00, 0x9E, 
                            0xFF, 0xFF, 0xFF, 0xFF}, //TODO - Test
                .msgFields = {UNUSED_F,  UNUSED_F, 
                              UNUSED_F, UNUSED_F,
                              UNUSED_F,     UNUSED_F,
                              UNUSED_F,     UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },

        };


        uint16_t _CAN_filterArray[CAN_MASK_FILTER_COUNT] =         // Define the filters you want to apply to incoming messages here
        {
            [CAN_MASK_0] = 0b00000000000,               // Applies to CAN_FILTER_0 and 1  (Receive buffer 0, RXB0)
            [CAN_MASK_1] = 0b00000000000,               // Applies to CAN_FILTER_2 to 5   (Recieve buffer 1, RXB1)

            [CAN_FILTER_0] = 0b00000000000,
            [CAN_FILTER_1] = 0b00000000000,
            [CAN_FILTER_2] = 0b00000000000,
            [CAN_FILTER_3] = 0b00000000000,
            [CAN_FILTER_4] = 0b00000000000,
            [CAN_FILTER_5] = 0b00000000000
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

        bool _sendMsgNow = false;


    /******************************************************************************
    *          P U B L I C   F U N C T I O N   D E C L A R A T I O N S            *
    ******************************************************************************/
    public:
        UTFR_CAN_TEMP(uint8_t CS1);                                                      // Constructor for one node
        UTFR_CAN_TEMP(uint8_t CS1, uint8_t CS2);                                         // Constructor for two nodes

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
        #endif

        unsigned long getField(CAN_msgNames_E msgName, uint8_t fieldName);          // Get data from field by name (defined at top of this file)
        void setField(CAN_msgNames_E msgName, uint8_t fieldName, long fieldData);   // Set field data by name, pass in data you want to set as well

        void printMsgData(CAN_msgNames_E msgName);  // Prints all data stored in a given message (Note: will be in decimal format)

        void readSensors(void);                     // Read all sensors this MCU is responsible for
	    void setMsgFreq(int freq);                  // Sets frequency to send messages at
                                                    // Note: Choose a freq that evenly divides into lower frequencies
                                                    // Example good choice: 60Hz, can choose to send other messages at 40Hz, 20Hz, 10Hz, etc.
                                                    // via if statements in msgSendISR

    /******************************************************************************
    *          P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
    ******************************************************************************/
    private:

        friend void msgSendISR(UTFR_CAN_TEMP& SELF);     // Sends messages when interrupt raised by HW timer 1
                                                    // Must be a friend because ISRs cannot be members of class
                                                    // They must have an ordinary function pointer (something about compiling idk)
                                                    
        void receiveMsgsCommonFcn(unsigned long canID, uint8_t dataLength, uint8_t buf[8]); // parts of receiveMsgs common to all nodes
};

#endif
