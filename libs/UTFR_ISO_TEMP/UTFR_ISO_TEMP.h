#ifndef _UTFR_ISO_TEMP_H_
#define _UTFR_ISO_TEMP_H_

#include "Arduino.h"

// Selects for isolated muxes
#define SEL_A_PIN 1
#define SEL_B_PIN 0
#define SEL_C_PIN 3

// Selects for on-board mux
#define SEL_0_PIN 3
#define SEL_1_PIN 4
#define SEL_2_PIN 5
#define SEL_3_PIN 6

// Analog read pin
#define T_IN_PIN A0

// ISOtemp board transfer function parameters
#define TF_OFFSET 1.25
#define TF_SLOPE 0.408

// Temp lookup board
#define LUT_LENGTH 33


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

typedef struct temp_S
{
  float highest_temp;
  float lowest_temp;
  float avg_temp;
} temp_S;

class UTFR_ISO_TEMP {
 private:

  accum_S accum_;
  temp_S temps_;


  float lut_[33][2] = {
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

 public:

  void begin();

  float lookup(float voltage);

  uint8_t determineSegmentNumber(uint8_t muxSelect);

  void readMux(uint8_t sensor_id);

  void sensorLoop();

  void resetSegments();

  void printSegments();

  temp_S getTemps();
};

#endif

