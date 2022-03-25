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

#include "UTFR_CAN.h"
//#include <TimerInterrupt.h>
//#include <SPI.h>

#define HW_PIN_CAN2_INT 2
#define HW_PIN_CAN1_INT 3
#define HW_PIN_CAN1_CS 47
#define HW_PIN_CAN2_CS 48

#define HW_PIN_CM_INT 2
#define HW_PIN_CM_CS 10

const int MSG_FREQ = 5;                             // Times per seconds a each msg is transmitted; Hz
const int data_change_interval = 1000;              // delay before changing data bytes; milliseconds
const int test_minutes = 10;                        // Length of test; minutes

const int ID_RF0 = 0b01010000000;         // Message IDs
const int ID_RF1 = 0b01010000001;
const int ID_LF0 = 0b01010000010;
const int ID_LF1 = 0b01010000011;
const int ID_RFstatus = 0b01010001000;
const int ID_LFstatus = 0b01010001001;

unsigned char TEC_RF, TEC_LF, EFLG_RF, EFLG_LF, Status_RF, Status_LF = 0;   // Data to report node status stored in these

unsigned char RF0[8] = {0, 0, 0, 0, 0, 1, 0, 1};                            // Message data
unsigned char RF1[8] = {0, 0, 0, 0, 0, 3, 0, 3};
unsigned char LF0[8] = {0, 0, 0, 0, 0, 5, 0, 5};   
unsigned char LF1[8] = {0, 0, 0, 0, 0, 7, 0, 7};

unsigned char RFstatus[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char LFstatus[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned long start, tick, tock, run_time, tx_count = 0;
int log_after = 10;                         // send log msg every (data_change_interval)*(log_after) milliseconds
int change_count = 0;                       // counts up to log_after
int msg_delay = round(1000/MSG_FREQ);       // delay before sending the same CAN message again; milliseconds

unsigned char flagRecv_CAN1, flagRecv_CAN2 = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

MCP_CAN CAN_1(HW_PIN_CAN1_CS);                   // Right Front Corner Module
MCP_CAN CAN_2(HW_PIN_CAN2_CS);                  // Left Front Corner Module

void CAN1_ISR()
{
  flagRecv_CAN1 = 1;
}

void CAN2_ISR()
{
  flagRecv_CAN2 = 1;
}

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN_1.begin(CAN_500KBPS))          // init can bus : baudrate = 500k
    {
        Serial.println("CAN_1 init fail");
        Serial.println("Init CAN_1 again");
        delay(100);
    }
    Serial.println("CAN_1 init ok!");
    
    while (CAN_OK != CAN_2.begin(CAN_500KBPS))          
    {
        Serial.println("CAN_2 init fail");
        Serial.println("Init CAN_2 again");
        delay(100);
    }
    Serial.println("CAN_2 init ok!");
    
    CAN_1.init_Mask(0, 0, 0b00000000000);            // set all mask bits active to block nothing
    CAN_1.init_Mask(1, 0, 0b00000000000);

    CAN_2.init_Mask(0, 0, 0b00000000000);
    CAN_2.init_Mask(1, 0, 0b00000000000);

    /*
    for (byte i = 0; i < 6; i++)                      // set filters to reject all message IDs given above             
    {
        //CAN_1.init_Filt(i, 0, 0b01111111111);
        //CAN_2.init_Filt(i, 0, 0b01111111111);
    }*/

    //pinMode(HW_PIN_CHASSIS_DSC, OUTPUT);
    //digitalWrite(HW_PIN_CHASSIS_DSC, HIGH);

    run_time = 60000 * test_minutes;          // (1 minute) * (# of minutes)
    
    start = millis();

    attachInterrupt(digitalPinToInterrupt(HW_PIN_CAN1_INT), CAN1_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(HW_PIN_CAN2_INT), CAN2_ISR, FALLING); 

}

void loop()
{
    // sendMsgBuf params:  id = 0x000, standard frame = 0 (ext frame = 1), data len = 8, data buf (unsigned char[data len])
    
    delay(msg_delay/4);
    
    CAN_1.sendMsgBuf(ID_RF0, 0, 8, RF0);

    delay(msg_delay/4);

    CAN_1.sendMsgBuf(ID_RF1, 0, 8, RF1); 

    delay(msg_delay/4);

    CAN_2.sendMsgBuf(ID_LF0, 0, 8, LF0);     

    delay(msg_delay/4);

    CAN_2.sendMsgBuf(ID_LF1, 0, 8, LF1);    

    tx_count = tx_count + 1;

    // Change data bytes once every second, send log msg once every (data_change_interval)*(log_after) milliseconds
    tock = millis();
    if ((tock - tick) > data_change_interval){

        if (RF0[0] == 1){
            RF0[0] = 2;
            RF0[2] = 2;
            RF1[0] = 4;
            RF1[2] = 4;
            LF0[0] = 6;
            LF0[2] = 6;
            LF1[0] = 8;
            LF1[2] = 8;
        }
        else{
            RF0[0] = 1;
            RF0[2] = 1;
            RF1[0] = 3;
            RF1[2] = 3;
            LF0[0] = 5;
            LF0[2] = 5;
            LF1[0] = 7;
            LF1[2] = 7;
        }
    } 

    Serial.print("flagRecv_CAN1: "); Serial.println(flagRecv_CAN1);             // ======================= Debug ==========================
    if(flagRecv_CAN1) 
    {                                   // check if get data

        flagRecv_CAN1 = 0;                   // clear flag

        // iterate over all pending messages
        // If either the bus is saturated or the MCU is busy,
        // both RX buffers may be in use and reading a single
        // message does not clear the IRQ conditon.
        while (CAN_MSGAVAIL == CAN_1.checkReceive()) 
        {
            // read data,  len: data length, buf: data buf
            CAN_1.readMsgBuf(&len, buf);

            // print the data
            for(int i = 0; i<len; i++)
            {
                Serial.print(buf[i]);Serial.print("\t");
            }
            Serial.println();
        }
    }

    Serial.print("flagRecv_CAN2: "); Serial.println(flagRecv_CAN2);             // ======================= Debug ==========================
    if(flagRecv_CAN2) 
    {                                   // check if get data

        flagRecv_CAN2 = 0;                   // clear flag

        // iterate over all pending messages
        // If either the bus is saturated or the MCU is busy,
        // both RX buffers may be in use and reading a single
        // message does not clear the IRQ conditon.
        while (CAN_MSGAVAIL == CAN_2.checkReceive()) 
        {
            // read data,  len: data length, buf: data buf
            CAN_2.readMsgBuf(&len, buf);

            // print the data
            for(int i = 0; i<len; i++)
            {
                Serial.print(buf[i]);Serial.print("\t");
            }
            Serial.println();
        }
    }
    
    /*
    if(CAN_MSGAVAIL == CAN_1.checkReceive())            // check if data coming
    {
        CAN_1.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned int canId = CAN_1.getCanId();
        
        Serial.println("-----------------------------");
        Serial.print("Get data from ID: ");
        Serial.println(canId, HEX);

        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }
        Serial.println();
    }*/
    
}
