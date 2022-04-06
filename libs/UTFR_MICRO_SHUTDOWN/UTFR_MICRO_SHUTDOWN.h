/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "UTFR_APPS.h"


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_MICRO_SHUTDOWN_H_
#define _UTFR_MICRO_SHUTDOWN_H_

#define HW_PIN_MSCOM1 2         // Call SDC_ISR() when this pin set high by Mega
#define HW_PIN_MSCOM3 6         // Set this pin high when zero torque commanded successfully


class UTFR_MICRO_SHUTDOWN
{   
    private:

        UTFR_APPS _APPS = NULL;

        const int _kREADBACK_DELAY = 50;        // MAY NEED ADJUSTMENT: MICROseconds between sending 0 torque and reading back output
        const unsigned long _kTIMEOUT = 150;    // MAY NEED ADJUSTMENT: MILLIseconds before returing to normal functionality if failure        

        int _readback = -1;

        void checkZeroTorqueOut(void);                  // Confirms DAC is commanding zero torque to inverter  
        void carOff(void);                              // Car is off, loop doing nothing
        friend void SDC_ISR(UTFR_MICRO_SHUTDOWN &SELF); // Called when MSCOM1 goes high (Mega wants Micro to shutdown) 

    public:
        
        UTFR_MICRO_SHUTDOWN(UTFR_APPS& APPS);           // Constructor
        
};


#endif