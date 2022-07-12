#include "UTFR_CAN_ACM.h"


// Constructor for 1 node
UTFR_CAN_ACM::UTFR_CAN_ACM(uint8_t CS1)
{   

    // Initialize CAN nodes
    _CS1 = CS1;
    _NODE1.init_CS(_CS1);

    #ifdef debugMode
    Serial.println("Nodes instantiated correctly.");
    #endif
}


// Constructor for 2 nodes
UTFR_CAN_ACM::UTFR_CAN_ACM(uint8_t CS1, uint8_t CS2)
{   

    // Initialize CAN nodes
    _CS1 = CS1;
    _CS2 = CS2;
    _NODE1.init_CS(_CS1);
    _NODE2.init_CS(_CS2);

    #ifdef debugMode
    Serial.println("Nodes instantiated correctly.");
    #endif
}

#ifdef _1_NODE_
// Starts node - must be called before any other functions
void UTFR_CAN_ACM::begin(uint8_t busSpeed)
{   
    bool validSpeed = false; 

    for (uint8_t speedOption=1; speedOption <= CAN_1000KBPS; speedOption++)     // check if busSpeed is valid option
    {    
        if (busSpeed == speedOption){
            validSpeed = true;
        }
    }

    if (validSpeed)
    {
        _busSpeed = busSpeed;

        while (CAN_OK != _NODE1.begin(_busSpeed))
        {
            Serial.println("CAN BUS Shield init fail");
            Serial.println("\t Init CAN BUS Shield again");
            delay(100);
        }
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("Invalid bus speed entered. CAN node NOT initialized. See line 417 in 'mcp_can_dfs.h' for valid options.");
    }
}


// Send all messages where .isTx == true
void UTFR_CAN_ACM::sendAll()
{
    uint16_t canID;

    for (uint8_t i=0; i<CAN_MSG_COUNT; i++)                                 // using uint8_t assumes we have less than 256 unique can msgs                             
    {                             
        if (_CAN_msgArray[i].isTx)
        {                                          
            canID = static_cast<uint16_t>(_CAN_msgArray[i].msgID & 0x0FFFF); // shortening 32 to 16 bits to save memory

            _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
        }
    }
}


// Send message by name
void UTFR_CAN_ACM::sendMsg(CAN_msgNames_E msgName)
{
    uint16_t canID;
    canID = static_cast<uint16_t>(_CAN_msgArray[msgName].msgID & 0x0FFFF);   // shortening unsigned long to 16 bits to save memory

    _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
}


// Receive pending messages and update message data in-place (in _CAN_msgArray)
void UTFR_CAN_ACM::receiveMsgs()               
{
        uint8_t dataLength = 0;
        uint8_t buf[8];
        unsigned long canID;

        while (CAN_MSGAVAIL == _NODE1.checkReceive())                         // iterate over all pending messages
        {
            _NODE1.readMsgBufID(&canID, &dataLength, buf);    

            for (uint8_t msgIdx=0; msgIdx<CAN_MSG_COUNT; msgIdx++)          // find matching ID and update that msgData with received data 
            {                                                               // iterating should be fine as each MCU handles very few unique msgs (<10)
                if (canID == _CAN_msgArray[msgIdx].msgID){
                    std::copy(buf + 0, buf + dataLength, _CAN_msgArray[msgIdx].msgData);  // higher speed performance, takes more memory to load std lib   
                    break;
                }
                
                _CAN_msgArray[msgIdx].isDirty = true;
            }

            #ifdef debugMode
            Serial.print("Received msg:\t"); Serial.println(canID);
            for(uint8_t i = 0; i<dataLength; i++)
            {
                Serial.print(buf[i]);Serial.print("\t");
            }
            Serial.println();
            #endif
        }
}


// Set masks and filters according to CAN_filterArray
void UTFR_CAN_ACM::setFilters()
{
    for (uint8_t i=0; i<CAN_MASK_FILTER_COUNT; i++)
    {   
        if (i < 2)
        {
            _NODE1.init_Mask(i, _ext, _CAN_filterArray[i]);
        }
        else
        {
            _NODE1.init_Filt(i-2, _ext, _CAN_filterArray[i]);
        }
    }
}


// Set masks and filters so that all messages sent by other nodes are received
void UTFR_CAN_ACM::setFilters_permitAll()
{
    _NODE1.init_Mask(0, _ext, 0);
    _NODE1.init_Mask(1, _ext, 0);
}


// Set masks and filters so that this node receives NO messages
void UTFR_CAN_ACM::setFilters_permitNone()
{
    for (uint8_t i=0; i<CAN_MASK_FILTER_COUNT; i++)
    {   
        if (i < 2)
        {
            _NODE1.init_Mask(i, _ext, 0b11111111111);
        }
        else
        {
            _NODE1.init_Filt(i-2, _ext, 0);
        }
    }
}
#endif

#ifdef _2_NODES_                                                                // This gets defined in 2 node constructor
// Starts node - must be called before any other functions
void UTFR_CAN_ACM::begin(uint8_t busSpeed, uint8_t nodeNumber)
{   
    bool validSpeed = false; 

    for (uint8_t speedOption=1; speedOption <= CAN_1000KBPS; speedOption++)     // check if busSpeed is valid option
    {    
        if (busSpeed == speedOption){
            validSpeed = true;
        }
    }

    if (validSpeed)
    {
        _busSpeed = busSpeed;


        // NODE 1 ------------------------------------------------------
        if (nodeNumber == 1)
        {
            while (CAN_OK != _NODE1.begin(_busSpeed))
            {
                Serial.println("Node 1: CAN BUS Shield init fail");
                Serial.println("\t Init CAN BUS Shield again");
                delay(100);
            }
            Serial.println("Node 1: CAN BUS Shield init ok!");    
        }

        // NODE 2 ------------------------------------------------------
        else if (nodeNumber == 2)
        {
            while (CAN_OK != _NODE2.begin(_busSpeed))
            {
                Serial.println("Node 2: CAN BUS Shield init fail");
                Serial.println("\t Init CAN BUS Shield again");
                delay(100);
            }
            Serial.println("Node 2: CAN BUS Shield init ok!");    
        }

        // ALL NODES ---------------------------------------------------
        else if (nodeNumber == 3)
        {
            while (CAN_OK != _NODE1.begin(_busSpeed))
            {
                Serial.println("Node 1: CAN BUS Shield init fail");
                Serial.println("\t Init CAN BUS Shield again");
                delay(100);
            }
            Serial.println("Node 1: CAN BUS Shield init ok!");  
            
            while (CAN_OK != _NODE2.begin(_busSpeed))
            {
                Serial.println("Node 2: CAN BUS Shield init fail");
                Serial.println("\t Init CAN BUS Shield again");
                delay(100);
            }
            Serial.println("Node 2: CAN BUS Shield init ok!");    
        }


    }
    else
    {
        Serial.println("Invalid bus speed entered. CAN node NOT initialized. See line 417 in 'mcp_can_dfs.h' for valid options.");
    }
}


// Send all messages where .isTx == true
void UTFR_CAN_ACM::sendAll(uint8_t nodeNumber)
{
    uint16_t canID;

    for (uint8_t i=0; i<CAN_MSG_COUNT; i++)                                 // using uint8_t assumes we have less than 256 unique can msgs                             
    {                             
        if (_CAN_msgArray[i].isTx)
        {                                          
            canID = static_cast<uint16_t>(_CAN_msgArray[i].msgID & 0x0FFFF); // shortening 32 to 16 bits to save memory

            // NODE 1 ------------------------------------------------------
            if (nodeNumber == 1)
            {
                _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
            }
            // NODE 2 ------------------------------------------------------
            else if (nodeNumber == 2)
            {
                _NODE2.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
            }
            // ALL NODES ---------------------------------------------------
            else if (nodeNumber == 3)
            {
                _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
                _NODE2.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
            }
        }
    }
}


// Send message by name
void UTFR_CAN_ACM::sendMsg(CAN_msgNames_E msgName, uint8_t nodeNumber)
{
    uint16_t canID;
    canID = static_cast<uint16_t>(_CAN_msgArray[msgName].msgID & 0x0FFFF);   // shortening unsigned long to 16 bits to save memory
    
    // NODE 1 ------------------------------------------------------
    if (nodeNumber == 1)
    {
        _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
    }
    // NODE 2 ------------------------------------------------------
    else if (nodeNumber == 2)
    {
        _NODE2.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
    }
    // ALL NODES ---------------------------------------------------
    else if (nodeNumber == 3)
    {
        _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
        _NODE2.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
    }
    
}


void UTFR_CAN_ACM::receiveMsgsCommonFcn(unsigned long canID, uint8_t dataLength, uint8_t buf[8])
{
    for (uint8_t msgIdx=0; msgIdx<CAN_MSG_COUNT; msgIdx++)          // find matching ID and update that msgData with received data 
    {                                                               // iterating should be fine as each MCU handles very few unique msgs (<10)
        if (canID == _CAN_msgArray[msgIdx].msgID){
            std::copy(buf + 0, buf + dataLength, _CAN_msgArray[msgIdx].msgData);  // higher speed performance, takes more memory to load std lib   
            break;
        }
                
        _CAN_msgArray[msgIdx].isDirty = true;
    }

    #ifdef debugMode
    Serial.print("Received msg:\t"); Serial.println(canID);
    for(uint8_t i = 0; i<dataLength; i++)
    {
        Serial.print(buf[i]);Serial.print("\t");
    }
    Serial.println();
    #endif
}


// Receive pending messages and update message data in-place (in _CAN_msgArray)
void UTFR_CAN_ACM::receiveMsgs(uint8_t nodeNumber)               
{
        uint8_t dataLength = 0;
        uint8_t buf[8];
        unsigned long canID;


    // NODE 1 ------------------------------------------------------
    if (nodeNumber == 1)
    {
        while (CAN_MSGAVAIL == _NODE1.checkReceive())                         // iterate over all pending messages
        {
            _NODE1.readMsgBufID(&canID, &dataLength, buf);    
            receiveMsgsCommonFcn(canID, dataLength, buf);
        }
    }
    // NODE 2 ------------------------------------------------------
    else if (nodeNumber == 2)
    {
        while (CAN_MSGAVAIL == _NODE2.checkReceive())                         // iterate over all pending messages
        {
            _NODE2.readMsgBufID(&canID, &dataLength, buf);  
            receiveMsgsCommonFcn(canID, dataLength, buf);
            
        }
    }
    // ALL NODES ---------------------------------------------------
    else if (nodeNumber == 3)
    {
        while (CAN_MSGAVAIL == _NODE1.checkReceive())                         // iterate over all pending messages
        {
            _NODE1.readMsgBufID(&canID, &dataLength, buf);    
            receiveMsgsCommonFcn(canID, dataLength, buf);
        } 
        while (CAN_MSGAVAIL == _NODE2.checkReceive())                         // iterate over all pending messages
        {
            _NODE2.readMsgBufID(&canID, &dataLength, buf);    
            receiveMsgsCommonFcn(canID, dataLength, buf);
        }
    }
}


// Set masks and filters according to CAN_filterArray
void UTFR_CAN_ACM::setFilters(uint8_t nodeNumber)
{
    for (uint8_t i=0; i<CAN_MASK_FILTER_COUNT; i++)
    {   
        if (i < 2)
        {
            // NODE 1 ------------------------------------------------------
            if (nodeNumber == 1)
            {
                _NODE1.init_Mask(i, _ext, _CAN_filterArray[i]);
            }    
            // NODE 2 ------------------------------------------------------
            else if (nodeNumber == 2)
            {
                _NODE2.init_Mask(i, _ext, _CAN_filterArray[i]);
            }
            // ALL NODES ----------------------------------------------------
            else if (nodeNumber == 3)
            {
                _NODE1.init_Mask(i, _ext, _CAN_filterArray[i]);
                _NODE2.init_Mask(i, _ext, _CAN_filterArray[i]);
            }
        }
        else
        {
            // NODE 1 ------------------------------------------------------
            if (nodeNumber == 1)
            {
                _NODE1.init_Filt(i-2, _ext, _CAN_filterArray[i]);
            }  
            // NODE 2 ------------------------------------------------------
            else if (nodeNumber == 2)
            {
                _NODE2.init_Filt(i-2, _ext, _CAN_filterArray[i]);
            }
            // ALL NODES ---------------------------------------------------
            else if (nodeNumber == 3)
            {
                _NODE1.init_Filt(i-2, _ext, _CAN_filterArray[i]);
                _NODE2.init_Filt(i-2, _ext, _CAN_filterArray[i]);
            }
        }
    }
}


// Set masks and filters so that all messages sent by other nodes are received
void UTFR_CAN_ACM::setFilters_permitAll(uint8_t nodeNumber)
{
    // NODE 1 ------------------------------------------------------
    if (nodeNumber == 1)
    {
        _NODE1.init_Mask(0, _ext, 0);
        _NODE1.init_Mask(1, _ext, 0);
    }
    // NODE 2 ------------------------------------------------------
    else if (nodeNumber == 2)
    {
        _NODE2.init_Mask(0, _ext, 0);
        _NODE2.init_Mask(1, _ext, 0);
    }
    // ALL NODES ---------------------------------------------------
    else if (nodeNumber == 3)
    {
        _NODE1.init_Mask(0, _ext, 0);
        _NODE1.init_Mask(1, _ext, 0);
        _NODE2.init_Mask(0, _ext, 0);
        _NODE2.init_Mask(1, _ext, 0);
    }
}


// Set masks and filters so that this node receives NO messages
void UTFR_CAN_ACM::setFilters_permitNone(uint8_t nodeNumber)
{
    for (uint8_t i=0; i<CAN_MASK_FILTER_COUNT; i++)
    {   
        if (i < 2)
        {
            // NODE 1 ------------------------------------------------------
            if (nodeNumber == 1)
            {
                _NODE1.init_Mask(i, _ext, 0b11111111111);
            }
            // NODE 2 ------------------------------------------------------
            else if (nodeNumber == 2)
            {
                _NODE2.init_Mask(i, _ext, 0b11111111111);
            }
            // ALL NODES ---------------------------------------------------
            else if (nodeNumber == 3)
            {
                _NODE1.init_Mask(i, _ext, 0b11111111111);
                _NODE2.init_Mask(i, _ext, 0b11111111111);
            }

        }
        else
        {
            // NODE 1 ------------------------------------------------------
            if (nodeNumber == 1)
            {
                _NODE1.init_Filt(i-2, _ext, 0);
            }
            // NODE 2 ------------------------------------------------------
            else if (nodeNumber == 2)
            {
                _NODE2.init_Filt(i-2, _ext, 0);
            }
            // ALL NODES ---------------------------------------------------
            else if (nodeNumber == 3)
            {
                _NODE1.init_Filt(i-2, _ext, 0);
                _NODE2.init_Filt(i-2, _ext, 0);
            }
            
        }
    }
}

/*!  
*  @brief Sends error over UTFR_CAN node. 
*         Message and Field indexes are found using the error_map_.
*
*  @param[in] error Type of error to send, from errorNames_E enum.
*/
void UTFR_CAN_ACM::sendError(errorNames_E error)
{
    for (uint8_t msgIndex = 0; msgIndex < CAN_MSG_COUNT; msgIndex++) {
        for (uint8_t fieldIndex = 0; fieldIndex < 8; fieldIndex++) {
            if (error_map[msgIndex][fieldIndex] == error) {
                setField(msgIndex, fieldIndex, 0x00);
                sendMsg(msgIndex, 2);
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
    Serial.print("UTFR_ERROR::sendError: inputted error name not found in error map");
}

#endif

/*!
* @brief    Get data field by name.
*
* @param[in]        msgName    Name of message, defined in _CAN_msgArray
* @param[in]        fieldName  Name of field, defined at top of UTFR_CAN_ACM_version.h
*
* @return    unsigned long     Data of gotten field
*
*/
unsigned long UTFR_CAN_ACM::getField(CAN_msgNames_E msgName, uint8_t fieldName)
{
    long fieldData = 0;                                             // unsigned long is 4 bytes - assumes no field is longer than this
    uint8_t fieldIndex = 0;                                         // fieldIndex: tracks loop position in field data
    uint8_t msgField = 0;                          

    for (uint8_t dataIndex=0; dataIndex < 8; dataIndex++)
    {   
        msgField = _CAN_msgArray[msgName].msgFields[dataIndex];  

        if (msgField == fieldName)
        {   
            _CAN_msgArray[msgName].isDirty = false; 

            // Get 1-byte of data, cast from uint8_t to long, shift appropriately, add to total
            fieldData +=  static_cast<long>(_CAN_msgArray[msgName].msgData[dataIndex]) << (fieldIndex * 8); 
            fieldIndex += 1;
        }
    }

    if (fieldIndex == 0)
    {
        Serial.print("getField failed: Field "); Serial.print(fieldName); Serial.print(" not found in message "); Serial.println(msgName);
    }
    
    return fieldData;
}


/*!
* @brief    Set data field by name.
*
* @param[in]        msgName    Name of message, defined in _CAN_msgArray
* @param[in]        fieldName  Name of field, defined at top of UTFR_CAN_ACM_version.h
* @param[in]        fieldData  The data you want to copy into the field
*
*/
void UTFR_CAN_ACM::setField(CAN_msgNames_E msgName, uint8_t fieldName, long fieldData)
{
    unsigned long fieldMask = 0xFF;
    uint8_t fieldIndex = 0;
    uint8_t msgField = 0;

    if (!_CAN_msgArray[msgName].isTx)
    {
        Serial.print("Should not manually set field in Rx-only message "); Serial.println(msgName); 
        return;
    }

    for (uint8_t dataIndex=0; dataIndex < 8; dataIndex++)
    {   
        msgField = _CAN_msgArray[msgName].msgFields[dataIndex];  

        if (msgField == fieldName)
        {
            // Mask out correct byte of fieldData, shift back to LSB, cast to uint8_t, copy to msg array
            _CAN_msgArray[msgName].msgData[dataIndex] = static_cast<uint8_t>( (fieldData & (fieldMask << (fieldIndex * 8))) >> (fieldIndex * 8) ); 
            fieldIndex += 1;
        }
    }

    if (fieldIndex == 0)
    {
        Serial.print("setField failed: Field "); Serial.print(fieldName); Serial.print(" not found in message "); Serial.println(msgName);
    }
}


// Prints all data stored in a given message (Note: will be in decimal format)
void UTFR_CAN_ACM::printMsgData(CAN_msgNames_E msgName)
{
    Serial.print("Message "); Serial.print(msgName); Serial.print(" data: { ");

    for (uint8_t dataIdx=0; dataIdx<8; dataIdx++)
    {
        Serial.print(_CAN_msgArray[msgName].msgData[dataIdx]); 
        if (dataIdx < 7)
        {
            Serial.print(" , ");
        }
    }

    Serial.println(" }");
}


/*!
* @brief    Checks if this message has been received since last getField
*
* @param[in]        msgName    Name of message, defined in _CAN_msgArray
*
* @return  true - msg received since last getField, false - msg not received
*/
bool UTFR_CAN_ACM::msgDirty(CAN_msgNames_E msgName)
{
    return _CAN_msgArray[msgName].isDirty;
}