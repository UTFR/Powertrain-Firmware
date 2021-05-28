//Sample code for the Matrix library

#include "Matrix.hpp"

void setup() {
  
}

void loop() {
  
}


int comp(const void* a, const void* b) {
  return ((int)(*(float*)a - *(float*)b));
}

void medDev(float* sample, int size, float* median, float* med_abs_dev) {
  //  Takes input column vector (sample), calculates median and median absolute deviation. 
  
  // copied the array so that the original array doesn't get sorted 
  float* arr = malloc(sizeof(float) * size); 
  for (int i = 0; i < size; i++) arr[i] = sample[i];

  // sort values, find median 
  qsort(arr, size, sizeof(float), comp);
  if (size % 2 == 0) *median = (arr[(size / 2) - 1] + arr[size/ 2]) / 2;
  else *median = arr[size / 2];

  // calculate med_abs_dev: subtract median from all values and find absolute value 
  for (int j = 0; j < size; j++) {
    arr[j] = arr[j] - (*median);
    if (arr[j] < 0) arr[j] = -1 * arr[j]; 
  }
  
  //sort values, find median 
  qsort(arr, size, sizeof(float), comp);
  if (size % 2 == 0) *med_abs_dev = (arr[(size / 2) - 1] + arr[size / 2]) / 2;
  else *med_abs_dev = arr[size / 2];

}

// Every element of z-scores array = (Element of sample array - median)/Med_abs_dev
float * calcZscores(float* sample, int size, float median, float med_abs_dev) {
  float* zscores = malloc(sizeof(float) * size); // since the number of zscores = size of sample
  for (int i = 0; i < size; i++) zscores[i] = (sample[i] - median) / med_abs_dev; 
  return zscores; 
}
