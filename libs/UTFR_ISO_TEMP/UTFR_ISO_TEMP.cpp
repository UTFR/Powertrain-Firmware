#include "UTFR_ISO_TEMP.h"

void UTFR_ISO_TEMP::begin(){

  pinMode(SEL_A_PIN, OUTPUT);
  pinMode(SEL_B_PIN, OUTPUT);
  pinMode(SEL_C_PIN, OUTPUT);

  pinMode(SEL_0_PIN, OUTPUT);
  pinMode(SEL_1_PIN, OUTPUT);
  pinMode(SEL_2_PIN, OUTPUT);
  pinMode(SEL_3_PIN, OUTPUT);

  pinMode(T_IN_PIN, INPUT);

  digitalWrite(SEL_A_PIN, LOW);
  digitalWrite(SEL_B_PIN, LOW);
  digitalWrite(SEL_C_PIN, LOW);

  digitalWrite(SEL_0_PIN, LOW);
  digitalWrite(SEL_1_PIN, LOW);
  digitalWrite(SEL_2_PIN, LOW);
  digitalWrite(SEL_3_PIN, LOW);

  resetSegments();

}

float UTFR_ISO_TEMP::lookup(float voltage){
  float temperature = 120;
  int index = 12; // initial guess, 20 deg C
  bool higherTemp = (voltage <= lut_[index][0]); // Voltage decreases with temperature
  // Split into two halves, for more efficient searching.
  if (higherTemp)
  {
    for (index; index < LUT_LENGTH; index++)
    {
      if (voltage > (lut_[index][0]))
      {
        // sensor voltage is between this voltage and the last measured. Linear interpolation time
        float localSlope = 5.0 / (lut_[index][0] - lut_[index-1][0]);
        // point-slope form: y2 - y1 = m(x2-x1), so y2 = m(x2-x1) - y1
        temperature = localSlope * (voltage - lut_[index][0]) + lut_[index][1];
        return temperature;
      }
    }
    //return ((5.0 / (lut_[LUT_LENGTH-1][0]-lut_[LUT_LENGTH-2][0])) * (voltage-lut_[LUT_LENGTH-1][0])) + lut_[LUT_LENGTH-1][1]; // edge case
    return 125; //max value 
  }
  else
  {
    for (index; index > 0; index--)
    {
      if (voltage < (lut_[index][0]))
      {
        float localSlope = 5.0 / (lut_[index][0] - lut_[index-1][0]);
        temperature = localSlope * (voltage - lut_[index][0]) + lut_[index][1];
        return temperature;
      }
    }
    //return ((5.0 / (lut_[1][0]-lut_[0][0])) * (voltage-lut_[0][0])) + lut_[0][1];
    return -45; //min value
  }
  return temperature;
}


uint8_t UTFR_ISO_TEMP::determineSegmentNumber(uint8_t muxSelect){

  uint8_t segmentNumber = 0;
  if ((muxSelect < 3))
  {
    segmentNumber = 0;
  }
  else if ((muxSelect >= 3) && (muxSelect < 6))
  {
    segmentNumber = 1;
  }
  else if ((muxSelect >= 6) && (muxSelect < 9))
  {
    segmentNumber = 2;
  }
  else if ((muxSelect >= 9) && (muxSelect < 12))
  {
    segmentNumber = 3;
  }
  else if ((muxSelect >= 12) && (muxSelect < 15))
  {
    segmentNumber = 4;
  }

  return segmentNumber;
}


void UTFR_ISO_TEMP::readMux(uint8_t sensor_id) // these are the outputted select signals
{
  uint8_t muxSelect = 0;
  int rawVal = 0;
  float measuredVoltage;
  float sensorVoltage;
  int segmentNumber = 0;
  int boardNumber = 0;
  int total_counter = 0;
  float current_temp = 0;

  float total_temp = 0;
  
  for (muxSelect; muxSelect < 15; muxSelect++) // select #15 is the calibration resistor
  {
    segmentNumber = determineSegmentNumber(muxSelect);
    boardNumber = (muxSelect % 3);

    digitalWrite(SEL_0_PIN, muxSelect & 0b00001);
    digitalWrite(SEL_1_PIN, (muxSelect & 0b00010) >> 1);
    digitalWrite(SEL_2_PIN, (muxSelect & 0b00100) >> 2);
    digitalWrite(SEL_3_PIN, (muxSelect & 0b01000) >> 3);
    rawVal = analogRead(T_IN_PIN);
    measuredVoltage = (5.0) * (rawVal / 1023.0);
    sensorVoltage = (measuredVoltage - TF_OFFSET)/TF_SLOPE;

    if ((boardNumber == 1) && (sensor_id == 6))
    {
      // do nothing, sensor 6 not populated on board 1 (middle board)
    }
    else
    {
      current_temp = lookup(sensorVoltage);
      accum_.segs[segmentNumber].boards[boardNumber].sensors[sensor_id] = current_temp;
      total_temp += current_temp;
      ++total_counter;
      if (current_temp > temps_.highest_temp){
        temps_.highest_temp = current_temp;
      }
      if (current_temp < temps_.lowest_temp){
        temps_.lowest_temp = current_temp;
      }
    }
  }

  temps_.avg_temp = total_temp/total_counter;
}

void UTFR_ISO_TEMP::sensorLoop() {
  // Select a sensor from 0-6
  uint8_t a, b, c;
  resetSegments();
  for (uint8_t sel_out = 0; sel_out < 7; ++sel_out)
  {
    //Serial.print("CHECKING SEL OUT:");
    Serial.println(sel_out);
    a = (sel_out & 0b00000001);
    b = (sel_out & 0b00000010) >> 1;
    c = (sel_out & 0b00000100) >> 2;
    digitalWrite(SEL_A_PIN, a);
    digitalWrite(SEL_B_PIN, b);
    digitalWrite(SEL_C_PIN, c);

    readMux(sel_out); // this function reads all the on-board multiplexed voltages, and populates arrays with the values.
  }
  this->printSegments();
}

void UTFR_ISO_TEMP::resetSegments() {
  for (int segment = 0; segment < 5; segment++)
  {
    for (int brd = 0; brd < 3; brd++)
    {
      for (int sensor = 0; sensor < 7; sensor++)
      {
        accum_.segs[segment].boards[brd].sensors[sensor] = 0;
      }
      temps_.highest_temp = 0;
      temps_.lowest_temp = 999999;
      temps_.avg_temp = 0;
    }
  }
}

void UTFR_ISO_TEMP::printSegments()
{
  for (int segment = 0; segment < 5; segment++)
  {
    Serial.print("Segment: ");
    Serial.print(segment);
    for (int brd = 0; brd < 3; brd++)
    {
      Serial.print(" board : ");
      Serial.println(brd);
      for (int sensor = 0; sensor < 7; sensor++)
      {
        Serial.print("Sensor:");
        Serial.print(sensor);
        Serial.print(" : ");
        Serial.println(accum_.segs[segment].boards[brd].sensors[sensor]);
      }
    }
  }

  Serial.print("Highest Temp :");
  Serial.println(temps_.highest_temp);

  Serial.print("Low Temp :");
  Serial.println(temps_.lowest_temp);

  Serial.print("Avg Temp :");
  Serial.println(temps_.avg_temp);
}


temp_S UTFR_ISO_TEMP::getTemps(){
  return temps_;
}
