
/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "lib_util.h"
#include "UTFR_CAN_MEGA.h"
#include "UTFR_PIN_DRIVER_MEGA.h"

/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/
#ifndef _UTFR_COOLING_MEGA_H_
#define _UTFR_COOLING_MEGA_H_

//#define debugMode

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/
class UTFR_COOLING_MEGA
{
  public: 

    UTFR_COOLING_MEGA();                                   // Constructor
    bool checkCoolingLoop(UTFR_CAN_MEGA& CAN);          // Checks if cooling loop parameters are within safe bounds

  private:
    
    /******************************************************************************
    *              P R I V A T E   D A T A   D E F I N I T I O N S               *
    *****************************************************************************/
    //===Pressure===
    const float kLOW_PRESS = 0;
    const float kHIGH_PRESS = 100;

    const float kLOW_PRESS_RAW = 102;
    const float kHIGH_PRESS_RAW = 921;

    const float kPRESS_DANGER_THRESHOLD = 10;   // TO DO: Get better estimated thresholds from Christian before testing

    const uint8_t kLOWPRESS_THRESHOLD = 5;      //How many loops before false is sent
    uint8_t inv_lowpress_counter = 0;
    uint8_t mot_lowpress_counter = 0;

    //===Temperature===
    const int kLOW_TEMP = 0; 
    const int kHIGH_TEMP = 275;

    const int kLOW_TEMP_RAW = 102;
    const int kHIGH_TEMP_RAW = 921;

    const int kMOTOR_TEMP_FAN_ON_THRESHOLD = 50;    //This is controlled by external fan controller (hopefully)
    const int kMOTOR_TEMP_FAN_OFF_THRESHOLD = 20;
    const int kMOTOR_TEMP_DANGER_THRESHOLD = 80;

    const int kINV_TEMP_FAN_ON_THRESHOLD = 50;
    const int kINV_TEMP_FAN_OFF_THRESHOLD = 20;
    const int kINV_TEMP_DANGER_THRESHOLD = 80;

    const uint8_t kOVERTEMP_THRESHOLD = 5;    //How many loops before false is sent
    uint8_t overtemp_counter = 0;

    //===Flow===
    const float kLOW_FLOW = 0;
    const float kHIGH_FLOW = 65;

    const float kLOW_FLOW_RAW = 2;
    const float kHIGH_FLOW_RAW = 1023;

    const float kFLOW_DANGER_THRESHOLD = 10;

    const uint8_t kLOWFLOW_THRESHOLD = 5;  //How many loops before false is sent
    uint8_t lowflow_counter = 0;

    //==Fan states==
    bool FAN_L = false;     // Pumps are always on when car is on therefore we are controlling the fans
    bool FAN_R = false;     // TO DO: Confirm pins are mapped to correct sides of car
  

    /******************************************************************************
    *         P R I V A T E   F U N C T I O N   D E C L A R A T I O N S          *
    *****************************************************************************/
    bool checkTemp(UTFR_CAN_MEGA& can);    // Gets coolant temps, stores values in CAN array, returns false if critical overheating
    bool checkPress(UTFR_CAN_MEGA& can);   // Gets coolant pressures, returns false if overpressure (stores values in CAN array)
    bool checkFlow(UTFR_CAN_MEGA& can);    // Gets coolant flow rate, returns false if pump is ON && no flow, true otherwise

};
#endif