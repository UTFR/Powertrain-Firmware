/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "lib_util.h"
#include "UTFR_CAN.h"


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/
#ifndef _UTFR_COOLING_H_
#define _UTFR_COOLING_H_

//Pins:
//===Pressure===
#define MOT_IN_PRESS_PIN A10 //Check pinout
#define MOT_OUT_PRESS_PIN A9 

#define INV_IN_PRESS_PIN A6
#define INV_OUT_PRESS_PIN A7

//===Temperature===
#define MOT_IN_TEMP_PIN A8
#define MOT_OUT_TEMP_PIN A11

#define INV_IN_TEMP_PIN A4
#define INV_OUT_TEMP_PIN A5

//===Flow===
#define INV_IN_FLOW_PIN A2
#define MOT_IN_FLOW_PIN A3

//===MISC===
#define MOT_PUMP_PIN 8
#define INV_PUMP_PIN 9

//#define debugMode

class UTFR_COOLING{
  public: 
    UTFR_COOLING();

    bool checkTemp(UTFR_CAN& can);

    bool checkPress(UTFR_CAN& can);

    bool checkFlow(UTFR_CAN& can);

  private:
    //<---------------- GLOBAL CLASS VARIABLES ------------------->
    //===Pressure===
    const float kLOW_PRESS = 0;
    const float kHIGH_PRESS = 100;

    const float kLOW_PRESS_VOLT = 102;
    const float kHIGH_PRESS_VOLT = 921;

    const float kPRESS_DANGER_THRESHOLD = 10;

    const uint8_t kLOWPRESS_THRESHOLD = 10; //How many loops before false is sent
    uint8_t inv_lowpress_counter = 0;
    uint8_t mot_lowpress_counter = 0;

    //===Temperature===
    const int kLOW_TEMP = 0; 
    const int kHIGH_TEMP = 275;

    const int kLOW_TEMP_VOLT = 102;
    const int kHIGH_TEMP_VOLT = 921;

    const int kMOTOR_TEMP_PUMP_ON_THRESHOLD = 50;
    const int kMOTOR_TEMP_PUMP_OFF_THRESHOLD = 20;
    const int kMOTOR_TEMP_DANGER_THRESHOLD = 80;

    const int kINV_TEMP_PUMP_THRESHOLD = 50;
    const int kINV_TEMP_PUMP_OFF_THRESHOLD = 20;
    const int kINV_TEMP_DANGER_THRESHOLD = 80;

    const uint8_t kOVERTEMP_THRESHOLD = 10; //How many loops before false is sent
    uint8_t overtemp_counter = 0;

    //===Flow===
    const float kLOW_FLOW = 0;
    const float kHIGH_FLOW = 65;

    const float kLOW_FLOW_VOLT = 2;
    const float kHIGH_FLOW_VOLT = 1023;

    const float kFLOW_DANGER_THRESHOLD = 10;

    const uint8_t kLOWFLOW_THRESHOLD = 10; //How many loops before false is sent
    uint8_t lowflow_counter = 0;

    //===Pump===
    bool inv_pump = false;
    bool mot_pump = false;

};
#endif