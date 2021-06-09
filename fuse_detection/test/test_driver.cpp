#include <iostream>
#include <cassert>
#include "Matrix.hpp"
#include "fuse_detection.hpp"

#define EPSILON 0.0001f

float sample_frame[10][10] = {
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100},
  {100,100,100,100,100,100,100,100,100,100}
};

float sample[10] = {100,100,100,100,100,100,100,100,100,100};

int main() {
  std::cout << "Testing fuse detection" << std::endl;
  Matrix memory_frame(10,10);

  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      memory_frame.cell(i,j) = sample_frame[i][j];
    }
  }

  std::cout << "Passing array of all 100's and sample of all 100's. Should return false." << std::endl;
  assert(!fuseDetectionAlgorithm(memory_frame, sample, 10, Z_SCORE_THRESHOLD));
  std::cout << "Passed." << std::endl;


  float decrease_by = 100;
  std::cout << "Now decreasing one cell by " << decrease_by << " until detection trips." << std::endl;
  for (size_t i = 0; i < 100/decrease_by; i++) {
    sample[0]-=decrease_by;
    bool ret = fuseDetectionAlgorithm(memory_frame, sample, 10, Z_SCORE_THRESHOLD);
    std::cout << "Cell is at " << sample[0] << ". ";
    if (ret) {
      std::cout << "Tripped." << std::endl;
      break;
    }else{
      std::cout << "Still did not trip." << std::endl;
    }
  }
  if(sample[0] < EPSILON)
  std::cout << decrease_by << " was too slow." << std::endl;


  return 0;
}

void print(float **matrix){
    int i, j;
    for (i = 0; i < 10; ++i){
        for (j = 0; j < 10; ++j)
            std::cout << matrix[i][j] << " ";
          std::cout << std::endl;
    }
}
