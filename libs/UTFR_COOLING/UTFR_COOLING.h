/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "lib_util.h"
#include "UTFR_CAN_RC.h"
#include "UTFR_ERROR.h"


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/
#ifndef _UTFR_COOLING_H_
#define _UTFR_COOLING_H_

// Pins:
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

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
class UTFR_COOLING
{
  public: 

    UTFR_COOLING();                                                 // Constructor
    bool checkCoolingLoop(UTFR_CAN_RC& CAN, UTFR_ERROR& ERRORS);   // Checks if cooling loop parameters are within safe bounds

  private:
    
    /******************************************************************************
    *              P R I V A T E   D A T A   D E F I N I T I O N S               *
    *****************************************************************************/
    //===Pressure===
    const float kLOW_PRESS = 0;
    const float kHIGH_PRESS = 100;

    const float kLOW_PRESS_RAW = 102;
    const float kHIGH_PRESS_RAW = 921;

    const float kPRESS_DANGER_THRESHOLD = 10;

    const uint8_t kLOWPRESS_THRESHOLD = 10; //How many loops before false is sent
    uint8_t inv_lowpress_counter = 0;
    uint8_t mot_lowpress_counter = 0;

    //===Temperature===
    const int kLOW_TEMP = 0; 
    const int kHIGH_TEMP = 275;

    const int kLOW_TEMP_RAW = 102;
    const int kHIGH_TEMP_RAW = 921;

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

    const float kLOW_FLOW_RAW = 2;
    const float kHIGH_FLOW_RAW = 1023;

    const float kFLOW_DANGER_THRESHOLD = 10;

    const uint8_t kLOWFLOW_THRESHOLD = 10; //How many loops before false is sent
    uint8_t lowflow_counter = 0;

    //===Pump===
    bool inv_pump = false;
    bool mot_pump = false;

    //===Fail Check===
    const uint8_t maxFailed = 3;        // How many times a shutdown-causing state should be re-checked before car shutdown
    uint8_t failedChecks = 0;           // Counts up to maxFailed then shuts down the car


    /******************************************************************************
    *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
    *****************************************************************************/
    bool checkTemp(UTFR_CAN_RC& can);    // Gets coolant temps, stores values in CAN array, returns false if critical overheating
    bool checkPress(UTFR_CAN_RC& can);   // Gets coolant pressures, returns false if overpressure (stores values in CAN array)
    bool checkFlow(UTFR_CAN_RC& can);    // Gets coolant flow rate, returns false if pump is ON && no flow, true otherwise

};
#endif