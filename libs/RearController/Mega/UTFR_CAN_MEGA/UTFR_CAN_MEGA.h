/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "ArduinoSTL.h"
#include "mcp_can.h"       


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/

#ifndef _UTFR_CAN_MEGA_H_
#define _UTFR_CAN_MEGA_H_

//#define debugMode                         // Uncomment this line to enable debug prints


// ---------->> CHANGE THESE FOR EACH SPECIFIC IMPLEMENTATION OF THIS LIBRARY
// ----------------------------------------------------------------------------------------------------->>

//#define _1_NODE_                            // Comment out for MCU that uses two nodes
#define _2_NODES_                         // Comment out for MCU that uses only one node

#define UNUSED_F 0                          // Define your message data fields here so you can access them by name later

// ER0 - Error CAN Message 0 (LV BMS, APPS, SDC)
#define ER_BMS_OVERTEMP_F   1
#define ER_BMS_UNDERVOLT_F  2
#define ER_APPS_MISMATCH_F  3
#define ER_APPS_OUTPUT_F    4
#define ER_SDC_TRIPPED_F    5


// ER1 - Error CAN Message 1 (Coolant Loop)
#define ER_COOLANT_OVERTEMP_F   1
#define ER_COOLANT_PRESSURE_F   2
#define ER_COOLANT_FLOW_F       3


// Inverter Temps #1
#define INV_IGBT_A_TEMP_F       1
#define INV_IGBT_B_TEMP_F       2
#define INV_IGBT_C_TEMP_F       3
#define INV_GATE_DRIVER_TEMP_F  4


// Inverter Temps #3
#define INV_COOLANT_TEMP_F  1
#define INV_HOTSPOT_TEMP_F  2
#define INV_MOTOR_TEMP_F    3
#define INV_TORQUE_SHUD_F   4


// Inverter State Message
#define INV_VSM_STATE_F                 1
#define INV_PWM_FREQ_F                  2
#define INV_INV_STATE_F                 3
#define INV_REALY_STATE_F               4
#define INV_RUN_MODE_DISCHARGE_STATE_F  5


// APPS Message
#define APPS_F      1


// Motor Temps Message
#define MOTOR_TEMP_IN_F    1
#define MOTOR_TEMP_OUT_F   2


// Inverter Temps Message
#define INV_TEMP_IN_F     1
#define INV_TEMP_OUT_F    2


enum CAN_msgNames_E                          // Define all CAN message names here so you can access them by name later                                    
{                                          
    CAN_MSG_ER0,
    CAN_MSG_ER1,
    CAN_MSG_INV_TEMPS_1,                     // Internal inverter sensors   
    CAN_MSG_INV_TEMPS_3,                     // Internal inverter sensors
    CAN_MSG_INV_INTERNAL_STATE,
    CAN_MSG_APPS,
    CAN_MSG_MOTOR_TEMPS,
    CAN_MSG_INV_TEMPS,                       // Cooling loop sensor
    
    CAN_MSG_COUNT
};


enum errorNames_E 
{                                           // Define all error types here
  ERR_NONE,
  ERR_BMS_OVERTEMP,
  ERR_BMS_UNDERVOLT,
  ERR_APPS_MISMATCH,
  ERR_APPS_OUTPUT,
  ERR_SDC_TRIPPED,
  ERR_COOL_OVERTEMP,
  ERR_COOL_NOPRESS,
  ERR_COOL_NOFLOW,
};

const uint8_t error_map[2][8] =             // Using 0xFF to represent empty fields, 0x00 to represent active errors
{           
      {ERR_BMS_OVERTEMP, ERR_BMS_UNDERVOLT, ERR_APPS_MISMATCH, ERR_APPS_OUTPUT, ERR_SDC_TRIPPED, 0xFF, 0xFF, 0xFF},
      {ERR_COOL_OVERTEMP, ERR_COOL_NOPRESS, ERR_COOL_NOFLOW, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

// ----------------------------------------------------------------------------------------------------->>

class UTFR_CAN_MEGA
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
            volatile bool isDirty;                    // true if new data received since last read
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
            [CAN_MSG_ER0] = 
            {
                .msgID = 0x1B1, //TBD
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {ER_BMS_OVERTEMP_F,  ER_BMS_UNDERVOLT_F, 
                              ER_APPS_MISMATCH_F, ER_APPS_OUTPUT_F,
                              ER_SDC_TRIPPED_F,   UNUSED_F,
                              UNUSED_F,           UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_ER1] = 
            {
                .msgID = 0x1B2, //TBD
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {ER_COOLANT_OVERTEMP_F,  ER_COOLANT_PRESSURE_F, 
                              ER_COOLANT_FLOW_F,      UNUSED_F,
                              UNUSED_F,               UNUSED_F,
                              UNUSED_F,               UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_INV_TEMPS_1] = 
            {
                .msgID = 0x0A0,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {INV_IGBT_A_TEMP_F,      INV_IGBT_A_TEMP_F, 
                              INV_IGBT_B_TEMP_F,      INV_IGBT_B_TEMP_F,
                              INV_IGBT_C_TEMP_F,      INV_IGBT_C_TEMP_F,
                              INV_GATE_DRIVER_TEMP_F, INV_GATE_DRIVER_TEMP_F},
                .isTx = false,
                .isRx = true,
                .isDirty = false,
            },
            [CAN_MSG_INV_TEMPS_3] = 
            {
                .msgID = 0x0A2,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {INV_COOLANT_TEMP_F,     INV_COOLANT_TEMP_F, 
                              INV_HOTSPOT_TEMP_F,     INV_HOTSPOT_TEMP_F,
                              INV_MOTOR_TEMP_F,       INV_MOTOR_TEMP_F,
                              INV_TORQUE_SHUD_F,      INV_TORQUE_SHUD_F},
                .isTx = false,
                .isRx = true,
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
            [CAN_MSG_APPS] =
            {
                .msgID = 0x01B4,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {APPS_F,    APPS_F, 
                              APPS_F,    APPS_F,
                              UNUSED_F,  UNUSED_F,
                              UNUSED_F,  UNUSED_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_MOTOR_TEMPS] =
            {
                .msgID = 0x01B5,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {MOTOR_TEMP_IN_F,  MOTOR_TEMP_IN_F, 
                              MOTOR_TEMP_IN_F,  MOTOR_TEMP_IN_F,
                              MOTOR_TEMP_OUT_F, MOTOR_TEMP_OUT_F,
                              MOTOR_TEMP_OUT_F, MOTOR_TEMP_OUT_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            },
            [CAN_MSG_INV_TEMPS] =
            {
                .msgID = 0x01B6,
                .msgData = {0xFF, 0xFF, 0xFF, 0xFF, 
                            0xFF, 0xFF, 0xFF, 0xFF},
                .msgFields = {INV_TEMP_IN_F,  INV_TEMP_IN_F, 
                              INV_TEMP_IN_F,  INV_TEMP_IN_F,
                              INV_TEMP_OUT_F, INV_TEMP_OUT_F,
                              INV_TEMP_OUT_F, INV_TEMP_OUT_F},
                .isTx = true,
                .isRx = false,
                .isDirty = false,
            }
        };


        unsigned long _CAN_filterArray[CAN_MASK_FILTER_COUNT] =         // Define the filters you want to apply to incoming messages here
        {
            // TO DO: Configured to accept only critical inverter messages, add more for inverter datalogging
            [CAN_MASK_0] = 0b00000000000,             // Applies to CAN_FILTER_0 and 1  (Receive buffer 0, RXB0)
            [CAN_MASK_1] = 0b00000000000,             // Applies to CAN_FILTER_2 to 5   (Recieve buffer 1, RXB1)

            [CAN_FILTER_0] = 0x0A0,
            [CAN_FILTER_1] = 0x0A0,
            [CAN_FILTER_2] = 0x0A0,
            [CAN_FILTER_3] = 0x0A0,
            [CAN_FILTER_4] = 0x0A0,
            [CAN_FILTER_5] = 0x0A0
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
        UTFR_CAN_MEGA(uint8_t CS1);                                                      // Constructor for one node
        UTFR_CAN_MEGA(uint8_t CS1, uint8_t CS2);                                         // Constructor for two nodes

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
        void sendError(errorNames_E error);                                         // Sends error messages by name
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