#include "UTFR_CAN.h"
#include "UTFR_CAN.h"

// Constructor
UTFR_CAN::UTFR_CAN(uint8_t CS)
{   
    // Initialize CAN node
    _CS = CS;
    _NODE.init_CS(_CS);

}


// Starts node - must be called before any other functions
void UTFR_CAN::begin(uint8_t busSpeed)
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

        while (CAN_OK != _NODE.begin(_busSpeed))
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
void UTFR_CAN::sendAll()
{
    uint16_t canID;

    for (uint8_t i=0; i<CAN_MSG_COUNT; i++)                                 // using uint8_t assumes we have less than 256 unique can msgs                             
    {                             
        if (_CAN_msgArray[i].isTx)
        {                                          
            canID = static_cast<uint16_t>(_CAN_msgArray[i].msgID & 0x0FFFF); // shortening 32 to 16 bits to save memory

            _NODE.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[i].msgData);       // sendMsgBuf(ID, extended format?, data length, data)
        }
    }
}


// Send message by name
void UTFR_CAN::sendMsg(CAN_msgNames_E msgName)
{
    uint16_t canID;
    canID = static_cast<uint16_t>(_CAN_msgArray[msgName].msgID & 0x0FFFF);   // shortening unsigned long to 16 bits to save memory

    _NODE.sendMsgBuf(canID, _ext, 8, _CAN_msgArray[msgName].msgData);         // sendMsgBuf(ID, extended format?, data length, data)
}


// Get field data by name
unsigned long UTFR_CAN::getField(CAN_msgNames_E msgName, uint8_t fieldName)
{
    long fieldData = 0;                                             // unsigned long is 4 bytes - assumes no field is longer than this
    uint8_t fieldIndex = 0;                                         // fieldIndex: tracks loop position in field data
    uint8_t msgField = 0;

    _CAN_msgArray[msgName].isDirty = false;                           

    for (uint8_t dataIndex=0; dataIndex < 8; dataIndex++)
    {   
        msgField = _CAN_msgArray[msgName].msgFields[dataIndex];  

        if (msgField == fieldName)
        {
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


// Set field data by name
void UTFR_CAN::setField(CAN_msgNames_E msgName, uint8_t fieldName, long fieldData)
{
    unsigned long fieldMask = 0xFF;
    uint8_t fieldIndex = 0;
    uint8_t msgField = 0;

    if (!_CAN_msgArray[msgName].isTx)           // Shouldn't be setting fields in receive-only messages
    {
        Serial.print("Should not manually set field in Rx-only message "); Serial.println(msgName); 
        return;
    }

    for (uint8_t dataIndex=0; dataIndex < 8; dataIndex++)
    {   
        msgField = _CAN_msgArray[msgName].msgFields[dataIndex];  

        if (msgField == fieldName)
        {
            _CAN_msgArray[msgName].msgData[dataIndex] = fieldData & (fieldMask << (fieldIndex * 8)); 
            fieldIndex += 1;
        }
    }

    if (fieldIndex == 0)
    {
        Serial.print("setField failed: Field "); Serial.print(fieldName); Serial.print(" not found in message "); Serial.println(msgName);
    }
}


// Receive pending messages and update message data in-place (in _CAN_msgArray)
void UTFR_CAN::receiveMsgs()               
{
        uint8_t dataLength = 0;
        uint8_t buf[8];
        unsigned long canID;

        while (CAN_MSGAVAIL == _NODE.checkReceive())                         // iterate over all pending messages
        {
            _NODE.readMsgBufID(&canID, &dataLength, buf);    

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
void UTFR_CAN::setFilters()
{
    for (uint8_t i=0; i<CAN_MASK_FILTER_COUNT; i++)
    {   
        if (i < 2)
        {
            _NODE.init_Mask(i, _ext, _CAN_filterArray[i]);
        }
        else
        {
            _NODE.init_Filt(i-2, _ext, _CAN_filterArray[i]);
        }
    }
}


// Set masks and filters so that all messages sent by other nodes are received
void UTFR_CAN::setFilters_permitAll()
{
    _NODE.init_Mask(0, _ext, 0);
    _NODE.init_Mask(1, _ext, 0);
}


// Set masks and filters so that this node receives NO messages
void UTFR_CAN::setFilters_permitNone()
{
    for (uint8_t i=0; i<CAN_MASK_FILTER_COUNT; i++)
    {   
        if (i < 2)
        {
            _NODE.init_Mask(i, _ext, 0b11111111111);
        }
        else
        {
            _NODE.init_Filt(i-2, _ext, 0);
        }
    }
}