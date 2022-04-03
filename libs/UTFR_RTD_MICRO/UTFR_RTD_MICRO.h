/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
//#include UTFR_APPS.h
#include <Arduino.h>
#include "UTFR_RTD_MICRO_CONFIG.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
//Pins:
#define MEGA_OUT_PIN        24
#define BRAKE_IN_PIN        20          // Change to real values
#define THROTTLE_IN_PIN     19          // 

class UTFR_RTD_MICRO{
    public:
        UTFR_RTD_MICRO();

        /*! Check Throttle & Brake
         *  @returns True if throttle and brake are ready for RTD for
         *           for more than kCHECK_COUNTER_ loops.
         */
        bool confirmReady();
    private:
        /*! Check Throttle Input using APPS library
         *  @returns True if throttle is below kTHROTTLE_THRESHOLD_, 
         *           false otherwise
         */
        bool checkThrottle();

        /*! Check Brake Input 
         *  @returns True if Brake is above kBRAKE_THRESHOLD_, 
         *           false otherwise
         */
        bool checkBrake();

        int kCHECK_COUNTER_ = 5; //Number of loops that each condition must be met 
                                 //for RTD
        int kBRAKE_THRESHOLD_ = 100; //Brake threshold for active RTD
        int kTHROTTLE_THRESHOLD_ = 10; //Throttle threshold for active RTD
};