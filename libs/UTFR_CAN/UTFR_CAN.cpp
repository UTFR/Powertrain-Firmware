#include "UTFR_CAN.h"


// Constructor
UTFR_CAN::UTFR_CAN(uint8_t CS)
{   
    // Initialize CAN node
    _CS = CS;
    _NODE1.init_CS(_CS);
    _NODE2.init_CS(_CS);


    #ifdef debugMode
    Serial.println("Nodes instantiated correctly.");
    #endif

}


// Starts node - must be called before any other functions
void UTFR_CAN::begin(uint8_t busSpeed, int nodeNumber)
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
        else if (nodeNumber == 12)
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
void UTFR_CAN::sendAll(int nodeNumber)
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
            else if (nodeNumber == 12)
            {
                _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
                _NODE2.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
            }
        }
    }
}

// Send message by name
void UTFR_CAN::sendMsg(CAN_msgNames_E msgName, int nodeNumber)
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
    else if (nodeNumber == 12)
    {
        _NODE1.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
        _NODE2.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
    }
    
}


/*!
* @brief    Get data field by name.
*
* @param[in]        msgName    Name of message, defined in _CAN_msgArray
* @param[in]        fieldName  Name of field, defined at top of UTFR_CAN_version.h
*
* @return    unsigned long     Data of gotten field
*
*/
unsigned long UTFR_CAN::getField(CAN_msgNames_E msgName, uint8_t fieldName)
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
* @param[in]        fieldName  Name of field, defined at top of UTFR_CAN_version.h
* @param[in]        fieldData  The data you want to copy into the field
*
*/
void UTFR_CAN::setField(CAN_msgNames_E msgName, uint8_t fieldName, long fieldData)
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

void UTFR_CAN::receiveMsgsCommonFcn()
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
void UTFR_CAN::receiveMsgs(int nodeNumber)               
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
            receiveMsgsCommonFcn();
        }
    }
    // NODE 2 ------------------------------------------------------
    else if (nodeNumber == 2)
    {
        while (CAN_MSGAVAIL == _NODE2.checkReceive())                         // iterate over all pending messages
        {
            _NODE2.readMsgBufID(&canID, &dataLength, buf);    
            receiveMsgsCommonFcn();
        }
    }
    // ALL NODES ---------------------------------------------------
    else if (nodeNumber == 12)
    {
        while (CAN_MSGAVAIL == _NODE1.checkReceive())                         // iterate over all pending messages
        {
            _NODE1.readMsgBufID(&canID, &dataLength, buf);    
            receiveMsgsCommonFcn();
        } 
        while (CAN_MSGAVAIL == _NODE2.checkReceive())                         // iterate over all pending messages
        {
            _NODE2.readMsgBufID(&canID, &dataLength, buf);    
            receiveMsgsCommonFcn();
        }
    }

        
}

// Set masks and filters according to CAN_filterArray
void UTFR_CAN::setFilters(int nodeNumber)
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
            else if (nodeNumber == 12)
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
            else if (nodeNumber == 12)
            {
                _NODE1.init_Filt(i-2, _ext, _CAN_filterArray[i]);
                _NODE2.init_Filt(i-2, _ext, _CAN_filterArray[i]);
            }
            
        }
    }
}


// Set masks and filters so that all messages sent by other nodes are received
void UTFR_CAN::setFilters_permitAll(int nodeNumber)
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
    else if (nodeNumber == 12)
    {
        _NODE1.init_Mask(0, _ext, 0);
        _NODE1.init_Mask(1, _ext, 0);
        _NODE2.init_Mask(0, _ext, 0);
        _NODE2.init_Mask(1, _ext, 0);
    }
    
}


// Set masks and filters so that this node receives NO messages
void UTFR_CAN::setFilters_permitNone(int nodeNumber)
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
            else if (nodeNumber == 12)
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
            else if (nodeNumber == 12)
            {
                _NODE1.init_Filt(i-2, _ext, 0);
                _NODE2.init_Filt(i-2, _ext, 0);
            }
            
        }
    }
}


// Prints all data stored in a given message (Note: will be in decimal format)
void UTFR_CAN::printMsgData(CAN_msgNames_E msgName)
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