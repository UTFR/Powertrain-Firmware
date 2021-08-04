#include <iostream>
#include <cassert>
#include <csignal>
#include "Matrix.hpp"
#include "fuse_detection.hpp"
#include "test_driver.hpp"

#define EPSILON 0.0001f

/* Testing stuff
6 cells in parallel, each with 50mOhm resistance
Current through all cells --> 40-200
V = IR_total
R_initial = (6/50)-1
R_final = (5/50)-1

git status --> what you've changed
git add --all
git commit -m "whatever"
git push
git checkout -b user/jsprenger/TEST
*/

float sample_frame[0][10];

float sample[10] = {101,100,100,99,98,91,105,109,103,100};

int main() {
  std::cout << "Testing fuse detection with empty memory frame." << std::endl;
  Matrix memory_frame(10,10);

  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      memory_frame.cell(i,j) = sample_frame[i][j];
    }
  }

  assert(!fuseDetectionAlgorithm(memory_frame, sample, 10, Z_SCORE_THRESHOLD));
  std::cout << "Passed." << std::endl;


  float decrease_by = 10;
  std::cout << "Now decreasing one cell by " << decrease_by << " until detection trips." << std::endl;
  for (size_t i = 0; i < 100/decrease_by; i++) {
    // std::raise(SIGINT);
    sample[0]-=decrease_by;
    print(sample, 10);
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

void print(float **matrix, int row, int col){
  int i, j;
    for (i = 0; i < row; ++i){
        for (j = 0; j < col; ++j)
            std::cout << matrix[i][j] << " ";
          std::cout << std::endl;
    }
}

void print(float *arr, int len){
  std::cout << "[";
  int i;
  for (i = 0; i < len - 1; ++i){
    std::cout << arr[i] << ", ";
  }
  std::cout << arr[i] << "]" << std::endl;
}
