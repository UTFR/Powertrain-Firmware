/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_CAN.h";
#include <Arduino.h>


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
  SDC_TRIPPED,
}

class UTFR_ERROR{
  public: 
    /*! UTFR_ERROR Class constructor
     *  Initalizes the constructor for the UTFR_CAN class 
     *  @param[in] CS CAN pin
     */
    UTFR_ERROR(uint8_t CS);

    /*! Initalizes UTFR_ERROR class by starting UTFR_CAN node.
     *  Sets UTFR_CAN node to transmit only.
     */
    void begin();

    /*! Sends error over UTFR_CAN node. 
     *  The Message and Field indexes are found using the error_map_.
     *
     * @param[in] error Type of error to send, from errorNAMES_E enum.
     */
    void sendError(errorNAMES_E error);

  private:
    //<---------------- GLOBAL CLASS VARIABLES ------------------->
    UTFR_CAN ERROR_NODE;

    uint8_t error_map_[2][8] = { //using 0xFF to represent empty fields
      {BMS_OVERTEMP, BMS_UNDERVOLT, BMS_OVERVOLT, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {APPS_MISMATCH, SDC_TRIPPED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    }
};
#endif