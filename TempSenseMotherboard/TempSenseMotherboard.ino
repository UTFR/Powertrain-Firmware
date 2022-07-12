// Temperature Sensor Motherboard
// To do: implement this using proper hardware abstraction, including pin driver...

// Selects for isolated muxes
#define SEL_A 1
#define SEL_B 0
#define SEL_C 3

// Selects for on-board mux
#define SEL_0 3
#define SEL_1 4
#define SEL_2 5
#define SEL_3 6

// Analog read pin
#define T_in A0

// ISOtemp board transfer function parameters
#define TF_offset 1.25
#define TF_slope 0.408

typedef struct isoTemp_S
{
  float sensors[7];
} isoTemp_S;

typedef struct segment_S
{
  isoTemp_S boards[3];
} segment_S;

typedef struct accum_S
{
  segment_S segs[5];
} accum_S;

accum_S accum;

// TEMPERATURES LOOKUP TABLE

#define LUT_LENGTH 33
float lut[33][2] =
{
  {2.44, -40.0},
  {2.42, -35.0},
  {2.40, -30.0},
  {2.38, -25.0},
  {2.35, -20.0},
  {2.32, -15.0},
  {2.27, -10.0},
  {2.23, -5.0},
  {2.17, 0.0},
  {2.11, 5.0},
  {2.05, 10.0},
  {1.99, 15.0},
  {1.92, 20.0}, // index 12
  {1.86, 25.0},
  {1.80, 30.0},
  {1.74, 35.0},
  {1.68, 40.0},
  {1.63, 45.0},
  {1.59, 50.0},
  {1.55, 55.0},
  {1.51, 60.0},
  {1.48, 65.0},
  {1.45, 70.0},
  {1.43, 75.0},
  {1.40, 80.0},
  {1.38, 85.0},
  {1.37, 90.0},
  {1.35, 95.0},
  {1.34, 100.0},
  {1.33, 105.0},
  {1.32, 110.0},
  {1.31, 115.0},
  {1.30, 120.0}
};

// Source: trust me bro
float lookup(float voltage)
{
  float temperature = 120;
  int index = 12; // initial guess, 20 deg C
  bool higherTemp = (voltage <= lut[index][0]); // Voltage decreases with temperature
  // Split into two halves, for more efficient searching.
  if (higherTemp)
  {
    for (index; index < LUT_LENGTH; index++)
    {
      if (voltage > (lut[index][0]))
      {
        // sensor voltage is between this voltage and the last measured. Linear interpolation time
        float localSlope = 5.0 / (lut[index][0] - lut[index-1][0]);
        // point-slope form: y2 - y1 = m(x2-x1), so y2 = m(x2-x1) - y1
        temperature = localSlope * (voltage - lut[index][0]) + lut[index][1];
        return temperature;
      }
    }
    return ((5.0 / (lut[LUT_LENGTH-1][0]-lut[LUT_LENGTH-2][0])) * (voltage-lut[LUT_LENGTH-1][0])) + lut[LUT_LENGTH-1][1]; // edge case
  }
  else
  {
    for (index; index > 0; index--)
    {
      if (voltage < (lut[index][0]))
      {
        float localSlope = 5.0 / (lut[index][0] - lut[index-1][0]);
        temperature = localSlope * (voltage - lut[index][0]) + lut[index][1];
        return temperature;
      }
    }
    return ((5.0 / (lut[1][0]-lut[0][0])) * (voltage-lut[0][0])) + lut[0][1]; // edge case
  }

  return temperature;
}

// Helper function to quickly determine which segment the current sensor lives in
int determineSegmentNumber(uint8_t muxSelect)
{
  int segmentNumber = 0;
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

void readMux(uint8_t sensor_id) // these are the outputted select signals
{
  uint8_t muxSelect = 0;
  int rawVal = 0;
  float measuredVoltage;
  float sensorVoltage;
  int segmentNumber = 0;
  int boardNumber = 0;
  for (muxSelect; muxSelect < 15; muxSelect++) // select #15 is the calibration resistor
  {
    segmentNumber = determineSegmentNumber(muxSelect);
    boardNumber = (muxSelect % 3);
    
    digitalWrite(SEL_0, muxSelect & 0b00001);
    digitalWrite(SEL_1, (muxSelect & 0b00010) >> 1);
    digitalWrite(SEL_2, (muxSelect & 0b00100) >> 2);
    digitalWrite(SEL_3, (muxSelect & 0b01000) >> 3);
    rawVal = analogRead(T_in);
    measuredVoltage = (5.0) * (rawVal / 1023.0);
    sensorVoltage = (measuredVoltage - TF_offset)/TF_slope;

    if ((boardNumber == 1) && (sensor_id == 6))
    {
      // do nothing, sensor 6 not populated on board 1 (middle board)
    }
    else
    {
      accum.segs[segmentNumber].boards[boardNumber].sensors[sensor_id] = lookup(sensorVoltage);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);
  pinMode(SEL_C, OUTPUT);

  pinMode(SEL_0, OUTPUT);
  pinMode(SEL_1, OUTPUT);
  pinMode(SEL_2, OUTPUT);
  pinMode(SEL_3, OUTPUT);

  pinMode(T_in, INPUT);

  digitalWrite(SEL_A, LOW);
  digitalWrite(SEL_B, LOW);
  digitalWrite(SEL_C, LOW);

  digitalWrite(SEL_0, LOW);
  digitalWrite(SEL_1, LOW);
  digitalWrite(SEL_2, LOW);
  digitalWrite(SEL_3, LOW);

  // ensure all initial temperatures are set to zero
  for (int segment = 0; segment < 5; segment++)
  {
    for (int brd = 0; brd < 3; brd++)
    {
      for (int sensor = 0; sensor < 7; sensor++)
      {
        accum.segs[segment].boards[brd].sensors[sensor] = 0;
      }
    }
  }
}

uint8_t sel_out = 0;
int a;
int b;
int c;

void loop() {
  // loop through, reading all sensors

  sel_out = 0;
  // Select a sensor from 0-6
  for (sel_out; sel_out < 7; ++sel_out)
  {
    a = (sel_out & 0b0001);
    b = (sel_out & 0b0010) >> 1;
    c = (sel_out & 0b0100) >> 2;
    digitalWrite(SEL_A, a);
    digitalWrite(SEL_B, b);
    digitalWrite(SEL_C, c);

    readMux(sel_out); // this function reads all the on-board multiplexed voltages, and populates arrays with the values.

  }
  
  
}
