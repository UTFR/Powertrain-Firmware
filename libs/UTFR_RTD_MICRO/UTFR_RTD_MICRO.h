/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "UTFR_RTD_MICRO_CONFIG.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/

#ifndef _UTFR_RTD_MICRO_H_
#define _UTFR_RTD_MICRO_H_

// Pins:
#define MEGA_OUT_PIN 6
#define BRAKE_IN_PIN A2 

class UTFR_RTD_MICRO
{
public:
    UTFR_RTD_MICRO();

    /*! Check Throttle & Brake
     *  @param[in] throttle position from APPS
     *  @returns True if throttle and brake are ready for RTD for
     *           for more than kCHECK_COUNTER_ loops.
     */
    bool confirmReady(int throttle);

private:
    /*! Check Throttle Input
     *  @param[in] throttle position
     *  @returns True if throttle is below kTHROTTLE_THRESHOLD_,
     *           false otherwise
     */
    bool checkThrottle(int throttle);

    /*! Check Brake Input
     *  @returns True if Brake is above kBRAKE_THRESHOLD_,
     *           false otherwise
     */
    bool checkBrake();

    int kCHECK_COUNTER_ = 5;       // Number of loops that each condition must be met
                                   // for RTD
    int kBRAKE_THRESHOLD_ = 95;   // Brake threshold for active RTD
    int kTHROTTLE_THRESHOLD_ = 10; // Throttle threshold for active RTD
};

#endif
