/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "lib_util.h"


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/
#ifndef _UTFR_COOLING_H_
#define _UTFR_COOLING_H_

//Pins:
//===Pressure===
#define MOT_PRESS_IN_PIN = A10 //Check pinout
#define MOT_PRESS_OUT_PIN = A9 

#define INV_PRESS_IN_PIN = A6
#define INV_PRESS_OUT_PIN = A7

//===Temperature===
#define MOT_TEMP_IN_PIN = A8
#define MOT_TEMP_OUT_PIN = A11

#define INV_TEMP_IN_PIN = A4
#define INV_TEMP_OUT_PIN = A5

//===Flow===
#define INV_FLOW_IN_PIN = A2
#define INV_FLOW_OUT_PIN = A3

//#define debugMode

class UTFR_COOLING{
  public: 
    bool checkTemp(UTFR_CAN& can);

    bool checkPress(UTFR_CAN& can);

    bool checkFlow(UTFR_CAN& can);

  private:
    //<---------------- GLOBAL CLASS VARIABLES ------------------->
    const float lowFlow = 0;
    const float highFlow = 65;
    
    const 
};
#endif