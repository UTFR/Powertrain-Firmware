//Sample code for the Matrix library

#include "Matrix.hpp"

void setup() {
  
}

void loop() {
  
}


int comp(const void* a, const void* b) {
  return ((int)(*(float*)a - *(float*)b));
}

/*
 * Returns fuse flag
 */
bool fuseDetectionAlgorithm(Matrix & memory_frame, float* sample, int size, int threshold){
  float median, med_abs_dev;
  medDev(sample, size, &median, &med_abs_dev);
  float *zscores = calcZscores(sample, size, median, med_abs_dev);
  updateMemory(memory_frame, zscores);
  return detect_fuse(memory_frame, threshold);
}

/*
 * Takes a memory frame input, with n most recent
 * Z-Score measurements for each parallel cell brick. Each parallel cell
 * brick is checked to see if a fuse has blown.
 * 
 * Parameters:
 * memoryframe: Array of z-scores. Each column corresponds to a sample from a given time step. Each row corresponds to a parallel cell brick.
 * threshold: A constant integer. If the difference in z-score from the beginning of the memory frame to the end exceeds this value, then a fuse has blown.
 * 
 * Returns: true if we think a fuse has blown
 */
bool detect_fuse(Matrix memory_frame, int threshold){
  for(int i = 0; i < memory_frame.rows(); i++){
    //Measures difference between first and last values in each row, and compares to threshold
    if(abs(memory_frame.cell(i,0) - memory_frame.cell(i,memory_frame.rows() - 1)) > threshold){
      return true;
    }
  }
  return false;
}

/*
 * Copies in the new sample and shifts the memory by one row
 */
void updateMemory(Matrix & memory_frame, float * sample){
  //Shift all the collumns by one
  for(int i = memory_frame.rows() - 1; i > 0; i--){
    for(int j = 0; j < memory_frame.cols(); j++){
      memory_frame.cell(j, i) = memory_frame.cell(j, i-1);
    }
  }

  //Copy in the sample
  for(int i = 0; i < memory_frame.cols(); i++){
    memory_frame.cell(i, 0) = sample[i];
  }
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
