
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

#include <mcp_can.h>
//#include <TimerInterrupt.h>

#include <SPI.h>

const int MSG_FREQ = 5;                             // Times per seconds a each msg is transmitted; Hz
const int data_change_interval = 1000;              // delay before changing data bytes; milliseconds
const int test_minutes = 10;                        // Length of test; minutes

const int CS_PIN_9 = 9;                   // this CAN board's INT pin connected to pin 3 on Uno
const int CS_PIN_10 = 10;                 // this CAN board's INT pin connected to pin 2 on Uno

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

MCP_CAN CAN_RF(CS_PIN_9);                   // Right Front Corner Module
MCP_CAN CAN_LF(CS_PIN_10);                  // Left Front Corner Module

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

    while (CAN_OK != CAN_LF.begin(CAN_500KBPS))          
    {
        Serial.println("CAN_LF init fail");
        Serial.println("Init CAN_LF again");
        delay(100);
    }
    Serial.println("CAN_LF init ok!");

    CAN_RF.init_Mask(0, 0, 0b11111111111);            // set all mask bits active to block all incoming msgs
    CAN_RF.init_Mask(1, 0, 0b11111111111);

    CAN_LF.init_Mask(0, 0, 0b11111111111);
    CAN_LF.init_Mask(1, 0, 0b11111111111);

    for (byte i = 0; i < 6; i++)                      // set filters to reject all message IDs given above             
    {
        CAN_RF.init_Filt(i, 0, 0b01111111111);
        CAN_LF.init_Filt(i, 0, 0b01111111111);
    }

    run_time = 60000 * test_minutes;          // (1 minute) * (# of minutes)
    
    start = millis();
    tick = start;
}

void loop()
{
    // sendMsgBuf params:  id = 0x000, standard frame = 0 (ext frame = 1), data len = 8, data buf (unsigned char[data len])
    
    delay(msg_delay/4);

    CAN_RF.sendMsgBuf(ID_RF0, 0, 8, RF0);

    delay(msg_delay/4);

    CAN_RF.sendMsgBuf(ID_RF1, 0, 8, RF1); 

    delay(msg_delay/4);

    CAN_RF.sendMsgBuf(ID_LF0, 0, 8, LF0);     

    delay(msg_delay/4);

    CAN_RF.sendMsgBuf(ID_LF1, 0, 8, LF1);    

    tx_count = tx_count + 2;

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
        
        if (change_count >= log_after) {        // log mcp2515 Transmit Error Count (TEC) and Error Flags (EFLG)  
        
            TEC_RF = CAN_RF.mcp2515_readRegister(MCP_TEC);
            TEC_LF = CAN_LF.mcp2515_readRegister(MCP_TEC);

            EFLG_RF = CAN_RF.mcp2515_readRegister(MCP_EFLG);
            EFLG_LF = CAN_LF.mcp2515_readRegister(MCP_EFLG);

            RFstatus[0] = TEC_RF;                // fill status messages with data
            //RFstatus[1] = REC_RF;
            RFstatus[2] = EFLG_RF;
            RFstatus[3] = tx_count & 0xFF;
            RFstatus[4] = (tx_count >> 8) & 0xFF;
            RFstatus[5] = (tx_count >> 16) & 0xFF;

            LFstatus[0] = TEC_LF;
            //LFstatus[1] = REC_LF;
            LFstatus[2] = EFLG_LF;
            LFstatus[3] = RFstatus[3];
            LFstatus[4] = RFstatus[4];
            LFstatus[5] = LFstatus[5];

            CAN_RF.sendMsgBuf(ID_RFstatus, 0, 8, RFstatus);
            CAN_LF.sendMsgBuf(ID_LFstatus, 0, 8, LFstatus);

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
