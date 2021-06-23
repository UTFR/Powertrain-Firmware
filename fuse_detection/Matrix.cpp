#include "Matrix.hpp"
#include <stdlib.h>
#include <string.h>
#include "MemoryFree.h"
// FIXME: Not commenting in this file since I believe Ege said that no dynamic memory will be used. Will
// comment later
Matrix::Matrix(const int &num_rows, const int &num_cols){
  failed = false;
  #ifdef ARDUINO
  if(freeMemory() < (num_rows * sizeof(float*) + num_rows * num_cols * sizeof(float))){
    failed = true;
    return;
  }
  #endif
  this->num_rows = num_rows;
  this->num_cols = num_cols;

  data = malloc(num_rows * sizeof(float*));
  for(int i = 0; i < num_rows; i++){
    data[i] = malloc(num_cols * sizeof(float));
    for(int j = 0; j < num_cols; j++){
      data[i][j] = 0;
    }
  }
}

Matrix::Matrix(const int &num_rows, const int &num_cols, float **data){
  failed = false;
  #ifdef ARDUINO
  if(freeMemory() < (num_rows * sizeof(float*) + num_rows * num_cols * sizeof(float))){
    failed = true;
    return;
  }
  #endif
  this->num_rows = num_rows;
  this->num_cols = num_cols;

  this->data = data;
}

Matrix::Matrix(const Matrix& m){
  failed = false;
  #ifdef ARDUINO
  if(freeMemory() < (num_rows * sizeof(float*) + num_rows * num_cols * sizeof(float))){
    failed = true;
    return;
  }
  #endif
  num_rows = m.num_rows;
  num_cols = m.num_cols;

  data = malloc(m.num_rows * sizeof(float*));
  for(int i = 0; i < m.num_rows; i++){
    data[i] = malloc(m.num_cols * sizeof(float));
    for(int j = 0; j < m.num_cols; j++){
      data[i][j] = m.data[i][j];
    }
  }
}

Matrix::~Matrix(){
  for(int i = 0; i < num_rows; i++){
    free(data[i]);
  }
  free(data);
}

Matrix & Matrix::operator+(const Matrix &rhs){
  for(int i = 0; i < rhs.num_rows; i++){
    for(int j = 0; j < rhs.num_cols; j++){
      data[i][j] += rhs.data[i][j];
    }
  }
  return *this;
}

Matrix & Matrix::operator-(const Matrix &rhs){
  for(int i = 0; i < rhs.num_rows; i++){
    for(int j = 0; j < rhs.num_cols; j++){
      data[i][j] -= rhs.data[i][j];
    }
  }
  return *this;
}

Matrix & Matrix::operator*(const float &i){
  for(int row = 0; row < num_rows; row++){
    for(int col = 0; col < num_cols; col++){
      data[row][col] *= i;
    }
  }
  return *this;
}

Matrix & Matrix::operator*(const Matrix &rhs){
  for(int row = 0; row < num_rows; row++){
    float this_row[num_cols];
    for(int col = 0; col < num_cols; col++){
      for(int i = 0; i < rhs.num_rows; i++){
        this_row[i] = data[row][col] * rhs.data[row][i];
      }
    }
    memcpy(data[row], this_row, num_cols);
  }
  return *this;
}

Matrix & Matrix::operator=(const Matrix &rhs){
  for(int row = 0; row < num_rows; row++){
    for(int col = 0; col < num_cols; col++){
      data[row][col] *= rhs.data[row][col];
    }
  }
  return *this;
}

bool Matrix::operator==(const Matrix &rhs){
  for(int row = 0; row < num_rows; row++){
    for(int col = 0; col < num_cols; col++){
      if(data[row][col] != rhs.data[row][col]) return false;
    }
  }
  return true;
}


bool Matrix::operator!=(const Matrix &rhs){
  for(int row = 0; row < num_rows; row++){
    for(int col = 0; col < num_cols; col++){
      if(data[row][col] != rhs.data[row][col]) return true;
    }
  }
  return false;
}

float & Matrix::cell(int row, int col){
  return data[row][col];
}

int Matrix::rows(){
  return num_rows;
}

int Matrix::cols(){
  return num_cols;
}

bool Matrix:: has_failed(){
  return failed;
}
