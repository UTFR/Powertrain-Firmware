/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "Arduino.h"
#include "UTFR_RTD_MEGA_CONFIG.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#ifndef _UTFR_RTD_MEGA_H_
#define _UTFR_RTD_MEGA_H_

//Pins:
#define RTD_IN_PIN          24 //TODO - change to real values
#define RTD_OUT_PIN         23
#define IGNITION_IN_PIN     22
#define IGNITION_OUT_PIN    21

class UTFR_RTD_MEGA
{
    public:
        UTFR_RTD_MEGA();
        /*! Checks RTD logic using RTD & Ignition In Pins, and the below bools.
         *  @param[in] sdc bool of SDC status - True when not active.
         *  @param[in] inv bool of INV status - True when Ready
         *  @param[in] micro bool of Micro RTD status - True when Micro RTD is ready
         *  @param[in] cooling bool of cooling status - True when no issues arise
         *  @returns True if RTD & Ignition signals are both recieved, and all 
         *           above bools are true - false otherwise.
         */
        bool confirmReady(bool sdc, bool inv, bool micro, bool cooling);
};
#endif

