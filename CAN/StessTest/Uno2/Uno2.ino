/**
 * @file Uno1.ino
 * @author Spencer Ball 
 * @brief CANbus Stress Test: 4 transmitting nodes, one recieving node (AIM datalogger).
 *        This program simulates Rear Right and Rear Left corner modules on the 2022 UTFR car.
 *        Periodically sends node status to datalogger.
 *        Uses software timers, version 1.1 will use hardware timers. 
 * @version 1.0
 * @date 2022-02-02
 * 
 * @copyright None. Property of University of Toronto Formula Racing. 
 * 
 */

#include <mcp_can.h>
//#include <TimerInterrupt.h>

#include <SPI.h>

const int MSG_FREQ = 5;                       // Times per seconds a each msg is transmitted; Hz
const int data_change_interval = 1000;        // delay before changing data bytes; milliseconds
const int test_minutes = 10;                        // Length of test; minutes

const int CS_PIN_9 = 9;                       // this CAN board's INT pin connected to pin 3 on Uno
const int CS_PIN_10 = 10;                     // this CAN board's INT pin connected to pin 2 on Uno

const int ID_RR0 = 0b01010000100;             // Message IDs
const int ID_RR1 = 0b01010000101;
const int ID_LR0 = 0b01010000110;
const int ID_LR1 = 0b01010000111;
const int ID_RRstatus = 0b01010001010;
const int ID_LRstatus = 0b01010001011;

unsigned char TEC_RR, TEC_LR, EFLG_RR, EFLG_LR, Status_RR, Status_LR = 0;   // Data to report node status stored in these

unsigned char RR0[8] = {0, 0, 0, 0, 0, 1, 0, 1};                            // Message data
unsigned char RR1[8] = {0, 0, 0, 0, 0, 3, 0, 3};
unsigned char LR0[8] = {0, 0, 0, 0, 0, 5, 0, 5};   
unsigned char LR1[8] = {0, 0, 0, 0, 0, 7, 0, 7};

unsigned char RRstatus[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char LRstatus[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned long start, tick, tock, run_time, tx_count = 0;
int log_after = 10;                         // send log msg every (data_change_interval)*(log_after) milliseconds
int change_count = 0;                       // counts up to log_after
int msg_delay = round(1000/MSG_FREQ);       // delay before sending the same CAN message again; milliseconds

//MCP_CAN CAN_RR(CS_PIN_9);                   // Right Rear Corner Module
MCP_CAN CAN_LR(CS_PIN_10);                  // Left Rear Corner Module

void setup()
{
    Serial.begin(115200);
/*
    while (CAN_OK != //CAN_RR.begin(CAN_500KBPS))          // init can bus : baudrate = 500k
    {
        Serial.println("//CAN_RR init fail");
        Serial.println("Init //CAN_RR again");
        delay(100);
    }
    Serial.println("//CAN_RR init ok!"); */

    while (CAN_OK != CAN_LR.begin(CAN_500KBPS))          
    {
        Serial.println("CAN_LR init fail");
        Serial.println("Init CAN_LR again");
        delay(100);
    }
    Serial.println("CAN_LR init ok!");

    //CAN_RR.init_Mask(0, 0, 0b11111111111);                    // set all mask bits active to block all incoming msgs
    //CAN_RR.init_Mask(1, 0, 0b11111111111);

    CAN_LR.init_Mask(0, 0, 0b11111111111);
    CAN_LR.init_Mask(1, 0, 0b11111111111);

    for (byte i = 0; i < 6; i++)                             // set filters to reject all message IDs given above             
    {
        //CAN_RR.init_Filt(i, 0, 0b01111111111);
        CAN_LR.init_Filt(i, 0, 0b01111111111);
    }

    const int run_time = (1000 * 60) * (test_minutes);      // Total run time of test in milliseconds; (1 minute) * (# of minutes)
    start = millis();
    tick = start;
}

void loop()
{
    // sendMsgBuf params:  id = 0x000, standard frame = 0 (ext frame = 1), data len = 8, data buf (unsigned char[data len])
    
    delay(msg_delay/4);

    //CAN_RR.sendMsgBuf(ID_RR0, 0, 8, RR0);

    delay(msg_delay/4);

    //CAN_RR.sendMsgBuf(ID_RR1, 0, 8, RR1); 

    delay(msg_delay/4);

    //CAN_RR.sendMsgBuf(ID_LR0, 0, 8, LR0);     

    delay(msg_delay/4);

    //CAN_RR.sendMsgBuf(ID_LR1, 0, 8, LR1);    

    tx_count = tx_count + 2;

    // Change data bytes once every second, send log msg once every (data_change_interval)*(log_after) milliseconds
    tock = millis();
    if ((tock - tick) > data_change_interval){

        if (RR0[0] == 1){
            RR0[0] = 2;
            RR0[2] = 2;
            RR1[0] = 4;
            RR1[2] = 4;
            LR0[0] = 6;
            LR0[2] = 6;
            LR1[0] = 8;
            LR1[2] = 8;
        }
        else{
            RR0[0] = 1;
            RR0[2] = 1;
            RR1[0] = 3;
            RR1[2] = 3;
            LR0[0] = 5;
            LR0[2] = 5;
            LR1[0] = 7;
            LR1[2] = 7;
        }
        
        if (change_count >= log_after) {        // log mcp2515 Transmit Error Count (TEC) and Error Flags (EFLG)  
        
            //TEC_RR = CAN_RR.mcp2515_readRegister(MCP_TEC);
            TEC_LR = CAN_LR.mcp2515_readRegister(MCP_TEC);

            //EFLG_RR = CAN_RR.mcp2515_readRegister(MCP_EFLG);
            EFLG_LR = CAN_LR.mcp2515_readRegister(MCP_EFLG);

            RRstatus[0] = TEC_RR;                // fill status messages with data
            //RRstatus[1] = REC_RR;
            RRstatus[2] = EFLG_RR;
            RRstatus[3] = tx_count & 0xFF;
            RRstatus[4] = (tx_count >> 8) & 0xFF;
            RRstatus[5] = (tx_count >> 16) & 0xFF;

            LRstatus[0] = TEC_LR;
            //LRstatus[1] = REC_LR;
            LRstatus[2] = EFLG_LR;
            LRstatus[3] = RRstatus[3];
            LRstatus[4] = RRstatus[4];
            LRstatus[5] = LRstatus[5];

            //CAN_RR.sendMsgBuf(ID_RRstatus, 0, 8, RRstatus);
            CAN_LR.sendMsgBuf(ID_LRstatus, 0, 8, LRstatus);

            change_count = 0;
        }

        change_count++;
        tick = millis();
    }

    if ((tock - start) > run_time){         // end program execution after set duration, run_time
        while(true){
            Serial.println("Test complete.");
            delay(1000);
        }
    }
}
