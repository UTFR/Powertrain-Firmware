/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#include "UTFR_COOLING.h"


void UTFR_COOLING::UTFR_COOLING() {
    // Initialize Pins
    pinMode(MOT_IN_PRESS_PIN, INPUT);
    pinMode(MOT_OUT_PRESS_PIN, INPUT);
    pinMode(INV_IN_PRESS_PIN, INPUT);
    pinMode(INV_OUT_PRESS_PIN, INPUT);

    pinMode(MOT_IN_TEMP_PIN, INPUT);
    pinMode(MOT_OUT_TEMP_PIN, INPUT);
    pinMode(INV_IN_TEMP_PIN, INPUT);
    pinMode(INV_OUT_TEMP_PIN, INPUT);

    pinMode(MOT_FLOW_IN_PIN, INPUT);
    pinMode(INV_FLOW_IN_PIN, INPUT);

    pinMode(MOT_PUMP_PIN, OUTPUT);
    pinMode(INV_PUMP_PIN, OUTPUT)
}

void UTFR_COOLING::checkTemp(UTFR_CAN& can){

    float motor_in_temp_analog = analogRead(MOT_IN_TEMP_PIN)
    float motor_out_temp_analog = analogRead(MOT_OUT_TEMP_PIN)

    float motor_in_temp = map_Generic(
            mot_temp_in_analog, 
            kLOW_FLOW_VOLT, 
            kHIGH_FLOW_VOLT, 
            kLOW_FLOW, 
            kHIGH_FLOW);

    float motor_out_temp = map_Generic(
            mot_temp_out_analog, 
            kLOW_FLOW_VOLT, 
            kHIGH_FLOW_VOLT, 
            kLOW_FLOW, 
            kHIGH_FLOW);

    float inv_in_temp_analog = analogRead(INV_IN_TEMP_PIN)
    float inv_out_temp_analog = analogRead(INV_OUT_TEMP_PIN)

    float inv_in_temp = map_Generic(
            inv_in_temp_analog, 
            kLOW_FLOW_VOLT, 
            kHIGH_FLOW_VOLT, 
            kLOW_FLOW, 
            kHIGH_FLOW);

    float inv_out_temp = map_Generic(
            inv_out_temp_analog, 
            kLOW_FLOW_VOLT, 
            kHIGH_FLOW_VOLT, 
            kLOW_FLOW, 
            kHIGH_FLOW);

    //can->setField(COOLING_FIELD, INV_IN_TEMP, inv_temp_i)
    //can->setField(COOLING_FIELD, MOTOR_IN_TEMP, motor_temp), etc

    //Check temps for motor and inv and turn on pump if need be.
    if (motor_in_temp > kTEMP_PUMP_THRESHOLD ||
            motor_out_temp > kTEMP_PUMP_THRESHOLD) {
        digitalWrite(MOT_PUMP_PIN, HIGH);
        mot_pump = true;
    }

    if (inv_in_temp > kTEMP_PUMP_THRESHOLD ||
            inv_out_temp > kTEMP_PUMP_THRESHOLD) {
        digitalWrite(INV_PUMP_PIN, HIGH);
        inv_pump = true;
    }

    //Check to make sure temps are within safety range
    if (motor_in_temp > kTEMP_SAFETY_THRESHOLD || 
            motor_out_temp > kTEMP_SAFETY_THRESHOLD ||
            inv_in_temp > kTEMP_SAFETY_THRESHOLD ||
            inv_out_temp > kTEMP_SAFETY_THRESHOLD) {
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

void UTFR_COOLING::checkPress(UTFR_CAN& can){

    float motor_in_pressure_analog = analogRead(MOT_IN_PRESS_PIN);
    float motor_out_pressure_analog = analogRead(MOT_OUT_PRESS_PIN);

    float motor_in_pressure = map_Generic(
            (motor_in_pressure_analog, 
            kLOW_PRESS_VOLT, 
            kHIGH_PRESS_VOLT, 
            kLOW_PRESS, 
            kHIGH_PRESS);
    
    float motor_out_pressure = map_Generic(
            (motor_out_pressure_analog, 
            kLOW_PRESS_VOLT, 
            kHIGH_PRESS_VOLT, 
            kLOW_PRESS, 
            kHIGH_PRESS);

    float inv_in_pressure_analog = analogRead(INV_IN_PRESS_PIN);
    float inv_out_pressure_analog = analogRead(INV_OUT_PRESS_PIN);

    float inv_in_pressure = map_Generic(
            (inv_in_pressure_analog, 
            kLOW_PRESS_VOLT, 
            kHIGH_PRESS_VOLT, 
            kLOW_PRESS, 
            kHIGH_PRESS);
    
    float inv_out_pressure = map_Generic(
            (inv_out_pressure_analog, 
            kLOW_PRESS_VOLT, 
            kHIGH_PRESS_VOLT, 
            kLOW_PRESS, 
            kHIGH_PRESS);

    //can->setField(COOLING_FIELD, INV_IN_PRESS, inv_in_press) ...etc
    
    //Check if Inv Pump is running - if so, check pressure are above min threshold
    if (inv_pump && (
            inv_in_pressure < kPRESS_SAFETY_THRESHOLD || 
            inv_out_pressure < kPRESS_SAFETY_THRESHOLD) {
        inv_lowpress_counter++;
        if (inv_lowpress_counter > kLOWPRESS_THRESHOLD) {
            return false;
        }
    }

    //Check if Motor Pump is running - if so, check pressures are above min threshold
    if (mot_pump && (
            mot_pressure_in < kPRESS_DANGER_THRESHOLD || 
            mot_pressure_out < kPRESS_DANGER_THRESHOLD) {
        mot_lowpress_counter++;
        if (mot_lowpress_counter > kLOWPRESS_THRESHOLD) {
            return false;
        }
    }

    else {
        inv_lowpress_counter = 0;
        mot_lowpress_counter = 0;
    }

    return true;
}

bool UTFR_COOLING::checkFlow(UTFR_CAN& can){
    float motor_in_flow_analog = analogRead(MOT_FLOW_IN_PIN);
    float motor_in_flow = map_Generic(
        motor_in_flow_analog,
        kLOW_FLOW_VOLT,
        kHIGH_FLOW_VOLT,
        kLOW_FLOW,
        kHIGH_FLOW);

    float inv_in_flow_analog = analogRead(INV_FLOW_IN_PIN);
    float inv_in_flow = map_Generic(
        inv_in_flow_analog,
        kLOW_FLOW_VOLT,
        kHIGH_FLOW_VOLT,
        kLOW_FLOW,
        kHIGH_FLOW);
    
    //can->setField(COOLING_FIELD, INV_FLOW, inv_in_flow) ...etc

    if (motor_in_flow < kLOWFLOW_THRESHOLD ||
            inv_in_flow < kLOWFLOW_THRESHOLD) {
        lowflow_counter++;
        if (lowflow_counter > kLOW)
    }
    else {
        lowflow_counter = 0;
    }

    return true;


}