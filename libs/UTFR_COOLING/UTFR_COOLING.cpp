/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_COOLING.h"


UTFR_COOLING::UTFR_COOLING() 
{
}

// Gets coolant temps, stores values in CAN array, returns false if critical overheating
bool UTFR_COOLING::checkTemp(UTFR_CAN_RC& can){

    float motor_in_temp_analog = HW_analogRead(HW_PIN_COOL_TEMP_MTR_IN_ANALOG);
    float motor_out_temp_analog = HW_analogRead(HW_PIN_COOL_TEMP_MTR_OUT_ANALOG);

    float motor_in_temp = map_Generic(
            motor_in_temp_analog, 
            kLOW_TEMP_RAW, 
            kHIGH_TEMP_RAW, 
            kLOW_TEMP, 
            kHIGH_TEMP);

    float motor_out_temp = map_Generic(
            motor_out_temp_analog, 
            kLOW_TEMP_RAW, 
            kHIGH_TEMP_RAW, 
            kLOW_TEMP, 
            kHIGH_TEMP);

    float inv_in_temp_analog = HW_analogRead(HW_PIN_COOL_TEMP_INV_IN_ANALOG);
    float inv_out_temp_analog = HW_analogRead(HW_PIN_COOL_TEMP_INV_OUT_ANALOG);

    float inv_in_temp = map_Generic(
            inv_in_temp_analog, 
            kLOW_TEMP_RAW, 
            kHIGH_TEMP_RAW, 
            kLOW_TEMP, 
            kHIGH_TEMP);

    float inv_out_temp = map_Generic(
            inv_out_temp_analog, 
            kLOW_TEMP_RAW, 
            kHIGH_TEMP_RAW, 
            kLOW_TEMP, 
            kHIGH_TEMP);

    //can.setField(COOLING_FIELD, INV_IN_TEMP, inv_temp_i)                         // TO DO: Add functionality
    //can.setField(COOLING_FIELD, MOTOR_IN_TEMP, motor_temp), etc                  // TO DO: Add functionality

    //Check temps for motor and inv and turn on corresponding fan if need be.
    if ((motor_in_temp > kMOTOR_TEMP_FAN_ON_THRESHOLD ||
            motor_out_temp > kMOTOR_TEMP_FAN_ON_THRESHOLD) && !FAN_R) {
        HW_digitalWrite(HW_PIN_FAN_R_DIGITAL, true);
        FAN_R = true;
    }

    if ((motor_in_temp < kMOTOR_TEMP_FAN_OFF_THRESHOLD &&
            motor_out_temp < kMOTOR_TEMP_FAN_OFF_THRESHOLD) && FAN_R) { 
        HW_digitalWrite(HW_PIN_FAN_R_DIGITAL, false);
        FAN_R = false;
    }

    if ((inv_in_temp > kMOTOR_TEMP_FAN_ON_THRESHOLD ||
            inv_out_temp > kMOTOR_TEMP_FAN_ON_THRESHOLD) && !FAN_L) {
        HW_digitalWrite(HW_PIN_FAN_L_DIGITAL, true);
        FAN_L = true;
    }

    if ((inv_in_temp < kMOTOR_TEMP_FAN_OFF_THRESHOLD &&
            inv_out_temp < kMOTOR_TEMP_FAN_OFF_THRESHOLD) && FAN_L) {
        HW_digitalWrite(HW_PIN_FAN_L_DIGITAL, false);
        FAN_L = false;     
    }

    //Check to make sure temps are within safety range
    if (motor_in_temp > kMOTOR_TEMP_DANGER_THRESHOLD || 
            motor_out_temp > kMOTOR_TEMP_DANGER_THRESHOLD ||
            inv_in_temp > kINV_TEMP_DANGER_THRESHOLD ||
            inv_out_temp > kINV_TEMP_DANGER_THRESHOLD) {
        overtemp_counter++;
         //If over safety range for too many checks:
        if (overtemp_counter > kOVERTEMP_THRESHOLD) {
            return false;
        }
    }
    else {
        overtemp_counter = 0;
    }
    return true;
}

// Gets coolant pressures, returns false if overpressure (stores values in CAN array)
bool UTFR_COOLING::checkPress(UTFR_CAN_RC& can){

    float motor_in_pressure_analog = HW_analogRead(HW_PIN_COOL_PSR_MTR_IN_ANALOG);
    float motor_out_pressure_analog = HW_analogRead(HW_PIN_COOL_PSR_MTR_OUT_ANALOG);

    float motor_in_pressure = map_Generic(
            motor_in_pressure_analog, 
            kLOW_PRESS_RAW, 
            kHIGH_PRESS_RAW, 
            kLOW_PRESS, 
            kHIGH_PRESS);
    
    float motor_out_pressure = map_Generic(
            motor_out_pressure_analog, 
            kLOW_PRESS_RAW, 
            kHIGH_PRESS_RAW, 
            kLOW_PRESS, 
            kHIGH_PRESS);

    float inv_in_pressure_analog = HW_analogRead(HW_PIN_COOL_PSR_INV_IN_ANALOG);
    float inv_out_pressure_analog = HW_analogRead(HW_PIN_COOL_PSR_INV_OUT_ANALOG);

    float inv_in_pressure = map_Generic(
            inv_in_pressure_analog, 
            kLOW_PRESS_RAW, 
            kHIGH_PRESS_RAW, 
            kLOW_PRESS, 
            kHIGH_PRESS);
    
    float inv_out_pressure = map_Generic(
            inv_out_pressure_analog, 
            kLOW_PRESS_RAW, 
            kHIGH_PRESS_RAW, 
            kLOW_PRESS, 
            kHIGH_PRESS);

    //can->setField(COOLING_FIELD, INV_IN_PRESS, inv_in_press) ...etc                   // TO DO: Add functionality
    
    //Check if Inv Pump is running - if so, check pressure are above min threshold
    if (inv_in_pressure < kPRESS_DANGER_THRESHOLD || 
        inv_out_pressure < kPRESS_DANGER_THRESHOLD) {
            
        inv_lowpress_counter++;
        
        if (inv_lowpress_counter > kLOWPRESS_THRESHOLD) {
            return false;
        }
    }
    else {
        inv_lowpress_counter = 0;
    }

    // Check if Motor Pump is running - if so, check pressures are above min threshold
    if (motor_in_pressure < kPRESS_DANGER_THRESHOLD || 
        motor_out_pressure < kPRESS_DANGER_THRESHOLD) {

        mot_lowpress_counter++;

        if (mot_lowpress_counter > kLOWPRESS_THRESHOLD) {
            return false;
        }
    }

    else {
        mot_lowpress_counter = 0;
    }

    return true;
}

// Gets coolant flow rate, returns false if pump is ON && no flow, true otherwise
bool UTFR_COOLING::checkFlow(UTFR_CAN_RC& can){
    float motor_in_flow_analog = HW_analogRead(HW_PIN_FLOW_RATE_MTR_ANALOG);
    float motor_in_flow = map_Generic(
        motor_in_flow_analog,
        kLOW_FLOW_RAW,
        kHIGH_FLOW_RAW,
        kLOW_FLOW,
        kHIGH_FLOW);

    float inv_in_flow_analog = HW_analogRead(HW_PIN_FLOW_RATE_INV_ANALOG);
    float inv_in_flow = map_Generic(
        inv_in_flow_analog,
        kLOW_FLOW_RAW,
        kHIGH_FLOW_RAW,
        kLOW_FLOW,
        kHIGH_FLOW);
    
    //can->setField(COOLING_FIELD, INV_FLOW, inv_in_flow) ...etc            // TO DO: Add functionality

    if ((motor_in_flow < kFLOW_DANGER_THRESHOLD) ||
            (inv_in_flow < kFLOW_DANGER_THRESHOLD)) {

        lowflow_counter++;

        if (lowflow_counter > kLOWFLOW_THRESHOLD) {
            return false;
        }
    }
    else {
        lowflow_counter = 0;
    }
    return true;
}

bool UTFR_COOLING::checkCoolingLoop(UTFR_CAN_RC& CAN, UTFR_ERROR& ERRORS)
{
    if (!checkTemp(CAN))                            // Returns true if in safe range, turns on pumps if over a certain temp threshold (Ask Cristian)          
    { 
        ERRORS.sendError(CAN, ERR_COOL_OVERTEMP);    // TO DO: Make sure this error type exists. Add if not.                                   
        return false;                 
    }
            
    if (!checkPress(CAN))                           // returns false also if pump is ON && pressure is too low/zero
    {
        ERRORS.sendError(CAN, ERR_COOL_NOPRESS);     // TO DO: Make sure this error type exists. Add if not.  
        return false;
    }
    
    if (!checkFlow(CAN))                           
    {
        ERRORS.sendError(CAN, ERR_COOL_NOFLOW);      // TO DO: Make sure this error type exists. Add if not.
        return false;
    }

    return true;
}