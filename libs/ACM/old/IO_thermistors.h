#ifndef IO_THERMISTORS_H
#define IO_THERMISTORS_H

/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/


/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/


/******************************************************************************
 *                              T Y P E D E F S                               *
 *****************************************************************************/


/******************************************************************************
 *              P R I V A T E   D A T A   D E F I N I T I O N S               *
 *****************************************************************************/

static const float r_ref = 4700;
static const float r_thermistor_base = 18640;
static const float t_thermistor_base = 273;  
static const float beta_value = 4488.13; //from datasheet and online tool => www.thinksrs.com 
static const float v_dd = 5.0; 
static float r_thermistor = 0;
static float t_thermistor_in_C = 0;

/******************************************************************************
 *          P U B L I C   F U N C T I O N   D E C L A R A T I O N S           *
 *****************************************************************************/

float getTempInCelcius(const float v_in);

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

// Note to self - do not implement functions in header. Compiles fine for now, need to make .c files

// Credit to Muhammed Yakubu
float getTempInCelcius(const float v_in) {
  // determine the resistance of thermistor
  r_thermistor = (r_ref * v_in)/(v_dd - v_in);

  // using the beta formula to calculate the temp value
  // and -273 to convert from kelvin 
  // output the temp
  return ( ( t_thermistor_base * beta_value ) / log ( r_thermistor_base / r_thermistor ) ) 
  / ( beta_value / log ( r_thermistor_base / r_thermistor ) - t_thermistor_base ) - 273;
}

#endif /* IO_THERMISTORS_H */
