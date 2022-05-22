#include "UTFR_IO_THERMISTORS_ACM.h"

/******************************************************************************
 *                      P U B L I C   F U N C T I O N S                       *
 *****************************************************************************/

// Credit to Muhammed Yakubu
float getTempInCelsius(float v_in) 
{
  // determine the resistance of thermistor
  float r_thermistor = (r_ref * v_in)/(v_dd - v_in);

  // using the beta formula to calculate the temp value
  // and -273 to convert from kelvin 
  // output the temp
  return ( ( t_thermistor_base * beta_value ) / log( r_thermistor_base / r_thermistor ) ) 
  / ( beta_value / log( r_thermistor_base / r_thermistor ) - t_thermistor_base ) - 273;
}

bool checkBoardTemps(UTFR_CAN_ACM &CAN)
{
  float temp_1 = getTempInCelsius(HW_readMux(HW_MUX_PCB_TEMP_1));
  float temp_2 = getTempInCelsius(HW_readMux(HW_MUX_PCB_TEMP_2));
  float temp_3 = getTempInCelsius(HW_readMux(HW_MUX_PCB_TEMP_3));
  float temp_4 = getTempInCelsius(HW_readMux(HW_MUX_PCB_TEMP_4));

  //CAN.setField();                 // TO DO: Store temps in CAN array here

  #ifdef debug_Thermistors
  Serial.print("PCB Temp 1: "); Serial.print(temp_1); Serial.println("C");
  Serial.print("PCB Temp 2: "); Serial.print(temp_2); Serial.println("C");
  Serial.print("PCB Temp 3: "); Serial.print(temp_3); Serial.println("C");
  Serial.print("PCB Temp 4: "); Serial.print(temp_4); Serial.println("C");
  #endif

  if ((temp_1 > boardTempLimit) ||   // Check PCB temps
      (temp_2 > boardTempLimit) ||
      (temp_3 > boardTempLimit) ||
      (temp_4 > boardTempLimit))
  {
    return false;
  }

  return true;
}