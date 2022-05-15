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

enum errorNames_E {   //All possible types of errors
  ERR_NONE,
  ERR_BMS_OVERTEMP,
  ERR_BMS_UNDERVOLT,
  ERR_APPS_MISMATCH,
  ERR_APPS_OUTPUT,
  ERR_SDC_TRIPPED,
  ERR_COOL_OVERTEMP,
  ERR_COOL_NOPRESS,
  ERR_COOL_NOFLOW,
};

class UTFR_ERROR{
  public: 
    /*! Sends error over UTFR_CAN node. 
     *  The Message and Field indexes are found using the error_map_.
     *
     * @param[in] error Type of error to send, from errorNAMES_E enum.
     */
    void sendError(UTFR_CAN_RC& CAN, errorNames_E error);

  private:
    //<---------------- GLOBAL CLASS VARIABLES ------------------->

    const uint8_t error_map_[2][8] = { //using 0xFF to represent empty fields, 0x00 to represent active fields
      {ERR_BMS_OVERTEMP, ERR_BMS_UNDERVOLT, ERR_APPS_MISMATCH, ERR_APPS_OUTPUT, ERR_SDC_TRIPPED, 0xFF, 0xFF, 0xFF},
      {ERR_COOL_OVERTEMP, ERR_COOL_NOPRESS, ERR_COOL_NOFLOW, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    };
};
#endif