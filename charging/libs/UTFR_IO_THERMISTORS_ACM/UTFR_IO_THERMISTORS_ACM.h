#ifndef IO_THERMISTORS_H
#define IO_THERMISTORS_H


/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include <Arduino.h>
#include "UTFR_HW_ACM.h"
#include "UTFR_CAN_ACM.h"

/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/
#define debug_Thermistors           // uncomment for debug prints

/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

static const float r_ref = 4700;
static const float r_thermistor_base = 18640;
static const float t_thermistor_base = 273;  
static const float beta_value = 4488.13;            // from datasheet and online tool => www.thinksrs.com 
static const float v_dd = 5.0; 

static const float boardTempLimit = 60.0;           // Shutdown if PCB gets hotter than this (C)


/******************************************************************************
 *          P U B L I C   F U N C T I O N   D E C L A R A T I O N S           *
 *****************************************************************************/

float getTempInCelsius(float v_in);         // Convert thermistor voltage reading to celsius
bool checkBoardTemps(UTFR_CAN_ACM &CAN);    // Return true if PCB temps ok, false if too hot

#endif /* IO_THERMISTORS_H */