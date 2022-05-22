/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "UTFR_PIN_DRIVER_MICRO.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_RTD_MICRO_H_
#define _UTFR_RTD_MICRO_H_

//#define debug_RTD_Micro                       // uncomment for debug prints

class UTFR_RTD_MICRO
{
public:
    UTFR_RTD_MICRO();

    /*! Check Throttle & Brake
     *  @param[in] throttle position from APPS
     *  @returns True if throttle and brake are ready for RTD for
     *           for more than kCHECK_COUNTER_ loops.
     */
    bool confirmReady(void);

private:
    /*! Check Throttle Input
     *  @returns True if throttle is below kTHROTTLE_THRESHOLD_,
     *           false otherwise
     */
    bool checkThrottle(void);

    /*! Check Brake Input
     *  @returns True if Brake is above kBRAKE_THRESHOLD_,
     *           false otherwise
     */
    bool checkBrake(void);

    int kCHECK_COUNTER_ = 5;       // Number of loops that each condition must be met
                                   // for RTD
    int kBRAKE_THRESHOLD_ = 95;   // Brake threshold for active RTD             TO DO: calibrate these
    int kTHROTTLE_THRESHOLD_ = 10; // Throttle threshold for active RTD         TO DO: calibrate these
};

#endif
