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
#define RTD_IN_PIN          25 
#define RTD_OUT_PIN         24
#define IGNITION_IN_PIN     21
#define IGNITION_OUT_PIN    38
#define MICRO_IN_PIN        20     

class UTFR_RTD_MEGA
{
    public:
        UTFR_RTD_MEGA();
        /*! Checks RTD logic using RTD & Ignition In Pins, and the below bools.
         *  @param[in] sdc bool of SDC status - True when SDC closed. False when no current thru SDC.
         *  @param[in] inv bool of INV status - True when Ready.
         *  @param[in] cooling bool of cooling status - True when no issues arise
         *  @returns True if RTD & Ignition signals are both recieved, and all 
         *           above bools are true - false otherwise.
         */
        bool confirmReady(bool sdc, bool inv, bool cooling);
};
#endif

