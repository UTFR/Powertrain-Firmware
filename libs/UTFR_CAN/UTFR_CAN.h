/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "ArduinoSTL.h"       
#include "mcp_can.h"


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/

#ifndef _UTFR_CAN_H_
#define _UTFR_CAN_H_

//#define debugMode                         // Uncomment this line to enable debug prints


//Ports for RF0 sensors                     // TO DO: REMOVE THIS WHEN PIN DRIVER ADDED
#define HW_PIN_RF_OUT_TIRE_TEMP  A0
#define HW_PIN_RF_CTR_TIRE_TEMP  A1
#define HW_PIN_RF_INR_TIRE_TEMP  A2
#define HW_PIN_RF_ROTOR_TEMP A3
//Ports for RF1 Sensors
#define HW_PIN_RF_DAMPER_POT  A4
#define HW_PIN_FW_STRAIN_GAUGE_TIP A5
#define HW_PIN_STEERING_ANGLE  A6


// ---------->> CHANGE THESE FOR EACH SPECIFIC IMPLEMENTATION OF THIS LIBRARY
// ----------------------------------------------------------------------------------------------------->>

#define _1_NODE_                            // Comment out for MCU that uses two nodes
//#define _2_NODES_                           // Comment out for MCU that uses only one node

#define UNUSED_F 0                          // Define your message data fields here so you can access them by name later

// RF0 - Right Front corner module CAN message 0
#define RF_OUT_TIRE_TEMP_F  1               // 1st field
#define RF_CTR_TIRE_TEMP_F  2               // 2nd field
#define RF_INR_TIRE_TEMP_F  3               // etc...
#define RF_ROTOR_TEMP_F     4

// RF1 - Right Front corner module CAN message 1
#define RF_DAMP_POT_F       1
#define FW_STRAIN_TIP_F     2
#define STEER_ANG_F         3

// ER0 - Error CAN Message 0
#define ER_BMS_OVERTEMP_F   1
#define ER_BMS_OVERVOLT_F   2
#define ER_BMS_UNDERVOLT_F  3

// ER1 - Error CAN Message 1
#define ER_APPS_MISMATCH    1
#define ER_APPS_OUTPUT      2
#define ER_SDC_TRIPPED      3

// Inverter CAN Messages
#define INV_VSM_STATE_F                 1
#define INV_PWM_FREQ_F                  2
#define INV_INV_STATE_F                 3
#define INV_REALY_STATE_F               4
#define INV_RUN_MODE_DISCHARGE_STATE_F  5

enum CAN_msgNames_E                          // Define all CAN message names here so you can access them by name later                                    
        {     
            CAN_MSG_ER0,
            CAN_MSG_ER1,                                      
            CAN_MSG_RF0,  
            CAN_MSG_RF1,
            CAN_MSG_INV_INTERNAL_STATE,

    CAN_MSG_COUNT
};

// ----------------------------------------------------------------------------------------------------->>

class UTFR_CAN
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
            [CAN_MSG_ER0] =         
            {
                .msgID = 0x1B2, //TBD
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {ER_BMS_OVERTEMP_F,  ER_BMS_OVERVOLT_F, 
                              ER_BMS_UNDERVOLT_F, UNUSED_F,
                              UNUSED_F,     UNUSED_F,
                              UNUSED_F,     UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_ER1] = 
            {
                .msgID = 0x1B3, //TBD
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {ER_APPS_MISMATCH,  ER_APPS_OUTPUT, 
                              ER_SDC_TRIPPED,     UNUSED_F,
                              UNUSED_F,     UNUSED_F,
                              UNUSED_F,     UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_RF0] = 
            {
                .msgID = 0x1B0,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF,                         
                            0xFF, 0xFF, 0xFF, 0xFF},                
                .msgFields = {RF_OUT_TIRE_TEMP_F, RF_OUT_TIRE_TEMP_F, 
                              RF_CTR_TIRE_TEMP_F, RF_CTR_TIRE_TEMP_F,
                              RF_INR_TIRE_TEMP_F, RF_INR_TIRE_TEMP_F,
                              RF_ROTOR_TEMP_F,    RF_ROTOR_TEMP_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_RF1] = 
            {
                .msgID = 0x1B1,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {RF_DAMP_POT_F,   RF_DAMP_POT_F, 
                              FW_STRAIN_TIP_F, FW_STRAIN_TIP_F,
                              STEER_ANG_F,     STEER_ANG_F,
                              UNUSED_F,        UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_INV_INTERNAL_STATE] = 
            {
                .msgID = 0x0AA,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {INV_VSM_STATE_F,   INV_PWM_FREQ_F, 
                              INV_INV_STATE_F, INV_REALY_STATE_F,
                              INV_RUN_MODE_DISCHARGE_STATE_F,     UNUSED_F,
                              UNUSED_F,        UNUSED_F},
                .isTx = false,
                .isRx = true,
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
        UTFR_CAN(uint8_t CS1);                                                      // Constructor for one node
        UTFR_CAN(uint8_t CS1, uint8_t CS2);                                         // Constructor for two nodes

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

        friend void msgSendISR(UTFR_CAN& SELF);     // Sends messages when interrupt raised by HW timer 1
                                                    // Must be a friend because ISRs cannot be members of class
                                                    // They must have an ordinary function pointer (something about compiling idk)
                                                    
        void receiveMsgsCommonFcn(unsigned long canID, uint8_t dataLength, uint8_t buf[8]); // parts of receiveMsgs common to all nodes
};

#endif
