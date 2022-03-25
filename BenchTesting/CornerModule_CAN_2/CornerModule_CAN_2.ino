/**
 * @file Uno1.ino
 * @author Spencer Ball 
 * @brief CANbus Stress Test: 4 transmitting nodes, one recieving node (AIM datalogger).
 *        This program simulates Front Right and Front Left corner modules on the 2022 UTFR car.
 *        Periodically sends node status to datalogger.
 *        Uses software timers, version 1.1 will use hardware timers. 
 * @version 1.0
 * @date 2022-02-02
 * 
 * @copyright University of Toronto Formula Racing 
 * 
 */

#include "mcp_can.h"
//#include <TimerInterrupt.h>
#include <SPI.h>

#define HW_PIN_INT 2
#define HW_PIN_CS 10

const int MSG_FREQ = 5;                             // Times per seconds a each msg is transmitted; Hz
const int data_change_interval = 1000;              // delay before changing data bytes; milliseconds
const int test_minutes = 10;                        // Length of test; minutes

const int ID_RF0 = 0b01001000000;         // Message IDs
const int ID_RF1 = 0b01001000001;

unsigned char RF0[8] = {0, 0, 0, 0, 0, 1, 0, 1};                            // Message data
unsigned char RF1[8] = {0, 0, 0, 0, 0, 3, 0, 3};

unsigned long tick, tock= 0;
int log_after = 10;                         // send log msg every (data_change_interval)*(log_after) milliseconds
int change_count = 0;                       // counts up to log_after
int msg_delay = round(1000/MSG_FREQ);       // delay before sending the same CAN message again; milliseconds

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

MCP_CAN CAN_RF(HW_PIN_CS);                   // Right Front Corner Module

void MCP2515_ISR()
{
    flagRecv = 1;
}

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN_RF.begin(CAN_500KBPS))          // init can bus : baudrate = 500k
    {
        Serial.println("CAN_RF init fail");
        Serial.println("Init CAN_RF again");
        delay(100);
    }
    Serial.println("CAN_RF init ok!");
    /*
    while (CAN_OK != CAN_LF.begin(CAN_500KBPS))          
    {
        Serial.println("CAN_LF init fail");
        Serial.println("Init CAN_LF again");
        delay(100);
    }
    Serial.println("CAN_LF init ok!");
    */
    CAN_RF.init_Mask(0, 0, 0b00000000000);            // set all mask bits active to block nothing
    CAN_RF.init_Mask(1, 0, 0b00000000000);

    //CAN_LF.init_Mask(0, 0, 0b11111111111);
    //CAN_LF.init_Mask(1, 0, 0b11111111111);
    /*
    for (byte i = 0; i < 6; i++)                      // set filters to reject all message IDs given above             
    {
        CAN_RF.init_Filt(i, 0, 0b01111111111);
        //CAN_LF.init_Filt(i, 0, 0b01111111111);
    }*/
    
    attachInterrupt(digitalPinToInterrupt(HW_PIN_INT), MCP2515_ISR, FALLING);   // start interrupt
}

void loop()
{
    // sendMsgBuf params:  id = 0x000, standard frame = 0 (ext frame = 1), data len = 8, data buf (unsigned char[data len])
    
    delay(msg_delay/4);

    CAN_RF.sendMsgBuf(ID_RF0, 0, 8, RF0);

    delay(msg_delay/4);

    CAN_RF.sendMsgBuf(ID_RF1, 0, 8, RF1); 

    delay(msg_delay/4);

    delay(msg_delay/4); 


    // Change data bytes once every second, send log msg once every (data_change_interval)*(log_after) milliseconds
    tock = millis();
    if ((tock - tick) > data_change_interval){

        if (RF0[0] == 1){
            RF0[0] = 2;
            RF0[2] = 2;
            RF1[0] = 4;
            RF1[2] = 4;
        }
        else{
            RF0[0] = 1;
            RF0[2] = 1;
            RF1[0] = 3;
            RF1[2] = 3;
        }
    }
    Serial.print("flagRecv: "); Serial.println(flagRecv);             // ======================= Debug ==========================
    if(flagRecv) 
    {                                   // check if get data
        flagRecv = 0;                   // clear flag

        // Iterate over all pending messages
        // If either the bus is saturated or the MCU is busy,
        // both RX buffers may be in use and reading a single
        // message does not clear the IRQ conditon.
        while (CAN_MSGAVAIL == CAN_RF.checkReceive()) 
        {
            // read data,  len: data length, buf: data buf
            CAN_RF.readMsgBuf(&len, buf);

            // print the data
            for(int i = 0; i<len; i++)
            {
                Serial.print(buf[i]);Serial.print("\t");
            }
            Serial.println();
        }
    }
/*
    if(CAN_MSGAVAIL == CAN_RF.checkReceive())            // check if data coming
    {
        CAN_RF.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned int canId = CAN_RF.getCanId();
        
        Serial.println("-----------------------------");
        Serial.print("Get data from ID: ");
        Serial.println(canId, HEX);

        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }
        Serial.println();
    }
*/
}
