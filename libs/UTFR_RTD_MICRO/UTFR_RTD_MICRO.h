/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
//#include UTFR_APPS.h
#include UTFR_RTD_MICRO_CONFIG.h

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_RTD_MICRO_H_
#define _UTFR_RTD_MICRO_H_

//Pins:
#define MEGA_OUT_PIN     = 24
#define BRAKE_IN_PIN     = 20          // Change to real values
#define THROTTLE_IN_PIN  = 20          // 

class UTFR_RTD_MICRO{
    public:
        UTFR_RTD_MICRO(); //Constructor
        bool confirmReady();
    private:
        int kCHECK_COUNTER_ = 5; //Number of loops that each condition must be met 
                                 //for RTD
        int kBRAKE_THRESHOLD_ = 100; //Brake threshold for active RTD
        int kTHROTTLE_THRESHOLD_ = 10; //Throttle threshold for active RTD
}

#endif