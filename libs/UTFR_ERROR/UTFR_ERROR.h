/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "UTFR_CAN_RC.h"


/******************************************************************************
 *                              D E F I N E S                                 *
 *****************************************************************************/
#ifndef _UTFR_ERROR_H_
#define _UTFR_ERROR_H_

//#define debugMode

enum errorNAMES_E { //All possible types of errors
  BMS_OVERTEMP,
  BMS_UNDERVOLT,
  BMS_OVERVOLT,
  APPS_MISMATCH,
  APPS_OUTPUT,
  SDC_TRIPPED,
  COOLANT_OVERHEAT,
  COOLANT_PRESSURE,
  COOLANT_FLOW
};

class UTFR_ERROR{
  public: 
    /*! Sends error over UTFR_CAN node. 
     *  The Message and Field indexes are found using the error_map_.
     *
     * @param[in] error Type of error to send, from errorNAMES_E enum.
     */
    void sendError(UTFR_CAN_RC& error_node, errorNAMES_E error);

  private:
    //<---------------- GLOBAL CLASS VARIABLES ------------------->

    const uint8_t error_map_[2][8] = { //using 0xFF to represent empty fields, 0x00 to represent active fields
      {BMS_OVERTEMP, BMS_UNDERVOLT, BMS_OVERVOLT, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {APPS_MISMATCH, APPS_OUTPUT, SDC_TRIPPED, 0xFF, 0xFF, 0xFF, 0xFF},
      {COOLANT_OVERHEAT, COOLANT_PRESSURE, COOLANT_FLOW, }
    };
};
#endif