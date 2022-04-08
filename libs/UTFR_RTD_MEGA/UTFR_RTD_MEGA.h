/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include UTFR_RTD_MEGA_CONFIG.h

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_RTD_MEGA_H_
#define _UTFR_RTD_MEGA_H_

//Pins:
#define RTD_OUT_PIN       = 24 //Change to real values
#define IGNITION_OUT_PIN  = 21

class UTFR_RTD_MEGA{
    public:
        UTFR_RTD_MEGA();
        bool confirmReady(bool sdc, bool inv, bool micro, bool cooling);
    private:
}

#endif