#include "lib_util.h"

//COOLANT LOOP TESTING
//4 Pressure Sensors (have a datasheet) (pressure in and out of motor, pressure in and out of inverter)
//2 Flow Rate sensors (no data) (flow into motor, flow into inverter)
//4 Temp Sensors (no data) (temp in and out of motor, temp in and out of inverter)

// PRESSURE ----------------------------------------------------------------------------------------------------------------------------------------------

// assigning pins for pressure in and pressure out of motor and inverter
const int pressMotPinIN = A10;
const int pressMotPinOUT = A9;        // WRONG ON SCHEMATIC ! SWAPPED WITH tempMotPinOUT !

const int pressInvPinIN = A6;
const int pressInvPinOUT = A7;

// assigning variables for range of map function
// pressure for motor and inverter
const int lowPress = 0; // psi
const int highPress = 100;

// note
// instead of 0 to 5 V, do 0.5 to 4.5 V  (0 PSI, 100 PSI)
// 0.5/5 * 1023 = 102 and 4.5/5 * 1023 = 921
// which means mapping from 102, 921 in map function
const int lowPressVolt = 102;     
const int highPressVolt = 921;

// assigning variables for pressure in and out values for motor and inverter
// actual press/temp/flow values are calculated in loop using map function
int pressMotIN;
int pressMotOUT;
int actPressMotIN; 
int actPressMotOUT; 

int pressInvIN;
int pressInvOUT;
int actPressInvIN; 
int actPressInvOUT;

// TEMPERATURE  ------------------------------------------------------------------------------------------------------------------------------------------
// assigning pins for temperature in and temperature out of motor and inverter
const int tempMotPinIN = A8;
const int tempMotPinOUT = A11;

const int tempInvPinIN = A4;
const int tempInvPinOUT = A5;

// assigning variables for range of map function
// temperature for motor and inverter
const int lowTemp = 0; // unknown
const int highTemp = 1023; // unknown

const int lowTempVolt = 102;    // 0.5/5 * 1023 = 102
const int highTempVolt = 921;   // 4.5/5 * 1023 = 921

// assigning variables for temperature in and out for motor and inverter
int tempMotIN;
int tempMotOUT;
int actTempMotIN; 
int actTempMotOUT; 

int tempInvIN;
int tempInvOUT;
int actTempInvIN; 
int actTempInvOUT; 

// FLOW RATE ---------------------------------------------------------------------------------------------------------------------------------------------
// assigning pins for flow rate in of motor and inverter
const int flowInvPinIN = A2;
const int flowMotPinIN = A3;

// assigning variables for range of map function
// flow rate for motor and inverter
const float lowFlow = 0;              
const float highFlow = 65;        // based on: ANALOG OUT = (150-175) @ 10 LPM 

const float lowFlowVolt = 2;      
const float highFlowVolt = 1023;    

// assigning variables for flow rate in for motor and inverter
float flowMotIN;
float actFlowMotIN; 
float avgFlowMotIN = 0.0;

float flowInvIN;
float actFlowInvIN; 

//  ----------------------------------------------------------------------------------------------------------------------------------------------------

float tempVolt;     // Holds raw voltage outputs
int avgOver = 100;  // take average over this many ms
int readingCnt = 0;

void setup() {
  // put your setup code here, to run once:
  // nothing here for now

  pinMode(pressMotPinIN, INPUT);
  pinMode(pressMotPinOUT, INPUT);
  pinMode(pressInvPinIN, INPUT);
  pinMode(pressInvPinOUT, INPUT);

  pinMode(tempMotPinIN, INPUT);
  pinMode(tempMotPinOUT, INPUT);
  pinMode(tempInvPinIN, INPUT);
  pinMode(tempInvPinOUT, INPUT);

  pinMode(flowMotPinIN, INPUT);
  pinMode(flowInvPinIN, INPUT);
  Serial.begin(115200);
  
}

void loop() {

// PRESSURE --------------------------------------------------------------------------------------------------------------------------------------------
// at 0.5V, the pressure should be 0 psi
// at 4.5V, the pressure should be 100 psi   
  
// READING PRESSURE INTO MOTOR
  pressMotIN = analogRead(pressMotPinIN); // read in motor pressure-in voltage
  actPressMotIN = map(pressMotIN, lowPressVolt, highPressVolt, lowPress, highPress); 
  
  Serial.print("Pressure into Motor: ");
  Serial.print(actPressMotIN);
  Serial.println(" Psi");

  
  tempVolt = static_cast<float>(pressMotIN); 
  tempVolt = ((tempVolt * 5.0)/1023.0);
  Serial.print("Voltage of last reading: ");
  Serial.print(tempVolt);
  Serial.println(" V");

// READING PRESSURE OUT OF MOTOR
  pressMotOUT = analogRead(pressMotPinOUT); // read in motor pressure-out voltage
  actPressMotOUT = map(pressMotOUT, lowPressVolt, highPressVolt, lowPress, highPress); 

  Serial.print("Pressure out of Motor: ");
  Serial.print(actPressMotOUT);
  Serial.println(" Psi");

  
  tempVolt = static_cast<float>(pressMotOUT); 
  tempVolt = ((tempVolt * 5.0)/1023.0);
  Serial.print("Voltage of last reading: ");
  Serial.print(tempVolt);
  Serial.println(" V");
  

// READING PRESSURE INTO INVERTER
  pressInvIN = analogRead(pressInvPinIN); // read in inverter pressure-out voltage
  actPressInvIN = map(pressInvIN, lowPressVolt, highPressVolt, lowPress, highPress); 
  
  Serial.print("Pressure into Inverter: ");
  Serial.print(actPressInvIN);
  Serial.println(" psi");

// READING PRESSURE OUT OF INVERTER
  pressInvOUT = analogRead(pressInvPinOUT); // read in inverter pressure-out voltage
  actPressInvOUT = map(pressInvOUT, lowPressVolt, highPressVolt, lowPress, highPress); 
  
  Serial.print("Pressure out of Inverter: ");
  Serial.print(actPressInvOUT);
  Serial.println(" psi");

// TEMPERATURE  --------------------------------------------------------------------------------------------------------------------------------------------
// at 0.5V, the temperature should be ? C°
// at 4.5V, the temperature should be ? C°

// READING TEMPERATURE INTO MOTOR
  tempMotIN = analogRead(tempMotPinIN); // read in motor temperature-in voltage
  actTempMotIN = map(tempMotIN, lowTempVolt, highTempVolt, lowTemp, highTemp); 
  
  Serial.print("Temperature into Motor: ");
  Serial.print(actTempMotIN);
  Serial.println(" C°");

// READING TEMPERATURE OUT OF MOTOR
  tempMotOUT = analogRead(tempMotPinOUT); // read in motor temperature-out voltage
  actTempMotOUT = map(tempMotOUT, lowTempVolt, highTempVolt, lowTemp, highTemp); 
  
  Serial.print("Temperature out of Motor: ");
  Serial.print(actTempMotOUT);
  Serial.println(" degrees C°");


// READING TEMPERATURE INTO INVERTER
  tempInvIN = analogRead(tempInvPinIN); // read in inverter temperature-in voltage
  actTempInvIN = map(tempInvIN, lowTempVolt, highTempVolt, lowTemp, highTemp); 
  
  Serial.print("Temperature into Motor: ");
  Serial.print(actTempInvIN);
  Serial.println(" C°");

// READING TEMPERATURE OUT OF INVERTER
  tempInvOUT = analogRead(tempInvPinOUT); // read in inverter temperature-out voltage
  actTempInvOUT = map(tempInvOUT, lowTempVolt, highTempVolt, lowTemp, highTemp); 
  
  Serial.print("Temperature out of Inverter: ");
  Serial.print(actTempInvOUT);
  Serial.println(" C°");
  
// FLOW RATE  --------------------------------------------------------------------------------------------------------------------------------------------
// at 0.5V, the flow rate should be ? m^3/s
// at 4.5V, the flow rate should be ? m^3/s

// READING FLOW RATE INTO MOTOR
  flowMotIN = analogRead(flowMotPinIN); // read in motor flow rate-in voltage
  actFlowMotIN = map_Generic(flowMotIN, lowFlowVolt, highFlowVolt, lowFlow, highFlow);
  avgFlowMotIN += actFlowMotIN;
  
  Serial.print("Flow Rate into Motor: ");
  Serial.print(avgFlowMotIN);
  Serial.println(" LPM");


// READING FLOW RATE INTO INVERTE
/*
  flowInvIN = analogRead(flowInvPinIN); // read in inverter flow rate-in voltage
  actFlowInvIN = map_Generic(flowInvIN, lowFlowVolt, highFlowVolt, lowFlow, highFlow); 
  avgFlowInvIN = (avgFlowInvIN + actFlowInvIN) / readingCnt; 
  
  Serial.print("Flow Rate into Inverter: ");
  Serial.print(actFlowInvIN);
  Serial.println(" LPM");
*/
  delay(200);

  Serial.println("=================================================================================== ");
  Serial.println();

  readingCnt += 1;
}
